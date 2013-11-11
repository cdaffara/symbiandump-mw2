/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  mmftsplay test component
*
*/

#include "SimpleSoundPlayer.h"
#include <AudioPreference.h>

const TInt CSimpleSoundPlayer::KPlayerActionPlay = 1;
const TInt CSimpleSoundPlayer::KPlayerActionPause = 2;
const TInt CSimpleSoundPlayer::KPlayerActionStopPlayFile = 3;
const TInt CSimpleSoundPlayer::KPlayerActionReportPosition = 4;
const TInt CSimpleSoundPlayer::KPlayerActionReportDuration = 5;

const TInt CSimpleSoundPlayer::KPlayerActionStopPlayDes = 6;

const TInt CSimpleSoundPlayer::KPlayerActionReportVolume = 7;

const TInt CSimpleSoundPlayer::KPlayerActionStopPlayUrl = 8;

const TInt CSimpleSoundPlayer::KPlayerActionStopPlayFileHandler = 9;

const TInt CSimpleSoundPlayer::KPlayerActionReportDuration_Para = 10;
const TInt CSimpleSoundPlayer::KPlayerActionPauseResume = 11;




CSimpleSoundPlayer::CSimpleSoundPlayer(CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
			: iConsole(aConsole) , iImmediate(aImmediate) , iLogger(aLogger) , iSoundOpen(EFalse),
				iFinalError(KErrNone)
	{
	iSupervisor =0 ;
	iEventTarget=0;
	iParameters =0;
	iPlayerId =0 ;
	playVolume = false;
	metaInfo = false;
	playWindow = false;
	clearWindow = false;
	setBalance = false;
	iDontPlayAudioClip = false;  // Binh
	iIsLoopPlayEnabled = EFalse;
	}

void CSimpleSoundPlayer::ConstructUrlL(const TDesC &aUrl, TInt aIapId, const TDesC8 &aMime)
	{
	iMdaPlayer=CMdaAudioPlayerUtility::NewL(*this);
	//KUseDefaultIap
	iMdaPlayer->OpenUrlL(aUrl, aIapId, aMime );
	iSoundOpen=ETrue;

	_LIT(KPlayerCreated, "SoundPlayerCreated");
	iConsole.Printf(100, KPlayerCreated, KPlayerCreated);
	}
CSimpleSoundPlayer* CSimpleSoundPlayer::NewUrlL(const TDesC &aUrl, TInt aIapId, const TDesC8 &aMime, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = NewUrlLC(aUrl, aIapId, aMime, aConsole, aLogger, aImmediate);
	CleanupStack::Pop(self);
    return self;
	}
CSimpleSoundPlayer* CSimpleSoundPlayer::NewUrlLC(const TDesC &aUrl, TInt aIapId, const TDesC8 &aMime, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = new (ELeave) CSimpleSoundPlayer(aConsole, aLogger, aImmediate);
    CleanupStack::PushL(self);
    self->ConstructUrlL(aUrl, aIapId, aMime);
    return self;
	}


void CSimpleSoundPlayer::ConstructL(const TDesC8 &aDescriptor)
	{
	// Create a file audio player utility instance


	iMdaPlayer=CMdaAudioPlayerUtility::NewL(*this);		//Por Pasos
	iMdaPlayer->OpenDesL(aDescriptor);
	iSoundOpen=ETrue;

	_LIT(KPlayerCreated, "SoundPlayerCreated");
	iConsole.Printf(100, KPlayerCreated, KPlayerCreated);
	}







void CSimpleSoundPlayer::ConstructL(TBool aUseSharedHeap)
{

		iMdaPlayer=CMdaAudioPlayerUtility::NewL(*this, EMdaPriorityNormal,
	    EMdaPriorityPreferenceTimeAndQuality);


    	_LIT(KRootDir,"\\");
 	 	_LIT(KTestMP3File,"10s_smooth.mp3");
		TBuf8<60> buf2;
		RFs iFs2;
		User::LeaveIfError(iFs2.Connect());
	    TFindFile fileFind2(iFs2);
		TRAPD(err2,fileFind2.FindByDir(KTestMP3File,KRootDir));
		buf2.Copy((TDesC&) fileFind2.File());

		TBuf8<60> buf3;
		RFs iFs3;
		User::LeaveIfError(iFs3.Connect());
 	    TFindFile fileFind3(iFs3);
		TRAPD(err3,fileFind3.FindByDir(KTestMP3File,KRootDir));
		buf3.Copy((TDesC&) fileFind3.File());


        iMdaPlayer_1=CMdaAudioPlayerUtility::NewFilePlayerL(KTestMP3File, *this, EMdaPriorityNormal,
	    EMdaPriorityPreferenceTimeAndQuality);

	    iMdaPlayer_2=CMdaAudioPlayerUtility::NewDesPlayerL(buf2, *this);	// creación inmediata
		iMdaPlayer_3=CMdaAudioPlayerUtility::NewDesPlayerReadOnlyL(buf3, *this);


  	    iFs2.Close();
 	    iFs3.Close();

 	    iLogger.Log(_L("Trying to stop player") );
		iMdaPlayer_1->Stop();
		iLogger.Log(_L("Trying to close player") );
		iMdaPlayer_1->Close();
		iLogger.Log(_L("  Player stopped") );
    	delete iMdaPlayer_1;
    	iMdaPlayer_1 = NULL;

   		iLogger.Log(_L("Trying to stop player") );
		iMdaPlayer_2->Stop();
		iLogger.Log(_L("Trying to close player") );
		iMdaPlayer_2->Close();
		iLogger.Log(_L("  Player stopped") );
    	delete iMdaPlayer_2;
    	iMdaPlayer_2 = NULL;

    	iLogger.Log(_L("Trying to stop player") );
		iMdaPlayer_3->Stop();
		iLogger.Log(_L("Trying to close player") );
		iMdaPlayer_3->Close();
		iLogger.Log(_L("  Player stopped") );
    	delete iMdaPlayer_3;
    	iMdaPlayer_3 = NULL;


}

CSimpleSoundPlayer* CSimpleSoundPlayer::NewLC(CTestModuleIf &aConsole, CStifLogger &iLogger,
                                                    TBool aUseSharedHeap)
{
    CSimpleSoundPlayer* self = new (ELeave) CSimpleSoundPlayer(aConsole, iLogger);
    CleanupStack::PushL(self);
    self->ConstructL(aUseSharedHeap);
    return self;
}

CSimpleSoundPlayer* CSimpleSoundPlayer::NewL(CTestModuleIf &aConsole, CStifLogger &iLogger,
                                                    TBool aUseSharedHeap)
{
    RDebug::Print(_L("CSimpleSoundPlayer::NewL:"));

    CSimpleSoundPlayer* self = NewLC(aConsole, iLogger, aUseSharedHeap);
	CleanupStack::Pop(self);
    return self;
}






CSimpleSoundPlayer* CSimpleSoundPlayer::NewL(const TDesC8 &aDescriptor, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = NewLC(aDescriptor, aConsole, aLogger, aImmediate);
	CleanupStack::Pop(self);
    return self;
	}
CSimpleSoundPlayer* CSimpleSoundPlayer::NewLC(const TDesC8 &aDescriptor, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = new (ELeave) CSimpleSoundPlayer(aConsole, aLogger, aImmediate);
    CleanupStack::PushL(self);
    self->ConstructL(aDescriptor);
    return self;
	}

void CSimpleSoundPlayer::ConstructL(const TFileName &aFile)
	{


	iMdaPlayer=CMdaAudioPlayerUtility::NewL(*this);		//Instantiation by steps

	// Create a file audio player utility instance
	//iMdaPlayer_1=CMdaAudioPlayerUtility::NewFilePlayerL(aFile, *this);	//For inmediate instantiation


	iMdaPlayer->UseSharedHeap();  // Ensures that any subsequent calls to OpenXYZ() will create controllers that share a heap.

	iMdaPlayer->OpenFileL(aFile);
	iSoundOpen=ETrue;

	_LIT(KPlayerCreated, "SoundPlayerCreated");
	iConsole.Printf(100, KPlayerCreated, KPlayerCreated);
	
    iAudioOutput=NULL;
	}




CSimpleSoundPlayer* CSimpleSoundPlayer::NewLC(const TFileName &aFile, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = new (ELeave) CSimpleSoundPlayer(aConsole, aLogger, aImmediate);

    CleanupStack::PushL(self);
    self->ConstructL(aFile);
    return self;
	}

CSimpleSoundPlayer* CSimpleSoundPlayer::NewL(const TFileName &aFile, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = NewLC(aFile, aConsole, aLogger, aImmediate);
	CleanupStack::Pop(self);
    return self;
	}

void CSimpleSoundPlayer::ConstructL(const RFile &aFile)
	{
	// Create a file audio player utility instance
	//iMdaPlayer=CMdaAudioPlayerUtility::NewFilePlayerL(aFile, *this);	//For inmediate instantiation

	iMdaPlayer=CMdaAudioPlayerUtility::NewL(*this);		//Instantiation by steps
	iMdaPlayer->OpenFileL(aFile);
	iSoundOpen=ETrue;

	_LIT(KPlayerCreated, "SoundPlayerCreated");
	iConsole.Printf(100, KPlayerCreated, KPlayerCreated);
	}

CSimpleSoundPlayer* CSimpleSoundPlayer::NewLC(const RFile &aFile, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = new (ELeave) CSimpleSoundPlayer(aConsole, aLogger, aImmediate);
    CleanupStack::PushL(self);
    self->ConstructL(aFile);
    return self;
	}

CSimpleSoundPlayer* CSimpleSoundPlayer::NewL(const RFile &aFile, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate)
	{
    CSimpleSoundPlayer* self = NewLC(aFile, aConsole, aLogger, aImmediate);
	CleanupStack::Pop(self);
    return self;
	}





TInt CSimpleSoundPlayer::OpenL(const TFileName &aFileName , const TBool &aImmediate=ETrue)
	{
	iImmediate = aImmediate;
	Stop();
	iLogger.Log(_L("Stop before opening file"));

	iLogger.Log(_L("Closed, opening [%S]") , &aFileName);
	iMdaPlayer->OpenFileL(aFileName);
	iSoundOpen=ETrue;
	iLogger.Log(_L("Opened"));

	return KErrNone;
	}

TInt CSimpleSoundPlayer::OpenFileHandlerL(const TFileName &aFileName , const TBool &aImmediate=ETrue)
	{
	iImmediate = aImmediate;
	Stop();
	iLogger.Log(_L("Stop before opening file"));

	iLogger.Log(_L("Closed, opening [%S]") , &aFileName);

	RFs aFs;
	RFile aFileHandler;
	User::LeaveIfError(aFs.Connect());

	User::LeaveIfError(aFileHandler.Open( aFs, aFileName, EFileRead));
	iMdaPlayer->OpenFileL(aFileHandler);

	aFileHandler.Close();
	aFs.Close();

	iSoundOpen=ETrue;
	iLogger.Log(_L("Opened"));
	return KErrNone;
	}

TInt CSimpleSoundPlayer::OpenUrlL(const TDesC &aUrl, TInt aIapId, const TDesC8 &aMime)
	{
	iLogger.Log(_L("Opening url [%S] with IapId [%s]") , &aUrl , aIapId );
	iLogger.Log(_L8("   and MIME Type [%S]") , &aMime);
	iMdaPlayer->OpenUrlL(aUrl, aIapId, aMime);
	return ETrue;
	}




TInt CSimpleSoundPlayer::OpenDesL(const TDesC8 &aSoundDes , const TBool &aImmediate)
	{
	iImmediate = aImmediate;
	Stop();
	iLogger.Log(_L("Stop before opening descriptor"));

	iLogger.Log(_L("Closed, opening second descriptor of length (%d)") , aSoundDes.Length() );

	iMdaPlayer->OpenDesL(aSoundDes);
	iSoundOpen=ETrue;
	iLogger.Log(_L("Opened"));
	////iAutoClose = ETrue;	///K
	return KErrNone;
	}

TInt CSimpleSoundPlayer::Play()
	{
	if(iState==EReady || iState==EPaused)
		{
		iState=EPlaying;
		iLastPlayTime.HomeTime();
		iLastStopTime.HomeTime();
		iMdaPlayer->Play();
		iLogger.Log(_L("Requested Play"));
		}
	else
		{
		iLogger.Log(_L("Requested Play, Not Ready to play"));
		iImmediate = true;
		}
	return KErrNone;
	}

TInt CSimpleSoundPlayer::Stop(const TBool &aAlsoClose)
	{
	iLogger.Log(_L("Requested Stop,,,,,"));
	iMdaPlayer->Stop();

	iLogger.Log(_L("Requested Stop,,,,, successful"));
	iLastStopTime.HomeTime();
	if (aAlsoClose)
		{
		iLogger.Log(_L("iMdaPlayer->Close()" ));
		iMdaPlayer->Close();
		iLogger.Log(_L("iMdaPlayer->Close() successful" ));
		iSoundOpen=EFalse;
		iState = ENotReady;
		}
	else
		{
		if (iState==EPlaying || iState==EPaused) {iState = EReady;}
		}
	return KErrNone;
	}


TInt CSimpleSoundPlayer::Pause()
	{
	iLogger.Log(_L("Requested Pause"));
	iState=EPaused;
	// Resume is not supported in Devsound adaptation on HW ... this will crash the phone for WAV.
#ifdef __WINS__
	iMdaPlayer->WillResumePlay();
#endif
	iMdaPlayer->Pause();
	return KErrNone;
	}

TInt CSimpleSoundPlayer::PauseResume()
	{
	iLogger.Log(_L("Requested Pause"));
	iState=EPaused;
    // Resume is not supported in Devsound adaptation on HW ... this will crash the phone for WAV.
#ifdef __WINS__
	iMdaPlayer->WillResumePlay();
#endif
	iMdaPlayer->Pause();
	return KErrNone;
	}

void CSimpleSoundPlayer::MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds &/*aDuration*/)
	{
	iLogger.Log(_L("MapcInitComplete"));

	if (iImmediate)
		{
		if (aError == KErrNone)
			{
			_LIT(KGonnaPlay, "MapcInitComplete, GoingToPlay");
			_LIT(KGoing, "Going to");
			_LIT(KDone, "Done Started Playing");
			iConsole.Printf(100, KGonnaPlay, KGoing);



			if (playVolume)
			{
		//	TInt VolumeHigh;
			iLogger.Log(_L("SetVolume") );
			TInt maxVolume = iMdaPlayer->MaxVolume();
			iLogger.Log(_L("MaxVolume is %d"), maxVolume );
			iMdaPlayer->SetVolume(maxVolume);
			iLogger.Log(_L("SetVolume(iMdaPlayer->MaxVolume())  max=(%d)"),  iMdaPlayer->MaxVolume() );
			}

			if (metaInfo)
			{
				TInt NumMetaDatas;
				TInt MetaDataError = iMdaPlayer->GetNumberOfMetaDataEntries(NumMetaDatas);
				if ( MetaDataError )
				{	//Error getting meta data info
					Exit(MetaDataError);
				}
				else
				{	//No error yet
					TInt currMetaDataIndex;
					for ( currMetaDataIndex=0 ; currMetaDataIndex < NumMetaDatas ; currMetaDataIndex++)
					{
						CMMFMetaDataEntry *currMetaData = iMdaPlayer->GetMetaDataEntryL(currMetaDataIndex);
						// check for jpeg image
						if (currMetaData->Name() == _L("attachedpicture"))
							iLogger.Log(_L("MetaData[%d]: Name: [%S], Value: [image]") , currMetaDataIndex , &(currMetaData->Name()) );
						else
							iLogger.Log(_L("MetaData[%d]: Name: [%S], Value: [%S]") , currMetaDataIndex , &(currMetaData->Name()) , &(currMetaData->Value()) );
						delete currMetaData;
					}
				}
			}


			
			TInt pwErr =0;		            
			
			if (playWindow)
			{
				iLogger.Log(_L("SetPlayWindow, start=[%d]"),startPosition.Int64());
				iLogger.Log(_L("SetPlayWindow, end=[%d]"), endPosition.Int64() );
				pwErr = iMdaPlayer->SetPlayWindow(startPosition, endPosition);
				iLogger.Log(_L("SetPlayWindow err=[%d]"), pwErr);
				if (clearWindow)
				{
					iLogger.Log(_L("ClearPlayWindow"));
					iMdaPlayer->ClearPlayWindow();

				}
			}
			if (iNegativePlayBackWindow && pwErr ) 
			{
			   iLogger.Log(_L("SetPlayWindow Expected Err for negative test case"));
			   CActiveScheduler::Stop(); 
			}
			if (setBalance)
			{
				SetBalance(aBalance);
			}

			// Binh
            if (iDontPlayAudioClip)
            {
                iState = EReady;
                Exit(aError);
            }
            else
                {
			iState = EPlaying;
			iLastPlayTime.HomeTime();
			iLastStopTime.HomeTime();
                if (! playWindow)
                   {
                    iLogger.Log(_L("Position before Play (%d)"), I64INT(iPosition.Int64())/1000000);
			iMdaPlayer->SetPosition(iPosition);
                   }
			iMdaPlayer->Play();

			iLogger.Log(_L("Play() was called"));

			iConsole.Printf(100, KGonnaPlay, KDone);
                }
			}
		else
			{
			iLogger.Log(_L("InitError (%d)"), aError);
			iState = ENotReady;
			Exit(aError);
			}
		}
	else
		{
		iLogger.Log(_L("InitComplete, error(%d)") , aError);
		iState = aError ? ENotReady : EReady;
		if (aError)
			{
			Exit(aError);
			}
		//added
		else
		    {
		    CActiveScheduler::Stop();
		    }
		
		}
	}

