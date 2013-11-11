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
* Description:  Implementation for call object
*
*/


#ifndef CCCECALL_H
#define CCCECALL_H

#include <e32base.h>
#include <cccpcallparameters.h>

#include "ccpdefs.h" // TCCPError defs

#include "mccpcallobserver.h"     // MCCPCallObserver
#include "mccptransferobserver.h" // MCCPTransferObserver
#include "mccpforwardobserver.h"  // MCCPForwardObserver
#include "mccecall.h"             // MCCECall
#include "mccecallobserver.h"     // For enumeration use
#include "mccecallinfo.h"
#include "ccecallenums.h"

class CCCEDurationTimer;
class CCCECallContainer;
class MCCPCall;
class MCCPTransferProvider;
class CCCETransferController;
class MCCPForwardProvider;
class CCCECallObserverAsynchroniser;

/**
 *  Internal call object
 *
 *  This object contains logic for this specific call
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCECall) : public CBase,
                              public MCCPCallObserver,
                              public MCCPTransferObserver,
                              public MCCPForwardObserver,
                              public MCCECall,
                              public MCCECallInfo
    {
    
public:

    /** Enumeration for requests */ 
    enum TCallRequest
       {
       ECCENone                  =   0x00000000, // 0 must be set to 0
       ECCEDial,                                 // 1
       ECCEReleaseActive,                        // 2
       ECCEReleaseHeld,                          // 3
       ECCEReleaseOutgoing,                      // 4
       ECCERejectIncoming,                       // 5
       ECCETerminateDisconnected,                // 6
       ECCEHold,                                 // 7
       ECCEResume,                               // 8
       ECCESwap,                                 // 9
       ECCETransfer,                             //10
       ECCEConferenceJoin,                       //11
       ECCEMakeEmergencyCall,                    //12
       ECCEAnswer,                               //13
       ECCEConferencePrivate,                    //14
       ECCEConferenceDropParticipant,            //15
       ECCEConferenceTerminate,                  //16
       ECCESwitchCallMode,                       //17
       ECCEReplaceActive,                        //18
       ECCETerminateAll,                         //19
       ECCETerminateAllAndWaiting,               //20
       ECCETerminateAllActiveCalls,              //21
       ECCETerminateAllHeldCalls,                //22
       ECCEClearBlackListing,                    //23
       ECCETerminateAndDial,                     //24    
       ECCEActivateCcbs,                         //25
       ECCERejectCcbs,                           //26
       ECCEUnattendedTransfer,                   //27  
       ECCEPark                                  //28
       };

private:

 
    class TStateTransition
        {
    public:
        TStateTransition() 
            {
            iState = CCPCall::EStateIdle;
            iInband = EFalse;
            }

        CCPCall::TCallState iState;
        TBool iInband;
        };
public:

    /**
     * Two-phased constructor.
     * @param aCallContainer Reference callcontainer
     * @param aCallId Defines id for this call
     */
    static CCCECall* NewLC( CCCECallContainer& aCallContainer, TInt aCallId );
    
    /**
     * Destructor.
     */
    virtual ~CCCECall();

public:

    /**
     * Sets new converged call into to use.
     *
     * @since S60 v3.2
     * @param aCall Pointer to used ccp call
     * @param aImplementationUid Implementation uid of plugin that call is from
     * @param aTransferred Flag indicating if this was transferred call. In that
     * case some of the initilizations made in this method should not be done
     * @return None
     */
    void SetConvergedCallL( 
        MCCPCall* aCall,
        TUid aImplementationUid,
        TBool aTransferred = EFalse );

    /**
     * Informs to observers that calls request has been completed.
     *
     * @since S60 v3.2
     * @param aRequest Request to be completed
     * @param aError Error code for request( KErrNone if succesful )
     */
    void NotifyRequestComplete( TInt aRequest, TCCPError aError );

    /**
     * Complete currently ongoing request
     *
     * @since S60 v3.2
     * @param aError Error code ( KErrNone if succesful )
     */
    void RequestComplete( TCCPError aError );

    /**
     * Do request that is pending. This is needed in multicall dial and answer.
     *
     * @since S60 v3.2
     * @return KErrNone if request was started succesfully. 
     */
    TInt DoPendingRequest();

    /**
     * Call object in use.
     *
     * @since S60 v3.2
     * @return ETrue if in use. 
     */
    TBool IsInUse() const;

    /**
     * Service id of this call.
     *
     * @since S60 v3.2
     * @return Service id of this call, 0 if not in use. 
     */
     TUint32 ServiceId() const;

    /**
     * Reference to owned converged call provider call.
     *
     * @since S60 v3.2
     * @return Reference to MCCPCall. 
     */    
    MCCPCall& GetCCPCall() const;

    /**
     * Returns currently ongoing request.
     *
     * @since S60 v3.2
     * @return Currently ongoing request 
     */
    TCallRequest Request() const;
        
     /**
     * Id of this call
     *
     * @since S60 v3.2
     * @return Id of this call. 
     */
    TInt CallId() const;

    /**
     * Adds new observer for MCCECall object.
     * 
     * @since S60 v3.2
     * @param aCallObserver Observer
     */
    void SetObserver( const MCCECallObserver& aObserver );
    
    /**
     * Put call on hold but dont complete request.
     * This is used in multicall situations.
     *
     * @since S60 v3.2
     * @return KErrNone if request was started succesfully. 
     */
    TInt HoldNoNotify();

    /**
     * Resume call but dont complete request.
     * This is used in multicall situations.
     *
     * @since S60 v3.2
     * @return KErrNone if request was started succesfully. 
     */
    TInt ResumeNoNotify();
    
    /**
     * Called by the TransferController object in case when remote end has been
     * inititated transfer
     * @since S60 v3.2
     * @param aAttended Type of the transfer ETrue if type is attended 
     * otherwise EFalse
     * @param aTransferController Reference to TransferController
     * @return None
     */
    void HandleTransfer( const TBool aAttented,
                         CCCETransferController& aTransferController );
    
     /**
     * Sets TransferController handle
     * @since S60 v3.2
     * @param aTransferController Pointer to TransferController object
     * @return None
     */
    void SetTransferController( CCCETransferController* aTransferController );
    
    /**
     * Sets new call index if call index not valid.
     * 
     * @since S60 5.0
     */
    void SetNewCallIndex();

