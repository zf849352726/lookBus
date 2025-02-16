//
//  pipecls.cpp
//  seeBus
//
//  Created by zf on 2025/2/14.
//
#include "pipecls.h"

PipeCls::PipeCls(QObject *parent)
    : QObject(parent),
      themeList(new QList<QString>()),
      model(new QStringListModel(*themeList, this)) {
}

PipeCls:: ~PipeCls(){};

QList<QString> * PipeCls::geThemeListPointer(){
    return this->themeList;
}

QStringListModel *PipeCls:: getModelObj(){
    return this->model;
}
