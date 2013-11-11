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

#ifndef UT_BLUETOOTHKEYSEQUENCEHANDLER_H
#define UT_BLUETOOTHKEYSEQUENCEHANDLER_H

#include <QtTest/QtTest>

class BluetoothKeySequenceHandler;

class UT_BluetoothKeySequenceHandler : public QObject
{
    Q_OBJECT

public:    
    
    UT_BluetoothKeySequenceHandler();
    ~UT_BluetoothKeySequenceHandler();
    
private slots:

    void init();
    void cleanup();
    void testShowBTLocalAddressKeySequence();
    void testActivateBTDutModeKeySequence();
    void testToggleBTSSPDebugModeKeySequence();
    void testInvalidKeySequence();
    
private:
    BluetoothKeySequenceHandler *m_handler;
};

#endif  // UT_BLUETOOTHKEYSEQUENCEHANDLER_H
