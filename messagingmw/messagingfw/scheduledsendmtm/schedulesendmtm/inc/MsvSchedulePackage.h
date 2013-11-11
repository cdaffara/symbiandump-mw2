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

#ifndef MSV_SCHEDULE_PACKAGE_H_
#define MSV_SCHEDULE_PACKAGE_H_

#include <s32std.h>
#include <msvstd.h>

//
//
//	TMsvSchedulePackage Declaration
//
//

const TInt KMaxParameterLength = 256;

class TTaskInfo;


/**
Packaged message scheduling information. 

@publishedAll
@released
*/

class TMsvSchedulePackage
	{
public:
	IMPORT_C TMsvSchedulePackage();
	
	IMPORT_C void PackLC(TTaskInfo& aTask, HBufC*& aDes) const;
	IMPORT_C void UnpackL(const TTaskInfo& aTask, const TDesC& aDes);

private:

	HBufC* PackLC() const;
	void UnpackL(const TDesC& aDes);

	void ExternalizeL(RWriteStream&) const;
	void InternalizeL(RReadStream&);

	TPtrC Convert(const TDesC8& aDes) const;
	TPtrC8 Convert(const TDesC16& aDes) const;

public:

	/** Message ID. */
	TMsvId iId;

	/** Command ID to be passed to CMsvSession::TransferCommandL() when it is
	time to send the messages. */
	TInt iCommandId;

	/** Intervals at which the scheduler should check the sending operation's
	progress. */
	TTimeIntervalMicroSeconds32 iPollProgress;

	/** Server MTM specific binary data to be passed to 
	CMsvSession::TransferCommandL() when it is time to send the messages. */
	TBuf8<KMaxParameterLength> iParameter;

	};

#endif
