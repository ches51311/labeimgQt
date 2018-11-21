#include "function.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <iostream>
#include <QDir>
#include <QDebug>
#include <QXmlStreamReader>

int workspace_total_kind(QString path1)
{
    int num = 0;

    QDir dir(path1);
    if (dir.cd("Classes") == false){return -1;}
    dir.setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
    QStringList items = dir.entryList();
    QString item1;
    QString item2;
    for (int i=0; i<items.count(); i++)
    {
        item1 = path1+"/Classes/"+items.at(i)+"/JPEGImages";
        item2 = path1+"/Classes/"+items.at(i)+"/Annotations";
        if (QDir(item1).exists() && QDir(item2).exists()){num = num+1;}
    }
    return num;

}


KindOfPicsInfo* workspace_picture_information(QString path1, int kind_nums)
{
    KindOfPicsInfo *info_array = new KindOfPicsInfo[kind_nums];

    QDir dir(path1);
    QDir dirjpg;
    QDir dirxml;
    QStringList items;
    QString item1;
    QString item2;

    dir.cd("Classes");
    dir.setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
    items = dir.entryList();

    for (int i=0; i<items.count(); i++)
    {
        info_array[i].kindof = items.at(i);
        info_array[i].path = path1+"/Classes/"+items.at(i);
        info_array[i].pics = each_kind_pic_info(path1+"/Classes/"+items.at(i));
    }


    return info_array;
}


QList<PicInfo> each_kind_pic_info(QString path1)
{
    QList<PicInfo> picinfolist;
    PicInfo picinfo;
    QDir dir1(path1+"/JPEGImages");
    QStringList items;
    QString itemxml;
    bool is_pic;

    //dir.setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
    dir1.setFilter(QDir::Files);
    items = dir1.entryList();

    for (int i=0; i<items.count(); i++)
    {
        itemxml = items[i];
        is_pic = false;
        if (itemxml.endsWith(".jpg"))
        {
            itemxml.remove(".jpg");
            itemxml.append(".xml");
            is_pic = true;
        }

        if (itemxml.endsWith(".png"))
        {
            itemxml.remove(".png");
            itemxml.append(".xml");
            is_pic = true;
        }

        if (itemxml.endsWith(".jpeg"))
        {
            itemxml.remove(".jpeg");
            itemxml.append(".xml");
            is_pic = true;
        }



        if (is_pic == true)
        {
            picinfo.picname = items[i];
            if (QFile::exists(path1+"/Annotations/"+itemxml))
            {
                picinfo.xmlstatus = 1;
                picinfo.pboxs = each_pic_xml(path1+"/Annotations/"+itemxml);
            }
            else
            {
                picinfo.xmlstatus = -1;
            }
            picinfolist.append(picinfo);
        }

    }


    return picinfolist;

}


QList<PicBoundingBox> each_pic_xml(QString file1)
{
       QList<PicBoundingBox> pbblist;
       PicBoundingBox pbb;

       QFile file2(file1);
       file2.open(QIODevice::ReadOnly | QIODevice::Text);
       QXmlStreamReader reader(&file2);
       QString read1;
       QString labname;
       int xmin, xmax, ymin, ymax;
       while (!reader.atEnd())
       {
           read1 = reader.name().toString();
           if (read1 == "name")
           {
               reader.readNext();
               labname = reader.text().toString();
               reader.readNext();
           }
           if (read1 == "xmin")
           {
               reader.readNext();
               xmin = reader.text().toInt();
               reader.readNext();
               reader.readNext();
               reader.readNext();
               reader.readNext();
               ymin = reader.text().toInt();
               reader.readNext();
               reader.readNext();
               reader.readNext();
               reader.readNext();
               xmax = reader.text().toInt();
               reader.readNext();
               reader.readNext();
               reader.readNext();
               reader.readNext();
               ymax = reader.text().toInt();
               pbb.label = labname;
               pbb.xmin = xmin;
               pbb.ymin = ymin;
               pbb.xmax = xmax;
               pbb.ymax = ymax;
               pbblist.append(pbb);
           }
           reader.readNext();
     }





       /*for (int i=0; i<6; i++)
       {
           pbb.label = "cola";
           *pbb.xmin = 30;
           *pbb.xmax = 30;
           *pbb.ymin = 30;
           *pbb.ymax = 30;
           pbblist.append(pbb);
       }*/


       return pbblist;
}

