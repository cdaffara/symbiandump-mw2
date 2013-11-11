// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "Testsetup.h"
#include <msvrcpt.h>
#include <msvipc.h>
#include <msvuids.h>
#include <msvids.h>
#include "msvutils.h"

RTest test(_L("MSV Basic Testrig"));

_LIT(KTestFilename, "c:\\basic test file");

//

LOCAL_C void TestPrioritiesAndFlagsL()
	{
	// testing an unitilialsed entry
	TTestMsvEntry entry;
	test(entry.Id()==KMsvNullIndexEntryId);
	test(entry.Parent()==KMsvNullIndexEntryId);
	test(entry.Priority()==EMsvMediumPriority);
	test(!entry.Owner());
	test(!entry.Deleted());
	test(entry.Complete());
	test(!entry.New());
	test(!entry.Unread());
	test(!entry.Failed());
	test(!entry.Operation());
	test(entry.Visible());
	test(!entry.MultipleRecipients());
	test(!entry.ReadOnly());
	test(!entry.Connected());
	test(!entry.InPreparation());
	//
	test(entry.PcSyncCount()==0);
	//
	test(entry.iServiceId==KMsvNullIndexEntryId);
	test(entry.iRelatedId==KMsvNullIndexEntryId);
	test(entry.iType==KUidMsvNullEntry);
	test(entry.iMtm==KUidMsvNullEntry);
	test(entry.iError==KErrNone);
	test(entry.iBioType==0);
	test(entry.MtmData1()==0);
	test(entry.MtmData2()==0);
	test(entry.MtmData3()==0);
	//
	test(entry.iDescription==_L(""));
	test(entry.iDetails==_L(""));


	// changing priority
	TTestMsvEntry nullEntry;	
	nullEntry.SetPriority(EMsvLowPriority);
	test(nullEntry.Priority()==EMsvLowPriority);
	nullEntry.SetPriority(EMsvHighPriority);
	test(nullEntry.Priority()==EMsvHighPriority);
	nullEntry.SetPriority(EMsvMediumPriority);
	test(nullEntry.Priority()==EMsvMediumPriority);
	nullEntry.SetPriority(EMsvLowPriority);
	test(nullEntry.Priority()==EMsvLowPriority);
	nullEntry.SetPriority(EMsvMediumPriority);
	test(nullEntry.Priority()==EMsvMediumPriority);
	nullEntry.SetPriority(EMsvHighPriority);
	test(nullEntry.Priority()==EMsvHighPriority);
	nullEntry.SetPriority(EMsvLowPriority);
	test(nullEntry.Priority()==EMsvLowPriority);
	
	// setting some flags
	nullEntry.SetComplete(ETrue);
	nullEntry.SetNew(ETrue);
	nullEntry.SetUnread(ETrue);
	nullEntry.SetFailed(ETrue);
	nullEntry.SetOperation(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));

	// unsetting and setting one
	nullEntry.SetComplete(EFalse);
	test(!(nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	nullEntry.SetComplete(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	//
	nullEntry.SetNew(EFalse);
	test(!(!nullEntry.Complete() || nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	nullEntry.SetNew(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	//
	nullEntry.SetUnread(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	nullEntry.SetUnread(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	//
	nullEntry.SetFailed(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	nullEntry.SetFailed(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	//
	nullEntry.SetOperation(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly()));
	nullEntry.SetOperation(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly()));
	//
	nullEntry.SetReadOnly(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	nullEntry.SetReadOnly(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || !nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	nullEntry.SetReadOnly(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	//
	nullEntry.SetMultipleRecipients(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	nullEntry.SetMultipleRecipients(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || !nullEntry.MultipleRecipients()));
	nullEntry.SetMultipleRecipients(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	//
	nullEntry.SetOwner(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	nullEntry.SetOwner(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || !nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	nullEntry.SetOwner(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients()));
	//
	nullEntry.SetVisible(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible()));
	nullEntry.SetVisible(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || !nullEntry.Visible()));
	nullEntry.SetVisible(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible()));
	//
	nullEntry.SetDeleted(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible() || nullEntry.Deleted()));
	nullEntry.SetDeleted(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible() || !nullEntry.Deleted()));
	nullEntry.SetDeleted(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible() || nullEntry.Deleted()));
	//
	nullEntry.SetInPreparation(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible() || nullEntry.Deleted()  || !nullEntry.InPreparation()));
	nullEntry.SetInPreparation(EFalse);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible() || nullEntry.Deleted()  || nullEntry.InPreparation()));
	nullEntry.SetInPreparation(ETrue);
	test(!(!nullEntry.Complete() || !nullEntry.New() || !nullEntry.Unread() || !nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner() || nullEntry.ReadOnly() || nullEntry.MultipleRecipients() || nullEntry.Visible() || nullEntry.Deleted()  || !nullEntry.InPreparation()));


	// unsetting all flags (not the attachment flag)
	nullEntry.SetComplete(EFalse);
	nullEntry.SetNew(EFalse);
	nullEntry.SetUnread(EFalse);
	nullEntry.SetFailed(EFalse);
	nullEntry.SetOperation(EFalse);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));

	// setting and unsetting one
	nullEntry.SetComplete(ETrue);
	test(!(!nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	nullEntry.SetComplete(EFalse);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	//
	nullEntry.SetNew(ETrue);
	test(!(nullEntry.Complete() || !nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	nullEntry.SetNew(EFalse);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	//
	nullEntry.SetUnread(ETrue);
	test(!(nullEntry.Complete() || nullEntry.New() || !nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner() || nullEntry.Connected()));
	nullEntry.SetUnread(EFalse);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner() || nullEntry.Connected()));
	//
	nullEntry.SetFailed(ETrue);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || !nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner() || nullEntry.Connected()));
	nullEntry.SetFailed(EFalse);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner() || nullEntry.Connected()));
	//
	nullEntry.SetOperation(ETrue);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || !nullEntry.Operation() || nullEntry.Owner()));
	nullEntry.SetOperation(EFalse);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	//
	nullEntry.SetConnected(ETrue);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner() || !nullEntry.Connected()));
	nullEntry.SetConnected(EFalse);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner() || nullEntry.Connected()));
	//

	// changing priority with one flag false
	nullEntry.SetPriority(EMsvHighPriority);
	test(! (nullEntry.Priority()!=EMsvHighPriority));
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	// changing priority with one flag true
	nullEntry.SetPriority(EMsvMediumPriority);
	test(! (nullEntry.Priority()!=EMsvMediumPriority));
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	// changing two flags true while medium
	nullEntry.SetNew(ETrue);
	nullEntry.SetComplete(ETrue);
	test(nullEntry.Priority()==EMsvMediumPriority);
	test(!(!nullEntry.Complete() || !nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	// changing two flags false while medium
	nullEntry.SetNew(EFalse);
	nullEntry.SetComplete(EFalse);
	test(nullEntry.Priority()==EMsvMediumPriority);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	// changing to low
	nullEntry.SetPriority(EMsvLowPriority);
	// changing two flags true while medium
	nullEntry.SetNew(ETrue);
	nullEntry.SetComplete(ETrue);
	test(nullEntry.Priority()==EMsvLowPriority);
	test(!(!nullEntry.Complete() || !nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	// changing two flags false while medium
	nullEntry.SetNew(EFalse);
	nullEntry.SetComplete(EFalse);
	test(nullEntry.Priority()==EMsvLowPriority);
	test(!(nullEntry.Complete() || nullEntry.New() || nullEntry.Unread() || nullEntry.Failed() || nullEntry.Operation() || nullEntry.Owner()));
	

	// PC count
	test(nullEntry.PcSyncCount()==0);
	nullEntry.IncPcSyncCount();
	test(nullEntry.PcSyncCount()==1);
	nullEntry.DecPcSyncCount();
	test(nullEntry.PcSyncCount()==0);
	nullEntry.IncPcSyncCount();
	nullEntry.IncPcSyncCount();
	test(nullEntry.PcSyncCount()==2);
	nullEntry.DecPcSyncCount();
	test(nullEntry.PcSyncCount()==1);
	nullEntry.DecPcSyncCount();
	test(nullEntry.PcSyncCount()==0);
	nullEntry.DecPcSyncCount();
	test(nullEntry.PcSyncCount()==-1);

	}


LOCAL_C TBool EntriesEqual(TTestMsvEntry& aEntry1, TTestMsvEntry&aEntry2)
	{
	if (aEntry1.Id() != aEntry2.Id())
		return EFalse;
	if (aEntry1.Parent() != aEntry2.Parent())
		return EFalse;
	if (aEntry1.Owner() != aEntry2.Owner())
		return EFalse;
	if (aEntry1.Deleted() != aEntry2.Deleted())
		return EFalse;
	//
	if (aEntry1.Priority() != aEntry2.Priority())
		return EFalse;
	if (aEntry1.Complete() != aEntry2.Complete())
		return EFalse;
	if (aEntry1.New() != aEntry2.New())
		return EFalse;
	if (aEntry1.Unread() != aEntry2.Unread())
		return EFalse;
	if (aEntry1.Failed() != aEntry2.Failed())
		return EFalse;
	if (aEntry1.Operation() != aEntry2.Operation())
		return EFalse;
	if (aEntry1.Visible() != aEntry2.Visible())
		return EFalse;
	if (aEntry1.MultipleRecipients() != aEntry2.MultipleRecipients())
		return EFalse;
	if (aEntry1.ReadOnly() != aEntry2.ReadOnly())
		return EFalse;
	//
	if (aEntry1.PcSyncCount() != aEntry2.PcSyncCount())
		return EFalse;
	if (aEntry1.Reserved() != aEntry2.Reserved())
		return EFalse;
	//
	if (aEntry1.iServiceId != aEntry2.iServiceId)
		return EFalse;
	if (aEntry1.iRelatedId != aEntry2.iRelatedId)
		return EFalse;
	if (aEntry1.iType != aEntry2.iType)
		return EFalse;
	if (aEntry1.iMtm != aEntry2.iMtm)
		return EFalse;
	if (aEntry1.iDate != aEntry2.iDate)
		return EFalse;
	if (aEntry1.iSize != aEntry2.iSize)
		return EFalse;
	if (aEntry1.iError != aEntry2.iError)
		return EFalse;
	if (aEntry1.iBioType != aEntry2.iBioType)
		return EFalse;
	if (aEntry1.MtmData1() != aEntry2.MtmData1())
		return EFalse;
	if (aEntry1.MtmData2() != aEntry2.MtmData2())
		return EFalse;
	if (aEntry1.MtmData3() != aEntry2.MtmData3())
		return EFalse;
	//
	if (aEntry1.iDescription != aEntry2.iDescription)
		return EFalse;
	if (aEntry1.iDetails != aEntry2.iDetails)
		return EFalse;
	
	return ETrue;
	}



LOCAL_C void TestCopyingL()
	{
	TTestMsvEntry entry1;
	entry1.SetId(1);
	entry1.SetPriority(EMsvHighPriority);
	entry1.SetOperation(ETrue);
	entry1.SetParent(2);
	entry1.iServiceId = 3;
	entry1.iRelatedId = 2;
	TUid uid1,uid2;
	uid1.iUid = 1;
	uid2.iUid = 2;
	entry1.iType = uid1;
	entry1.iMtm	 = uid2;
	TBuf<32> des,det;
	des.Copy(_L("description"));
	det.Copy(_L("details"));
	entry1.iDescription.Set(des);
	entry1.iDetails.Set(det);
	entry1.iDate.HomeTime();
	entry1.iSize = 1234;
	entry1.SetMtmData1(1111);
	entry1.SetMtmData2(2222);
	entry1.SetMtmData3(3333);

	// copy c'tor
	TTestMsvEntry entry2(entry1);
	test(EntriesEqual(entry1,entry2));

	// assignment operator
	TTestMsvEntry entry3;
	entry3=entry1;
	test(EntriesEqual(entry1,entry3));

	// equality test when exactly equal
	test(entry1==entry2);
	test(entry1==entry3);

	// equality test with priority different
	entry3.SetPriority(EMsvMediumPriority);
	test(!(entry1==entry3));
	test(!(entry2==entry3));

	// equality test with different des pointers
	entry3.SetPriority(EMsvHighPriority);
	entry3.iDetails.Set(des);
	test(!(entry1==entry3));
	test(!(entry2==entry3));

	// equality test with different reserved data 2 (last data member)
	entry3.iDetails.Set(det);
	entry3.SetMtmData2(0);
	test(!(entry1==entry3));
	test(!(entry2==entry3));

	// permanent data only
	entry1=entry2;
	test(entry1.PermanentDataUnchanged(entry2));
	entry2.SetConnected(ETrue);
	test(entry1.PermanentDataUnchanged(entry2));
	entry1.SetConnected(ETrue);
	test(entry2.PermanentDataUnchanged(entry1));
	entry1.SetOwner(ETrue);
	test(!entry2.PermanentDataUnchanged(entry1));
	test(!entry1.PermanentDataUnchanged(entry2));
	entry1.SetConnected(EFalse);
	test(!entry2.PermanentDataUnchanged(entry1));
	test(!entry1.PermanentDataUnchanged(entry2));
	entry2.SetConnected(EFalse);
	test(!entry2.PermanentDataUnchanged(entry1));
	test(!entry1.PermanentDataUnchanged(entry2));
	
	}


LOCAL_C void TestPackingL()
	{
	TInt count;
	
	TBuf<32> details = _L("Nothing much");
	TBuf<32> description = _L("A lot more but means very little");
	
	TMsvEntry entry1;
	entry1.iServiceId = 101;
	entry1.iDescription.Set(description);
	entry1.iDetails.Set(details);

	TMsvEntry entry2;
	entry2.iServiceId = 546456;
	entry2.iDescription.Set(description);
	
	TMsvEntry entry3;
	entry2.iServiceId = 43;
	entry2.iDetails.Set(details);

	TMsvEntry outEntry;

	//
	// entry packing
	HBufC8* buffer = HBufC8::NewL(256);

	TMsvPackedEntry pEntry(buffer);

	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);

	REPORT_IF_ERROR(pEntry.PackEntry(entry2));
	pEntry.UnpackEntry(outEntry);
	test(entry2==outEntry);

	REPORT_IF_ERROR(pEntry.PackEntry(entry3));
	pEntry.UnpackEntry(outEntry);
	test(entry3==outEntry);
	
	// different string lengths
	entry1.iDescription.Set(_L("A lot more but means very littleX"));
	entry1.iDetails.Set(_L("Nothing muchX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	//
	entry1.iDescription.Set(_L("A lot more but means very littleXX"));
	entry1.iDetails.Set(_L("Nothing muchX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	//
	entry1.iDescription.Set(_L("A lot more but means very littleXXX"));
	entry1.iDetails.Set(_L("Nothing muchX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	//
	entry1.iDescription.Set(_L("A lot more but means very littleXXXX"));
	entry1.iDetails.Set(_L("Nothing muchX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);
	entry1.iDetails.Set(_L("Nothing muchXXXX"));
	REPORT_IF_ERROR(pEntry.PackEntry(entry1));
	pEntry.UnpackEntry(outEntry);
	test(entry1==outEntry);



	delete buffer;

	//
	// entry array packing

	buffer = HBufC8::NewL(1024);

	TMsvPackedEntryArray pEntryArray(buffer);

	entry1.iDescription.Set(_L("A lot more but means very little"));
	entry1.iDetails.Set(_L("Nothing much"));
	entry2.iDescription.Set(_L("A lot more but means very littleX"));
	entry2.iDetails.Set(_L("Nothing muchX"));
	entry3.iDescription.Set(_L("A lot more but means very littleXX"));
	entry3.iDetails.Set(_L("Nothing muchXXX"));

	// single entry
	REPORT_IF_ERROR(pEntryArray.PackEntry(entry1));
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry1==outEntry);

	// two entries
	pEntryArray.Reset();
	REPORT_IF_ERROR(pEntryArray.PackEntry(entry2));
	REPORT_IF_ERROR(pEntryArray.PackEntry(entry1));
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry2==outEntry);
	pEntryArray.UnpackEntry(1, outEntry);
	test(entry1==outEntry);
	pEntryArray.UnpackEntry(1, outEntry);
	test(entry1==outEntry);
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry2==outEntry);
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry2==outEntry);

	// three entries
	pEntryArray.Reset();
	REPORT_IF_ERROR(pEntryArray.PackEntry(entry3));
	REPORT_IF_ERROR(pEntryArray.PackEntry(entry2));
	REPORT_IF_ERROR(pEntryArray.PackEntry(entry1));
	pEntryArray.UnpackEntry(2, outEntry);
	test(entry1==outEntry);
	pEntryArray.UnpackEntry(1, outEntry);
	test(entry2==outEntry);
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry3==outEntry);
	pEntryArray.UnpackEntry(1, outEntry);
	test(entry2==outEntry);
	pEntryArray.UnpackEntry(2, outEntry);
	test(entry1==outEntry);
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry3==outEntry);
	pEntryArray.UnpackEntry(2, outEntry);
	test(entry1==outEntry);
	pEntryArray.UnpackEntry(2, outEntry);
	test(entry1==outEntry);
	pEntryArray.UnpackEntry(1, outEntry);
	test(entry2==outEntry);
	pEntryArray.UnpackEntry(1, outEntry);
	test(entry2==outEntry);
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry3==outEntry);
	pEntryArray.UnpackEntry(0, outEntry);
	test(entry3==outEntry);


	delete buffer;

	//
	// operation packing

	buffer = HBufC8::NewL(256);

	CMsvEntrySelection* inSl	= new (ELeave) CMsvEntrySelection;
	CMsvEntrySelection* outSl	= new (ELeave) CMsvEntrySelection;
	TMsvPackedOperation pO(buffer);

	// packing empty selection
	TInt p1I = 23, p1O;
	TInt p2I = -765, p2O;
	REPORT_IF_ERROR(pO.Pack(*inSl, p1I));
	pO.UnpackL(*outSl, p1O, p2O);
	test(p1I==p1O);
	test(0==p2O);
	test(inSl->Count()==outSl->Count());

	// packing actual selection
	p1I = -23;
	inSl->AppendL(11);
	inSl->AppendL(22);
	REPORT_IF_ERROR(pO.Pack(*inSl, p1I, p2I));
	pO.UnpackL(*outSl, p1O, p2O);
	test(p1I==p1O);
	test(p2I==p2O);
	test(inSl->Count()==outSl->Count());
	for (count=0; count<inSl->Count(); count++)
		test(inSl->At(count)==outSl->At(count));
	outSl->Reset();

	
	// moving the buffer
	TMsvPackedOperation pO1(buffer);
	pO1.UnpackL(*outSl, p1O, p2O);
	test(p1I==p1O);
	test(p2I==p2O);
	test(inSl->Count()==outSl->Count());
	for (count=0; count<inSl->Count(); count++)
		test(inSl->At(count)==outSl->At(count));
	
	
	// too small a buffer
	buffer->ReAlloc(32);
	inSl->AppendL(10, 11);
	test(pO.Pack(*inSl, p1I, p2I)==KErrOverflow);


	delete buffer;

	//
	// change notifiction packing
	TMsvNotifBuffer buffer2;

	TMsvPackedChangeNotification changeNotPackage(buffer2);

	TMsvServerChangeNotificationType inCh, outCh;
	TInt inP1, outP1, inP2, outP2;

	// packing empty selection
	inSl->Reset();
	outSl->Reset();
	inCh = EMsvEntriesCreated;
	inP1 = 345;
	inP2 = 67;
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==inSl->Count());
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);

	// packing single id
	inP2 = 99;
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==inSl->Count());
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);

	inSl->Reset();
	outSl->Reset();

	// packing single selection
	inSl->AppendL(123);
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==inSl->Count());
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);

	inSl->Reset();
	outSl->Reset();

	// testing different change types
	inCh = EMsvEntriesCreated;
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==inSl->Count());
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);
	//
	inCh = EMsvEntriesChanged;
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==inSl->Count());
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);
	//
	inCh = EMsvEntriesDeleted;
	inP1 = 567;
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==inSl->Count());
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);
	//
	inCh = EMsvEntriesMoved;
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==inSl->Count());
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);

	// packing max size selection
	inSl->Reset();
	outSl->Reset();
	for (count=0; count<TMsvPackedChangeNotification::KMsvPackedChangeLimit; count++)
		inSl->AppendL(count+4);
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==TMsvPackedChangeNotification::KMsvPackedChangeLimit);
	for (count=0; count<TMsvPackedChangeNotification::KMsvPackedChangeLimit; count++)
		test(outSl->At(count)==count+4);
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);

	// packing subselection 
	inSl->Reset();
	outSl->Reset();
	for (count=0; count<TMsvPackedChangeNotification::KMsvPackedChangeLimit; count++)
		inSl->AppendL(count);
	changeNotPackage.Pack(inCh, *inSl, inP1, inP2, 4, 10);
	changeNotPackage.UnpackL(outCh, *outSl, outP1, outP2);
	test(outSl->Count()==7);
	for (count=0; count<outSl->Count(); count++)
		test(outSl->At(count)==count+4);
	test(inCh==outCh);
	test(inP1==outP1);
	test(inP2==outP2);



	delete inSl;
	delete outSl;
	}


	
