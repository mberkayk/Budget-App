#ifndef WEEKLYVIEW_H
#define WEEKLYVIEW_H

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include "database.h"
#include "entrygroup.h"

class WeeklyView {

private:
	QDate *date;

	QWidget *widget;

	QVBoxLayout *mainLayout;
	QHBoxLayout *titleBarLayout;
	QHBoxLayout *budgetInfoLayout;
	QVBoxLayout *weekSectionsLayout;

	QPushButton *menuBtn;
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

public:
	WeeklyView();
	~WeeklyView();

	void loadEntries(Database *db);

	QWidget* getWidget();

	QPushButton* getMenuBtn();

};

#endif // WEEKLYVIEW_H
