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

#include <QPainter>

#include <hbmainwindow.h>
#include <hbcolorscheme.h>
#include <hbevent.h>
#include <hbinputkeymapfactory.h>
#include <hbinputkeymap.h>
#include <hbinpututils.h>
#include <hbinputsettingproxy.h>
#include <hbinputlanguage.h>

#include "dialpadnumericbutton.h"
#include "dialpadnumericbuttongrid.h"

static const int DialpadRowCount = 4;
static const int DialpadColumnCount = 3;
// layout values in units
static const qreal DialpadPrimaryTextSizeV = 5.25;
static const qreal DialpadPrimaryTextSizeH = 4.5;
static const qreal DialpadSecondaryTextSizeV = 3.8;
static const qreal DialpadSecondaryTextSizeH = 3.3;
static const qreal DialpadSecondaryTextSize2RowsV = 2.8;
static const qreal DialpadSecondaryTextSize2RowsH = 2.6;
static const qreal DialpadIconSizeV = 5.0;
static const qreal DialpadIconSizeH = 4.25;
static const qreal DialpadPrimaryTextLeftMargin  = 1.5;
static const qreal DialpadPrimarySecondaryMargin  = 1.0;
static const qreal DialpadButtonBorderSize  = 0.5;
static const qreal DialpadSecondaryTextOpacity = 0.4;

static const QLatin1String vmbxIcon("qtg_mono_voice_mailbox");

static const int DialpadKeyCodeTable[DialpadRowCount*DialpadColumnCount] =
{
    Qt::Key_1,        Qt::Key_2,      Qt::Key_3,
    Qt::Key_4,        Qt::Key_5,      Qt::Key_6,
    Qt::Key_7,        Qt::Key_8,      Qt::Key_9,
    Qt::Key_Asterisk, Qt::Key_0,      Qt::Key_NumberSign
};

DialpadNumericButtonGrid::DialpadNumericButtonGrid(
    const HbMainWindow& mainWindow,
    QGraphicsItem* parent) :
    HbInputButtonGroup(parent),
    mMainWindow(mainWindow),
    mUnit(.0),
    mMaxPrimaryLineWidth(.0),
    mPrimaryTextBaseline(.0)
{
    setObjectName("numericButtons");

    // create buttons
    setGridSize(QSize(DialpadColumnCount, DialpadRowCount));
    setButtonBorderSize(DialpadButtonBorderSize);

    QList<HbInputButton*> buttons;

    for (int i = 0; i < DialpadRowCount * DialpadColumnCount; ++i) {
        DialpadNumericButton *item = new DialpadNumericButton(
            DialpadKeyCodeTable[i],
            QPoint(i % DialpadColumnCount, i / DialpadColumnCount));
        buttons.append(item);

        item->setType(HbInputButton::ButtonTypeNormal);
    }

    setButtons(buttons);

    // connect signals from buttons
    QObject::connect(this, SIGNAL(buttonPressed(const QKeyEvent&)),
                     this, SLOT(updateButtonLabels()));
    QObject::connect(this, SIGNAL(buttonReleased(const QKeyEvent&)),
                     this, SLOT(updateButtonLabels()));
    QObject::connect(this, SIGNAL(pressedButtonChanged(const QKeyEvent&,
                                                       const QKeyEvent&)),
                     this, SLOT(updateButtonLabels()));

    // update colors
    updateColorArray();
    // set button texts
    setButtonTexts();
    // set button icons
    button(0)->setIcon(HbIcon(vmbxIcon));

    // update button texts when input language is changed
    connect(HbInputSettingProxy::instance(),
            SIGNAL(globalInputLanguageChanged(HbInputLanguage)),
            this,
            SLOT(setButtonTexts()));
}

DialpadNumericButtonGrid::~DialpadNumericButtonGrid()
{
}

void DialpadNumericButtonGrid::resetButtons()
{
    cancelButtonPress();
}

