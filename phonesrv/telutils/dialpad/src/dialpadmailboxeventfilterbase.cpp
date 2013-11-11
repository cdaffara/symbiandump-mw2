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
#include <QTimer>
#include <QKeyEvent>
#include <hblineedit.h>
#include <hbstringutil.h>
#include "dialpadmailboxeventfilterbase.h"
#include "dialpadsymbianwrapper.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

#ifdef Q_OS_SYMBIAN
#include <xqservicerequest.h>
#include <xqserviceutil.h>
#endif //Q_OS_SYMBIAN

const int DialpadLongKeyPressButtonCount(3);
static const int DialpadLongKeyPressSupportingButtons[DialpadLongKeyPressButtonCount] =
{ Qt::Key_1, Qt::Key_2, Qt::Key_NumberSign };


DialpadMailboxEventFilterBase::DialpadMailboxEventFilterBase(Dialpad* dialpad, QObject* parent) :
    QObject(parent), mDialpad(dialpad), mKeyEvent(NULL)
{
    PHONE_TRACE;
    mLongPressTimer = new QTimer(this);
    mLongPressTimer->setSingleShot(true);
    connect(mLongPressTimer,SIGNAL(timeout()),this,SLOT(handleLongKeyPress()));
    mSymbianWrapper = new DialpadSymbianWrapper(this);
}

DialpadMailboxEventFilterBase::~DialpadMailboxEventFilterBase()
{
}

bool DialpadMailboxEventFilterBase::checkIfSendEventAndConsumeEvent(const int pressedKey, const int eventType)
{
    PHONE_TRACE4("pressedKey:", pressedKey, "eventType:", eventType);
    bool sendKeyHandled(false);
    // first check that pressed key is send key.
    if (pressedKey == Qt::Key_Yes ||
        pressedKey == Qt::Key_Enter) {
       if (eventType == QEvent::KeyPress) {
           sendKeyHandled = handleCallButtonPress();
       } else if ((eventType == QEvent::KeyRelease) &&
                  (!mDialpad->editor().text().isEmpty())) {
           sendKeyHandled = true;
       }
    }
    return sendKeyHandled;
}

bool DialpadMailboxEventFilterBase::isLongKeyPressSupported(const int key)
{
    PHONE_TRACE2("key:", key);
    bool longKeySupport(false);
    // check if dialpad button is pressed.
    for (int i = 0; i < DialpadLongKeyPressButtonCount; i++) {
        if (key==DialpadLongKeyPressSupportingButtons[i]) {
            longKeySupport = true;
            // Save key code for handleCallButtonPress.
            mKeyEvent = key;
        }
    }
    return longKeySupport;
}

void DialpadMailboxEventFilterBase::clearEditor()
{
    PHONE_TRACE;
    // Erase data from dialpad editor.
    mDialpad->editor().setText(QString(""));
}

void DialpadMailboxEventFilterBase::createCall(const QString &phoneNumber, bool createVideoCall)
{
    PHONE_TRACE2("phoneNumber:", phoneNumber);
#ifdef Q_OS_SYMBIAN
    if(createVideoCall) {
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dialVideo(QString)", false);
        snd << phoneNumber;
        QVariant retValue;
        snd.send(retValue);    	
    } else {
        XQServiceRequest snd("com.nokia.symbian.ICallDial","dial(QString)", false);
        snd << phoneNumber;
        QVariant retValue;
        snd.send(retValue);
    }
#endif // Q_OS_SYMBIAN
}
