#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

	mainLayout = new QVBoxLayout;
	centralWidget = new QWidget();
	centralWidget->setLayout(mainLayout);

	stackedWidget = new QStackedWidget();
	mainLayout->addWidget(stackedWidget);

	weeklyView = new WeeklyView();
	stackedWidget->addWidget(weeklyView->getWidget());

	monthlyView = new MonthlyView();
	stackedWidget->addWidget(monthlyView->getWidget());

	settingsView = new SettingsView;
	stackedWidget->addWidget(settingsView->getWidget());

	bottomBar = new BottomBar();
	mainLayout->addWidget(bottomBar->getWidget());

	setCentralWidget(centralWidget);

	//sidebar actions
	QObject::connect(bottomBar->getWeeklyViewAction(), SIGNAL(triggered()),
					 this, SLOT(showWeeklyView()));
	QObject::connect(bottomBar->getMonthlyViewAction(), SIGNAL(triggered()),
					 this, SLOT(showMonthlyView()));
	QObject::connect(bottomBar->getSettingsViewAction(), SIGNAL(triggered()),
					 this, SLOT(showSettingsView()));
}

MainWindow::~MainWindow() {
	delete mainLayout;
	delete stackedWidget;
	delete centralWidget;
	delete weeklyView;
	delete monthlyView;
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

void MainWindow::setDatabase(Database *d){
	db = d;
}

QStackedWidget * MainWindow::getWidget(){return stackedWidget;}
