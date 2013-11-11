// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSV_ENTRY_SCHEDULE_DATA_H_
#define MSV_ENTRY_SCHEDULE_DATA_H_

//
//
//	Includes
//
//

#include <msvstore.h>
#include <schinfo.h>
#include <csch_cli.h>
#include <schtask.h>

//
//
//	Constants
//
//

const TUid KUidMsvFileScheduleData			= {0x100056A2};	//Used by TMsvEntryScheduleData
const TInt16 KMsvEntryScheduleDataVersion	= 1;

//
//
//	TMsvEntryScheduleData Declaration
//
//

/**
Stores message-specific data related to scheduling.

The data is not required by the Client/UI side. This data is stored in a 
stream associated with the TMsvEntry. 

@publishedAll
@released
*/

class TMsvEntryScheduleData
	{
public:
	IMPORT_C TMsvEntryScheduleData();

	IMPORT_C void RestoreL(CMsvStore& aStore);
	IMPORT_C void StoreL(CMsvStore& aStore) const;
	IMPORT_C void RemoveL(CMsvStore& aStore) const;

	IMPORT_C void Reset();
	IMPORT_C TBool IsReset() const;

	IMPORT_C TInt Retries() const;
	IMPORT_C void IncreaseRetries();
	IMPORT_C void ResetRetries();

	/**
	The Task ID returned by the Task Scheduler when the message is 
	scheduled.
	*/
	TInt				iTaskId;
	
	/**
	The TSchedulerItemRef returned by the Task Scheduler when the schedule 
	on which the message is scheduled is created.
	*/
	TSchedulerItemRef	iRef;

protected:
	/** The number of retries that have been made to send this message. */
	TInt				iRetryCount;
	/** Structure version. */
	TInt16				iVersion;

	void ExternalizeL(RWriteStream& aWriteStream) const;
	void InternalizeL(RReadStream& aReadStream);
	};

#endif
