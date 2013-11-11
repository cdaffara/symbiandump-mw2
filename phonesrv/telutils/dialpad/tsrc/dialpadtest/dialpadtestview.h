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
* Description:
*
*/

#ifndef DIALPADTESTVIEW_H
#define DIALPADTESTVIEW_H

#include <hbview.h>

class HbMainWindow;
class HbListWidget;
class Dialpad;
class DialpadKeyHandler;

QT_BEGIN_NAMESPACE
class TestabilityInterface;
QT_END_NAMESPACE

class DialpadTestView : public HbView
{
    Q_OBJECT

public:
    DialpadTestView( HbMainWindow& mainWindow );
    ~DialpadTestView();

protected:
    bool eventFilter(QObject * watched, QEvent * event);
    bool event(QEvent * event);

private:
    void createListWidget();
    void setDialpadPosition();
    void handleDial();
    void loadTestabilityPlugin();

private slots:
    void openDialpad();
    void onDialpadClosed();
    void switchOrientation();
    void onOrientationChange(Qt::Orientation orientation);
    void onEditorContentChanged();
    void handleLongKeyPress();
    void setTapOutsideDismiss();
    void setArabicMode();
    void setArabicDigit();

private:
    HbMainWindow& mMainWindow;
    HbListWidget* mListWidget;
    Dialpad* mDialpad;
    DialpadKeyHandler *mKeyhandler;
    QTimer* mLongPressTimer;
    QString mPressedKey;
    bool mTapOutsideDismiss;
    bool mArabicMode;
    bool mArabicDigit;

    TestabilityInterface* mTestabilityInterface;
};

#endif // DIALPADTESTVIEW_H