void CSimpleSoundPlayer::MapcPlayComplete(TInt aError)
	{
	iLastStopTime.HomeTime();
	iLogger.Log(_L("MapcPlayComplete, error (%d)") , aError);

	_LIT(KGonnaPlay, "MapcPlayComplete, GoingToPlay");
	_LIT(KGoing, "Going to");

	iConsole.Printf(100, KGonnaPlay, KGoing);

	iState = aError ? ENotReady : EReady;
	iConsole.Printf(100, KGonnaPlay, KGoing);

	Exit(aError);
	if (iEventTarget && iParameters)
		{
		Stop();
		iEventTarget->ExecuteL(iParameters) ;
		}
	}


void CSimpleSoundPlayer::DoCancel()
{
    iLogger.Log(_L("DoCancel"));
}

void CSimpleSoundPlayer::MaloLoadingStarted()
{
	iLogger.Log(_L("MaloLoadingStarted"));
}

void CSimpleSoundPlayer::MaloLoadingComplete()
{
	iLogger.Log(_L("MaloLoadingComplete"));
}

void CSimpleSoundPlayer::MarncResourceAvailable(TUid aNotificationEventId, const TDesC8 &aNotificationData)
{
	iLogger.Log(_L("MarncResourceAvailable"));
}

CSimpleSoundPlayer::~CSimpleSoundPlayer()
	{
	if(iAudioOutput)
	  {
	  delete iAudioOutput;
	  iAudioOutput=NULL;
	  }
	iLogger.Log(_L("Trying to stop player") );
	iMdaPlayer->Stop();
	iLogger.Log(_L("Trying to close player") );
	iMdaPlayer->Close();
	iLogger.Log(_L("  Player stopped") );
    delete iMdaPlayer;
    iMdaPlayer = NULL;
	}