LOCAL_C void TestUtilities1L()
	{
	TTestMsvEntry entry;

	const TUid uid1={0x1000112};

	// blank
	test(!MsvUtils::ValidEntry(entry));

	//
	// Message
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= KUidMsvNullEntry;
	entry.SetParent(KMsvNullIndexEntryId);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvNullIndexEntryId);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(1234);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(1234);
	entry.iServiceId = 5678;
	test(MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvNullIndexEntryId);
	entry.iServiceId = 5678;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iServiceId = 5678;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iServiceId = KMsvRootIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm	= uid1;
	entry.SetParent(12324);
	entry.iServiceId = 123435;
	test(MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvFolderEntry;
	entry.iMtm	= uid1;
	entry.SetParent(12324);
	entry.iServiceId = 123435;
	test(MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvAttachmentEntry;
	entry.iMtm	= uid1;
	entry.SetParent(12324);
	entry.iServiceId = 123435;
	test(MsvUtils::ValidEntry(entry));
	//
	entry.SetId(32432);
	entry.iType = KUidMsvAttachmentEntry;
	entry.iMtm	= uid1;
	entry.SetParent(12324);
	entry.iServiceId = 123435;
	test(MsvUtils::ValidEntry(entry));


	//
	// Service
	entry.SetId(KMsvNullIndexEntryId);
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm	= KUidMsvNullEntry;
	entry.SetParent(KMsvNullIndexEntryId);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvNullIndexEntryId);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm	= uid1;
	entry.SetParent(1234);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm	= uid1;
	entry.SetParent(1234);
	entry.iServiceId = 45546;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iServiceId = KMsvNullIndexEntryId;
	test(MsvUtils::ValidEntry(entry));
	//
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iServiceId = 43534;
	test(!MsvUtils::ValidEntry(entry));
	//
	entry.SetId(43534);
	entry.iType = KUidMsvServiceEntry;
	entry.iMtm	= uid1;
	entry.SetParent(KMsvRootIndexEntryId);
	entry.iServiceId = 43534;
	test(MsvUtils::ValidEntry(entry));
	}

