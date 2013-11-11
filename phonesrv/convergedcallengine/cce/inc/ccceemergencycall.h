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
* Description:  Implementation for emergency call object
*
*/


#ifndef CCCEEMERGENCYCALL_H
#define CCCEEMERGENCYCALL_H

#include <e32base.h>
#include <mccpcallobserver.h>
#include <mccpemergencycall.h>

#include "ccpdefs.h" // TCCPError defs
#include "mccecall.h"
#include "mccecallobserver.h"
#include "cccepluginmanager.h"
#include "cccecallobserverasynchroniser.h"
#include "mccecallinfo.h"
#include "ccecallenums.h"

// CONSTANTS
const TInt KMaxEmergencyNumberLenght = 16;

class CCCEDurationTimer;

/**
 *  Internal emergency call object
 *
 *  This object contains logic for emergency call
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCEEmergencyCall) : public CActive,
                              public MCCECall,
                              public MCCPCallObserver,
                              public MCCECallInfo
    {
public:

    /**
     * Two-phased constructor.
     * @param aCallContainer Reference callcontainer
     * @param aPluginManager Reference to plugin managerl
     */
    static CCCEEmergencyCall* NewL(CCCECallContainer& aCallContainer, 
                                   CCCEPluginManager& aPluginManager );
   
    /**
     * Two-phased constructor.
     * @param aCallContainer Reference callcontainer
     * @param aPluginManager Reference to plugin managerl
     */
    static CCCEEmergencyCall* NewLC(CCCECallContainer& aCallContainer, 
                                    CCCEPluginManager& aPluginManager );
    
    /**
     * Destructor.
     */
    virtual ~CCCEEmergencyCall();
    
    /**
     * Returnd ETrue if emergency call is in progress
     */
    TBool IsInProgress();
 
// from class CActive

    /** @see CActive::RunL */  
    void RunL();

    /** @see CActive::RunError */
    TInt RunError(TInt aError);

    /** @see CActive::DoCancel */
    void DoCancel();
 
// from class MCCECall

    /** @see MCCECall::Dial */
    void SetObserver( const MCCECallObserver& aObserver );
   
    /** @see MCCECall::DialEmergencyCall */
    void DialEmergencyCall( const TDesC16& aNumber );

    /** @see MCCECall::HangUp */
    TInt HangUp();

    /** @see MCCECall::Answer */
    TInt Answer();

    /** @see MCCECall::IsEmergency and MCCECallInfo::IsEmergency */
    TBool IsEmergency() const;
   
    /** @see MCCECall::GetMobileCallInfo */
    void GetMobileCallInfo( TDes8& aCallInfo ) const;
    
    /** @see MCCECall::Parameters */
    const CCCECallParameters& Parameters() const;
    
    /** @see MCCECall::State */
    CCPCall::TCallState State() const;

// from class MCCPCallObserver

    /** @see MCCPCallObserver::ErrorOccurred */
    void ErrorOccurred( TCCPError aError, MCCPCall* aCall ); 
    
    /** @see MCCPCallObserver::CallStateChanged */
    void CallStateChanged( TCCPCallState aState, MCCPCall* aCall ); 
    
    /** @see MCCPCallObserver::CallEventOccurred */
    void CallEventOccurred( TCCPCallEvent aEvent, MCCPCall* aCall ); 
    
    /** @see MCCPCallObserver::CallCapsChanged */
    void CallCapsChanged( TUint32 aCapsFlags, MCCPCall* aCall ); 
    
    /** @see MCCPCallObserver::CallStateChangedWithInband */
    void CallStateChangedWithInband( TCCPCallState aState, MCCPCall* aCall ); 
    
// from class MCCECallInfo
    
    /** @see MCCECallInfo::ImplementationUid */
    const TUid ImplementationUid() const;
    
    /** @see MCCECallInfo::Direction */
    CCPCall::TCallDirection Direction() const;
    
    /** @see MCCECallInfo::CallType */
    CCPCall::TCallType CallType() const;
    
    /** @see MCCECallInfo::CallIndex, MCCECall::CallIndex */
    TInt CallIndex() const;
    
private:

    enum TEmergencyCallRequest
        {
        ECCENone                  =   0x00000000, // 0 must be set to 0
        ECCEDial,                                 // 1
        ECCEReleaseActive                         // 2
        };

    enum TCCEEmergencyState
        {
        EPrimary = 1,
        EOther,
        ETerminated
        };

    
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
    /**
     * Constructor.
     */  
    CCCEEmergencyCall(CCCECallContainer& aCallContainer, CCCEPluginManager& aPluginManager );
   
    /**
     * leaving parts of constructor.
     */
    void ConstructL();
    
    /**
     * Next available plugin.
     */
    void NextPluginL();
   
    /**
     * Removes used plugin if was other than primary.
     */
    void ReleasePlugin();
    
    /**
     * State event handler iplementation.
     */
    void NotifyCallStatusChange( 
        CCPCall::TCallState aCallStatus, 
        TBool aInband, 
        CCECallEnums::TNotifyType aNotify );
   
    /**
     * State event handler iplementation.
     */
    void HandleCallStateChanged( TCCPCallState aState, TBool aInband );
   
    

