#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QComboBox>
#include <QString>
#include <QList>
#include <QStringList>

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    bool checkFormat(QList<QStringList>);

    ~Setting();
signals:
    void sendData(QList<QStringList>);
private slots:
    void onBtnAddClicked();
    void onBtnDelClicked();
    void onBtnApplyClicked();

private:
    Ui::Setting *ui;
    QTableWidget *table;
};

#endif // SETTING_H
