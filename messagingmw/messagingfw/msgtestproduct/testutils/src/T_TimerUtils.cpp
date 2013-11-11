// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the Cpp file which contains the utility functions for the CTimer derived objects
// 
//

/**
 @file
*/


// user include
#include <t_timerutils.h>


const TInt KValue={1000000};
/**
NewL()
It performs the two phase construction and returns an object
of type CT_MsgTimerUtils

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgTimerUtils
*/
EXPORT_C CT_MsgTimerUtils* CT_MsgTimerUtils::NewL()
	{
	CT_MsgTimerUtils* self = new (ELeave) CT_MsgTimerUtils();
	CleanupStack::PushL(self);
	self->ConstructL();
	CActiveScheduler::Add(self);
	CleanupStack::Pop();
	return self;
	}
	


/**
CT_MsgTimerUtils()
Constructor
*/
CT_MsgTimerUtils::CT_MsgTimerUtils() : CTimer(EPriorityHigh)
	{
	}



/**
~CT_MsgTimerUtils()
Destructor
*/
CT_MsgTimerUtils::~CT_MsgTimerUtils()
	{
	iRequestStatus = 0; //To Remove Lint Warning
	}


/**
After()
Calls the After function CTimer for the given number of secs

@param aSec
No. of seconds

@param aStatus
Completion status of a request

@see CTimer::After()
*/
EXPORT_C void CT_MsgTimerUtils::After(TInt aSec,TRequestStatus& aStatus)
	{
	iRequestStatus = &aStatus;
	aStatus = KRequestPending;
	TTimeIntervalMicroSeconds32 interval = aSec*KValue;
	CTimer::After(interval);
	}


/**
RunL()
Gives the status of the operation 
*/
void CT_MsgTimerUtils::RunL()
	{
	User::RequestComplete(iRequestStatus,iStatus.Int());
	}

