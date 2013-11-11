/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This is the implementation of the CCallAudioControlImpl class.
*
*/

// INCLUDE FILES
#include "CallAudioControlImpl.h"
#include "CallAudioControlUtility.h"
#include "CallAudioControlProperty.h"
#include "CallAudioControlRepository.h"
#include "CallAudioControlCommon.h"


#ifdef __SERIES60_31__
#include <features.hrh>
#include <FeatMgr.h>
#include <MediatorDomainUIDs.h>
#include <e32property.h>
#include <KPhEngConstants.h>
_LIT(KPhoneCltExDllName, "PhoneClientExt.dll");
#endif

// -----------------------------------------------------------------------------
// CCCallAudioControlImpl::NewL
//
// -----------------------------------------------------------------------------
//
CCallAudioControlImpl* CCallAudioControlImpl::NewL()
    {
    CCallAudioControlImpl* self = new (ELeave) CCallAudioControlImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::CCallAudioControlImpl
// C++ constructor.
// -----------------------------------------------------------------------------
//
CCallAudioControlImpl::CCallAudioControlImpl()
    {
    }

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCallAudioControlImpl::ConstructL()
{
	CAC_TRACE1(_L("CCallAudioControlImpl::ConstructL enter"));
#ifdef __WINS__
	iWinsTesting = ETrue;
#endif
	//Create objects for data base access (both Central Repository and Publish and Subscribe):
	iCsNotifier = CCallAudioControlProperty::NewL(this, KUidCallInfo, KTelephonyCallState, ECallState );
#ifdef __SERIES60_31__
	CAC_TRACE1(_L("CCallAudioControlImpl::ConstructL Creating CMediatorCommandInitiator for 3.1 "));
    iCommandInitiator = CMediatorCommandInitiator::NewL( this );
    iMuteNotifier = CCallAudioControlProperty::NewL(this,KUidSystemCategory, KUidMute.iUid, EMute);
#else
    CAC_TRACE1(_L("CCallAudioControlImpl::ConstructL Creating CPhoneClient for non 3.1 "));
    iPhoneClient = CPhCltCommandHandler::NewL();
    iMuteNotifier = CCallAudioControlProperty::NewL(this,KUidMute, KTelephonyMute, EMute);
#endif
	iIhfVolumeNotifier = CCallAudioControlRepository::NewL(this,KUidVolume, KTelephonyIHFVolume, EIHFVolume);
	iEpVolumeNotifier = CCallAudioControlRepository::NewL(this, KUidVolume, KTelephonyEPVolume, EEPVolume);
    // Get current call state and is it active:
    TBool active = IsCallActive();
	CAC_TRACE2(_L("CCallAudioControl: Current Call State: %d"),iCallState);
   	// If in active call create TAR, get current audio output routing and
   	// get volume (based on audio routing) and set iVolume:
	if (active)
	{
        CAC_TRACE1(_L("CCallAudioControl: Active Call: Setting iVolume, etc..."));
		iTelephonyAudioRouting = CTelephonyAudioRouting::NewL(*this);
		iCurrentAudioOutput = (CCallAudioControl::TAudioOutput)iTelephonyAudioRouting->Output();
		TInt error = GetVolume();
	}
	else // Otherwise, get EP volume and set iVolume = EPVolume
	{
   		TInt error = iEpVolumeNotifier->Get(iVolume);
   		CAC_TRACE2(_L("CCallAudioControl: Not active call, setting iVolume to EPVolume = %d"),iVolume);
	}
	// Subscribe for call state changes:
	// if CAC is created before an active call, then we need to subscribe to Telephony notifications whenever the
	// the call is active, so that CAC can create TAR for audio routing
   	iCsNotifier->Subscribe();
	CAC_TRACE1(_L("CCallAudioControlImpl::ConstructL exit"));
}

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::DestructL
// Destructor
// -----------------------------------------------------------------------------
//
CCallAudioControlImpl::~CCallAudioControlImpl()
    {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::~AudioOutputControlImpl enter"),this);
    // Empty arrays:
    iRoutingObservers.Reset();
    iDownlinkVolumeObservers.Reset();
  	iUplinkGainObservers.Reset();
	CAC_TRACE2(_L("~CAudioOutputControlImpl: Deleting Volume Notifiers..."),this);
	delete iEpVolumeNotifier;
	delete iIhfVolumeNotifier;
	CAC_TRACE2(_L("~CAudioOutputControlImpl: Deleting Call State Notifier..."),this);
	delete iCsNotifier;
	CAC_TRACE2(_L("~CAudioOutputControlImpl: Deleting Mute Notifier..."),this);
	delete iMuteNotifier;
	CAC_TRACE2(_L("~CAudioOutputControlImpl: Deleting TAR..."),this);
	delete iTelephonyAudioRouting;
#ifdef __SERIES60_31__
    CAC_TRACE2(_L("~CAudioOutputControlImpl: Deleting iCommandInitiator (3.1 ONLY)..."),this);
    delete iCommandInitiator;
#else
	CAC_TRACE2(_L("~CAudioOutputControlImpl: Deleting iPhoneClient..."),this);
	delete iPhoneClient;
#endif


	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::~AudioOutputControlImpl exit"),this);

    }

// ---------------------------------------------------------
// CCallAudioControlImpl::AppendRoutingObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::AppendRoutingObserver(CRoutingObserver& aObserver)
    {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::AppendRoutingObserver enter"),this);
	// Return with error if it's already in list:
	for( TInt i = 0; i < iRoutingObservers.Count(); i++ )
   	{
        if ( iRoutingObservers[i] == &aObserver )
       	{
			CAC_TRACE1(_L("AppendRoutingObserver ERROR: Already Exists"));
            return KErrAlreadyExists;
      	}
 	}
	iRoutingObservers.Append(&aObserver);
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::AppendRoutingObserver exit"),this);
	return KErrNone;

    }

// ---------------------------------------------------------
// CCallAudioControlImpl::RemoveRoutingObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::RemoveRoutingObserver(CRoutingObserver& aObserver)
    {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::RemoveRoutingObserver enter"),this);
	TInt error(KErrNotFound);
	for( TInt i = 0; i < iRoutingObservers.Count(); i++ )
   	{
        if ( iRoutingObservers[i] == &aObserver )
       	{
            iRoutingObservers.Remove(i);
            error = KErrNone;
            break;
      	}
 	}
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::RemoveRoutingObserver exit with error: %d"),this, error);
	return error;

    }

// ---------------------------------------------------------
// CCallAudioControlImpl::AppendDownlinkVolumeObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::AppendDownlinkVolumeObserver(CDownlinkVolumeObserver& aObserver)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::AppendDownlinkVolumeObserver enter"),this);
	// Return with error if it's already in list:
	for( TInt i = 0; i < iDownlinkVolumeObservers.Count(); i++ )
   	{
        if ( iDownlinkVolumeObservers[i] == &aObserver )
       	{
			CAC_TRACE1(_L("AppendDownlinkVolumeObserver ERROR: Already Exists"));
            return KErrAlreadyExists;
      	}
 	}
	iDownlinkVolumeObservers.Append(&aObserver);
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::AppendDownlinkVolumeObserver exit"),this);
	return KErrNone;

}

