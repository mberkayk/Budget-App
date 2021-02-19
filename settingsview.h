#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QFormLayout>
#include <QTapAndHoldGesture>
#include <QInputDialog>



class EditableSettingLabel : public QLabel {
    Q_OBJECT

public:
    EditableSettingLabel(QString, QString);

    QString getSetting();

private:

    QString setting;

    bool event(QEvent*) override;

signals:
    void setttingEditedSignal(EditableSettingLabel*);

};



class SettingsView : public QWidget {

    Q_OBJECT

private:
    QVBoxLayout *layout;
    QLabel *title;

    QSettings settings;
    EditableSettingLabel *dailyDefaultEditableSetting;
    EditableSettingLabel *weeklyDefaultEditableSetting;
    EditableSettingLabel *monthlyDefaultEditableSetting;

public:
    SettingsView();

    QWidget *getWidget();

private slots:
    void settingEditedSlot(EditableSettingLabel*);
};

#endif // SETTINGSVIEW_H
