/*!
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: Implements grid of numeric buttons
*
*/

#ifndef DIALPADNUMERICBUTTONGRID_H
#define DIALPADNUMERICBUTTONGRID_H

#include <QTextLayout>
#include <QColor>
#include <hbinputbuttongroup.h>

class DialpadNumericButton;

class DialpadNumericButtonGrid : public HbInputButtonGroup
{
    Q_OBJECT

public:
    explicit DialpadNumericButtonGrid(
        const HbMainWindow& mainWindow,
        QGraphicsItem* parent=0);

    ~DialpadNumericButtonGrid();

public:
    void resetButtons();

    QChar inputCharacter(int keyCode) const;

protected slots:
    void setButtonTexts();
    void updateButtonLabels();
    void cancelButtonPress();

protected:
    inline DialpadNumericButton* button(int i) const;
    void updateColorArray();
    void updateIconColor();
    void updateTextLayouts(const QSizeF &size);
    void resolveTextContent(QList<QString> &content, bool& useTwoRows);
    void createTextLayouts(const QSizeF &size,
                           const QList<QString> &content,
                           bool useTwoRows);
    void layoutTextLines(const QSizeF &size,
                         QTextLayout &textLayout,
                         int state,
                         int type,
                         bool useTwoRows);
    inline void layoutPrimaryText(
        QTextLayout& layout,
        DialpadNumericButton& button,
        qreal cellWidth,
        qreal cellHeight,
        qreal textHeight) const;
    inline void layoutSecondaryText(
        QTextLayout& layout,
        DialpadNumericButton& button,
        qreal cellWidth,
        qreal cellHeight,
        qreal textHeight,
        qreal textDescent,
        bool useTwoRows) const;
    bool isButtonPressed() const;
    void updateIconSizes();

protected:
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);
    void setGeometry(const QRectF &rect);
    void changeEvent(QEvent *event);

private:
    enum ButtonState {
        Normal = 0,
        Pressed,
        StateCount
    };

    enum TextType {
        PrimaryText = 0,
        SecondaryText,
        TextTypeCount
    };

private:
    const HbMainWindow& mMainWindow;
    QList<QColor> mColors;
    QList<QTextLayout*> mTextLayouts;
    qreal mUnit;
    qreal mMaxPrimaryLineWidth;
    qreal mPrimaryTextBaseline;
    QMap<int,QChar> mGeneratedChar;
};

#endif // DIALPADNUMERICBUTTONGRID_H
