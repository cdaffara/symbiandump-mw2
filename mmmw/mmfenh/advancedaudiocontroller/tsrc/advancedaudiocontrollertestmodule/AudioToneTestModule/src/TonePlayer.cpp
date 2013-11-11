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
* Description:  MmfTsTone class member functions
*
*/


#include "TonePlayer.h"
#include "TemplateParameters.h"
#include "DTMFParameters.h"



const TInt CTonePlayer::KPlayerActionPlay = 1;
const TInt CTonePlayer::KPlayerActionReportVolume = 2;
const TInt CTonePlayer::KPlayerActionStop = 3;
const TInt CTonePlayer::KPlayerActionCancelPrepare = 4;

const TInt CTonePlayer::KPlayerActionPrepareFileSequence = 5;
const TInt CTonePlayer::KPlayerActionPrepareFixedSequence = 6;
const TInt CTonePlayer::KPlayerActionPrepareTone = 7;
const TInt CTonePlayer::KPlayerActionPrepareDualTone = 8;
const TInt CTonePlayer::KPlayerActionPrepareDTMFSequence = 9;
const TInt CTonePlayer::KPlayerActionPrepareDesSequence = 10;
const TInt CTonePlayer::KPlayerActionChangeVolumeToMax = 11;
const TInt CTonePlayer::KPlayerActionChangeVolumeToAvg = 12;
const TInt CTonePlayer::KPlayerActionChangeVolumeToZero = 13;
// Frequency and duration of the tone to be played
/*
CTonePlayer::CTonePlayer(CLogger *aLogger)
	:	iSupervisor(0), iId(0), iFinalError(KErrNone), iLogger(aLogger), 
		iLastPrepareError(KErrNone), iPrepareCalledCounter(0),
		iExitEventTarget(0), iExitEventParameters(0)
		//, iLastPlayTime(0), iLastStopTime(0)
	{
	iFreeToPlay=EFalse;
	}
*/

/*
CTonePlayer* CTonePlayer::NewL(CLogger *aLogger, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately)
	{
	CTonePlayer* self = NewLC(aLogger, aFrequency, aDuration, aPlayInmediately);
	CleanupStack::Pop(self);
	return self;
	}
*/

/*
CTonePlayer* CTonePlayer::NewLC(CLogger *aLogger, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately)
	{
	CTonePlayer* self = new(ELeave)CTonePlayer(aLogger);
	CleanupStack::PushL(self);
	self->ConstructL(aFrequency, aDuration, aPlayInmediately);
	return self;
	}
*/

void CTonePlayer::ConstructL(TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately)
	{
	iFrequency=aFrequency;
	iDuration=aDuration;
	iPlayInmediately=aPlayInmediately;
	iMdaAudioToneUtility = CMdaAudioToneUtility::NewL(*this);
     
       
        
        

	// Configure the audio tone utility to play a single tone
	if (iMdaAudioToneUtility)
		{
		iMdaAudioToneUtility->PrepareToPlayTone(iFrequency, TTimeIntervalMicroSeconds(iDuration));
        
       // const TUid KUidBassBoostEffect = {0x10203827};  
       // iMdaAudioToneUtility->CustomInterface(KUidBassBoostEffect);  
		}
	else
		{	//console->Printf(KNotPlayed);
		}
	///console->Printf(KPlayed);
	}

void CTonePlayer::ConstructL()
	{
	iFrequency=0;
	iDuration=0;
	iMdaAudioToneUtility = CMdaAudioToneUtility::NewL(*this);
	}

/*
CTonePlayer* CTonePlayer::NewL(CLogger *aLogger)
	{
	CTonePlayer *_self = NewLC(aLogger);
	CleanupStack::Pop(_self);
	return _self;
	}
*/
/*
CTonePlayer* CTonePlayer::NewLC(CLogger *aLogger)
	{
	CTonePlayer *_self = new(ELeave)CTonePlayer(aLogger);
	CleanupStack::PushL(_self);
	_self->ConstructL();
	return _self;
	}
*/

// ///////////////////////////////////////////////////
CTonePlayer::CTonePlayer(COutputPrinter *aOutputPrinter)
	:	iSupervisor(0), iId(0), iFinalError(KErrNone), 
		iLastPrepareError(KErrNone), iPrepareCalledCounter(0),
		iExitEventTarget(0), iExitEventParameters(0), iOutputPrinter(aOutputPrinter)
		//, iLastPlayTime(0), iLastStopTime(0)
	{
	iFreeToPlay=EFalse;
	}

