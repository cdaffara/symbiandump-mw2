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
* Description: Audio stream test component
*
*/


#include "StreamerBase.h"

const TInt CStreamerBase::KBitRate8=8;
const TInt CStreamerBase::KSampleRate8000Hz  =  8000;
const TInt CStreamerBase::KSampleRate11025Hz = 11025;
const TInt CStreamerBase::KSampleRate12000Hz = 12000;
const TInt CStreamerBase::KSampleRate16000Hz = 16000;
const TInt CStreamerBase::KSampleRate22050Hz = 22050;
const TInt CStreamerBase::KSampleRate24000Hz = 24000;
const TInt CStreamerBase::KSampleRate32000Hz = 32000;
const TInt CStreamerBase::KSampleRate44100Hz = 44100;
const TInt CStreamerBase::KSampleRate48000Hz = 48000;
const TInt CStreamerBase::KSampleRate64000Hz = 64000;
const TInt CStreamerBase::KSampleRate96000Hz = 96000;

const TInt CStreamerBase::KChannelsMono = 1;
const TInt CStreamerBase::KChannelsStereo = 2;

CStreamerBase::CStreamerBase(CStifLogger *aLogger)
	:	iFinalError(KErrNone), iId(0), iLogger(aLogger) , iCurrentBuffer(-1) //, iBuffers(16)
	{
	iExitAfterOpen = EFalse;
	}

CStreamerBase::~CStreamerBase()
	{
	iBuffers.ResetAndDestroy();
	}

void CStreamerBase::SetId(TInt aId) { iId = aId; }

TInt CStreamerBase::GetId() { return iId; }

TInt CStreamerBase::GetFinalError() { return iFinalError; }

TTimeIntervalMicroSeconds CStreamerBase::GetExecDuration() { return iLastStopTime.MicroSecondsFrom(iLastStartTime); }

TTime CStreamerBase::GetStartTime() { return iLastStartTime; }

TTime CStreamerBase::GetStopTime() { return iLastStopTime; }

TInt CStreamerBase::AppendBuffer(TDes8 *aBuffer)
	{
	return iBuffers.Append(aBuffer);
	}

TMdaAudioDataSettings::TAudioCaps CStreamerBase::TranslateRate(TInt aSampleRate, TMdaAudioDataSettings::TAudioCaps aDefaultSampleRate)
	{
	TMdaAudioDataSettings::TAudioCaps retVal=aDefaultSampleRate;

    switch(aSampleRate)
    {
        case KSampleRate8000Hz:
    		iLogger->Log(_L("Setting sample rate to 8000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate8000Hz;
            break;
        case KSampleRate11025Hz:
    		iLogger->Log(_L("Setting sample rate to 11025"));
	    	retVal=TMdaAudioDataSettings::ESampleRate11025Hz;
            break;
        case KSampleRate12000Hz:
    		iLogger->Log(_L("Setting sample rate to 12000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate12000Hz;
            break;
        case KSampleRate16000Hz:
    		iLogger->Log(_L("Setting sample rate to 16000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate16000Hz;
            break;
        case KSampleRate22050Hz:
    		iLogger->Log(_L("Setting sample rate to 22050"));
	    	retVal=TMdaAudioDataSettings::ESampleRate22050Hz;
            break;
        case KSampleRate24000Hz:
    		iLogger->Log(_L("Setting sample rate to 24000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate24000Hz;
            break;
        case KSampleRate32000Hz:
    		iLogger->Log(_L("Setting sample rate to 32000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate32000Hz;
            break;
        case KSampleRate44100Hz:
    		iLogger->Log(_L("Setting sample rate to 44100"));
	    	retVal=TMdaAudioDataSettings::ESampleRate44100Hz;
            break;
        case KSampleRate48000Hz:
    		iLogger->Log(_L("Setting sample rate to 48000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate48000Hz;
            break;
        case KSampleRate64000Hz:
    		iLogger->Log(_L("Setting sample rate to 64000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate64000Hz;
            break;
        case KSampleRate96000Hz:
    		iLogger->Log(_L("Setting sample rate to 96000"));
	    	retVal=TMdaAudioDataSettings::ESampleRate96000Hz;
            break;
        default:
            break;
    		iLogger->Log(_L("Setting sample rate to default"));
    }
	return retVal;
	}

void CStreamerBase::Exit(TInt aExitCode)
	{
	iLogger->Log(_L("%d) CStreamerBase::Exit(%d)"), iId, aExitCode);
	iFinalError = aExitCode;
	CActiveScheduler::Stop();
	}

void CStreamerBase::SetExitAfterOpen(TBool aFlag)
{
    iExitAfterOpen = aFlag;
}

