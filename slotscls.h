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
    
public slots:
    void on_hottest_clicked();
    
signals:
    void slotFinished();  // 定义一个信号，当槽函数执行完毕时发出
    
private:
    WebScraper *scraper;
};

