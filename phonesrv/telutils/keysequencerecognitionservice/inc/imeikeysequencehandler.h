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

#ifndef IMEIKEYSEQUENCEHANDLER_H
#define IMEIKEYSEQUENCEHANDLER_H

#include "keysequencehandler.h"

class HbDeviceMessageBox;

class ImeiKeySequenceHandler : public KeySequenceHandler
{
    Q_OBJECT

public:

    ImeiKeySequenceHandler(QObject *parent = 0);
    ~ImeiKeySequenceHandler();
    
    bool executeKeySequence(const QString &keySequence);

private:
    void launchImeiDialog();
    
private slots:
    void destroyMessageBox();

private:    
    HbDeviceMessageBox *m_messageBox;
};

#endif // IMEIKEYSEQUENCEHANDLER_H
