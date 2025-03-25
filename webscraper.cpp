// webscraper.cpp
// seeBus

#include "webscraper.h"
#include <QThread>


//WebScraper::WebScraper() {}

// WebScraper.cpp 中的构造函数
WebScraper::WebScraper(QObject *parent)
: QObject(parent), manager(new QNetworkAccessManager(this)), htmlparse(new HtmlParser(this)), pipe(new PipeCls(this)), hotModel(new ItemModel(this)) {
}

//WebScraper:: WebScraper(QSharedPointer<ItemModel> model){
//    this->model = model;
//}

ItemModel *WebScraper:: getModel(){
    return this->hotModel;
}

PipeCls *WebScraper:: getPipeObj(){
    return this->pipe;
}

void WebScraper::fetchData(const QUrl &url, std::function<void(const QByteArray&)> parseFunction) {
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    // 连接finished信号
    connect(reply, &QNetworkReply::finished, [this, reply, parseFunction]() {
        // 获取响应
        if (!reply) return;

        // 请求成功时处理
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();  // 获取网页内容
            
            // 确保内容是 UTF-8 编码
            QString content = QString::fromUtf8(responseData);
            const QByteArray responseDataUtf8 = content.toUtf8();  // 添加 const 修饰，防止修改
            
//            qDebug() << content;

            // 解析 HTML 数据并设置 xpathCtx
            // 创建一个新的 xmlDocPtr 文档对象
            xmlDocPtr doc = htmlReadMemory(responseDataUtf8.constData(), static_cast<int>(responseDataUtf8.size()), NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
            if (!doc) {
                qDebug() << "Failed to parse HTML.";
                return;
            }

            // 创建 XPath 上下文
            htmlparse->xpathCtx = xmlXPathNewContext(doc);
            if (!htmlparse->xpathCtx) {
                qDebug() << "Failed to create XPath context.";
                xmlFreeDoc(doc);
                return;
            }

            // 将 xpathCtx 和 doc 传递给解析函数
            parseFunction(responseDataUtf8);

            // 释放资源
            xmlXPathFreeContext(htmlparse->xpathCtx);
            xmlFreeDoc(doc);
        } else {
            qDebug() << "Request failed:" << reply->errorString();
        }

        // 释放资源
        reply->deleteLater();
    });
}


void WebScraper::parseDefault(const QByteArray &data) {
    const QList<xmlNodePtr> ulElements = htmlparse->getElementsByClass("nav navbar-nav");
    const QList<xmlNodePtr> liElements = htmlparse->findNodesByTag(ulElements, "li");
    const QList<xmlNodePtr> aElements = htmlparse->findNodesByTag(liElements, "a");
//    for (const xmlNodePtr &node:aElements){
//        htmlparse->printNodeContent(node);
//    }
    nextUrl = htmlparse->getHref(aElements[3]);

    if (!nextUrl.isEmpty()) {
        qDebug() << "Found next URL (default):" << nextUrl;
        // 使用 Lambda 表达式递归调用
        this->fetchData(nextUrl, [this](const QByteArray &data){
            this->parseForum(data);
        });
    } else {
        qDebug() << "No next page found, stopping.";
    }
}

void WebScraper::parseForum(const QByteArray &data) {
    QList<xmlNodePtr> divElements = htmlparse->getElementsById("category_1");
    QList<xmlNodePtr> trElements = htmlparse->findNodesByTag({divElements[1]}, "tr", true);
    QList<xmlNodePtr> tdElements = htmlparse->findNodesByTag({trElements[0]}, "td");
    QList<xmlNodePtr> aElements = htmlparse->findNodesByTag({tdElements[1]}, "a", true);
    
    nextUrl = htmlparse->getHref(aElements[0]);
    
    // 获取当前 URL 的字符串表示
    QString currentUrlStr = nextUrl.toString();

    // 拼接前缀
    QStringList list = currentUrlStr.split("/");
    QString fullUrlStr = "https://www.javbus.com/forum/" + list[2];

    // 创建一个新的 QUrl 对象
    nextUrl = QUrl(fullUrlStr);
    
    if (!nextUrl.isEmpty()) {
        qDebug() << "Found next URL (forum):" << nextUrl;
        // 使用 Lambda 表达式递归调用
        this->fetchData(nextUrl, [this](const QByteArray &data){
            this->parseForumJp(data);
        });
    } else {
        qDebug() << "No next page found, stopping.";
    }
}

void WebScraper::parseForumJp(const QByteArray &data) {
    QList <xmlNodePtr> aElements = htmlparse->getElementsByClass("s");
    // 获取主题
    
    QList<Item> items = htmlparse->getAcontent(aElements);
    this->hotModel->setDataFromList(items);
    nextUrl = htmlparse->getHref(aElements[0]);
    
    // 获取当前 URL 的字符串表示
    QString currentUrlStr = nextUrl.toString();

    // 拼接前缀
    QStringList list = currentUrlStr.split("/");
    QString fullUrlStr = "https://www.javbus.com/forum/" + list[2];

    // 创建一个新的 QUrl 对象
    nextUrl = QUrl(fullUrlStr);
    
    if (!nextUrl.isEmpty()) {
        qDebug() << "Found next URL (forum):" << nextUrl;
        // 使用 Lambda 表达式递归调用
        this->fetchData(nextUrl, [this](const QByteArray &data){
            this->parseForumDetail(data);
        });  // 调用成员函数
    } else {
        qDebug() << "No next page found, stopping.";
    }
}

void WebScraper::parseForumDetail(const QByteArray &data) {
    QList<xmlNodePtr> divElements = htmlparse->getElementsByClass("t_fsz");
    QList<xmlNodePtr> tdElements = htmlparse->findNodesByTag(divElements, "td", true);
    nextUrl = QUrl();
    // 获取当前 URL 的字符串表示
    QString currentUrlStr = nextUrl.toString();
    //*[@class="t_fsz"]//td
    // 拼接前缀
    QString fullUrlStr = "" + currentUrlStr;

    // 创建一个新的 QUrl 对象
    nextUrl = QUrl(fullUrlStr);
    
    if (!nextUrl.isEmpty()) {
        qDebug() << "Found next URL (forum):" << nextUrl;
        // 使用 Lambda 表达式递归调用
        this->fetchData(nextUrl, [this](const QByteArray &data){
            qDebug() << "Data fetched, no further parsing required.";
        });
    } else {
        qDebug() << "No next page found, stopping.";
    }
//    qDebug() << "hotModel 行数：" << this->getModel()->rowCount();
}
