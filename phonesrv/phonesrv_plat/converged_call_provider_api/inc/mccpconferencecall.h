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
* Description:  CCP Conference call functionality.
*
*/


#ifndef MCCPCONFERENCECALL_H
#define MCCPCONFERENCECALL_H

//  INCLUDES
#include <e32base.h>
#include <mccpcall.h>

class MCCPConferenceCallObserver; 

/**
* Conference call object.
* 
* @since S60 3.2
*/
class MCCPConferenceCall
    {
protected:
    /** 
    * Protected destructor. Object cannot be deleted from plug-in client (CCE).
    * @since S60 3.2
    */
    virtual inline ~MCCPConferenceCall() {};

public:

    /**
    * Returns the service-id used for the call.
    * @since S60 3.2
    * @param None.
    * @return Service-id of the call.
    */
    virtual TUint32 ServiceId() const = 0;

    /**
    * Ends an ongoing conference call.
    * Does not delete the call. Call deletion is requested calling 
    * CConvergedCallProvider::ReleaseConferenceCall.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None
    * @return KErrNone if request was started succesfully. 
    * @return KErrAlreadyExists if conference is already in idle state.
    * @pre Call state is not MCCPConferenceCallObserver::ECCPConferenceIdle.
    * @since S60 3.2
    */
    virtual TInt HangUp() = 0;

    /**
    * Puts conference call on hold.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None
    * @return KErrNone if request was started succesfully.
    * @return KErrNotReady if conferencel is not in connected state.
    * @pre Call state is MCCPConferenceCallObserver::ECCPConferenceActive.
    * @since S60 3.2
    */
    virtual TInt Hold() = 0;
      
    /**
    * Resumes previously held conference call.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None
    * @return KErrNone if request was started succesfully.
    * @return KErrNotReady if call is not in on-hold state.
    * @pre Call state is MCCPConferenceCallObserver::ECCPConferenceHold.
    * @since S60 3.2
    */
    virtual TInt Resume() = 0;

    /**
    * Swaps a connected conference call between Hold and Resume. 
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in on-hold state or connected state
    * @pre Call state is MCCPConferenceCallObserver::ECCPConferenceHold or 
    * MCCPConferenceCallObserver::ECCPConferenceActive
    * @since S60 3.2
    */
    virtual TInt Swap() = 0;

    /**
    * Adds a new call to conference call. 
    * When plug-in regards the call as added it will notify observer about it 
    * with call added notification MCCPConferenceCallObserver::ECCPConferenceCallAdded.
    * 
    * @since S60 3.2
    * @param aCall Call to be added to conference.
    * @return None
    * @leave In case of an error system wide error code.
    * @leave KErrAlreadyExists call is already part of conference.
    * @leave KErrNotReady In case conference is not on hold or idle.
    * @pre Conference call state is MCCPConferenceCallObserver::ECCPConferenceHold or 
    * MCCPConferenceCallObserver::ECCPConferenceIdle. 
    * @post Added call is notified calling MCCPConferenceCallObserver::ConferenceCallEventOccurred.
    * @post If conference becomes active regarding its internal plug-in based logic state is 
    * MCCPConferenceCallObserver::ECCPConferenceActive.
    */
    virtual void AddCallL( MCCPCall* aCall ) = 0;

    /**
    * Removes a call from conference call. Can be called on any state.
    * When plug-in regards the call as removed it will notify observer about it 
    * with call removed notification MCCPConferenceCallObserver::ECCPConferenceCallRemoved.
    * @since S60 3.2
    * @param aCall Call to be removed from conference.
    * @return None
    * @leave In case of an error system wide error code.
    * @leave KErrNotFound call was not part of conference.
    * @leave KErrNotReady In case conference is not connected (MCCPConferenceCallObserver::ECCPConferenceActive) state.
    * @pre Conference call state is MCCPConferenceCallObserver::ECCPConferenceActive or 
    * MCCPConferenceCallObserver::ECCPConferenceIdle. 
    * @post Removed call is notified calling MCCPConferenceCallObserver::ConferenceCallEventOccurred.
    * @post If conference becomes idle regarding its internal plug-in based logic state is 
    * MCCPConferenceCallObserver::ECCPConferenceIdle. 
    */
    virtual void RemoveCallL( MCCPCall* aCall ) = 0;

    /**
    * Returns number of calls active calls in the conference.
    * @since S60 3.2
    * @param None
    * @return aCallCount Number of calls in conference.
    */
    virtual TInt CallCount() const = 0;

    /**
    * Switch to a private call with given call that is part of conference. When call is regared 
    * as removed from conference it will be notified using 
    * MCCPConferenceCallObserver::ECCPConferenceCallRemoved.
    * @since S60 3.2
    * @param aCall Call to go one-to-one with.
    * @return None
    * @leave KErrNotFound call was not part of conference.
    * @post After successful actions conference call state is MCCPConferenceCallObserver::ECCPConferenceHold 
    * and private call will become active one. 
    */
    virtual void GoOneToOneL( MCCPCall& aCall ) = 0;

    /**
    * Add all current calls in the plug-in to conference. Each succesfully added call will 
    * be separately notified using MCCPConferenceCallObserver::ECCPConferenceCallAdded.
    * If call is not added to conference it is not notified and can be regareded as not 
    * part of conference
    * @since S60 3.2
    * @param None
    * @return None
    * @leave In case of an error system wide error code.
    */
    virtual void CurrentCallsToConferenceL() = 0;
    
    /**
    * Gets conference participants
    * @since S60 3.2
    * @param aCallArray Reference to call array
    * @return Error code
    */
    virtual TInt GetCallArray( RPointerArray<MCCPCall>& aCallArray ) = 0;
    
    /**
    * Add an observer for conference call related events.
    * Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer to be added
    * @return None
    * @leave System error if observer adding fails.
    */
    virtual void AddObserverL( const MCCPConferenceCallObserver& aObserver ) = 0;

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer for conference.
    * @return KErrNone if removed succesfully.
    * @return KErrNotFound if observer was not found.
    */
    virtual TInt RemoveObserver( const MCCPConferenceCallObserver& aObserver ) = 0;
    };

#endif // MCCPCONFERENCECALL_H