TTimeIntervalMicroSeconds CSimpleSoundPlayer::GetDuration()
	{
	TTimeIntervalMicroSeconds Duration = iMdaPlayer->Duration();
	iLogger.Log(_L("CMdaAudioPlayerUtility::Duration() returned [%d]") , Duration.Int64());
	return Duration;
	}


TTimeIntervalMicroSeconds CSimpleSoundPlayer::GetDuration_Para()
	{

	TTimeIntervalMicroSeconds Duration = iMdaPlayer->Duration();
	iLogger.Log(_L("CMdaAudioPlayerUtility::Duration() returned [%d]") , Duration.Int64());


	TTimeIntervalMicroSeconds duration;
	TMMFDurationInfo durationInfo;


	durationInfo = iMdaPlayer->Duration(duration);
	iLogger.Log(_L("CMdaAudioPlayerUtility::Duration(duration) returned [%d]") , duration.Int64());
	return duration;
	}

TTimeIntervalMicroSeconds CSimpleSoundPlayer::GetPlayDuration() { return iLastStopTime.MicroSecondsFrom(iLastPlayTime); }

TTime CSimpleSoundPlayer::GetPlayStartTime() { return iLastPlayTime; }

TTime CSimpleSoundPlayer::GetPlayStopTime() { return iLastStopTime; }