LOCAL_C void TestUtilities2L()
	{
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// MsvUtils::ConstructEntryName() has already been tested as part of Unittef tests for PREQ557.
	// Also, the below tests do not conform to the TMsvId's generated in PREQ557.
	TFileName name;

	// entry
	name=_L("c:\\a directory\\");
	MsvUtils::ConstructEntryName(TMsvId(0x00112233), TMsvId(0x44556677), name, MsvUtils::EStore);
	test(name==_L("c:\\a directory\\00112233_S\\7\\44556677"));
	name=_L("c:\\a directory\\");
	MsvUtils::ConstructEntryName(TMsvId(0x00112233), TMsvId(0x44556677), name, MsvUtils::EFolder);
	test(name==_L("c:\\a directory\\00112233_S\\7\\44556677_F\\"));
	name=_L("c:\\a directory\\");
	MsvUtils::ConstructEntryName(TMsvId(0xAABBCCDD), TMsvId(0x20000001), name, MsvUtils::EStore);
	test(name==_L("c:\\a directory\\aabbccdd_S\\1\\20000001"));
	name=_L("c:\\a directory\\");
	MsvUtils::ConstructEntryName(TMsvId(0xAABBCCDD), TMsvId(0x20000001), name, MsvUtils::EFolder);
	test(name==_L("c:\\a directory\\aabbccdd_S\\1\\20000001_F\\"));

	// service
	name=_L("c:\\a directory\\");
	MsvUtils::ConstructEntryName(TMsvId(0xAABBCCee), TMsvId(0xAABBCCee), name, MsvUtils::EStore);
	test(name==_L("c:\\a directory\\aabbccee"));
	name=_L("c:\\a junk name\\");
	MsvUtils::ConstructEntryName(TMsvId(0xAABBCCee), TMsvId(0xAABBCCee), name, MsvUtils::EPath);
	test(name==_L("c:\\a junk name\\aabbccee_S\\"));
	name=_L("c:\\a junk name\\");
	MsvUtils::ConstructEntryName(TMsvId(0xAABBCCee), TMsvId(0xAABBCCee), name, MsvUtils::EFolder);
	test(name==_L("c:\\a junk name\\aabbccee_F\\"));
	
	// root
	name=_L("c:\\a directory\\");
	MsvUtils::ConstructEntryName(KMsvRootIndexEntryId, KMsvRootIndexEntryId, name, MsvUtils::EStore);
	test(name==_L("c:\\a directory\\00001000"));
#endif
	}

