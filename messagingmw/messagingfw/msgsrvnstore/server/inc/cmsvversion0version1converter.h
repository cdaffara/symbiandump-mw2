// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMSVVERSION0VERSION1CONVERTER_H__
#define __CMSVVERSION0VERSION1CONVERTER_H__

// includes
#include <e32base.h>
#include <s32file.h>
#include <f32file.h>
#include <msvstd.h>
#include "msvdbadapter.h"
#include "cmessageconvertermanager.h"
#include "cmsvconverterwaiter.h"

//constants
const TInt KMsvStreamIndexGranularity=8;
const TInt KIndexReadGranularity=10;


// fwd declaration
class MMsvMessageStoreConverter;

/**
CMsvVersion0Version1Converter
This class derives from MMsvMessageStoreConverter and converts message store version 0 to store version 1
It traverses the file containg the message information and inserts them into the database table.

@internalAll 
@released
*/
NONSHARABLE_CLASS (CMsvVersion0Version1Converter): public CBase , public MMsvMessageStoreConverter
{
public:	
	static CMsvVersion0Version1Converter* NewL(CMsvDBAdapter& aDBAapter,TDriveNumber aDriveNum,TBool aConversionResume);
	~CMsvVersion0Version1Converter();
	void ConvertMessageStoreL();
	
private:	
	CMsvVersion0Version1Converter(CMsvDBAdapter& aDBAapter,TDriveNumber aDriveNum,TBool aConversionResume);	
	void ConstructL();
	void RestoreStreamIndexL();
	void PopulateDatabaseL(TInt aFirstEntry, TInt aNextEntry);
	TBool ReadNextEntriesL();
	void InternalizeL(RReadStream& aStream);
	void UpdateVisibleParentL();
	TInt GetPropertyValue();
private:
	CPermanentFileStore* iIndexStore;
	CArrayFixFlat<TStreamId>* iStream;
	TInt iEntryStreamIndex;
	CMsvDBAdapter& iDBAdapter;
	
	RFile iFileName;
	TConversionResumeStatus iResumeStatus;
	TDriveNumber iDrive;
	TBool iResume;
	
	TMsvEntry iEntry;	
	TMsvEntry iNext;
	TMsvEntry iSibling;
	TMsvEntry iChild;
	TMsvDays	iLastChangeDays;
	TMsvMinutes iLastChangeMinutes;
	TMsvDays	iCreatedDays;
	TMsvMinutes iCreatedMinutes;
	HBufC* iDescriptionBuffer;
	HBufC* iDetailsBuffer;
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)	
	friend class CTestVersion0Version1Converter;
#endif
	};

#endif // __CMSVVERSION0VERSION1CONVERTER_H__


