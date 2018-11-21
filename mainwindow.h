#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QLabel>
#include <QListWidgetItem>

#include "function.h"

class KindOfPicsInfo;

class RectBox
{
public:
    void clear(){
        xmin.clear();
        ymin.clear();
        width.clear();
        height.clear();
        label.clear();
    }
public:
    QList<int> xmin;
    QList<int> ymin;
    QList<int> width;
    QList<int> height;
    QList<QString> label;
};

class LabelInfo
{
public:
    QList<QString> label;
    QList<int> r;
    QList<int> g;
    QList<int> b;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void CreateRectBox();

private slots:
    void on_actionSet_Workspace_triggered();

    void on_actionNew_Class_triggered();

    void on_actionDelete_Class_triggered();

    void on_actionImport_Images_triggered();

    void on_actionSave_triggered();

    void on_actionNext_Image_triggered();

    void on_actionPrev_Image_triggered();

    void on_actionCreate_RectBox_triggered();

    void on_actionDelete_Rectbox_triggered();

    void on_actionDuplicate_RectBox_triggered();


    void on_listWidgetimg_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidgetcls_itemDoubleClicked(QListWidgetItem *item);

    void closeEvent(QCloseEvent *event);







protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);


private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QImage image;
    QPixmap pimg;
    RectBox rectbox;


    bool is_in_scene(int x, int y);
    void set_scene_image(QString file);
    void scene_add_4_point(int x, int y, int w, int h);
    void mouse_touch_some_point_when_press();
    void mouse_touch_some_rect_when_press();
    void show_mouse_touch_some_point();
    void show_mouse_in_some_rectbox();
    void show_target_rectbox();
    void mouse_point_edit_rect();
    void mouse_move_rect();
    void sort_rectbox();
    void refresh_scene();
    void refresh_rect_box();

    void save_img_xml();
    void show_list_cls();
    void show_list_img();

    bool create_rectbox_check;
    bool mouse_press_check;
    int pic_left, pic_right, pic_up, pic_down;
    int press_x, press_y, move_x, move_y, release_x, release_y;
    int target_rectbox;
    int target_x, target_y;
    int kinds_num;
    int current_cls;
    int current_pic;
    int current_pic_h;
    int current_pic_w;
    double pic_resize_ratio;
    QString workspace_path;

    QList<int> mouse_press_rect_and_point;
    QList<KindOfPicsInfo*> ws_pic;


};

#endif // MAINWINDOW_H