LOCAL_C void TestStoreRestoreL(CDictionaryFileStore& aStore, TMsvSelectionOrdering& aOrder, TUid aUid)
	{
	RDictionaryWriteStream write;
	write.AssignLC(aStore, aUid);
	write << aOrder;
	write.CommitL();
	aStore.Commit();
	CleanupStack::PopAndDestroy();
	TMsvSelectionOrdering order;
	RDictionaryReadStream read;
	read.OpenLC(aStore, aUid);
	read >> order;
	CleanupStack::PopAndDestroy();
	test(aOrder==order);
	}

LOCAL_C void TestOrderingL()
	{
	// c'tor testing
	TMsvSelectionOrdering nullOrder;
	test(nullOrder.GroupByType() == EFalse);
	test(nullOrder.GroupByPriority() == EFalse);
	test(nullOrder.GroupByMtm() == EFalse);
	test(nullOrder.Sorting() == EMsvSortByNone);

	TMsvSelectionOrdering setOrder1(KMsvGroupByType, EMsvSortByDate);
	test(setOrder1.GroupByType());
	test(setOrder1.GroupByPriority() == EFalse);
	test(setOrder1.GroupByMtm() == EFalse);
	test(setOrder1.Sorting() == EMsvSortByDate);

	TMsvSelectionOrdering setOrder2(KMsvGroupByType|KMsvGroupByPriority, EMsvSortByDetails);
	test(setOrder2.GroupByType());
	test(setOrder2.GroupByPriority());
	test(setOrder2.GroupByMtm() == EFalse);
	test(setOrder2.Sorting() == EMsvSortByDetails);

	TMsvSelectionOrdering setOrder3(KMsvGroupByPriority|KMsvGroupByMtm, EMsvSortByDescriptionReverse);
	test(setOrder3.GroupByType()==EFalse);
	test(setOrder3.GroupByPriority());
	test(setOrder3.GroupByMtm());
	test(setOrder3.Sorting() == EMsvSortByDescriptionReverse);

	// comparison testing
	TMsvSelectionOrdering order1;
	TMsvSelectionOrdering order2;
	test(order1==order2);
	order1.SetGroupByType(ETrue);
	test(!(order1==order2));
	order2.SetGroupByType(ETrue);
	test(order1==order2);
	order1.SetGroupByPriority(ETrue);
	test(!(order1==order2));
	order2.SetGroupByPriority(ETrue);
	test(order1==order2);
	order1.SetGroupByMtm(ETrue);
	test(!(order1==order2));
	order2.SetGroupByMtm(ETrue);
	test(order1==order2);
	order1.SetSorting(EMsvSortByDetails);
	test(!(order1==order2));
	order2.SetSorting(EMsvSortByDetails);
	test(order1==order2);
	order1.SetSorting(EMsvSortByDescriptionReverse);
	test(!(order1==order2));
	order2.SetSorting(EMsvSortByDescriptionReverse);
	test(order1==order2);


	// test standard folders settings
	order1 = TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByNone, ETrue);
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders());	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupStandardFolders(EFalse); 
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupStandardFolders(ETrue); 
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders());	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	//
	order1 = TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue);
	test(order1.GroupByType()==EFalse);	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupStandardFolders(ETrue); 
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders());	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupStandardFolders(EFalse); 
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	//
	order1 = TMsvSelectionOrdering(KMsvGroupByType, EMsvSortByNone, ETrue);
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupByType(EFalse); 
	test(order1.GroupByType()==EFalse);	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupByType(ETrue); 
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	//
	order1 = TMsvSelectionOrdering(KMsvGroupByType, EMsvSortByNone, ETrue);
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupStandardFolders(ETrue); 
	test(order1.GroupByType());	
	test(order1.GroupStandardFolders());	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());
	//
	order1.SetGroupByType(EFalse); 
	test(order1.GroupByType()==EFalse);	
	test(order1.GroupStandardFolders()==EFalse);	
	test(order1.GroupByPriority()==EFalse);	
	test(order1.GroupByMtm()==EFalse);	
	test(order1.ShowInvisibleEntries());


	// test group all flag
	order1 = TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, EFalse);
	test(order1.GroupingOn()==EFalse);
	order1 = TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue);
	test(order1.GroupingOn()==EFalse);
	order1 = TMsvSelectionOrdering(KMsvGroupByType, EMsvSortByNone, ETrue);
	test(order1.GroupingOn());
	order1 = TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByNone, ETrue);
	test(order1.GroupingOn());
	order1 = TMsvSelectionOrdering(KMsvGroupByPriority, EMsvSortByNone, ETrue);
	test(order1.GroupingOn());
	order1 = TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone, ETrue);
	test(order1.GroupingOn());



	// storing/restoring
	TMsvSelectionOrdering orderA;
	CDictionaryFileStore* fileStore = CDictionaryFileStore::OpenLC(theUtils->FileSession(), KTestFilename, TUid());
	TUid uid={0x10002323};

	// grouping flags
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetGroupByType(ETrue);
	orderA.SetGroupByPriority(EFalse);
	orderA.SetGroupByMtm(EFalse);
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetGroupByType(EFalse);
	orderA.SetGroupStandardFolders(ETrue);
	orderA.SetGroupByPriority(ETrue);
	orderA.SetGroupByMtm(EFalse);
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetGroupByType(EFalse);
	orderA.SetGroupStandardFolders(EFalse);
	orderA.SetGroupByPriority(EFalse);
	orderA.SetGroupByMtm(ETrue);
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetGroupByType(ETrue);
	orderA.SetGroupByPriority(ETrue);
	orderA.SetGroupByMtm(ETrue);
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;

	// sort order
	orderA.SetGroupByType(ETrue);
	orderA.SetGroupByPriority(ETrue);
	orderA.SetGroupByMtm(ETrue);
	orderA.SetSorting(EMsvSortByNone);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetSorting(EMsvSortByDate);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetSorting(EMsvSortByDateReverse);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetGroupByType(EFalse);
	orderA.SetGroupStandardFolders(ETrue);
	orderA.SetGroupByPriority(ETrue);
	orderA.SetGroupByMtm(EFalse);
	orderA.SetSorting(EMsvSortBySize);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetSorting(EMsvSortBySizeReverse);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetSorting(EMsvSortByDetails);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetGroupByType(EFalse);
	orderA.SetGroupStandardFolders(ETrue);
	orderA.SetGroupByPriority(ETrue);
	orderA.SetGroupByMtm(EFalse);
	orderA.SetSorting(EMsvSortByDetailsReverse);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetSorting(EMsvSortByDescription);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetSorting(EMsvSortByDescriptionReverse);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;
	//
	orderA.SetSorting(EMsvSortByNone);	
	TestStoreRestoreL(*fileStore, orderA, uid);
	uid.iUid++;

	CleanupStack::PopAndDestroy(); // fileStore
	theUtils->FileSession().Delete(KTestFilename);
	}


