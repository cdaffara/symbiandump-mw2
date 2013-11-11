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

#ifndef UT_PSUINOTES_H
#define UT_PSUINOTES_H

#include <QtTest/QtTest>
#include <mockservice.h>
#include <etelmm.h>
#include <psetcalldivertingwrapper.h>

class HbDialog;

class UT_PsUiNotes : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_PsUiNotes();
    ~UT_PsUiNotes();
    
private: // From QObject
    void timerEvent( QTimerEvent* event );
    
private: // Tools
    HbDialog *visibleDialog();
    void verifyGlobalNote( const QString& noteText );
    void appendAllEtelServices(QList<PSCallDivertingStatus*>& list);
    void appendService(
            QList<PSCallDivertingStatus*>& list, 
            RMobilePhone::TMobileService service);
    
private slots:

    void init();
    void cleanup();
    
    void t_memleak();
    
    void t_showProgressNote();
    void t_showGlobalProgressNote();
    void t_showGlobalNote();
    void t_showGlobalErrorNote();
    void t_showCallDivertDetails();
    void t_showPasswordQueryDialog();
    void t_cancelNote();
    void t_showNotificationDialog();

private:
    int m_noteid1;
    int m_noteid2;
    int m_noteid3;
    int timerid;
    QString wantedNoteText;

};

#endif // UT_PSUINOTES_H 
