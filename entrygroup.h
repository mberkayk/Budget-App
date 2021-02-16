#ifndef WEEKSECTION_H
#define WEEKSECTION_H

#include <QWidget>
#include <QBoxLayout>
#include <QVector>
#include <QLabel>
#include <QGroupBox>
#include <QStackedWidget>
#include <QDebug>
#include <QEvent>
#include <QGesture>
#include <QTimer>

class Entry : public QWidget {

	Q_OBJECT

private:
	bool unsaved;
	bool toBeRemoved;

	int amount;
	QString desc;

	QHBoxLayout *layout;
	QLabel *descLabel;
	QLabel *amtLabel;

	bool event(QEvent*) override;

public:
	Entry(int, QString = "Expense");

	void setAmount(int);
	void setUnsaved(bool);
	void setToBeRemoved(bool);

	int getAmount();
	QString getDesc();
	bool getUnsaved();
	bool getToBeRemoved();

signals:
	void entrySelectedSignal(Entry*);

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

	QTimer mouseClickTimer;

	void updateTotal();
	void updateTitle();

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

public:
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

public slots:
	void entrySelectedSlot(Entry*);

signals:
	void entrySelectedSignal(EntryGroup*, Entry*);

};

#endif // WEEKSECTION_H
