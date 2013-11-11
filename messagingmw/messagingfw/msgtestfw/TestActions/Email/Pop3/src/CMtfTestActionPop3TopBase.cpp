// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file
*/

#include "CMtfTestCase.h"
#include "MtfTestActionUtilsUser.h"
#include "CMtfTestActionPop3TopBase.h"
#include "CMtfTestActionParameters.h"
#include "CPop3MailInfo.h"


CMtfTestActionPop3TopBase::CMtfTestActionPop3TopBase(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}

CMtfTestActionPop3TopBase::~CMtfTestActionPop3TopBase()
	{
	delete iOperation;
	}

void CMtfTestActionPop3TopBase::DoCancel()
	{
	iOperation->Cancel();
	}

void CMtfTestActionPop3TopBase::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus( *iOperation, iStatus );
	delete iOperation;
	iOperation = NULL;
	User::LeaveIfError(err);
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionPop3TopBase::DoInvokeAsyncFunctionL( TPop3Cmds aCommand )
	{
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(), ActionParameters().Parameter(0));
	CPop3ClientMtm *paramMtm = ObtainParameterReferenceL<CPop3ClientMtm>(TestCase(),ActionParameters().Parameter(1));
	CPop3MailInfo *paramMailInfo = ObtainParameterReferenceL<CPop3MailInfo>(TestCase(),ActionParameters().Parameter(2));
	CMsvEntrySelection* paramSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(3));

	TPop3MailInfoBuf paramPack( paramMailInfo->MailInfo() );
	paramSelection->InsertL( 0, paramServiceId ); 
	iOperation = paramMtm->InvokeAsyncFunctionL( aCommand, *paramSelection, paramPack, iStatus );	
	CActiveScheduler::Add(this);
	iStatus = KRequestPending;
	SetActive();
	}


