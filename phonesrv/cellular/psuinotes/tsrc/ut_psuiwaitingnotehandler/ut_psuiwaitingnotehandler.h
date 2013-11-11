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

#ifndef UT_PSUIWAITINGNOTEHANDLER_H
#define UT_PSUIWAITINGNOTEHANDLER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CPsetContainer;
class PSetCallWaitingWrapper;
class PsUiWaitingNoteHandler;

class UT_psuiwaitingnotehandler : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_psuiwaitingnotehandler();
    ~UT_psuiwaitingnotehandler();
    
private slots:
    void init();
    void cleanup();
    void t_memleak();
    
    void t_construction();
    void t_handleCallWaitingGetStatus();
    void t_handleCallWaitingChanged();
    void t_handleCallWaitingRequesting();
    void t_handleCallWaitingError();
   
 
private:
    CPsetContainer *m_psetContainer;
    PSetCallWaitingWrapper *m_mockWaitingWrapper;
    PsUiWaitingNoteHandler *m_waitingHandler;
};

#endif // UT_PSUIWAITINGNOTEHANDLER_H
