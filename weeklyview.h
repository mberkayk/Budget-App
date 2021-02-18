#ifndef WEEKLYVIEW_H
#define WEEKLYVIEW_H

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QGraphicsView>
#include <QToolButton>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QScrollBar>
#include <QStackedWidget>
#include <QMessageBox>
#include <QSettings>

#include "database.h"
#include "entrygroup.h"

class SpinBoxView : public QGraphicsView {

	Q_OBJECT

private:
	int fontSize;
	int spacing;
	int selectedItem;

	bool sceneUnstable;

	QGraphicsScene * scene;
	QGraphicsTextItem *daysGraphicsItem[7] = {};

	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

public:
	SpinBoxView(QVector<QString>);

	void updateViewPort();

	void select(int);
	int getSelectedItemIndex();

public slots:
	void incrementSelectedDay();
	void decrementSelectedDay();

//	void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
};

class SpinBox : public QWidget {

	Q_OBJECT

private:
	QVector<QString> items;

	QVBoxLayout *layout;

	QToolButton *upBtn;
	SpinBoxView *spinBoxView;
	QToolButton *downBtn;

public:
	SpinBox(QVector<QString>);

	void select(int);
	int getSelectedItemIndex();

};

class DailyEntryDialog : public QDialog {

	Q_OBJECT

private:
	QVBoxLayout *layout ;

	SpinBox *spinBox;

	QLineEdit *descBox;
	QLineEdit *amtBox;

	QPushButton *xBtn;
	QPushButton *okBtn;

public:
	DailyEntryDialog(QWidget *);

	Entry * createEntry();

	int getSelectedDay();
};

class WeeklyEntryDialog : public QDialog {

	Q_OBJECT

private:
	QVBoxLayout *layout ;
	QLineEdit *descBox;
	QLineEdit *amtBox;

	QPushButton *xBtn;
	QPushButton *okBtn;

public:
	WeeklyEntryDialog(QWidget *);

	Entry * createEntry();
};

class WeeklyView : public QWidget {

	Q_OBJECT

private:
	Database * db;

	int budget;
	int remaining;
	int todaysLimit;
	int dailyBudgetForTheWeek;
	int todaysRemaining;

	QDate date;

	QVBoxLayout *mainLayout;
	QHBoxLayout *titleBarLayout;
	QHBoxLayout *budgetInfoLayout;
	QHBoxLayout *buttonsLayout;

	QScrollArea *dailyEntryGroupsScrollArea;
	QStackedWidget *dailyGroupsStackedWidget;
	QLabel *noEntriesLabel;
	QWidget *dailyGroupsListWidget;
	QVBoxLayout *dailyEntryGroupsListLayout;

	QVBoxLayout *weekEntryGroupLayout;

	QLabel *titleLabel;

	QLabel *budgetLabel;
	QLabel *remainingInfoLabel;

	QPushButton *addDailyEntryButton;
	QPushButton *addWeeklyEntryButton;

	EntryGroup *groups[8] = {
		new EntryGroup("Monday"),
		new EntryGroup("Tuesday"),
		new EntryGroup("Wednesday"),
		new EntryGroup("Thursday"),
		new EntryGroup("Friday"),
		new EntryGroup("Saturday"),
		new EntryGroup("Sunday"),
		new EntryGroup("Weekly Expenses")
	};

	DailyEntryDialog *dailyEntryDialog;
	WeeklyEntryDialog *weeklyEntryDialog;

	void calculateNumbers();

private slots:
	void showDailyEntryDialog();
	void showWeeklyEntryDialog();
	void addNewDailyEntry();
	void addNewWeeklyEntry();

	void entrySelectedSlot(EntryGroup *, int);

public:
	WeeklyView(Database *database);

	void loadFromDatabase();
	void saveToDatabase();

	void setBudget(int);


};

#endif // WEEKLYVIEW_H