LOCAL_C void TestSelectionsL()
	{
	CMsvEntrySelection* select1;
	CMsvEntrySelection* select2;
	TInt max;

	//
	// CopyL()
	// copy empty selection
	select1 = new (ELeave) CMsvEntrySelection;
	select2 = select1->CopyL();
	test(select1->Count()==0);
	test(select2->Count()==0);
	delete select2;
	// single entry
	select1->AppendL(45);
	test(select1->Count()==1);
	test(select1->At(0)==45);
	select2 = select1->CopyL();
	test(select1->Count()==1);
	test(select2->Count()==1);
	test(select1->At(0)==45);
	test(select2->At(0)==45);
	delete select2;
	// mutiple entries
	for (max=0; max<50; max++)
		{
		select1->Reset();
		TInt ii;
		for (ii=0; ii<max; ii++)
			select1->AppendL(ii);
		test(select1->Count()==max);
		select2 = select1->CopyL();
		test(select1->Count()==max);
		test(select2->Count()==max);
		for (ii=0; ii<max; ii++)
			test(select2->At(ii)==ii);
		CleanupStack::PushL(select2);
		}
	CleanupStack::PopAndDestroy(max);
	delete select1;

	//
	// CopyLC()
	// copy empty selection
	select1 = new (ELeave) CMsvEntrySelection;
	select2 = select1->CopyLC();
	test(select1->Count()==0);
	test(select2->Count()==0);
	CleanupStack::PopAndDestroy();
	// single entry
	select1->AppendL(45);
	test(select1->Count()==1);
	test(select1->At(0)==45);
	select2 = select1->CopyLC();
	test(select1->Count()==1);
	test(select2->Count()==1);
	test(select1->At(0)==45);
	test(select2->At(0)==45);
	CleanupStack::PopAndDestroy();
	// mutiple entries
	for (max=0; max<50; max++)
		{
		select1->Reset();
		TInt ii;
		for (ii=0; ii<max; ii++)
			select1->AppendL(ii);
		test(select1->Count()==max);
		select2 = select1->CopyLC();
		test(select1->Count()==max);
		test(select2->Count()==max);
		for (ii=0; ii<max; ii++)
			test(select2->At(ii)==ii);
		}
	CleanupStack::PopAndDestroy(max);
	delete select1;
	

	//
	// Find()
	select1 = new (ELeave) CMsvEntrySelection;
	// empty selection
	test(select1->Find(3466)==KErrNotFound);
	// single entry
	select1->AppendL(1234);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==0);
	// double entry
	select1->AppendL(5678);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==0);
	test(select1->Find(5678)==1);
	// triple entry
	select1->AppendL(66);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==0);
	test(select1->Find(5678)==1);
	test(select1->Find(66)==2);
	// double entry
	select1->Delete(2);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==0);
	test(select1->Find(5678)==1);
	test(select1->Find(66)==KErrNotFound);
	// triple entry
	select1->AppendL(99);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==0);
	test(select1->Find(5678)==1);
	test(select1->Find(66)==KErrNotFound);
	test(select1->Find(99)==2);
	// double entry
	select1->Delete(1);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==0);
	test(select1->Find(5678)==KErrNotFound);
	test(select1->Find(66)==KErrNotFound);
	test(select1->Find(99)==1);
	// triple entry
	select1->AppendL(11);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==0);
	test(select1->Find(5678)==KErrNotFound);
	test(select1->Find(66)==KErrNotFound);
	test(select1->Find(99)==1);
	test(select1->Find(11)==2);
	// double entry
	select1->Delete(0);
	test(select1->Find(3466)==KErrNotFound);
	test(select1->Find(1234)==KErrNotFound);
	test(select1->Find(5678)==KErrNotFound);
	test(select1->Find(66)==KErrNotFound);
	test(select1->Find(99)==0);
	test(select1->Find(11)==1);
	//
	delete select1;
	}


