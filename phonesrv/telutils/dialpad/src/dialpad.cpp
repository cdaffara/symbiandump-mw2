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
* Description: Dialpad popup
*
*/

#include <QGraphicsLinearLayout>

#include <hbframedrawer.h>
#include <hbinstance.h>
#include <hbeffect.h>
#include <hbinstance.h>
#include <hbmainwindow.h>
#include <hbstyleloader.h>
#include <hblineedit.h>
#include <hbapplication.h>
#include <hbswipegesture.h>
#include <hbeffect.h>
#include <hbevent.h>

#include "dialpad.h"
#include "dialpadinputfield.h"
#include "dialpadkeypad.h"
#include "dialpadbutton.h"
#include "dialpadmultitaphandler.h"
#include "dialpadbackground.h"
#include "dialpadbutton.h"

static const QLatin1String backgroundGraphicsV("qtg_fr_input_v_bg");
static const QLatin1String backgroundGraphicsH("qtg_fr_input_h_bg");
static const QLatin1String minimizeIconV("qtg_graf_input_v_swipe");
static const QLatin1String minimizeIconH("qtg_graf_input_h_swipe");
static const int DialpadCloseAnimDuration = 200; // ms
static const int DialpadOpenAnimDuration = 200; // ms
static const qreal DialpadComponentMargin = 0.75; // units
static const qreal DialpadCloseHandleHeight = 3.0; // units
static const qreal DialpadCloseHandleWidth = 18.8; // units

static const QLatin1String handsetIcon("qtg_mono_call");
static const QLatin1String vmbxIcon("qtg_mono_voice_mailbox");

const QLatin1String DIALPAD_TO_PRT_FXML(":/dialpad_to_prt.fxml");
const QLatin1String DIALPAD_TO_LSC_FXML(":/dialpad_to_lsc.fxml");
const QLatin1String DIALPAD_TO_PRT_EVENT("prt_activated");
const QLatin1String DIALPAD_TO_LSC_EVENT("lsc_activated");

Dialpad::Dialpad() :
    mMainWindow(*hbInstance->allMainWindows().at(0)),
    mBackgroundItem(0),
    mOpenTimeLine(DialpadOpenAnimDuration),
    mCloseTimeLine(DialpadCloseAnimDuration),
    mAnimationOngoing(false),
    mOrientation(Qt::Vertical),
    mIsOpen(false)
{
    // this constuctor is deprecated
    initialize();
}

Dialpad::Dialpad(const HbMainWindow& mainWindow) :
    mMainWindow(mainWindow),
    mBackgroundItem(0),
    mOpenTimeLine(DialpadOpenAnimDuration),
    mCloseTimeLine(DialpadCloseAnimDuration),
    mAnimationOngoing(false),
    mOrientation(Qt::Vertical),
    mIsOpen(false)
{
    initialize();
}

void Dialpad::initialize()
{
    setObjectName(QLatin1String("dialpad"));
    setFocusPolicy(Qt::StrongFocus);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    // create input field
    mInputField = new DialpadInputField(mMainWindow,this);

    // create keypad
    mKeypad = new DialpadKeypad(mMainWindow,*mInputField,this);

    // layouting params
    qreal unit = HbDeviceProfile::profile(&mMainWindow).unitValue();
    qreal margin = DialpadComponentMargin * unit;
    mCloseHandleHeight = DialpadCloseHandleHeight * unit;
    mCloseHandleWidth = DialpadCloseHandleWidth * unit;

    // create popup background
    mBackgroundDrawer = new HbFrameDrawer();
    mIconDrawer = new HbFrameDrawer();

    // popup layout
    QGraphicsLinearLayout* popupLayout =
        new QGraphicsLinearLayout(Qt::Vertical);
    popupLayout->addItem(mInputField);
    popupLayout->addItem(mKeypad);
    popupLayout->setContentsMargins(margin, mCloseHandleHeight, margin, margin);
    popupLayout->setSpacing(0);
    popupLayout->setItemSpacing(0,margin);
    setLayout(popupLayout);
    setLayoutDirection(Qt::LeftToRight);

    // asterisk multitap handler
    mMultitap = new DialpadMultitapHandler(mInputField->editor(),this);
    mInputField->editor().installEventFilter(mMultitap);

    // close animation
    mCloseTimeLine.setUpdateInterval(16);
    mCloseTimeLine.setEasingCurve(QEasingCurve::InQuad);
    connect(&mCloseTimeLine, SIGNAL(finished()),
            SLOT(closeAnimFinished()));
    connect(&mCloseTimeLine, SIGNAL(valueChanged(qreal)),
            SLOT(closeAnimValueChanged(qreal)));

    // open animation
    mOpenTimeLine.setUpdateInterval(16);
    mOpenTimeLine.setEasingCurve(QEasingCurve::OutQuad);
    connect(&mOpenTimeLine, SIGNAL(finished()),
            SLOT(openAnimFinished()));
    connect(&mOpenTimeLine, SIGNAL(valueChanged(qreal)),
            SLOT(openAnimValueChanged(qreal)));

    // default values
    setPos(0,0);
    setPreferredSize(360,360);
    setZValue(0x00004000); // Hb PopupZValueRangeStart
    hide();
    mMainWindow.scene()->addItem(this);

    // custom button style
    HbStyleLoader::registerFilePath(QLatin1String(":/dialpad.css"));
    HbStyleLoader::registerFilePath(QLatin1String(":/dialpad_color.css"));

    // grab gestures so that those are not passed to widgets behind dialpad
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PinchGesture);

    // effects
    HbEffect::add(this, DIALPAD_TO_PRT_FXML, DIALPAD_TO_PRT_EVENT);
    HbEffect::add(this, DIALPAD_TO_LSC_FXML, DIALPAD_TO_LSC_EVENT);
}