private: // data
    
    /**
     * State 
     */
    TCCEEmergencyState iState;
    
    /**
     * Dialled number 
     */
    TBuf <KMaxEmergencyNumberLenght> iEmergencyNumber;
    
    /**
     * Is disconnect allowed 
     */
    TBool iAllowDisconnect;
    
    /**
     * Exit code to sent in mobilecallinfo 
     */
    TInt iExitCode;
   
    /**
     * Emergency call is in progress
     */  
    TBool iIsInProgress;
    
    /**
     * call parameters received from phone 
     */
    CCCECallParameters* iCallParameters;
    
    /**
     * Call observer
     * Not own 
     */
    CCCECallObserverAsynchroniser* iObserver;
    
    /**
     * Emergency call object from primary plugin
     * Not own 
     */
    MCCPEmergencyCall* iPrimaryEmergencyCall;
    
    /**
     * Available emergency call plugin TUids 
     */
    RArray<TUid> iAltEmergencyCallPlugins;
    
    /**
     * Currently active plugin 
     */
    TUid iEmergencyProvider;
    
    /**
     * Currently active emergency call object
     * Not own  
     */
    MCCPEmergencyCall* iEmergencyCallInUse;
    
    /**
     * State 
     */
    CCPCall::TCallState iCallState;
    
    /**
     * Reference to call container 
     */
    CCCECallContainer& iCallContainer; 
    
    /**
     *  Reference to plugin manager 
     */
    CCCEPluginManager& iPluginManager;
    
    /**
     *  Current call state 
     */
    TEmergencyCallRequest iActiveRequest;
    
    /**
     * Counts duration for this call
     * Own
     */
    CCCEDurationTimer* iCCEDurationTimer;

    /**
     *  Pointer to own iStatus
     *  NULL when Request is completed
     *  Should be set when iStatus is activated
     */
    TRequestStatus* iInitializing;
    
    /**
     * GSM spec call index.
     */
    TInt iCallIndex;

private:

    // all rest are not used. just stubs.

    /** @see MCCECall::Tone
     * Not Supported in emergency call
     */
    TCCPTone Tone() const;

    /** @see MCCECall::IsSecureSpecified 
     * Not Supported in emergency call
     */
    TBool SecureSpecified() const;

    /** @see MCCECall::IsSecured 
     * Not Supported in emergency call
     */
    TBool IsSecured() const;

    /** @see MCCECall::SetParameters 
     * Not Supported in emergency call
     */
    void SetParameters( const CCCECallParameters& aNewParams );

    /** @see MCCECall::Hold 
     * Not Supported in emergency call
     */
    TInt Hold();

    /** @see MCCECall::Resume 
     * Not Supported in emergency call
     */
    TInt Resume();

    /** @see MCCECall::Swap 
     * Not Supported in emergency call
     */
    TInt Swap();

    /** @see MCCECall::Dial 
     * Not Supported in emergency call
     */
    TInt Dial( const TDesC8& aCallParams );

    /** @see MCCECSCall::NoFDNCheck 
     * Not Supported in emergency call
     */
    void NoFDNCheck();

    /** @see MCCECall::Release 
     * Not Supported in emergency call
     */
    TInt Release();

    /** @see MCCECall::HoldNoNotifyL 
     * Not Supported in emergency call
     */
    void HoldNoNotifyL();

    /** @see MCCECall::ResumeNoNotifyL 
     * Not Supported in emergency call
     */
    void ResumeNoNotifyL();

    /** @see MCCECall::ForwardToAddressL 
     * Not Supported in emergency call
     */
    void ForwardToAddressL( TInt aIndex );

    /** @see MCCECall::CallDuration 
     * Not Supported in emergency call
     */
    TTimeIntervalSeconds CallDuration() const;

    /** @see MCCECall::StartTime 
     * Not Supported in emergency call
     */
    TDateTime StartTime() const;

    /** @see MCCECall::RemoteParty 
     * Not Supported in emergency call
     */
    const TDesC& RemoteParty() const;

    /** @see MCCECall::RemotePartyName
     * Not Supported in emergency call
     */
    const TDesC& RemotePartyName() const;

    /** @see MCCECall::DialledParty 
     * Not Supported in emergency call
     */
    const TDesC& DialledParty() const;

    /** @see MCCECall::ServiceId 
     * Not Supported in emergency call
     */
    TUint32 ServiceId() const;

    /** @see MCCECall::Caps 
     * Not Supported in emergency call
     */
    MCCECallObserver::TCCECallControlCaps Caps() const;

    /** @see MCCECall::GetMobileDataCallCaps 
     * Not Supported in emergency call
     */
    TInt GetMobileDataCallCaps( TDes8& aCaps ) const;

    /** @see MCCECall::LogDialedNumber 
     * Not Supported in emergency call
     */
    TBool LogDialedNumber() const;

    /** @see MCCECall::AttendedTransfer 
     * Not Supported in emergency call
     */
    TInt AttendedTransfer( const TDesC& aTransferTarget );

    /** @see MCCECall::UnattendedTransfer 
     * Not Supported in emergency call
     */
    TInt UnattendedTransfer( const TDesC& aTarget );

    /** @see MCCECall::AcceptTransfer 
     * Not Supported in emergency call
     */
    TInt AcceptTransfer( TBool aAccept );

    /** @see MCCECall::TransferTarget 
     * Not Supported in emergency call
     */
    const TDesC& TransferTarget() const;

    /** @see MCCECall::GetForwardAddressChoicesL 
     * Not Supported in emergency call
     */
    const CDesC8Array& GetForwardAddressChoicesL();

    /** @see MCCECall::SwitchAlternatingCall 
     * Not Supported in emergency call
     */
    TInt SwitchAlternatingCall();

    /** @see MCCECall::Tone 
     * Not Supported in emergency call
     */
    TDateTime iStartTime;

    /** @see MCCECall::ReplaceActive
     * Not Supported in emergency call
     */
    virtual TInt ReplaceActive();

    };

#endif // CCCEEMERGENCYCALL_H
