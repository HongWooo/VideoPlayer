#ifndef CHART_H
#define CHART_H

#include <QWidget>

namespace Ui {
class Chart;
}

class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = 0);
    ~Chart();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::Chart *ui;
    double getData(double time);

};

#endif // CHART_H
