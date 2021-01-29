#include "weeklyview.h"

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

void WeeklyView::addNewEntry(){

}
