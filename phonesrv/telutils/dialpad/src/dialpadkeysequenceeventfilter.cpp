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
* Description: Implements key sequence recognition filter for Dialpad.
*
*/
#include <QDebug>
#include <QKeyEvent>
#include <hblineedit.h>
#include <hbstringutil.h>
#ifdef Q_OS_SYMBIAN
#include <xqservicerequest.h>
#include <xqserviceutil.h>
#endif //Q_OS_SYMBIAN
#include "dialpadkeysequenceeventfilter.h"
#include "dialpad.h"
#include "qtphonesrvlog.h"

/*!
  DialpadKeySequenceEventFilter::DialpadKeySequenceEventFilter.
 */
DialpadKeySequenceEventFilter::DialpadKeySequenceEventFilter(
    Dialpad* dialpad, QObject* parent) 
    :
    QObject(parent), mDialpad(dialpad)
{
    PHONE_TRACE;
    
    constructKeySequenceToHandlerMappings();
}


/*!
  DialpadKeySequenceEventFilter::~DialpadKeySequenceEventFilter.
 */
DialpadKeySequenceEventFilter::~DialpadKeySequenceEventFilter()
{
    PHONE_TRACE;
}


/*!
  DialpadKeySequenceEventFilter::eventFilter.
 */
bool DialpadKeySequenceEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    
    const bool eventFiltered = false;
#ifdef Q_OS_SYMBIAN    
    // Code is executed after '#' is pressed as specified in Dialer UI 
    // specification.
    if (QEvent::KeyRelease == event->type()){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (Qt::Key_NumberSign == keyEvent->key())  {        
            QString keySequenceCandidate = HbStringUtil::convertDigitsTo(
                mDialpad->editor().text(), WesternDigit);
            XQAiwInterfaceDescriptor keySequenceHandler = 
                findKeySequenceHandler(keySequenceCandidate);
            if (keySequenceHandler.isValid()) {
                QScopedPointer<XQAiwRequest> request(mAiwMgr.create(
                    keySequenceHandler, 
                    "executeKeySequence(QString)",
                    false));
                request->setSynchronous(true);
                request->setBackground(true);
                QList<QVariant> arguments;
                arguments << keySequenceCandidate;
                request->setArguments(arguments);
                
                QVariant keySequenceProcessed;
                bool requestOk = request->send(keySequenceProcessed);
                if (requestOk && keySequenceProcessed.toBool()) {
                    mDialpad->editor().setText(QString(""));
                }
            }
        }
    }
#else
    Q_UNUSED(event)
#endif // Q_OS_SYMBIAN
    
    return eventFiltered;
}


/*!
  DialpadKeySequenceEventFilter::constructKeySequenceToHandlerMappings.
 */
void DialpadKeySequenceEventFilter::constructKeySequenceToHandlerMappings()
{
    PHONE_TRACE;
    
    QList<XQAiwInterfaceDescriptor> implementations = mAiwMgr.list(
        "com.nokia.symbian.IKeySequenceRecognition", 
        "");
    
    foreach (XQAiwInterfaceDescriptor d, implementations)
    {
        QScopedPointer<XQAiwRequest> request(mAiwMgr.create(
            d,
            "keySequenceValidator()",
            false));
        request->setSynchronous(true);
        request->setBackground(true);
        
        QVariant keySequenceValidator;
        bool requestOk = request->send(keySequenceValidator);
        if (requestOk && keySequenceValidator.toString().size()) {
            QString validator = keySequenceValidator.toString();
            mValidators[validator] = d;
        }
    }
}


/*!
  DialpadKeySequenceEventFilter::findKeySequenceHandler.
 */
XQAiwInterfaceDescriptor DialpadKeySequenceEventFilter::findKeySequenceHandler(
    const QString &keySequenceCandidate) 
{
    PHONE_TRACE;

    XQAiwInterfaceDescriptor keySequenceHandler;
    
    QList<QString> validatorExpressions = mValidators.keys();
    QList<QString>::const_iterator it;
    for (it = validatorExpressions.constBegin(); 
         (it != validatorExpressions.constEnd()) && (!keySequenceHandler.isValid());
         ++it) {
        QString validatorExpression = *it;
        QRegExp expression(validatorExpression);
        if (expression.exactMatch(keySequenceCandidate)) {
            keySequenceHandler = mValidators.value(*it);
        }
    }
    
    return keySequenceHandler;
}
