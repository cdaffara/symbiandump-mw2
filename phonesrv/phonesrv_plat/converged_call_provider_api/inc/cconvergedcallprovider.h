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
* Description:  CCP ECOM interface main class.
*
*/


#ifndef CCONVERGEDCALLPROVIDER_H
#define CCONVERGEDCALLPROVIDER_H

//  INCLUDES
#include <ecom/ecom.h>
#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include <badesca.h>

#include <ccpdefs.h>
#include <mccpcall.h>

// FORWARD DECLARATIONS
class MCCPCallObserver;
class MCCPObserver;
class MCCPSsObserver;

class MCCPEmergencyCall;
class MCCPConferenceCall;
class MCCPConferenceCallObserver;

class MCCPDTMFProvider;
class MCCPDTMFObserver;

class MCCPExtensionProvider;
class MCCPExtensionObserver;

/**
* Converged Call Provider API - main header.
* Main class in CCP. Used e.g. to create calls, access provider classes.
* MCCPObserver class is used for notifying error, state etc information.
*
*  @since S60 3.2
*/
class CConvergedCallProvider : public CBase
    {
public: 
    /**
    * Returns CCP API plug-in implementation identified by given UID.
    * @since S60 3.2
    * @param aImplementationUid CCP API interface implementation UID
    * @return Plug-in implementation pointer returned by ECOM framework.
    * @leave KErrNotFound If ECOM plug-in implementation is not found.
    * @leave KErrNoPermission If ECOM plug-in does not have required capabilities.
    * @leave Any other symbian ECOM leave code.
    */
    static CConvergedCallProvider* NewL(const TUid& aImplementationUid);

    /**
    * Destructor. Destroys the plug-in via ecom framework using iDtor_ID_Key
    * @since S60 3.2
    * @param None
    * @return None
    */
    virtual ~CConvergedCallProvider();

    /**
    * Lists all implementations for this interface.
    * @since S60 3.2
    * @param aImplementations Array containing pointers to all implementations
    * for this interface
    * @return None
    */
    static void ListImplementationsL(RImplInfoPtrArray& aImplementations);

protected:
    /**
    * @since S60 3.2
    * @param None
    * @return None
    */
    inline CConvergedCallProvider();

 
public: // API functions
    
    /**
    * Initialize CConvergedCallProvider after it is created with NewL.
    * @since S60 3.2
    * @param aServiceId service id used by convergedcallprovider
    * @param aObserver Observer class.
    * @param aSsObserver Supplementary services observer class.
    * @leave KErrAlreadyExists If this method has already been called.
    */
    virtual void InitializeL( const TUint32 aServiceId,
                              const MCCPObserver& aObserver, 
                              const MCCPSsObserver& aSsObserver );

    /**
    * Initialize CConvergedCallProvider after it is created with NewL.
    * @since S60 3.2
    * @param aObserver Observer class.
    * @param aSsObserver Supplementary services observer class.
    * @leave KErrAlreadyExists If this method has already been called.
    */
    virtual void InitializeL( const MCCPObserver& aObserver, 
                              const MCCPSsObserver& aSsObserver ) = 0;

    /**
    * Creates a new call.
    * @since S60 3.2
    * @param aParameters Call paramaters such as servive-id used and call type.
    * @param aRecipient Recipients address/number.
    * @param aObserver Observer for the created call.
    * @return MCCPCall Always returns a call pointer if no error occurs.
    * @leave KErrAlreadyExists Call already exists.
    * @leave KErrNotReady Temporarily not able to create the requested call. E.g. maximum 
    * supported number of calls already exists (CCE will not call this if the global limit 
    * of calls is reached, this refers to plug-in own call count limit). 
    * @leave KErrNoMemory Memory error.
    * @leave KErrNotSupported Call not supported.
    * @leave KErrArgument Invalid recipient address or service id.
    * @leave Other general system error.
    * @pre Initialize has been called.
    */ 
    virtual MCCPCall* NewCallL( const CCCPCallParameters& aParameters,
                                const TDesC& aRecipient,
                                const MCCPCallObserver& aObserver ) = 0;
    /**
    * Releases all bindings to call and deletes it. 
    * This will be called by CCE to free all the resources reserved for the call.
    * After this call CCE will not access the call object anymore and observer 
    * for the class must not be used in plug-in.
    * This operation must always succeed. If plug-in decides to pre-allocate the 
    * call object it can choose to do so, but plug-in has to make sure call is 
    * returned to the state it was before calling CConvergedCallProvider::NewCallL.
    * @param aCalltoRemove Call to remove.
    * @return KErrNone if succesfull.
    * @return KErrNotFound if removed call is not found.
    * @pre None, can always be called.
    * @post Plug-in must quarantee all resources for the call will be cleaned up 
    * and plug-in must not call observer after this call!
    * @since S60 3.2
    */
    virtual TInt ReleaseCall( MCCPCall& aCalltoRemove ) = 0;

    /**
    * Creates a new emergency call.
    * @since S60 3.2
    * @param aServiceId Service-Id (this might get removed)
    * @param aAddress Address where emergency call is to be made
    * Can be KNullDesC. Plug-in will use the given address and/or use its own information.
    * @param aObserver Observer for the emergency call.
    * @return MCCPEmergencyCall Created emergency call.
    * @leave plug-in does not support emergency calls.
    * @leave In case of failure system error code.
    * @pre Emergency call object is created.
    */
    virtual MCCPEmergencyCall* NewEmergencyCallL( const TUint32 aServiceId, 
                                                  const TDesC& aAddress,
                                                  const MCCPCallObserver& aObserver ) = 0;
    /**
    * Releases all bindings to call and deletes it. 
    * This must be called to free all the resources reserved for the call.
    * After this call CCE must not be access the call object anymore and observer 
    * for the class must not be used in plug-in.
    * This operation mustways succeed. If plug-in decides to pre-allocate the 
    * call object it can choose to do so, but client has to be sure call is 
    * cleared from the plug-in and all resuorces are returned to the state before calling 
    * CConvergedCallProvider::NewEmergencyCallL.
    * @param aCalltoRemove Call to remove.
    * @return KErrNone if succesfull.
    * @return KErrNotFound if removed call is not found.
    * @pre None, can always be called.
    * @post Plug-in must quarantee all resources for the call will be cleaned up. Call must not 
    * be referenced anymore from CCE and plug-in must not call observer after this call!
    * @since S60 3.2
    */
    virtual TInt ReleaseEmergencyCall( MCCPEmergencyCall& aCalltoRemove ) = 0;

    /**
    * Creates a new conference call.
    * @since S60 3.2
    * @param aServiceId Servive id used.
    * @param aObserver Conference call observer.
    * @return MCCPConferenceCall object.
    * @leave KErrNotReady Temporarily not able to create the requested conference.
    * @leave KErrNoMemory Memory error.
    * @leave KErrArgument Invalid service id.
    * @leave plug-in does not support conference calls.
    * @pre Initialize has been called.
    */
    virtual MCCPConferenceCall* NewConferenceL( const TUint32 aServiceId,
                                                const MCCPConferenceCallObserver& aObserver ) = 0;

    /**
    * Releases all bindings to conference call. CCE will call this after receiving 
    * MCCPConferenceCallObserver::ECCPConferenceIdle from plug-in or when it itself 
    * wants to release the call. In normal sequence called from CCE after HangUp is completed.
    * @param aCalltoRemove Call to remove.
    * @return KErrNone if succesfull.
    * @return KErrNotFound if removed call is not found.
    * @pre None, can always be called.
    * @post Plug-in must quarantee all resources for the call will be cleaned up. Call must not 
    * be referenced anymore from CCE and plug-in must not call observer after this call!
    * @since S60 3.2
    */
    virtual TInt ReleaseConferenceCall( MCCPConferenceCall& aCalltoRemove ) = 0;

    /**
    * Returns the plug-in UID.
    * @since S60 3.2
    * @param None.
    * @return Plug-in UID.
    */
    virtual const TUid& Uid() const = 0;

    /**
    * Get plug-in capabilities.
    * @since Series 60 3.2
    * @param None
    * @return On return contains the capability flags of plug-In.
    */
    virtual TUint32 Caps() const = 0;

    /**
    * Request DTMF provider for dtmf functionality extension. Only one instance exist. Must return the 
    * already created object if called again after initial creation.
    * @since Series 60 3.2
    * @param aObserver Observer for DTMF events.
    * @return Pointer to MCCPDTMFProvider if plug-in support dtmf, returns NULL if not supported.
    * In not supported return NULL but do not leave.
    * @pre Initialize has been called.
    * @leave KErrNoMemory Memory allocation error.
    * @leave Other general system error.
    */
    virtual MCCPDTMFProvider* DTMFProviderL( const MCCPDTMFObserver& aObserver ) = 0;

    /**
    * Request extension provider for custom command handling features. Must return the 
    * already created object if called again after initial creation.
    * @since Series 60 3.2
    * @param aObserver observer for extension(custom) events.
    * @return Pointer to MCCPExtensionProvider if plug-in support extensions, returns NULL if not supported.
    * In not supported return NULL but do not leave.
    * @pre Initialize has been called.
    * @leave KErrNoMemory Memory allocation error.
    * @leave Other general system error.
    */
    virtual MCCPExtensionProvider* ExtensionProviderL( const MCCPExtensionObserver& aObserver ) = 0;

    /**
    * This method gets the lifetime of the MS. The lifetime information 
    * includes the manufacturing date of the MS and the total amount of airtime use, 
    * from the manufacturing date until the call to this method. Calling this method 
    * does not reset any data. This is used only CS, other plug-ins may ignore this method.
    * @since S60 3.2
    * @param aLifeTimeInfo Life time information
    * @return Get succeeded or not.  Default if not implemeted return false. 
    */
    virtual TBool GetLifeTime( TDes8& /*aLifeTimeInfo*/ ) { return EFalse; };

    /**
    * This method gets cs specific information from cs-plugin. This is used only CS, 
    * other plug-ins may ignore this method.
    * @since S60 3.2
    * @param aCSInfo CS specific information from cs-plugin.
    * @return Get succeeded or not. Default if not implemeted return false.
    */
    virtual TBool GetCSInfo( CSInfo& /*aCSInfo*/ )  {  return EFalse; };

private: // Data

    // ECom indentifier key.
    TUid iDtor_ID_Key;
    };

#include <cconvergedcallprovider.inl>

#endif // CCONVERGEDCALLPROVIDER_H

// End of File
