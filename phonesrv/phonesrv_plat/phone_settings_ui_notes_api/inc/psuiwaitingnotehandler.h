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

#ifndef PSUIWAITINGNOTEHANDLER_H_
#define PSUIWAITINGNOTEHANDLER_H_

#include <QObject>
#include <QList>
#include <psetcallwaitingwrapper.h>

class PSetCallWaitingWrapper;
class PSCallWaitingCommand;
class PsUiSettingsWrapper;

#ifdef BUILD_PSUINOTES
#define PSUINOTES_EXPORT Q_DECL_EXPORT
#else
#define PSUINOTES_EXPORT Q_DECL_IMPORT
#endif

class PSUINOTES_EXPORT PsUiWaitingNoteHandler : public QObject
{
    Q_OBJECT
    
public:
    PsUiWaitingNoteHandler(PSetCallWaitingWrapper& callWaitingWrapper);
    ~PsUiWaitingNoteHandler();

public slots: // Slots: 

    /**
     * Shows notes for call waiting status interrogation
     * @param status    Status of the call waiting
     * @param basicServiceGroupIds
     */
    void handleCallWaitingGetStatus( 
        PSetCallWaitingWrapper::PsCallWaitingStatus status,
        const QList<unsigned char> &basicServiceGroupIds);
    
    /**
     * Shows notes for call waiting status change request
     * @param setting   Type of the call waiting request  
     * @param result    Result code of the request
     */
    void handleCallWaitingChanged( 
        PSetCallWaitingWrapper::PsCallWaitingCommand setting,
        int result );
    
    /**
     * Handles call waiting requesting notes
     * @param ongoing       If true, there is an active request   
     * @param interrupted   If true, means that requesting note 
     *                      should be cancelled
     */
    void handleCallWaitingRequesting( bool ongoing, bool interrupted ); 
    
    /**
     * Handles call waiting error notes
     * @param error     Error value   
     */
    void handleCallWaitingError( int error );

private:   // data
   
    // For diverting signals
    PSetCallWaitingWrapper& m_callWaitingWrapper;
    
    // Own.
    PsUiSettingsWrapper* m_settingsWrapper;
    
    // Current active note id 
    int m_activeNoteId;
    
    // Current progress note id
    int m_activeProgressNoteId;
   
};

#endif /* PSUIWAITINGNOTEHANDLER_H_ */
