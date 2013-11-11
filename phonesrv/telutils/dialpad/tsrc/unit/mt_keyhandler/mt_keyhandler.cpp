/*!
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

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbview.h>
#include <hblineedit.h>
#include <hbinstance.h>

#include "dialpadtest.h"
#include "dialpad.h"
#include "dialpadkeyhandler.h"
#include "dialpadvoicemailboxeventfilter.h"


// test cases
class mt_KeyHandler : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

private:
    HbMainWindow *mMainWindow;
    Dialpad *mDialpad;
    DialpadKeyHandler *mKeyhandler;
};

void mt_KeyHandler::initTestCase()
{
    mMainWindow = new HbMainWindow();
    mDialpad = new Dialpad(*mMainWindow);
    DialpadKeyHandler::DialpadKeyEventFilters filter = 
        DialpadKeyHandler::VoiceMailbox
        | DialpadKeyHandler::VideoMailBox
        | DialpadKeyHandler::Bluetooth
        | DialpadKeyHandler::KeySequence
        | DialpadKeyHandler::EmergencyCall
        | DialpadKeyHandler::Hash;
    mKeyhandler = new DialpadKeyHandler(mDialpad, filter);
    QVERIFY(mKeyhandler != NULL);
}

void mt_KeyHandler::cleanupTestCase()
{
    delete mKeyhandler;
    delete mDialpad;
    delete mMainWindow;
}


DIALPAD_TEST_MAIN(mt_KeyHandler)
#include "mt_keyhandler.moc"
