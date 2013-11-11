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

#if !defined(AFX_PLAYRAMP_H__0DB26F38_987F_4224_9F9D_6389F4FC61F4__INCLUDED_)
#define AFX_PLAYRAMP_H__0DB26F38_987F_4224_9F9D_6389F4FC61F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MdaAudioSampleEditor.h>
#include "TestCaseBase.h"
#include "MediaEvent.h"

class CPlayRamp  : public CTestCaseBase, public MMdaObjectStateChangeObserver/*, public MEventTarget*/
{
public:
	CPlayRamp(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CPlayRamp();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	virtual void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);

//	TInt ExecuteL(CParameters* aParams);

	static void GetFileName(TPtrC path, TFileName* fileName);


private:

	static CPlayRamp* NewL(CTestModuleIf *aConsole, CStifLogger *aLogger,TFileName* fileName);

	CMdaAudioRecorderUtility* recorder;

	TInt expectedVol;
	TTimeIntervalMicroSeconds rampTime;

	TInt previousVolume;

	void ConstructL(TFileName* fileName);

};

#endif // !defined(AFX_PLAYRAMP_H__0DB26F38_987F_4224_9F9D_6389F4FC61F4__INCLUDED_)
