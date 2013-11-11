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
* Description: Implements bluetooth key sequence handling.
*
*/

#include <qmobilityglobal.h>
#include <btxqserviceapi.h>
#include <QRegExp>
#ifdef Q_OS_SYMBIAN
#include <xqappmgr.h>
#endif //Q_OS_SYMBIAN

#include "keysequencerecognitionservicedefs.h"
#include "keysequencerecognitionservicelog.h"
#include "bluetoothkeysequencehandler.h"

/*!
  BluetoothKeySequenceHandler::BluetoothKeySequenceHandler.
 */
BluetoothKeySequenceHandler::BluetoothKeySequenceHandler(QObject* parent) :
    KeySequenceHandler(parent)
{
    DPRINT_METHODENTRYEXIT;
    
    setKeySequenceValidator(QRegExp::escape(KShowBTAddress) + "|" +
                            QRegExp::escape(KActivateBTTestMode) + "|" +
                            QRegExp::escape(KToggleBTSSPDebugMode));
}

/*!
  BluetoothKeySequenceHandler::~BluetoothKeySequenceHandler.
 */
BluetoothKeySequenceHandler::~BluetoothKeySequenceHandler()
{
    DPRINT_METHODENTRYEXIT;
}


/*!
  BluetoothKeySequenceHandler::executeKeySequence.
 */
bool BluetoothKeySequenceHandler::executeKeySequence(
    const QString &keySequence)
{
    DPRINT_METHODENTRYEXIT;   
    bool handled = true;
    
    if(keySequence == KShowBTAddress) {
        showBluetoothLocalAddress();
    } else if(keySequence == KActivateBTTestMode) {
        activateBluetoothTestMode();
    } else if(keySequence == KToggleBTSSPDebugMode) {
        toggleBluetoothDebugMode();
    } else {
        handled = false;
    }

    return handled;
}

void BluetoothKeySequenceHandler::showBluetoothLocalAddress()
{
    DPRINT_METHODENTRYEXIT;
#ifdef Q_OS_SYMBIAN
    XQApplicationManager appManager;
    QScopedPointer<XQAiwRequest> request(appManager.create(BluetoothServiceName, BluetoothInterfaceShowLocalAddress,
                                                           BluetoothShowLocalAddress, false));
    if (request == NULL) {
        return;
    }
    request->send();
#endif // Q_OS_SYMBIAN	
}

void BluetoothKeySequenceHandler::activateBluetoothTestMode()
{
    DPRINT_METHODENTRYEXIT;
#ifdef Q_OS_SYMBIAN
    XQApplicationManager appManager;
    QScopedPointer<XQAiwRequest> request(appManager.create(BluetoothServiceName, BluetoothInterfaceActivateDutMode,
                                                           BluetoothActivateDutMode, false));
    if (request == NULL) {
        return;
    }
    request->send();
#endif // Q_OS_SYMBIAN		
}

void BluetoothKeySequenceHandler::toggleBluetoothDebugMode()
{
    DPRINT_METHODENTRYEXIT;
#ifdef Q_OS_SYMBIAN
    XQApplicationManager appManager;
    QScopedPointer<XQAiwRequest> request(appManager.create(BluetoothServiceName, BluetoothInterfaceToggleSspDebugMode,
                                                           BluetoothToggleSspDebugMode, false));
    if (request == NULL) {
        return;
    }
    request->send();
#endif // Q_OS_SYMBIAN			
}
