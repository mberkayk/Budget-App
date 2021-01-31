#include "weeklyview.h"
#include <QDebug>

SpinBox::SpinBox(){
	selectedDay = 0;

	layout = new QVBoxLayout;
	setLayout(layout);

	upBtn = new QToolButton;
	upBtn->setArrowType(Qt::UpArrow);
	layout->addWidget(upBtn);
	layout->setAlignment(upBtn, Qt::AlignHCenter);

	daysScene = new QGraphicsScene;
	daysView = new QGraphicsView(daysScene);
	layout->addWidget(daysView);

	downBtn = new QToolButton;
	downBtn->setArrowType(Qt::DownArrow);
	layout->addWidget(downBtn);
	layout->setAlignment(downBtn, Qt::AlignHCenter);

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
