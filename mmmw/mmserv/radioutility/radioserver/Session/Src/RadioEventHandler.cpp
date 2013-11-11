/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object for each spontaneous event from Radio Server.
*				 Notifies MRadioEventObserver upon receiving a new event.
*
*/



// INCLUDE FILES
#include    "RadioEventHandler.h"
#include    "RadioClientServer.h"
#include    "RadioDebug.h"
#include    "trace.h"

// This has to be the last include. 
#ifdef STUB_CONSTELLATION
#   include "RadioStubManager.h"
#   define KRadioServerPropertyCategory KStub_KRadioServerPropertyCategory
#endif //STUB_CONSTELLATION

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioEventHandler::CRadioEventHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRadioEventHandler::CRadioEventHandler(
	MRadioObserver& aObserver,
	RRadioSession& aSession,
	TUint aKey )
	:	CActive(EPriorityStandard),
		iSession(aSession),
		iKey(aKey),
		iObserver(aObserver)
    {
    FUNC_LOG;
    if ( iKey == ERadioServPsAfSearchEnd )
    	{
		// This is necessary to make sure AfSearchEnd comes in before FrequencyChanged event.
		// Because the AO for FrequencyChanged is added to the scheduler first, AfSearchEnd
		// must have slightly higer priority to come in first.
    	SetPriority(EPriorityUserInput);
    	}
    }

// -----------------------------------------------------------------------------
// CRadioEventHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioEventHandler::ConstructL()
    {
    FUNC_LOG;
	CActiveScheduler::Add(this);
	User::LeaveIfError( iProperty.Attach(KRadioServerPropertyCategory, iKey) );
	iProperty.Subscribe(iStatus);
	SetActive();
    }

