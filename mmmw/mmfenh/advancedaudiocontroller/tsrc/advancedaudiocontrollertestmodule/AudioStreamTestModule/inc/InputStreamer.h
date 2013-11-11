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
* Description:  Audio stream test component
*
*/


#ifndef INPUT_STREAMER_H
#define INPUT_STREAMER_H

//#include <StifTestInterface.h>
#include <MdaAudioInputStream.h>
#include <StifLogger.h>
#include "mda/common/audio.h"
#include "EventTarget.h"
#include "StreamerBase.h"

class CInputStreamer : public CStreamerBase , public MMdaAudioInputStreamCallback
	{
	private:
		CInputStreamer(CStifLogger *aLogger);
		void ConstructL();
		void ConstructL(TInt aPriority, TMdaPriorityPreference aPref);

	public:
		// Standard two-phase construction
		static CInputStreamer* NewL(CStifLogger *aLogger, TInt aId=0);
		static CInputStreamer* NewLC(CStifLogger *aLogger, TInt aId=0);

		static CInputStreamer* NewL(TInt aPriority, TMdaPriorityPreference aPref,
		                            CStifLogger *aLogger, TInt aId=0);
		static CInputStreamer* NewLC(TInt aPriority, TMdaPriorityPreference aPref,
		                             CStifLogger *aLogger, TInt aId=0);

		~CInputStreamer();

        void SetAudioPropertiesL(TInt aSampleRate, TInt aChannels);
		void SetPriority(TInt aPriority, TMdaPriorityPreference aPref);
		TInt GetBalanceL();
		void SetBalanceL(TInt aBalance);
		void SetGain(const TInt aNewGain);
		TInt GetMaxGain();
		TInt GetGain();
		void SetDataTypeL(TFourCC aDataType);
		TFourCC GetDataType();
		void GetSupportedBitRatesL(RArray<TInt>& aSupportedBitRates);
		TInt BitRateL();
		void SetBitRateL(TInt aBitRate);

		void Stop();

		RPointerArray<TDes8> &GetBuffers();
		void StartRecording();

		TInt ExecuteL(CParameters *aParams);
		void Exit(TInt aExitCode);
		void SetSettings(TMdaPackage *aSettings);
		//void SetExitAfterOpen(TBool aFlag);

		// iherited from MMdaAudioInputStreamCallback
		void MaiscOpenComplete(TInt aError);
		void MaiscBufferCopied(TInt aError, const TDesC8& aBuffer);
		void MaiscRecordComplete(TInt aError);

	public:
		static const TInt KRecorderActionStop;
		static const TInt KSchedulerStop;
	private:
		CMdaAudioInputStream *iMdaAudioInputStream;
		TBool iAlreadyClosed;
		TBool iRecording;
		//TBool iExitAfterOpen;
		enum TState
			{
			EClosed,
			EFirstOpenSuccessful,
			EOpen,
			EPlaying,
			ERecording,
			EStopped
			};
		TState iState;
		TMdaPackage *iSettings;
	};

#endif	//INPUT_STREAMER_H
