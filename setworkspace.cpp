#include "setworkspace.h"
#include "ui_setworkspace.h"

extern QString globalclsname;

setworkspace::setworkspace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setworkspace)
{
    ui->setupUi(this);
}

setworkspace::~setworkspace()
{
    delete ui;
}

void setworkspace::on_buttonBox_accepted()
{
  globalclsname = ui->lineEdit->text();
}