// from base class MCCECall

    /** @see MCCECall::Release */
    TInt Release();

    /** @see MCCECall::ReplaceActive */
    TInt ReplaceActive();

    /** @see MCCECall::Hold */
    TInt Hold();

    /** @see MCCECall::Resume */
    TInt Resume();
    
    /** @see MCCECall::HangUp */
    TInt HangUp();

    /** @see MCCECall::State */
    CCPCall::TCallState State() const;
    
    /** @see MCCECall::Tone */
     TCCPTone Tone() const;

    /** @see MCCECall::CallDuration */
    TTimeIntervalSeconds CallDuration() const;

    /** @see MCCECall::Parameters */
    const CCCPCallParameters& Parameters() const;

// from base class MCCPCallObserver

    /** @see MCCPCallObserver::ErrorOccurred */
    void ErrorOccurred( TCCPError aError, MCCPCall* aCall );
    
    /** @see MCCPCallObserver::CallStateChanged */
    void CallStateChanged( TCCPCallState aState, MCCPCall* aCall );

// from class MCCECallInfo
    
    /** @see MCCECallInfo::ImplementationUid */
    const TUid ImplementationUid() const;
    
    /** @see MCCECallInfo::Direction */
    CCPCall::TCallDirection Direction() const;
    
    /** @see MCCECallInfo::CallType */
    CCPCall::TCallType CallType() const;
    
    /** @see MCCECallInfo::CallIndex and MCCECall::CallIndex */
    TInt CallIndex() const;
    
private: 

// from base class MCCECall

    /** @see MCCECall::SetParameters */
    void SetParameters( const CCCECallParameters& aNewParams );
    
    /** @see MCCECall::Swap */
    TInt Swap();

    /** @see MCCECall::Answer */
    TInt Answer();

    /** @see MCCECall::Dial */
    TInt Dial( const TDesC8& aCallParams );

    /** @see MCCECall::DialEmergencyCall */
    void DialEmergencyCall( const TDesC16& aNumber );

    /** @see MCCECall::ForwardToAddressL */
    void ForwardToAddressL( TInt aIndex );

    /** @see MCCECall::GetMobileCallInfo */
    void GetMobileCallInfo( TDes8& aCallInfo ) const;

    /** @see MCCECall::StartTime */
    TDateTime StartTime() const;

    /** @see MCCECall::Caps */
    MCCECallObserver::TCCECallControlCaps Caps() const;

    /** @see MCCECall::IsSecureSpecified */
    TBool SecureSpecified() const;

    /** @see MCCECall::IsSecured */
    TBool IsSecured() const;
    
    /** @see MCCECall::IsEmergency and MCCECallInfo::IsEmergency */
    TBool IsEmergency() const;

    /** @see MCCECall::RemoteParty */
    const TDesC& RemoteParty() const;

    /** @see MCCECall::RemotePartyName */
    const TDesC& RemotePartyName() const;

    /** @see MCCECall::DialledParty */
    const TDesC& DialledParty() const;
    
    /** @see MCCECall::AttendedTransfer */
    TInt AttendedTransfer( const TDesC& aTransferTarget );

    /** @see MCCECall::UnattendedTransfer */
    TInt UnattendedTransfer( const TDesC& aTarget );

    /** @see MCCECall::AcceptTransfer */
    TInt AcceptTransfer( const TBool aAccept );

    /** @see MCCECall::TransferTarget */
    const TDesC& TransferTarget() const;

    /** @see MCCECall::GetForwardAddressChoicesL */
    const CDesC8Array& GetForwardAddressChoicesL();

