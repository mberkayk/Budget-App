#include "database.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

Database::Database() {
	QString dirStr = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QDir dir;
	if(!dir.exists(dirStr)){
		qDebug() << "data directory does not exist";
		if(dir.mkdir(dirStr)) qDebug() << "data directory was created";
		else qDebug() << "data directory couldn't be created";
	}else{
		qDebug() << "data directory alredy exists";
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
	if(date.day() != 1){
		qDebug() << date.day();
		qDebug() << "day indicating month must be the first day of the month";
		return QVector<Entry*>();
	}

	QJsonObject rootObj = monthData->object();

	if(!rootObj.contains(date.toString())) {
		qDebug() << "Month doesnt exist in the database";
		return QVector<Entry*>();
	}

	QJsonObject monthObj = rootObj[date.toString()].toObject();
	QJsonArray entriesArr = monthObj["entries"].toArray();

	QVector<Entry*> result;
	for(int i = 0; i < entriesArr.size(); i++){
		QJsonObject entryObj = entriesArr[i].toObject();
		result.append(new Entry(entryObj.value("amt").toInt(),
					  entryObj.value("desc").toString()));
	}
	return result;
}



void Database::setDayEntries(QDate &date, QVector<Entry *> v) {
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

void Database::setWeekEntries(QDate &date, QVector<Entry *> v) {
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

	QJsonObject obj = monthData->object();
	QJsonObject weekObj = obj[date.toString()].toObject();
	weekObj["budget"] = budget;
	obj.insert(date.toString(), weekObj);
	monthData->setObject(obj);
}

void Database::setMonthEntries(QDate &date, QVector<Entry *> v){
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



void Database::loadFromFile(QFile *file, QJsonDocument *data){
	if(file->exists()){
		qDebug() << "file exists";
		qDebug() << "file opened: " <<
		file->open(QIODevice::ExistingOnly |
					 QIODevice::ReadOnly);
	}else{
		qDebug() << "file doesn't exist";
		qDebug() << "file opened: " <<
		file->open(QIODevice::NewOnly |
					 QIODevice::ReadOnly);
	}

	QByteArray arr = file->readAll();
	*dayData = QJsonDocument(QJsonDocument::fromJson(arr));
	if(arr.isEmpty() || data->isObject() == false) {
		data->setObject(QJsonObject());
	}

	file->close();
}


void Database::saveDayData(){
	if(dayFile->exists()){
		qDebug() << "file exists";
		qDebug() << "file opened: " <<
		dayFile->open(QIODevice::ExistingOnly |
					 QIODevice::WriteOnly);
	}else{
		qDebug() << "file doesn't exist";
		qDebug() << "file opened: " <<
		dayFile->open(QIODevice::NewOnly |
					 QIODevice::WriteOnly);
	}

	dayFile->write(dayData->toJson());
	dayFile->close();
}

void Database::saveWeekData(){
	if(weekFile->exists()){
		qDebug() << "file exists";
		qDebug() << "file opened: " <<
		weekFile->open(QIODevice::ExistingOnly |
					 QIODevice::WriteOnly);
	}else{
		qDebug() << "file doesn't exist";
		qDebug() << "file opened: " <<
		weekFile->open(QIODevice::NewOnly |
					 QIODevice::WriteOnly);
	}

	weekFile->write(weekData->toJson());
	weekFile->close();
}

void Database::saveMonthData(){
	if(monthFile->exists()){
		qDebug() << "file exists";
		qDebug() << "file opened: " <<
		monthFile->open(QIODevice::ExistingOnly |
					 QIODevice::WriteOnly);
	}else{
		qDebug() << "file doesn't exist";
		qDebug() << "file opened: " <<
		monthFile->open(QIODevice::NewOnly |
					 QIODevice::WriteOnly);
	}

	monthFile->write(monthData->toJson());
	monthFile->close();
}