QChar DialpadNumericButtonGrid::inputCharacter(int keyCode) const
{
    return mGeneratedChar.value(keyCode);
}

void DialpadNumericButtonGrid::setButtonTexts()
{
    HbInputLanguage inputLanguage =
        HbInputSettingProxy::instance()->globalInputLanguage();
    const HbKeymap *keymap =
        HbKeymapFactory::instance()->keymap(inputLanguage.language());

    mGeneratedChar.clear();

    if (keymap) {
        int buttonCount = (DialpadRowCount*DialpadColumnCount);
        for (int i = 0; i < buttonCount; i++) {
            int keyCode = DialpadKeyCodeTable[i];

            if (keyCode == Qt::Key_Asterisk) {
                // asterisk is not localized
                QChar asterisk('*');
                button(i)->setPrimaryLabel(asterisk);
                button(i)->setSecondaryLabel(QLatin1String("+"));
                mGeneratedChar.insert(Qt::Key_Asterisk, asterisk);
                continue;
            }

            if (keyCode == Qt::Key_NumberSign) {
                // number sign is not localized
                QChar numberSign('#');
                button(i)->setPrimaryLabel(numberSign);
                mGeneratedChar.insert(Qt::Key_NumberSign, numberSign);
                continue;
            }

            int index = i;
            if (keyCode==Qt::Key_0) {
                index = i-1;
            }

            const HbMappedKey *keyLabels =
                keymap->keyForIndex(HbKeyboardVirtual12KeyLabels, index);

            //
            // set digits
            // todo: input digit depend on digit type setting
            // QChar numberChar(QLocale::system().zeroDigit().unicode()+
            //                 (keyCode-0x30));
            //
            QChar numberChar;
            if (keyLabels && keyLabels->chars.count()>=1 &&
                keyLabels->chars.at(0).length()) {
                numberChar = keyLabels->chars.at(0).at(0);
            } else {
                // fallback to latin
                numberChar = QChar(keyCode);
            }

            button(i)->setPrimaryLabel(numberChar);
            mGeneratedChar.insert(keyCode,numberChar);

            //
            // set alphabets
            //
            if (keyLabels && keyLabels->chars.count()>=2) {
                button(i)->setSecondaryLabel(keyLabels->chars.at(1));
                if (keyLabels->chars.count()>=4) {
                    button(i)->setSecondaryLabelRow2(keyLabels->chars.at(3));
                } else {
                    button(i)->setSecondaryLabelRow2(QString());
                }
            }
        }

        updateButtonLabels();
    }
}

DialpadNumericButton* DialpadNumericButtonGrid::button(int i) const
{
    return static_cast<DialpadNumericButton*>(HbInputButtonGroup::button(i));
}

void DialpadNumericButtonGrid::updateButtonLabels()
{
    // update numeric buttons according to button state (pressed/released)
    updateIconColor();
    updateTextLayouts(rect().size());
    update();
}

void DialpadNumericButtonGrid::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Paints empty buttons
    HbInputButtonGroup::paint(painter,option,widget);

    qreal cellWidth = boundingRect().width() / gridSize().width();
    qreal cellHeight = boundingRect().height() / gridSize().height();

    for (int i = 0; i < DialpadRowCount * DialpadColumnCount; i++) {
        DialpadNumericButton *item = button(i);

        if (!item->icon().isNull()) {
            // same as secondary text
            qreal x = (item->position().x() * cellWidth) +
                      (DialpadPrimaryTextLeftMargin * mUnit) +
                      (DialpadPrimarySecondaryMargin * mUnit) +
                      mMaxPrimaryLineWidth;
            // top align with some margin
            qreal y = item->position().y() * cellHeight + (cellHeight / 20.0);

            qreal width = item->icon().size().width();
            qreal height = item->icon().size().height();

            Qt::Alignment alignment =
                static_cast<Qt::Alignment>(Qt::AlignTop | Qt::AlignLeft);
            item->icon().paint(painter,
                               QRectF(x,y,width,height),
                               Qt::KeepAspectRatio,
                               alignment);
        }                      
    }

    // Draw texts
    QPen origPen = painter->pen();
    for (int i = 0; i < mTextLayouts.count(); ++i) {
        if (i==SecondaryText) {
            // dimmed in normal state
            painter->setPen(mColors.at(Pressed+1));
        } else {
            // otherwise normal or pressed color
            painter->setPen(mColors.at(i/TextTypeCount));
        }
        mTextLayouts.at(i)->draw(painter, QPointF(0, 0));
    }
    painter->setPen(origPen);
}

