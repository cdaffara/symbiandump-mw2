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

#ifndef UT_PSETWRAPPER_H
#define UT_PSETWRAPPER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CPsetContainer;
class PSetWrapper;

class UT_PSetWrapper : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_PSetWrapper();
    ~UT_PSetWrapper();
    
private slots:

    void init();
    void cleanup();
    void t_construction();
    
    void t_cliWrapper();
    void t_callWaitingWrapper();
    void t_callDivertingWrapper();
    void t_networkWrapper();
    void t_callBarringWrapper();
    void t_exceptionSafety();
    
private:
    
    PSetWrapper* m_setWrapper;
    CPsetContainer *m_psetContainerMock;
};

#endif  // UT_PSETWRAPPER_H
