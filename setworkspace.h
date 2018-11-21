#ifndef SETWORKSPACE_H
#define SETWORKSPACE_H

#include <QDialog>

namespace Ui {
class setworkspace;
}

class setworkspace : public QDialog
{
    Q_OBJECT

public:
    explicit setworkspace(QWidget *parent = 0);
    ~setworkspace();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::setworkspace *ui;
};

#endif // SETWORKSPACE_H