TInt CSimpleSoundPlayer::GetBalance(TInt &aBalance)
	{
	TInt error = iMdaPlayer->GetBalance(aBalance);
	iLogger.Log(_L("Retrieving Balance (%d), error(%d)") , aBalance, error);
	return error;
	}

TInt CSimpleSoundPlayer::SetBalance(TInt aBalance=KMMFBalanceCenter)
	{
	TInt error=KErrNone;
	TInt error2=KErrNone;
	TInt OldBalance=0;

	iLogger.Log(_L("Changing Balance "));

	error=iMdaPlayer->GetBalance(OldBalance);
	if (error)
		{
		iLogger.Log(_L("   Error getting old balance (%d)"), error) ;
		}
	else
		{
		iLogger.Log(_L("   Previous balance: %d"), OldBalance);
		}
	iLogger.Log(_L("   Setting balance to %d"), aBalance);
	error2 = iMdaPlayer->SetBalance(aBalance);
	if (error2)
		{
		iLogger.Log(_L("   Error setting new balance (%d)") , error);
		}
	else
		{
		error=iMdaPlayer->GetBalance(OldBalance);
		if (error)
			{
			iLogger.Log(_L("   Error getting new balance (%d)"), error) ;
			}
		else
			{
			iLogger.Log(_L("   Current balance: %d"), OldBalance);
			}
		}

	if (OldBalance!= aBalance)
	{
		Exit(-1502);
	}

	return error2;
	}

