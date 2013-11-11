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
* Description: Implements manufacturer key sequence handling.
*
*/

#include <QDebug>
#include <xqserviceutil.h>
#include <xqservicerequest.h>
#include <hbapplication.h>
#include "manufacturerkeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "keysequencerecognitionservicelog.h"

/*!
  ManufacturerKeySequenceHandler::ManufacturerKeySequenceHandler.
 */
ManufacturerKeySequenceHandler::ManufacturerKeySequenceHandler(
    QObject* parent)
    : 
    KeySequenceHandler(parent),
    m_currentRequest(0)
{
    DPRINT_METHODENTRYEXIT;
    
    setKeySequenceValidator(
        QRegExp::escape(KCodeSwVersion) + "|" +
        QRegExp::escape(KCodeActivateRfsNormal)  + "|" +
        QRegExp::escape(KCodeActivateRfsDeep) + "|" +
        QRegExp::escape(KCodeBtAddress) + "|" +
        QRegExp::escape(KCodeRFLoopback));
    
    m_codeToInterfaceMappings[KCodeSwVersion] = InterfaceDescription(
        "com.nokia.services", "devicemanager", "showVersionNumber()");
    
    m_codeToInterfaceMappings[KCodeActivateRfsNormal] = InterfaceDescription(
        "com.nokia.symbian", "IFactoryReset", "showResetUi()");
    m_codeToInterfaceMappings[KCodeActivateRfsDeep] = InterfaceDescription(
        "com.nokia.symbian", "IFactoryReset", "showResetUi()");
    
    m_codeToInterfaceMappings[KCodeBtAddress] = InterfaceDescription(
        "com.nokia.services", "bluetooth", "showBluetoothDeviceAddress()");
    m_codeToInterfaceMappings[KCodeRFLoopback] = InterfaceDescription(
        "com.nokia.services", "bluetooth", "showBluetoothLoopback()");
}


/*!
  ManufacturerKeySequenceHandler::~ManufacturerKeySequenceHandler.
 */
ManufacturerKeySequenceHandler::~ManufacturerKeySequenceHandler()
{
    DPRINT_METHODENTRYEXIT;
}


/*!
  ManufacturerKeySequenceHandler::executeKeySequence.
 */
bool ManufacturerKeySequenceHandler::executeKeySequence(
    const QString &keySequence)
{
    DPRINT_METHODENTRYEXIT;
    
    bool handled = true;
    
    if (m_codeToInterfaceMappings.contains(keySequence)) {
        handled = issueServiceRequest(m_codeToInterfaceMappings[keySequence]);
    } else {
        handled = false;
    }
    
    return handled;
}


/*!
  ManufacturerKeySequenceHandler::issueServiceRequest.
 */
bool ManufacturerKeySequenceHandler::issueServiceRequest(
    const InterfaceDescription &description)
{
    DPRINT_METHODENTRYEXIT;
    
    bool serviceRequestOk = false;
    if (m_currentRequest.isNull()) {
        const bool isEmbedded = false;
        QScopedPointer<XQAiwRequest> request(m_aiwManager.create(
            description.m_service, 
            description.m_interface, 
            description.m_method, 
            isEmbedded));
        
        if (!request.isNull()) {
            m_currentRequest = request.data();
            
            // Due to a Qt Highway bug in assignment operator implementation we
            // need to set request as asynchronous with a setter function.
            request->setSynchronous(false);
            
            connect(
                request.data(), SIGNAL(requestOk(const QVariant &)), 
                this, SLOT(requestOk(const QVariant &)));
            connect(
                request.data(), SIGNAL(requestError(int, const QString&)), 
                this, SLOT(requestError(int, const QString&)));
            
            serviceRequestOk = request->send();
            if (serviceRequestOk) {
                m_currentRequest = request.take();
            } else {
                // On a controlled error Qt Highway should call requestError,
                // so clean scoped pointer here.
                request.take();
            }
        }
    }
    
    return serviceRequestOk;
}


/*!
  ManufacturerKeySequenceHandler::requestOk.
 */
void ManufacturerKeySequenceHandler::requestOk(
    const QVariant &returnValue)
{
    DPRINT_METHODENTRYEXIT;
    
    Q_UNUSED(returnValue)
    
    delete m_currentRequest;
    m_currentRequest = 0;
}


/*!
  ManufacturerKeySequenceHandler::requestError.
 */
void ManufacturerKeySequenceHandler::requestError(
    int error, const QString& errorMessage)
{
    DPRINT_METHODENTRYEXIT;
    
    Q_UNUSED(error)
    Q_UNUSED(errorMessage)
    
    delete m_currentRequest;
    m_currentRequest = 0;
}
