// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <csch_cli.h>
#include <msvapi.h>
#include <msvschedulepackage.h>
#include "SchSendExe.h"

//
// CMsvSendExeActive
//


/**

CMsvSendExeActive Factory Function

@return Newly constructed CMsvSendExeActive
@internalAll
@param				 aSession	Message server session
@param				 aParent	Used to callback on when the operation is complete
@param				 aPackage	Details of what type of operation to exectute
@param				 aMtm		The MTM of the message in aPackage
@param				 aPriority	CActive priority. Defaults to EPriorityStandard
*/
CMsvSendExeActive* CMsvSendExeActive::NewLC(CMsvSession& aSession, CMsvSendExe& aParent, const TMsvSchedulePackage& aPackage, const TUid& aMtm, TInt aPriority)
	{
	CMsvSendExeActive* self = new (ELeave) CMsvSendExeActive(aSession, aParent, aPackage, aMtm, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


/**

CMsvSendExeActive Destructor

@return			
@internalAll
*/
CMsvSendExeActive::~CMsvSendExeActive()
	{
	Cancel();
	delete iTimer;
	delete iOperation;
	delete iSelection;
	}


/**

The CMsvSendExeActive::StartL method
Starts the operation relating to iPackage.
Starts the timer that will poll iOperation's progress

@return			
@internalAll
*/
void CMsvSendExeActive::StartL()
	{
	__ASSERT_DEBUG(iOperation == NULL, gPanic(EObjectNotNull));
	__ASSERT_DEBUG(iTimer == NULL, gPanic(EObjectNotNull));

	iOperation = iSession.TransferCommandL(*iSelection, iPackage.iCommandId, iPackage.iParameter, iStatus);
	
	//Start the timer that will poll the progress on iOperation
	iTimer = CProgressTimer::NewL(*iOperation, iPackage.iPollProgress);
	iTimer->Start();
	SetActive();
	}
		

/**

CMsvSendExeActive Constructor

@return			
@internalAll
@param				 aSession	Message server session
@param				 aParent	Used to callback on when the operation is complete
@param				 aPackage	Details of what type of operation to exectute
@param				 aMtm		The MTM of the message in aPackage
@param				 aPriority	CActive priority. Defaults to EPriorityStandard
*/
CMsvSendExeActive::CMsvSendExeActive(CMsvSession& aSession, CMsvSendExe& aParent, const TMsvSchedulePackage& aPackage, const TUid& aMtm, TInt aPriority)
: CActive(aPriority), iSession(aSession), iParent(aParent), iPackage(aPackage), iMtm(aMtm)
	{
	CActiveScheduler::Add(this);
	}

void CMsvSendExeActive::ConstructL()
	{
	iSelection = new (ELeave) CMsvEntrySelection;
	}

void CMsvSendExeActive::RunL()
	{
	if (iTimer)
		iTimer->Cancel();

	iParent.OperationComplete(*this, iStatus.Int());
	}

void CMsvSendExeActive::DoCancel()
	{
	if (iOperation)
		iOperation->Cancel();

	if (iTimer)
		iTimer->Cancel();

	iParent.OperationComplete(*this, KErrCancel);
	}

void CMsvSendExeActive::AppendL(TMsvId aId)
	{
	if (iSelection->Find(aId) == KErrNotFound)
		iSelection->AppendL(aId);
	}


//
// CProgressTimer
//

CProgressTimer* CProgressTimer::NewL(CMsvOperation& aOperation, const TTimeIntervalMicroSeconds32& aPollInterval)
	{
	CProgressTimer* self = new (ELeave) CProgressTimer(aOperation, aPollInterval);
	CleanupStack::PushL(self);

	self->ConstructL();

	CleanupStack::Pop();

	return self;
	}

CProgressTimer::CProgressTimer(CMsvOperation& aOperation, const TTimeIntervalMicroSeconds32& aPollInterval)
: CTimer(EPriorityHigh), iOperation(aOperation), iPollInterval(aPollInterval)
	{
	CActiveScheduler::Add(this);
	}

CProgressTimer::~CProgressTimer()
	{
	Cancel();
	}

void CProgressTimer::Start()
	{
	if (iPollInterval.Int() > 0)
		{
		After(iPollInterval);
		}
	}

void CProgressTimer::RunL()
	{
	if (iPollInterval.Int() > 0)
		{
		// Ignore progress errors (we don't care)
		TRAPD(error, iOperation.ProgressL());
		After(iPollInterval);
		}
	}
