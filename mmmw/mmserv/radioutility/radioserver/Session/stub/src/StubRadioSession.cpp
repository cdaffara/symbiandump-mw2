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
* Description:  This class is the main interface to the RadioServer. It implements
*				 the client-side session.
*
*/



// INCLUDE FILES
#include 	<e32math.h>
#include    "StubRadioSession.h"
#include    "RadioClientServer.h"
#include	"RadioRequest.h"
#include	"RadioEventHandler.h"
#include    "RadioDebug.h"
#include    "trace.h"

// CONSTANTS
// Index to list of asynchronous requests that generates response to MRadioObserver.
const TInt KReqInternal				 = 0;
const TInt KReqRequestTunerControl   = 1;
const TInt KReqSetFrequencyRange     = 2;
const TInt KReqSetFrequency          = 3;
const TInt KReqStationSeek           = 4;
const TInt KReqPlay					 = 5;
const TInt KReqStationSeekByPTY      = 6;
const TInt KReqStationSeekByTA       = 7;
const TInt KReqStationSeekByTP       = 8;

// Max number of retries to start the server
const TInt KRadioStartRetry	= 2;
// Minimum of one for each asynchronous message + one to allow cancel
const TInt KRadioMessageSlots = 11;

#define TUNER  iRadioStubManager->iTuner
#define PLAYER  iRadioStubManager->iPlayer
#define RDS  iRadioStubManager->iRds


// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// StartServer
// Static function to start the server process thread.
// Start the server process/thread which lives in an EPOCEXE object.
// Returns: TInt: KErrNone (0) if no error
// -----------------------------------------------------------------------------
//
static TInt StartServer()
	{
    FUNC_LOG;

    const TUidType serverUid(KNullUid, KNullUid, KRadioServerUid3);

    // We just create a new server process. Simultaneous launching of two such
    // processes should be detected when the second one attempts to create
    // the server object, failing with KErrAlreadyExists.
    RProcess server;
    TInt r = server.Create(KRadioServerImg, KNullDesC, serverUid);

    if ( r != KErrNone )
        {
        INFO_1("server.Create() failed [%d]", r);
        return r;
        }
    TRequestStatus stat;
    server.Rendezvous(stat);
    if ( stat != KRequestPending )
        {
        server.Kill(0);     // abort startup
        }
    else
        {
        server.Resume();    // logon OK - start the server
        }
    User::WaitForRequest(stat);     // wait for start or death
    // we can't use the 'exit reason' if the server panicked as this
    // is the panic 'reason' and may be '0' which cannot be distinguished
    // from KErrNone
    r = (server.ExitType() == EExitPanic) ? KErrGeneral : stat.Int();
    server.Close();
    return r;
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RRadioSession::NewL
// Two-phased constructor except no need for ConstrucL
// -----------------------------------------------------------------------------
//
EXPORT_C RRadioSession* RRadioSession::NewL()
    {
    FUNC_LOG;
    RRadioSession* self = new (ELeave) RRadioSession();
    return self;
    }

// -----------------------------------------------------------------------------
// RRadioSession::RRadioSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RRadioSession::RRadioSession()
	:	iConnected(EFalse),
		iDestinationPckg(TMMFMessageDestination(KRadioServerInterfaceUid, KRadioServerObjectHandle)),
		iObserver(NULL),
		iPrimaryClient(EFalse),
		iRdsNotify(EFalse)
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// RRadioSession::Connect
// Create a client-side session. Start the server if not started already.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::Connect(
	MRadioObserver& aObserver,
	TBool aPrimaryClient )
	{
    FUNC_LOG;
    INFO_1("Size of RRadioSession: %i", sizeof(RRadioSession) );
    // check if already connected
    if ( iConnected )
        {
        INFO("already connected");
        return KErrAlreadyExists;
        }

    TInt retry = KRadioStartRetry;
    TInt err = KErrGeneral;
    TInt numMessageSlots = KRadioMessageSlots;
    for (;;)
        {
        // Try to create a new session with the server
        err = CreateSession(KRadioServerName, Version(), numMessageSlots);
        if ( (err != KErrNotFound) && (err != KErrServerTerminated) )
            {
            break;  // Connected to existing server - ok
            }
        if ( --retry == 0 )
            {
            break;  // Failed.
            }
        // Server not running, try to start it.
        err = StartServer();
        if ( (err != KErrNone) && (err != KErrAlreadyExists) )
            {
            break;  // Server not launched - propagate error
            }
        }

    if ( err != KErrNone )
        {
        INFO_1("Unable to start server [%d]", err);
        return err;
        }
    // Dooing some temporary manipulation for heap
    User::Check();
    User::CompressAllHeaps();
    CRadioRequest* req( NULL );
    TRAP(err, req = CRadioRequest::NewLC(*this, aObserver, ERadioServNone); CleanupStack::Pop( req ) );
    iExtraRequestForBugHunting = req;   
    User::Check();
    User::CompressAllHeaps();
    //delete iExtraRequestForBugHunting;
    //iExtraRequestForBugHunting = NULL;
    
    // Create active object request handlers and add it to scheduler
    TRAP(err, StartRequestHandlersL(aObserver));
    if ( err == KErrNone )
        {
        // Create active object event handlers and add it to scheduler
        TRAP(err, StartEventHandlersL(aObserver));
        if ( err == KErrNone )
            {
            iConnected = ETrue;
            iObserver = &aObserver;
            if ( aPrimaryClient )
                {
                iPrimaryClient = ETrue;
                err = SendReceive(ERadioServAddPrimaryClient, TIpcArgs(&iDestinationPckg));
                }
            }
        else
            {
            iRequests.ResetAndDestroy();
            iEventHandlers.ResetAndDestroy();
            }
        }
    else
        {
        iRequests.ResetAndDestroy();
        }
    // Open chunk for test configuration/control data
    // Open chunk for test configuration/control data
    User::Check();
    User::CompressAllHeaps();
    err = iRadioStubManagerChunk.OpenGlobal(
            KRadioStubManagerLocalChunkName,
            EFalse, // == Read | Write
            EOwnerThread );
    if (err)
        {
        goto exitAndReturn;
        }
    User::Check();
    User::CompressAllHeaps();
    if ( sizeof(SRadioStubManager) > iRadioStubManagerChunk.MaxSize() )
        {
        err =  KErrTooBig;
        goto exitAndReturn;
        }
    TUint8* basePtr = iRadioStubManagerChunk.Base();
    User::LeaveIfNull( basePtr );    
    if (!basePtr)
        {
        err = KErrGeneral;
        goto exitAndReturn;
        }    
    User::Check();
    User::CompressAllHeaps();
    iRadioStubManager = (SRadioStubManager*)basePtr;    

exitAndReturn:    
    User::Check();
    User::CompressAllHeaps();
    INFO_1("exit err=[%d]", err);
    return err;
	}