void DialpadNumericButtonGrid::updateColorArray()
{
    mColors.clear();

    QColor normalColor = HbColorScheme::color("qtc_input_button_normal");
    mColors.insert(Normal, normalColor);

    QColor pressedColor = HbColorScheme::color("qtc_input_button_pressed");
    mColors.insert(Pressed, pressedColor);

    // this is used for alphabets shown dimmed, use alpha until exact color
    // is specified
    QColor disabledColor = HbColorScheme::color("qtc_input_button_normal");
    disabledColor.setAlpha(DialpadSecondaryTextOpacity*256);
    mColors.insert(Pressed+1, disabledColor);
}

void DialpadNumericButtonGrid::updateIconColor()
{
    for (int i = 0; i < (DialpadRowCount * DialpadColumnCount); i++) {
        DialpadNumericButton *item = button(i);

        if (item->state()==HbInputButton::ButtonStatePressed) {
            item->icon().setColor(mColors.at(Pressed));
        } else {
            item->icon().setColor(mColors.at(Normal));
        }
    }
}

void DialpadNumericButtonGrid::cancelButtonPress()
{
    HbInputButtonGroup::cancelButtonPress();
    updateButtonLabels();
}


void DialpadNumericButtonGrid::setGeometry(const QRectF &rect)
{
    HbInputButtonGroup::setGeometry(rect);

    mUnit = HbDeviceProfile::profile(&mMainWindow).unitValue();

    updateTextLayouts(rect.size());

    updateIconSizes();
}

void DialpadNumericButtonGrid::updateIconSizes()
{
    qreal width =  mMainWindow.orientation() == Qt::Vertical ?
                   DialpadIconSizeV : DialpadIconSizeH;
    width *= mUnit;

    for (int i = 0; i < DialpadRowCount * DialpadColumnCount; i++) {
        DialpadNumericButton *item = button(i);

        if (!item->icon().isNull()) {
            item->icon().setWidth(width);
        }
    }
}

void DialpadNumericButtonGrid::changeEvent(QEvent *event)
{
    HbInputButtonGroup::changeEvent(event);

    if (event->type() == HbEvent::ThemeChanged) {
        updateColorArray();
        updateIconColor();
    }
}

void DialpadNumericButtonGrid::updateTextLayouts(const QSizeF &size)
{
    if (!size.width() && !size.height()) {
        return;
    }

    // get normal and pressed state texts
    QList<QString> textContent;
    bool useTwoRows;

    resolveTextContent(textContent, useTwoRows);

    // layout the texts
    createTextLayouts(size, textContent, useTwoRows);
}

