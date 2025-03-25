#pragma once

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <QUrl>
#include <QByteArray>
#include <string>
#include <QList>
#include <QObject>
#include <QPointer>
#include "slotscls.h"
#include "pipecls.h"
#include "itemmodel.h"

class WebScraper;  // 仅前置声明
class Item;

// 封装 libxml2 文档的 RAII 资源管理类
class XmlDocument {
public:
    XmlDocument(const char* xmlData, int size);
    ~XmlDocument();
    xmlDocPtr get() const;

private:
    xmlDocPtr doc_ = nullptr;
};

class HtmlParser : public QObject {
    Q_OBJECT
public:
    explicit HtmlParser(QObject *parent = nullptr);
    ~HtmlParser();
    
    xmlXPathContextPtr xpathCtx; // XPath 上下文指针

    // 根据 ID 获取元素
    QList<xmlNodePtr> getElementsById(const std::string &idName) const;

    // 根据 class 获取元素列表
    QList<xmlNodePtr> getElementsByClass(const std::string &className) const;
    
    // 根据tagname获取元素列表
    QList<xmlNodePtr> getElementsByTag(const std::string &tagName) const;
    
    // 在已知的 xmlNodePtr 列表中，根据 id 过滤符合条件的节点
    QList<xmlNodePtr> findNodesById(const QList<xmlNodePtr> &nodes, const std::string &idName);
    
    // 在已知的 xmlNodePtr 列表中，根据 class 过滤符合条件的节点
    QList<xmlNodePtr> findNodesByClass(const QList<xmlNodePtr> &nodes, const std::string &className, bool recursive = false);
        
    // 在已知的 xmlNodePtr 列表中，根据 tagName 过滤符合条件的节点
    QList<xmlNodePtr> findNodesByTag(const QList<xmlNodePtr> &nodes, const std::string &tagName, bool recursive = false);
        
    // 获取a元素的href
    QUrl getHref(xmlNodePtr aNode);
    
    // 获取a元素的文本内容
    QList<Item> getAcontent(QList<xmlNodePtr> &aNodeList);
    
    // 获取a元素的文本内容 返回
    void printNodeContent(xmlNodePtr node) const;

private:
    htmlDocPtr doc; // HTML 文档指针
    QUrl nextUrl;
    // 清理资源
    void cleanUp();
};
