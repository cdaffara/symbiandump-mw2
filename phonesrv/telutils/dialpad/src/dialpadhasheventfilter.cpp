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
#include <qtimer>
#include <hblineedit.h>
#include <hbnotificationdialog.h>
#include "dialpadhasheventfilter.h"
#include "dialpadsymbianwrapper.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

const QString HashCharacter("#");

DialpadHashEventFilter::DialpadHashEventFilter(Dialpad* dialpad, QObject* parent) :
    DialpadMailboxEventFilterBase(dialpad, parent)
{
}

DialpadHashEventFilter::~DialpadHashEventFilter()
{
}

bool DialpadHashEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    bool keyEventEaten(false);

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);        
        if ((isLongKeyPressSupported(keyEvent->key())) &&
            !(mDialpad->editor().text().length() >= 1)) {
            //Check that there is only one item in dialpad, if there is more than one
            //do not handle long key press.
            mLongPressTimer->stop();
            mLongPressTimer->start(DialpadLongKeyPressTimeOut);
        }
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (isLongKeyPressSupported(keyEvent->key())){
            mLongPressTimer->stop();
        }
    }

    return keyEventEaten;
}

void DialpadHashEventFilter::handleLongKeyPress()
{
    PHONE_TRACE;
    switch(mKeyEvent) {
    case Qt::Key_NumberSign:{
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

bool DialpadHashEventFilter::handleCallButtonPress()
{
    // not supported
    return EFalse;
}

void DialpadHashEventFilter::handleMailboxOperation()
{
    PHONE_TRACE;
    // change KCRUidProfileEngine KProEngSilenceMode value and show note  
    if (mSymbianWrapper->changeSilentModeState()) {
        HbNotificationDialog::launchDialog( 
                hbTrId( "txt_dialer_dpopinfo_silent_mode_activated" ) );
    }
    else {
        HbNotificationDialog::launchDialog( 
                hbTrId( "txt_dialer_dpopinfo_silent_mode_deactivated" ) );
    }
    clearEditor();
}
