#include "setting.h"
#include "ui_setting.h"
#include <qdebug.h>
#include <qmessagebox.h>

Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    table = ui->tableWidget;
    table->setColumnCount(4);
    table->setRowCount(0);
    QStringList colnames = {"名称", "地址","状态","安装地址"};
    table->setHorizontalHeaderLabels(colnames);
    table->setColumnWidth(0,200);
    table->setColumnWidth(1,500);
    table->setColumnWidth(2,100);
    table->setColumnWidth(3,600);
    table->horizontalHeader()->setStretchLastSection(true);
    connect(ui->btn_add,SIGNAL(clicked(bool)),this,SLOT(onBtnAddClicked()));
    connect(ui->btn_del,SIGNAL(clicked(bool)),this,SLOT(onBtnDelClicked()));
    connect(ui->btn_apply,SIGNAL(clicked(bool)),this,SLOT(onBtnApplyClicked()));

}

Setting::~Setting()
{
    delete ui;
}

void Setting::onBtnAddClicked(){
    int lastrow = table->rowCount();

    table->insertRow(table->rowCount());
    QComboBox *comBox = new QComboBox();
    comBox->addItem("on");
    comBox->addItem("off");
    table->setCellWidget(lastrow,2,comBox);

}

void Setting::onBtnDelClicked(){
    int rowIndex = table->currentRow();
    table->removeRow(rowIndex);
}

void Setting::onBtnApplyClicked(){
    QList<QStringList> datalist;    //二维数组，存储摄像机配置信息

    qDebug()<<table->rowCount()<<","<<table->columnCount()<<endl;
    for(int i=0;i<table->rowCount();i++){
        QStringList rowInfo;
        for(int j=0;j<table->columnCount();j++){
            if(j!=table->columnCount()-2){
                QString str;

                if(table->item(i,j)==NULL||             //判断指向该cell的指针是否为空
                             (table->item(i,j)&&
                              table->item(i,j)->text()==tr(""))) //判断该cell的text是否为空
                {
                        str = "";
                }
                else{
                    str = table->item(i,j)->text();
                }
                rowInfo.append(str);
            }
            else {
                //开关
                QWidget *wig = table->cellWidget(i,j);
                QComboBox *status = (QComboBox*)wig;
                rowInfo.append(status->currentText());
            }
        }
        datalist.append(rowInfo);
    }
    for(int i=0;i<datalist.size();i++){
        for(int j=0;j<datalist[i].size();j++){
            qDebug()<<datalist[i][j]<<'\t';
        }
        qDebug()<<endl;
    }



//    向monitor发送信息
    if(checkFormat(datalist))   //格式正常
        emit sendData(datalist);
    else{
        QMessageBox::information(NULL,"错误","内容不能为空");
    }
}

bool Setting::checkFormat(QList<QStringList> data){
    bool flag = true;

    for(int i=0;i<data.size();i++){
        for(int j=0;j<data[i].size();j++){
            if(data[i][j]=="" ||data[i][j].isEmpty() ||data[i][j].isNull())
                flag = false;
        }
    }
    qDebug()<<"flag is "<<flag;
    return flag;
}