// ---------------------------------------------------------
// CCallAudioControlImpl::RemoveDownlinkVolumeObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::RemoveDownlinkVolumeObserver(CDownlinkVolumeObserver& aObserver)
    {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::RemoveDownlinkVolumeObserver enter"),this);
	TInt error(KErrNotFound);
	for( TInt i = 0; i < iDownlinkVolumeObservers.Count(); i++ )
   	{
        if ( iDownlinkVolumeObservers[i] == &aObserver )
       	{
            iDownlinkVolumeObservers.Remove(i);
            error = KErrNone;
      	}
 	}
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::RemoveDownlinkVolumeObserver exit with error: %d"),this, error);
	return error;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::AppendUplinkGainObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::AppendUplinkGainObserver(CUplinkGainObserver& aObserver)
    {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::AppendUplinkGainObserver enter"),this);
	TInt error(KErrNone);
	// Return with error if it's already in list:
	for( TInt i = 0; i < iUplinkGainObservers.Count(); i++ )
   	{
        if ( iUplinkGainObservers[i] == &aObserver )
       	{
			CAC_TRACE1(_L("AppendUplinkGainObserver ERROR: Already Exists"));
            return KErrAlreadyExists;
      	}
 	}
	error = iUplinkGainObservers.Append(&aObserver);
    if (error == KErrNone)
    {
   		iMuteNotifier->Subscribe();
    }
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::AppendUplinkGainObserver Error [%d] exit"),this, error);
    return error;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::RemoveUplinkGainObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::RemoveUplinkGainObserver(CUplinkGainObserver& aObserver)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]RemoveUplinkGainObserver enter"),this);
	TInt error(KErrNotFound);
	for( TInt i = 0; i < iUplinkGainObservers.Count(); i++ )
   	{
        if ( iUplinkGainObservers[i] == &aObserver )
       	{
            iUplinkGainObservers.Remove(i);
            error = KErrNone;
      	}
 	}
 	// Unsubscribe for mute updates if list is empty and currently subscribed
 	if ( !iUplinkGainObservers.Count() )
 	{
		iMuteNotifier->Unsubscribe();
 	}
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::RemoveUplinkGainObserver exit with error: %d"),this, error);
 	return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::GetAvaliableOutputs
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::GetAvailableOutputs(RArray<CCallAudioControl::TAudioOutput>& aAvailableOutputs)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::GetAvailableOutputs enter"),this);
    TInt error(KErrNone);
    CCallAudioControl::TAudioOutput arrayElement;
    TBool active = IsCallActive();
   	if (active)
    {
    	if (!iTelephonyAudioRouting)
    	{
            CAC_TRACE1(_L("CCallAudioControlImpl[%x]::GetAvailableOutputs TAR not available "));
            return KErrCompletion;
    	}
		TArray<CTelephonyAudioRouting::TAudioOutput> availableOutputArray = iTelephonyAudioRouting->AvailableOutputs();
		TInt arrayCount = availableOutputArray.Count();
 		for (TInt i = 0; i < arrayCount; i++)
  		{
       		CAC_TRACE3(_L("AvailableOutput[%d] = %d"), i, availableOutputArray[i]);
       		arrayElement = (CCallAudioControl::TAudioOutput)(availableOutputArray[i]);
      		aAvailableOutputs.Append(arrayElement);
   		}
    }
    else
    	error = KErrPermissionDenied;
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::GetAvailableOutputs exit with error: %d"),this, error);
    return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::GetOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::GetOutput(CCallAudioControl::TAudioOutput& aOutput)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::GetOutput enter"),this);
	TInt error(KErrNone);
    TBool active = IsCallActive();
   	if (active)
    {
    	if (!iTelephonyAudioRouting)
    	{
            CAC_TRACE1(_L("CCallAudioControlImpl[%x]::GetOutput TAR not available "));
            return KErrCompletion;
    	}
 		aOutput = (CCallAudioControl::TAudioOutput)(iTelephonyAudioRouting->Output());
 		iCurrentAudioOutput = aOutput;
		CAC_TRACE2(_L("GetOutput: Output retrieved: %d"),aOutput);
    }
    else
    	error = KErrPermissionDenied;
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::GetOutput exit with error: %d"),this, error);

    return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::::SetOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::SetOutput(CCallAudioControl::TAudioOutput aOutput)
{
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetOutput with: %d"),this, aOutput);
    TInt error(KErrNone);
    TBool active = IsCallActive();
   	if (active)
    {
    	if (!iTelephonyAudioRouting)
    	{
            CAC_TRACE1(_L("CCallAudioControlImpl[%x]::SetOutput TAR not available "));
            return KErrCompletion;
    	}

    	if ((aOutput == CTelephonyAudioRouting::ENone) || (aOutput == CTelephonyAudioRouting::ENotActive))
    	{
    		return (KErrArgument);
    	}
		TRAP(error,iTelephonyAudioRouting->SetOutputL((CTelephonyAudioRouting::TAudioOutput)aOutput));
    }
    else
    {
		error = KErrPermissionDenied;
    }
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetOutput exit with error: %d"),this, error);
    return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::::PreviousOutput
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::PreviousOutput(CCallAudioControl::TAudioOutput& aPreviousOutput)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::PreviousOutput enter"),this);
    TInt error(KErrNone);
    TBool active = IsCallActive();
   	if (active)
    {
    	if (!iTelephonyAudioRouting)
    	{
            CAC_TRACE1(_L("CCallAudioControlImpl[%x]::PreviousOutput TAR not available "));
            return KErrCompletion;
    	}
		aPreviousOutput = (CCallAudioControl::TAudioOutput)iTelephonyAudioRouting->PreviousOutput();
    }
    else
    {
    	error = KErrPermissionDenied;
    }
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::PreviousOutput exit with error: %d"),this, error);
    return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::GetMinDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::GetMinDownlinkVolume(TUint& aVolume)
    {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::GetMinDownlinkVolume"),this);
    // Hard-coded to 0:
    aVolume = KMinVolume;
    return KErrNone;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::GetDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::GetDownlinkVolume(TUint& aVolume)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::GetDownlinkVolume will return: iVolume"),this, iVolume);
	TInt error(KErrNone);
	if (iVolume < 0) // An error occurred while getting iVolume form DBase prior
	{
		error = iVolume; // return error to client
	}
	else
	{
    	aVolume = iVolume;
	}
    return error;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::GetMaxDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::GetMaxDownlinkVolume(TUint& aVolume)
    {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::GetMaxDownlinkVolume"),this);
    // Hard-coded to 10:
    aVolume = KMaxVolume;
    return KErrNone;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::SetDownlinkVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::SetDownlinkVolume(TUint aVolume)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetDownlinkVolume to: %d"),this, aVolume);
	TInt newVolume = 0;
    // Verify OK to set volume, which is only allowed during an
    // active call and if audio routing is appropriate:
    TInt error = CheckIfOkToSetVolume();
    if (error != KErrNone)
    {
	CAC_TRACE2(_L("SetDownlinkVolume: ERROR: %d"), error);
   	return error;
    }
    // Make sure aVolume range is valid and set newVolume:
    newVolume = VerifyAndSetVolume(aVolume);
    TUint currVol;
    RETURN_IF_ERROR(GetDownlinkVolume(currVol));
    if (currVol !=  aVolume)
        {
        // Update correct volume key, based on current audio routing:
        if ( (iCurrentAudioOutput == CTelephonyAudioRouting::EHandset) || (iCurrentAudioOutput == CTelephonyAudioRouting::EWiredAudioAccessory))
        {
            error = iEpVolumeNotifier->Set(newVolume);
            iPendingEpVolLevelOp = ETrue;
            iRequestedEpVolLevel = aVolume;
        }
        else if (iCurrentAudioOutput == CTelephonyAudioRouting::ELoudspeaker)
        {
            error = iIhfVolumeNotifier->Set(newVolume);
            iPendingIhfVolLevelOp = ETrue;
            iRequestedIhfVolLevel = aVolume;
        }
        else
            error = KErrArgument; // invalid iCurrentAudioOutput value
        }
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetDownlinkVolume exit with error: %d"),this, error);
    return error;

    }

// ---------------------------------------------------------
// CCallAudioControlImpl::GetUplinkMute
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::GetUplinkMute(TBool& aMute)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::GetUplinkMute enter"),this);
   	TInt error = iMuteNotifier->Get(aMute);
   	RETURN_IF_ERROR(error);
#ifdef __SERIES60_31__
    if (aMute == EPSMicMuteOn)
        aMute = ETrue;
    else if ((aMute == EPSMicMuteOff) || (aMute == EPSMicMuteStateUninitialized))
        aMute = EFalse;
    else
        error = KErrUnknown;
#else
    /* 3.2 and beyond: */
    /* 3.2 also appears to be an enum defined in s60/app/telephony/inc/KPhEngConstants.h
     * Check with component owner */
    #ifndef __SERIES60_32__ /* 5.0 and beyond */
        if (aMute == EPSTelMicMuteOn)
            aMute = ETrue;
        else if ((aMute == EPSTelMicMuteOff) || (aMute == EPSTelMicMuteStateUninitialized))
            aMute = EFalse;
        else
            error = KErrUnknown;
    #endif /*#ifndef __SERIES60_32__*/
#endif /*#ifdef __SERIES60_31__*/

	CAC_TRACE2(_L("GetUplinkMute: aMute: %d"),aMute);
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::GetUplinkMute exit with error: %d"),this, error);
	return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::SetUplinkMute
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::SetUplinkMute(TBool aMute)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetUplinkMute with %d"),this, aMute);
	// Set only if active call:
    TInt error(KErrNone);
    TBool active = IsCallActive();
	if (active)
	{
		if (iWinsTesting)
		{
			error = KErrPermissionDenied;
		}
		else
		{
		TBool curMuteState;
		RETURN_IF_ERROR(GetUplinkMute(curMuteState));
		if (aMute != curMuteState)
		    {
            error = SetMute(aMute); // Mute audio
            iPendingMuteSetOp = ETrue;
            iRequestedMuteState = aMute;
		    }
		}
	}
	else
	{
		error = KErrPermissionDenied;
	}
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetUplinkMute exit with error: %d"),this, error);
    return error;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::SetMute
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::SetMute(TBool aMute)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetMute Enter with %d"),this, aMute);
	TInt error(KErrNone);
#ifdef __SERIES60_31__
	TBool coverUIPresent = EFalse;
    FeatureManager::InitializeLibL();
    coverUIPresent = FeatureManager::FeatureSupported( KFeatureIdCoverDisplay );
    FeatureManager::UnInitializeLib();
	if ( coverUIPresent )
	    {
        TSESetMicrophonePckg pckg(aMute);
	    error = iCommandInitiator->IssueCommand( KMediatorSecondaryDisplayDomain,
	            KTSECategory, ECmdMuteOrUnmuteMicrophone, TVersion( KVersioMajor, KVersioMinor, KVersioBuild ), pckg );
	    }
	else
	    {
        TRequestStatus status;
        TInt libEntry;
        // In S60 3.1 CPhCltCommandHandler cannot be directly used. From 3.2 onwards it can be used directly.
        CPhCltExtFactory* phCltExtFactory = NULL;
        CPhCltCommandHandler* phCommandHandler = NULL;
        User::LeaveIfError( iPhoneClientServer.Connect() );
        User::LeaveIfError( iLibrary.Load( KPhoneCltExDllName ) );
        libEntry = iLibrary.Lookup(1)();
        phCltExtFactory = reinterpret_cast<CPhCltExtFactory*>( libEntry );
        if (phCltExtFactory)
            {
            phCommandHandler = phCltExtFactory->CPhCltCommandHandlerLD();
            User::LeaveIfError( phCommandHandler->Open( iPhoneClientServer ) );
            if (aMute)
                {
                phCommandHandler->MuteMicrophone(status, ETrue);
                }
            else
                {
                phCommandHandler->MuteMicrophone(status, EFalse);
                }
            User::WaitForRequest( status );
            if (status.Int() != KErrNone)
                {
                error = KErrGeneral;
                }
            }
        phCommandHandler = NULL;
        phCltExtFactory = NULL;
        iLibrary.Close();
        iPhoneClientServer.Close();
	    }
#else
	TRequestStatus status;
	if (aMute)
		iPhoneClient->MuteMicrophone(status, ETrue);
	else
		iPhoneClient->MuteMicrophone(status, EFalse);
	User::WaitForRequest( status );
	if (status.Int() != KErrNone)
	{
		CAC_TRACE2(_L("SetMute: ERROR from PhoneClient->MuteMic: %d"),status.Int());
		error = KErrGeneral;
	}
#endif /*#ifdef __SERIES60_31__*/
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetMute exit with error: %d"),this, error);
    return error;
    }
// ---------------------------------------------------------
// CCallAudioControlImpl::GetVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::GetVolume()
 {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::GetVolume enter"),this);
	CAC_TRACE2(_L("GetVolume: current audio output = %d"),iCurrentAudioOutput);
	TInt error(KErrNone);
	switch (iCurrentAudioOutput)
	{
		case CTelephonyAudioRouting::ENotActive:
		case CTelephonyAudioRouting::ENone:
        case CTelephonyAudioRouting::EBTAudioAccessory:
		case CTelephonyAudioRouting::ETTY:
			CAC_TRACE1(_L("GetVolume: Audio Routing = Not Active, None, BT or TTY. iVolume set to -19"));
			iVolume = KErrUnknown;
			iEpVolumeNotifier->NotifyCancel();
			iIhfVolumeNotifier->NotifyCancel();
			break;
		case CTelephonyAudioRouting::EHandset:
		case CTelephonyAudioRouting::EWiredAudioAccessory:
		    // iVolume = EP Volume, cancel IHF subscription, subscribe to EP
			CAC_TRACE1(_L("GetVolume: Audio Routing = Handset or Wired accessory: Get EPVolume"));
   			error = iEpVolumeNotifier->Get(iVolume);
   			if (error != KErrNone)
   			{
				CAC_TRACE2(_L("GetVolume: Error returned from EP Volume Get: %d"),error);
   			}
			iIhfVolumeNotifier->NotifyCancel();
			iEpVolumeNotifier->NotifyRequest();
			break;
		case CTelephonyAudioRouting::ELoudspeaker:
			// iVolume = IHF volume, cancel EP subscription, subscribe to IHF
			CAC_TRACE1(_L("GetVolume: Audio Routing = Loudspeaker: Get IHF Volume"));
   			error = iIhfVolumeNotifier->Get( iVolume );
   			if (error != KErrNone)
   			{
				CAC_TRACE2(_L("GetVolume: Error returned from IHFVolume Get: %d"),error);
   			}
			iEpVolumeNotifier->NotifyCancel();
			iIhfVolumeNotifier->NotifyRequest();
			break;
		default:
			break;
	} // End Switch

	if (error != KErrNone) // Set iVolume to error code in case there was an error getting it
	{
		CAC_TRACE2(_L("GetVolume: ERROR, so iVolume = error = %d"), error);
		iVolume = error;
	}
	CAC_TRACE4(_L("CCallAudioControlImpl[%x]::GetVolume exit with error: %d, and iVolume = %d"),this, error, iVolume);
	return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::CheckIfOkToSetVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::CheckIfOkToSetVolume()
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::CheckIfOkToSetVolume enter"),this);
	// if WINS, it's ok to set volume:
	if (iWinsTesting)
		return KErrNone;
    TBool active = IsCallActive();
	if (!active)
	{
		CAC_TRACE2(_L("CheckIfOkToSetVolume NOT OK, Not active call. iCallState: %d"), iCallState);
		return KErrPermissionDenied;
	}
	// IF device is BT/TTY/ENone/EActive return KErrNotSupported:
	if ((iCurrentAudioOutput == CTelephonyAudioRouting::ENotActive) ||
	    (iCurrentAudioOutput == CTelephonyAudioRouting::ENone) ||
	    (iCurrentAudioOutput == CTelephonyAudioRouting::EBTAudioAccessory) ||
	    (iCurrentAudioOutput == CTelephonyAudioRouting::ETTY) )
	{
		CAC_TRACE2(_L("CheckIfOkToSetVolume NOT OK, Audio Output: %d"), iCurrentAudioOutput);
		return KErrNotSupported;
	}
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::CheckIfOkToSetVolume exit"),this);
	return KErrNone;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::VerifyAndSetVolume
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::VerifyAndSetVolume(TInt aVolume)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::VerifyAndSetVolume for: %d "),this, aVolume);
	TInt newVolume;
	if (aVolume > KMaxVolume)
		newVolume = KMaxVolume;
	else if (aVolume < KMinVolume)
		newVolume = KMinVolume;
	else
		newVolume = aVolume;
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::VerifyAndSetVolume: newVolume = %d "),this, newVolume);
	return newVolume;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::HandleCallStateChange
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::HandleCallStateChange()
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::HandleCallStateChange enter"), this);
	TInt error(KErrNone);
    TBool active = IsCallActive();
	if (active ) // Active call or WINS
	{
		if (!iTelephonyAudioRouting)
		{
			TRAP(error,iTelephonyAudioRouting = CTelephonyAudioRouting::NewL(*this));
			if (error != KErrNone)
				return error;
		}
		iCurrentAudioOutput = (CCallAudioControl::TAudioOutput)iTelephonyAudioRouting->Output();
		error = GetVolume();
		if (error != KErrNone)
		{
			CAC_TRACE2(_L("HandleCallStateChange: Error trying to get Volume: %d "), error);
			return error;
		}
	} // end if
    else // Not active, delete TAR if it exists, and set iVolume = EPVolume
    {
    	if (iTelephonyAudioRouting)
    	{
    		delete iTelephonyAudioRouting;
    		iTelephonyAudioRouting = NULL;
    	}
    	error = iEpVolumeNotifier->Get(iVolume);
		if (error != KErrNone)
		{
			CAC_TRACE2(_L("HandleCallStateChange: Error trying to get EPVolume: %d "), error);
			iVolume = error; // set iVolume with error code instead of dBase garbage
		}
    } // end else
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::HandleCallStateChange exit with error: %d"), this, error);
    return error;
}

// ---------------------------------------------------------
// CCallAudioControlImpl::HandleEPVolumeChange
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::HandleEPVolumeChange(TInt aVolume)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::HandleEPVolumeChange for: %d"),this, aVolume);
	TInt error(KErrPermissionDenied);
	// Ignore volume update unless active call and routing is earpiece or wired accessory:
    TBool active = IsCallActive();
	if (active)
	{
		error = KErrNone;
		if ((iCurrentAudioOutput == CTelephonyAudioRouting::EHandset) || (iCurrentAudioOutput == CTelephonyAudioRouting::EWiredAudioAccessory) )
		{
        if (iPendingEpVolLevelOp && (iRequestedEpVolLevel == aVolume))
            {
            return error;
            }
        else
            {
            iPendingEpVolLevelOp = EFalse;
            iVolume = aVolume;
            //Notify clients:
            NotifyVolumeObservers();
			}
		}
	}
	return error;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::HandleIHFVolumeChange
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::HandleIHFVolumeChange(TInt aVolume)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::HandleIHFVolumeChange for: %d "),this, aVolume);
	TInt error(KErrPermissionDenied);
	// Ignore volume update unless active call and routing is Loudspeaker:
    TBool active = IsCallActive();
	if (active)
	{
		error = KErrNone;
		if (iCurrentAudioOutput == CTelephonyAudioRouting::ELoudspeaker)
		{
        if (iPendingIhfVolLevelOp && (iRequestedIhfVolLevel == aVolume))
            {
            return error;
            }
        else
            {
            iPendingIhfVolLevelOp = EFalse;
            iVolume = aVolume;
            //Notify clients:
            NotifyVolumeObservers();
            }
		}
	}
	return error;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::HandleMuteChange
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCallAudioControlImpl::HandleMuteChange(TBool aMute)
    {
	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::HandleMuteChange for: %d "),this, aMute);
	TInt error(KErrPermissionDenied);
    TBool active = IsCallActive();
	// Ignore mute update unless active call and mute state has changed
	if (active)
	{
		error = KErrNone;
        if (iPendingMuteSetOp &&
                ( iRequestedMuteState == aMute))
            {
            return error;
            }
        else
            {
            iPendingMuteSetOp = EFalse;
			//Notify clients:
			NotifyGainObservers();
            }
	}
	return error;
    }

// ---------------------------------------------------------
// CCallAudioControlImpl::NotifyVolumeObservers
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlImpl::NotifyVolumeObservers()
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::NotifyVolumeObservers "),this);
	for( TInt i = 0; i < iDownlinkVolumeObservers.Count(); i++ )
   	{
		CAC_TRACE2(_L("NotifyVolumeObservers[%d] "), i);
    	iDownlinkVolumeObservers[i]->DownlinkVolumeEvent(*(CCallAudioControl*)this, CDownlinkVolumeObserver::KDownlinkVolumeChanged);
 	}
}

// ---------------------------------------------------------
// CCallAudioControlImpl::NotifyGainObservers
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlImpl::NotifyGainObservers()
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::NotifyGainObservers "),this);
	for( TInt i = 0; i < iUplinkGainObservers.Count(); i++ )
   	{
		CAC_TRACE2(_L("NotifyGainObservers[%d] "), i);
    	iUplinkGainObservers[i]->UplinkGainEvent(*(CCallAudioControl*)this, CUplinkGainObserver::KUplinkMuteStateChanged);
 	}
}

