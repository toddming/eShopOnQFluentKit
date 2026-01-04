#include "MainWindow.h"

#include "QFluent/Navigation/NavigationPanel.h"
#include "QFluent/Navigation/NavigationWidget.h"
#include "QFluent/Dialog/MessageDialog.h"
#include "QFluent/FluentTitleBar.h"
#include "QFluent/ImageLabel.h"
#include "QFluent/ToolButton.h"

#include "Theme.h"
#include "FluentIcon.h"
#include "HomeInterface.h"
#include "SettingInterface.h"
#include "ScrollInterface.h"

#include "ConfigManager.h"

using FIT = Fluent::IconType;
using NIP = Fluent::NavigationItemPosition;
using FWB = Fluent::WindowButtonHint;

MainWindow::MainWindow()
{
    setWindowTitle("QFluentKit");
    setWindowButtonHints(FWB::Close | FWB::Maximize | FWB::Minimize | FWB::ThemeToggle);
    setWindowIcon(QPixmap(":/res/example.png"));
    resize(1125, 800);

    initWidget();

    initTabBar();
}

void MainWindow::initTabBar()
{
    QWidget *windowBar = titleBar()->centerWidget();
    auto hBoxLayout = new QHBoxLayout(windowBar);
    hBoxLayout->setContentsMargins(5, 0, 25, 0);

    auto leftButton = new PillToolButton(FluentIcon(FIT::ARROW_DOWN), windowBar);
    leftButton->setCheckable(false);
    hBoxLayout->addWidget(leftButton, 0, Qt::AlignLeft);

    m_tabBar = new TabBar(windowBar);
    hBoxLayout->addWidget(m_tabBar, 1, Qt::AlignBottom);

    connect(m_tabBar, &TabBar::tabAddRequested, this, [this](){ addTab(); });
    connect(m_tabBar, &TabBar::tabCloseRequested, this, [this](int index){ removeTab(index); });
    connect(m_tabBar, &TabBar::tabBarClicked, this, [=](int index){
        m_navigationBar->setCurrentItem("1");
        const QString routeKey = m_tabBar->tabItem(index)->routeKey();
        m_stackedWidget->setCurrentWidget(m_stackedWidget->findChild<QWidget *>(routeKey));
    });

    auto avatar = new TransparentDropDownToolButton(FluentIcon(":/res/avatar.png"), this);
    avatar->setIconSize(QSize(26, 26));
    avatar->setFixedHeight(30);
    hBoxLayout->addWidget(avatar);

    connect(avatar, &TransparentDropDownToolButton::clicked, this, [this](){ showDialog(); });

    setHitTestVisible(m_tabBar->addButton(), true);
    setHitTestVisible(leftButton, true);
    setHitTestVisible(avatar, true);
    addTab();
}

void MainWindow::initWidget()
{
    m_navigationBar = new NavigationBar(this);
    m_navigationBar->addItem("1", FluentIcon(FIT::QUICK_NOTE), "标签", [=](){
        if (m_tabBar->count() > 0) {
            switchWidget(m_tabBar->currentTab()->routeKey());
        } else {
            switchWidget("emptyWidget");
        }
    }, true, NIP::TOP);
    // m_navigationBar->addSeparator();
    m_navigationBar->addItem("2", FluentIcon(FIT::SHOPPING_CART), "产品", [=](){
        switchWidget("homeInterface");
    }, true, NIP::SCROLL);
    m_navigationBar->addItem("3", FluentIcon(FIT::DATE_TIME), "日期", nullptr, true, NIP::SCROLL);
    m_navigationBar->addItem("4", FluentIcon(FIT::MESSAGE), "信息框", nullptr, true, NIP::SCROLL);
    // m_navigationBar->addSeparator(NIP::BOTTOM);
    m_navigationBar->addItem("5", FluentIcon(FIT::SETTING), "设置", [=](){
        switchWidget("settingInterface");
    }, true, NIP::BOTTOM);
    m_navigationBar->setCurrentItem("1");

    m_stackedWidget = new StackedWidget(this);

    m_stackedWidget->addWidget(new QWidget(this));
    m_stackedWidget->addWidget(new HomeInterface(this));
    m_stackedWidget->addWidget(new SettingInterface(this));
    m_stackedWidget->addWidget(createWidget(-1, "emptyWidget_2"));

    auto widget = new QWidget(this);
    auto layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_navigationBar, 0);
    layout->addWidget(m_stackedWidget, 1);

    setCentralWidget(widget);
}


void MainWindow::addTab()
{
    static int tabCount = 0;
    int count = ++tabCount;
    const QString routeKey = QString("objectName_%1").arg(count);

    auto tab = m_tabBar->addTab(routeKey, QString("新建标签x%1").arg(count), QIcon(":/res/tab.png"));
    setHitTestVisible(tab, true);

    m_stackedWidget->addWidget(createWidget(count, routeKey));

    if (m_tabBar->count() > 0) {
        m_tabBar->setCurrentTab(routeKey);
        if (m_stackedWidget->currentWidget()->objectName() != routeKey) {
            m_stackedWidget->setCurrentWidget(m_stackedWidget->findChild<QWidget *>(routeKey));
        }
    }
}

void MainWindow::removeTab(int index)
{
    const QString routeKey = m_tabBar->tabItem(index)->routeKey();
    m_stackedWidget->removeWidget(m_stackedWidget->findChild<QWidget *>(routeKey));
    m_tabBar->removeTab(index);
    if (m_tabBar->count() > 0) {
        const QString routeKey = m_tabBar->currentTab()->routeKey();
        if (m_stackedWidget->currentWidget()->objectName() != routeKey) {
            m_stackedWidget->setCurrentWidget(m_stackedWidget->findChild<QWidget *>(routeKey));
        }
    }
}


void MainWindow::showDialog()
{
    auto box = new MessageDialog("你是遇到问题了吗🧐",
                                 "遇到问题？欢迎加我 QQ（1912229135）反馈～看到后我会第一时间修复，感谢你让这个项目变得越来越棒！",
                                 this->window());
    box->setIsClosableOnMaskClicked(true);
    box->exec();
}

void MainWindow::switchWidget(const QString &objectName)
{
    m_stackedWidget->setCurrentWidget(m_stackedWidget->findChild<QWidget *>(objectName), false);
}


QWidget *MainWindow::createWidget(int index, const QString &objectName)
{
    const QString text = (index < 0) ? "暂时没有标签页~" : QString("这是一个新的标签页 x%1").arg(index);
    auto label = new SubtitleLabel(text, m_stackedWidget);
    label->setAlignment(Qt::AlignCenter);

    auto w = new QWidget(m_stackedWidget);
    w->setObjectName(objectName);

    uint emojiCodePoint = 0x1F600 + index;
    QString emoji = QString::fromUcs4(&emojiCodePoint, 1);
    auto image = new QLabel(index < 0 ? "🫗" : emoji, m_stackedWidget);
    Theme::instance()->setFont(image, 200);
    auto vBoxLayout = new QVBoxLayout(w);
    vBoxLayout->addStretch();
    vBoxLayout->addWidget(image, 0, Qt::AlignCenter);
    vBoxLayout->addWidget(label, 0, Qt::AlignCenter);
    vBoxLayout->addStretch();

    return w;
}
