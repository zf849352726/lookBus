#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "webscraper.h"
#include "slotscls.h"


int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    
    engine.loadFromModule("loveBus", "Main");
    

    WebScraper *webscraper = new WebScraper();

    // 创建 WebScraper 实例
    SlotsCls *slotscls = new SlotsCls();
    
    // 连接 SlotsCls 的 slotFinished 信号到主线程中的槽
      QObject::connect(slotscls, &SlotsCls::slotFinished, [&]() {
          // 在槽函数执行完之后暴露模型到 QML
          engine.rootContext()->setContextProperty("hotModel", webscraper->getModel());
          qDebug() << "Model exposed to QML";
      });
    
    engine.rootContext()->setContextProperty("slotscls", slotscls);

    return app.exec();
}
