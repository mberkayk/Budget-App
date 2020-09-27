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

	dbFile = new QFile(dirStr+"/db.json");
	qDebug() << dbFile->fileName();

	data = new QJsonDocument();

	loadFromFile();

}

void Database::selectWeek(QDate d){
	selectedWeek = d;
	QJsonObject obj = data->object();
	QString jsonKey = selectedWeek.toString();
	if(!obj.contains(jsonKey)) createNewWeek(selectedWeek);
}

double Database::getBudget(){
	if(selectedWeek.isNull()) return -1;
	QJsonObject obj = data->object();
	QJsonObject weekObj = obj[selectedWeek.toString()].toObject();
	return weekObj["budget"].toDouble();
}

Database::Color Database::getColor(){
	if(selectedWeek.isNull()) return RED;
	QJsonObject obj = data->object();
	QJsonObject weekObj = obj[selectedWeek.toString()].toObject();
	return (Color)weekObj["color"].toInt();
}

QVector<double> Database::getDay(int d){
	if(selectedWeek.isNull()) return QVector<double>();

	QJsonObject rootObj = data->object();
	QJsonObject weekObj = rootObj[selectedWeek.toString()].toObject();
	QJsonArray days = weekObj["days"].toArray();
	QJsonArray day = days[d].toArray();

	QVector<double> result;
	for(int i = 0; i < day.size(); i++){
		result.append(day[i].toDouble());
	}
	return result;
}

QVector<double> Database::getWeeklySpendings(){
	QJsonObject rootObj = data->object();
	QJsonObject weekObj = rootObj[selectedWeek.toString()].toObject();
	QJsonArray arrObj = weekObj["weekly spendings"].toArray();

	QVector<double> result;
	for(int i = 0; i < arrObj.size(); i++){
		result.append(arrObj[i].toDouble());
	}

	return result;
}

void Database::createNewWeek(QDate date){
	if(date.dayOfWeek() != 1){
		qDebug() << date.dayOfWeek();
		qDebug() << "day indicating week must be a monday";
		return;
	}
	QJsonObject week;
	week["color"] = GREEN;
	week["budget"] = 0;
	QJsonArray arr = QJsonArray();
	for(int i = 0; i < 7; i++){
		QJsonArray spendings = QJsonArray();
		spendings.append(0);
		arr.append(spendings);
	}
	week["days"] = arr;
	arr = QJsonArray();
	arr.append(0);
	week["weekly spendings"] = arr;
	QJsonObject obj = data->object();
	obj[date.toString()] = week;
	data->setObject(obj);

}

void Database::updateBudget(QDate date, double b){
	if(date.dayOfWeek() != 1){
		qDebug() << date.dayOfWeek();
		qDebug() << "day indicating week must be a monday";
		return;
	}
	QJsonObject obj = data->object();
	QJsonObject weekObj = obj[date.toString()].toObject();
	weekObj["budget"] = b;
	obj.insert(date.toString(), weekObj);
	data->setObject(obj);
}

void Database::updateDay(QDate date, QVector<double> v){
	QDate weekDate = date.addDays(1-date.dayOfWeek());
	QJsonObject rootObj = data->object();
	QJsonObject weekObj = rootObj[weekDate.toString()].toObject();
	QJsonArray days = weekObj["days"].toArray();
	QJsonArray day = QJsonArray();
	for(int i = 0; i < v.size(); i++){
		day.append(v[i]);
	}
	days[date.dayOfWeek()-1] = day;
	weekObj.insert("days", days);
	rootObj.insert(weekDate.toString(), weekObj);
	data->setObject(rootObj);
}

void Database::updateWeeklySpending(QDate date, QVector<double> v){
	if(date.dayOfWeek() != 1){
		qDebug() << date.dayOfWeek();
		qDebug() << "day indicating week must be a monday";
		return;
	}
	QJsonObject rootObj = data->object();
	QJsonObject weekObj = rootObj[date.toString()].toObject();
	QJsonArray weeklySpendings = QJsonArray();
	for(int i = 0; i < v.size(); i++){
		weeklySpendings.append(v[i]);
	}
	weekObj.insert("weekly spendings", weeklySpendings);
	rootObj.insert(date.toString(), weekObj);
	data->setObject(rootObj);
}

void Database::setColor(QDate date, Color c){
	if(date.dayOfWeek() != 1){
		qDebug() << date.dayOfWeek();
		qDebug() << "day indicating week must be a monday";
		return;
	}
	QJsonObject obj = data->object();
	QJsonObject weekObj = obj[date.toString()].toObject();
	weekObj["color"] = c;
	obj.insert(date.toString(), weekObj);
	data->setObject(obj);
}

void Database::loadFromFile(){
	if(dbFile->exists()){
		qDebug() << "file exists";
		qDebug() << "file opened: " <<
		dbFile->open(QIODevice::ExistingOnly |
					 QIODevice::ReadOnly);
	}else{
		qDebug() << "file doesn't exist";
		qDebug() << "file opened: " <<
		dbFile->open(QIODevice::NewOnly |
					 QIODevice::ReadOnly);
	}

	QByteArray arr = dbFile->readAll();
	*data = QJsonDocument(QJsonDocument::fromJson(arr));
	if(arr.isEmpty() || data->isObject() == false) {
		data->setObject(QJsonObject());
	}

	dbFile->close();
}

void Database::saveToFile(){
	if(dbFile->exists()){
		qDebug() << "file exists";
		qDebug() << "file opened: " <<
		dbFile->open(QIODevice::ExistingOnly |
					 QIODevice::WriteOnly);
	}else{
		qDebug() << "file doesn't exist";
		qDebug() << "file opened: " <<
		dbFile->open(QIODevice::NewOnly |
					 QIODevice::WriteOnly);
	}

	dbFile->write(data->toJson());
	dbFile->close();
}

Database::~Database(){

	saveToFile();

	delete dbFile;
	delete data;
}
