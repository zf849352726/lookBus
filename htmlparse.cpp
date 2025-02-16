#include "htmlparse.h"
#include <QDebug>
#include <sstream>
#include <regex>
#include "webscraper.h"

// 封装 libxml2 文档的 RAII 资源管理类
XmlDocument::XmlDocument(const char* xmlData, int size) {
    doc_ = xmlParseMemory(xmlData, size);
    if (doc_ == nullptr) {
        throw std::runtime_error("Failed to parse XML data");
    }
}

XmlDocument::~XmlDocument() {
    if (doc_) {
        xmlFreeDoc(doc_);
    }
}

xmlDocPtr XmlDocument::get() const { return doc_; }

// 构造函数
HtmlParser::HtmlParser(QObject *parent) : QObject(parent), doc(nullptr), xpathCtx(nullptr) {}

// 析构函数
HtmlParser::~HtmlParser() {
    cleanUp();
}

// 清理资源
void HtmlParser::cleanUp() {
    if (xpathCtx) {
        xmlXPathFreeContext(xpathCtx);
        xpathCtx = nullptr;
    }
    if (doc) {
        xmlFreeDoc(doc);
        doc = nullptr;
    }
}

// 根据 ID 获取元素
QList<xmlNodePtr> HtmlParser::getElementsById(const std::string &idName) const {
    QList<xmlNodePtr> nodes;
    if (!xpathCtx) return nodes;

    std::string xpathExpr = "//*[@id='" + idName + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(reinterpret_cast<const xmlChar *>(xpathExpr.c_str()), xpathCtx);
    if (!xpathObj || xmlXPathNodeSetIsEmpty(xpathObj->nodesetval)) {
        if (xpathObj) xmlXPathFreeObject(xpathObj);
        return nodes;
    }
    
    // 遍历所有匹配节点
    for (int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) {
        nodes.append(xpathObj->nodesetval->nodeTab[i]);
    }
    xmlXPathFreeObject(xpathObj);
    return nodes;
}

// 根据 class 获取元素列表
QList<xmlNodePtr> HtmlParser::getElementsByClass(const std::string &className) const {
    QList<xmlNodePtr> nodes;
    if (!xpathCtx || className.empty()) return nodes;

    std::string xpathExpr = "//*[@class='" + className + "']";
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(reinterpret_cast<const xmlChar *>(xpathExpr.c_str()), xpathCtx);
    if (!xpathObj || xmlXPathNodeSetIsEmpty(xpathObj->nodesetval)) {
        if (xpathObj) xmlXPathFreeObject(xpathObj);
        return nodes;
    }

    for (int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) {
        nodes.append(xpathObj->nodesetval->nodeTab[i]);
    }

    xmlXPathFreeObject(xpathObj);
    return nodes;
}

// 根据tagname获取元素列表
QList<xmlNodePtr> HtmlParser::getElementsByTag(const std::string &tagName) const {
    QList<xmlNodePtr> nodes;
    if (!xpathCtx) return nodes;

    // XPath 语法：查找所有指定标签的元素
    std::string xpathExpr = "//" + tagName;

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(reinterpret_cast<const xmlChar *>(xpathExpr.c_str()), xpathCtx);
    if (!xpathObj || xmlXPathNodeSetIsEmpty(xpathObj->nodesetval)) {
        if (xpathObj) xmlXPathFreeObject(xpathObj);
        return nodes;
    }

    // 遍历所有匹配的节点
    for (int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) {
        nodes.append(xpathObj->nodesetval->nodeTab[i]);
    }

    xmlXPathFreeObject(xpathObj);
    return nodes;
}

// 在已知的 xmlNodePtr 列表中，根据 id 过滤符合条件的节点
QList<xmlNodePtr> HtmlParser::findNodesById(const QList<xmlNodePtr> &nodes, const std::string &idName) {
    QList<xmlNodePtr> foundNodes;
    if (idName.empty()) return foundNodes;  // 修正 nullptr 判断

    for (xmlNodePtr node : nodes) {
        if (!node || node->type != XML_ELEMENT_NODE) continue;

        // 获取 id 属性
        xmlChar *idAttr = xmlGetProp(node, reinterpret_cast<const xmlChar *>("id"));
        if (idAttr) {
            std::string idStr(reinterpret_cast<const char *>(idAttr));
            xmlFree(idAttr);  // 确保只在 idAttr 不为 nullptr 时释放

            // 判断 id 是否匹配
            if (idStr == idName) {
                foundNodes.append(node);
            }
        }
    }
    return foundNodes;
}

