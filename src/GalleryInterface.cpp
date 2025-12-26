#include "GalleryInterface.h"

#include "FluentIcon.h"
#include "Theme.h"
#include "StyleSheet.h"

SeparatorWidget::SeparatorWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(6, 16);
}

void SeparatorWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen(Qt::SolidLine);
    pen.setCosmetic(true);

    QColor c = Theme::instance()->isDarkTheme() ? QColor(255, 255, 255, 21) : QColor(0, 0, 0, 15);
    pen.setColor(c);
    painter.setPen(pen);

    int x = width() / 2;
    painter.drawLine(x, 0, x, height());
}

ToolBar::ToolBar(const QString &title, const QString &subtitle, QWidget *parent)
    : QWidget(parent)
{
    titleLabel = new TitleLabel(title, this);
    Theme::instance()->setFont(titleLabel, 28, QFont::Normal);
    appendButton = new TransparentPushButton("New product", FluentIcon(Fluent::IconType::ADD_TO), this);
    separator = new SeparatorWidget(this);
    filterButton = new TransparentPushButton("Fliter", FluentIcon(Fluent::IconType::FILTER), this);
    refreshButton = new TransparentPushButton("Refresh", FluentIcon(Fluent::IconType::SYNC), this);
    windowButton = new TransparentPushButton("New window", FluentIcon(Fluent::IconType::LINK), this);

    hBoxLayout = new QHBoxLayout(this);

    initWidget();
}

void ToolBar::updateTitle(const QString &title)
{
    titleLabel->setText(title);
}

void ToolBar::initWidget()
{
    setFixedHeight(80);

    hBoxLayout->setContentsMargins(35, 20, 35, 0);
    hBoxLayout->addWidget(titleLabel);
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(appendButton);
    hBoxLayout->addWidget(separator);
    hBoxLayout->addWidget(filterButton);
    hBoxLayout->addWidget(refreshButton);
    hBoxLayout->addWidget(windowButton);
}

ExampleCard::ExampleCard(const QString &title, QWidget *widget, const QString &sourcePath,
                         int stretch, QWidget *parent)
    : QWidget(parent)
    , widget(widget)
    , stretch(stretch)
    , sourcePath(sourcePath)
{
    titleLabel = new StrongBodyLabel(title, this);
    Theme::instance()->setFont(titleLabel, 14, QFont::Normal);
    card = new QFrame(this);
    sourceWidget = new QFrame(card);
    sourcePathLabel = new BodyLabel("源代码", sourceWidget);
    linkIcon = new IconWidget(FluentIcon(Fluent::IconType::LINK), sourceWidget);

    vBoxLayout = new QVBoxLayout(this);
    cardLayout = new QVBoxLayout(card);
    topLayout = new QHBoxLayout();
    bottomLayout = new QHBoxLayout(sourceWidget);

    initWidget();
}

void ExampleCard::initWidget()
{
    linkIcon->setFixedSize(16, 16);
    initLayout();

    sourceWidget->setCursor(Qt::PointingHandCursor);
    sourceWidget->installEventFilter(this);

    card->setObjectName("card");
    sourceWidget->setObjectName("sourceWidget");
}

void ExampleCard::initLayout()
{
    vBoxLayout->setSizeConstraint(QLayout::SetMinimumSize);
    cardLayout->setSizeConstraint(QLayout::SetMinimumSize);
    topLayout->setSizeConstraint(QLayout::SetMinimumSize);

    vBoxLayout->setSpacing(12);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setContentsMargins(12, 12, 12, 12);
    bottomLayout->setContentsMargins(18, 18, 18, 18);
    cardLayout->setContentsMargins(0, 0, 0, 0);

    vBoxLayout->addWidget(titleLabel, 0, Qt::AlignTop);
    vBoxLayout->addWidget(card, 0, Qt::AlignTop);
    vBoxLayout->setAlignment(Qt::AlignTop);

    cardLayout->setSpacing(0);
    cardLayout->setAlignment(Qt::AlignTop);
    cardLayout->addLayout(topLayout, 0);
    cardLayout->addWidget(sourceWidget, 0, Qt::AlignBottom);

    widget->setParent(card);
    topLayout->addWidget(widget);
    if (stretch == 0) {
        topLayout->addStretch(1);
    }

    widget->show();

    bottomLayout->addWidget(sourcePathLabel, 0, Qt::AlignLeft);
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(linkIcon, 0, Qt::AlignRight);
    bottomLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

bool ExampleCard::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == sourceWidget && e->type() == QEvent::MouseButtonRelease) {
        QDesktopServices::openUrl(QUrl(sourcePath));
    }
    return QWidget::eventFilter(obj, e);
}

GalleryInterface::GalleryInterface(const QString &title, const QString &subtitle, QWidget *parent)
    : ScrollArea(parent)
{
    view = new QWidget(this);
    toolBar = new ToolBar(title, subtitle.isEmpty() ? "遇到问题可以告诉我哦，看到就会及时处理。🐧1912229135" : subtitle, this);
    vBoxLayout = new QVBoxLayout(view);

    setViewportMargins(0, toolBar->height(), 0, 0);
    setWidget(view);
    setWidgetResizable(true);

    vBoxLayout->setSpacing(30);
    vBoxLayout->setAlignment(Qt::AlignTop);
    vBoxLayout->setContentsMargins(36, 20, 36, 36);

    view->setObjectName("view");

    auto styleSource = std::make_shared<TemplateStyleSheetFile>(":/res/style/{theme}/gallery_interface.qss");
    StyleSheetManager::instance()->registerWidget(styleSource, this);
}

ExampleCard* GalleryInterface::addExampleCard(const QString &title, QWidget *widget,
                                              const QString &sourcePath, int stretch)
{
    ExampleCard *card = new ExampleCard(title, widget, sourcePath, stretch, view);
    vBoxLayout->addWidget(card, 0, Qt::AlignTop);
    return card;
}

void GalleryInterface::scrollToCard(int index)
{

}

void GalleryInterface::resizeEvent(QResizeEvent *e)
{
    ScrollArea::resizeEvent(e);
    toolBar->resize(width(), toolBar->height());
}

QLayout *GalleryInterface::layout() const
{
    return vBoxLayout;
}
