#include "weeklyview.h"
#include <QDebug>
#include <cmath>

SpinBoxView::SpinBoxView(QString days[]) : QGraphicsView() {
	selectedDay = 0;
	fontSize = 20;
	spacing = 30;

	preMouseY = 0;
	sceneUnstable = false;
	mouseDragDir = 0;

	scene = new QGraphicsScene;
	for(int i = 0; i < 7; i++){
		QGraphicsTextItem *item = new QGraphicsTextItem(days[i]);
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
		mouseDragDir = event->y() - preMouseY;
	}
}

void SpinBoxView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);

	if(sceneUnstable == true){
		double d = mapToScene(0, 0).y() / spacing;
		selectedDay = std::round(d);
		verticalScrollBar()->setValue(selectedDay*spacing);
		sceneUnstable = false;
	}

}

void SpinBoxView::updateViewPort(){
	centerOn(scene->items(Qt::AscendingOrder).at(selectedDay));
}

void SpinBoxView::incrementSelectedDay(){
	if (selectedDay != 6){
		selectedDay++;
	}
	updateViewPort();
}

void SpinBoxView::decrementSelectedDay(){
	if (selectedDay != 0){
		selectedDay--;
	}
	updateViewPort();
}



SpinBox::SpinBox(){

	layout = new QVBoxLayout;
	setLayout(layout);

	upBtn = new QToolButton;
	upBtn->setArrowType(Qt::UpArrow);
	layout->addWidget(upBtn);
	layout->setAlignment(upBtn, Qt::AlignHCenter);

	daysView = new SpinBoxView(days);
	layout->addWidget(daysView);

	downBtn = new QToolButton;
	downBtn->setArrowType(Qt::DownArrow);
	layout->addWidget(downBtn);
	layout->setAlignment(downBtn, Qt::AlignHCenter);

	QObject::connect(upBtn, SIGNAL(pressed()), daysView, SLOT(decrementSelectedDay()));
	QObject::connect(downBtn, SIGNAL(pressed()), daysView, SLOT(incrementSelectedDay()));

}

WeeklyEntryDialog::WeeklyEntryDialog(QWidget *parent) : QDialog(parent) {
	layout = new QVBoxLayout;
	setLayout(layout);

	spinBox = new SpinBox;
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

WeeklyView::WeeklyView(Database *database) : QWidget() {

	db = database;

	budget = 0;

	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	titleBarLayout = new QHBoxLayout();
	mainLayout->addLayout(titleBarLayout);

	budgetInfoLayout = new QHBoxLayout();
	mainLayout->addLayout(budgetInfoLayout);

	weekSectionsLayout = new QVBoxLayout();
	mainLayout->addLayout(weekSectionsLayout);

	titleLabel = new QLabel("this week");
	titleBarLayout->addWidget(titleLabel);

	addBtn = new QPushButton("+");
	titleBarLayout->addWidget(addBtn);
	QObject::connect(addBtn, SIGNAL(pressed()), this, SLOT(showEntryDialog()));

	budgetLabel = new QLabel("budget: " + QString::number(budget));
	budgetInfoLayout->addWidget(budgetLabel);

	remainingInfoLabel = new QLabel("remaining: ");
	budgetInfoLayout->addWidget(remainingInfoLabel);

	for(int i = 0; i < 8; i++){
		weekSectionsLayout->addWidget(groups[i]);
	}

	date = new QDate();
	*date = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

	loadFromDatabase();

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
		db->appendDayEntries(d, groups[i]->getEntries());
	}
	db->appendWeekEntries(*date, groups[7]->getEntries());
	budget = db->getWeeklyBudget(*date);
}

void WeeklyView::showEntryDialog(){
	entryDialog = new WeeklyEntryDialog(this);
	QObject::connect(entryDialog, SIGNAL(accepted()), this, SLOT(addNewEntry()));
	entryDialog->exec();
	delete entryDialog;
}

void WeeklyView::addNewEntry(){

}
