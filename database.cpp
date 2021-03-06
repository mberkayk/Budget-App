#include "database.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

Database::Database() {
    QString dirStr = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dirStr);
    if(!dir.exists()){
        qDebug() << "data directory does not exist";
        if(dir.mkpath(dirStr)) qDebug() << "data directory was created";
        else qDebug() << "data directory couldn't be created";
    }else{
        qDebug() << "data directory alredy exists" << dirStr;
    }

    dayFile = new QFile(dirStr+"/db-day.json");
    weekFile = new QFile(dirStr+"/db-week.json");
    monthFile = new QFile(dirStr+"/db-month.json");
    persistentFile = new QFile(dirStr+"/persistent.json");

    dayData = new QJsonDocument();
    weekData = new QJsonDocument();
    monthData = new QJsonDocument();
    persistentData = new QJsonDocument();

    loadFromFile(dayFile, dayData);
    loadFromFile(weekFile, weekData);
    loadFromFile(monthFile, monthData);
    loadFromFile(persistentFile, persistentData);

}

Database::~Database(){
    delete dayFile;
    delete weekFile;
    delete monthFile;
    delete persistentFile;

    delete dayData;
    delete weekData;
    delete monthData;
    delete persistentData;
}


QVector<Entry*> Database::getDayEntries(QDate &date){
    QString jsonKey = date.toString();

    QJsonObject rootObj = dayData->object();

    if(!rootObj.contains(jsonKey)) return QVector<Entry*>();

    QJsonObject dayObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = dayObj["entries"].toArray();

    QVector<Entry*> result;
    for(int i = 0; i < entriesArr.size(); i++){
        QJsonObject entryObj = entriesArr[i].toObject();
        result.append(new Entry(entryObj.value("amt").toInt(),
                      entryObj.value("desc").toString()));
    }
    return result;
}

double Database::getWeeklyBudget(QDate &date){
    if(date.dayOfWeek() != 1){
        qDebug() << date.dayOfWeek();
        qDebug() << "day indicating week must be a monday";
        return -1;
    }

    QJsonObject obj = weekData->object();

    if(!obj.contains(date.toString())) {
        qDebug() << "week doesn't exist in the database";
        return -1;
    }

    QJsonObject weekObj = obj[date.toString()].toObject();
    return weekObj["budget"].toDouble();
}

QVector<Entry*> Database::getWeekEntries(QDate &date){
    if(date.dayOfWeek() != 1){
        qDebug() << date.dayOfWeek();
        qDebug() << "day indicating week must be a monday";
        return QVector<Entry*>();
    }

    QJsonObject rootObj = weekData->object();

    if(!rootObj.contains(date.toString())) {
        qDebug() << "week doesn't exist in the database";
        return QVector<Entry*>();
    }

    QJsonObject weekObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = weekObj["entries"].toArray();

    QVector<Entry*> result;
    for(int i = 0; i < entriesArr.size(); i++){
        QJsonObject entryObj = entriesArr[i].toObject();
        result.append(new Entry(entryObj.value("amt").toInt(),
                      entryObj.value("desc").toString()));
    }
    return result;
}

double Database::getMonthlyBudget(QDate &date){
    if(date.day() != 1){
        qDebug() << date.day();
        qDebug() << "day indicating month must be the first day of the month";
        return -1;
    }

    QJsonObject rootObj = monthData->object();

    if(!rootObj.contains(date.toString())) {
        qDebug() << "Month doesnt exist in the database";
        return persistentData->object()["budget"].toDouble();
    }

    QJsonObject monthObj = rootObj[date.toString()].toObject();
    return monthObj["budget"].toDouble();
}

QVector<Entry*> Database::getMonthEntries(QDate &date){
    if(date.day() != 1){
        qDebug() << date.day();
        qDebug() << "day indicating month must be the first day of the month";
        return QVector<Entry*>();
    }

    QJsonObject rootObj = monthData->object();
    QJsonObject monthObj = rootObj[date.toString()].toObject();

    if(rootObj.contains(date.toString()) == false) {
        qDebug() << "Month doesn't exist in the database: " << date.toString();
        monthObj = persistentData->object();
    }

    QJsonArray entriesArr = monthObj["entries"].toArray();

    QVector<Entry*> result;
    for(int i = 0; i < entriesArr.size(); i++){
        QJsonObject entryObj = entriesArr[i].toObject();
        result.append(new Entry(entryObj.value("amt").toInt(),
                      entryObj.value("desc").toString()));
    }
    return result;
}

double Database::getPersistentBudget(){
    QJsonObject obj = persistentData->object();
    return obj["budget"].toDouble();
}

QVector<Entry*> Database::getPersistentEntries(){
    QJsonObject rootObj = monthData->object();

    QJsonArray entriesArr = rootObj["entries"].toArray();

    QVector<Entry*> result;
    for(int i = 0; i < entriesArr.size(); i++){
        QJsonObject entryObj = entriesArr[i].toObject();
        result.append(new Entry(entryObj.value("amt").toInt(),
                      entryObj.value("desc").toString()));
    }
    return result;
}


void Database::appendDayEntries(QDate &date, QVector<Entry *> v) {
    QJsonObject rootObj = dayData->object();
    QJsonObject dayObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = dayObj["entries"].toArray();

    for(int i = 0; i < v.size(); i++){
        QJsonObject entryObj = QJsonObject();
        entryObj.insert("desc", v.at(i)->getDesc());
        entryObj.insert("amt", v.at(i)->getAmount());
        entriesArr.append(entryObj);
    }
    dayObj.insert("entries", entriesArr);
    rootObj.insert(date.toString(), dayObj);
    dayData->setObject(rootObj);
}

