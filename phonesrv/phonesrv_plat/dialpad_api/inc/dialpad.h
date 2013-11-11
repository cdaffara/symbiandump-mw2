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

#ifndef DIALPAD_H
#define DIALPAD_H

#include <hbwidget.h>
#include <QTime>
#include <QTimeLine>

class HbLineEdit;
class HbFrameDrawer;
class HbMainWindow;
class DialpadInputField;
class DialpadKeypad;
class DialpadMultitapHandler;
class DialpadBackground;

#ifdef BUILD_DIALPAD
#define DIALPAD_EXPORT Q_DECL_EXPORT
#else
#define DIALPAD_EXPORT Q_DECL_IMPORT
#endif

class DIALPAD_EXPORT Dialpad : public HbWidget
{
    Q_OBJECT

public:
    explicit Dialpad(); // deprecated
    explicit Dialpad(const HbMainWindow& mainWindow);
    virtual ~Dialpad();

    HbLineEdit& editor() const;

    bool isOpen() const;
    
    bool isCallButtonEnabled() const;

public slots:
    void openDialpad();

    void closeDialpad();

    void setCallButtonEnabled(bool enabled);

    void setTapOutsideDismiss(bool dismiss);

protected:
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget);

    void changeEvent(QEvent *event);
               
    void showEvent(QShowEvent *event);

    void hideEvent(QHideEvent *event);

    void closeEvent(QCloseEvent * event);

    void gestureEvent(QGestureEvent *event);

protected slots:
    void closeAnimValueChanged(qreal value);
    void closeAnimFinished();
    void openAnimValueChanged(qreal value);
    void openAnimFinished();
    void orientationChangeStarted();
    void orientationChangeFinished(Qt::Orientation current);

private:
    void startCloseAnimation();
    void layoutBackgroundItem();
    void updateLayout(Qt::Orientation orientation);
    void initialize();

signals:
    void aboutToOpen();
    void aboutToClose();

private:
    const HbMainWindow& mMainWindow;
    HbFrameDrawer *mBackgroundDrawer;
    HbFrameDrawer *mIconDrawer;
    DialpadBackground* mBackgroundItem;
    DialpadInputField* mInputField;
    DialpadKeypad* mKeypad;
    DialpadMultitapHandler* mMultitap;
    QTimeLine mOpenTimeLine;
    QTimeLine mCloseTimeLine;
    bool mAnimationOngoing;
    QPointF mPosition;
    int mCloseHandleHeight;
    int mCloseHandleWidth;
    int mTitleBarHeight;
    int mOrientation;
    bool mIsOpen;

    friend class DialpadBackground;
};

#endif // DIALPAD_H
