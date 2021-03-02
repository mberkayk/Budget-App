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
    double getPersistentBudget();
    QVector<Entry*> getPersistentEntries(QDate&);

    void appendDayEntries(QDate&, QVector<Entry*>);
    void setWeeklyBudget(QDate&, double);
    void appendWeekEntries(QDate&, QVector<Entry*>);
    void setMonthlyBudget(QDate&, double);
    void appendMonthEntries(QDate&, QVector<Entry*>);

    void removeDailyEntry(QDate&, int);
    void removeWeeklyEntry(QDate&, int);
    void removeMonthlyEntry(QDate&, int);

    void saveDayDataToFile();
    void saveWeekDataToFile();
    void saveMonthDataToFile();
    void saveDataToFile(QJsonDocument*, QFile*);

    bool monthExists(QDate&);

private:
    QFile *dayFile;
    QFile *weekFile;
    QFile *monthFile;
    QFile *persistentFile;

    QJsonDocument *dayData;
    QJsonDocument *weekData;
    QJsonDocument *monthData;
    QJsonDocument *persistentData;

    void loadFromFile(QFile*, QJsonDocument*);

};

#endif // DATABASE_H
