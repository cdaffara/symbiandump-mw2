// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// @file 
// [TestStep Name]
// RecordCurrentTime
// [Paramaters]
// EventCode		:	Event code defined in TInstrumentationPoint.
// Creates a logging statement in EventLog server with the time, and
// event code information.
// [APIs Used]
// 
//


/* User includes */
#include "T_RecordCurrentTime.h"
#include "t_utilsenumconverter.h"
#include "t_testinstrumentation.h"

/* Epoc Include */
#include <systemmonitor/instrumentationhandler.h>


/* Literals Used*/
_LIT(KEventCode, "EventCode");

/**
  Function : CT_MsgRecordCurrentTime
  Description : Constructor
  @return : none
*/
CT_MsgRecordCurrentTime::CT_MsgRecordCurrentTime()
	{
	SetTestStepName(KRecordCurrentTime);
	}


/**
  Function : doTestStepL
  Description : This function is responsible for generating a log statement in EventLog server
  				by providing the event code to instrumentationhandler API.
  @return : TVerdict Test result
*/
TVerdict CT_MsgRecordCurrentTime::doTestStepL()
	{
	INFO_PRINTF1(_L("TestStep : RecordCurrentTime"));
	TPtrC eventCodeFromIni;
	if(!GetStringFromConfig(ConfigSection(), KEventCode, eventCodeFromIni))
		{
		ERR_PRINTF1(_L("Event code is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		TInstrumentationPoint eventcode = CT_MsgUtilsEnumConverter::ConvertDesToEventCode(eventCodeFromIni);
	
		CInstrumentationHandler *handler = CInstrumentationHandler::NewL();
		CleanupStack::PushL(handler);
		handler->RaiseEventNotification(eventcode);
		CleanupStack::PopAndDestroy(handler);
		}
	return TestStepResult();
	}


