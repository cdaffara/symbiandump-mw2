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

#ifndef UT_KEYSEQUENCEHANDLER_H
#define UT_KEYSEQUENCEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>
#include "keysequencehandler.h"

class TestClassDriver;

class TestClassDriver : public KeySequenceHandler
{
public:
    bool executeKeySequence(const QString &keySequence);
    void setKeySequenceValidator(const QString &validator);  
};

class UT_KeySequenceHandler : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_KeySequenceHandler();
    ~UT_KeySequenceHandler();
    
private slots:

    void init();
    void cleanup();
    
    void t_keySequenceValidator();
    void t_setKeySequenceValidator();
    
private:
    TestClassDriver *m_handler;
};

#endif  // UT_KEYSEQUENCEHANDLER_H
