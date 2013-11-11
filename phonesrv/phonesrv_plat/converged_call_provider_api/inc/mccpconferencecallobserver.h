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
* Description:  CCP Conference Call observer
*
*/


#ifndef MCCPCONFERENCECALLOBSERVER_H
#define MCCPCONFERENCECALLOBSERVER_H

class MCCPCall;

// INCLUDES
#include <e32base.h>

#include <ccpdefs.h> // conference call error defenitions

/**
* CCP API call ConferenceCall observer class
*
*  @since S60 3.2
*/
class MCCPConferenceCallObserver
    {
public: 
    enum TCCPConferenceCallEvent
       {
       /** Conference call added. */
       ECCPConferenceCallAdded,
       /** Conference call removed */
       ECCPConferenceCallRemoved,
       /** Names of the two calls used to build the conference call returned in aCallName. */
       ECCPConferenceBuilt,
       /** No name provided */
       ECCPConferenceTerminated,
       /** No name provided. */
       ECCPConferenceSwapped,
       /** Name of the call being seperated. */
       ECCPConferenceSplit
       };


    /** Conference call status. */
    enum TCCPConferenceCallState
       {
       /** The conference call is in the idle state. */
       ECCPConferenceIdle,
       /** The conference call is in the active, connected state. */
       ECCPConferenceActive,
       /** The conference call is in the held, connected state. */
       ECCPConferenceHold,
       };

    /** Conference call capabilities. */
    enum TCCPConferenceCallCaps
      {
      /** Indicates that a conference call can be created. */
      ECCPCapsCreate = 0x00000001,
      /** Indicates that a conference call can be terminated. */
      ECCPCapsHangUp = 0x00000002,
      /** Indicates that a conference call exists and can be swapped to the opposite 
      state (active or hold). */
      ECCPCapsSwap = 0x00000004
      };



protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual inline ~MCCPConferenceCallObserver() {};
    
public:

    /**
    * The state of the conference call has changed.
    * @since S60 3.2
    * @param aState Conference call state.
    * @return none
    */
    virtual void ConferenceCallStateChanged( 
                 const MCCPConferenceCallObserver::TCCPConferenceCallState aState ) = 0;

    /**
    * A ConferenceCall event has occurred.
    * @since S60 3.2
    * @param aEvent Occurred event.
    * @param aReferredCall Pointer to call that the event refers to. 
    * Used e.g. for 
    * MCCPConferenceCall::AddCallL and 
    * MCCPConferenceCall::RemoveCallL
    * Default value is NULL.
    * @return none
    */
    virtual void ConferenceCallEventOccurred( 
                 const MCCPConferenceCallObserver::TCCPConferenceCallEvent aEvent,
                 MCCPCall* aReferredCall=NULL ) = 0;
                 
    /**
    * Notifies changed conference call capabilities.
    * @since S60 3.2
    * @param aCaps New capabilities for the conference call.
    * @return none
    */
    virtual void ConferenceCallCapsChanged( 
                 const MCCPConferenceCallObserver::TCCPConferenceCallCaps aCaps ) = 0;
    
    /**
    * An error has occurred concerning a conference call.
    * @since S60 3.2
    * @param aError Error code.
    * @return none
    */
    virtual void ErrorOccurred( TCCPConferenceCallError aError ) = 0;
    };

#endif // MCCPCONFERENCECALLOBSERVER_H

// End of File
