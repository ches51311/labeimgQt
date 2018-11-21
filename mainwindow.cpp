#include "function.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <iostream>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <dialoglabel.h>
#include <setworkspace.h>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QListWidgetItem>



LabelInfo globallabelinfo;
QString globallabel;
QString globalclsname;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->centralWidget->setMouseTracking(true);
    setMouseTracking(true);    

    ui->lineEdit_workspace->setReadOnly(true);
    ui->centralWidget->setFocus();

    ui->graphicsView->setScene(&scene);
    ui->graphicsView->setDisabled(true);
    ui->graphicsView->fitInView(scene.sceneRect());
    //ui->graphicsView->setAlignment(Qt::AlignCenter);


    //ui->listWidgetimg->setTabKeyNavigation(false);


    ui->actionSet_Workspace->setIcon(QIcon("icons/open.png"));
    ui->actionNew_Class->setIcon(QIcon("icons/open.png"));
    ui->actionImport_Images->setIcon(QIcon("icons/open.png"));
    ui->actionCamera->setIcon(QIcon("icons/camera.png"));
    ui->actionSave->setIcon(QIcon("icons/save.png"));
    ui->actionNext_Image->setIcon(QIcon("icons/next.png"));
    ui->actionPrev_Image->setIcon(QIcon("icons/prev.png"));
    ui->actionCreate_RectBox->setIcon(QIcon("icons/objects.png"));
    ui->actionDuplicate_RectBox->setIcon(QIcon("icons/copy.png"));
    ui->actionDelete_Rectbox->setIcon(QIcon("icons/cancel.png"));
    ui->actionDelete_Class->setIcon(QIcon("icons/cancel.png"));

    ui->actionNew_Class->setDisabled(true);
    ui->actionImport_Images->setDisabled(true);
    ui->actionNext_Image->setDisabled(true);
    ui->actionPrev_Image->setDisabled(true);
    ui->actionSave->setDisabled(true);
    ui->actionCamera->setDisabled(true);
    ui->actionCreate_RectBox->setDisabled(true);
    ui->actionDuplicate_RectBox->setDisabled(true);
    ui->actionDelete_Rectbox->setDisabled(true);
    ui->actionDelete_Class->setDisabled(true);


    target_rectbox = -1;
    mouse_press_rect_and_point.append(-1);






}

MainWindow::~MainWindow()
{
    delete ui;
}


void deleteDir(const QString &d)
{
    QDir dir(d);
    foreach (QFileInfo inf, dir.entryInfoList(QDir::Dirs|QDir::Files)) {
        if(inf.isFile())
            dir.remove(inf.absoluteFilePath());
        else if(inf.isDir() &&
                inf.fileName() != "." &&
                inf.fileName() != "..")
            deleteDir(inf.absolutePath()+"/"+inf.fileName());
    }
    dir.rmdir(dir.absolutePath());
}

void MainWindow::on_actionSet_Workspace_triggered()
{

    QString file1 = QFileDialog::getExistingDirectory(this, tr("Open workspace"),"/home/swimdi/swimfile/caffe_ssd_tool0808");

    ui->lineEdit_workspace->setText(file1);

    kinds_num = workspace_total_kind(file1);

    if (kinds_num == -1){
        QMessageBox::about(NULL, "Remind", "wrong workspace");
        return;
    }
    if (kinds_num == 0){
        QMessageBox::about(NULL, "Remind", "wrong workspace");
        return;
    }

    workspace_path = file1;

    KindOfPicsInfo* info_array = new KindOfPicsInfo;

    info_array = workspace_picture_information(file1, kinds_num);
    for (int i=0; i<kinds_num; i++)
    {
        ws_pic.append(&info_array[i]);
        /*for (int j=0; j<ws_pic[i]->pics.count(); j++)
        {
            if (ws_pic[i]->pics[j].picname == "vitamin_C_000103.jpg"){
                qDebug() << ws_pic[i]->pics[j].pboxs[0].label;
                qDebug() << ws_pic[i]->pics[j].pboxs[0].xmin;
                qDebug() << ws_pic[i]->pics[j].pboxs[0].ymin;
                qDebug() << ws_pic[i]->pics[j].pboxs[0].xmax;
                qDebug() << ws_pic[i]->pics[j].pboxs[0].ymax;

            }
        }*/
    }


    current_cls = 0;
    current_pic = 0;

    show_list_cls();
    show_list_img();





    QString picpath = ws_pic.at(current_cls)->path+"/JPEGImages/"+ws_pic.at(current_cls)->pics.at(current_pic).picname;
    set_scene_image(picpath);
    refresh_rect_box();
    refresh_scene();


    ui->actionNew_Class->setDisabled(false);
    ui->actionImport_Images->setDisabled(false);
    ui->actionNext_Image->setDisabled(false);
    ui->actionPrev_Image->setDisabled(false);
    ui->actionSave->setDisabled(false);
    ui->actionCamera->setDisabled(false);
    ui->actionCreate_RectBox->setDisabled(false);
    ui->actionDelete_Class->setDisabled(false);
    ui->actionSet_Workspace->setDisabled(true);




}

