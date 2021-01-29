#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class SettingsView : public QWidget {

	Q_OBJECT

private:
	QVBoxLayout *layout;
	QLabel *title;

public:
	SettingsView();

	QWidget *getWidget();
};

#endif // SETTINGSVIEW_H