TInt CSimpleSoundPlayer::SetPriority(TInt aPriority, TMdaPriorityPreference aPref)
	{
	iLogger.Log(_L("Trying to set priority settings to (%d),(%d) ") , aPriority, aPref);
	TInt PriorityError = iMdaPlayer->SetPriority(aPriority, aPref);
	if (PriorityError)
		{
		iLogger.Log(_L("Error setting priority to (%d),(%d) ") , aPriority, aPref);
		}
	else
		{
		iLogger.Log(_L("Priority set to (%d),(%d)") , aPriority, aPref);
		}


	return PriorityError;
	}

TInt CSimpleSoundPlayer::SetThread_Priority()
	{
	TThreadPriority threadPriority =   EPriorityNormal;
	iLogger.Log(_L("Trying to set thread priority settings to (%d) ") , threadPriority);
	TInt PriorityError = iMdaPlayer->SetThreadPriority(threadPriority);
	if (PriorityError)
		{
		iLogger.Log(_L("Error setting thread priority to (%d) ") , threadPriority);
		}
	else
		{
		iLogger.Log(_L("Thread Priority set to (%d)") , threadPriority);
		}
	return PriorityError;
	}


void CSimpleSoundPlayer::GetLoad_Percentage()
	{
	TInt percentage;
	iLogger.Log(_L("Trying to GetLoad_Percentage\n ") );
	TRAPD(err,iMdaPlayer->GetAudioLoadingProgressL(percentage)); //Controller doesnt support GetAudioLoadingProgressL.
	if( err == KErrNotSupported )
	    {
	    iFinalError = 0;
	    }
	iLogger.Log(_L("GetAudioLoadingProgressL (%d)") , percentage);
	}

void CSimpleSoundPlayer::GetBit_Rate()
	{
	TUint bitRate;
	iLogger.Log(_L("Trying to GetLoad_Percentage\n ") );
	iMdaPlayer->GetBitRate(bitRate);
	iLogger.Log(_L("GetAudioLoadingProgressL (%d)") , bitRate);
	}

