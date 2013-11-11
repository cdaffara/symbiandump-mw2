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

#include <e32base.h>
#include <e32std.h>
#include <msvstd.h>
#include <msvschedulepackage.h>
#include <s32mem.h>
#include <schinfo.h>

//
//
//	TMsvSchedulePackage Implementation
//
//

/**
Default constructor.
*/

EXPORT_C TMsvSchedulePackage::TMsvSchedulePackage()
: iId(0), iCommandId(0), iPollProgress(0)
	{
	}


HBufC* TMsvSchedulePackage::PackLC() const
	{
	//Externalize this into a CBufFlat
	CBufFlat* flat = CBufFlat::NewL(sizeof(TMsvSchedulePackage));
	CleanupStack::PushL(flat);

	RBufWriteStream writeStream(*flat);
	ExternalizeL(writeStream);
	writeStream.CommitL();

	const TPtr8 ptr8(flat->Ptr(0));

	//Convert the CBufFlat into a HBufC16
	HBufC* buf16 = Convert(ptr8).AllocL();

	CleanupStack::PopAndDestroy(flat);
	CleanupStack::PushL(buf16);

	return buf16;
	}


/**
Packs the object into a 16-bit descriptor. 

The function then attempts to copy the descriptor into aTask.iName, 
if aTask.iName is large enough; otherwise, it copies it into aDes (a new 
HBufC). 

aDes is placed on the cleanup stack.

@param aTask
Task information for this message.

@param aDes On return, a new descriptor, possibly packaging the object.
aDes.Length() == 0 if the object was successfully packed in aTask.iName 
instead.
*/

EXPORT_C void TMsvSchedulePackage::PackLC(TTaskInfo& aTask, HBufC*& aDes) const
	{
	aTask.iName.Zero();
	aDes = PackLC();

	if (aDes->Length() <= aTask.iName.MaxLength())
		{
		aTask.iName.Copy(*aDes);
		CleanupStack::PopAndDestroy(aDes);
		aDes = HBufC::NewLC(0);
		}
	}


/**
Restores this object from a 16-bit descriptor that was packed using PackLC().

@param aTask 
The object is restored from aTask.iName if aTask.iName.Length() doesn't equal
zero.

@param aDes 
The object is restored from this descriptor if aTask.iName.Length() equals 
zero.
*/

EXPORT_C void TMsvSchedulePackage::UnpackL(const TTaskInfo& aTask, const TDesC& aDes)
	{
	if (aTask.iName.Length() != 0)
		UnpackL(aTask.iName);
	else
		UnpackL(aDes);
	}


void TMsvSchedulePackage::UnpackL(const TDesC& aDes)
	{
	const TPtrC8 ptr8(Convert(aDes));
	RDesReadStream readStream(ptr8);
	InternalizeL(readStream);
	}


void TMsvSchedulePackage::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(iId);
	aStream.WriteInt32L(iCommandId);
	aStream.WriteInt32L(iPollProgress.Int());
	aStream << iParameter;
	}


void TMsvSchedulePackage::InternalizeL(RReadStream& aStream)
	{
	iId = aStream.ReadInt32L();
	iCommandId = aStream.ReadInt32L();
	iPollProgress = (TTimeIntervalMicroSeconds32) aStream.ReadInt32L();
	aStream >> iParameter;
	}


TPtrC TMsvSchedulePackage::Convert(const TDesC8& aDes) const
	{
	const TInt size8 = aDes.Size();
	const TInt size16 = (size8 / 2) + (size8 % 2);
	return TPtrC(REINTERPRET_CAST(const TUint16*, aDes.Ptr()), size16);
	}


TPtrC8 TMsvSchedulePackage::Convert(const TDesC16& aDes) const
	{
	return TPtrC8(REINTERPRET_CAST(const TUint8*, aDes.Ptr()), aDes.Size());
	}
