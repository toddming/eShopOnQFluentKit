#include "SampleCard.h"
#include "QFluent/IconWidget.h"
#include <QGridLayout>
#include "StyleSheet.h"
#include "Theme.h"

SampleCard::SampleCard(const QString &icon, const QString &title, const QString &content,
                       const QString &routeKey, int index, QWidget *parent)
    : CardWidget(parent)
    , m_index(index)
    , m_routeKey(routeKey)
{
    m_iconWidget = new ImageLabel(icon, this);
    m_titleLabel = new BodyLabel(title, this);
    m_subTitleLabel = new CaptionLabel(content, this);
    QColor color = Theme::instance()->themeColor();
    m_markIcon = new IconWidget(ColoredFluentIcon(":/res/icons/selected.svg", color, color), this);

    m_markIcon->setHidden(!title.contains("Pro Max"));

    m_titleLabel->setContentsMargins(13, 0, 0, 0);
    m_subTitleLabel->setContentsMargins(13, 0, 0, 0);

    initWidget();
}

void SampleCard::initWidget()
{
    setCursor(Qt::PointingHandCursor);
    setFixedSize(185, 290);

    m_iconWidget->scaledToWidth(140);
    m_iconWidget->setBorderRadius(5, 5, 5, 5);

    m_markIcon->move(185 - m_markIcon->width() - 9, 9);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setContentsMargins(9, 20, 9, 9);
    vBoxLayout->setAlignment(Qt::AlignVCenter);

    vBoxLayout->addStretch(1);
    vBoxLayout->addWidget(m_iconWidget, 0, Qt::AlignCenter);
    vBoxLayout->addStretch(1);

    vBoxLayout->addWidget(m_titleLabel);
    vBoxLayout->addWidget(m_subTitleLabel);
    vBoxLayout->addStretch(1);
}

void SampleCard::mouseReleaseEvent(QMouseEvent *event)
{
    CardWidget::mouseReleaseEvent(event);
    emit clicked(m_routeKey, m_index);
}

SampleCardView::SampleCardView(const QString &title, QWidget *parent)
    : QWidget(parent)
    , m_flowLayout(nullptr)
{
    m_titleLabel = new SubtitleLabel(title, this);
    m_vBoxLayout = new QVBoxLayout(this);

    initWidget();
    createFlowLayout();
}

void SampleCardView::initWidget()
{
    m_vBoxLayout->setContentsMargins(36, 0, 36, 0);
    m_vBoxLayout->setSpacing(16);

    m_vBoxLayout->addWidget(m_titleLabel);
}

void SampleCardView::createFlowLayout()
{
    m_flowLayout = new FlowLayout(nullptr, true);
    m_flowLayout->setContentsMargins(0, 0, 0, 0);
    m_flowLayout->setHorizontalSpacing(12);
    m_flowLayout->setVerticalSpacing(12);
    
    m_vBoxLayout->addLayout(m_flowLayout);
}

void SampleCardView::addSampleCard(const QString &iconPath, const QString &title,
                                   const QString &content, const QString &routeKey, int index)
{
    SampleCard *card = new SampleCard(iconPath, title, content, routeKey, index, this);
    m_flowLayout->addWidget(card);

    connect(card, &SampleCard::clicked, this, &SampleCardView::clicked);
}

