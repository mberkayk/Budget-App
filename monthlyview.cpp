#include "monthlyview.h"

MonthlyEntryDialog::MonthlyEntryDialog(QWidget *parent) : QDialog(parent) {

	layout= new QVBoxLayout;
	setLayout(layout);

	layout->addWidget(new QLabel("Description:"));

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
	okBtn = new QPushButton("OK");
	btnLayout->addWidget(xBtn);
	btnLayout->addWidget(okBtn);

	QObject::connect(xBtn, SIGNAL(pressed()), this, SLOT(reject()));
	QObject::connect(okBtn, SIGNAL(pressed()), this, SLOT(accept()));

}

Entry * MonthlyEntryDialog::getEntry(){
	return new Entry(amtBox->text().toInt(), descBox->text());
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
	QObject::connect(addBtn, SIGNAL(pressed()), this, SLOT(showEntryDialog()));

	budgetInfoLayout = new QHBoxLayout();
	mainLayout->addLayout(budgetInfoLayout);

	budgetLabel = new QLabel("Budget: " + QString::number(budget));
	budgetInfoLayout->addWidget(budgetLabel);

	entries = new EntryGroup("");
	entries->expand();
	scrollArea = new QScrollArea;
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(entries);
	mainLayout->addWidget(scrollArea);

	date = new QDate();
	*date = QDate::currentDate().addDays(-QDate::currentDate().day() + 1);

	loadFromDatabase();

	newEntries = QVector<Entry*>();

}

MonthlyView::~MonthlyView(){
	delete date;
}

void MonthlyView::loadFromDatabase(){
	entries->setEntries(db->getMonthEntries(*date));
	setBudget(db->getMonthlyBudget(*date));
}

void MonthlyView::saveToDatabase(){
	db->appendMonthEntries(*date, newEntries);
	db->setMonthlyBudget(*date, budget);
	db->saveMonthDataToFile();
	newEntries.clear();
}

void MonthlyView::showEntryDialog(){
	entryDialog = new MonthlyEntryDialog(this);
	QObject::connect(entryDialog, SIGNAL(accepted()), this, SLOT(addNewEntry()));
	entryDialog->exec();
	delete entryDialog;
}

void MonthlyView::addNewEntry(){
	Entry * newEntry = entryDialog->getEntry();
	entries->addEntry(newEntry);
	newEntries.append(newEntry);
	saveToDatabase();
}

void MonthlyView::setBudget(int b){
	budget = b;
	budgetLabel->setText("Budget: " + QString::number(b));
}
