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

#ifndef UT_LIFETIMERKEYSEQUENCEHANDLER_H
#define UT_LIFETIMERKEYSEQUENCEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>
#include <etel.h>

class LifeTimerKeySequenceHandler;

class UT_LifeTimerKeySequenceHandler : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_LifeTimerKeySequenceHandler();
    ~UT_LifeTimerKeySequenceHandler();

private:
    void setLifeTimerData(
        TUint32 aKey,
        TInt & aValue );
    void setMobilePhoneCount(
        TInt & aNoOfPhones);
    void setMobilePhoneInfo(
        const TInt aIndex,
        RTelServer::TPhoneInfo & aInfo );
    void setLifeTimeData(
        TDes8& aLifeTimeInfo);
    
private slots:

    void init();
    void cleanup();
    
    void t_constructionFails();
    void t_keySequenceValidator();
    void t_executeValidKeySequence();
    void t_executeInvalidKeySequence();
    void t_executeKeySequenceEtelConnectionCreationFails();
    void t_executeKeySequenceLifeTimeDataQueryFails();
    
private:
    LifeTimerKeySequenceHandler *m_handler;
};

#endif  // UT_LIFETIMERKEYSEQUENCEHANDLER_H