// -----------------------------------------------------------------------------
// RRadioSession::Version
// -----------------------------------------------------------------------------
//
EXPORT_C TVersion RRadioSession::Version() const
	{
	return(TVersion(KRadioServerVersionMajor, KRadioServerVersionMinor, KRadioServerVersionBuild));
	}

// -----------------------------------------------------------------------------
// RRadioSession::Close
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::Close()
	{
    FUNC_LOG;

    //delete iExtraRequestForBugHunting;
    iEventHandlers.ResetAndDestroy();
    iEventHandlers.Close();

    iRdsEventHandlers.ResetAndDestroy();
    iRdsEventHandlers.Close();

    for ( TInt i = (iRequests.Count() - 1) ; i >= 0; i = (iRequests.Count()- 1)  )
        {
        INFO_2("iRequests[i] pointer: 0x%x, no: %i", *&iRequests[i], i );
        CRadioRequest* req = *&iRequests[i];
        iRequests.Remove(i);
        delete req;
        }   
    //iRequests.ResetAndDestroy();
    iRequests.Close();

    if ( iConnected )
        {
        iConnected = EFalse;
        if ( iPrimaryClient )
            {
            SendReceive(ERadioServRemovePrimaryClient, TIpcArgs(&iDestinationPckg));
            }
        RSessionBase::Close();
        }
    // Dooing some temporary manipulation for heap
    //User::Check();
    //User::CompressAllHeaps();
    delete iExtraRequestForBugHunting;
	}

//********** TunerUtility control begins

