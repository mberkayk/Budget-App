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

class EntryGroupWidget : public QGroupBox {

public:
	EntryGroupWidget();
	EntryGroupWidget(QString s);
	~EntryGroupWidget();

	void collapse();
	void expand();

private:
	int total;
	bool collapsed;

	QStackedLayout *stackedLayout;
	QWidget *collapsedWidget;
	QWidget *expandedWidget;

	Entry *totalEntry;

	QHBoxLayout *collapsedLayout;

	QVBoxLayout *expandedLayout;

};

class EntryGroup {
private:

	EntryGroupWidget *widget;

	QHBoxLayout *mainLayout;
	QVBoxLayout *descLayout;
	QVBoxLayout *amountLayout;

	QLabel *titleLabel;
	QLabel *totalLabel;

	QVector<Entry *> entries;

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
