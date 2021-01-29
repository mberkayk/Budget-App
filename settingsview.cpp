#include "settingsview.h"

SettingsView::SettingsView() : QWidget (){

	layout = new QVBoxLayout;
        setLayout(layout);

	title = new QLabel("Settings View");
	layout->addWidget(title);

}
