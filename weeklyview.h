#ifndef WEEKLYVIEW_H
#define WEEKLYVIEW_H

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include "database.h"
#include "entrygroup.h"

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

	void addNewEntry();

public:
	WeeklyView(Database *database);
	~WeeklyView();

	void loadFromDatabase();
	void saveToDatabase();

};

#endif // WEEKLYVIEW_H