// -----------------------------------------------------------------------------
// RRadioSession::RequestTunerControl
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::RequestTunerControl(
	TRsTuner /*aTuner*/ )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        if ( !iRequests[KReqRequestTunerControl]->IsActive() )
            {
            iRequests[KReqRequestTunerControl]->CompleteRequest(ERadioServRequestTunerControl, KErrNone);
            }
		}
	else
		{
		iRequests[KReqInternal]->CompleteRequest(ERadioServRequestTunerControl, KErrDisconnected);
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetTunerCapabilities
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetTunerCapabilities(
	TRsTunerCapabilities& aCaps ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetCapabilitiesError.Error() );
        if ( KErrNone == err )
            {
            aCaps.iFrequencyRange = TUNER.iCaps.iFrequencyRange;
            aCaps.iCapabilities = TUNER.iCaps.iCapabilities;
            aCaps.iAdditionalFunctions1 = TUNER.iCaps.iAdditionalFunctions1;
            aCaps.iAdditionalFunctions2 = TUNER.iCaps.iAdditionalFunctions2;            
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::EnableTunerInOfflineMode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::EnableTunerInOfflineMode(
	TBool aEnable )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iEnableTunerInOfflineModeError.Error() );
        if ( KErrNone == err )
            {
            TUNER.iTunerInOfflineMode = aEnable;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::SetFrequencyRange
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::SetFrequencyRange(
	TRsFrequencyRange aRange )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iSetFrequencyRangeError.Error() );
        if ( KErrNone == err )
            {
            TUNER.iPreviousRange = TUNER.iRange;
            TUNER.iRange = aRange;
            }
        if ( iRequests[KReqSetFrequencyRange]->IsActive() )
            {
            iRequests[KReqSetFrequencyRange]->Cancel();
            }
        iRequests[KReqSetFrequencyRange]->CompleteRequest(ERadioServSetFrequencyRange, err);
		}
	else
		{
		iRequests[KReqInternal]->CompleteRequest(ERadioServSetFrequencyRange, KErrDisconnected);
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelSetFrequencyRange
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelSetFrequencyRange()
	{
    FUNC_LOG;
	if ( iConnected && iRequests[KReqSetFrequencyRange]->IsActive() )
		{
		iRequests[KReqSetFrequencyRange]->Cancel();
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetFrequencyRange
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetFrequencyRange(
	TRsFrequencyRange& aRange,
	TInt& aMinFreq,
	TInt& aMaxFreq) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetFrequencyRangeError.Error() );
        if ( KErrNone == err )
            {
            aRange = TUNER.iRange;
            aMinFreq = TUNER.iMinFreq;
            aMaxFreq = TUNER.iMaxFreq;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::SetFrequency
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::SetFrequency(
	TInt aFrequency )
	{
    FUNC_LOG;
	if ( iConnected )
		{
	    TUint err( TUNER.iSetFrequencyError.Error() );
	    if  ( KErrNone == err )
	        {
	        TUNER.iPreviousFrequency = TUNER.iFrequency;
	        TUNER.iFrequency = aFrequency;
	        }       
        if ( iRequests[KReqSetFrequency]->IsActive() )
            {
            iRequests[KReqSetFrequency]->Cancel();
            }
        iRequests[KReqSetFrequency]->CompleteRequest(ERadioServSetFrequency, err);
		}
	else
		{
		iRequests[KReqInternal]->CompleteRequest(ERadioServSetFrequency, KErrDisconnected);
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelSetFrequency
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelSetFrequency()
	{
    FUNC_LOG;
	if ( iConnected && iRequests[KReqSetFrequency]->IsActive() )
		{
		iRequests[KReqSetFrequency]->Cancel();
		}
    TUNER.iFrequency = TUNER.iPreviousFrequency;
    TUNER.iPreviousFrequency = 0;
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetFrequency
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetFrequency(
	TInt& aFrequency ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetFrequencyError.Error() );
        if ( KErrNone == err )
            {
            aFrequency = TUNER.iFrequency;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::StationSeek
// -----------------------------------------------------------------------------
EXPORT_C void RRadioSession::StationSeek(
	TBool aUpwards )
	{
    FUNC_LOG;
    INFO_1("aUpwards: %D", aUpwards );
	if ( iConnected )
		{
	    TUNER.iStationSeekUpwards = aUpwards;
        if ( iRequests[KReqStationSeek]->IsActive() )
            {
            iRequests[KReqStationSeek]->Cancel();
            }
        TInt err( KErrNone);
        if ( KErrNotFound == TUNER.iScanStations.iCount )
            {
            err = KErrNotFound;
            }
        else if ( TUNER.iScanStations.iCount )
            {
            iRequests[KReqStationSeek]->iInt = TUNER.iScanStations.iMinFreq + ( TUNER.iScanStations.iCount * TUNER.iScanStations.iFrequencyStepSize );
            --TUNER.iScanStations.iCount;
            if ( 0 == TUNER.iScanStations.iCount )
                {
                TUNER.iScanStations.iCount = KErrNotFound;
                }
            }
        else
            {
            iRequests[KReqStationSeek]->iInt = TUNER.iFrequency;
            }
        iRequests[KReqStationSeek]->CompleteRequest(ERadioServStationSeek, err);
		}
	else
		{
		iRequests[KReqInternal]->CompleteRequest(ERadioServStationSeek, KErrDisconnected);
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelStationSeek
// -----------------------------------------------------------------------------
EXPORT_C void RRadioSession::CancelStationSeek()
	{
    FUNC_LOG;
	if ( iConnected && iRequests[KReqStationSeek]->IsActive() )
		{
		iRequests[KReqStationSeek]->Cancel();
		}
	TUNER.iScanStations.iCount = 0;
    TUNER.iStationSeekUpwards = EFalse;
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetSignalStrength
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetSignalStrength(
	TInt& aSignalStrength ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetSignalStrengthError.Error() );
        if ( KErrNone == err )
            {
            aSignalStrength = TUNER.iSignalStrength;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetMaxSignalStrength
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetMaxSignalStrength(
	TInt& aMaxSignalStrength ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetMaxSignalStrengthError.Error() );
        if ( KErrNone == err )
            {
            aMaxSignalStrength = TUNER.iMaxSignalStrength;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetStereoMode
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetStereoMode(
	TBool& aStereo ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetStereoModeError.Error() );
        if ( KErrNone == err )
            {
            aStereo = TUNER.iStereoMode;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::ForceMonoReception
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::ForceMonoReception(
	TBool aForcedMono )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iForcedMonoReceptionError.Error() );
        if ( KErrNone == err )
            {
            TUNER.iForcedMono = aForcedMono;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetForceMonoReception
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetForceMonoReception(
	TBool& aForcedMono ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetForcedMonoReceptionError.Error() );
        if ( KErrNone == err )
            {
            aForcedMono = TUNER.iForcedMono;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::SetSquelch
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::SetSquelch(
	TBool aEnabled )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iSetSquelchError.Error() );
        if ( KErrNone == err )
            {
            TUNER.iSquelch = aEnabled;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetSquelch
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetSquelch(
	TBool& aSquelch ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( TUNER.iGetSquelchError.Error() );
        if ( KErrNone == err )
            {
            aSquelch = TUNER.iSquelch;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

//********** PlayerUtility control begins

// -----------------------------------------------------------------------------
// RRadioSession::PlayerState
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::PlayerState(
	TRsPlayerState& aState ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
	    aState = PLAYER.iPlayerState;
        return KErrNone;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::Play
// -----------------------------------------------------------------------------
EXPORT_C void RRadioSession::Play()
	{
    FUNC_LOG;
	if ( iConnected )
		{
		PLAYER.iPlayerState = ERsPlayerPlaying;
        if ( iRequests[KReqPlay]->IsActive() )
            {
            iRequests[KReqStationSeek]->Cancel();
            }
        iRequests[KReqPlay]->CompleteRequest(ERadioServPlay, KErrNone);
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::Stop
// -----------------------------------------------------------------------------
EXPORT_C void RRadioSession::Stop(
	TBool aIfOnlyPrimaryClient )
	{
    FUNC_LOG;
    INFO_1("aIfOnlyPrimaryClient: [%d]", aIfOnlyPrimaryClient);
    if ( iConnected & aIfOnlyPrimaryClient )
        {
        PLAYER.iPlayerState = ERsPlayerIdle;
        RProperty::Set( KRadioServerPropertyCategory, ERadioServPsPlayerState,  PLAYER.iPlayerState );
        }
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetMaxVolume
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetMaxVolume(
    TInt& aMaxVolume ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( PLAYER.iGetMaxVolumeError.Error() );
        if ( KErrNone == err )
            {
            aMaxVolume = PLAYER.iMaxVolume;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::SetVolume
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::SetVolume(
	TInt aVolume )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( PLAYER.iSetVolumeError.Error() );
        if ( KErrNone == err )
            {
            PLAYER.iVolume = aVolume;
            RProperty::Set( KRadioServerPropertyCategory, ERadioServPsVolume, PLAYER.iVolume );      
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetVolume
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetVolume(
	TInt& aVolume ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( PLAYER.iGetVolumeError.Error() );
        if ( KErrNone == err )
            {
            aVolume = PLAYER.iVolume;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::SetVolumeRamp
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::SetVolumeRamp(
	const TTimeIntervalMicroSeconds& aRampInterval )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( PLAYER.iSetVolumeRampError.Error() );
        if ( KErrNone == err )
            {
            PLAYER.iRampInterval = aRampInterval;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::Mute
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::Mute(
	TBool aMute )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( PLAYER.iMuteError.Error() );
        if ( KErrNone == err )
            {
            PLAYER.iMuteStatus = aMute;
            RProperty::Set( KRadioServerPropertyCategory, ERadioServPsMuteStatus, PLAYER.iMuteStatus );      
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetMuteStatus
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::GetMuteStatus(
	TBool& aMute ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
		aMute = PLAYER.iMuteStatus;
		return KErrNone;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::SetBalance
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::SetBalance(
	TInt aLeftPercentage,
	TInt aRightPercentage )
	{
    FUNC_LOG;
	INFO_2("left=[%d] right=[%d]", aLeftPercentage, aRightPercentage);
	if ( iConnected )
		{
		TInt err( PLAYER.iSetBalanceError.Error() );
		if ( KErrNone == err )
		    {
	        PLAYER.iLeftPercentage = aLeftPercentage;
	        PLAYER.iRightPercentage = aRightPercentage;
	        //iRadioPlayerUtilityClient.MrpoBalanceChange( STUB.iLeftPercentage, STUB.iRightPercentage );
	        TPckgBuf<TRsSettingsData> balance;
	        balance().iData1 = aLeftPercentage;     // Left
	        balance().iData2 = aRightPercentage;    // Right
	        RProperty::Set( KRadioServerPropertyCategory, ERadioServPsBalance,  balance );      
		    }
	    return err;

		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetBalance
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetBalance(
	TInt& aLeftPercentage,
	TInt& aRightPercentage ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( PLAYER.iSetBalanceError.Error() );
        if ( KErrNone == err )
            {
            aLeftPercentage = PLAYER.iLeftPercentage;
            aRightPercentage = PLAYER.iRightPercentage;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

//********** RDSUtility control begins

// -----------------------------------------------------------------------------
// RRadioSession::GetRdsCapabilities
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetRdsCapabilities(
	TRsRdsCapabilities& aCaps ) const
	{
    FUNC_LOG;
	if ( iConnected )
		{
		TInt err( RDS.iGetCapabilitiesError.Error() );
	    if ( KErrNone == err )
	        {
	        aCaps.iRdsFunctions = RDS.iCaps.iRdsFunctions;
	        aCaps.iAdditionalFunctions1 = RDS.iCaps.iAdditionalFunctions1;
	        aCaps.iAdditionalFunctions2 = RDS.iCaps.iAdditionalFunctions2;
	        }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetRdsSignalStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetRdsSignalStatus(
	TBool& aRdsSignal ) const
    {
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetRdsSignalStatusError.Error() );
        if ( KErrNone == err )
            {
            aRdsSignal = RDS.iRdsSignal;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::NotifyRdsDataChange
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::NotifyRdsDataChange(
	TRsRdsData aRdsData )
	{
    FUNC_LOG;
	INFO_1("aRdsData.iRdsFunctions [%d]", aRdsData.iRdsFunctions);
	if ( !iConnected )
		{
		return KErrDisconnected;
		}

	if ( !iObserver )
		{
		return KErrNotReady;
		}

	TRAPD(err, StartRdsEventHandlersL(aRdsData.iRdsFunctions));
	if ( err != KErrNone )
		{
		INFO_1("err=[%d]", err);
		return err;
		}
    iRdsNotify = ETrue;
	return err;
    }

// -----------------------------------------------------------------------------
// RRadioSession::CancelNotifyRdsDataChange
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelNotifyRdsDataChange()
    {
    FUNC_LOG;
	if ( iConnected && iRdsNotify )
		{
		iRdsEventHandlers.ResetAndDestroy();
	    iRdsNotify = EFalse;
		}
    }

// -----------------------------------------------------------------------------
// RRadioSession::NotifyRadioTextPlusChange
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::NotifyRadioTextPlusChange(
	RArray<TInt>& /*aRtPlusClasses*/ )
	{
    FUNC_LOG;
	return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelNotifyRadioTextPlusChange
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelNotifyRadioTextPlusChange()
	{
    FUNC_LOG;
	// NotifyRadioTextPlusChange is not supported
	}

// -----------------------------------------------------------------------------
// RRadioSession::SetAutomaticSwitching
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::SetAutomaticSwitching(
	TBool aAuto )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iSetAutomaticSwitchingError.Error() );
        if ( KErrNone == err )
            {
            RDS.iAutomaticSwitching = aAuto;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetAutomaticSwitching
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetAutomaticSwitching(
	TBool& aAuto )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetAutomaticSwitchingError.Error() );
        if ( KErrNone == err )
            {
            aAuto = RDS.iAutomaticSwitching;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelAFSearch
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelAFSearch()
	{
    FUNC_LOG;
	}

 // -----------------------------------------------------------------------------
// RRadioSession::SetAutomaticTrafficAnnouncement
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::SetAutomaticTrafficAnnouncement(
	TBool /*aAuto*/ )
	{
    FUNC_LOG;
	return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetAutomaticTrafficAnnouncement
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetAutomaticTrafficAnnouncement(
	TBool& /*aAuto*/ )
	{
    FUNC_LOG;
	return KErrNotSupported;
   	}

// -----------------------------------------------------------------------------
// RRadioSession::StationSeekByPTY
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::StationSeekByPTY(
	TRsRdsProgrammeType aPty,
	TBool aSeekUp )
   	{
    FUNC_LOG;
   	INFO_2("pty=[%d], aSeekUp=%d", aPty, aSeekUp);
	if ( iConnected )
		{
        if ( iRequests[KReqStationSeekByPTY]->IsActive() )
            {
            iRequests[KReqStationSeekByPTY]->Cancel();
            }
		iRequests[KReqStationSeekByPTY]->iInt = RDS.iFrequency;
		iRequests[KReqInternal]->CompleteRequest(ERadioServStationSeekByPTY, KErrNone);
		}
	else
		{
		iRequests[KReqInternal]->CompleteRequest(ERadioServStationSeekByPTY, KErrDisconnected);
		}
   	}

// -----------------------------------------------------------------------------
// RRadioSession::StationSeekByTA
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::StationSeekByTA(
	TBool /*aSeekUp*/ )
   	{
    FUNC_LOG;
	if ( iConnected )
		{
        if ( iRequests[KReqStationSeekByTA]->IsActive() )
            {
            iRequests[KReqStationSeekByTA]->Cancel();
            }
        iRequests[KReqStationSeekByTA]->iInt = RDS.iFrequency;
	    iRequests[KReqInternal]->CompleteRequest(ERadioServStationSeekByTA, KErrNone);
		}
	else
		{
		iRequests[KReqInternal]->CompleteRequest(ERadioServStationSeekByTA, KErrDisconnected);
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::StationSeekByTP
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::StationSeekByTP(
	TBool /*aSeekUp*/ )
   	{
    FUNC_LOG;
	if ( iConnected )
		{
        if ( iRequests[KReqStationSeekByTP]->IsActive() )
            {
            iRequests[KReqStationSeekByTP]->Cancel();
            }
        iRequests[KReqStationSeekByTP]->iInt = RDS.iFrequency;
        iRequests[KReqInternal]->CompleteRequest(ERadioServStationSeekByTP, KErrNone);
		}
	else
		{
		iRequests[KReqInternal]->CompleteRequest(ERadioServStationSeekByTP, KErrDisconnected);
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelRdsStationSeek
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelRdsStationSeek()
  	{
    FUNC_LOG;
  	}

// -----------------------------------------------------------------------------
// RRadioSession::GetFreqByPTY
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::GetFreqByPTY(
	TRsRdsProgrammeType /*aPty*/ )
	{
    FUNC_LOG;
	iRequests[KReqInternal]->CompleteRequest(ERadioServGetFreqByPTY, KErrNotSupported);
  	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelGetFreqByPTY
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelGetFreqByPTY()
	{
    FUNC_LOG;
	// GetFreqByPTY is not supported
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetFreqByTA
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::GetFreqByTA()
	{
    FUNC_LOG;
	iRequests[KReqInternal]->CompleteRequest(ERadioServGetFreqByTA, KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// RRadioSession::CancelGetFreqByTA
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelGetFreqByTA()
    {
    FUNC_LOG;
    // GetFreqByTA is not supported
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetPSByPTY
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::GetPSByPTY(
	TRsRdsProgrammeType /*aPty*/ )
    {
    FUNC_LOG;
	iRequests[KReqInternal]->CompleteRequest(ERadioServGetPSByPTY, KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// RRadioSession::CancelGetPSByPTY
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelGetPSByPTY()
    {
    FUNC_LOG;
    // GetPSByPTY is not supported
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetPSByTA
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::GetPSByTA()
    {
    FUNC_LOG;
	iRequests[KReqInternal]->CompleteRequest(ERadioServGetPSByTA, KErrNotSupported);
    }

// -----------------------------------------------------------------------------
// RRadioSession::CancelGetPSByTA
// -----------------------------------------------------------------------------
//
EXPORT_C void RRadioSession::CancelGetPSByTA()
    {
    FUNC_LOG;
    // GetPSByTA is not supported
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetProgrammeIdentification
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetProgrammeIdentification(
	TInt& aPi )
    {
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetProgrammeIdentificationError.Error() );
        if ( KErrNone == err )
            {
            aPi = RDS.iPi;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetProgrammeType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetProgrammeType(
	TRsRdsProgrammeType& aPty )
    {
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetProgrammeTypeError.Error() );
        if ( KErrNone == err )
            {
            aPty = RDS.iPty;
            }
        return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetProgrammeService
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetProgrammeService(
	TRsRdsPSName& aPs )
    {
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetProgrammeServiceError.Error() );
        if ( KErrNone == err )
            {
            aPs = RDS.iPs;
            }
        return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetRadioText
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetRadioText(
	TRsRdsRadioText& aRt )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetRadioTextError.Error() );
        if ( KErrNone == err )
            {
            aRt = RDS.iRt;
            }
        return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetRadioTextPlus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetRadioTextPlus(
	TRsRdsRTplusClass /*aRtPlusClass*/,
	TRsRdsRadioText& /*aRtPlusData*/ )
	{
    FUNC_LOG;
	return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetClockTime
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetClockTime(
	TDateTime& aCt )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetClockTimeError.Error() );
        if ( KErrNone == err )
            {
            aCt = RDS.iCt;
            }
        return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetTrafficAnnouncementStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetTrafficAnnouncementStatus(
	TBool& aTaStatus )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetTrafficAnnouncementStatusError.Error() );
        if ( KErrNone == err )
            {
            aTaStatus = RDS.iTaStatus;
            }
	    return err;
 		}
 	else
 		{
 		return KErrDisconnected;
 		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::GetTrafficProgrammeStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RRadioSession::GetTrafficProgrammeStatus(
	TBool& aTpStatus )
	{
    FUNC_LOG;
	if ( iConnected )
		{
        TInt err( RDS.iGetTrafficProgrammeStatusError.Error() );
        if ( KErrNone == err )
            {
            aTpStatus = RDS.iTpStatus;
            }
	    return err;
		}
	else
		{
		return KErrDisconnected;
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::CustomCommandSync
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::CustomCommandSync(
    const TMMFMessageDestinationPckg& aDestination,
    TInt aFunction,
    const TDesC8& aDataTo1,
    const TDesC8& aDataTo2 )
    {
    FUNC_LOG;
    if ( iConnected )
        {
        return SendReceive(aFunction, TIpcArgs(&aDestination, &aDataTo1, &aDataTo2));
        }
    else
        {
        return KErrDisconnected;
        }
	}

// -----------------------------------------------------------------------------
// RRadioSession::CustomCommandSync
// -----------------------------------------------------------------------------
EXPORT_C TInt RRadioSession::CustomCommandSync(
    const TMMFMessageDestinationPckg& aDestination,
    TInt aFunction,
    const TDesC8& aDataTo1,
    const TDesC8& aDataTo2,
    TDes8& aDataFrom )
    {
    FUNC_LOG;
    if ( iConnected )
        {
        return SendReceive(aFunction, TIpcArgs(&aDestination, &aDataTo1, &aDataTo2, &aDataFrom));
        }
    else
        {
        return KErrDisconnected;
        }
	}

// -----------------------------------------------------------------------------
// RRadioSession::CustomCommandAsync
// -----------------------------------------------------------------------------
EXPORT_C void RRadioSession::CustomCommandAsync(
    const TMMFMessageDestinationPckg& aDestination,
    TInt aFunction,
    const TDesC8& aDataTo1,
    const TDesC8& aDataTo2,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;
    if ( iConnected )
        {
        SendReceive(aFunction, TIpcArgs(&aDestination, &aDataTo1, &aDataTo2), aStatus);
        }
    else
        {
        TRequestStatus* stat = &aStatus;
        User::RequestComplete(stat, KErrDisconnected);
        }
	}

// -----------------------------------------------------------------------------
// RRadioSession::CustomCommandAsync
// -----------------------------------------------------------------------------
EXPORT_C void RRadioSession::CustomCommandAsync(
    const TMMFMessageDestinationPckg& aDestination,
    TInt aFunction,
    const TDesC8& aDataTo1,
    const TDesC8& aDataTo2,
    TDes8& aDataFrom,
    TRequestStatus& aStatus )
    {
    FUNC_LOG;
    if ( iConnected )
        {
        SendReceive(aFunction, TIpcArgs(&aDestination, &aDataTo1, &aDataTo2, &aDataFrom), aStatus);
        }
    else
        {
        TRequestStatus* stat = &aStatus;
        User::RequestComplete(stat, KErrDisconnected);
        }
	}

// -----------------------------------------------------------------------------
// RRadioSession::CancelRequest
// -----------------------------------------------------------------------------
//
void RRadioSession::CancelRequest(
	TInt aRequest )
	{
    FUNC_LOG;
	if ( iConnected )
		{
		SendReceive(ERadioServCancel, TIpcArgs(&iDestinationPckg, aRequest));
		}
	}

// -----------------------------------------------------------------------------
// RRadioSession::StartRequestHandlersL
// Create asynchronous request handlers.
// ** NOTE: The order in which each handler is appended to the iRequests array is critical.
// Access to each request is done with constants declared at the beginning of this file.
// -----------------------------------------------------------------------------
//
void RRadioSession::StartRequestHandlersL(
	MRadioObserver& aObserver )
	{
    FUNC_LOG;

	CRadioRequest* req = NULL;

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServNone);
    User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
	INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServRequestTunerControl);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServSetFrequencyRange);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServSetFrequency);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServStationSeek);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServPlay);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServStationSeekByPTY);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServStationSeekByTA);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );

	req = CRadioRequest::NewLC(*this, aObserver, ERadioServStationSeekByTP);
	User::LeaveIfError( iRequests.Append(req) );
	CleanupStack::Pop( req );
    INFO_2("CRadioRequest* req: 0x%x, no: %i", req, iRequests.Count() );
	}

// -----------------------------------------------------------------------------
// RRadioSession::StartEventHandlersL
// -----------------------------------------------------------------------------
//
void RRadioSession::StartEventHandlersL(
	MRadioObserver& aEventObserver )
	{
    FUNC_LOG;

	CRadioEventHandler* handler = NULL;
	
	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsPlayerState);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsAntennaStatus);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsOfflineMode );
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsTransmitterStatus );
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsFrequency);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

    handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsFrequencyRange);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsForceMonoReception);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsSquelch);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsVolume);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsMuteStatus);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(aEventObserver, *this, ERadioServPsBalance);
	User::LeaveIfError( iEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );
	}

// -----------------------------------------------------------------------------
// RRadioSession::StartRdsEventHandlersL
// -----------------------------------------------------------------------------
//
void RRadioSession::StartRdsEventHandlersL(
	TUint32 aRdsFunction )
	{
    FUNC_LOG;

	iRdsEventHandlers.ResetAndDestroy();
	CRadioEventHandler* handler = NULL;

	// If RDS notification started always return the latest RdsSignalStatus
	handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsRdsSignalStatus);
	User::LeaveIfError( iRdsEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

	handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsAutoSwitchStatus);
	User::LeaveIfError( iRdsEventHandlers.Append(handler) );
	CleanupStack::Pop( handler );

    if (aRdsFunction & TRsRdsData::ERsRdsProgrammeIdentification)
    	{
    	handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsProgrammeIdentification);
		User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );
    	}

    if (aRdsFunction & TRsRdsData::ERsRdsProgrammeType)
    	{
    	handler = CRadioEventHandler::NewLC(*iObserver, *this,ERadioServPsProgrammeType );
		User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );
    	}

	if (aRdsFunction & TRsRdsData::ERsRdsProgrammeService)
		{
		handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsProgrammeService);
		User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );
		}

	if(aRdsFunction & TRsRdsData::ERsRdsRadioText)
		{
		handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsRadioText);
		User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );
		}

  if(aRdsFunction & TRsRdsData::ERsRdsRadioTextPlus)
    {
    handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsRadioTextPlusObjects);
    User::LeaveIfError( iRdsEventHandlers.Append(handler) );
    CleanupStack::Pop( handler );
    }

	if (aRdsFunction & TRsRdsData::ERsRdsClockTime)
		{
	    handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsClockTime);
		User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );
		}

	if (aRdsFunction & TRsRdsData::ERsRdsTrafficAnnouncement)
		{
		handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsTrafficAnnouncementStatus);
		User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );
		}

	if (aRdsFunction & TRsRdsData::ERsRdsAlternateFrequency)
		{
	    handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsAfSearchBegin);
	    User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );

	    handler = CRadioEventHandler::NewLC(*iObserver, *this, ERadioServPsAfSearchEnd);
	    User::LeaveIfError( iRdsEventHandlers.Append(handler) );
		CleanupStack::Pop( handler );
		}
	}

//  End of File
