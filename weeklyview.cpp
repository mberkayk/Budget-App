#include "weeklyview.h"
#include <QDebug>
#include <cmath>

SpinBoxView::SpinBoxView(QVector<QString> items) : QGraphicsView() {
	selectedItem = 0;
	fontSize = 20;
	spacing = 30;

	sceneUnstable = false;

	scene = new QGraphicsScene;
	for(int i = 0; i < items.size(); i++){
		QGraphicsTextItem *item = new QGraphicsTextItem(items[i]);
		QFont f = QFont();
		f.setPixelSize(20);
		item->setFont(f);
		scene->addItem(item);
		item->setPos(100 - item->boundingRect().width()/2 , spacing * i);
	}

	setScene(scene);
	setInteractive(false);
	setDragMode(QGraphicsView::ScrollHandDrag);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMaximumSize(200, 30);
	updateViewPort();

}

void SpinBoxView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);
	if(event->buttons().testFlag(Qt::LeftButton)){
		sceneUnstable = true;
	}
}

void SpinBoxView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);

	if(sceneUnstable == true){
		double d = mapToScene(0, 0).y() / spacing;
		selectedItem = std::round(d);
		verticalScrollBar()->setValue(selectedItem*spacing);
		sceneUnstable = false;
	}

}

void SpinBoxView::updateViewPort(){
	centerOn(scene->items(Qt::AscendingOrder).at(selectedItem));
}

void SpinBoxView::incrementSelectedDay(){
	if (selectedItem != 6){
		selectedItem++;
	}
	updateViewPort();
}

void SpinBoxView::decrementSelectedDay(){
	if (selectedItem != 0){
		selectedItem--;
	}
	updateViewPort();
}

void SpinBoxView::select(int i){
	selectedItem = i;
	updateViewPort();
}

int SpinBoxView::getSelectedItemIndex(){
	return selectedItem;
}




SpinBox::SpinBox(QVector<QString> items){

	layout = new QVBoxLayout;
	setLayout(layout);

	upBtn = new QToolButton;
	upBtn->setArrowType(Qt::UpArrow);
	upBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	layout->addWidget(upBtn);
	layout->setAlignment(upBtn, Qt::AlignHCenter);

	spinBoxView = new SpinBoxView(items);
	layout->addWidget(spinBoxView);

	downBtn = new QToolButton;
	downBtn->setArrowType(Qt::DownArrow);
	downBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	layout->addWidget(downBtn);
	layout->setAlignment(downBtn, Qt::AlignHCenter);

	layout->setStretchFactor(upBtn, 1);
	layout->setStretchFactor(downBtn, 1);
	layout->setStretchFactor(spinBoxView, 2);

	QObject::connect(upBtn, SIGNAL(pressed()), spinBoxView, SLOT(decrementSelectedDay()));
	QObject::connect(downBtn, SIGNAL(pressed()), spinBoxView, SLOT(incrementSelectedDay()));

}

void SpinBox::select(int i){
	spinBoxView->select(i);
}

int SpinBox::getSelectedItemIndex(){
	return spinBoxView->getSelectedItemIndex();
}



WeeklyEntryDialog::WeeklyEntryDialog(QWidget *parent) : QDialog(parent) {
	layout = new QVBoxLayout;
	setLayout(layout);

	QVector<QString> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	spinBox = new SpinBox(days);
	spinBox->select(QDate::currentDate().dayOfWeek()-1);
	layout->addWidget(spinBox);

	descBox = new QLineEdit;
	descBox->setPlaceholderText("Expense");
	layout->addWidget(descBox);

	layout->addWidget(new QLabel("Amount:"));

	amtBox = new QLineEdit;
	amtBox->setPlaceholderText("0");
	layout->addWidget(amtBox);

	QHBoxLayout *btnLayout = new QHBoxLayout;
	layout->addLayout(btnLayout);

	xBtn = new QPushButton("X");
	xBtn->setAutoDefault(false);
	okBtn = new QPushButton("OK");
	okBtn->setAutoDefault(true);
	btnLayout->addWidget(xBtn);
	btnLayout->addWidget(okBtn);

	QObject::connect(xBtn, SIGNAL(pressed()), this, SLOT(reject()));
	QObject::connect(okBtn, SIGNAL(pressed()), this, SLOT(accept()));
}

int WeeklyEntryDialog::getSelectedDay(){
	return spinBox->getSelectedItemIndex();
}