CTonePlayer* CTonePlayer::NewL(COutputPrinter *aOutputPrinter, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately)
	{
	CTonePlayer* self = NewLC(aOutputPrinter, aFrequency, aDuration, aPlayInmediately);
	CleanupStack::Pop(self);
	return self;
	}

CTonePlayer* CTonePlayer::NewLC(COutputPrinter *aOutputPrinter, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately)
	{
	CTonePlayer* self = new(ELeave)CTonePlayer(aOutputPrinter);
	CleanupStack::PushL(self);
	self->ConstructL(aFrequency, aDuration, aPlayInmediately);
	return self;
	}

CTonePlayer* CTonePlayer::NewL(COutputPrinter *aOutputPrinter)
	{
	CTonePlayer *_self = NewLC(aOutputPrinter);
	CleanupStack::Pop(_self);
	return _self;
	}
CTonePlayer* CTonePlayer::NewLC(COutputPrinter *aOutputPrinter)
	{
	CTonePlayer *_self = new(ELeave)CTonePlayer(aOutputPrinter);
	CleanupStack::PushL(_self);
	_self->ConstructL();
	return _self;
	}

// ///////////////////////////////////////////////////
CTonePlayer::~CTonePlayer()
	{
	delete iMdaAudioToneUtility;    
	iMdaAudioToneUtility = NULL;
	}

void CTonePlayer::CancelPrepare()
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::CancelPrepare()"), iId);
	iMdaAudioToneUtility->CancelPrepare();
	}

TBool CTonePlayer::Play()
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::Play()"), iId);
	if(iFreeToPlay)
		{
		iFreeToPlay = EFalse;
		iOutputPrinter->Printf(_L("%d) CTonePlayer::Play(), Starting playback"), iId);
		iLastPlayTime.HomeTime();
		iLastStopTime.HomeTime();
		iMdaAudioToneUtility->Play();
		return ETrue;
		} 
	else
		{
		return EFalse;	
		}
	}

void CTonePlayer::Stop()
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::Stop()"), iId);
	iMdaAudioToneUtility->CancelPlay();
	iLastStopTime.HomeTime();
	Exit(KErrNone);
	}

void CTonePlayer::SetVolumeRamp(TTimeIntervalMicroSeconds aRamp) { iMdaAudioToneUtility->SetVolumeRamp(aRamp); }

void CTonePlayer::SetVolume(TInt aVolume) { iOutputPrinter->Printf(_L("%d) CTonePlayer::SetVolume()"), iId);
iMdaAudioToneUtility->SetVolume(aVolume); }

TInt CTonePlayer::GetVolume() { return iMdaAudioToneUtility->Volume(); }

TInt CTonePlayer::GetMaxVolume() { return iMdaAudioToneUtility->MaxVolume(); }

void CTonePlayer::SetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds &aTrailingSilence)
	{
	iMdaAudioToneUtility->SetRepeats(aRepeatNumberOfTimes,aTrailingSilence);
	}

TInt CTonePlayer::GetFixedSequenceCount()
	{
	TInt Sequences = iMdaAudioToneUtility->FixedSequenceCount();
	iOutputPrinter->Printf(_L("%d) CTonePlayer::GetFixedSequenceCount(), Number of fixed sequences: (%d)") , iId, Sequences );
	return Sequences;
	}
	
	

const TDesC &CTonePlayer::GetFixedSequenceName(TInt aSequenceNumber)
	{
	const TDesC &Name = iMdaAudioToneUtility->FixedSequenceName(aSequenceNumber);
	iOutputPrinter->Printf(_L("%d) CTonePlayer::GetFixedSequenceName(%d), result: is %S") , iId, aSequenceNumber , &Name );
	return Name;
	}

void CTonePlayer::PrepareTone(TInt aFreq, const TTimeIntervalMicroSeconds &aToneDuration, TBool aPlayInmediately)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareTone(%d, %d, %d)") , iId, aFreq, aToneDuration, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	iMdaAudioToneUtility->PrepareToPlayTone(aFreq, aToneDuration);
	}

void CTonePlayer::PrepareFixedSequence(TInt aSequenceNumber, TBool aPlayInmediately)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareFixedSequence(%d, %d)") , iId, aSequenceNumber, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	iMdaAudioToneUtility->PrepareToPlayFixedSequence(aSequenceNumber);
	}
	
void CTonePlayer::PrepareFixedSequenceCustomInterface(TInt aSequenceNumber, TBool aPlayInmediately)  
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareFixedSequenceCustomInterface(%d, %d)") , iId, aSequenceNumber, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	
	
    const TUid KUidBassBoostEffect = {0x10203827};  
    iMdaAudioToneUtility->CustomInterface(KUidBassBoostEffect);  
	
	iMdaAudioToneUtility->PrepareToPlayFixedSequence(aSequenceNumber);
	}	

