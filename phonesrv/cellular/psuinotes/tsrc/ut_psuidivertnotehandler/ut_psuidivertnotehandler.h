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

#ifndef UT_PSUIDIVERTNOTEHANDLER_H
#define UT_PSUIDIVERTNOTEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class UT_psuidivertnotehandler : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_psuidivertnotehandler();
    ~UT_psuidivertnotehandler();
    
private slots:
    void init();
    void cleanup();
    void t_memleak();
    
    void t_construction();
    void t_handleDivertingChanged();
    void t_handleDivertingStatus();
    void t_handleDivertingError();
    void t_handleCFRequesting();
    
 
private: 
    
};

#endif // UT_PSUIDIVERTNOTEHANDLER_H
