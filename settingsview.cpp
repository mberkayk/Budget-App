#include "settingsview.h"

SettingsView::SettingsView() {

	widget = new QWidget;

	layout = new QVBoxLayout;
	widget->setLayout(layout);

	title = new QLabel("Settings View");
	layout->addWidget(title);

}


SettingsView::~SettingsView(){
	delete widget;
	delete layout;
	delete title;
}

QWidget *SettingsView::getWidget(){
	return widget;
}
