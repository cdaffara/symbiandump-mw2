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
* Description:  Main interface class 
*
*/


#ifndef CCCE_H
#define CCCE_H

#include <e32base.h>

#include "mccecall.h"
#include "ccpdefs.h"

class MCCEObserver;
class MCCESsObserver;

class MCCECallObserver;

class MCCEConferenceCall;
class MCCEConferenceCallObserver;

class MCCEDtmfInterface;
class MCCEDtmfObserver;

class MCCEExtensionInterface;
class MCCEExtensionObserver;

class CCCEEmergencyCall;
class CCCEPluginManager;
class CCCECallContainer;
class CCCETransferController;

class CCCEObserverAsynchroniser;

/** Hold fail errorcode */
const TInt KCCEErrHoldFail = -37123;

/** Max number of call objects */
const TInt KCCECallObjects = 8;

// Max number of active calls
const TInt KCCEMaxNumberOfCalls = 3;   // = 1 active, 1 onhold, 1 waiting

// defined in RMmCustomAPI.h, check if already defined 
#ifndef RMMCUSTOMAPI_H 
/** defenition for OG, most likely will be deprecated later on */
///_LIT(KTerminateAllCalls,"ALL_CALLS");
#include <rmmcustomapi.h> // Add reference there, cs classes need that too 
#endif

/**
 *  Converged Call Engine
 *
 *  The purpose of the Converged Call Engine (CCE) subsystem 
 *  is to map the call management requests between the Phone 
 *  Engine and Call Provider plug-ins, which can implement 
 *  the requests using some specific signaling protocol, such
 *  as SIP.
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class CCCE :  public CBase
    {
public:
    /**
    * Creates an instance of the CCCE component.
    * @param none     
    * @return new CCCE object pointer 
    * @leave If creating fails system error code
    * @since S60 3.2
    */
    IMPORT_C static CCCE* NewL();

    /**
    * Creates an instance of the CCCE component.
    * @param none     
    * @return new CCCE object pointer 
    * @leave If creating fails system error code
    * @since S60 3.2
    */
    IMPORT_C static CCCE* NewLC();

    /**
    * Creates an instance of the CCCE component.
    * @param aObserver Reference to CCCE observer
    * @return new CCCE object pointer 
    * @leave If creating fails system error code
    * @since S60 3.2
    */
    IMPORT_C static CCCE* NewL( MCCEObserver& aObserver );

    /**
    * Creates an instance of the CCCE component.
    * @param aObserver Reference to CCCE observer
    * @return new CCCE object pointer 
    * @leave If creating fails system error code
    * @since S60 3.2
    */
    IMPORT_C static CCCE* NewLC( MCCEObserver& aObserver );


    /**
    * Destructor
    *
    * @since S60 3.2
    */
    virtual ~CCCE();