void MainWindow::on_actionNew_Class_triggered()
{
    globalclsname.clear();
    setworkspace setw;
    setw.setWindowTitle("new class");
    setw.setModal(true);
    setw.exec();

    if (globalclsname.isEmpty())
    {
        QMessageBox::about(NULL, "Remind", "please say a name");
        return;
    }
    for (int i=0; i<ws_pic.count(); i++)
    {
        if (ws_pic[i]->kindof==globalclsname)
        {
            QMessageBox::about(NULL, "Remind", "class name have been used");
            return;
        }

    }


    PicBoundingBox pbb;
    QList<PicBoundingBox> pbbs;
    pbbs.append(pbb);

    PicInfo picinfo;
    QList<PicInfo> picinfos;
    //picinfos.append(picinfo);

    KindOfPicsInfo *kpicinfo = new KindOfPicsInfo;
    kpicinfo->kindof = globalclsname;
    kpicinfo->path = workspace_path+"/Classes/"+globalclsname;
    kpicinfo->pics = picinfos;


    ws_pic.append(kpicinfo);
    //qDebug() << ws_pic.count();


    current_cls = ws_pic.count()-1;
    current_pic = -1;
    set_scene_image("no_pic");
    refresh_rect_box();
    refresh_scene();
    show_list_cls();
    show_list_img();

    /*ui->listWidgetcls->clear();
    ui->listWidgetimg->clear();

    for (int i=0; i<ws_pic.count(); i++)
    {
        ui->listWidgetcls->addItem(ws_pic.at(i)->kindof);
    }
    for (int j=0; j<ws_pic.at(current_cls)->pics.count(); j++)
    {
        ui->listWidgetimg->addItem(ws_pic.at(current_cls)->pics.at(j).picname);
    }

    ui->listWidgetcls->item(current_cls)->setSelected(true);
    ui->listWidgetimg->item(current_pic)->setSelected(true);*/

    QDir createcls;
    createcls.mkdir(workspace_path+"/Classes/"+globalclsname);
    createcls.mkdir(workspace_path+"/Classes/"+globalclsname+"/JPEGImages");
    createcls.mkdir(workspace_path+"/Classes/"+globalclsname+"/Annotations");




}

void MainWindow::on_actionDelete_Class_triggered()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, tr("Warning"), tr("Delete Class ?\n"),
                                                               QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    if (resBtn == QMessageBox::No){return;}


    deleteDir(ws_pic[current_cls]->path);

    ws_pic.removeAt(current_cls);
    current_cls = 0;
    current_pic = -1;

    show_list_cls();
    show_list_img();

}

void MainWindow::on_actionImport_Images_triggered()
{
    QString file1 = QFileDialog::getExistingDirectory(this, tr("Open workspace"),"/home/swimdi/Desktop/testimg");
    QDir dir(file1);
    QStringList items;
    dir.setFilter(QDir::Files|QDir::NoDotAndDotDot);
    items = dir.entryList();


    PicInfo clspic;
    int picnum = ws_pic[current_cls]->pics.count();
    QString picnumstr;

    for (int i=0; i<items.count(); i++)
    {
        if (items[i].endsWith(".jpg"))
        {
            picnum = picnum + 1;
            picnumstr = QString::number(picnum);
            for (int j=0; j<6; j++)
            {
                if (picnumstr.size()<6)
                {
                    picnumstr = "0"+picnumstr;
                }
            }

            clspic.picname = ws_pic[current_cls]->kindof+"_"+picnumstr+".jpg";
            clspic.xmlstatus = -1;


            QFile::copy(file1+"/"+items[i],workspace_path+"/Classes/"+ws_pic[current_cls]->kindof+"/JPEGImages/"+clspic.picname);


            ws_pic[current_cls]->pics.append(clspic);
        }
    }

    show_list_img();







}

void MainWindow::on_actionSave_triggered()
{
    save_img_xml();
    refresh_rect_box();
    refresh_scene();
}

