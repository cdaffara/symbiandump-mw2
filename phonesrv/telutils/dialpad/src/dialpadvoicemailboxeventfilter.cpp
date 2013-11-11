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
#include "dialpadvoicemailboxeventfilter.h"
#include "dialpadsymbianwrapper.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

#ifdef Q_OS_SYMBIAN
#include <xqservicerequest.h>
#include <xqserviceutil.h>
#endif //Q_OS_SYMBIAN

const QString VmbxCharacter("1");

DialpadVoiceMailboxEventFilter::DialpadVoiceMailboxEventFilter(Dialpad* dialpad, QObject* parent) :
    DialpadMailboxEventFilterBase(dialpad, parent)
{
}

DialpadVoiceMailboxEventFilter::~DialpadVoiceMailboxEventFilter()
{
}

bool DialpadVoiceMailboxEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    bool keyEventEaten(false);
    
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (checkIfSendEventAndConsumeEvent(keyEvent->key(), event->type())) {
            keyEventEaten = true;
        } else if ((isLongKeyPressSupported(keyEvent->key())) &&
                !(mDialpad->editor().text().length() >= 1)) {
            //Check that there is only one item in dialpad, if there is more than one
            //do not handle long key press.
            mLongPressTimer->stop();
            mLongPressTimer->start(DialpadLongKeyPressTimeOut);
        }
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (checkIfSendEventAndConsumeEvent(keyEvent->key(), event->type())) {
            keyEventEaten = true;
        } else if (isLongKeyPressSupported(keyEvent->key())){
            mLongPressTimer->stop();
        }
    }

    return keyEventEaten;
}

void DialpadVoiceMailboxEventFilter::handleLongKeyPress()
{
    PHONE_TRACE;
    switch(mKeyEvent) {
    case Qt::Key_1:{
       handleMailboxOperation();
       break;
       }
   default:
       // Do nothing.
       break;
    }

    // Reset key code.
    mKeyEvent = NULL;
}

bool DialpadVoiceMailboxEventFilter::handleCallButtonPress()
{
    PHONE_TRACE;
    bool callButtonhandled(false);
    if (!mDialpad->editor().text().isEmpty()) {
#ifdef Q_OS_SYMBIAN
        // check if editor has '1' character if does then
        // get MailboxNumber.
        QString editorContent = HbStringUtil::convertDigitsTo(
            mDialpad->editor().text(), WesternDigit);
        if (VmbxCharacter==editorContent) {
            handleMailboxOperation();
            callButtonhandled = true;
        }
#endif //Q_OS_SYMBIAN
    }
    return callButtonhandled;
}

void DialpadVoiceMailboxEventFilter::handleMailboxOperation()
{
    PHONE_TRACE;
    QString mailboxNumber;
    int error = mSymbianWrapper->getMailboxNumber(mailboxNumber);
    // If here is no vmbx number and dialpad must start vmbx number definition procedures.
    if (DialpadErrorNone != error || mailboxNumber.length() == 0) {
        mDialpad->closeDialpad();
        // If define mailbox query was interupted than reopen dialpad.
        error = mSymbianWrapper->defineMailboxNumber(mailboxNumber);
        if (DialpadErrorCancel == error) {
            mDialpad->openDialpad();
        }
    }
    // Valid vmbx number found or defined and there vmbx didnt
    // return error values then create a call.
    if ((DialpadErrorNone == error) &&
        (mailboxNumber.length() != 0)) {
        createCall(mailboxNumber);
        clearEditor();
        mDialpad->openDialpad();
    }
}
