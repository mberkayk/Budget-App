#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class SettingsView {

private:
	QWidget *widget;
	QVBoxLayout *layout;
	QLabel *title;

public:
	SettingsView();
	~SettingsView();

	QWidget *getWidget();
};

#endif // SETTINGSVIEW_H