void Database::setWeeklyBudget(QDate &date, double b){
    if(date.dayOfWeek() != 1){
        qDebug() << date.dayOfWeek();
        qDebug() << "day indicating week must be a monday";
        return;
    }
    QJsonObject obj = weekData->object();
    QJsonObject weekObj = obj[date.toString()].toObject();
    weekObj["budget"] = b;
    obj.insert(date.toString(), weekObj);
    weekData->setObject(obj);
}

void Database::appendWeekEntries(QDate &date, QVector<Entry *> v) {
    if(date.dayOfWeek() != 1){
        qDebug() << date.dayOfWeek();
        qDebug() << "day indicating week must be the first day of the week";
        return;
    }
    QJsonObject rootObj = weekData->object();
    QJsonObject weekObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = weekObj["entries"].toArray();

    for(int i = 0; i < v.size(); i++){
        QJsonObject entryObj = QJsonObject();
        entryObj.insert("desc", v.at(i)->getDesc());
        entryObj.insert("amt", v.at(i)->getAmount());
        entriesArr.append(entryObj);
    }
    weekObj.insert("entries", entriesArr);
    rootObj.insert(date.toString(), weekObj);
    weekData->setObject(rootObj);
}

void Database::setMonthlyBudget(QDate &date, double budget){
    if(date.day() != 1){
        qDebug() << date.day();
        qDebug() << "day indicating month must be the first day of the month";
        return;
    }

    QJsonObject rootObj = monthData->object();
    QJsonObject monthObj = rootObj[date.toString()].toObject();
    monthObj["budget"] = budget;
    rootObj.insert(date.toString(), monthObj);
    monthData->setObject(rootObj);
}

void Database::appendMonthEntries(QDate &date, QVector<Entry *> v){
    if(date.day() != 1){
        qDebug() << date.day();
        qDebug() << "day indicating month must be the first day of the month";
        return;
    }
    QJsonObject rootObj = monthData->object();
    QJsonObject monthObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = monthObj["entries"].toArray();

    for(int i = 0; i < v.size(); i++){
        QJsonObject entryObj = QJsonObject();
        entryObj.insert("desc", v.at(i)->getDesc());
        entryObj.insert("amt", v.at(i)->getAmount());
        entriesArr.append(entryObj);
    }
    monthObj.insert("entries", entriesArr);
    rootObj.insert(date.toString(), monthObj);
    monthData->setObject(rootObj);
}


void Database::removeDailyEntry(QDate &date, int id){
    QJsonObject rootObj = dayData->object();
    QJsonObject dayObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = dayObj["entries"].toArray();

    entriesArr.removeAt(id);

    dayObj.insert("entries", entriesArr);
    rootObj.insert(date.toString(), dayObj);
    dayData->setObject(rootObj);
}

void Database::removeWeeklyEntry(QDate &date , int id){
    if(date.dayOfWeek() != 1){
        qDebug() << date.dayOfWeek();
        qDebug() << "day indicating week must be the first day of the week";
        return;
    }
    QJsonObject rootObj = weekData->object();
    QJsonObject weekObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = weekObj["entries"].toArray();

    entriesArr.removeAt(id);

    weekObj.insert("entries", entriesArr);
    rootObj.insert(date.toString(), weekObj);
    weekData->setObject(rootObj);
}

void Database::removeMonthlyEntry(QDate &date, int id){
    if(date.day() != 1){
        qDebug() << date.day();
        qDebug() << "day indicating month must be the first day of the month";
        return;
    }
    QJsonObject rootObj = monthData->object();
    QJsonObject monthObj = rootObj[date.toString()].toObject();
    QJsonArray entriesArr = monthObj["entries"].toArray();

    entriesArr.removeAt(id);

    monthObj.insert("entries", entriesArr);
    rootObj.insert(date.toString(), monthObj);
    monthData->setObject(rootObj);
}


void Database::loadFromFile(QFile *file, QJsonDocument *data){
    bool b;
    if(file->exists()){
        b = file->open(QIODevice::ExistingOnly |
                     QIODevice::ReadOnly);
    }else{
        b = file->open(QIODevice::NewOnly |
                     QIODevice::ReadOnly);
    }
    Q_ASSERT(b);

    QByteArray arr = file->readAll();
    *data = QJsonDocument(QJsonDocument::fromJson(arr));
    if(arr.isEmpty() || data->isObject() == false) {
        data->setObject(QJsonObject());
    }

    file->close();
}


void Database::saveDayDataToFile(){
    saveDataToFile(dayData, dayFile);
}

void Database::saveWeekDataToFile(){
    saveDataToFile(weekData, weekFile);
}

void Database::saveMonthDataToFile(){
    if(*monthData != *persistentData){
        QDate month = QDate::currentDate().addDays(1 - QDate::currentDate().day());
        persistentData->setObject(monthData->object()[month.toString()].toObject());
    }
    saveDataToFile(monthData, monthFile);
    saveDataToFile(persistentData, persistentFile);
}

void Database::saveDataToFile(QJsonDocument *data, QFile *file){
    bool b;
    if(file->exists()){
        b = file->open(QIODevice::ExistingOnly |
                     QIODevice::WriteOnly);
    }else{
        b = file->open(QIODevice::NewOnly |
                     QIODevice::WriteOnly);
    }
    Q_ASSERT(b);

    file->write(data->toJson());
    file->close();
}


bool Database::monthExists(QDate &date){
    return monthData->object().contains(date.toString());
}
