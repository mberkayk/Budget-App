#include "settingsview.h"


EditableSettingLabel::EditableSettingLabel(QString _setting, QString label) : QLabel(label) {
    setting = _setting;
    grabGesture(Qt::TapAndHoldGesture);
}

bool EditableSettingLabel::event(QEvent *event){
    if(event->type() == QEvent::Gesture){
        QGestureEvent *gEvent = static_cast<QGestureEvent*>(event);
        if(QGesture *hold = gEvent->gesture(Qt::TapAndHoldGesture)){
            if(hold->state() == Qt::GestureFinished){
                setText(QInputDialog::getText(this, "Edit Setting", "Enter A New Value:", QLineEdit::Normal, text()));
                emit setttingEditedSignal(this);
            }
        }
    }

    return QWidget::event(event);
}

QString EditableSettingLabel::getSetting(){return setting;}


SettingsView::SettingsView() : QWidget () , settings(){

    layout = new QVBoxLayout;
    setLayout(layout);

    title = new QLabel("Settings View");
    layout->addWidget(title);

    if(settings.contains("default daily description") == false)
        settings.setValue("default daily description", "Food");

    if(settings.contains("default weekly description") == false)
        settings.setValue("default weekly description", "Groceries");

    if(settings.contains("default monthly description") == false)
        settings.setValue("default monthly description", "Monthly Bill");

    QFormLayout *formLayout = new QFormLayout;
    layout->addLayout(formLayout);

    dailyDefaultEditableSetting = new EditableSettingLabel("default daily description",
                                                           settings.value("default daily description").toString());
    formLayout->addRow(new QLabel("Default Daily Description: "), dailyDefaultEditableSetting);
    QObject::connect(dailyDefaultEditableSetting, &EditableSettingLabel::setttingEditedSignal,
                     this, &SettingsView::settingEditedSlot);


    weeklyDefaultEditableSetting = new EditableSettingLabel("default weekly description",
                                                            settings.value("default weekly description").toString());
    formLayout->addRow(new QLabel("Default Weekly Description: "), weeklyDefaultEditableSetting);
    QObject::connect(weeklyDefaultEditableSetting, &EditableSettingLabel::setttingEditedSignal,
                     this, &SettingsView::settingEditedSlot);

    monthlyDefaultEditableSetting = new EditableSettingLabel("default monthly description",
                                                           settings.value("default monthly description").toString());
    formLayout->addRow(new QLabel("Default monthly Description: "), monthlyDefaultEditableSetting);
    QObject::connect(monthlyDefaultEditableSetting, &EditableSettingLabel::setttingEditedSignal,
                     this, &SettingsView::settingEditedSlot);

}

void SettingsView::settingEditedSlot(EditableSettingLabel* label){
    settings.setValue(label->getSetting(), label->text());
}
