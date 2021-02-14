#ifndef WEEKSECTION_H
#define WEEKSECTION_H

#include <QWidget>
#include <QBoxLayout>
#include <QVector>
#include <QLabel>
#include <QGroupBox>
#include <QStackedWidget>
#include <QDebug>

class Entry : public QWidget {

	Q_OBJECT

private:
	bool unsaved;

	int amount;
	QString desc;

	QHBoxLayout *layout;
	QLabel *descLabel;
	QLabel *amtLabel;

public:
	Entry(int, QString = "Expense");

	void setAmount(int);
	void setUnsaved(bool);

	int getAmount();
	QString getDesc();
	bool getUnsaved();
};

class EntryGroup : public QGroupBox {

	Q_OBJECT

private:
	int total;
	bool collapsed;
	QString titleStr;

	QStackedWidget *stackedWidget;
	QWidget *collapsedWidget;
	QWidget *expandedWidget;

	QVBoxLayout *expandedLayout;

	QVector<Entry *> entries;

	void updateTotal();
	void updateTitle();

	void mousePressEvent(QMouseEvent *event) override;

public:
	EntryGroup();
	EntryGroup(QString s);
	~EntryGroup();

	void collapse();
	void expand();

	void setEntries(QVector<Entry *>);
	void addEntry(Entry *);
	void removeEntry(int);

	QVector<Entry *> getUnsavedEntries();
	QVector<Entry *> getEntries();
	int getTotal();

};

#endif // WEEKSECTION_H
