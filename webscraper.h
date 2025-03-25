//
//  webscraper.h
//  seeBus
//
//  Created by zf on 2025/1/10.
//

#ifndef WEBSCRAPER_H
#define WEBSCRAPER_H

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <stdexcept>
#include <QEventLoop>
#include <functional>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/HTMLparser.h>
#include "htmlparse.h"
#include "pipecls.h"
#include "itemmodel.h"

class HtmlParser;
class ItemModel;

class WebScraper : public QObject
{
    Q_OBJECT
    
public:
//    WebScraper();
    explicit WebScraper(QObject *parent = nullptr);
    
//    explicit WebScraper(QSharedPointer<ItemModel> model);
    ItemModel *getModel();

    // 发起请求，传递解析函数
    void fetchData(const QUrl &url, std::function<void(const QByteArray&)> parseFunction = nullptr);
    
    void parseDefault(const QByteArray &data);
    
    void parseForum(const QByteArray &data);
    
    void parseForumJp(const QByteArray &data);
    
    void parseForumDetail(const QByteArray &data);

    PipeCls *getPipeObj();
signals:
    void noParseFunctionProvided();  // 添加这个信号

private:
    QNetworkAccessManager *manager;
    HtmlParser *htmlparse;
    QUrl nextUrl;
    PipeCls *pipe;
    ItemModel *hotModel;
    
};

#endif // WEBSCRAPER_H
