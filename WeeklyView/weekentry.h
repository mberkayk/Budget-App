#ifndef DAYENTRY_H
#define DAYENTRY_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>

#include "customlineedit.h"

class WeekEntry : public QWidget {

Q_OBJECT

signals:
	void sumChanged();

public slots:
	void manageEntryBoxes();
	void checkIfEmpty();

public:
	WeekEntry(QWidget *parent, QString s, QVector<double> v = QVector<double>());
	~WeekEntry();
	void addEntryBox(QString s = "");
	void updateSum();
	double getSum();

	void setAddEmptyLineEdit(bool);

	QVector<double> getEntries();
	void setEntries(QVector<double>);

private:
	bool addEmptyLineEdit;
	double sum;
	QVBoxLayout *VLayout;
	QHBoxLayout *HLayout;
	QLabel *nameLabel;
	QLabel *sumLabel;

	QDoubleValidator *doubleVal;
	QVector<CustomLineEdit*> entries;
};

#endif // DAYENTRY_H
