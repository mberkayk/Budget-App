#ifndef WEEKLYVIEW_H
#define WEEKLYVIEW_H

#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QDate>
#include "database.h"

#include "weekentry.h"

class WeeklyView : public QWidget {

public:
	WeeklyView();
	~WeeklyView();

public slots:
	void budgetSet();
	void calculate();

private:

	Database *db;

	double budget;
	double remaining;
	double dailyLimit;

	QDate *currentWeek;
	QDate *activeWeek;

	QToolBar *toolbar;
	QAction *nextWeekAct, *preWeekAct;
	QLabel *dateLabel;
	QVBoxLayout *layout;
	QHBoxLayout *budgetLine;
	QLineEdit *budgetLineEdit;

	QLabel *remainingLabel;
	QLabel *dailyLimitLabel;

	QVector<WeekEntry *> days;
	WeekEntry *weeklySpendings;

	void initWeekEntries(QDate date, Database *db = nullptr);

	void setBudget(double);
	void setRemaining(double);
	void setDailyLimit();

	void saveToDatabase();

	void switchWeek(QDate date);

	void preWeek();
	void nextWeek();

	void loadFromDatabase(QDate date, Database *db);
};

#endif // WEEKLYVIEWTOOLBAR_H
