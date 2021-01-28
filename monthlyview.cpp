#include "monthlyview.h"

MonthlyView::MonthlyView(Database *database) {

	db = database;

	budget = 0;

	widget = new QWidget();

	mainLayout = new QVBoxLayout();
	widget->setLayout(mainLayout);

	titleBarLayout = new QHBoxLayout();
	mainLayout->addLayout(titleBarLayout);

	menuBtn = new QPushButton("M");
	titleBarLayout->addWidget(menuBtn);

	titleLabel = new QLabel("This Month");
	titleBarLayout->addWidget(titleLabel);

	budgetInfoLayout = new QHBoxLayout();
	mainLayout->addLayout(budgetInfoLayout);

	budgetLabel = new QLabel("Budget: " + QString::number(budget));
	budgetInfoLayout->addWidget(budgetLabel);

	entries = new EntryGroup("");
	mainLayout->addWidget(entries);

	date = new QDate();
	*date = QDate::currentDate().addDays(-QDate::currentDate().day() + 1);

	loadFromDatabase();
}

MonthlyView::~MonthlyView(){
	delete widget;
	delete date;
	delete budgetLabel;
}

void MonthlyView::loadFromDatabase(){
	entries->setEntries(db->getMonthEntries(*date));
	budget = db->getMonthlyBudget(*date);
}

void MonthlyView::saveToDatabase(){
	db->setMonthEntries(*date, entries->getEntries());
	db->setMonthlyBudget(*date, budget);
}

QWidget * MonthlyView::getWidget(){return widget;}

QPushButton * MonthlyView::getMenuBtn(){return menuBtn;}
