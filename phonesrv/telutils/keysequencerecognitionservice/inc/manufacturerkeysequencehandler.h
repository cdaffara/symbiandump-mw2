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

#ifndef MANUFACTURERKEYSEQUENCEHANDLER_H
#define MANUFACTURERKEYSEQUENCEHANDLER_H

#include <xqappmgr.h>
#include "keysequencehandler.h"

class XQAiwRequest;

class InterfaceDescription
{
public:
    InterfaceDescription() 
        :
        m_service(""),
        m_interface(""),
        m_method("") {}
    
    InterfaceDescription(
        const QString &service, 
        const QString &interface, 
        const QString &method) 
    :
    m_service(service),
    m_interface(interface),
    m_method(method) {}

public:
    QString m_service;
    QString m_interface;
    QString m_method;
};

class ManufacturerKeySequenceHandler : public KeySequenceHandler
{
    Q_OBJECT

public:

    ManufacturerKeySequenceHandler(QObject *parent = 0);
    ~ManufacturerKeySequenceHandler();
    
    bool executeKeySequence(
        const QString &keySequence);

private:
    bool issueServiceRequest(
        const InterfaceDescription &description);

private slots:
    void requestOk(const QVariant &returnValue);
    void requestError(int error, const QString& errorMessage);

private:
    XQApplicationManager m_aiwManager;
    QMap<QString, InterfaceDescription> m_codeToInterfaceMappings;
    QPointer<XQAiwRequest> m_currentRequest;
};

#endif // MANUFACTURERKEYSEQUENCEHANDLER_H