void CTonePlayer::PrepareFileSequence(const TDesC &aFileName , TBool aPlayInmediately)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareFileSequence(%S, %d)") , iId, &aFileName, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	iMdaAudioToneUtility->PrepareToPlayFileSequence(aFileName);
	//iMdaAudioToneUtility->PrepareToPlayDTMFString(aFileName);
	}


void CTonePlayer::PrepareRFileSequence(const TDesC &aFileName, TBool aPlayInmediately)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareRFileSequence(%S, %d)") , iId, &aFileName, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	
	RFs	  RFsPtr;
	RFile rfile;

	if( RFsPtr.Connect() == KErrNone )
		{
		if( rfile.Open( RFsPtr, aFileName, EFileRead ) == KErrNone )
			{
			
			iMdaAudioToneUtility->PrepareToPlayFileSequence(rfile);
			}
		}
	
	rfile.Close();
	RFsPtr.Close();
	
	//iMdaAudioToneUtility->PrepareToPlayDTMFString(aFileName);
	}


void CTonePlayer::PrepareDTMFSequence(const TDesC &aSequence, TBool aPlayInmediately)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareDTMFSequence(%S, %d)") , iId, &aSequence, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	iMdaAudioToneUtility->PrepareToPlayDTMFString(aSequence);
	}

void CTonePlayer::PrepareDesSequence(const TDesC8 &aSequence, TBool aPlayInmediately)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareDesSequence(sequence, %d)") , iId, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	iMdaAudioToneUtility->PrepareToPlayDesSequence(aSequence);
	}

void CTonePlayer::PrepareDualTone(TInt aFreq1, TInt aFreq2, const TTimeIntervalMicroSeconds &aToneDuration, TBool aPlayInmediately)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::PrepareDualTone(%d, %d, %d, %d)") , iId, aFreq1, aFreq2, aToneDuration, aPlayInmediately);
	iPlayInmediately=aPlayInmediately;
	iMdaAudioToneUtility->PrepareToPlayDualTone(aFreq1, aFreq2, aToneDuration);
	}

void CTonePlayer::SetPriority(TInt aPriority, TMdaPriorityPreference aPref)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::SetPriority(%d, %d)") , iId, aPriority, aPref);
	iMdaAudioToneUtility->SetPriority(aPriority, aPref);
	}

void CTonePlayer::SetId(TInt aId) { iId = aId; }

TInt CTonePlayer::GetId() { return iId; }

TInt CTonePlayer::GetFinalError() { return iFinalError; }

TInt CTonePlayer::GetLastPrepareCompleteError() {return iLastPrepareError; }

TInt CTonePlayer::GetPrepareCompleteCallbackCount() {return iPrepareCalledCounter;}

void CTonePlayer::SetSupervisor(TObjectCountDown *aCounter)
	{
	iSupervisor = aCounter;
	iSupervisor->NotifyCreation();
	}

void CTonePlayer::SetExitCompleteTarget(MEventTarget *aTarget, CParameters *aParameters)
	{
	iExitEventTarget = aTarget;
	iExitEventParameters = aParameters;
	}

TTimeIntervalMicroSeconds CTonePlayer::GetPlayDuration() { return iLastStopTime.MicroSecondsFrom(iLastPlayTime); }

TTime CTonePlayer::GetPlayStartTime() { return iLastPlayTime; }

TTime CTonePlayer::GetPlayStopTime() { return iLastStopTime; }

TInt CTonePlayer::GetBalanceL()
	{
	TInt retVal = iMdaAudioToneUtility->GetBalanceL();
	return retVal;
	}

void CTonePlayer::SetBalanceL(TInt aBalance)
	{
	iMdaAudioToneUtility->SetBalanceL(aBalance);
	}

void CTonePlayer::SetDTMFLengths(TTimeIntervalMicroSeconds32 iToneOnLength, TTimeIntervalMicroSeconds32 iToneOffLength, TTimeIntervalMicroSeconds32 iPauseLength)
	{
	iMdaAudioToneUtility->SetDTMFLengths(iToneOnLength, iToneOffLength, iPauseLength);
	}