void CSimpleSoundPlayer::GetDRMCmd()
	{

	// Create DRM custom command
    MMMFDRMCustomCommand* drmCustomCommand;
    drmCustomCommand = iMdaPlayer->GetDRMCustomCommand();
    iLogger.Log(_L("called iMdaPlayer->GetDRMCustomCommand "));
     if (drmCustomCommand == NULL)
            {
            iLogger.Log(_L("MMMFDRMCustomCommand is NULL"));
            }
      else if(drmCustomCommand)
            { 
            TBool DisableAutoIntent=ETrue;
            TInt error=KErrNone;
            error=drmCustomCommand->DisableAutomaticIntent(DisableAutoIntent);
            iLogger.Log(_L("Error getting from drmCustomCommand[%d]") , error);
            if (error==KErrPermissionDenied)
                 {
                 iLogger.Log(_L("MMMFDRMCustomCommand is not NULL-DisableAutomaticIntent[%d]"),error);
                 }
            else
                {
                iLogger.Log(_L("MMMFDRMCustomCommand -DisableAutomaticIntent[%d]"),error);
                }
            error=drmCustomCommand->EvaluateIntent(ContentAccess::EPlay);
            if (error==KErrPermissionDenied)
               {
               iLogger.Log(_L("MMMFDRMCustomCommand is not NULL-EvaluateIntent[%d]"),error);
               }
            else
               {
               iLogger.Log(_L("MMMFDRMCustomCommand-EvaluateIntent[%d]"),error);
               }
            error=drmCustomCommand->ExecuteIntent(ContentAccess::EPlay);
            if (error==KErrPermissionDenied)
               {
               iLogger.Log(_L("MMMFDRMCustomCommand is not NULL-ExecuteIntent[%d]"),error);
               }
           else
               {
               iLogger.Log(_L("MMMFDRMCustomCommand-ExecuteIntent[%d]"),error);
               }
           error=drmCustomCommand->SetAgentProperty(ContentAccess::EAgentPropertyAgentUI,0);
           if (error==KErrPermissionDenied)
               {
               iLogger.Log(_L("MMMFDRMCustomCommand is not NULL-SetAgentProperty[%d]"),error);
               }
           else
               {
               iLogger.Log(_L("MMMFDRMCustomCommand-SetAgentProperty[%d]"),error);
               }
            }  
	 
     iLogger.Log(_L("Exits CSimpleSoundPlayer::GetDRMCmd "));

	}


void CSimpleSoundPlayer::RegNotification()
	{
	iLogger.Log(_L("Trying to GetLoad_Percentage\n ") );

	iMdaPlayer->RegisterAudioResourceNotification(*this, KUidInterfaceMMFController,KNullDesC8);
	iLogger.Log(_L("RegisterAudioResourceNotification() finished") );
	}


void CSimpleSoundPlayer::RegLoadNotification()
	{
	iLogger.Log(_L("Trying to GetLoad_Percentage\n ") );
	MMMFDRMCustomCommand* cmd = iMdaPlayer->GetDRMCustomCommand();

	iMdaPlayer->RegisterForAudioLoadingNotification(*this);
	iLogger.Log(_L("RegisterForAudioLoadingNotification() finished ")  );
	}

void CSimpleSoundPlayer::CancelNotification()
	{

	iLogger.Log(_L("Trying to GetLoad_Percentage\n ") );
	TInt CanError = iMdaPlayer->CancelRegisterAudioResourceNotification(KUidInterfaceMMFController);
	iLogger.Log(_L("CancelRegisterAudioResourceNotification finished") );
	}

void CSimpleSoundPlayer::CtrImpInfo()
	{
	iLogger.Log(_L("Trying to CtrImpInfo\n ") );
    const CMMFControllerImplementationInformation& info = iMdaPlayer->ControllerImplementationInformationL();
	iLogger.Log(_L("CtrImpInfo finished") );
	}

void CSimpleSoundPlayer::Set_Priority()
	{
	iLogger.Log(_L("Trying to Set_Prioritye\n ") );

	TInt PriError = iMdaPlayer->SetPriority(KAudioPriorityRealOnePlayer, (TMdaPriorityPreference)KAudioPrefRealOneLocalPlayback);
	iLogger.Log(_L("Set_Prioritye() finished [%d]"), PriError );
	}



void CSimpleSoundPlayer::SetPosition(const TTimeIntervalMicroSeconds &aPosition)
	{
	iMdaPlayer->SetPosition(aPosition);
	}


TInt CSimpleSoundPlayer::GetPosition(TTimeIntervalMicroSeconds &aPosition)
	{
	TTimeIntervalMicroSeconds Position;
	TInt PositionError = iMdaPlayer->GetPosition(Position);

	if (!PositionError)
		{
		iLogger.Log(_L("Current position [%d]") , Position.Int64());
		if (iState != EPlaying) {iLogger.Log(_L("  But not playing"));}
		if ( ! iSoundOpen ) {iLogger.Log(_L("  But not open"));}
		aPosition=Position;
		}
	else
		{
		Exit(PositionError);
		}
	return PositionError;
	}