Dialpad::~Dialpad()
{
    delete mBackgroundDrawer;
    delete mIconDrawer;
    delete mBackgroundItem;
}

void Dialpad::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // paint popup background
    if ( mOrientation == Qt::Vertical ) {
        mBackgroundDrawer->setFrameGraphicsName(backgroundGraphicsV);
        mIconDrawer->setFrameGraphicsName(minimizeIconV);
        mBackgroundDrawer->setFrameType(HbFrameDrawer::ThreePiecesVertical);
        mBackgroundDrawer->setBorderWidths(0.0, mCloseHandleHeight, 0.0, 0.0);
    } else {
        mBackgroundDrawer->setFrameGraphicsName(backgroundGraphicsH);
        mIconDrawer->setFrameGraphicsName(minimizeIconH);
        mBackgroundDrawer->setFrameType(HbFrameDrawer::ThreePiecesHorizontal);
        mBackgroundDrawer->setBorderWidths(mCloseHandleHeight, 0.0, 0.0, 0.0);
    }

    // draw background
    QRectF rect = boundingRect();
    mBackgroundDrawer->setFillWholeRect(true);
    mBackgroundDrawer->paint(painter, rect);

    // adjust rectangle to close bar position
    if ( mOrientation == Qt::Vertical ) {
        rect.setLeft((rect.width()- mCloseHandleWidth)/2);
        rect.setWidth(mCloseHandleWidth);
        rect.setHeight(mCloseHandleHeight);
    } else {
        rect.setTop((rect.height() - mCloseHandleWidth)/2);
        rect.setWidth(mCloseHandleHeight);
        rect.setHeight(mCloseHandleWidth);
    }

    mIconDrawer->setFrameType(HbFrameDrawer::OnePiece);
    mIconDrawer->paint(painter, rect);
}

void Dialpad::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LayoutDirectionChange) {
        mBackgroundDrawer->setLayoutDirection(layoutDirection());
    } else if (event->type() == HbEvent::ThemeChanged) {
        mBackgroundDrawer->themeChanged();
        mIconDrawer->themeChanged();
    }
}

bool Dialpad::isOpen() const
{
    return mIsOpen;
}

bool Dialpad::isCallButtonEnabled() const
{
    return mKeypad->isCallButtonEnabled();
}

void Dialpad::openDialpad()
{
    mKeypad->resetButtons();
    
    if (mIsOpen) {
        return;
    }

    connect(&mMainWindow,SIGNAL(aboutToChangeOrientation()),
            SLOT(orientationChangeStarted()));
    connect(&mMainWindow,SIGNAL(orientationChanged(Qt::Orientation)),
            SLOT(orientationChangeFinished(Qt::Orientation)));

    // set offset for open animation
    int previousOrientation = mOrientation;
    mOrientation = mMainWindow.orientation();

    mPosition = pos();
    if (mOrientation==Qt::Vertical) {
        qreal height = geometry().height();
        setPos(mPosition.x(),mPosition.y()+height);
    } else {
        qreal width = geometry().width();
        setPos(mPosition.x()+width,mPosition.y());
    }

    if (mOrientation!=previousOrientation) {
        updateLayout((Qt::Orientation)mOrientation);
        mInputField->updateLayout((Qt::Orientation)mOrientation);
    }

    show();

    setFocusProxy(&mInputField->editor());
    mInputField->editor().setFocus();

    mOpenTimeLine.start();
    mAnimationOngoing = true;
    if (!mInputField->editor().text().isEmpty()) {
        setCallButtonEnabled(true);
    }
    mIsOpen = true;

    emit aboutToOpen();
}

void Dialpad::closeDialpad()
{
    close();

    disconnect(&mMainWindow,SIGNAL(aboutToChangeOrientation()),
               this, SLOT(orientationChangeStarted()));
    disconnect(&mMainWindow,SIGNAL(orientationChanged(Qt::Orientation)),
               this, SLOT(orientationChangeFinished(Qt::Orientation)));

    if (mBackgroundItem) {
        mBackgroundItem->hide();
    }
}    

HbLineEdit& Dialpad::editor() const
{
    return mInputField->editor();
}

