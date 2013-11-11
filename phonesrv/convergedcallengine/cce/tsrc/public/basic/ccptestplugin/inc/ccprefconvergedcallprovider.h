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
* Description:  CCP test plug-in implementation.
*
*/


#ifndef CCPREFCONVERGEDCALLPROVIDER_H
#define CCPREFCONVERGEDCALLPROVIDER_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include <badesca.h>
#include <cconvergedcallprovider.h>
#include <mccpssobserver.h>
#include <mccpcsobserver.h>
#include "ccprefcontroller.h"
#include "configurationhandler.h"

// Forward declarations

class CCPRefCall;
class CCPRefEmergencyCall;
class CCPRefExtensionProvider; 
class CCPRefDTMFProvider;
class CCPRefConferenceCall;
class CCPRefController;


/**
* Main class for the CCP API interface. In this example plug-in case. NewL will be the 
* first method to be called when ecom starts us up. Afterwards Initialize will be called.
*/
class CCPRefConvergedCallProvider : public CConvergedCallProvider
    {
    
public: 

    /*
    * startup function for ecom
    */
    IMPORT_C static CCPRefConvergedCallProvider* CsNewL();
    
    /*
    * startup function for ecom
    */
    IMPORT_C static CCPRefConvergedCallProvider* VccNewL();
    
    /*
    * startup function for ecom
    */
    IMPORT_C static CCPRefConvergedCallProvider* PsNewL();

    /*
    * dtor
    */
    ~CCPRefConvergedCallProvider();    
    
private:

    /**
    * constructor.
    */
	CCPRefConvergedCallProvider();
	
	/**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( 
        TUint aPluginUid, CCPCall::TCallType aCallType, TInt aRequestValue );

    /**
    * Initializes plug-in. All required parameters must be passed
    * with this function in order to use services.
    * Simplifies pluging construct removing need to pass parameters to ECom interface
    * @since S60 3.2
    * @param aAppUID Application UID of Telephony application
    * @param aObserver Observer
    */
    
    void InitializeL( const MCCPObserver& aObserver,
    				  const MCCPSsObserver& aSsObserver );
    /**
    * Creates a new call and checks secure status from profile.
    * @since S60 3.2
    * @param aParameters Call paramaters such as servive-id used and call type.
    * @param aProtocolProfileId Id of the profile used
    * @param aRecipient Recipients address/number
    * @param aObserver Observer
    * @return MCCPMoCall - always returns a call pointer if no error occurs.
    * @leave KErrAlreadyExists - call already exists 
    * @leave KErrNotReady - call was not possible to create, e.g. network connection is not available
    * @leave KErrNoMemory - memory error
    * @leave KErrNotSupported - call not supported
    * @leave KErrArgument - illegal recipient address
    */ 
    MCCPCall* NewCallL( const CCCPCallParameters& aParameters,
			            const TDesC& aRecipient,
						const MCCPCallObserver& aObserver );


	TInt ReleaseCall( MCCPCall& aCalltoRemove );

    /**    
    * Creates a new Emergency call and add user-agent header.
    * @since S60 3.2
    * @param aServiceId Service id used
    * @param aAddress Address where emergency call is to be made. 
    * Can be KNullDesC. Plug-in will use given address and/or its own known one.
    * @param aObserver Observer
    * @return MCCPEmergencyCall
    */
    MCCPEmergencyCall* NewEmergencyCallL( const TUint32 aServiceId, 
					                      const TDesC& aAddress,
                                          const MCCPCallObserver& aObserver );
                                          
    TInt ReleaseEmergencyCall( MCCPEmergencyCall& aCalltoRemove );

    /**
    * Creates a new conference call.
    * @since Series 60 3.2
    * @param aServiceId Service id used
    * @param aObserver - observer class
    * @return Pointer to MCCPCSCallConfernece if succesfull, NULL if not available
    * @leave on error case system error code
    */
    MCCPConferenceCall* NewConferenceL( const TUint32 aServiceId,
                                        const MCCPConferenceCallObserver& aObserver );
                                        
    TInt ReleaseConferenceCall( MCCPConferenceCall& aCalltoRemove );
    
    /**
	* Get DTMF provider
	* @since Series 60 3.2
    * @param aObserver observer for DTMF events
	* @return Pointer to MCCPDTMFProvider if succesfull, NULL if not available
    * @leave on error case system error code
	*/
	MCCPDTMFProvider* DTMFProviderL( const MCCPDTMFObserver& aObserver );

	/**
	* Get extension provider
	* @since Series 60 3.2
    * @param aObserver observer for extension(custom) events
	* @return Pointer to MCCPExtensionProvider if succesfull, NULL if not available
    * @leave on error case system error code
	*/
	MCCPExtensionProvider* ExtensionProviderL( const MCCPExtensionObserver& aObserver );

	/**
	* This method gets the lifetime of the MS. The lifetime information 
	* includes the manufacturing date of the MS and the total amount of airtime use, 
	* from the manufacturing date until the call to this method. Calling this method 
	* does not reset any data. This is used only CS, other plug-ins may ignore this method.
	* @since S60 3.2
	* @param aLifeTimeInfo Life time information
	* @return Get succeeded or not. 
	*/
	TBool GetLifeTime( TDes8& aLifeTimeInfo );

	/**
	* This method gets cs specific information from cs-plugin. This is used only CS, 
	* other plug-ins may ignore this method.
	* @since S60 3.2
	* @param aCSInfo CS specific information from cs-plugin.
	* @return Get succeeded or not. 
	*/
	TBool GetCSInfo( CSInfo& aCSInfo );


    /* Call this from CCPRefCall::Release, and delete the call here
    *
    */
    TInt DeleteCall( CCPRefCall& aCallToDelete );
    
public:	// Public methods

    /**
    * Returns Uid of plug-in which is on use.
    * @since S60 3.2
    * @return Uid of plug-in
    */
    const TUid& Uid() const;

    /**
    * Get plug-in capabilities.
    * @since Series 60 3.2
    * @param None
    * @return On return contains the capability flags of plug-In.
    */
    TUint32 Caps() const;
    
    void IncomingTransfer( TInt aOriginatorCallID, TBool aAttended, TInt serviceID );
               
    void CreateMtCall(TInt aServiceID);
    
    void CountRemainingCalls();
    
    /*
    * Return service-id. Not in APi
    */
    TUint32 ServiceId() const; 
    
    
    /*
    * Return configuration handler.
    */
	CConfigurationHandler& ConfigurationHandler();
	
    /**
     * Get call with specific id.
     *
     * @since S60 v3.2
     * @param aCallId Specifies which call is returned
     * @return Found call, NULL if none is found.
     */
    CCPRefCall* GetCall( TInt aCallId ) const;
    
    void TerminateAllCalls();
    
private: // Private data members

    /*
    * Active Object that is used for controlling events
    * from test module
    */
	CCPRefController* iPluginCtrl;
    
    TUint iServiceId;
    
    CCPCall::TCallType iCallType;
    TUid iPluginUid;
    TUid iAppUID;
    TUint32 iCaps;
        
public: // Public data members
        
    // Plug-in call pointers
    RPointerArray<CCPRefCall> iCallArray;
    
    MCCPObserver* iObserver;
    
    MCCPSsObserver* iSsObserver;
       
    CCPRefEmergencyCall* iEmergencyCallPtr;
    
    CCPRefExtensionProvider* iExtensionProvider; 
    
    CCPRefDTMFProvider* iDTMFProvider;
    
    CCPRefConferenceCall* iConferenceCallProvider;
    CCPRefConferenceCall* iConferenceCallProviderTemp;
    
    HBufC* iCalledParty;

	// flag if to return ext if or not(NULL). 
	// Setting is valid only for one get request at a time.
	TBool iIsExtIfSupported;
	
	// value used for leaving when calling 
	// Setting is valid only for one get request at a time.
	// KErrNone - does not leave
	TInt iExtIfLeaveValue;

	// configuration handler
	mutable CConfigurationHandler iConfigurationHandler;

	// let this class tamper us
	friend class CCPRefExtensionProvider;
    };

#endif      // CCPREFCONVERGEDCALLPROVIDER_H

// End of File
