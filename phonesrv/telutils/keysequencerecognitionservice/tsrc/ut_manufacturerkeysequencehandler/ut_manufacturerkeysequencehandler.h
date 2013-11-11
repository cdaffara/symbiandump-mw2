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

#ifndef UT_MANUFACTURERKEYSEQUENCEHANDLER_H
#define UT_MANUFACTURERKEYSEQUENCEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class ManufacturerKeySequenceHandler;

class UT_ManufacturerKeySequenceHandler : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_ManufacturerKeySequenceHandler();
    ~UT_ManufacturerKeySequenceHandler();
    
private slots:

    void init();
    void cleanup();
    
    void t_keySequenceValidator();
    void t_executeKeySequenceDeviceManagerCodes();
    void t_executeKeySequenceFactoryResetCodes();
    void t_executeKeySequenceBluetoothCodes();
    void t_executeInvalidKeySequence();
    void t_executeKeySequenceRequestDispatchingFails();
    void t_executeKeySequenceSimultaneousRequest();
    
private:
    ManufacturerKeySequenceHandler *m_handler;
};

#endif  // UT_MANUFACTURERKEYSEQUENCEHANDLER_H