LOCAL_C void TestStreamingL()
	{
	TFileName storename = _L("c:\\T_BASIC_STORE");
	TUid uid={0x000000};

	// create a dictionary files store to test streaming
	theUtils->FileSession().Delete(storename);
	CDictionaryFileStore* store = CDictionaryFileStore::OpenLC(theUtils->FileSession(), storename, uid);
	
	RDictionaryWriteStream write;
	RDictionaryReadStream read;

	TTestMsvEntry entry1,entry2;

	//
	// blank entry
	uid.iUid++;
	write.AssignL(*store, uid);
	write << entry1;
	write.CommitL();
	write.Close();
	//
	entry2.iSize=1;
	test(!(entry1==entry2));
	read.OpenL(*store, uid);
	read >> entry2;
	read.Close();
	test(entry1==entry2);

	//
	// id
	uid.iUid++;
	write.AssignL(*store, uid);
	entry1.SetId(1234);
	write << entry1;
	write.CommitL();
	write.Close();
	//
	test(!(entry1==entry2));
	read.OpenL(*store, uid);
	read >> entry2;
	read.Close();
	test(entry1==entry2);

	//
	// iMtmData3
	uid.iUid++;
	write.AssignL(*store, uid);
	entry1.SetMtmData3(90);
	write << entry1;
	write.CommitL();
	write.Close();
	//
	test(!(entry1==entry2));
	read.OpenL(*store, uid);
	read >> entry2;
	read.Close();
	test(entry1==entry2);

	//
	// rest
	uid.iUid++;
	write.AssignL(*store, uid);
	entry1.SetParent(456);
	entry1.SetPriority(EMsvHighPriority);
	entry1.IncPcSyncCount();
	entry1.iServiceId = 2345;
	entry1.iRelatedId = 5;
	entry1.iType.iUid = 923;
	entry1.iMtm.iUid = 23245;
	entry1.iDate.HomeTime();
	entry1.iSize = -4545;
	entry1.iError = -34;
	entry1.iBioType = 122112;
	entry1.SetMtmData1(111);
	entry1.SetMtmData2(222);
	entry1.SetMtmData3(333);
	entry1.SetReserved(343536);
	write << entry1;
	write.CommitL();
	write.Close();
	//
	test(!(entry1==entry2));
	read.OpenL(*store, uid);
	read >> entry2;
	read.Close();
	test(entry1==entry2);

	// non-persisted flags
	//
	// iMtmData3
	entry1=entry2;
	uid.iUid++;
	write.AssignL(*store, uid);
	entry1.SetConnected(ETrue);
	write << entry1;
	write.CommitL();
	write.Close();
	//
	test(!(entry1==entry2));
	read.OpenL(*store, uid);
	read >> entry2;
	read.Close();
	test(!entry2.Connected());
	test(!(entry1==entry2));
	entry1.SetConnected(EFalse);
	test(entry1==entry2);



	CleanupStack::PopAndDestroy(); // store
	theUtils->FileSession().Delete(storename);
	}


