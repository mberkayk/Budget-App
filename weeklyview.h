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

#include "database.h"
#include "entrygroup.h"

class SpinBoxView : public QGraphicsView {

	Q_OBJECT

private:
	int fontSize;
	int spacing;
	int selectedDay;

	int preMouseY;
	bool sceneUnstable;
	int mouseDragDir;

	QGraphicsScene * scene;
	QGraphicsTextItem *daysGraphicsItem[7] = {};

	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

public:
	SpinBoxView(QString[]);

	void updateViewPort();

public slots:
	void incrementSelectedDay();
	void decrementSelectedDay();

//	void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
};

class SpinBox : public QWidget {

	Q_OBJECT

private:
	QString days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

	QVBoxLayout *layout;

	QToolButton *upBtn;
	QGraphicsView *daysView;
	QToolButton *downBtn;

public:
	SpinBox();

	void setDay(QString);

	QString getSelectedDay();

};

class WeeklyEntryDialog : public QDialog {

	Q_OBJECT

private:
	QVBoxLayout *layout ;

	SpinBox *spinBox;

	QLineEdit *descBox;
	QLineEdit *amtBox;

	QPushButton *xBtn;
	QPushButton *okBtn;

public:
	WeeklyEntryDialog(QWidget *);

	Entry * getEntry();
};

class WeeklyView : public QWidget {

	Q_OBJECT

private:
	Database * db;
	int budget;
	QDate *date;

	QVBoxLayout *mainLayout;
	QHBoxLayout *titleBarLayout;
	QHBoxLayout *budgetInfoLayout;
	QVBoxLayout *weekSectionsLayout;

	QLabel *titleLabel;
	QPushButton *addBtn;

	QLabel *budgetLabel;
	QLabel *remainingInfoLabel;

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

	WeeklyEntryDialog *entryDialog;

private slots:
	void showEntryDialog();
	void addNewEntry();

public:
	WeeklyView(Database *database);
	~WeeklyView();

	void loadFromDatabase();
	void saveToDatabase();

};

#endif // WEEKLYVIEW_H
