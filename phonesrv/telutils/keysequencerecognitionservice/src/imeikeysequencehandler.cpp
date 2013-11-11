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
* Description: Implements IMEI key sequence handling.
*
*/

#include <qsysteminfo.h>
#include <hbmessagebox.h>
#include <hbdevicemessagebox.h>
#include "imeikeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "keysequencerecognitionservicelog.h"

QTM_USE_NAMESPACE

/*!
  ImeiKeySequenceHandler::ImeiKeySequenceHandler.
 */
ImeiKeySequenceHandler::ImeiKeySequenceHandler(
    QObject* parent)
    :
    KeySequenceHandler(parent),
    m_messageBox(0)
{
    DPRINT_METHODENTRYEXIT;
    
    setKeySequenceValidator(QRegExp::escape(KCodeImei));
}


/*!
  ImeiKeySequenceHandler::~ImeiKeySequenceHandler.
 */
ImeiKeySequenceHandler::~ImeiKeySequenceHandler()
{
    DPRINT_METHODENTRYEXIT;
    
    destroyMessageBox();
}


/*!
  ImeiKeySequenceHandler::executeKeySequence.
 */
bool ImeiKeySequenceHandler::executeKeySequence(
    const QString &keySequence)
{
    DPRINT_METHODENTRYEXIT;
    
    bool handled = true;
    
    if (KCodeImei == keySequence) {
        launchImeiDialog();
    } else {
        handled = false;
    }
    
    return handled;
}


/*!
  ImeiKeySequenceHandler::launchImeiDialog().
 */
void ImeiKeySequenceHandler::launchImeiDialog()
{
    DPRINT_METHODENTRYEXIT;
    
    destroyMessageBox();
    
    QScopedPointer<QSystemDeviceInfo> deviceInfo(new QSystemDeviceInfo()); 
    QString serialNumber = deviceInfo->imei();
    QString serialNumberNoteText = 
        hbTrId("txt_phone_info_serial_no").arg(serialNumber);
    m_messageBox = new HbDeviceMessageBox(
        serialNumberNoteText, 
        HbMessageBox::MessageTypeInformation);
    m_messageBox->setTimeout(HbPopup::NoTimeout);
    
    QObject::connect(
        m_messageBox, SIGNAL(aboutToClose()), 
        this, SLOT(destroyMessageBox()));
    
    m_messageBox->show();
}


/*!
  ImeiKeySequenceHandler::destroyMessageBox.
 */
void ImeiKeySequenceHandler::destroyMessageBox()
{
    DPRINT_METHODENTRYEXIT;
    
    if (m_messageBox) {
        m_messageBox->deleteLater();
        m_messageBox = 0;
    }
}
