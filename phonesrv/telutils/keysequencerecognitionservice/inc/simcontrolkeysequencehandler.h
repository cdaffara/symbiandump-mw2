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
* Description: Implements simcontrol key sequence handling.
*
*/

#ifndef SIMCONTROLKEYSEQUENCEHANDLER_H
#define SIMCONTROLKEYSEQUENCEHANDLER_H

#include "keysequencehandler.h"

class CManualSecuritySettings;

class SimControlKeySequenceHandler : public KeySequenceHandler
{
    Q_OBJECT

public:

    SimControlKeySequenceHandler(QObject *parent = 0);
    ~SimControlKeySequenceHandler();
    
    bool executeKeySequence(const QString &keySequence);
    
    enum SimOperation
        {
        None,
        Pin1,
        Pin2,
        Unblock1,
        Unblock2
        };

private:

    bool parseString(const QString &keySequence);
    void processChangePin(
            SimOperation op,
            const QString &oldPin,
            const QString &newPin,
            const QString &verifyNew);
    
    void processUnblockPin(
            SimOperation op,
            const QString &puk,
            const QString &newPin,
            const QString &verifyNew);
    
private:    
    CManualSecuritySettings* m_securityModel;
};

#endif // SIMCONTROLKEYSEQUENCEHANDLER_H
