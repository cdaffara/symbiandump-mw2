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
* Description:  Defines the interface to CallAudioControlImpl
*
*/

#ifndef CALLAUDIOCONTROLIMPL_H
#define CALLAUDIOCONTROLIMPL_H

//  INCLUDES
#include <e32base.h>
#include <TelephonyAudioRouting.h>
#include <RoutingObserver.h>
#include <UplinkGainObserver.h>
#include <DownlinkVolumeObserver.h>
#include <MTelephonyAudioRoutingObserver.h>
#include <centralrepository.h>

#ifdef __SERIES60_31__
#include <TSEngInterface.h>
#include <RPhCltServer.h> 
#include <PhCltExt.h>       // link against PhoneClientExt.lib
#include <MediatorCommandInitiator.h>
#else
#include <cphcltcommandhandler.h> // wasn't introduced until 3.2
#endif

#include "CallAudioControlUtility.h"
#include "CallAudioControl.h"

// FORWARD DECLARATIONS
class CRoutingObserver;
class CCallAudioControlProperty;
class CCallAudioControlRepository;

// CLASS DECLARATION
class CCallAudioControlImpl : public CBase
            , public MTelephonyAudioRoutingObserver
#ifdef __SERIES60_31__
            , public MMediatorCommandResponseObserver
#endif
    {

    public:  

        static CCallAudioControlImpl* NewL();

 		~CCallAudioControlImpl();

     	TInt AppendRoutingObserver( CRoutingObserver& aObserver ); 
     	TInt RemoveRoutingObserver( CRoutingObserver& aObserver );
		TInt AppendDownlinkVolumeObserver( CDownlinkVolumeObserver& aObserver );
		TInt RemoveDownlinkVolumeObserver( CDownlinkVolumeObserver& aObserver );
		TInt AppendUplinkGainObserver( CUplinkGainObserver& aObserver );
		TInt RemoveUplinkGainObserver( CUplinkGainObserver& aObserver );
		TInt GetAvailableOutputs( RArray<CCallAudioControl::TAudioOutput>& aAvailableOutputs );
		TInt GetOutput( CCallAudioControl::TAudioOutput& aOutput );
		TInt SetOutput( CCallAudioControl::TAudioOutput aOutput );
		TInt PreviousOutput( CCallAudioControl::TAudioOutput& aPrevOutput );
		TInt GetMinDownlinkVolume( TUint& aVolume );
		TInt GetDownlinkVolume( TUint& aVolume );
		TInt GetMaxDownlinkVolume( TUint& aVolume );
		TInt SetDownlinkVolume( TUint aVolume );
		TInt GetUplinkMute( TBool& aMute );
    	TInt SetUplinkMute( TBool aMute );
		void NotifyL(TAction aAction, TInt aError, TInt aValue);		    	
        
    private:
    
    	// For MTelephonyAudioRoutingObserver:
		void AvailableOutputsChanged( CTelephonyAudioRouting& aTelephonyAudioRouting/*, const TArray<CTelephonyAudioRouting::TAudioOutput>& aOutputs*/);
		void SetOutputComplete( CTelephonyAudioRouting& aTelephonyAudioRouting, TInt aErr);
  		void OutputChanged( CTelephonyAudioRouting& aTelephonyAudioRouting);   		  	
    	void ConstructL();
    	CCallAudioControlImpl();
		TInt GetVolume(); 
		TInt CheckIfOkToSetVolume();
		TInt VerifyAndSetVolume(TInt aVolume);
		TInt HandleCallStateChange();
		TInt HandleEPVolumeChange(TInt aVolume);
		TInt HandleIHFVolumeChange(TInt aVolume);
		TInt HandleMuteChange(TBool aMute);
		void NotifyVolumeObservers();
		void NotifyGainObservers();	
		void NotifyRoutingObservers(TUint aEvent, TInt aError);
		void NotifyRoutingObservers(TUint aEvent);		
		TInt SetMute(TBool aMute);
		TBool IsCallActive();		
#ifdef __SERIES60_31__
		// For MMediatorCommandResponseObserver
		void CommandResponseL( TUid /*aKMediatorSecondaryDisplayDomain*/, TUid /*aKTSECategory*/, 
                        TInt aCommandId,TInt aStatus, const TDesC8& /*aData*/ );				
															   	
#endif
    
    	// Data:
   		CTelephonyAudioRouting* iTelephonyAudioRouting;		
   		RPointerArray<CRoutingObserver> iRoutingObservers;
    	RPointerArray<CDownlinkVolumeObserver> iDownlinkVolumeObservers;
   		RPointerArray<CUplinkGainObserver> iUplinkGainObservers;    	  		
   		CCallAudioControl::TAudioOutput iCurrentAudioOutput;
   		CCallAudioControl::TAudioOutput iPreviousAudioOutput;
		CCallAudioControlProperty* iCsNotifier;
		CCallAudioControlRepository* iEpVolumeNotifier;
		CCallAudioControlRepository* iIhfVolumeNotifier;
		CCallAudioControlProperty* iMuteNotifier; 

#ifdef __SERIES60_31__
 		CMediatorCommandInitiator* 	iCommandInitiator;
	    RPhCltServer iPhoneClientServer;
	    RLibrary iLibrary;
#else
        CPhCltCommandHandler*   iPhoneClient;
#endif 			  		  		
   		TInt iCallState;
		TInt iVolume;
		/* Needed to prevent notification going to the client modifying mute state */
        TBool iPendingMuteSetOp; /* Holds true if there is a pending setmute opreation */
        TBool iRequestedMuteState; /* Holds the current mute state */
        TBool iPendingEpVolLevelOp;
        TBool iRequestedEpVolLevel;
        TBool iPendingIhfVolLevelOp;
        TBool iRequestedIhfVolLevel;

		
        TBool iWinsTesting;
    };
    
#endif      // CALLAUDIOCONTROLIMPL_H

// End of File
