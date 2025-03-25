//
//  slotscls.cpp
//  seeBus
//
//  Created by zf on 2025/2/12.
//

#include "slotscls.h"

SlotsCls:: SlotsCls(QObject *parent) : QObject(parent), scraper(new WebScraper(this)) {}

SlotsCls:: ~SlotsCls() {}

//void SlotsCls:: setMainWindow(MainWindow *mainWindow) {
//    m_mainWindow = mainWindow;
//}

void SlotsCls::on_hottest_clicked() {
    this->scraper->fetchData(QUrl("https://javbus.com"), [this](const QByteArray &data) {
        this->scraper->parseDefault(data);
    });
    qDebug() << "Button Clicked!";
    emit slotFinished();
}


