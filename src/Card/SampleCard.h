#ifndef SAMPLE_CARD_H
#define SAMPLE_CARD_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "QFluent/Label.h"
#include "QFluent/ImageLabel.h"
#include "QFluent/CardWidget.h"
#include "QFluent/Layout/FlowLayout.h"

class IconWidget;
class FlowLayout;
class CardWidget;

class SampleCard : public CardWidget
{
    Q_OBJECT

public:
    SampleCard(const QString &icon, const QString &title, const QString &content,
               const QString &routeKey, int index, QWidget *parent = nullptr);

signals:
    void clicked(const QString &routeKey, int index);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int m_index;
    QString m_routeKey;
    ImageLabel *m_iconWidget;
    BodyLabel *m_titleLabel;
    CaptionLabel *m_subTitleLabel;
    IconWidget *m_markIcon;

    void initWidget();
};

class SampleCardView : public QWidget
{
    Q_OBJECT

public:
    explicit SampleCardView(const QString &title, QWidget *parent = nullptr);

    void addSampleCard(const QString &iconPath, const QString &title, const QString &content, 
                       const QString &routeKey, int index);

signals:
    void clicked(const QString &routeKey, int index);

private:
    SubtitleLabel *m_titleLabel;
    QVBoxLayout *m_vBoxLayout;
    FlowLayout *m_flowLayout;

    void initWidget();
    void createFlowLayout();
};

#endif // SAMPLE_CARD_H
