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
* Description:  AudioRecorder test
*
*/


#if !defined(AFX_RECORDINGBASE_H__AAF8C761_831F_409A_9DC1_D2D05E1180DA__INCLUDED_)
#define AFX_RECORDINGBASE_H__AAF8C761_831F_409A_9DC1_D2D05E1180DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MdaAudioSampleEditor.h>
#include "TestCaseBase.h"
#include "MediaEvent.h"
#include <f32file.h>
class CRecordingBase;

typedef CRecordingBase* recPtr;
typedef  recPtr (*CreateObjectFunction)(CTestModuleIf*, CStifLogger*);

class CRecordingBase : public CTestCaseBase, public MMdaObjectStateChangeObserver, public MEventTarget
{
public:
	CRecordingBase(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordingBase();

	//To recieve AudioRecorderUtility notifications
	virtual void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);

	//To process after timer is completed
	virtual TInt ExecuteL(CParameters *aParams);

	static CRecordingBase* NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName, CreateObjectFunction coFunc = NULL);

	TInt StartRecordingL();

	CMdaAudioRecorderUtility* recorder;

	TTimeIntervalMicroSeconds timeToRecord;

//	TUint maxFileSize;
	TInt maxFileSize;
	TInt relativeGain;
	TInt callbackErr;
	TInt expectedBalance;
	TInt expectedBitRate;
	TInt expectedChannel;
	TInt expectedSampleRate;
	TInt fromBeging;
	TInt relativePos;
	TInt64 existingDuration;

	TTimeIntervalMicroSeconds timeAvailable;
	TTimeIntervalMicroSeconds fileDuration;
	TTimeIntervalMicroSeconds newFileDuration;
	TTimeIntervalMicroSeconds position;

	TFileName recordingFileName;
	bool CropTest;
	bool isMp4;
	bool isWav;
	bool isAmr;
	bool isBitRateTest;

	RFs iFs;

	        /**
	        * Pointer to a file manager object. Owned.
	        */
  CFileMan* iFileMan;

	//To create an object of the derived class
	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	//To forward the run test case
	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter, CreateObjectFunction coFunc = NULL);

	//Pre process before start recording
	virtual TInt PreProcess(CStifSectionParser *aParser);

	//Post process after recording is done
	virtual TInt PostProcess(CStifSectionParser *aParser);

	static TInt SimpleRecordL(CTestModuleIf *aConsole, CStifLogger *aLogger, CRecordingBase* recObj, TBool doProcess = false, CStifSectionParser* aParser = NULL, TBool doTimer = true);

	static TInt CreateRecorderL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter, CreateObjectFunction coFunc, CRecordingBase*& recObj, TBool nameReq = true, TBool sizeReq = true);

	void ConstructL(TFileName * fileName);

	static void GetFileName(TPtrC path, TFileName* fileName, TInt clipCounter);

	TTimeIntervalMicroSeconds Duration();
};

#endif // !defined(AFX_RECORDINGBASE_H__AAF8C761_831F_409A_9DC1_D2D05E1180DA__INCLUDED_)
