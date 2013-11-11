/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation for call object container
*
*/


#ifndef CCCECALLCONTAINER_H
#define CCCECALLCONTAINER_H

#include <e32base.h>

#include "ccpdefs.h"

#include "mccecallobserver.h"           // TCCECallState
#include "mcceconferencecallobserver.h" // TCCEConferenceCallState
#include "ccce.h"                       // CCCECallParameters
#include "mccecallarray.h"
#include "ccecallenums.h"

class CCCE;
class CCCEDtmf;
class CCCECall;
class CCCEDurationTimer;
class CCCEConferenceCall;
class CCCEPluginManager;
class MCCEConferenceCallObserver;
class MCCEDtmfObserver;
class MCCPCall;
class CConvergedCallProvider;
class CCCEExtension;
class MCCESsObserver;
class MCCECallInfo;
class CCCECallInfoMediator;

// UID of CSPlugin
const TUid KCSProviderUid = { 0x102828E5 }; 
const TUid KVCCProviderUid = { 0x2000CFAA }; 

/**
 *  Container
 *
 *  This object contains normal calls, emergency call and conference call.
 *  Also Dtmf and extension interfaces are located here.
 *  
 *  Multicall situations are also handled here.
 *  
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCECallContainer) : public CBase, public MCCECallArray
    {
public:

    /**
     * Two-phased constructor.
     * @param aObserver Reference to observer
     */
    static CCCECallContainer* NewL( MCCEObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CCCECallContainer();

public: 

    /** @see CCCE::SetObserver */
    void SetObserver( MCCEObserver& aObserver, MCCESsObserver& aSsObserver );

    /** @see CCCE::OpenNewCallL */
    MCCECall& OpenNewCallL( const TDesC& aAddress,
         const CCCECallParameters& aCallParameters,
         const MCCECallObserver& aCallObserver );

    /** @see CCCE::ConferenceCallL */
    CCCEConferenceCall& ConferenceCallL(
        const MCCEConferenceCallObserver& aObserver );

    /** @see CCCE::EmergencyCall */
    MCCECall& EmergencyCall( const MCCECallObserver& aCallObserver );

    /** @see CCCE::GetAirTimeDuration */
    void GetAirTimeDuration( TTimeIntervalSeconds& aCSDuration, 
        TTimeIntervalSeconds& aPSDuration);

    /** @see CCCE::DtmfInterfaceL */
    CCCEDtmf& DtmfInterfaceL(
        const MCCEDtmfObserver& aObserver );

    /** @see CCCE::ExtensionInterfaceL */
    MCCEExtensionInterface& ExtensionInterfaceL(
        const MCCEExtensionObserver& aObserver );


    /**
     * Get call which isnt in use.
     *
     * @since S60 v3.2
     * @return Found call, NULL if none is found.
     */
    CCCECall* GetNextFreeCall() const;

    /**
     * Get call with specific id.
     *
     * @since S60 v3.2
     * @param aCallId Specifies which call is returned
     * @return Found call, NULL if none is found.
     */
    CCCECall* GetCall( TInt aCallId ) const;

    /**
     * This is called from cccecall.cpp when it is taken in use.
     *
     * @since S60 v3.2
     * @param aCallId CallId to be reserved
     */
    void ReserveCallInfoCallId( TInt aCallId );

    /**
     * This is called from cccecall.cpp when it is removed from use.
     *
     * @since S60 v3.2
     * @param aCallId CallId to be released
     */
    void ReleaseCallInfoCallId( TInt aCallId );

    /**
     * This is called when call status is changed.
     *
     * @since S60 v3.2
     * @param aStatus New call state
     * @param aCall Pointer to call which is changed
     * @aNotify Type of notify
     */
    void HandleCallStatusChange( 
        CCPCall::TCallState aStatus, 
        CCCECall* aCall,
        CCECallEnums::TNotifyType aNotify );
      
    /**
     * This is called when conference status is changed.
     *
     * @since S60 v3.2
     * @param aStatus New conference state 
     */
    void HandleConferenceStatusChange( 
        MCCEConferenceCallObserver::TCCEConferenceCallState aStatus );   
    
    /**
     * Indication from emergency call about status change.
     * 
     * @since S60 v5.0
     * @param aNotify Type of notify.
     */
    void HandleEmergencyCallStatusChange( CCECallEnums::TNotifyType aNotify );

    /**
     * Releases call
     *
     * @since S60 v3.2
     * @param aCalltoRemove Call to be removed
     * @param aUid Uid for this call
     * @return KErrNone if succesful
     */
    TInt ReleaseCall( MCCPCall& aCalltoRelease, const TUid aUid );

    /**
     * Releases call
     *
     * @since S60 v3.2
     * @param aCalltoRemove Call to be removed
     * @return KErrNone if succesful
     */
    TInt ReleaseCall( MCCPCall& aCalltoRelease );

    /**
     * Returns reference to conferencecall
     *
     * @since S60 v3.2
     * @return Conference call
     */
    CCCEConferenceCall& ConferenceCall() const;

    /**
     * Get call with specific state, Skip given call
     *
     * @since S60 v3.2
     * @param aCallStatus Specified callstatus.
     * @param aSkippedCall Skip this call
     * @return Found call, NULL if none is found.
     */
    CCCECall* GetCall( 
        CCPCall::TCallState aCallStatus, 
        CCCECall* aSkippedCall ) const;

    /**
     * Get call which owns specific MCCPCall.
     * If specified call is NULL return next free call
     *
     * @since S60 v3.2
     * @param aMCCPCall Call to be found.
     * @return Found call.
     */
    CCCECall* GetCall( const MCCPCall* aMCCPCall) const;

    /**
     * Get call array of specific type
     *
     * @since S60 v3.2
     * @param aImplementationUid Specifies calltype to be found
     * @param aCallArray Calls are inserted in this array
     * @return Count of found calls, KErrNotFound if none found
     */
    TInt GetCall( TUid aImplementationUid, 
        RPointerArray<CCCECall>& aCallArray ) const;

    /**
     * Checks if there is any connected, connecting or dialling calls
     *
     * @since S60 v3.2
     * @param aCallArray Calls are inserted in this array
     * @return Count of found calls, KErrNotFound if none found
     */
    TInt GetActiveOrConnectingCalls( RPointerArray<MCCECallInfo>& aCallArray ) const;
    
    /**
     * Counts calls of specified type
     *
     * @since S60 v3.2
     */
    TInt CountCalls( TUid aImplementationUid ) const;
    
    /**
     * Counts calls of specified type
     * Check implementation UID and call type
     * so that call provided by CS plugin
     */
    TInt CountCsCalls( TUid aImplementationUid ) const;
    
    /**
     * Create room to incoming emergency
     *
     * @since S60 v3.2
     */
    void TerminateAllButRingingAndQueuedCalls();
    
     /**
     * Create room to emergency
     *
     * @since S60 v3.2
     */
    void TerminateAllCalls();
    
    /**
    * Create room to emergency
    *
    * @since S60 v3.2
    */
   void TerminateAllVoIPCalls();
   
    /**
     * Call propagated error
     *
     * @since S60 v3.2
     * @param aCall CCCECall failed
     * @param aError error occured
     */
    void ErrorOccurred( CCCECall* aCall, TCCPError aError );

    /**
     * Get CCE observer
     * @since S60 v3.2
     * @param none
     * @return CCE observer reference.
     */
    MCCEObserver* CCEObserver();

    /**
     * Get ss observer
     * @since S60 v3.2
     * @param none
     * @return CCE observer reference.
     */
    MCCESsObserver* SsObserver();
    
    /**
     * Get life time info from CS plugin.
     * @since S60 v3.2
     * @param aLifeTimeInfo Life time info.
     * @return Get succeeded ot not.
     */
    TBool GetLifeTime( TDes8& aLifeTimeInfo );

    /**
     * Get CS specific misc info from CS plugin.
     * @since S60 v3.2
     * @param aCSInfo CS info.
     * @return Get succeeded ot not.
     */
    TBool GetCSInfo( CSInfo& aCSInfo );
    
    /**
     * Stop airtime duration timer
     *
     * @since S60 v3.2
     * @param aCall CCCECall is used for checking call type
     */
    void AirTimeDurationStop( CCCECall* aCall ) const;
    
    /**
     * Switch Air Duration timer, is needed for handover
     * 
     * Stop current timer and starts another one. New timer is initialized 
     * with the same value that old timer had when it was stopped
     *
     * @since S60 v3.2
     * @param aCall aNewCallType new call type, indicated direction of 
     * timer switching
     */
    void AirTimeDurationSwap(const CCPCall::TCallType aNewCallType);
    
    /**
     * Start airtime duration timer
     *
     * @since S60 v3.2
     * @param aCall CCCECall is used for checking call type
     */
    void AirTimeDurationStart( CCCECall* aCall ) const;
    
    /**
     * From base class MCCECallArray
     * @see MCCECallArray
     */
    MCCECallInfo* EmergencyCallInfo() const;
    
    /**
     * From base class MCCECallArray
     * @see MCCECallArray
     */
    MCCECallInfo* CallInfo( TInt aIndex ) const;
       
    /**
     * From base class MCCECallArray
     * @see MCCECallArray
     */
    TInt MaxNumberOfCalls() const;

private:

    /**
     * Gets call with active request, skip call which is given as param
     *
     * @since S60 v3.2
     * @param aCall Call to be skipped
     * @return Found call, NULL if notfound
     */
    CCCECall* GetCallWithActiveRequest( CCCECall* aCall ) const;

    /**
     * Creates call array
     *
     * @since S60 v3.2
     */
    void CreateCallDataArraysL();

   
    
    /**
     * Add a call to release list
     *
     * @since S60 v3.2
     * @param aCall CCCECall is used for checking call type
     */
     void ScheduleReleaseCall( MCCPCall& aCalltoRelease, const TUid aUid );
 
   /**
     * This is called when call(s) is(are) scheduled to be released
     *
     * @since S60 v3.2
     * @param aPluginManager 
     * @return KErrNone if successful
     *         ( KErrNone always or else there is posiibility to loop
     */
    static TInt ReleaseCalls( TAny* aCallContainer );
   
    /**
     * Clear the array of calls to be released
     *
     * @since S60 v3.2
     */
    void DoReleaseCalls();
    
    /**
     * Gets call which isnt part of conference
     *
     * @since S60 v3.2
     */
    CCCECall* GetNonConferenceCall() const;
    
    /**
     * Tells if cs plugin is used in current call
     *
     * @since S60 v3.2
     * @param aCall Call to be checked
     * @return ETrue if cs plugin used
     */
    TBool IsCsPlugin( CCCECall* aCall, TBool aConference ) const;
    
private:

    /** Constructor */
    CCCECallContainer( MCCEObserver& aObserver );

    /** Leaving stuff in constructor */
    void ConstructL();
    
private: // data
    
    /**
     * Class to hold removed cal information 
     * used in delayed release
     */
    class TCallReleaseEntry
        {
        public:
            /**
             * Pointer to call object
             * Not own
             */
            MCCPCall* iCallToRelease;
            /**
             * Uid of plugin where call is located 
             */
            TUid iUid; 
         };
     
     RArray<TCallReleaseEntry> iCallstoRelease;
    /**
     * Dealyed releaser
     * Own
     */
    CIdle* iIdle;    
   
    /**
     * Observer
     * Not own
     */
    MCCEObserver* iMCCEObserver;
   
    /**
     * Observer
     * Not own
     */
    MCCESsObserver* iMCCESsObserver;
    
    /**
     * Conference call object
     * Own
     */
    CCCEConferenceCall* iConferenceCall;
    
    /**
     * Pluginmanager, manages plugin loading
     * Own
     */
    CCCEPluginManager* iPluginManager;

    /**
     * Emergency call object
     * Own
     */
    CCCEEmergencyCall*  iEmergencyCall;
   
    /**
     * Transfer controller
     * Own
     */
    CCCETransferController* iTransferController;
   
    /**
     * Dtmf logic container
     * Own
     */
    CCCEDtmf* iDtmf;
    
    /**
     * Extension interface logic
     * Own
     */
    CCCEExtension* iExtension;
    
    /**
     * Array of call objects
     * Own
     */
    CArrayPtrFlat<CCCECall>* iCallArray;
       
    /**
     * CS call duration
     * Own
     */
    CCCEDurationTimer* iCSDuration;
      
    /**
     * PS call duration
     * Own
     */
    CCCEDurationTimer* iPSDuration;
    
    /**
     * Table is filled so that call info callid 0 is used by
     * callid in table index 0 etc.
     */
    TFixedArray<TInt, KCCECallObjects> iCallInfoCallId;
    
    /** 
     * Call info mediator.
     * Own.
     */
    CCCECallInfoMediator* iCallStateMediator;
    
    };
    
    
#endif
