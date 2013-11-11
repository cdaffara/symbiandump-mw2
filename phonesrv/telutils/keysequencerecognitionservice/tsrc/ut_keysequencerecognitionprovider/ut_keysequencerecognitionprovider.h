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

#ifndef UT_KEYSEQUENCERECOGNITIONPROVIDER_H
#define UT_KEYSEQUENCERECOGNITIONPROVIDER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class KeySequenceRecognitionProvider;

class UT_KeySequenceRecognitionProvider : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_KeySequenceRecognitionProvider();
    ~UT_KeySequenceRecognitionProvider();
    
private slots:

    void init();
    void cleanup();
    
    void t_setupLocalization();
    void t_keySequenceValidator();
    void t_keySequenceValidatorEmptyString();
    void t_executeValidKeySequence();
    void t_executeInvalidKeySequence();
    
private:
    KeySequenceRecognitionProvider *m_provider;
};

#endif  // UT_KEYSEQUENCERECOGNITIONPROVIDER_H
