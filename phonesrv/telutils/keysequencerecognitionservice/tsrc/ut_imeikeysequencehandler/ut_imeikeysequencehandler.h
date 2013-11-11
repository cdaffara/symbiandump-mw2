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

#ifndef UT_IMEIKEYSEQUENCEHANDLER_H
#define UT_IMEIKEYSEQUENCEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class ImeiKeySequenceHandler;

class UT_ImeiKeySequenceHandler : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_ImeiKeySequenceHandler();
    ~UT_ImeiKeySequenceHandler();
    
private slots:

    void init();
    void cleanup();

    void t_keySequenceValidator();
    void t_executeValidKeySequence();
    void t_executeInvalidKeySequence();
    
private:
    ImeiKeySequenceHandler *m_handler;
};

#endif  // UT_IMEIKEYSEQUENCEHANDLER_H
