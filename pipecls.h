//
//  pipecls.h
//  seeBus
//
//  Created by zf on 2025/2/14.
//
#pragma once
#include <QObject>
#include <QStringListModel>

class PipeCls: public QObject{
    Q_OBJECT
public:
    PipeCls(QObject *parent);
    ~PipeCls();
    
    QList<QString> * geThemeListPointer();
    
    QStringListModel *getModelObj();
private:
    //  主题内容
    QList<QString> *themeList;
    QStringListModel *model;
};
