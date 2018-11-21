#ifndef DIALOGLABEL_H
#define DIALOGLABEL_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class DialogLabel;
}

class DialogLabel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLabel(QWidget *parent = 0);
    ~DialogLabel();

private slots:
    void on_buttonBox_accepted();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::DialogLabel *ui;
};

#endif // DIALOGLABEL_H
