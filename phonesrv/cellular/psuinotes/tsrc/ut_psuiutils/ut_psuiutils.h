/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_PSUIUTILS_H
#define UT_PSUIUTILS_H

#include <QtTest/QtTest>
#include <mockservice.h>

class PsUiSettingsWrapper;

class UT_PsUiUtils : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_PsUiUtils();
    ~UT_PsUiUtils();
    
private slots:

    void init();
    void cleanup();
    
    void t_memleak();
    
    void t_errorCodeTextMapping();
    
    void t_isFeatureCallWaitingDistiquishNotProvisionedEnabled();
    void t_isPhoneOffline();
    
private:
    PsUiSettingsWrapper *mWrapper;
    
};

#endif
