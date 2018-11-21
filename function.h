#ifndef FUNCTION_H
#define FUNCTION_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <iostream>



class PicBoundingBox{
public:

    QString label;
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

class PicInfo
{

public:
    QString picname;
    int xmlstatus;
    QList<PicBoundingBox> pboxs;

};

//one class with one KindOfPicsInfo, ex:KindOfPicsInfo medicine[8];
class KindOfPicsInfo
{

public:

    QString kindof;
    QString path;
    QList<PicInfo> pics;

};


int workspace_total_kind(QString path1);

KindOfPicsInfo* workspace_picture_information(QString path1, int kind_nums);

QList<PicInfo> each_kind_pic_info(QString path1);

QList<PicBoundingBox> each_pic_xml(QString file1);

#endif // FUNCTION_H
