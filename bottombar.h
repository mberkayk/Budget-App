#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>

class BottomBar : public QWidget {

	Q_OBJECT

private:
	QHBoxLayout *layout;

	QButtonGroup butts;
	QPushButton *weeklyViewButt;
	QPushButton *monthlyViewButt;
	QPushButton *settingsViewButt;

public:
	BottomBar();

	QWidget *getWeeklyViewAction();
	QWidget *getMonthlyViewAction();
	QWidget *getSettingsViewAction();

private slots:
	void exitSlot();

signals:
	void exit();
};

#endif // SIDEBAR_H
