#include "HomeInterface.h"

#include <QDir>
#include <QMap>

#include "Card/SampleCard.h"
#include "FluentIcon.h"
#include "Theme.h"
#include "StyleSheet.h"

#include "MainWindow.h"


HomeInterface::HomeInterface(QWidget *parent)
    : ScrollArea(parent)
{    
    m_view = new QWidget(this);
    m_vBoxLayout = new QVBoxLayout(m_view);
    m_toolBar = new ToolBar("Products", "", this);

    initWidget();
    loadSamples();
}

void HomeInterface::initWidget()
{
    m_view->setObjectName("view");
    setObjectName("homeInterface");

    auto styleSource = std::make_shared<TemplateStyleSheetFile>(":/res/style/{theme}/home_interface.qss");
    StyleSheetManager::instance()->registerWidget(styleSource, this);

    setWidget(m_view);
    setWidgetResizable(true);
    setViewportMargins(0, 0, 0, 0);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 36);
    m_vBoxLayout->setSpacing(40);

    m_vBoxLayout->setAlignment(Qt::AlignTop);
    m_vBoxLayout->addWidget(m_toolBar);
}

void HomeInterface::loadSamples()
{
    // 基础输入样例
    SampleCardView *vistaGeralView = new SampleCardView("Most popular", m_view);

    QMap<QString, QString> map = listFilesInDirectory("iPhone");
    QStringList keys = map.keys() + map.keys() + map.keys() + map.keys();

    for (int i=0; i < keys.size(); i++) {
        QString key = keys.at(i);
        QString value = map.value(key);
        const QString model = key.split("_").first();
        const QString color = key.split("_").last();
        vistaGeralView->addSampleCard(value, model, color, "iconInterface", i+1);
    }

    m_vBoxLayout->addWidget(vistaGeralView);
    m_toolBar->updateTitle(QString("Products (%1)").arg(keys.size()));
}

QMap<QString, QString> HomeInterface::listFilesInDirectory(const QString &folderPath)
{
    QMap<QString, QString> fileMap;
    QDir dir(folderPath);

    if (!dir.exists()) {
        return fileMap;
    }

    QFileInfoList entries = dir.entryInfoList(QDir::Files);

    for (const QFileInfo &entry : entries) {
        QString fileName = entry.baseName();
        QString absolutePath = entry.absoluteFilePath();
        fileMap.insert(fileName, absolutePath);
    }

    return fileMap;
}
