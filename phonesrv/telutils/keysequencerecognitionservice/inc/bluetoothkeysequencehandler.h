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

#ifndef BLUETOOTHKEYSEQUENCEHANDLER_H
#define BLUETOOTHKEYSEQUENCEHANDLER_H

#include "keysequencehandler.h"

class BluetoothKeySequenceHandler : public KeySequenceHandler
{
    Q_OBJECT

public:
    BluetoothKeySequenceHandler(QObject *parent = 0);
    ~BluetoothKeySequenceHandler();
    
    bool executeKeySequence(const QString &keySequence);

private:
    void showBluetoothLocalAddress();
    void activateBluetoothTestMode();
    void toggleBluetoothDebugMode();
};

#endif // BLUETOOTHKEYSEQUENCEHANDLER_H
