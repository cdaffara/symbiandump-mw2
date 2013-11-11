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
#include <QKeyEvent>
#include <hblineedit.h>
#include <hbstringutil.h>
#include "dialpademergencycalleventfilter.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

#ifdef Q_OS_SYMBIAN
#include <cphcltemergencycall.h>
#include <phclttypes.h>
#endif //Q_OS_SYMBIAN


DialpadEmergencyCallEventFilter::DialpadEmergencyCallEventFilter(Dialpad* dialpad, QObject* parent) :
    QObject(parent), mDialpad(dialpad), mEmergency(0)
{
    PHONE_TRACE;
#ifdef Q_OS_SYMBIAN
    TRAP_IGNORE(mEmergency = CPhCltEmergencyCall::NewL( NULL ));
#endif // Q_OS_SYMBIAN
    mDialpad->setCallButtonEnabled(false);
    connect(&mDialpad->editor(),SIGNAL(contentsChanged()),
            this,SLOT(editorTextChanged()));
}

DialpadEmergencyCallEventFilter::~DialpadEmergencyCallEventFilter()
{
}

bool DialpadEmergencyCallEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    PHONE_TRACE;
    Q_UNUSED(watched)
    
    bool eventFiltered = false;
#ifdef Q_OS_SYMBIAN
       
    const int eventType = event->type();
       
    if (eventType == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        const int keyCode = keyEvent->key();
        if (keyCode == Qt::Key_Yes ||
             keyCode == Qt::Key_Enter) {           
            if (isEmergencyNumber(editorText())) {
                createCall(editorText());
                eventFiltered = true;
            }            
        }
    }
#else
    Q_UNUSED(event)
#endif // Q_OS_SYMBIAN
    
    return eventFiltered;
}

bool DialpadEmergencyCallEventFilter::isEmergencyNumber(QString string)
{
    PHONE_TRACE;
    bool ret = false;
#ifdef Q_OS_SYMBIAN
    if (KPhCltTelephoneNumberLength >= string.length()) {
        TBool emergency;
        TPhCltTelephoneNumber number(reinterpret_cast<const TUint16*>(string.utf16()));
        if (mEmergency && 
           KErrNone == mEmergency->IsEmergencyPhoneNumber(number, emergency)) {
            ret = emergency;
        }               
    }
#else
    Q_UNUSED(string)
#endif // Q_OS_SYMBIAN
    return ret;
}

QString DialpadEmergencyCallEventFilter::editorText()
{
    PHONE_TRACE;
    return HbStringUtil::convertDigitsTo(
                mDialpad->editor().text(), WesternDigit);
}

void DialpadEmergencyCallEventFilter::createCall(const QString &phoneNumber)
{
    PHONE_TRACE2("phoneNumber:", phoneNumber);
#ifdef Q_OS_SYMBIAN
    TPtrC number (static_cast<const TUint16*>(phoneNumber.utf16()),
            phoneNumber.length());
    TRAP_IGNORE(mEmergency->DialEmergencyCallL(number));
#endif // Q_OS_SYMBIAN
}

void DialpadEmergencyCallEventFilter::editorTextChanged()
{
    mDialpad->setCallButtonEnabled(isEmergencyNumber(editorText())); 
}

