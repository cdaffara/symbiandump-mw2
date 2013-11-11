/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsule the access to ETel MM API in sat
*
*/


#ifndef CSATMULTIMODEAPI_H
#define CSATMULTIMODEAPI_H

#include <e32base.h>
#include "msatmultimodeapi.h"

class CSatSActiveWrapper;

/**
 *  Encapsule the access to ETel MM API in sat
 *  
 *  This class is an active object. It will submit
 *  Etel asynchronous request and wait for response.
 *  Clients for this class will implement the interface
 *  in MSatETelClient which is the callback function
 *  when CSatMultiModeApi get the ETel request response.
 *
 *  @lib satengine.lib
 *  @since S60 v5.0
 */
class CSatMultiModeApi : public CBase,
                         public MSatMultiModeApi
    {

public:
    /**
    * Two-phased constructor.
    */
    static CSatMultiModeApi* NewL();

    /**
    * Destructor
    */
    virtual ~CSatMultiModeApi();

// from base class MSatMultiModeApi
    
    /**
     * Gives pointer to active RMobilePhone session
     *
     * @return pointer to active RMobilePhone server session
     */
    RMobilePhone* Phone();

    /**
     * Gives reference to active Dummy RMobilePhone session
     *
     * @return Reference to active Dummy RMobilePhone server session
     */
    RMobilePhone* DummyPhone();

    /**
     * Gives pointer to active RMmCustomApi session
     *
     * @return pointer to active RMmCustomApi server session
     */
    RMmCustomAPI* CustomApi();

    /**
     * Lowers error granularity to basic errors
     */
    void LowerErrorGranularity();

    /**
     * Raises error granularity to extended errors
     */
    void RaiseErrorGranularity();

    /**
     * Access RMobilePhone::GetNetworkRegistrationStatus
     * for the paramter information please see the etelmm.h
     */
    void GetNetworkRegistrationStatus( TRequestStatus& aReqStatus, 
            RMobilePhone::TMobilePhoneRegistrationStatus& aStatus );
    
    /**
     * Access RMobilePhone::NotifyNetworkRegistrationStatusChange
     * for the paramter information please see the etelmm.h
     */ 
    void NotifyNetworkRegistrationStatusChange( TRequestStatus& aReqStatus,
            RMobilePhone::TMobilePhoneRegistrationStatus& aStatus );

    /**
     * Access RMobilePhone::NotifyNetworkRegistrationStatusChange
     * for the paramter information please see the etelmm.h
     */ 
    void GetSubscriberId( TRequestStatus& aReqStatus, 
            RMobilePhone::TMobilePhoneSubscriberId& aId );

    /**
     * Access RMobilePhone::SendDTMFTones
     * for the paramter information please see the etelmm.h
     */ 
    void SendDTMFTones( TRequestStatus& aReqStatus, const TDesC& aTones);   
    
    /**
     * Access RMobilePhone::ContinueDTMFStringSending
     * for the paramter information please see the etelmm.h
     */ 
    TInt ContinueDTMFStringSending( TBool aContinue );
    
    /**
     * Access RMobilePhone::CancelAsyncRequest
     * for the paramter information please see the etelmm.h
     */
    void CancelAsyncRequest(TInt aReqToCancel);

    /**
     * Access RMobilePhone::SendNetworkServiceRequestNoFdnCheck
     * for the paramter information please see the etelmm.h
     */
    void SendNetworkServiceRequestNoFdnCheck( TRequestStatus& aReqStatus, 
            const TDesC& aServiceString);
        
    /**
     * Access RMobileCall::DialNoFdnCheck
     * for the paramter information please see the etelmm.h
     */
    void DialNoFdnCheck( TRequestStatus& aStatus,
            const TDesC8& aCallParams,const TDesC& aTelNumber );
                                                        
    /**
     * Access RMobileUssdMessaging::SendMessageNoFdnCheck
     * for the paramter information please see the etelmm.h
     */
    void SendMessageNoFdnCheck( TRequestStatus& aReqStatus,
            const TDesC8& aMsgData, const TDesC8& aMsgAttributes );
    /**
     * Access RMobileCall::DialCancel
     */
    void DialCancel();

    /**
     * Check if there is an incoming call 
     * @return ETrue if there is an incoming call
     */
    TBool IsCallIncoming();
    
    /**
     * Access RMobileCall::NotifyMobileCallStatusChange
     */
    void NotifyMobileCallStatusChange( TRequestStatus& aReqStatus,
                    RMobileCall::TMobileCallStatus& aStatus) ; 

    /**
     * Access RMobileCall::NotifyStatusChangeCancel
     */
    void NotifyCallStatusChangeCancel();
    /**
     * Access RMobilePhone::TerminateActiveCalls NotifyMobileCallStatusChange
     */
    void TerminateActiveCalls(TRequestStatus& aReqStatus);

    
    /**
     * Access RMboileCall::GetMobileCallInfo
     */
    TInt GetMobileCallInfo(TDes8& aCallInfo);
    
    /**
     * Access RMboileCall::DialEmergencyCall
     */
    virtual void DialEmergencyCall(TRequestStatus& aReqStatus, 
                const TDesC& aNumber);

    /**
     * Check validity of a given Data Coding Cheme (Dcs).
     * used by call control handler and send ussd handler
     * @param aDcs Data Coding Cheme to be validated.
     * @return TBool indicating Dcs validity.
     */           
    virtual TBool IsValidUssdDcs( const TUint8 aDcs );
            
private:

    /**
    * Default C++ constructor
    */
    CSatMultiModeApi();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();
    
// New functions
    /**
     * Makes the connections to ETel Server
     */
    void ConnectETelServerL();

    /**
     * Loads and opens phone module
     *
     * @param aPhone reference to phone that is loaded
     * @param aModuleName Name of the phone module
     * @param aPhoneName Name of the phone
     */
    void LoadPhoneModuleL( RMobilePhone& aPhone, const TDesC& aModuleName,
            const TDesC& aPhoneName );

    /**
     * Open the RMobileUssdMessaging
     */ 
    void LoadUssdMessagingL();
    
    /**
     * Open the RMobileCall
     */
    TInt LoadMobileCall();
    

private: // data

    /**
     * The Etel server.
     * Own
     */
    RTelServer iTelServer;

    /**
     * Used to access phone based functionality
     * Own
     */
    RMobilePhone iPhone;

    /**
     * Phone module for Dummy TSY
     * Own
     */
    RMobilePhone iDummyPhone;

    /**
     * RMmCustomAPI
     * Own
     */
    RMmCustomAPI iCustomPhone;
    
    /**
     * Line needed by iCall
     * own
     */
    RMobileLine iLine;
    
    /**
     * Access call based functionality
     * own
     */
    RMobileCall iCall;
    
    /**
     * Access USSD Messaging
     * own
     */
    RMobileUssdMessaging iUssd;

    /**
     * Pointer to Active Wrapper. Used in startup
     * Own.
     */
    CSatSActiveWrapper* iWrapper;
    
    /**
     * Indicate if the iLine has been openned
     */
    TBool iLineOpened;

    /**
     * Indicate if the iCall has been openned
     */
    TBool iCallOpened;
    
    /**
     * Indicate if the iUssd has been openned
     */
    TBool iUssdOpened;
    
    };

#endif // CSATMULTIMODEAPI_H
