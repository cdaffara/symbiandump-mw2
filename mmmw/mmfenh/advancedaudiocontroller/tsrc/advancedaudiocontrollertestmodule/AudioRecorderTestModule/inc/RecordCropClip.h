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


#if !defined(AFX_RecordCropClip_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)
#define AFX_RecordCropClip_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <MdaAudioSampleEditor.h>
//#include "TestCaseBase.h"

#include "RecordingBase.h"

//class CRecordCropClip : public CTestCaseBase, public MMdaObjectStateChangeObserver
class CRecordCropClip : public CRecordingBase
{
public:
	CRecordCropClip(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordCropClip();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

//	static CRecordCropClip* NewL(CTestModuleIf *aConsole, CStifLogger *aLogger);

//	virtual void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

protected:
	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

//	TTimeIntervalMicroSeconds errorRange;
	TTimeIntervalMicroSeconds errRange;

//	TTimeIntervalMicroSeconds timeAvailable;

//	void ConstructL();

};

#endif // !defined(AFX_RECPOSITIONCLIP_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)






/*	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

protected:

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TTimeIntervalMicroSeconds errorRange;

	TTimeIntervalMicroSeconds timeAvailable; */