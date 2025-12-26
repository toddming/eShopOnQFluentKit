#ifndef HOME_INTERFACE_H
#define HOME_INTERFACE_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>

#include "QFluent/ScrollArea.h"
#include "GalleryInterface.h"

class SampleCardView;

class HomeInterface : public ScrollArea
{
    Q_OBJECT

public:
    explicit HomeInterface(QWidget *parent = nullptr);

private:
    QWidget *m_view;
    QVBoxLayout *m_vBoxLayout;
    ToolBar *m_toolBar;

    void initWidget();
    void loadSamples();
    QMap<QString, QString> listFilesInDirectory(const QString &folderPath);
};

#endif // HOME_INTERFACE_H
