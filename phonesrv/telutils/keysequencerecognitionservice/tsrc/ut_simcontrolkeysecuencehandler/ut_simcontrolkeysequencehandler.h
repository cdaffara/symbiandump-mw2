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

#ifndef UT_SIMCONTROLKEYSEQUENCEHANDLER_H
#define UT_SIMCONTROLKEYSEQUENCEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class SimControlKeySequenceHandler;

class UT_SimControlKeySequenceHandler : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_SimControlKeySequenceHandler();
    ~UT_SimControlKeySequenceHandler();
    
private slots:

    void init();
    void cleanup();
    void t_constructionFails();
    void t_changePin1();
    void t_changePin2();
    void t_unblockPin1();
    void t_unblockPin2();
    void t_keySequenceValidator();
    
    
private:
    SimControlKeySequenceHandler *m_handler;
};

#endif  // UT_SIMCONTROLKEYSEQUENCEHANDLER_H