void MainWindow::on_actionNext_Image_triggered()
{
    save_img_xml();
    if (current_pic < ui->listWidgetimg->count()-1)
    {
        current_pic = current_pic + 1;
    }
    if (current_pic>-1 && current_pic<ws_pic.at(current_cls)->pics.count())
    {
        QString picpath = ws_pic.at(current_cls)->path+"/JPEGImages/"+ws_pic.at(current_cls)->pics.at(current_pic).picname;
        ui->listWidgetimg->setCurrentRow(current_pic);
        set_scene_image(picpath);
        refresh_rect_box();
        refresh_scene();
        show_list_img();
        target_rectbox = -1;
    }
    else
    {
        set_scene_image("no_pic");
        refresh_rect_box();
        refresh_scene();
    }
}

void MainWindow::on_actionPrev_Image_triggered()
{
    save_img_xml();
    if (current_pic > 0)
    {
        current_pic = current_pic - 1;
    }
    if (current_pic>-1 && current_pic<ws_pic.at(current_cls)->pics.count())
    {
        QString picpath = ws_pic.at(current_cls)->path+"/JPEGImages/"+ws_pic.at(current_cls)->pics.at(current_pic).picname;
        ui->listWidgetimg->setCurrentRow(current_pic);
        set_scene_image(picpath);
        refresh_rect_box();
        refresh_scene();
        show_list_img();
        target_rectbox = -1;
    }
    else
    {
        set_scene_image("no_pic");
        refresh_rect_box();
        refresh_scene();
    }
}

void MainWindow::on_actionCreate_RectBox_triggered()
{
    if (pimg.isNull() == true){
        QMessageBox::about(NULL, "Remind", "please select an image");
        return;
    }

    if (create_rectbox_check == false)
    {
        create_rectbox_check = true;
        ui->actionCreate_RectBox->setDisabled(true);
    }
    else
    {
        create_rectbox_check = false;
        ui->actionCreate_RectBox->setDisabled(false);
    }

}

void MainWindow::on_actionDuplicate_RectBox_triggered()
{
    if (target_rectbox != -1)
    {
        rectbox.height.append(rectbox.height[target_rectbox]);
        rectbox.width.append(rectbox.width[target_rectbox]);
        rectbox.xmin.append(rectbox.xmin[target_rectbox]+10);
        rectbox.ymin.append(rectbox.ymin[target_rectbox]+10);
        rectbox.label.append(rectbox.label[target_rectbox]);

        refresh_scene();

    }
}

void MainWindow::on_actionDelete_Rectbox_triggered()
{
    if (target_rectbox != -1)
    {
        rectbox.height.removeAt(target_rectbox);
        rectbox.width.removeAt(target_rectbox);
        rectbox.xmin.removeAt(target_rectbox);
        rectbox.ymin.removeAt(target_rectbox);
        rectbox.label.removeAt(target_rectbox);
        target_rectbox = -1;
        refresh_scene();

    }
}

void MainWindow::on_listWidgetimg_itemDoubleClicked(QListWidgetItem *item)
{
    save_img_xml();
    current_pic = ui->listWidgetimg->currentRow();
    QString picpath = ws_pic.at(current_cls)->path+"/JPEGImages/"+ws_pic.at(current_cls)->pics.at(current_pic).picname;
    set_scene_image(picpath);
    refresh_rect_box();
    refresh_scene();

}