void CSimpleSoundPlayer::Exit(TInt aExitCode)
	{
	iFinalError = aExitCode;
	if (iSupervisor)
		{
		iSupervisor->NotifyDestruction();
		}
	else
		{
		CActiveScheduler::Stop();
		}
	}

void CSimpleSoundPlayer::SetVolumeRamp(const TTimeIntervalMicroSeconds &aRampDuration)
	{
	iMdaPlayer->SetVolumeRamp(aRampDuration);
	}

void CSimpleSoundPlayer::SetVolume(TInt aNewVolume)
	{
	iMdaPlayer->SetVolume(aNewVolume);
	}

TInt CSimpleSoundPlayer::GetVolume(TInt &aVolume)
	{
	TInt VolumeError = iMdaPlayer->GetVolume(aVolume);
	if (VolumeError)
		{
		iLogger.Log(_L("Error (%d) getting volume") , VolumeError );
		}
	else
		{
		iLogger.Log(_L("Getting volume: (%d)") , aVolume);
		}
	return VolumeError;
	}

TInt CSimpleSoundPlayer::MaxVolume()
	{
	return iMdaPlayer->MaxVolume();
	}

void CSimpleSoundPlayer::SetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds &aTrailingSilence)
	{
	iMdaPlayer->SetRepeats(aRepeatNumberOfTimes , aTrailingSilence);
	if ((aRepeatNumberOfTimes == -2) || (aRepeatNumberOfTimes > 0))
	    iIsLoopPlayEnabled = ETrue;
	else
	    iIsLoopPlayEnabled = EFalse;
	}

TInt CSimpleSoundPlayer::SetPlayWindow(const TTimeIntervalMicroSeconds &aStart, const TTimeIntervalMicroSeconds &aEnd)
	{
	iLogger.Log(_L("iMdaPlayer->SetPlayWindow() Start[%d] End[%d]"), aStart.Int64(), aEnd.Int64());
	return iMdaPlayer->SetPlayWindow(aStart, aEnd);
	}

TInt CSimpleSoundPlayer::ClearPlayWindow()
	{
	iLogger.Log(_L("iMdaPlayer->ClearPlayWindow()"));
	return iMdaPlayer->ClearPlayWindow();
	}



void CSimpleSoundPlayer::SetSupervisor(TObjectCountDown *aCounter)
	{
	iSupervisor = aCounter;
	iSupervisor->NotifyCreation();
	}

void CSimpleSoundPlayer::SetPlayCompleteTarget(MEventTarget *aTarget, CParameters *aParameters)
	{
	iEventTarget = aTarget;
	iParameters = aParameters;
	}





TInt CSimpleSoundPlayer::GetNumberOfMetaDataEntries(TInt &aNumEntries)
	{


	TInt ErrorCode = iMdaPlayer->GetNumberOfMetaDataEntries(aNumEntries);
	if (ErrorCode)
		{
		iLogger.Log(_L("Error (%d) getting number of meta data entries") , ErrorCode);
		}
	else
		{
		iLogger.Log(_L("Clip has (%d) meta data entries") , aNumEntries);
		}
	return ErrorCode;
	}

CMMFMetaDataEntry *CSimpleSoundPlayer::GetMetaDataEntryL(TInt aMetaDataIndex)
	{
	iLogger.Log(_L("Obtaining meta data with index (%d)") , aMetaDataIndex);
	return iMdaPlayer->GetMetaDataEntryL(aMetaDataIndex);
	}

TInt CSimpleSoundPlayer::ExecuteL(CParameters *aParams)
	{
	TTimeIntervalMicroSeconds tmpPosition = TTimeIntervalMicroSeconds(0);
	CUrlParameters *p;
	switch (aParams->iAction)
		{
		case KPlayerActionPause:
			Pause();
			break;
		case KPlayerActionPauseResume:
			PauseResume();
			break;
		case KPlayerActionPlay:
			Play();
			break;
		case KPlayerActionStopPlayFile:
			OpenL( (static_cast<CFileNameParameters*>(aParams) )->GetFileName() , ETrue);
			break;
		case KPlayerActionReportPosition:
			GetPosition(tmpPosition);
			return ETrue;
		case KPlayerActionReportDuration:
			GetDuration();
			return ETrue;
		case KPlayerActionReportDuration_Para:
			GetDuration_Para();
			return ETrue;
		case KPlayerActionStopPlayDes:
			OpenDesL( (static_cast<CDes8Parameters *>(aParams) )->GetDes8() , ETrue);
			break;
		case KPlayerActionReportVolume:
			TInt tmpVolume;
			GetVolume(tmpVolume);
			return ETrue;
		case KPlayerActionStopPlayUrl:
			p = static_cast<CUrlParameters *>(aParams);
			OpenUrlL(p->GetUrl() , p->GetIapId() , p->GetMimeType() );
			break;

		case KPlayerActionStopPlayFileHandler:
			OpenFileHandlerL((static_cast<CFileNameParameters*>(aParams) )->GetFileName() , ETrue);
		//	OpenFileHandlerL( (static_cast<CFileHandlerParameters*>(aParams) )->GetFileHandler() , ETrue);
			break;
		}
    if (iIsLoopPlayEnabled)
         return ETrue;
    else
	return EFalse;
	}


