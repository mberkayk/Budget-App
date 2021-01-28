#include "mainwindow.h"

MainWindow::MainWindow(Database *database) : QMainWindow() {

	db = database;

	mainLayout = new QVBoxLayout;
	centralWidget = new QWidget();
	centralWidget->setLayout(mainLayout);

	stackedWidget = new QStackedWidget();
	mainLayout->addWidget(stackedWidget);
	mainLayout->setStretchFactor(stackedWidget, 9);

	weeklyView = new WeeklyView(db);
	stackedWidget->addWidget(weeklyView->getWidget());

	monthlyView = new MonthlyView(db);
	stackedWidget->addWidget(monthlyView->getWidget());

	settingsView = new SettingsView;
	stackedWidget->addWidget(settingsView->getWidget());

	bottomBar = new BottomBar();
	mainLayout->addWidget(bottomBar->getWidget());
	mainLayout->setStretchFactor(bottomBar->getWidget(), 1);

	setCentralWidget(centralWidget);

	//sidebar actions
	QObject::connect(bottomBar->getWeeklyViewAction(), SIGNAL(pressed()),
					 this, SLOT(showWeeklyView()));
	QObject::connect(bottomBar->getMonthlyViewAction(), SIGNAL(pressed()),
					 this, SLOT(showMonthlyView()));
	QObject::connect(bottomBar->getSettingsViewAction(), SIGNAL(pressed()),
					 this, SLOT(showSettingsView()));
}

MainWindow::~MainWindow() {
	delete mainLayout;
	delete stackedWidget;
	delete centralWidget;
	delete weeklyView;
	delete monthlyView;
	delete settingsView;
}

void MainWindow::showMonthlyView(){
	stackedWidget->setCurrentWidget(monthlyView->getWidget());
}

void MainWindow::showWeeklyView(){
	stackedWidget->setCurrentWidget(weeklyView->getWidget());
}

void MainWindow::showSettingsView(){

	stackedWidget->setCurrentWidget(settingsView->getWidget());
}

QStackedWidget * MainWindow::getWidget(){return stackedWidget;}
