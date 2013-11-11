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
* Description:  Interface for conference call
*
*/


#ifndef MCCECONFERENCECALL_H
#define MCCECONFERENCECALL_H

#include "mcceconferencecallobserver.h"

class MCCECall;

/**
 *  Interface for conference call
 *
 *  
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCEConferenceCall
    {
public:

    /**
    * Returns the service-id used for the conference call..
    * @since S60 3.2
    * @param none
    * @return TUint32 service-id
    */
    virtual TUint32 ServiceId() const = 0;

    /**
    * Releases all bindings to conference call. PE will call this after receiving 
    * MCCEConferenceCallObserver::ECCEConferenceIdle from CCE or when it itself 
    * wants to release the call. In normal sequence called in PE after HangUp is completed.
    * @param none
    * @return KErrNone
    * @pre None Can always be called.
    * @post Plug-in must quarantee all resources for the call will be cleaned up. Call MUST not 
    * be referenced anymore from PE and CCE must not call observer after this call!
    * @since S60 3.2
    */
    virtual TInt Release() = 0;

    /**
    * Adds new call to conference call.
    * @since S60 3.2
    * @param aCall Call to be added to conference.
    * @return none 
    * @leave In case of an error system wide error code
    * @leave KErrAlreadyExists call is already part of conference.
    * @leave KErrNotReady In case conference is not on hold or idle.
    * @pre Conference call state is MCCEConferenceCallObserver::ECCEConferenceHold or 
    * MCCEConferenceCallObserver::ECCEConferenceIdle. 
    * @post Added call is notified calling MCCEConferenceCallObserver::CallEventOccurred.
    * @post If conference becomes active regarding its internal plug-in based logic state is 
    * MCCEConferenceCallObserver::ECCEConferenceActive.
    */
    virtual void AddCallL( MCCECall& aCall ) = 0;
     
    /**
    * Removes call from conference call. 
    * @since S60 3.2
    * @param aCall Call to be removed from conference
    * @return none
    * @leave In case of an error system wide error code
    * @leave KErrNotFound call was not part of conference
    * @leave KErrNotReady In case conference is not connected 
    * (MCCEConferenceCallObserver::ECCEConferenceActive) state.
    * @pre Conference call state is MCCEConferenceCallObserver::ECCEConferenceActive or 
    * MCCEConferenceCallObserver::ECCEConferenceIdle. 
    * @post Removed call is notified calling MCCEConferenceCallObserver::CallEventOccurred.
    * @post If conference becomes idle regarding its internal plug-in based logic state is 
    * MCCEConferenceCallObserver::ECCEConferenceIdle. 
    * 
    */
    virtual void RemoveCallL( MCCECall& aCall ) = 0;

    /**
    * Returns call count of the conference call. 
    *
    * @since S60 3.2
    * @param none 
    * 
    * @return Number of calls in conference
    */
    virtual TInt EnumerateCalls() = 0;
     
    /**
    * Return capabilities of the conference call.
    *
    * @since S60 3.2
    * @param none
    * @return Conference call capabilities.
    */
    virtual MCCEConferenceCallObserver::TCCEConferenceCallCaps Caps() const = 0;
     
    /**
    * End conference call. 
    * Client needs to call Release to notify CCE it has no use for the call anymore.
    *
    * @since S60 3.2
    * param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrAlreadyExists if call is already in idle state
    * @pre Call state is not MCCEConferenceCallObserver::ECCEConferenceIdle
    */
    virtual TInt HangUp() = 0;
     
    /**
    * Allows a client to switch a conference call between "Active" and "Hold" states.
    *
    * @since S60 3.2
    * param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in on-hold state or connected state
    * @pre Call state is MCCEConferenceCallObserver::ECCEConferenceHold or 
    * MCCEConferenceCallObserver::ECCEConferenceActive
    
    */
    virtual TInt Swap() = 0;
     
    /**
    * Private call with one call from conference call.
    * @since S60 3.2
    * @param aCall
    * @return none
    * @leave In case of an error system wide error code
    * @leave KErrNotFound call was not part of conference
    * @post After successful actions conference call state is MCCEConferenceCallObserver::ECCEConferenceHold 
    * (if idle state will not be applied as call count decreases) and private call will become active one. 
    */
    virtual void GoOneToOneL( MCCECall& aCall ) = 0;

    /**
    * Gets conference participants
    * @since S60 3.2
    * @param aCallArray Reference to call array
    * @return Error code
    */
    virtual TInt GetCallArray( RPointerArray<MCCECall>& aCallArray ) = 0;

    /**
    * Sets observer to conference call events. Observer is given when conference 
    * creation request is given. Client is not required to call this. If called will 
    * replace current observer with the new one.
    *
    * @since S60 3.2
    * @param aObserver  Reference to observer interface implementation.
    * @return none
    */
    virtual void SetObserver( const MCCEConferenceCallObserver& aObserver ) = 0;

    /**
    * Returns duration of a conference call. 
    * CCE will count duration from connected state to idle state.
    *
    * @since S60 v3.2
    * @param none
    * @return On return contains the call duration in seconds.
    */
    virtual TTimeIntervalSeconds CallDuration() const = 0;
    };


#endif // MCCECONFERENCECALL

