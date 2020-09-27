#ifndef DATABASE_H
#define DATABASE_H

#include <QDate>
#include <QFile>
#include <QDate>
#include <QJsonDocument>

class Database {

	Q_ENUMS(Color)
public:
	Database();
	~Database();
	enum Color {RED, GREEN};

	void selectWeek(QDate);
	double getBudget();
	Color getColor();
	QVector<double> getDay(int);
	QVector<double> getWeeklySpendings();

	void createNewWeek(QDate);

	void updateBudget(QDate, double);
	void updateDay(QDate date, QVector<double>);
	void updateWeeklySpending(QDate, QVector<double>);

	void setColor(QDate, Color);

	void saveToFile();

private:
	QDate selectedWeek;

	QFile *dbFile;
	QJsonDocument *data;
	void loadFromFile();

};

#endif // DATABASE_H