void MainWindow::on_listWidgetcls_itemDoubleClicked(QListWidgetItem *item)
{
    save_img_xml();
    current_cls = ui->listWidgetcls->currentRow();
    ui->listWidgetimg->clear();
    current_pic = -1;
    set_scene_image("no_pic");
    refresh_rect_box();
    refresh_scene();
    show_list_img();

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, tr("Warning"), tr("Close the window?\n"),
                                                               QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes){event->accept();}
    else{event->ignore();}
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_W)
    {
        if (pimg.isNull() == true){
            QMessageBox::about(NULL, "Remind", "please select an image");
            return;
        }

        if (create_rectbox_check == false)
        {
            create_rectbox_check = true;
        }
        else
        {
            create_rectbox_check = false;
            ui->actionCreate_RectBox->setDisabled(false);
        }
    }



    if (event->key()==Qt::Key_Delete)
    {
        if (target_rectbox != -1)
        {
            rectbox.height.removeAt(target_rectbox);
            rectbox.width.removeAt(target_rectbox);
            rectbox.xmin.removeAt(target_rectbox);
            rectbox.ymin.removeAt(target_rectbox);
            rectbox.label.removeAt(target_rectbox);
            target_rectbox = -1;

            refresh_scene();

        }
    }

    if (event->key()==Qt::Key_D)
    {
        save_img_xml();
        if (current_pic < ui->listWidgetimg->count()-1)
        {
            current_pic = current_pic + 1;
        }
        if (current_pic>-1 && current_pic<ws_pic.at(current_cls)->pics.count())
        {
            QString picpath = ws_pic.at(current_cls)->path+"/JPEGImages/"+ws_pic.at(current_cls)->pics.at(current_pic).picname;
            ui->listWidgetimg->setCurrentRow(current_pic);
            set_scene_image(picpath);
            refresh_rect_box();
            refresh_scene();
            show_list_img();
            target_rectbox = -1;
        }
        else
        {
            set_scene_image("no_pic");
            refresh_rect_box();
            refresh_scene();
        }
    }

    if (event->key()==Qt::Key_A)
    {
        save_img_xml();
        if (current_pic > 0)
        {
            current_pic = current_pic - 1;
        }
        if (current_pic>-1 && current_pic<ws_pic.at(current_cls)->pics.count())
        {
            QString picpath = ws_pic.at(current_cls)->path+"/JPEGImages/"+ws_pic.at(current_cls)->pics.at(current_pic).picname;
            ui->listWidgetimg->setCurrentRow(current_pic);
            set_scene_image(picpath);
            refresh_rect_box();
            refresh_scene();
            show_list_img();
            target_rectbox = -1;
        }
        else
        {
            set_scene_image("no_pic");
            refresh_rect_box();
            refresh_scene();
        }
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    press_x = event->pos().x();
    press_y = event->pos().y();

    scene.clear();
    scene.addPixmap(pimg);
    for (int i=0; i<rectbox.label.size(); i++){
        scene.addRect(rectbox.xmin[i], rectbox.ymin[i], rectbox.width[i], rectbox.height[i]);
        scene_add_4_point(rectbox.xmin[i], rectbox.ymin[i], rectbox.width[i], rectbox.height[i]);
    }

    mouse_press_check = true;
    target_rectbox = -1;
    mouse_touch_some_point_when_press();
    mouse_touch_some_rect_when_press();
    if (target_rectbox != -1)
    {
        target_x = rectbox.xmin[target_rectbox];
        target_y = rectbox.ymin[target_rectbox];
    }
    show_target_rectbox();

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move_x = event->pos().x();
    move_y = event->pos().y();
    refresh_scene();

    show_target_rectbox();

    if (is_in_scene(move_x, move_y))
    {
        if (create_rectbox_check == false && mouse_press_check == false)
        {
            show_mouse_in_some_rectbox();
            show_mouse_touch_some_point();
        }

        if (create_rectbox_check == false && mouse_press_check == true)
        {
            mouse_point_edit_rect();
            mouse_move_rect();
            show_mouse_touch_some_point();

        }

    }


    int xx, yy, ww, hh;
    xx = std::min(press_x,move_x)-pic_left;
    yy = std::min(press_y,move_y)-pic_up;
    ww = std::abs(press_x-move_x);
    hh = std::abs(press_y-move_y);

    if (is_in_scene(move_x, move_y) && create_rectbox_check == true && mouse_press_check == false)
    {
        scene.addLine(move_x-pic_left, 0, move_x-pic_left, image.height());
        scene.addLine(0 , move_y-pic_up, image.width(), move_y-pic_up);
    }



    if (is_in_scene(press_x, press_y) && is_in_scene(move_x, move_y))
    {
        if (create_rectbox_check == true && mouse_press_check == true)
        {
            scene.addRect(xx,yy,ww,hh);
        }

    }



    sort_rectbox();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){

    release_x = event->pos().x();
    release_y = event->pos().y();

    if (is_in_scene(press_x,press_y) && is_in_scene(release_x, release_y) && create_rectbox_check == true
            && std::abs(press_x-release_x)>5 && std::abs(press_y-release_y)>5)
    {
        if (ui->checkBox->isChecked()==false){
            DialogLabel dilabel;
            dilabel.setGeometry(release_x+60.5,release_y+60.5,250,300);
            dilabel.setWindowTitle("Select Label");
            dilabel.setModal(true);
            dilabel.exec();
        }

        if (globallabel.isEmpty() == false)
        {
            bool check = false;
            int n=0;

            for (int i=0; i<globallabelinfo.label.size(); i++)
            {

                if (globallabelinfo.label[i] == globallabel)
                {
                    check = true;
                    n = i;
                }
            }
            if (check == false)
            {
                globallabelinfo.label.append(globallabel);
                globallabelinfo.r.append(std::rand() % 255);
                globallabelinfo.g.append(std::rand() % 255);
                globallabelinfo.b.append(std::rand() % 255);
                n=globallabelinfo.label.size()-1;
            }
            rectbox.xmin.append(std::min(press_x,release_x)-pic_left);
            rectbox.ymin.append(std::min(press_y,release_y)-pic_up);
            rectbox.width.append(std::abs(press_x-release_x));
            rectbox.height.append(std::abs(press_y-release_y));
            rectbox.label.append(globallabel);

            scene_add_4_point(std::min(press_x,release_x)-pic_left, std::min(press_y,release_y)-pic_up,
                              std::abs(press_x-release_x), std::abs(press_y-release_y));

        }
    }
    create_rectbox_check = false;
    ui->actionCreate_RectBox->setDisabled(false);
    mouse_press_check = false;

    ui->centralWidget->setFocus();

    if (target_rectbox == -1)
    {
        ui->actionDuplicate_RectBox->setDisabled(true);
        ui->actionDelete_Rectbox->setDisabled(true);
        ui->label_5->setText("");
    }
    else
    {
        ui->actionDuplicate_RectBox->setDisabled(false);
        ui->actionDelete_Rectbox->setDisabled(false);
        ui->label_5->setText(rectbox.label[target_rectbox]);
    }

}



