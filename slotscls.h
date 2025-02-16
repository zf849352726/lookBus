//
//  slotscls.h
//  seeBus
//
//  Created by zf on 2025/2/12.
//

#pragma once
#include <QObject>
#include <vector>
#include <QDebug>
#include "webscraper.h"

class MainWindow; // 前向声明
class HtmlParser;
class WebScraper;

class SlotsCls : public QObject {
    Q_OBJECT
public:
    explicit SlotsCls(QObject *parent = nullptr);
    ~SlotsCls();
    void setMainWindow(MainWindow *mainWindow); // 设置MainWindow指针
    
    void listViewShow();
public slots:
    void on_latest_clicked();
    
private:
    WebScraper *scraper;
    MainWindow *m_mainWindow; // 保存MainWindow指针
};