// ---------------------------------------------------------
// CCallAudioControlImpl::NotifyRoutingObservers
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlImpl::NotifyRoutingObservers(TUint aEvent, TInt aError)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::NotifyRoutingObservers "),this);
	for( TInt i = 0; i < iRoutingObservers.Count(); i++ )
   	{
		CAC_TRACE2(_L("NotifyRoutingObservers[%d] "), i);
    	iRoutingObservers[i]->RoutingEvent(*(CCallAudioControl*)this, aEvent, aError);
 	}
}

// ---------------------------------------------------------
// CCallAudioControlImpl::NotifyRoutingObservers
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlImpl::NotifyRoutingObservers(TUint aEvent)
{
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::NotifyRoutingObservers "),this);
	for( TInt i = 0; i < iRoutingObservers.Count(); i++ )
   	{
		CAC_TRACE2(_L("NotifyRoutingObservers[%d] "), i);
    	iRoutingObservers[i]->RoutingEvent(*(CCallAudioControl*)this, aEvent);
 	}
}

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::AvailableOutputsChanged
// The following methods are required for MTelephonyAudioRoutingObserver
// -----------------------------------------------------------------------------
//
void CCallAudioControlImpl::AvailableOutputsChanged( CTelephonyAudioRouting& /*aTelephonyAudioRouting*/)
{
	CAC_TRACE1(_L("CCallAudioControlImpl::AvailableOutputsChanged"));
	// ignore callback unless this condition is true
    TBool active = IsCallActive();
	if (active)
    {
    	// Perform AvailableOutputsChanged callback to all existing CRoutingObservers:
    	NotifyRoutingObservers(CRoutingObserver::KAvailableOutputsChanged);
    }
}

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::OutputChanged
// -----------------------------------------------------------------------------
//
  void CCallAudioControlImpl::OutputChanged( CTelephonyAudioRouting& aTelephonyAudioRouting)
  {
  	CAC_TRACE1(_L("CCallAudioControlImpl[%x]::OutputChanged Enter"));
	// Get and save previous and current audio output values:
   	iCurrentAudioOutput = (CCallAudioControl::TAudioOutput)aTelephonyAudioRouting.Output();
   	iPreviousAudioOutput = (CCallAudioControl::TAudioOutput)aTelephonyAudioRouting.PreviousOutput();
	// ignore callback unless this condition is true
    TBool active = IsCallActive();
	if (active)
    {
     	TInt tempVol = iVolume; // save off iVolume
    	// get volume from keys, based on new audio output, and set iVolume:
    	GetVolume();
    	// Perform OutputChanged callback to all existing CRoutingObservers:
    	NotifyRoutingObservers(CRoutingObserver::KOutputChanged);
    	// If there's a volume change, notify all existing CDownlinkVolumeObservers:
    	if (tempVol != iVolume)
    	{
    		NotifyVolumeObservers();
    	}
    }
  	CAC_TRACE1(_L("CCallAudioControlImpl::OutputChanged Exit"));
  }

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::SetOutputComplete
// -----------------------------------------------------------------------------
//
  void CCallAudioControlImpl::SetOutputComplete( CTelephonyAudioRouting& aTelephonyAudioRouting, TInt aErr)
  {

  	CAC_TRACE3(_L("CCallAudioControlImpl[%x]::SetOutputComplete With Error Code: %d"), this, aErr);
   	iCurrentAudioOutput = (CCallAudioControl::TAudioOutput)aTelephonyAudioRouting.Output();
   	iPreviousAudioOutput = (CCallAudioControl::TAudioOutput)aTelephonyAudioRouting.PreviousOutput();
	// ignore callback unless this condition is true
   	TBool active = IsCallActive();
	if (active)
    {
    	TInt tempVol = iVolume; // save off iVolume
    	// get volume from keys, based on new audio output, and set iVolume:
    	GetVolume();
    	// Perform OutputChanged callback to all existing CRoutingObservers:
    	NotifyRoutingObservers(CRoutingObserver::KSetOutputComplete, aErr);
    	// If there's a volume change, notify all existing CDownlinkVolumeObservers:
    	if (tempVol != iVolume)
    	{
    		NotifyVolumeObservers();
    	}
    }
  }

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::NotifyL
// -----------------------------------------------------------------------------
//
void CCallAudioControlImpl::NotifyL(TAction aAction, TInt aError, TInt aValue)
    {
    CAC_TRACE5(_L("CCallAudioControlImpl[%x]::NotifyL for Action: %d, Error: %d and Value: %d"),this, aAction, aError, aValue);
    switch (aAction)
        {
        case ECallState: // Call state change notification
            {
            CAC_TRACE1(_L("NotifyL: Call State Change Notification"));
            iCallState = aValue;
            HandleCallStateChange();
            }
            break;
        case EEPVolume: // EP Volume change notification
            {
            CAC_TRACE1(_L("NotifyL: EP VOlume Change Notification"));
            TInt tempVol = aValue;
            HandleEPVolumeChange(tempVol);
            }
            break;
        case EIHFVolume: // Handle IHF volume change Notification
            {
            CAC_TRACE1(_L("NotifyL: IHF Volume Change Notification"));
            TInt tempVol = aValue;
            HandleIHFVolumeChange(tempVol);
            }
            break;

        case EMute: // Handle mute change Notification
            {
            CAC_TRACE1(_L("NotifyL: Uplink Mute Change Notification"));
            TBool tempMute = aValue;
#ifdef __SERIES60_31__
            if (aValue == EPSMicMuteOn)
                tempMute = ETrue;
            else
                tempMute = EFalse;
#endif
            HandleMuteChange(tempMute);
            }
            break;
        default:
            CAC_TRACE1(_L("NotifyL: UNKNOWN Notification"));
            break;
        } // End switch
    }

