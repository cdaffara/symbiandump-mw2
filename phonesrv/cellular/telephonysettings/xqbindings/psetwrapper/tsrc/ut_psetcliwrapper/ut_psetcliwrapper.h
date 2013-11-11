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

#ifndef UT_PSETCLIWRAPPER_H
#define UT_PSETCLIWRAPPER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CPsetContainer;
class PSetCliWrapper;
class CPsetCli;

class UT_PSetCliWrapper : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_PSetCliWrapper();
    ~UT_PSetCliWrapper();
    
private slots:

    void init();
    void cleanup();
    
    void t_construction();
    void t_getColpMode();
    void t_getClipMode();
    void t_getClirMode();
    void t_getColrMode();
    void t_getCnap();
    void t_cancelAll();
    
    // signal emitting/private implementation tests
    void t_handleCliRequesting();
    void t_cliInformation();
    void t_handleCliStatus();
    void t_handleCnapStatus();
    void t_handleCliError();
    void t_setEngineContact();
    
    void t_exceptionSafety();
    
private:
    
    void SimulateLeaveAtMockMethodCallL();
    
private:
    
    int m_numOfLeaveSimulations;
    CPsetContainer *m_psetContainerMock;
    CPsetCli* m_cliMock;
    PSetCliWrapper* m_wrapper;
};

#endif  // UT_PSETCLIWRAPPER_H
