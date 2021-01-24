#ifndef WEEKSECTION_H
#define WEEKSECTION_H

#include <QWidget>
#include <QBoxLayout>
#include <QVector>
#include <QLabel>
#include <QGroupBox>
#include <QStackedLayout>

class Entry : public QWidget {

private:
	int amount;
	QString desc;

	QHBoxLayout *layout;
	QLabel *descLabel;
	QLabel *amtLabel;

public:
	Entry(int, QString = "Expense");
	~Entry();

	int getAmount();
	QString getDesc();
};

class EntryGroup : public QGroupBox {

public:
	EntryGroup();
	EntryGroup(QString s);
	~EntryGroup();

	void collapse();
	void expand();

	void setEntries(QVector<Entry *>);
	void addEntry(Entry *);
	void removeEntry(int);

	QVector<Entry *> getEntries();
	int getTotal();

private:
	int total;
	bool collapsed;

	QStackedLayout *stackedLayout;
	QWidget *collapsedWidget;
	QWidget *expandedWidget;

	Entry *totalEntry;

	QHBoxLayout *collapsedLayout;

	QVBoxLayout *expandedLayout;

	QVector<Entry *> entries;

	void updateTotal();

};

#endif // WEEKSECTION_H
