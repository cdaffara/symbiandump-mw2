/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_DIALPADHASHEVENTFILTER_H
#define UT_DIALPADHASHEVENTFILTER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class Dialpad;
class DialpadHashEventFilter;
class HbLineEdit;
class QTimer;

// helper class
class KeyEventCatcher : public QObject
{
public:
    bool eventFilter(QObject* watched, QEvent * event)
    {
        Q_UNUSED(watched);

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            mKeyPresses.append(keyEvent->key());
            return false;
        } else if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            mKeyReleases.append(keyEvent->key());
            return false;
        }
        return false;
    }

public:
    QList<int> mKeyPresses;
    QList<int> mKeyReleases;
};

class UT_DialpadHashEventFilter : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_DialpadHashEventFilter();
    ~UT_DialpadHashEventFilter();
    
private slots:

    void init();
    void cleanup();
    // void callEventFilter();
    
#ifdef Q_OS_SYMBIAN
    // These are actual hash event filter unit tests
    void testeventFilter();
    void testhandleLongKeyPress();
    void testhandleCallButtonPress();
    void testhandleMailboxOperation();
#endif
    
private:
    Dialpad *m_dialPad;
    DialpadHashEventFilter *m_eventFilter;
    HbLineEdit *m_lineEditMock;
    QTimer *mLongPressTimer;
};

#endif  // UT_DIALPADHASHEVENTFILTER_H