TInt CSimpleSoundPlayer::OpenFile(const TFileName &aFile)
{
	iLogger.Log(_L("CSimpleSoundPlayer::OpenFile"));

    TRAPD(err, iMdaPlayer->SetPriority(KAudioPriorityRealOnePlayer,
    (TMdaPriorityPreference)KAudioPrefRealOneLocalPlayback));
        {
        if (err != KErrNone)
            {
            iFinalError = err;
            return err;
            }
        }


	iMdaPlayer->OpenFileL(aFile);

	return KErrNone;
}


TInt CSimpleSoundPlayer::OpenWithFileHandler(const RFile &aFile)
{
	iLogger.Log(_L("CSimpleSoundPlayer::OpenWithFileHandler"));

   /* TRAPD(err, iMdaPlayer->SetPriority(KAudioPriorityRealOnePlayer,
    (TMdaPriorityPreference)KAudioPrefRealOneLocalPlayback));
        {
        if (err != KErrNone)
            {
            iFinalError = err;
            return err;
            }
        }*/


	iMdaPlayer->OpenFileL(aFile);

	return KErrNone;
}


TInt CSimpleSoundPlayer::OpenWithFileSource(const TMMSource &aSource)
{
	iLogger.Log(_L("CSimpleSoundPlayer::OpenWithFileSource"));

    TRAPD(err, iMdaPlayer->SetPriority(KAudioPriorityRealOnePlayer,
    (TMdaPriorityPreference)KAudioPrefRealOneLocalPlayback));
        {
        if (err != KErrNone)
            {
            iFinalError = err;
            return err;
            }
        }


	iMdaPlayer->OpenFileL(aSource);

	return KErrNone;
}

TInt CSimpleSoundPlayer::OpenWithDesc(const TDesC8 &aDescriptor)
{
	iLogger.Log(_L("CSimpleSoundPlayer::OpenWithDesc"));

    TRAPD(err, iMdaPlayer->SetPriority(KAudioPriorityRealOnePlayer,
    (TMdaPriorityPreference)KAudioPrefRealOneLocalPlayback));
        {
        if (err != KErrNone)
            {
            iFinalError = err;
            return err;
            }
        }


	iMdaPlayer->OpenDesL(aDescriptor);

	return KErrNone;
}

void CSimpleSoundPlayer::AudioOutputCreationL()
    {
    CAudioOutput::TAudioOutputPreference output(CAudioOutput::EAll);
    TBool secure=ETrue;
    TInt err=KErrNone;
    
    iLogger.Log(_L("calling CAudioOutput"));
    TRAP(err,iAudioOutput=CAudioOutput::NewL(*iMdaPlayer));
    if(err!=KErrNone)
       {
       iLogger.Log(_L("AudioOutput Creation Error - err=%d"),err);
       User::Leave(err);        
       }
    else
       {
       iLogger.Log(_L("AudioOutput Created successfully - err=%d"),err);
       }
    TRAP(err,iAudioOutput->SetAudioOutputL(output)); 
    if (err == KErrNone)
       {
       iLogger.Log(_L("CAudioOutput/SetAudioOutputL - err=%d"),err);
       
       }
    else
       iLogger.Log(_L("CAudioOutput/SetAudioOutputL else - err=%d"),err);
    iLogger.Log(_L("calling setsecureoutputl"));
    TRAP(err,iAudioOutput->SetSecureOutputL(secure));
    if(err!=KErrNone)
       {
       iLogger.Log(_L("CAudioOutput/SetSecureOutputL - err if=%d"),err);
       
       }
    else
       {
       iLogger.Log(_L("CAudioOutput/SetSecureOutputL - err else=%d"),err);
       }
    }

void CSimpleSoundPlayer::LoopPlayL()
    {
    iLogger.Log(_L("CMyPlayer::SetRepeatsForever"));
    //iMdaPlayer->SetRepeats(KMdaRepeatForever, TTimeIntervalMicroSeconds(3000000));
    iMdaPlayer->SetRepeats(KRepeatTimes, TTimeIntervalMicroSeconds(3000000));
    }



/*void CSimpleSoundPlayer::SetRunCase(TInt aRunCase)
{
    iRunCase = aRunCase;
}

void CSimpleSoundPlayer::SetSubCase(TInt aSubCase)
{
    iSubCase = aSubCase;
}*/
