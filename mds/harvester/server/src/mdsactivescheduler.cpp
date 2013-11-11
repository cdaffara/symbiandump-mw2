/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/

#include "mdsactivescheduler.h"
#include "harvesterlog.h"

EXPORT_C void CMdsActiveScheduler::WaitForAnyRequest()
	{
		if(iAllowedAOStatus != NULL)
			{
			User::WaitForRequest(*iAllowedAOStatus);
			}
		else
			{
			CActiveScheduler::WaitForAnyRequest();
			}
	}

EXPORT_C void CMdsActiveScheduler::SetAllowedAOStatus(TRequestStatus& aStatus)
	{
	WRITELOG("CMdsActiveScheduler::SetBackupSubscribeStatus()");
	iAllowedAOStatus = &aStatus;
	}

EXPORT_C void CMdsActiveScheduler::RemoveAllowedAOStatus()
	{
	WRITELOG("CMdsActiveScheduler::RemoveBackupSubscribeStatus()");
	iAllowedAOStatus = NULL;
	}

EXPORT_C void CMdsActiveScheduler::Error(TInt aError) const
	{
	_LIT( KPanicNote, "CMdsActiveScheduler");
	if ( aError != KErrNone )
		{
		User::Panic( KPanicNote, aError );
		}
	}
	