// 在已知的 xmlNodePtr 列表中，根据 class属性 过滤符合条件的节点
QList<xmlNodePtr> HtmlParser::findNodesByClass(const QList<xmlNodePtr> &nodes, const std::string &className, bool recursive) {
    QList<xmlNodePtr> foundNodes;
    if (className.empty()) return foundNodes;  // 避免无效查找

    for (xmlNodePtr node : nodes) {
        if (!node) continue;

        if (recursive) {
            // 用栈进行 DFS 递归查找
            QList<xmlNodePtr> stack = {node};

            while (!stack.isEmpty()) {
                xmlNodePtr current = stack.takeLast();

                if (current->type == XML_ELEMENT_NODE) {
                    // 获取 class 属性
                    xmlChar *classAttr = xmlGetProp(current, reinterpret_cast<const xmlChar *>("class"));
                    if (classAttr) {
                        std::string classStr(reinterpret_cast<const char *>(classAttr));
                        xmlFree(classAttr);  // 释放 xmlGetProp 分配的内存

                        // 解析 class 并匹配
                        std::istringstream iss(classStr);
                        std::string token;
                        while (iss >> token) {
                            if (token == className) {
                                foundNodes.append(current);
                                break;  // 避免重复插入
                            }
                        }
                    }
                }

                // 从右到左遍历子节点，保证 DFS 遍历顺序
                QList<xmlNodePtr> children;
                for (xmlNodePtr child = current->children; child; child = child->next) {
                    children.append(child);
                }
                stack.append(children);
            }
        } else {
            // 仅查找直接子节点
            for (xmlNodePtr child = node->children; child; child = child->next) {
                if (child->type != XML_ELEMENT_NODE) continue;

                // 获取 class 属性
                xmlChar *classAttr = xmlGetProp(child, reinterpret_cast<const xmlChar *>("class"));
                if (classAttr) {
                    std::string classStr(reinterpret_cast<const char *>(classAttr));
                    xmlFree(classAttr);

                    std::istringstream iss(classStr);
                    std::string token;
                    while (iss >> token) {
                        if (token == className) {
                            foundNodes.append(child);
                            break;
                        }
                    }
                }
            }
        }
    }
    return foundNodes;
}

// 根据 tagName 过滤符合条件的节点
QList<xmlNodePtr> HtmlParser::findNodesByTag(const QList<xmlNodePtr> &nodes, const std::string &tagName, bool recursive) {
    QList<xmlNodePtr> foundNodes;
    if (tagName.empty()) return foundNodes;  // 避免无效查找

    for (xmlNodePtr node : nodes) {
        if (!node) continue;

        if (recursive) {
            // 用栈进行 DFS 递归查找
            QList<xmlNodePtr> stack = {node};

            while (!stack.isEmpty()) {
                xmlNodePtr current = stack.takeLast();

                if (current->type == XML_ELEMENT_NODE && xmlStrcmp(current->name, reinterpret_cast<const xmlChar *>(tagName.c_str())) == 0) {
                    foundNodes.append(current);
                }

                // 从右到左遍历子节点，保证 DFS 遍历顺序正确
                QList<xmlNodePtr> children;
                for (xmlNodePtr child = current->children; child; child = child->next) {
                    children.append(child);
                }
                stack.append(children);
            }
        } else {
            // 仅查找直接子节点
            for (xmlNodePtr child = node->children; child; child = child->next) {
                if (child->type == XML_ELEMENT_NODE && xmlStrcmp(child->name, reinterpret_cast<const xmlChar *>(tagName.c_str())) == 0) {
                    foundNodes.append(child);
                }
            }
        }
    }
    return foundNodes;
}

QUrl HtmlParser::getHref(xmlNodePtr aNode) {
    if (!aNode) return QUrl();

    // 获取 href 属性
    xmlChar *href = xmlGetProp(aNode, BAD_CAST "href");
    if (href) {
        std::string hrefStr(reinterpret_cast<const char*>(href));
        xmlFree(href);  // 释放内存
        nextUrl = QUrl::fromUserInput(QString::fromStdString(hrefStr));
        return nextUrl;
    }

    return QUrl();
}

QList<QString> HtmlParser::getAcontent(QList<xmlNodePtr> &aNodeList) {
    QList<QString> resultList;

    // 遍历所有传入的 <a> 标签节点
    for (xmlNodePtr aNode : aNodeList) {
        if (!aNode) continue;  // 跳过空节点

        // 确保是 <a> 标签
        if (aNode->type != XML_ELEMENT_NODE || xmlStrcmp(aNode->name, BAD_CAST "a") != 0) {
            continue;  // 如果不是 <a> 标签，跳过
        }

        // 获取 <a> 标签的文本内容
        xmlChar *content = xmlNodeGetContent(aNode);
        if (content) {
            // 将内容转换为 QString 并加入到结果列表
            resultList.append(QString::fromUtf8(reinterpret_cast<const char*>(content)));
            xmlFree(content);  // 释放内存
        }
    }

    return resultList;
}


// 打印节点内容
void HtmlParser::printNodeContent(xmlNodePtr node) const {
    if (!node) return;

    xmlChar *content = xmlNodeGetContent(node);
    if (content) {
        qDebug() << "Node content:" << QString::fromUtf8(reinterpret_cast<const char *>(content));
        xmlFree(content);
    }
}
