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
* Description: Implements Life Timer key sequence handling.
*
*/

#ifndef LIFETIMERKEYSEQUENCEHANDLER_H
#define LIFETIMERKEYSEQUENCEHANDLER_H

#include <centralrepository.h>
#include "keysequencehandler.h"

class HbDeviceMessageBox;
class RTelServer;
class RMobilePhone;
class RMmCustomAPI;

class LifeTimerKeySequenceHandler : public KeySequenceHandler
{
    Q_OBJECT

public:

    LifeTimerKeySequenceHandler(QObject *parent = 0);
    ~LifeTimerKeySequenceHandler();
    
    bool executeKeySequence(const QString &keySequence);

private:
    void launchLifeTimerDialog();
    QString constructLifeTimerData();
    
    void openEtelConnectionL(
        RTelServer &telephonyServer,
        RMobilePhone &mobilePhone,
        RMmCustomAPI &mmCustomApi);
    void closeEtelConnection(
        RTelServer &telephonyServer,
        RMobilePhone &mobilePhone,
        RMmCustomAPI &mmCustomApi);
    
    bool lifeTimerFeatureEnabled();
    
private slots:
    void destroyMessageBox();

private:    
    HbDeviceMessageBox *m_messageBox;
    QScopedPointer<CRepository> m_repository;
};

#endif // LIFETIMERKEYSEQUENCEHANDLER_H
