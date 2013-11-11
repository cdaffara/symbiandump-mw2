/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer interface for conference call
*
*/


#ifndef MCCECONFERENCECALLOBSERVER_H
#define MCCECONFERENCECALLOBSERVER_H

class MCCECall;

#include <ccpdefs.h> // conference call error defenitions

/**
 *  Observer interface for conference call
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCEConferenceCallObserver
    {
public:
    /** Conference events. */
    enum TCCEConferenceCallEvent
      {
      /** A call has been added to the conference. */
      ECCEConferenceCallAdded,   
      /** A call has been removed from the conference. */
      ECCEConferenceCallRemoved, 
      /** Names of the two calls used to build the conference call returned in aCallName. */
      ECCEConferenceBuilt,     
      /** No name provided */
      ECCEConferenceTerminated,  
      /** No name provided. */
      ECCEConferenceSwapped,       
      /** Name of the call being seperated. */
      ECCEConferenceSplit          
      };
        
    /** Conference call State. */
    enum TCCEConferenceCallState
      {
      /** The conference call is in the idle state. */
      ECCEConferenceIdle,
      /** The conference call is in the active, connected state. */
      ECCEConferenceActive,
      /** The conference call is in the held, connected state. */
      ECCEConferenceHold
      };
        
    /** Conference call capabilities. */
    enum TCCEConferenceCallCaps
      {
      /** Indicates that a conference call can be created. */
      KCCECapsCreate = 0x00000001,
      /** Indicates that a conference call can be terminated. */
      KCCECapsHangUp = 0x00000002,
      /** Indicates that a conference call exists and can be swapped to the opposite 
      state (active or hold). */
      KCCECapsSwap = 0x00000004
      };

    /**
    * A ConferenceCall event has occurred.
    * @since S60 3.2
    * @param aEvent Occurred event.
    * @param aReferredCall Pointer to call that the event refers to. 
    * Used e.g. for 
    * MCCEConferenceCall::AddCallL and 
    * MCCEConferenceCall::RemoveCallL
    * Default value is NULL.
    * @return none
    */
    virtual void CallEventOccurred( 
                 const MCCEConferenceCallObserver::TCCEConferenceCallEvent aEvent,
                 MCCECall* aReferredCall=NULL  ) = 0;

    /**
    * Conference state has changed.
    *
    * @since S60 3.2
    * @param aStateContains the state of the conference call
    * @return none
    */
    virtual void CallStateChanged( 
                 const MCCEConferenceCallObserver::TCCEConferenceCallState aState ) = 0;


    /**
    * Conference capabilities have changed.
    *
    * @since S60 3.2
    * @param aCaps Updated call control capabilities
    * @return none
    */
    virtual void CallCapsChanged( 
                 const MCCEConferenceCallObserver::TCCEConferenceCallCaps aCaps ) = 0;
     
    /**
    * An error has occurred regarding the conference.
    *
    * @since S60 3.2
    * @param aError Occurred error code. 
    * @return none
    */
    virtual void ErrorOccurred( TCCPConferenceCallError aError  ) = 0;

    /**
    * Notifies observer the change in call duration.
    *
    * @since S60 v3.2
    * @param aDuration Duration of the call in seconds
    * return none
    */
    virtual void CallDurationChanged( const TTimeIntervalSeconds aDuration ) = 0;
    };




#endif // MCCECONFERENCECALLOBSERVER