void CTonePlayer::Exit(TInt aExitCode)
	{
	iOutputPrinter->Printf(_L("%d) CTonePlayer::Exit(%d)"), iId, aExitCode);
	iFinalError = aExitCode;
	if (iSupervisor)
		{
		iSupervisor->NotifyDestruction();
		iSupervisor=0;
		}
	else
		{
		CActiveScheduler::Stop();
		}
	if (iExitEventTarget && iExitEventParameters)
		{
		iExitEventTarget->ExecuteL(iExitEventParameters) ;
		}
	}


TInt CTonePlayer::State()
    {
    return iMdaAudioToneUtility->State();
    }
	
// from MMdaAudioToneObserver
void CTonePlayer::MatoPrepareComplete(TInt aError)
	{
	iLastPrepareError=aError;
	iPrepareCalledCounter++;
	iOutputPrinter->Printf(_L("%d) CTonePlayer::MatoPrepareComplete, error (%d)"), iId, aError);
	if (aError)
		{
		Exit(aError);
		}
	else
		{
		iFreeToPlay = ETrue;
		if (iPlayInmediately) 
			{
			Play();
			}
		else
		    {
		   CActiveScheduler::Stop();
		    }
		}
	iFinalError=aError;
	}

// from MMdaAudioToneObserver
void CTonePlayer::MatoPlayComplete(TInt aError)
	{
	iLastStopTime.HomeTime();
	iOutputPrinter->Printf(_L("%d) CTonePlayer::MatoPlayComplete, error (%d)"), iId, aError);
	if (aError == KErrNotSupported) 
	{
		iFinalError = KErrNone;
		aError = KErrNone;
	}
	else
	{
		iFinalError=aError;
	}
	iFreeToPlay = ETrue;
	Exit(aError);
	}

// from MEventTarget
TInt CTonePlayer::ExecuteL(CParameters *aParams)
	{
	switch (aParams->iAction)
		{
		case KPlayerActionPlay:
			Play();
			break;
		case KPlayerActionReportVolume:
			iOutputPrinter->Printf(_L("%d) Retrieving volume: (%d)") , iId, GetVolume());
			return ETrue;
			break;
		case KPlayerActionStop:
			Stop();
			break;
		case KPlayerActionCancelPrepare:
			CancelPrepare();
			break;
		case KPlayerActionPrepareFileSequence:
			{
			CTemplateParameters<TFileName> *fn=static_cast< CTemplateParameters<TFileName>* >(aParams);
			PrepareFileSequence(fn->GetData());
			//(*fn).GetData();
			break;
			}
						
		case KPlayerActionPrepareFixedSequence:
			{
			CTemplateParameters<TInt> *fs=static_cast< CTemplateParameters<TInt>* >(aParams);
			PrepareFixedSequence(fs->GetData());
			break;
			}
		case KPlayerActionPrepareTone:
			{
			CTemplateParameters<TFixedArray<TInt64, 2> > *ToneData = static_cast< CTemplateParameters<TFixedArray<TInt64, 2> >* >(aParams);
			TFixedArray<TInt64, 2> params = ToneData->GetData();
			//PrepareTone( params[0].GetTInt() , params[1]);
			PrepareTone( I64INT(params[0]), params[1]);
			break;
			}
		case KPlayerActionPrepareDualTone:
			{
			CTemplateParameters<TFixedArray<TInt64, 4> > *ToneData = static_cast< CTemplateParameters<TFixedArray<TInt64, 4> >* >(aParams);
			TFixedArray<TInt64, 4> params = ToneData->GetData();
		
			PrepareDualTone( I64INT(params[0]), I64INT(params[1]), params[2],params[3]);
			break;
			}
		case KPlayerActionPrepareDTMFSequence:
			{
			//CTemplateParameters<TBufC<50> > *ToneData = static_cast< CTemplateParameters<TBufC<50> >* >(aParams);
			CDTMFParameters *params = static_cast<CDTMFParameters*>(aParams);

			PrepareDTMFSequence( *(params->GetDTMFString()) );
			SetDTMFLengths(params->GetToneDuration(), params->GetToneOffDuration(), params->GetPauseDuration() );
			break;
			}
        case KPlayerActionPrepareDesSequence:
            CTemplateParameters<HBufC8*> *params =
                    static_cast<CTemplateParameters<HBufC8*> *> (aParams);
            PrepareDesSequence(*(params->GetData()));
            break;
        case KPlayerActionChangeVolumeToMax:
            {
            SetVolume( GetMaxVolume());
            break;
            }
        case KPlayerActionChangeVolumeToAvg:
            {
            SetVolume(GetMaxVolume() / 2);
            break;
            }
        case KPlayerActionChangeVolumeToZero:
            {
            SetVolume(0);
            break;
            }
        }
	return EFalse;
	}