class CTestMsvRecipient : public CMsvRecipient 
	{
public:
	CTestMsvRecipient() {};
	};

LOCAL_C TBool RecipientsEqual(const CMsvRecipient& aR1, const CMsvRecipient& aR2)
	{
	if (aR1.Status()!=aR2.Status() || aR1.Error()!=aR2.Error() || aR1.Time()!=aR2.Time() || aR1.Retries()!=aR2.Retries())
		return EFalse;
	return ETrue;
	}

LOCAL_C void TestRecipientsL()
	{
	CMsvRecipient* rcpt1;
	CMsvRecipient* rcpt2;

	// c'tor
	rcpt1 = new(ELeave) CTestMsvRecipient;
	test(rcpt1->Status()==CMsvRecipient::ENotYetSent);
	test(rcpt1->Error()==KErrNone);
	test(rcpt1->Retries()==0);
	TTime time;
	time.HomeTime();
	test(rcpt1->Time()!=time);

	// status
	rcpt1->SetStatus(CMsvRecipient::ESentSuccessfully);
	test(rcpt1->Status()==CMsvRecipient::ESentSuccessfully);
	rcpt1->SetStatus(CMsvRecipient::EFailedToSend);
	test(rcpt1->Status()==CMsvRecipient::EFailedToSend);
	rcpt1->SetStatus(CMsvRecipient::ESentSuccessfully);
	test(rcpt1->Status()==CMsvRecipient::ESentSuccessfully);

	// error
	rcpt1->SetError(KErrGeneral);
	test(rcpt1->Error()==KErrGeneral);

	// retries
	rcpt1->IncreaseRetries();
	test(rcpt1->Retries()==1);
	rcpt1->IncreaseRetries();
	test(rcpt1->Retries()==2);
	rcpt1->ResetRetries();
	test(rcpt1->Retries()==0);
	rcpt1->IncreaseRetries();
	test(rcpt1->Retries()==1);

	// time
	rcpt1->Time() = time;
	test(rcpt1->Time()==time);

	delete rcpt1;

	//
	// streaming
	TFileName storename = _L("c:\\T_BASIC_STORE");
	TUid uid={0x000000};

	// create a dictionary files store to test streaming
	theUtils->FileSession().Delete(storename);
	CDictionaryFileStore* store = CDictionaryFileStore::OpenLC(theUtils->FileSession(), storename, uid);
	
	RDictionaryWriteStream write;
	RDictionaryReadStream read;

	rcpt1 = new(ELeave) CTestMsvRecipient;
	rcpt2 = new(ELeave) CTestMsvRecipient;

	uid.iUid++;
	write.AssignL(*store, uid);
	write << *rcpt1;
	write.CommitL();
	write.Close();
	//
	rcpt2->SetError(-1);
	test(!RecipientsEqual(*rcpt1,*rcpt2));
	read.OpenL(*store, uid);
	read >> *rcpt2;
	read.Close();
	test(RecipientsEqual(*rcpt1,*rcpt2));

	uid.iUid++;
	rcpt1->SetStatus(CMsvRecipient::ESentSuccessfully);
	rcpt1->SetError(-2);
	rcpt1->IncreaseRetries();
	rcpt1->Time().HomeTime();
	write.AssignL(*store, uid);
	write << *rcpt1;
	write.CommitL();
	write.Close();
	//
	test(!RecipientsEqual(*rcpt1,*rcpt2));
	read.OpenL(*store, uid);
	read >> *rcpt2;
	read.Close();
	test(RecipientsEqual(*rcpt1,*rcpt2));



	delete rcpt1;
	delete rcpt2;
	CleanupStack::PopAndDestroy(); // store
	theUtils->FileSession().Delete(storename);
	}



