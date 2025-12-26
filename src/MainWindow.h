#pragma once

#include "QFluent/FluentWindow.h"
#include "StyleSheet.h"
#include "QFluent/Label.h"
#include "QFluent/StackedWidget.h"
#include "QFluent/Navigation/NavigationBar.h"
#include "QFluent/TabBar.h"


class MainWindow : public FluentWindow {
    Q_OBJECT
public:
    explicit MainWindow();

    void setCurrentInterface(const QString &routeKey, int index);

private:
    StackedWidget *m_stackedWidget;
    NavigationBar *m_navigationBar;
    TabBar *m_tabBar;

    void addTab();

    void removeTab(int index);

    void initTabBar();

    void initWidget();

    void showDialog();

    void switchWidget(const QString &objectName);

    QWidget *createWidget(int index, const QString &objectName);
};