bool MainWindow::is_in_scene(int x, int y)
{
    return x>pic_left && x<pic_right && y>pic_up && y<pic_down;
}

void MainWindow::set_scene_image(QString file1)
{
    if (file1.isEmpty()){return;}
    image = QImage(file1);
    current_pic_h = image.height();
    current_pic_w = image.width();
    pic_resize_ratio = 1;
    if (ui->graphicsView->width()*image.height() < image.width()*ui->graphicsView->height()){
        double a, b;
        a = image.width();
        b = ui->graphicsView->width();
        pic_resize_ratio = a/b;
        image = image.scaledToWidth(ui->graphicsView->width()-10);
    }
    else
    {
      double a, b;
      a = image.height();
      b = ui->graphicsView->height();
      pic_resize_ratio = a/b;
      image = image.scaledToHeight(ui->graphicsView->height()-10);
    }




    pimg = QPixmap(QPixmap::fromImage(image));
    scene.clear();
    scene.addPixmap(pimg);


    create_rectbox_check = false;
    ui->actionCreate_RectBox->setDisabled(false);
    int gx, gy, gw, gh, sw, sh;
    gx = ui->graphicsView->pos().x();
    gy = ui->graphicsView->pos().y();
    gw = ui->graphicsView->width();
    gh = ui->graphicsView->height();
    sw = ui->graphicsView->scene()->width();
    sh = ui->graphicsView->scene()->height();

    pic_left = gx+(gw-sw)/2;
    pic_right = gx+gw-(gw-sw)/2;
    pic_up = gy+(gh-sh)/2 + 70.5;
    pic_down = gy+gh-(gh-sh)/2 + 70.5;

}

void MainWindow::scene_add_4_point(int x, int y, int w, int h)
{
    scene.addEllipse(x-5, y-5, 10, 10, QPen(), QBrush(Qt::green));
    scene.addEllipse(x+w-5, y-5, 10, 10, QPen(), QBrush(Qt::green));
    scene.addEllipse(x-5, y+h-5, 10, 10, QPen(), QBrush(Qt::green));
    scene.addEllipse(x+w-5, y+h-5, 10, 10, QPen(), QBrush(Qt::green));
}

void MainWindow::mouse_touch_some_point_when_press()
{
    mouse_press_rect_and_point.clear();

    int x = press_x-pic_left;
    int y = press_y-pic_up;
    int xm, ym, w, h;

    for (int i=rectbox.label.size()-1; i>-1; i--){

        xm = rectbox.xmin[i];
        ym = rectbox.ymin[i];
        w = rectbox.width[i];
        h = rectbox.height[i];

        if (x>xm-8 && x<xm+8 && y>ym-8 && y<ym+8)
        {
            mouse_press_rect_and_point.append(i);
            mouse_press_rect_and_point.append(0);
            target_rectbox = i;
            return;
        }

        if (x>xm+w-8 && x<xm+w+8 && y>ym-8 && y<ym+8)
        {
            mouse_press_rect_and_point.append(i);
            mouse_press_rect_and_point.append(1);
            target_rectbox = i;
            return;
        }

        if (x>xm-8 && x<xm+8 && y>ym+h-8 && y<ym+h+8)
        {
            mouse_press_rect_and_point.append(i);
            mouse_press_rect_and_point.append(2);
            target_rectbox = i;
            return;
        }

        if (x>xm+w-8 && x<xm+w+8 && y>ym+h-8 && y<ym+h+8)
        {
            mouse_press_rect_and_point.append(i);
            mouse_press_rect_and_point.append(3);
            target_rectbox = i;
            return;
        }

    }
    mouse_press_rect_and_point.append(-1);
}