LOCAL_C void TestScheduledSendingL()
	{
	TMsvEntry entry;

	test(!entry.OffPeak());
	test(!entry.Scheduled());
	test(entry.SendingState() == 0);

	entry.SetOffPeak(ETrue);
	test(entry.OffPeak());
	entry.SetOffPeak(EFalse);
	test(!entry.OffPeak());

	entry.SetScheduled(ETrue);
	test(entry.Scheduled());
	entry.SetScheduled(EFalse);
	test(!entry.Scheduled());

	TUint state;
	for(state = 1; state < 16; state++)
		{
		entry.SetSendingState(state);
		test(entry.SendingState() == state);
		}

	entry.SetSendingState(0);
	test(entry.SendingState() == 0);
	}

/**
This is testing the changes made in response to the PREQ694 "Automatic sending
of SMS on network reconnection".  It added two new exports PendingConditions/SetPendingConditions

Things we need to test here:
1. Test Set and Get, to see if the values are match
2. Test the boundary condition, PendingConditions bit is adajacent with connected bit 
	and the sending state bit.  Make sure SetPendingConditions does not mis-set the 
	adajacent bits.
*/	
LOCAL_C void TestPendingConditions()
	{
	TMsvEntry entry;
	entry.SetConnected(ETrue);
	entry.SetSendingState(KMsvSendStateSent);
	entry.SetPendingConditions(EFalse);	
	test(entry.Connected() &&
		entry.SendingState() == KMsvSendStateSent &&
		!entry.PendingConditions());
	
	entry.SetConnected(EFalse);
	entry.SetSendingState(KMsvSendStateUnknown);
	entry.SetPendingConditions(ETrue);
	test(!entry.Connected() &&
		entry.SendingState() == KMsvSendStateUnknown &&
		entry.PendingConditions());	
	}	


	
LOCAL_C void doTestsL()
	{	
	CMsgsTestUtils::Start(_L("Flags"));
	TestPrioritiesAndFlagsL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Copying"));
	TestCopyingL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Packing"));
	TestPackingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Utility functions"));
	TestUtilities1L();
	TestUtilities2L();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Ordering"));
	TestOrderingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Selections"));
	TestSelectionsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Streaming"));
	TestStreamingL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Recipients"));
	TestRecipientsL();
	CMsgsTestUtils::Complete();


	CMsgsTestUtils::Next(_L("Scheduled Sending"));
	TestScheduledSendingL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Pending Conditions"));
	TestPendingConditions();
	CMsgsTestUtils::Complete();
	}