// -----------------------------------------------------------------------------
// CRadioEventHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioEventHandler* CRadioEventHandler::NewLC(
	MRadioObserver& aObserver,
	RRadioSession& aSession,
	TUint aKey )
    {
    FUNC_LOG;
    CRadioEventHandler* self = new( ELeave ) CRadioEventHandler(aObserver,aSession, aKey);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CRadioEventHandler::~CRadioEventHandler()
    {
    FUNC_LOG;
	if ( IsActive() )
		{
		Cancel();
		}
	iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CRadioEventHandler::DoCancel
// -----------------------------------------------------------------------------
//
void CRadioEventHandler::DoCancel()
    {
    FUNC_LOG;
	iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CRadioEventHandler::RunL
// -----------------------------------------------------------------------------
//
void CRadioEventHandler::RunL()
    {
    FUNC_LOG;
    // Subscribe immediately before analyzing the notification to ensure that we
    // don't miss further updates.
	iProperty.Subscribe(iStatus);
	SetActive();

	switch ( iKey )
		{
		case ERadioServPsPlayerState:
			{
			TPckgBuf<TRsSettingsData> playerState;
    		iProperty.Get(playerState);
            iObserver.RadioEventStateChange(playerState().iData1,	// Player state
               								playerState().iError);
			}
			break;
		case ERadioServPsAntennaStatus:
			{
			TBool attached;
			iProperty.Get(attached);
		    iObserver.RadioEventAntennaStatusChange(attached);
			}
			break;
		case ERadioServPsOfflineMode:
			{
			TBool offline;
			iProperty.Get(offline);
			iObserver.RadioEventOfflineModeChange(offline);
			}
			break;
		case ERadioServPsTransmitterStatus:
			{
			TBool transmitter;
			iProperty.Get(transmitter);
			iObserver.RadioEventTransmitterStatusChange(transmitter);
			}
			break;
		case ERadioServPsFrequency:
			{
			TInt frequency;
			iProperty.Get(frequency);
			iObserver.RadioEventFrequencyChange(frequency);
			}
			break;
		case ERadioServPsFrequencyRange:
			{
			TInt freqRange;
			iProperty.Get(freqRange);
			iObserver.RadioEventFrequencyRangeChanged((TRsFrequencyRange) freqRange);
			}
			break;
		case ERadioServPsForceMonoReception:
		    {
		    TBool forcedMono;
		    iProperty.Get(forcedMono);
			iObserver.RadioEventForcedMonoChanged(forcedMono);
			}
			break;
		case ERadioServPsSquelch:
			{
			TBool squelch;
			iProperty.Get(squelch);
			iObserver.RadioEventSquelchChanged(squelch);
			}
			break;
		case ERadioServPsVolume:
			{
			TInt volume;
			iProperty.Get(volume);
			iObserver.RadioEventVolumeChange(volume);
			}
			break;
		case ERadioServPsMuteStatus:
			{
			TBool muted;
			iProperty.Get(muted);
			iObserver.RadioEventMuteChange(muted);
			}
			break;
		case ERadioServPsBalance:
			{
			TPckgBuf<TRsSettingsData> balance;
			iProperty.Get(balance);
			iObserver.RadioEventBalanceChange(balance().iData1,		// Left
			                                  balance().iData2);	// Right
			}
			break;
		case ERadioServPsRdsSignalStatus:
			{
			TBool signal;
   			iProperty.Get(signal);
       		iObserver.RadioEventRdsSignalChange(signal);
			}
			break;
		case ERadioServPsAutoSwitchStatus:
			{
			TBool autoSwitch;
			iProperty.Get(autoSwitch);
			iObserver.RadioEventRdsAutomaticSwitchingChange(autoSwitch);
			}
			break;
		case ERadioServPsProgrammeIdentification:
			{
			TInt pi;
			iProperty.Get(pi);
			iObserver.RadioEventRdsDataPI(pi);
			}
			break;
	    case ERadioServPsProgrammeType:
	        {
			TRsRdsProgrammeType pty;
			iProperty.Get(pty);
	        iObserver.RadioEventRdsDataPTY(pty);
			}
			break;
		case ERadioServPsProgrammeService:
			{
			TRsRdsPSName ps;
	 		iProperty.Get(ps);
			iObserver.RadioEventRdsDataPS(ps);
			}
			break;
		case ERadioServPsRadioText:
			{
			TRsRdsRadioText rt;
			iProperty.Get(rt);
	        iObserver.RadioEventRdsDataRT(rt);
			}
			break;
    case ERadioServPsRadioTextPlusObjects:
      {
      TPckgBuf<TRsRdsRTPlusObjectData> objData;
      TUint32 iClassType1;
      TUint32 iClassType2;
      TRsRdsRadioText aRTobject1;
      TRsRdsRadioText aRTobject2;

      iProperty.Get(objData);
      
      aRTobject1 = objData().aRtObj1;
      iClassType1 = objData().iContentType1;
      aRTobject2 = objData().aRtObj2;
      iClassType2 = objData().iContentType2;

      iObserver.RadioEventRdsDataRTplus( (TRsRdsRTplusClass)iClassType1, aRTobject1 );
      iObserver.RadioEventRdsDataRTplus( (TRsRdsRTplusClass)iClassType2, aRTobject2 );
      }
      break;
		case ERadioServPsClockTime:
			{
			TPckgBuf<TDateTime> rdsCt;
			iProperty.Get(rdsCt);
			iObserver.RadioEventRdsDataCT(rdsCt());
			}
			break;
		case ERadioServPsTrafficAnnouncementStatus:
			{
			TBool taOn;
 			iProperty.Get(taOn);
			iObserver.RadioEventRdsDataTA(taOn);
			}
			break;
		case ERadioServPsAfSearchBegin:
			{
			iObserver.RadioEventRdsSearchBeginAF();
			}
			break;
		case ERadioServPsAfSearchEnd:
			{
			TPckgBuf<TRsSettingsData> afData;
			iProperty.Get(afData);
			iObserver.RadioEventRdsSearchEndAF(afData().iError,
				                     afData().iData1);	// Frequency
			}
			break;
		default:
			{
			INFO("ERROR case default !!!");
			User::Panic(_L("RadioServer"), KErrGeneral );
			}
			break;
		}
	}

//  End of File