void DialpadNumericButtonGrid::resolveTextContent(
    QList<QString> &content, bool& useTwoRows)
{
    QString normalState;
    QString normalStateSecondary;
    QString pressedState;
    QString pressedStateSecondary;

    useTwoRows = false;

    for (int i = 0; i < (DialpadRowCount*DialpadColumnCount); i++) {
        DialpadNumericButton *item = button(i);
        if (item->state()==HbInputButton::ButtonStatePressed) {
            if (item->primaryLabel().length()) {
                pressedState.append(item->primaryLabel());
                pressedState.append(QChar(QChar::LineSeparator));
            }

            if (item->secondaryLabel().length()) {
                pressedStateSecondary.append(item->secondaryLabel());
                pressedStateSecondary.append(QChar(QChar::LineSeparator));
                if (item->secondaryLabelRow2().length()) {
                    pressedStateSecondary.append(item->secondaryLabelRow2());
                    pressedStateSecondary.append(QChar(QChar::LineSeparator));
                    useTwoRows = true;
                }
            }
        } else { // ButtonStateNormal
            if (item->primaryLabel().length()) {
                normalState.append(item->primaryLabel());
                normalState.append(QChar(QChar::LineSeparator));
            }

            if (item->secondaryLabel().length()) {
                normalStateSecondary.append(item->secondaryLabel());
                normalStateSecondary.append(QChar(QChar::LineSeparator));
                if (item->secondaryLabelRow2().length()) {
                    normalStateSecondary.append(item->secondaryLabelRow2());
                    normalStateSecondary.append(QChar(QChar::LineSeparator));
                    useTwoRows = true;
                }
            }
        }
    }

    content.insert(PrimaryText, normalState);
    content.insert(SecondaryText, normalStateSecondary);
    content.insert(TextTypeCount + Pressed, pressedState);
    content.insert(StateCount + SecondaryText, pressedStateSecondary);
}

void DialpadNumericButtonGrid::createTextLayouts(
    const QSizeF &size, const QList<QString> &content, bool useTwoRows)
{
    // clear old layouts
    qDeleteAll(mTextLayouts);
    mTextLayouts.clear();

    if(!isButtonPressed()) {
        mMaxPrimaryLineWidth = 0;
    }

    QFont primaryFont = HbFontSpec(HbFontSpec::Primary).font();
    qreal primarySize = mMainWindow.orientation() == Qt::Vertical ?
                     DialpadPrimaryTextSizeV :
                     DialpadPrimaryTextSizeH;
    primaryFont.setPixelSize(primarySize * mUnit);

    QFont secondaryFont = HbFontSpec(HbFontSpec::Secondary).font();
    if (useTwoRows) {
        qreal secondarySize = mMainWindow.orientation() == Qt::Vertical ?
                         DialpadSecondaryTextSize2RowsV :
                         DialpadSecondaryTextSize2RowsH;
        secondaryFont.setPixelSize(secondarySize * mUnit);
    } else {
        qreal secondarySize = mMainWindow.orientation() == Qt::Vertical ?
                         DialpadSecondaryTextSizeV :
                         DialpadSecondaryTextSizeH;
        secondaryFont.setPixelSize(secondarySize * mUnit);
    }

    for (int i=0; i < (StateCount*TextTypeCount); i++ ) {
        QString text = content.at(i);

        if (!text.isNull()) {
            QTextLayout* textLayout;
            int type;

            if (i%TextTypeCount) {
                textLayout = new QTextLayout(text,secondaryFont);
                type = SecondaryText;
            } else {
                textLayout = new QTextLayout(text,primaryFont);
                type = PrimaryText;
            }

            mTextLayouts.append(textLayout);

            textLayout->beginLayout();

            int state = (i>=TextTypeCount) ? Pressed : Normal;

            layoutTextLines(size,*textLayout,state,type,useTwoRows);

            textLayout->endLayout();

            textLayout->setCacheEnabled(true);
        }
    }
}

