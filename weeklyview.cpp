#include "weeklyview.h"

WeeklyView::WeeklyView(){

	budget = 0;

	widget = new QWidget();

	mainLayout = new QVBoxLayout();
	widget->setLayout(mainLayout);

	titleBarLayout = new QHBoxLayout();
	mainLayout->addLayout(titleBarLayout);

	budgetInfoLayout = new QHBoxLayout();
	mainLayout->addLayout(budgetInfoLayout);

	weekSectionsLayout = new QVBoxLayout();
	mainLayout->addLayout(weekSectionsLayout);


	menuBtn = new QPushButton("M");
	titleBarLayout->addWidget(menuBtn);

	titleLabel = new QLabel("this week");
	titleBarLayout->addWidget(titleLabel);

	addBtn = new QPushButton("+");

	budgetLabel = new QLabel("budget: " + QString::number(budget));
	budgetInfoLayout->addWidget(budgetLabel);

	remainingInfoLabel = new QLabel("remaining: ");
	budgetInfoLayout->addWidget(remainingInfoLabel);

	for(int i = 0; i < 8; i++){
		weekSectionsLayout->addWidget(groups[i]);
	}

	date = new QDate();
	*date = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

}


WeeklyView::~WeeklyView(){
	delete widget;
	delete mainLayout;
	delete titleBarLayout;
	delete budgetInfoLayout;
	delete weekSectionsLayout;

	delete menuBtn;
	delete titleLabel;
	delete addBtn;

	delete budgetLabel;
	delete remainingInfoLabel;
	for(int i = 0; i < 8; i++){
		delete groups[i];
	}
	delete date;
}

void WeeklyView::loadFromDatabase(Database *db){
	for(int i = 0; i < 7; i++){
		QDate d = date->addDays(i);
		groups[i]->setEntries(db->getDayEntries(d));
	}
	groups[7]->setEntries(db->getWeekEntries(*date));
	budget = db->getWeeklyBudget(*date);
}

void WeeklyView::saveToDatabase(Database *db){
	for(int i = 0; i < 7; i++){
		QDate d = date->addDays(i);
		db->setDayEntries(d, groups[i]->getEntries());
	}
	db->setWeekEntries(*date, groups[7]->getEntries());
	budget = db->getWeeklyBudget(*date);
}

QWidget * WeeklyView::getWidget(){return widget;}


QPushButton * WeeklyView::getMenuBtn(){return menuBtn;}