Entry * WeeklyEntryDialog::createEntry(){
	return new Entry(amtBox->text().toInt(), descBox->text());
}


WeeklyView::WeeklyView(Database *database) : QWidget() {

	db = database;

	budget = 0;

	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	titleBarLayout = new QHBoxLayout();
	mainLayout->addLayout(titleBarLayout);

	budgetInfoLayout = new QHBoxLayout();
	mainLayout->addLayout(budgetInfoLayout);

	dailyEntryGroupsScrollArea = new QScrollArea;
	mainLayout->addWidget(dailyEntryGroupsScrollArea);
	dailyGroupsStackedWidget = new QStackedWidget;
	noEntriesLabel = new QLabel("No Entries for this week yet");
	dailyGroupsStackedWidget->addWidget(noEntriesLabel);

	dailyGroupsListWidget = new QWidget;
	dailyEntryGroupsLayout = new QVBoxLayout();
	dailyGroupsListWidget->setLayout(dailyEntryGroupsLayout);
	dailyGroupsStackedWidget->addWidget(dailyGroupsListWidget);

	weekEntryGroupLayout = new QVBoxLayout;
	mainLayout->addLayout(weekEntryGroupLayout);

	titleLabel = new QLabel("this week");
	titleBarLayout->addWidget(titleLabel);

	addBtn = new QPushButton("+");
	titleBarLayout->addWidget(addBtn);
	QObject::connect(addBtn, SIGNAL(pressed()), this, SLOT(showEntryDialog()));

	budgetLabel = new QLabel("budget: " + QString::number(budget));
	budgetInfoLayout->addWidget(budgetLabel);

	remainingInfoLabel = new QLabel("remaining: ");
	budgetInfoLayout->addWidget(remainingInfoLabel);

	for(int i = 0; i < 7; i++){
		dailyEntryGroupsLayout->addWidget(groups[i]);
	}
	//scrollArea widget has to be set after everything is added to the layouts
	dailyEntryGroupsScrollArea->setWidget(dailyGroupsStackedWidget);

	weekEntryGroupLayout->addWidget(groups[7]);

	//expand week group and today's group by default
	groups[7]->expand();
	groups[QDate::currentDate().dayOfWeek() - 1]->expand();


	date = new QDate();
	*date = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

	loadFromDatabase();

	//If an entry group doesn't have any entries don't display it
	bool allEmpty = true;
	for(int i = 0; i < 7; i++){
		if(groups[i]->getEntries().size() == 0){
			groups[i]->setVisible(false);
		}else {
			allEmpty = false;
		}
	}
	if(allEmpty){
		dailyGroupsStackedWidget->setCurrentWidget(noEntriesLabel);
	}
}

WeeklyView::~WeeklyView(){
	delete date;
}

void WeeklyView::loadFromDatabase(){
	for(int i = 0; i < 7; i++){
		QDate d = date->addDays(i);
		groups[i]->setEntries(db->getDayEntries(d));
	}
	groups[7]->setEntries(db->getWeekEntries(*date));
	budget = db->getWeeklyBudget(*date);
}

void WeeklyView::saveToDatabase(){
	for(int i = 0; i < 7; i++){
		QDate d = date->addDays(i);
		db->appendDayEntries(d, groups[i]->getUnsavedEntries());
	}
	db->appendWeekEntries(*date, groups[7]->getUnsavedEntries());
	budget = db->getWeeklyBudget(*date);

	db->saveDayDataToFile();
	db->saveWeekDataToFile();
}

void WeeklyView::showEntryDialog(){
	entryDialog = new WeeklyEntryDialog(this);
	QObject::connect(entryDialog, SIGNAL(accepted()), this, SLOT(addNewEntry()));
	entryDialog->exec();
	delete entryDialog;
}

void WeeklyView::addNewEntry(){
	EntryGroup *g = groups[entryDialog->getSelectedDay()];

	g->addEntry(entryDialog->createEntry());
	saveToDatabase();

	if(dailyGroupsStackedWidget->currentWidget() == noEntriesLabel){
		dailyGroupsStackedWidget->setCurrentWidget(dailyGroupsListWidget);
	}
	for(int i = 0; i < 7; i++){
		EntryGroup *e = groups[i];
		if(e->getEntries().size() > 0 && e->isVisible() == false){
			e->expand();
			e->setVisible(true);
		}
	}
}
