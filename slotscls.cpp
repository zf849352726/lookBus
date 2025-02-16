//
//  slotscls.cpp
//  seeBus
//
//  Created by zf on 2025/2/12.
//

#include "slotscls.h"

SlotsCls:: SlotsCls(QObject *parent) : QObject(parent), scraper(new WebScraper(this)),  m_mainWindow(nullptr) {}

SlotsCls:: ~SlotsCls() {}

void SlotsCls:: setMainWindow(MainWindow *mainWindow) {
    m_mainWindow = mainWindow;
}

void SlotsCls::on_latest_clicked() {
    if (m_mainWindow) {
        this->scraper->fetchData(QUrl("https://javbus.com"), [this](const QByteArray &data) {
            this->scraper->parseDefault(data);
            listViewShow();
        });
    }
    qDebug() << "Button Clicked!";
}

void SlotsCls:: listViewShow(){
//    m_mainWindow->getUiObj()->listView->setModel(this->scraper->getPipeObj()->getModelObj());
}
