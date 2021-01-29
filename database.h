#ifndef DATABASE_H
#define DATABASE_H

#include <QDate>
#include <QFile>
#include <QJsonDocument>

#include "entrygroup.h"

class Database {

public:
	Database();
	~Database();

	QVector<Entry*> getDayEntries(QDate&);
	double getWeeklyBudget(QDate&);
	QVector<Entry*> getWeekEntries(QDate&);
	double getMonthlyBudget(QDate&);
	QVector<Entry*> getMonthEntries(QDate&);

	void appendDayEntries(QDate&, QVector<Entry*>);
	void setWeeklyBudget(QDate&, double);
	void appendWeekEntries(QDate&, QVector<Entry*>);
	void setMonthlyBudget(QDate&, double);
	void appendMonthEntries(QDate&, QVector<Entry*>);

	void saveDayData();
	void saveWeekData();
	void saveMonthDataToFile();

private:
	QFile *dayFile;
	QFile *weekFile;
	QFile *monthFile;

	QJsonDocument *dayData;
	QJsonDocument *weekData;
	QJsonDocument *monthData;

	void loadFromFile(QFile*, QJsonDocument*);

};

#endif // DATABASE_H