public:
    /**
    * Sets observer for CCCE. Repleaces any existing observer set.
    * Called right after creating CCCE object.
    * @since S60 3.2
    * @param aObserver Reference to CCCE observer
    * @param aSsObserver Reference to supplementary services observer.
    * @return none
    */
    IMPORT_C void SetObserver( MCCEObserver& aObserver,
                               MCCESsObserver& aSsObserver );

    /**
    * Opens new call. 
    *
    * @since S60 3.2
    * @param aAddress Address / number to call
    * @param aCallParameters Call parameter struct containing call information 
    * such as Service-Id, call typeand  line type.
    * @param aCallObserver Observer for the created call.
    * @return On return contains the reference to the MCCECall
    * @pre Initialize has been called and observer has been set
    * @leave KErrAlreadyExists - call already exists 
    * @leave KErrNotReady - temporarily not able to create the requested call.
    * @leave KErrNoMemory - memory error
    * @leave KErrNotSupported - call not supported
    * @leave KErrArgument - illegal recipient address or service id
    * @leave Other general system error
    */
    IMPORT_C MCCECall& OpenNewCallL( const TDesC& aAddress,
                                     const CCCECallParameters& aCallParameters,
                                     const MCCECallObserver& aCallObserver );

    /**
    * Opens and initializes the emergency call session. Emergency call instance is created  
    * when creating CCCE instance.
    *
    * @since S60 3.2
    * @param aCallObserver Observer handle.
    * @return On return contains the reference to the MCCECall interface
    * @pre Initialize has been called and observer has been set
    */
    IMPORT_C MCCECall& InitializeEmergencyCall( const MCCECallObserver& aCallObserver );

    /**
    * Retrieves the current airtime duration of the calls
    *
    * @since S60 3.2
    * @param aCSDuration Airtime duration of all CS calls.
    * @param aPSDuration Airtime duration of the PS calls.
    * @return none
    */
    IMPORT_C void GetAirTimeDuration( TTimeIntervalSeconds& aCSDuration, 
                                      TTimeIntervalSeconds& aPSDuration  );

    /**
    * Terminates all the existing calls on the phone. 
    * Results are returned via each call observer.
    *
    * @since S60 3.2
    * @return none
    */
    IMPORT_C void TerminateAllCalls();
    
    /**
    * Terminates all the existing but calls in Ringing or Queued state.
    * Results are returned via each call observer.
    * @since S60 3.2
    * @return none
    */
    IMPORT_C void TerminateAllButRingingAndQueuedCalls();
    
    /**
    * Terminates a call synchronously. For OG PE.
    * To be deprecated later on.
    * NOT SUPPORTED at the moment
    * @since S60 3.2
    * @param aCallName call name
    * @return Error value
    */
    IMPORT_C TInt TerminateCall( TName& aCallName );

    /** Terminates a call synchronously. For OG PE.
    * To be deprecated later on.
    * NOT SUPPORTED at the moment, changed to TInt -> TRequestStatus is removed
    * @since S60 3.2
    * @param aState Request State
    * @param aCallName call name
    * @return none
    */
    IMPORT_C void TerminateCall( TInt& aState,
                                 TName& aCallName );

    /**
    * Gets conference call interface.
    *
    * @since S60 3.2
    * @param aObserver  Reference to observer interface implementation
    * @leave KErrNotReady temporarily cannot create the call.  
    * @leave System wide error code if unsuccesful
    */
    IMPORT_C MCCEConferenceCall& ConferenceCallL( const MCCEConferenceCallObserver& aObserver );

    /**
    * Gets interface for Dtmf handling
    *
    * @since S60 3.2
    * @param aObserver Reference to observer interface
    * @return On return contains reference to conference call
    * @leave KErrNotReady temporarily cannot return the interface. 
    * @leave System wide error code if unsuccesful
    */
    IMPORT_C MCCEDtmfInterface& DtmfInterfaceL( const MCCEDtmfObserver& aObserver );


    /**
    * Gets extension interface
    *
    * @since S60 3.2
    * @param aObserver Reference to observer interface
    * @return On return contains reference to extension interface
    * @leave KErrNotReady temporarily cannot return the interface.  
    * @leave System wide error code if unsuccesful
    */
   IMPORT_C MCCEExtensionInterface& ExtensionInterfaceL( const MCCEExtensionObserver& aObserver );

    /**
    * This method gets the lifetime of the MS. The lifetime information 
    * includes the manufacturing date of the MS and the total amount of airtime use, 
    * from the manufacturing date until the call to this method. Calling this method 
    * does not reset any data.
    * Forwarded to CS plugin.
    * @since S60 3.2
    * @param aLifeTimeInfo Life time information
    * @return Get succeeded or not.
    */
   IMPORT_C TBool GetLifeTime( TDes8& aLifeTimeInfo );

    /**
    * This method gets cs specific information from cs-plugin. This is used only CS, 
    * @since S60 3.2
    * @param aCSInfo CS specific information from cs-plugin.
    * @return Get succeeded or not.
    */
   IMPORT_C TBool GetCSInfo( CSInfo& aCSInfo );

private:

    /**
    * constructor
    */
    CCCE();

    /**
    * 
    */
    void ConstructL( MCCEObserver* aObserver);

private:

    /**
    * Call container
    */
    CCCECallContainer* iCallContainer;

    /**
    * observer pointer
    */
    CCCEObserverAsynchroniser* iObserver;
    
    };


#endif // CCCE_H
