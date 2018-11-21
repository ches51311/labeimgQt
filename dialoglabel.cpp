#include "dialoglabel.h"
#include "ui_dialoglabel.h"
#include "mainwindow.h"
#include <QListWidgetItem>

extern LabelInfo globallabelinfo;
extern QString globallabel;

DialogLabel::DialogLabel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLabel)
{
    ui->setupUi(this);

    ui->lineEdit->setText(globallabel);

    for (int i=0; i<globallabelinfo.label.size(); i++)
    {
        ui->listWidget->addItem(globallabelinfo.label[i]);
    }

    globallabel.clear();

}

DialogLabel::~DialogLabel()
{
    delete ui;
}

void DialogLabel::on_buttonBox_accepted()
{
    globallabel = ui->lineEdit->text();
}

void DialogLabel::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    globallabel = ui->listWidget->currentItem()->text();
    ui->lineEdit->setText(globallabel);

}