void MainWindow::mouse_touch_some_rect_when_press()
{
    bool check = false;

    int x = move_x-pic_left;
    int y = move_y-pic_up;
    int xm, ym, w, h;

    for (int i=rectbox.label.size()-1; i>-1; i--){
        if (check == true){return;}

        xm = rectbox.xmin[i];
        ym = rectbox.ymin[i];
        w = rectbox.width[i];
        h = rectbox.height[i];

        if (x>xm && x<xm+w && y>ym && y<ym+h)
        {
            for (int j=0; j<globallabelinfo.label.size(); j++)
            {
                if (globallabelinfo.label[j] == rectbox.label[i])
                {
                    scene.addRect(xm, ym, w, h, QPen(),QBrush(QColor(0, 50, 200, 70)));
                }
            }
            target_rectbox = i;
            check = true;
        }
    }


}

void MainWindow::show_mouse_touch_some_point()
{
    bool check = false;

    int x = move_x-pic_left;
    int y = move_y-pic_up;
    int xm, ym, w, h;

    for (int i=rectbox.label.size()-1; i>-1; i--){
        if (check == true){return;}
        xm = rectbox.xmin[i];
        ym = rectbox.ymin[i];
        w = rectbox.width[i];
        h = rectbox.height[i];

        if (x>xm-8 && x<xm+8 && y>ym-8 && y<ym+8)
        {
            scene.addRect(xm-8, ym-8, 16, 16, QPen(), QBrush(Qt::red));
            scene.addEllipse(xm+w-5, ym-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm-5, ym+h-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm+w-5, ym+h-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            check = true;
        }

        if (x>xm+w-8 && x<xm+w+8 && y>ym-8 && y<ym+8)
        {
            scene.addRect(xm+w-8, ym-8, 16, 16, QPen(), QBrush(Qt::red));
            scene.addEllipse(xm-5, ym-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm-5, ym+h-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm+w-5, ym+h-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            check = true;
        }

        if (x>xm-8 && x<xm+8 && y>ym+h-8 && y<ym+h+8)
        {
            scene.addRect(xm-8, ym+h-8, 16, 16, QPen(), QBrush(Qt::red));
            scene.addEllipse(xm+w-5, ym-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm-5, ym-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm+w-5, ym+h-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            check = true;
        }

        if (x>xm+w-8 && x<xm+w+8 && y>ym+h-8 && y<ym+h+8)
        {
            scene.addRect(xm+w-8, ym+h-8, 16, 16, QPen(), QBrush(Qt::red));
            scene.addEllipse(xm+w-5, ym-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm-5, ym+h-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            scene.addEllipse(xm-5, ym-5, 10 ,10 ,QPen(), QBrush(Qt::red));
            check = true;
        }

    }


}

void MainWindow::show_mouse_in_some_rectbox()
{
    bool check = false;

    int x = move_x-pic_left;
    int y = move_y-pic_up;
    int xm, ym, w, h;

    for (int i=rectbox.label.size()-1; i>-1; i--){
        if (check == true){return;}
        xm = rectbox.xmin[i];
        ym = rectbox.ymin[i];
        w = rectbox.width[i];
        h = rectbox.height[i];

        if (x>xm && x<xm+w && y>ym && y<ym+h && target_rectbox != i)
        {
            for (int j=0; j<globallabelinfo.label.size(); j++)
            {
                if (globallabelinfo.label[j] == rectbox.label[i])
                {
                    scene.addRect(xm, ym, w, h,
                                  QPen(),QBrush(QColor(globallabelinfo.r[j],
                                                       globallabelinfo.g[j],
                                                       globallabelinfo.b[j], 120)));
                }
            }
            check = true;
        }
    }

}

void MainWindow::show_target_rectbox()
{
    int xm, ym, w, h;
    if (target_rectbox != -1)
    {
        xm = rectbox.xmin[target_rectbox];
        ym = rectbox.ymin[target_rectbox];
        w = rectbox.width[target_rectbox];
        h = rectbox.height[target_rectbox];
            for (int j=0; j<globallabelinfo.label.size(); j++)
            {
                if (globallabelinfo.label[j] == rectbox.label[target_rectbox])
                {
                    scene.addRect(xm, ym, w, h, QPen(Qt::NoBrush),QBrush(QColor(0, 50, 200, 70)));
                }
            }
    }
}

void MainWindow::mouse_point_edit_rect()
{
    if (mouse_press_rect_and_point[0]==-1){return;}
    int x = move_x-pic_left;
    int y = move_y-pic_up;
    int xm, ym, w, h;

    int which_rect = mouse_press_rect_and_point[0];
    int which_pos = mouse_press_rect_and_point[1];

    xm = rectbox.xmin[which_rect];
    ym = rectbox.ymin[which_rect];
    w = rectbox.width[which_rect];
    h = rectbox.height[which_rect];

    if (which_pos == 0)
    {
        rectbox.xmin[which_rect] = x;
        rectbox.ymin[which_rect] = y;
        rectbox.width[which_rect] = w+(xm-x);
        rectbox.height[which_rect] = h+(ym-y);
    }

    if (which_pos == 1)
    {
        rectbox.ymin[which_rect] = y;
        rectbox.width[which_rect] = x-xm;
        rectbox.height[which_rect] = h+(ym-y);
    }

    if (which_pos == 2)
    {
        rectbox.xmin[which_rect] = x;
        rectbox.width[which_rect] = w+(xm-x);
        rectbox.height[which_rect] = y-ym;
    }

    if (which_pos == 3)
    {
        rectbox.width[which_rect] = x-xm;
        rectbox.height[which_rect] = y-ym;
    }

}

void MainWindow::mouse_move_rect()
{
    if (mouse_press_rect_and_point[0] == -1 && target_rectbox != -1)
    {
        if (target_x+move_x-press_x> 0 && target_x+move_x-press_x+rectbox.width[target_rectbox] < ui->graphicsView->width()
                && target_y+move_y-press_y> 0 && target_y+move_y-press_y+rectbox.height[target_rectbox] < ui->graphicsView->height())
        {
            rectbox.xmin[target_rectbox] = target_x + (move_x-press_x);
            rectbox.ymin[target_rectbox] = target_y + (move_y-press_y);
        }

    }
}

void MainWindow::sort_rectbox()
{
    for (int i=0; i<rectbox.label.size(); i++)
    {
        if (rectbox.width[i]<0)
        {
            rectbox.xmin[i] = rectbox.xmin[i]+rectbox.width[i];
            rectbox.width[i] = -rectbox.width[i];
            if (mouse_press_rect_and_point[1] == 0) {mouse_press_rect_and_point[1] = 1;}
            else if (mouse_press_rect_and_point[1] == 1) {mouse_press_rect_and_point[1] = 0;}
            else if (mouse_press_rect_and_point[1] == 2) {mouse_press_rect_and_point[1] = 3;}
            else if (mouse_press_rect_and_point[1] == 3) {mouse_press_rect_and_point[1] = 2;}
        }

        if (rectbox.height[i]<0)
        {
            rectbox.ymin[i] = rectbox.ymin[i]+rectbox.height[i];
            rectbox.height[i] = -rectbox.height[i];
            if (mouse_press_rect_and_point[1] == 0) {mouse_press_rect_and_point[1] = 2;}
            else if (mouse_press_rect_and_point[1] == 1) {mouse_press_rect_and_point[1] = 3;}
            else if (mouse_press_rect_and_point[1] == 2) {mouse_press_rect_and_point[1] = 0;}
            else if (mouse_press_rect_and_point[1] == 3) {mouse_press_rect_and_point[1] = 1;}
        }

    }

}

void MainWindow::refresh_scene()
{
    scene.clear();
    if (pimg.isNull() == true){return;}
    scene.addPixmap(pimg);
    for (int i=0; i<rectbox.label.size(); i++){
        scene.addRect(rectbox.xmin[i], rectbox.ymin[i], rectbox.width[i], rectbox.height[i]);
        if (mouse_press_rect_and_point[0] != i || mouse_press_check == false)
        {
            scene_add_4_point(rectbox.xmin[i], rectbox.ymin[i], rectbox.width[i], rectbox.height[i]);
        }

    }
}

void MainWindow::refresh_rect_box()
{
    rectbox.clear();
    if (current_cls > ws_pic.count() || current_cls < 0){return;}
    if (current_pic > ws_pic[current_cls]->pics.count() || current_pic < 0){return;}


    int xmin, ymin, width, height;
    bool check;
    for (int i = 0; i<ws_pic.at(current_cls)->pics.at(current_pic).pboxs.size(); i++)
    {
        xmin = ws_pic.at(current_cls)->pics.at(current_pic).pboxs[i].xmin/pic_resize_ratio;
        ymin = ws_pic.at(current_cls)->pics.at(current_pic).pboxs[i].ymin/pic_resize_ratio;
        width = ws_pic.at(current_cls)->pics.at(current_pic).pboxs[i].xmax/pic_resize_ratio - xmin;
        height = ws_pic.at(current_cls)->pics.at(current_pic).pboxs[i].ymax/pic_resize_ratio - ymin;
        check = false;

        for (int j=0; j<globallabelinfo.label.size(); j++)
        {

            if (globallabelinfo.label[j] == ws_pic.at(current_cls)->pics.at(current_pic).pboxs[i].label)
            {
                check = true;
            }
        }
        if (check == false)
        {
            globallabelinfo.label.append(ws_pic.at(current_cls)->pics.at(current_pic).pboxs[i].label);
            globallabelinfo.r.append(std::rand() % 255);
            globallabelinfo.g.append(std::rand() % 255);
            globallabelinfo.b.append(std::rand() % 255);
        }

        rectbox.label.append(ws_pic.at(current_cls)->pics.at(current_pic).pboxs[i].label);
        rectbox.xmin.append(xmin);
        rectbox.ymin.append(ymin);
        rectbox.width.append(width);
        rectbox.height.append(height);

    }


}


void MainWindow::save_img_xml()
{
    if (current_cls < 0 || current_pic < 0){return;}
    if (pimg.isNull() == true){return;}
    if (rectbox.label.isEmpty()){return;}
    QString fname;
    fname = ws_pic.at(current_cls)->path+"/Annotations/"+ws_pic.at(current_cls)->pics.at(current_pic).picname;
    fname.remove(".jpg");
    fname.append(".xml");

    QString strFile(fname);
    QFile file(strFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << QString("Cannot write file %1(%2).").arg(strFile).arg(file.errorString());
        return;
    }

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);


    writer.writeStartElement("annotation");
    writer.writeTextElement(QString::fromLocal8Bit("folder"), QString::fromLocal8Bit("JPEGImages"));
    writer.writeTextElement(QString::fromLocal8Bit("path"), ws_pic.at(current_cls)->path+"/JPEGImages/"+ws_pic.at(current_cls)->pics.at(current_pic).picname);
    writer.writeStartElement("size");
    writer.writeTextElement(QString::fromLocal8Bit("width"), QString::number(current_pic_w));
    writer.writeTextElement(QString::fromLocal8Bit("height"), QString::number(current_pic_h));
    writer.writeTextElement(QString::fromLocal8Bit("depth"), "3");
    writer.writeEndElement();


    int xmin, ymin, xmax, ymax;

    for (int i=0; i<rectbox.label.size(); i++)
    {
        xmin = rectbox.xmin[i]*pic_resize_ratio;
        ymin = rectbox.ymin[i]*pic_resize_ratio;
        xmax = (rectbox.xmin[i]+rectbox.width[i])*pic_resize_ratio;
        ymax = (rectbox.ymin[i]+rectbox.height[i])*pic_resize_ratio;
        writer.writeStartElement("object");
        writer.writeTextElement(QString::fromLocal8Bit("name"), rectbox.label[i]);
        writer.writeStartElement("bndbox");
        writer.writeTextElement(QString::fromLocal8Bit("xmin"), QString::number(xmin));
        writer.writeTextElement(QString::fromLocal8Bit("ymin"), QString::number(ymin));
        writer.writeTextElement(QString::fromLocal8Bit("xmax"), QString::number(xmax));
        writer.writeTextElement(QString::fromLocal8Bit("ymax"), QString::number(ymax));
        writer.writeEndElement();
        writer.writeEndElement();
    }

    writer.writeEndDocument();

    file.close();

    PicInfo picinfo;
    PicBoundingBox pbb;

    if (rectbox.label.size()>0)
    {
        picinfo.picname = ws_pic.at(current_cls)->pics.at(current_pic).picname;
        picinfo.xmlstatus = 1;

        for (int i=0; i<rectbox.label.size(); i++)
        {
            pbb.label = rectbox.label[i];
            pbb.xmin = rectbox.xmin[i]*pic_resize_ratio;
            pbb.ymin = rectbox.ymin[i]*pic_resize_ratio;
            pbb.xmax = (rectbox.xmin[i] + rectbox.width[i])*pic_resize_ratio;
            pbb.ymax = (rectbox.ymin[i] + rectbox.height[i])*pic_resize_ratio;
            picinfo.pboxs.append(pbb);
        }

        ws_pic.at(current_cls)->pics.replace(current_pic,picinfo);

    }



    rectbox.clear();

    refresh_scene();

}

void MainWindow::show_list_cls()
{
    ui->listWidgetcls->clear();

    for (int i=0; i<ws_pic.count(); i++)
    {
        ui->listWidgetcls->addItem(ws_pic.at(i)->kindof);
    }

    if (current_cls<ui->listWidgetcls->count() && current_cls>-1)
    {
        ui->listWidgetcls->item(current_cls)->setSelected(true);
    }


}

void MainWindow::show_list_img()
{
    ui->listWidgetimg->clear();

    for (int i=0; i<ws_pic[current_cls]->pics.count(); i++)
    {
        ui->listWidgetimg->addItem(ws_pic[current_cls]->pics[i].picname);

        if (ws_pic[current_cls]->pics[i].xmlstatus == -1)
        {
            ui->listWidgetimg->item(i)->setBackgroundColor(Qt::green);
        }
    }

    if (current_pic<ui->listWidgetimg->count() && current_pic>-1)
    {
        ui->listWidgetimg->item(current_pic)->setSelected(true);
    }

}
