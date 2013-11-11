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

#ifndef PSETCALLWAITINGWRAPPER_H_
#define PSETCALLWAITINGWRAPPER_H_

#include <QObject>
#include <psetwrappertypes.h>

// Forward declarations 
class CPsetContainer;
class CPsetCallWaiting;
class PSetCallWaitingWrapperPrivate;

#ifdef BUILD_PSETWRAPPER
#define PSETWRAPPER_EXPORT Q_DECL_EXPORT
#else
#define PSETWRAPPER_EXPORT Q_DECL_IMPORT
#endif

class PSETWRAPPER_EXPORT PSetCallWaitingWrapper: 
    public QObject
{
    Q_OBJECT

public:
    
    explicit PSetCallWaitingWrapper( CPsetContainer &psetContainer, 
            QObject *parent = NULL);
    
    ~PSetCallWaitingWrapper();


    // Call waiting status
    enum PsCallWaitingStatus
    {
        StatusActive,
        StatusDisabled,
        StatusNotAvailable,
        StatusNotProvisioned,
        StatusUnknown
    };

    // Call waiting action
    enum PsCallWaitingCommand
    {
        ActivateCallWaiting,
        DeactivateCallWaiting,
        CheckCallWaitingStatus
    };
    
public: // Functions (adaptees):  

    /**
    * Sets call waiting to the network.
    *
    * @param aSetting New settings for the Call Waiting.
    * @param aBsc Basic service group concerned.
    */
    void setCallWaiting( PsCallWaitingCommand aSetting,
            int aBasicServiceGroup );

    /**
    * Checks the call waiting status from network.
    */
    void getCallWaitingStatus();

    /**
    * Cancels the call waiting-request process.
    */
    void cancelProcess();
    
    /**
    * Get CPsetCallwaiting reference.
    */
    CPsetCallWaiting &  getCPsetCallWaiting() const;

signals: // Notify via signals     

    void handleCallWaitingGetStatus( 
        PSetCallWaitingWrapper::PsCallWaitingStatus aStatus,
        const QList<unsigned char> &basicServiceGroupIds);
    
    void handleCallWaitingChanged( 
        PSetCallWaitingWrapper::PsCallWaitingCommand aSetting,
        int aResult );
    
    void handleCallWaitingRequesting( bool ongoing, bool interrupted ); 

    void handleCallWaitingError( int aError );

private: // Data: 
    
    // Phone setting handlers, own
    CPsetCallWaiting *m_psetCallWaiting;
    
    // Own
    QScopedPointer<PSetCallWaitingWrapperPrivate> m_privateImpl;
    friend class PSetCallWaitingWrapperPrivate;
};


#endif /* PSETCALLWAITINGWRAPPER_H_ */
