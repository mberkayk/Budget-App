#include "monthlyview.h"

MonthlyEntryDialog::MonthlyEntryDialog() : QDialog() {
	QHBoxLayout layout;
	setLayout(&layout);
	layout.addWidget(new QLabel("Amount:"));
}

MonthlyEntryDialog::~MonthlyEntryDialog(){

}

MonthlyView::MonthlyView(Database *database) : QWidget() {

	db = database;

	budget = 0;

	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	titleBarLayout = new QHBoxLayout();
	mainLayout->addLayout(titleBarLayout);

	titleLabel = new QLabel("This Month");
	titleBarLayout->addWidget(titleLabel);

	addBtn = new QPushButton("+");
	titleBarLayout->addWidget(addBtn);

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
	delete date;
}

void MonthlyView::loadFromDatabase(){
	entries->setEntries(db->getMonthEntries(*date));
	budget = db->getMonthlyBudget(*date);
}

void MonthlyView::saveToDatabase(){
	db->setMonthEntries(*date, entries->getEntries());
	db->setMonthlyBudget(*date, budget);
}

void MonthlyView::addNewEntry(){

}
