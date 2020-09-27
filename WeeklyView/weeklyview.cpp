#include "weeklyview.h"

#include <QLabel>
#include <QDebug>

WeeklyView::WeeklyView(){

	db = new Database();

	currentWeek = new QDate();
	activeWeek = new QDate();

	*currentWeek = QDate::currentDate().addDays(
				1-QDate::currentDate().dayOfWeek());
	*activeWeek = *currentWeek;

	db->selectWeek(*activeWeek);


	remaining = 0;
	dailyLimit = 0;

	layout = new QVBoxLayout(this);

	toolbar = new QToolBar(this);

	preWeekAct = new QAction("Pre");
	toolbar->addAction(preWeekAct);
	QObject::connect(preWeekAct, &QAction::triggered,
					 this, &WeeklyView::preWeek);
	dateLabel = new QLabel("This Week");
	toolbar->addWidget(dateLabel);
	nextWeekAct = new QAction("Next");
	toolbar->addAction(nextWeekAct);
	QObject::connect(nextWeekAct, &QAction::triggered,
					 this, &WeeklyView::nextWeek);
	layout->addWidget(toolbar);

	budgetLine = new QHBoxLayout();
	budgetLine->addWidget(new QLabel("Budget: "));
	budgetLineEdit = new QLineEdit();
	QDoubleValidator *v = new QDoubleValidator(budgetLineEdit);
	v->setBottom(0);
	v->setNotation(QDoubleValidator::StandardNotation);
	budgetLineEdit->setValidator(v);
	budgetLine->addWidget(budgetLineEdit);
	layout->addLayout(budgetLine);

	QObject::connect(budgetLineEdit, &QLineEdit::editingFinished,
					 this, &WeeklyView::budgetSet);

	setBudget(db->getBudget());

	remainingLabel = new QLabel("Remaining: " + QString::number(remaining));
	layout->addWidget(remainingLabel);

	dailyLimitLabel = new QLabel("Daily Limit: " + QString::number(dailyLimit));
	layout->addWidget(dailyLimitLabel);

	days = QVector<WeekEntry *>(7);
	initWeekEntries(*activeWeek, db);

	calculate();
}

void WeeklyView::initWeekEntries(QDate date, Database *db){
	if(db != nullptr){
		days[0] = new WeekEntry(this,"Monday", db->getDay(0));
		days[1] = new WeekEntry(this,"Tuesday", db->getDay(1));
		days[2] = new WeekEntry(this,"Wednesday", db->getDay(2));
		days[3] = new WeekEntry(this,"Thursday", db->getDay(3));
		days[4] = new WeekEntry(this,"Friday", db->getDay(4));
		days[5] = new WeekEntry(this,"Saturday", db->getDay(5));
		days[6] = new WeekEntry(this,"Sunday", db->getDay(6));
		weeklySpendings = new WeekEntry(this, "Weekly Spendings", db->getWeeklySpendings());
	}else{
		days[0] = (new WeekEntry(this,"Monday"));
		days[1] = (new WeekEntry(this,"Tuesday"));
		days[2] = (new WeekEntry(this,"Wednesday"));
		days[3] = (new WeekEntry(this,"Thursday"));
		days[4] = (new WeekEntry(this,"Friday"));
		days[5] = (new WeekEntry(this,"Saturday"));
		days[6] = (new WeekEntry(this,"Sunday"));
		weeklySpendings = new WeekEntry(this, "Weekly Spendings");
	}
	days[date.currentDate().dayOfWeek()-1]->setAddEmptyLineEdit(true);
	weeklySpendings->setAddEmptyLineEdit(true);

	foreach(WeekEntry *de, days){
		layout->addWidget(de);
		QObject::connect(de, &WeekEntry::sumChanged,
						 this, &WeeklyView::calculate);
	}

	layout->addWidget(weeklySpendings);
}

WeeklyView::~WeeklyView(){

	saveToDatabase();

	delete db;

	delete currentWeek;
	delete activeWeek;

	foreach (WeekEntry *d, days) {
		delete d;
	}
	delete toolbar;
	delete preWeekAct;
	delete nextWeekAct;
	delete layout;
	delete dateLabel;
	delete budgetLine;
	delete weeklySpendings;
	delete budgetLineEdit;
	delete remainingLabel;
	delete dailyLimitLabel;
}

void WeeklyView::calculate(){
	double daysTotal = 0;
	foreach (WeekEntry *d, days) {
		daysTotal += d->getSum();
	}
	daysTotal += weeklySpendings->getSum();
	setRemaining(budget - daysTotal);
	setDailyLimit();
}

void WeeklyView::budgetSet(){
	budget = budgetLineEdit->text().toDouble();
	calculate();
}

void WeeklyView::setBudget(double d){
	budget = d;
	budgetLineEdit->setText(QString::number(budget));
}

void WeeklyView::setRemaining(double d){
	remaining = d;
	remainingLabel->setText("Remaining: " + QString::number(remaining));
}

void WeeklyView::setDailyLimit(){
	int remainingDays = 7-QDate::currentDate().dayOfWeek()+1;
	dailyLimit = remaining / remainingDays;
	dailyLimitLabel->setText("Daily Limit: " + QString::number(dailyLimit));
}

void WeeklyView::saveToDatabase(){
	db->updateBudget(*activeWeek, budget);
	for(int i = 0; i < 7; i++){
		db->updateDay(activeWeek->addDays(i), days[i]->getEntries());
	}
	db->updateWeeklySpending(*activeWeek, weeklySpendings->getEntries());
}

void WeeklyView::switchWeek(QDate date){
	saveToDatabase();
	*activeWeek = date;
	loadFromDatabase(date, db);
	if(activeWeek->daysTo(*currentWeek) == 0){
		dateLabel->setText("This Week");
	}else if(activeWeek->daysTo(*currentWeek) == 7){
		dateLabel->setText("Last Week");
	}else if (activeWeek->daysTo(*currentWeek) == -7){
		dateLabel->setText("Next Week");
	}else {
		dateLabel->setText(activeWeek->toString());
	}
}

void WeeklyView::preWeek(){
	switchWeek(activeWeek->addDays(-7));
}

void WeeklyView::nextWeek(){
	switchWeek(activeWeek->addDays(7));
}

void WeeklyView::loadFromDatabase(QDate date, Database *db){
	db->selectWeek(date);
	setBudget(db->getBudget());
	for(int i = 0; i < 7; i++){
		days[i]->setEntries(db->getDay(i));
	}
	weeklySpendings->setEntries(db->getWeeklySpendings());
	calculate();
}