void DialpadNumericButtonGrid::layoutTextLines(
    const QSizeF &size,
    QTextLayout &textLayout,
    int state,
    int type,
    bool useTwoRows)
{
    QFontMetricsF fontMetrics(textLayout.font());
    qreal textHeight = fontMetrics.height();
    qreal textDescent = fontMetrics.descent();

    qreal cellWidth = size.width() / gridSize().width();
    qreal cellHeight = size.height() / gridSize().height();
    qreal maxLineWidth = 0;

    for (int j = 0; j < (DialpadRowCount*DialpadColumnCount); j++) {
        DialpadNumericButton *item = button(j);

        if ((type==PrimaryText && item->primaryLabel().isNull()) ||
            (type==SecondaryText && item->secondaryLabel().isNull())) {
            continue; // no text for this button -> next button
        }

        if ( ( state==Normal &&
               item->state()==HbInputButton::ButtonStateReleased ) ||
             ( state==Pressed &&
               item->state()==HbInputButton::ButtonStatePressed ) ) {
            // layout text line
            if (type==SecondaryText) {
                layoutSecondaryText(textLayout,*item,cellWidth,
                                    cellHeight,textHeight,textDescent,
                                    useTwoRows);
            } else {
                layoutPrimaryText(textLayout,*item,cellWidth,
                                  cellHeight,textHeight);

                // store line width, for drawing secondary text
                qreal lineWidth = fontMetrics.width(item->primaryLabel());
                if (mMaxPrimaryLineWidth == 0 && (j>0 && j<10) &&
                    lineWidth > maxLineWidth) {
                    maxLineWidth = lineWidth;
                    mPrimaryTextBaseline = textHeight/2 - textDescent;
                }
            }
        }
    }

    if (maxLineWidth!=0) {
        mMaxPrimaryLineWidth = maxLineWidth;
    }
}

void DialpadNumericButtonGrid::layoutPrimaryText(
    QTextLayout& layout,
    DialpadNumericButton& button,
    qreal cellWidth,
    qreal cellHeight,
    qreal textHeight) const
{
    QTextLine line = layout.createLine();
    line.setNumColumns(button.primaryLabel().length());
    qreal textPositionX = (button.position().x() * cellWidth) +
                    (DialpadPrimaryTextLeftMargin * mUnit)
                    + buttonBorderSize();
    qreal textPositionY = (button.position().y() +
                    (0.5 * button.size().height())) *
                    cellHeight - (0.5 * textHeight);
    line.setPosition(QPointF(textPositionX, textPositionY));
}

void DialpadNumericButtonGrid::layoutSecondaryText(
    QTextLayout& layout,
    DialpadNumericButton& button,
    qreal cellWidth,
    qreal cellHeight,
    qreal textHeight,
    qreal textDescent,
    bool useTwoRows) const
{
    qreal textPositionX;
    qreal textPositionY;

    QTextLine line = layout.createLine();

    if (useTwoRows) {
        line.setNumColumns(button.secondaryLabel().length());

        textPositionX = (button.position().x() * cellWidth) +
                        (DialpadPrimaryTextLeftMargin * mUnit) +
                        mMaxPrimaryLineWidth +
                        (DialpadPrimarySecondaryMargin * mUnit)
                        + buttonBorderSize();

        if (button.secondaryLabelRow2().length()) {
            textPositionY = (button.position().y() +
                            (0.5 * button.size().height())) *
                            cellHeight;

            QTextLine line2 = layout.createLine();
            line2.setNumColumns(button.secondaryLabelRow2().length());
            line2.setPosition(QPointF(textPositionX,textPositionY-textHeight));
        } else {
            textPositionY = (button.position().y() +
                            (0.5 * button.size().height())) *
                            cellHeight - (0.5 * textHeight);
        }
    } else {
        line.setNumColumns(button.secondaryLabel().length());
        textPositionX = (button.position().x() * cellWidth) +
                        (DialpadPrimaryTextLeftMargin * mUnit) +
                        mMaxPrimaryLineWidth +
                        (DialpadPrimarySecondaryMargin * mUnit)
                        + buttonBorderSize();
        textPositionY = (button.position().y() +
                        (0.5 * button.size().height())) *
                        cellHeight + mPrimaryTextBaseline -
                        (textHeight - textDescent);
    }
    line.setPosition(QPointF(textPositionX, textPositionY));
}

bool DialpadNumericButtonGrid::isButtonPressed() const
{
    bool pressed = false;

    for (int i = 0; i < (DialpadRowCount*DialpadColumnCount); i++) {
        if (button(i)->state()==HbInputButton::ButtonStatePressed) {
            pressed = true;
            break;
        }
    }

    return pressed;
}