// -----------------------------------------------------------------------------
// CCallAudioControlImpl::CommandResponseL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
#ifdef __SERIES60_31__
void CCallAudioControlImpl::CommandResponseL( TUid /*aKMediatorSecondaryDisplayDomain*/,
						TUid /*aKTSECategory*/,
                        TInt aCommandId,
                        TInt aStatus,
                        const TDesC8& /*aData*/ )
     {
	CAC_TRACE2(_L("CCallAudioControlImpl[%x]::CommandResponseL "),this);
     if ( aStatus != KErrNone)
         {
         CAC_TRACE2(_L("CommandResponseL: Command error [%d] "), aStatus);
         }
     else
         {
         switch ( aCommandId )
             {
             case ECmdMuteOrUnmuteMicrophone:
		        CAC_TRACE1(_L("CommandResponseL: Mute Unmute microphone succeeded "));
                break;
            default:
                CAC_TRACE1(_L("CommandResponseL: ERROR: Unknown Command Response Received"));
                break;
             }
         } // End else
     }
#endif

// -----------------------------------------------------------------------------
//
//
//
// -----------------------------------------------------------------------------
//
TBool CCallAudioControlImpl::IsCallActive()
{
	CAC_TRACE1(_L("CCallAudioControlImpl::IsCallActive"));
	iCsNotifier->Get(iCallState);
	if ((iCallState == ECallStateConnected) || (iCallState == ECallStateHold) || iWinsTesting )
    {
		return ETrue;
    }
	return EFalse;
}
// End of file
