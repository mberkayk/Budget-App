#ifndef MONTHLYVIEW_H
#define MONTHLYVIEW_H

#include <QDialog>

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>

#include "database.h"
#include "entrygroup.h"

class MonthlyEntryDialog : public QDialog {

	Q_OBJECT

public:
	MonthlyEntryDialog();
	~MonthlyEntryDialog();
};

class MonthlyView : public QWidget {

	Q_OBJECT

private:
	Database *db;
	int budget;

	QDate *date;

	QVBoxLayout *mainLayout;
	QHBoxLayout *titleBarLayout;
	QHBoxLayout *budgetInfoLayout;

	QLabel *titleLabel;
	QPushButton *addBtn;

	QLabel *budgetLabel;

	EntryGroup *entries;

	void addNewEntry();

public:
	MonthlyView(Database *db);
	~MonthlyView();

	void loadFromDatabase();
	void saveToDatabase();
};

#endif // MONTHLYVIEW_H
