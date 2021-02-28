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

    dayData = new QJsonDocument();
    weekData = new QJsonDocument();
    monthData = new QJsonDocument();

    loadFromFile(dayFile, dayData);
    loadFromFile(weekFile, weekData);
    loadFromFile(monthFile, monthData);

}

Database::~Database(){
    delete dayFile;
    delete weekFile;
    delete monthFile;

    delete dayData;
    delete weekData;
    delete monthData;
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

    QJsonObject obj = monthData->object();

    if(!obj.contains(date.toString())) {
        qDebug() << "Month doesnt exist in the database";
        return -1;
    }

    QJsonObject monthObj = obj[date.toString()].toObject();
    return monthObj["budget"].toDouble();
}

QVector<Entry*> Database::getMonthEntries(QDate &date){
    QDate _date = date;
    if(_date.day() != 1){
        qDebug() << _date.day();
        qDebug() << "day indicating month must be the first day of the month";
        return QVector<Entry*>();
    }

    QJsonObject rootObj = monthData->object();

    //If the month is being loaded for the first time
    //load the last month's entries
    if(rootObj.contains(_date.toString()) == false) {
        qDebug() << "Month doesn't exist in the database: " << date.toString();
        _date = _date.addDays(-2); //get to the previous month
        _date = _date.addDays(1 - _date.day()); //get the first day of the month
    }

    QJsonObject monthObj = rootObj[_date.toString()].toObject();
    QJsonArray entriesArr = monthObj["entries"].toArray();

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
    bool b;
    if(dayFile->exists()){
        b = dayFile->open(QIODevice::ExistingOnly |
                     QIODevice::WriteOnly);
    }else{
        b = dayFile->open(QIODevice::NewOnly |
                     QIODevice::WriteOnly);
    }
    Q_ASSERT(b);

    dayFile->write(dayData->toJson());
    dayFile->close();
}

void Database::saveWeekDataToFile(){
    bool b;
    if(weekFile->exists()){
        b = weekFile->open(QIODevice::ExistingOnly |
                     QIODevice::WriteOnly);
    }else{
        b = weekFile->open(QIODevice::NewOnly |
                     QIODevice::WriteOnly);
    }
    Q_ASSERT(b);

    weekFile->write(weekData->toJson());
    weekFile->close();
}

void Database::saveMonthDataToFile(){
    bool b;
    if(monthFile->exists()){
        b = monthFile->open(QIODevice::ExistingOnly |
                     QIODevice::WriteOnly);
    }else{
        b = monthFile->open(QIODevice::NewOnly |
                     QIODevice::WriteOnly);
    }
    Q_ASSERT(b);

    monthFile->write(monthData->toJson());
    monthFile->close();
}

