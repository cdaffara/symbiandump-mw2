/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* @file 
* This is the header file for CT_MsgRecordCurrentTime
* 
*
*/





#ifndef __T_RECORDCURRENTTIME_H__
#define __T_RECORDCURRENTTIME_H__


/* User includes */
#include <test/testexecutestepbase.h>


/* Literals Used */
_LIT(KRecordCurrentTime,"RecordCurrentTime");


/* Implements the test step to record the execution time of this test step */
class CT_MsgRecordCurrentTime : public CTestStep
	{
public:
	CT_MsgRecordCurrentTime();

	/* CTestStep implementation */
	virtual TVerdict	doTestStepL();	
	};
#endif /* __T_RECORDCURRENTTIME_H__ s*/
