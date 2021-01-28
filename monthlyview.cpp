#include "monthlyview.h"

MonthlyView::MonthlyView() {

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
}

MonthlyView::~MonthlyView(){
	delete widget;
	delete date;
	delete budgetLabel;
}

void MonthlyView::loadFromDatabase(Database *db){
	entries->setEntries(db->getMonthEntries(*date));
	budget = db->getMonthlyBudget(*date);
}

QWidget * MonthlyView::getWidget(){return widget;}

QPushButton * MonthlyView::getMenuBtn(){return menuBtn;}