void Dialpad::setCallButtonEnabled(bool enabled)
{
    mKeypad->setCallButtonEnabled(enabled);
}

void Dialpad::setTapOutsideDismiss(bool dismiss)
{
    // set dismiss policy before opening dialpad
    Q_ASSERT(!isVisible());

    if (dismiss) {
        mBackgroundItem = new DialpadBackground(*this);
        mBackgroundItem->setZValue(zValue()-1);
        mMainWindow.scene()->addItem(mBackgroundItem);
        qreal chromeHeight = 0;
        hbInstance->style()->parameter(QLatin1String("hb-param-widget-chrome-height"),
                                       chromeHeight);
        mTitleBarHeight = chromeHeight;
    } else {
        delete mBackgroundItem;
        mBackgroundItem = 0;
    }
}

void Dialpad::startCloseAnimation()
{
    if (!mAnimationOngoing) {
        mCloseTimeLine.start();
        mAnimationOngoing = true;
   }
}

void Dialpad::showEvent(QShowEvent *event)
{
    HbWidget::showEvent(event);

    if (mBackgroundItem) {
        layoutBackgroundItem();
        mBackgroundItem->show();
    }
}

void Dialpad::hideEvent(QHideEvent *event)
{
    HbWidget::hideEvent(event);
}

void Dialpad::closeEvent(QCloseEvent * event)
{
    mIsOpen = false;
    HbWidget::closeEvent(event);
    emit aboutToClose();
}

void Dialpad::closeAnimValueChanged(qreal value)
{
    QPointF currentPos = pos();
    QPointF newPos;

    if (mOrientation==Qt::Vertical) {
        qreal height = geometry().height();

        newPos.setX(currentPos.x());
        newPos.setY(mPosition.y()+(height*value));
    } else {
        qreal width = geometry().width();

        newPos.setY(currentPos.y());
        newPos.setX(mPosition.x()+(width*value));
    }

    setPos(newPos);
}


void Dialpad::closeAnimFinished()
{
    mAnimationOngoing = false;
    closeDialpad();
    setPos(mPosition);
}

void Dialpad::openAnimValueChanged(qreal value)
{
    QPointF currentPos = pos();
    QPointF newPos;

    if (mOrientation==Qt::Vertical) {
        qreal height = geometry().height();

        newPos.setX(currentPos.x());
        newPos.setY(mPosition.y()+(height*(1-value)));
    } else {
        qreal width = geometry().width();

        newPos.setY(currentPos.y());
        newPos.setX(mPosition.x()+(width*(1-value)));
    }

    setPos(newPos);
}

void Dialpad::openAnimFinished()
{
    mAnimationOngoing = false;    
    setPos(mPosition);      
}

void Dialpad::orientationChangeStarted()
{
    hide();
}

void Dialpad::orientationChangeFinished(Qt::Orientation current)
{
    updateLayout(current);
    mInputField->updateLayout(current);

    show();

    // run orientation change effect
    if (current==Qt::Horizontal) {
        HbEffect::start(this, DIALPAD_TO_LSC_EVENT);
    } else {
        HbEffect::start(this, DIALPAD_TO_PRT_EVENT);
    }

    mOrientation = current;

    mPosition = pos();

    if (mBackgroundItem) {
        layoutBackgroundItem();
    }
}

void Dialpad::layoutBackgroundItem()
{
    Q_ASSERT(mBackgroundItem);
    QRectF backgroundRect(mMainWindow.layoutRect());
    backgroundRect.adjust(0,mTitleBarHeight,0,0);
    mBackgroundItem->setRect(backgroundRect);
}

void Dialpad::updateLayout(Qt::Orientation orientation)
{
    Q_ASSERT(layout());

    qreal unit = HbDeviceProfile::profile(&mMainWindow).unitValue();
    qreal margin = DialpadComponentMargin * unit;

    QGraphicsLinearLayout* mainLayout =
        static_cast<QGraphicsLinearLayout*>(layout());

    // close handle location changes, update margin values
    if (orientation==Qt::Vertical) {
        mainLayout->setContentsMargins(margin,
                                       mCloseHandleHeight,
                                       margin,
                                       margin);
    } else {
        mainLayout->setContentsMargins(mCloseHandleHeight,
                                       margin,
                                       margin,
                                       margin);
    }
}

void Dialpad::gestureEvent(QGestureEvent *event)
{
    bool closeGesture(false);

    if(HbSwipeGesture *gesture = qobject_cast<HbSwipeGesture*>(
           event->gesture(Qt::SwipeGesture))) {
        if (gesture->state() == Qt::GestureFinished) {
            if ( mOrientation==Qt::Vertical &&
                 gesture->sceneVerticalDirection() == QSwipeGesture::Down ) {
                closeGesture = true;
            } else if (mOrientation==Qt::Horizontal &&
                gesture->sceneHorizontalDirection() == QSwipeGesture::Right) {
                closeGesture = true;
            }
        }
    }

    if (closeGesture) {
        startCloseAnimation();
        event->accept();
    } else {
        event->ignore();
    }
}
