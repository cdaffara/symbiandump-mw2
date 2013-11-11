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

#ifndef UT_SSSETTINGWRAPPER_H
#define UT_SSSETTINGWRAPPER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class SsSettingsWrapper;

class UT_SsSettingsWrapper : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_SsSettingsWrapper();
    ~UT_SsSettingsWrapper();
    
private slots:

    void init();
    void cleanup();
    
    void t_memleak();
    void t_get();
    void t_set();
    
    void t_PhoneSettingChanged();

private:
    SsSettingsWrapper *mWrapper;

};

#endif
