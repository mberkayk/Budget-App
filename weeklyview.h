#ifndef WEEKLYVIEW_H
#define WEEKLYVIEW_H

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QGraphicsView>

#include "database.h"
#include "entrygroup.h"

class SpinBox : public QWidget {

	Q_OBJECT

private:
	QString days[7] = {"Monday", "Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday"};

	QVBoxLayout *layout;

	QPushButton *upBtn;
	QGraphicsScene *daysScene;
	QGraphicsView *daysView;
	QPushButton *downBtn;

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
