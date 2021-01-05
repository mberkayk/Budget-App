#ifndef WEEKSECTION_H
#define WEEKSECTION_H

#include <QWidget>
#include <QBoxLayout>
#include <QVector>
#include <QLabel>
#include "entry.h"

class EntryGroup {
private:

	QWidget *widget;

	QHBoxLayout *mainLayout;
	QVBoxLayout *descLayout;
	QVBoxLayout *amountLayout;

	QLabel *titleLabel;
	QLabel *totalLabel;
	int total;

	QVector<Entry *> entries;
	QVector<QLabel *> entryDesc;
	QVector<QLabel *> entryAmt;

	bool collapsed;

	void updateTotal();

public:
	EntryGroup(QString);
	~EntryGroup();

	QWidget *getWidget();

	void setEntries(QVector<Entry *>);
	void addEntry(Entry *);
	void removeEntry(int);

	void collapse();
	void expand();

	QVector<Entry *> getEntries();
	int getTotal();

};

#endif // WEEKSECTION_H