// from base class MCCECSCall

    /** @see MCCECSCall::SwitchAlternatingCall */
    TInt SwitchAlternatingCall();
    
    /** @see MCCECSCall::GetMobileDataCallCaps */
    TInt GetMobileDataCallCaps( TDes8& aCaps ) const;
    
    /** @see MCCECSCall::NoFDNCheck */
    void NoFDNCheck();
    
    /** @see MCCECSCall::LogDialedNumber */
    TBool LogDialedNumber() const;
    
// from base class MCCPCallObserver
    
    /** @see MCCPCallObserver::CallEventOccurred */
    void CallEventOccurred( TCCPCallEvent aEvent, MCCPCall* aCall );
    
    /** @see MCCPCallObserver::CallCapsChanged */
    void CallCapsChanged( TUint32 aCapsFlags, MCCPCall* aCall );
    
    /** @see MCCPCallObserver::CallStateChangedWithInband */
    void CallStateChangedWithInband( TCCPCallState aState, MCCPCall* aCall );
    
// from base class MCCPTransferObserver

    /** @see MCCPTransferObserver::TransferEventOccurred */
    void TransferEventOccurred( TCCPTransferEvent aEvent );

// from base class MCCPForwardObserver

     /** @see MCCPForwardObserver::ForwardEventOccurred */
    void ForwardEventOccurred( 
        const MCCPForwardObserver::TCCPForwardEvent aEvent );

private:

    /**
     * Reset call members
     *
     * @since S60 v3.2
     */
    void Reset();

    /**
     * Handle call state change
     *
     * @since S60 v3.2
     * @param aCallState New call state ( TCCPCallState )
     * @param aInband ETrue if tones for this status is generated inband
     */
    void CallStateChanged( TCCPCallState aState, TBool aInband );

    /**
     * Notify call state changes to observers
     *
     * @since S60 v3.2
     * @param aCallStatus New call state
     * @param aInband ETrue if tones for this status is generated inband
     */
    void NotifyCallStatusChange( 
        CCPCall::TCallState aCallStatus, 
        TBool aInband, 
        CCECallEnums::TNotifyType aNotify);

    /**
     * Tells if cs plugin is used in current call
     *
     * @since S60 v3.2
     * @return ETrue if cs plugin used
     */
    TBool IsCsPlugin() const;
    
private:

    /** Constructor */
    CCCECall( CCCECallContainer& aCallContainer, TInt aCallId );

    /** Leaving stuff in constructor */
    void ConstructL();

private: // data

    /**
     * Id for this call
     */
    const TInt iCallId;

    /**
     * Callcontainer that owns this call
     */
    CCCECallContainer& iCallContainer;
        
    /**
     * Currently ongoing request
     */
    TCallRequest iActiveRequest;
    
    /**
     * KErrNone if everythink OK
     */
    TCCPError iExitCode;
    
    /**
     * Call direction
     */
    CCPCall::TCallDirection iCallDirection;

    /**
     * If request is made internally, its completion is not
     * notified to observer. This variable contains request
     * id of internal request
     */
    TInt iDoNotReportRequest;
    
    /**
     * Call status for internal use
     */
    CCPCall::TCallState iCallState;
    
    /**
     * Plugin implementation Uid
     */
    TUid iImplementationUid;
    
    /**
     * Callparameters
     */
    CCCPCallParameters* iCallParameters;
    
    /**
     * CS call only
     * Not own
     */
    const TDesC8* iCallParams;
    
    /**
     * Pointer to observer
     * Own
     */
    CCCECallObserverAsynchroniser* iObserver;
    
    /**
     * Pointer to converged call provider call
     * Own
     */
    MCCPCall* iCall;
    
    /**
     * Counts duration for this call
     * Own
     */
    CCCEDurationTimer* iCCEDurationTimer;
    
    /**
     * ForwardProvider interface handle
     * Own
     */
    MCCPForwardProvider* iForwardProvider;
    
    /**
     * TransferProvider interface handle. 
     * Not own
     */  
    MCCPTransferProvider* iTransferProvider;
     
    /**
     * TransferController handle. 
     * Not own
     */ 
    CCCETransferController* iTransferController;
    
    /**
     * GSM spec Call index.
     */
    TInt iCallIndex;
    
   
    };

#endif // CCCECALL_H
