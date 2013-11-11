// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <badesca.h>
#include "Testsetup.h"
#include <msvarray.h>
#include <msvuids.h>

#if !defined(_DEBUG)
#pragma warning( disable : 4710 ) // class TMsvEntry const * &  __thiscall CArrayFix<class TMsvEntry const *>::At(int)' not expanded
#endif

RTest	test(_L("MSV Sorting Testrig"));
LOCAL_D TInt id;
LOCAL_D CDesCArray* theBufferArray;
LOCAL_D CArrayFixFlat<TUid>* theMtmList;

const TUid uid1={0x0};
const TUid uid2={0x234};
const TUid uid3={0x122334};
const TUid uid4={0x12244546};
const TUid uid5={0x12244547};
const TUid uid6={0x12244548};

enum TSpeedTestSort 
	{ 
	ENoSorting,
	ESortingByDescriptionReverseNoGrouping, 
	ESortingByDescriptionReverseAllGrouping,
	ESortingByDescriptionNoGrouping, 
	ESortingByDescriptionAllGrouping,
	ESortingByDescriptionGroupingByPriority,
	ESortingByDescriptionGroupingByMessageType,
	};

//

LOCAL_C TMsvId AddEntryL(CMsvEntryArray* aArray, const TDesC& aDetails, const TDesC& aDescription, const TUid aType,
						 TUid aMtm, TMsvPriority aPriority,TTime aDate,TInt aSize, TBool aStandardFolder)
	{
	theBufferArray->AppendL(aDetails);
	theBufferArray->AppendL(aDescription);
	TTestMsvEntry* newEntry = new (ELeave) TTestMsvEntry;
	newEntry->SetId(id++);
	newEntry->SetStandardFolder(aStandardFolder);
	newEntry->iType=aType;
	newEntry->iMtm=aMtm;
	newEntry->iDetails.Set((*theBufferArray)[theBufferArray->Count()-2]);
	newEntry->iDescription.Set((*theBufferArray)[theBufferArray->Count()-1]);
	newEntry->SetComplete(ETrue);
	newEntry->iDate=aDate;
	newEntry->iSize=aSize;
	newEntry->SetPriority(aPriority);
	aArray->AppendL(newEntry);	
	return newEntry->Id();
	}

LOCAL_C TMsvId AddEntryL(CMsvEntryArray* aArray, const TDesC& aDetails, const TDesC& aDescription, const TUid aType,
						 TUid aMtm, TMsvPriority aPriority,TTime aDate,TInt aSize)
	{
	return AddEntryL(aArray, aDetails, aDescription,aType, aMtm, aPriority, aDate, aSize, EFalse);
	}

LOCAL_C TMsvId AddEntryL(CMsvEntryArray* aArray, const TDesC& aDetails, const TUid aType,
					   TMsvPriority aPriority,TTime aDate,TInt aSize)
	{
	return AddEntryL(aArray, aDetails, TPtrC(), aType, TUid(), aPriority, aDate, aSize);
	}

LOCAL_C void TestSortL()
	{
	theMtmList->Reset();
	theMtmList->AppendL(uid1);
	theMtmList->AppendL(uid2);
	theMtmList->AppendL(uid3);
	theMtmList->AppendL(uid4);
	
	TMsvSelectionOrdering ordering;
	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(*theMtmList);

	// sorting nothing
	entryArray->SortL(ordering);
	ordering.SetGroupByType(ETrue);
	entryArray->SortL(ordering);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(ETrue);
	entryArray->SortL(ordering);
	ordering.SetGroupByMtm(EFalse);
	ordering.SetSorting(EMsvSortBySize);
	entryArray->SortL(ordering);


	TMsvId idA=AddEntryL(entryArray,_L("Inbox"),_L(""),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0);
	TMsvId idB=AddEntryL(entryArray,_L("Outbox"),_L("ff"),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0);
	TMsvId idC=AddEntryL(entryArray,_L("Drafts"),_L(""),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0);
	TMsvId idD=AddEntryL(entryArray,_L("Sent"),_L(""),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	
	TMsvId id1=AddEntryL(entryArray,_L("nothing"),_L("once"),KUidMsvAttachmentEntry,uid2,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),1);
	TMsvId id2=AddEntryL(entryArray,_L("abc"),_L("upon"),KUidMsvAttachmentEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),87);
	TMsvId id3=AddEntryL(entryArray,_L("abcdefgh"),_L("a"),KUidMsvAttachmentEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),900);
	TMsvId id4=AddEntryL(entryArray,_L("qwerty"),_L("time"),KUidMsvAttachmentEntry,uid1,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),7);
	TMsvId id5=AddEntryL(entryArray,_L("Michelle"),_L("there"),KUidMsvAttachmentEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),34);

	TMsvId id6=AddEntryL(entryArray,_L("thing"),_L("was"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),88);
	TMsvId id7=AddEntryL(entryArray,_L("wibble"),_L("a"),KUidMsvMessageEntry,uid2,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),100);
	TMsvId id8=AddEntryL(entryArray,_L("astronaut"),_L("young"),KUidMsvMessageEntry,uid3,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),110);
	TMsvId id9=AddEntryL(entryArray,_L("bludgeon"),_L("billy"),KUidMsvMessageEntry,uid4,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),100);
	TMsvId id10=AddEntryL(entryArray,_L("Natascha"),_L("goat"),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),100);

	test.Printf(_L("Sorting with nothing set\n"));
	ordering = TMsvSelectionOrdering();
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->iDetails==_L("Inbox"));
	test(entryArray->At(1)->iDetails==_L("Outbox"));
	test(entryArray->At(2)->iDetails==_L("Drafts"));
	test(entryArray->At(3)->iDetails==_L("Sent"));
	test(entryArray->At(4)->iDetails==_L("nothing"));
	test(entryArray->At(5)->iDetails==_L("abc"));
	test(entryArray->At(6)->iDetails==_L("abcdefgh"));
	test(entryArray->At(7)->iDetails==_L("qwerty"));
	test(entryArray->At(8)->iDetails==_L("Michelle"));
	test(entryArray->At(9)->iDetails==_L("thing"));
	test(entryArray->At(10)->iDetails==_L("wibble"));
	test(entryArray->At(11)->iDetails==_L("astronaut"));
	test(entryArray->At(12)->iDetails==_L("bludgeon"));
	test(entryArray->At(13)->iDetails==_L("Natascha"));

	test.Printf(_L("Sorting by details, no grouping\n"));
	ordering.SetSorting(EMsvSortByDetails);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->iDetails==_L("abc"));
	test(entryArray->At(1)->iDetails==_L("abcdefgh"));
	test(entryArray->At(2)->iDetails==_L("astronaut"));
	test(entryArray->At(3)->iDetails==_L("bludgeon"));
	test(entryArray->At(4)->iDetails==_L("Drafts"));
	test(entryArray->At(5)->iDetails==_L("Inbox"));
	test(entryArray->At(6)->iDetails==_L("Michelle"));
	test(entryArray->At(7)->iDetails==_L("Natascha"));
	test(entryArray->At(8)->iDetails==_L("nothing"));
	test(entryArray->At(9)->iDetails==_L("Outbox"));
	test(entryArray->At(10)->iDetails==_L("qwerty"));
	test(entryArray->At(11)->iDetails==_L("Sent"));
	test(entryArray->At(12)->iDetails==_L("thing"));
	test(entryArray->At(13)->iDetails==_L("wibble"));



	test.Printf(_L("Sorting by details reverse, no grouping\n"));
	ordering.SetSorting(EMsvSortByDetailsReverse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(13)->iDetails==_L("abc"));
	test(entryArray->At(12)->iDetails==_L("abcdefgh"));
	test(entryArray->At(11)->iDetails==_L("astronaut"));
	test(entryArray->At(10)->iDetails==_L("bludgeon"));
	test(entryArray->At(9)->iDetails==_L("Drafts"));
	test(entryArray->At(8)->iDetails==_L("Inbox"));
	test(entryArray->At(7)->iDetails==_L("Michelle"));
	test(entryArray->At(6)->iDetails==_L("Natascha"));
	test(entryArray->At(5)->iDetails==_L("nothing"));
	test(entryArray->At(4)->iDetails==_L("Outbox"));
	test(entryArray->At(3)->iDetails==_L("qwerty"));
	test(entryArray->At(2)->iDetails==_L("Sent"));
	test(entryArray->At(1)->iDetails==_L("thing"));
	test(entryArray->At(0)->iDetails==_L("wibble"));



	test.Printf(_L("Sorting by details, grouping by entry type\n"));
	ordering.SetSorting(EMsvSortByDetails);
	ordering.SetGroupByType(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->iDetails==_L("Drafts"));
	test(entryArray->At(1)->iDetails==_L("Inbox"));
	test(entryArray->At(2)->iDetails==_L("Outbox"));
	test(entryArray->At(3)->iDetails==_L("Sent"));

	test(entryArray->At(4)->iDetails==_L("astronaut"));
	test(entryArray->At(5)->iDetails==_L("bludgeon"));
	test(entryArray->At(6)->iDetails==_L("Natascha"));
	test(entryArray->At(7)->iDetails==_L("thing"));
	test(entryArray->At(8)->iDetails==_L("wibble"));

	test(entryArray->At(9)->iDetails==_L("abc"));
	test(entryArray->At(10)->iDetails==_L("abcdefgh"));
	test(entryArray->At(11)->iDetails==_L("Michelle"));
	test(entryArray->At(12)->iDetails==_L("nothing"));
	test(entryArray->At(13)->iDetails==_L("qwerty"));
	

	test.Printf(_L("Sorting by details reverse, grouping by entry type\n"));
	ordering.SetSorting(EMsvSortByDetailsReverse);
	ordering.SetGroupByType(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->iDetails==_L("Drafts"));
	test(entryArray->At(1)->iDetails==_L("Inbox"));
	test(entryArray->At(2)->iDetails==_L("Outbox"));
	test(entryArray->At(3)->iDetails==_L("Sent"));

	test(entryArray->At(8)->iDetails==_L("astronaut"));
	test(entryArray->At(7)->iDetails==_L("bludgeon"));
	test(entryArray->At(6)->iDetails==_L("Natascha"));
	test(entryArray->At(5)->iDetails==_L("thing"));
	test(entryArray->At(4)->iDetails==_L("wibble"));

	test(entryArray->At(13)->iDetails==_L("abc"));
	test(entryArray->At(12)->iDetails==_L("abcdefgh"));
	test(entryArray->At(11)->iDetails==_L("Michelle"));
	test(entryArray->At(10)->iDetails==_L("nothing"));
	test(entryArray->At(9)->iDetails==_L("qwerty"));




	test.Printf(_L("Sorting by details, grouping by entry type & priority\n"));
	ordering.SetSorting(EMsvSortByDetails);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->iDetails==_L("Drafts"));
	test(entryArray->At(1)->iDetails==_L("Inbox"));
	test(entryArray->At(2)->iDetails==_L("Outbox"));
	test(entryArray->At(3)->iDetails==_L("Sent"));

	test(entryArray->At(4)->iDetails==_L("wibble"));
	test(entryArray->At(5)->iDetails==_L("Natascha"));
	test(entryArray->At(6)->iDetails==_L("thing"));
	test(entryArray->At(7)->iDetails==_L("astronaut"));
	test(entryArray->At(8)->iDetails==_L("bludgeon"));

	test(entryArray->At(9)->iDetails==_L("nothing"));
	test(entryArray->At(10)->iDetails==_L("qwerty"));
	test(entryArray->At(11)->iDetails==_L("abc"));
	test(entryArray->At(12)->iDetails==_L("abcdefgh"));
	test(entryArray->At(13)->iDetails==_L("Michelle"));




	test.Printf(_L("Sorting by details reverse, grouping by entry type & priority\n"));
	ordering.SetSorting(EMsvSortByDetailsReverse);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->iDetails==_L("Drafts"));
	test(entryArray->At(1)->iDetails==_L("Inbox"));
	test(entryArray->At(2)->iDetails==_L("Outbox"));
	test(entryArray->At(3)->iDetails==_L("Sent"));

	test(entryArray->At(4)->iDetails==_L("wibble"));
	test(entryArray->At(5)->iDetails==_L("thing"));
	test(entryArray->At(6)->iDetails==_L("Natascha"));
	test(entryArray->At(7)->iDetails==_L("bludgeon"));
	test(entryArray->At(8)->iDetails==_L("astronaut"));

	test(entryArray->At(9)->iDetails==_L("qwerty"));
	test(entryArray->At(10)->iDetails==_L("nothing"));
	test(entryArray->At(11)->iDetails==_L("Michelle"));
	test(entryArray->At(12)->iDetails==_L("abcdefgh"));
	test(entryArray->At(13)->iDetails==_L("abc"));




	test.Printf(_L("Sorting by details, grouping by priority\n"));
	ordering.SetSorting(EMsvSortByDetails);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->iDetails==_L("nothing"));
	test(entryArray->At(1)->iDetails==_L("qwerty"));
	test(entryArray->At(2)->iDetails==_L("wibble"));

	test(entryArray->At(3)->iDetails==_L("abc"));
	test(entryArray->At(4)->iDetails==_L("abcdefgh"));
	test(entryArray->At(5)->iDetails==_L("Drafts"));
	test(entryArray->At(6)->iDetails==_L("Inbox"));
	test(entryArray->At(7)->iDetails==_L("Michelle"));
	test(entryArray->At(8)->iDetails==_L("Natascha"));
	test(entryArray->At(9)->iDetails==_L("Outbox"));
	test(entryArray->At(10)->iDetails==_L("Sent"));
	test(entryArray->At(11)->iDetails==_L("thing"));

	test(entryArray->At(12)->iDetails==_L("astronaut"));
	test(entryArray->At(13)->iDetails==_L("bludgeon"));




	test.Printf(_L("Sorting by details reverse, grouping by priority\n"));
	ordering.SetSorting(EMsvSortByDetailsReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(2)->iDetails==_L("nothing"));
	test(entryArray->At(1)->iDetails==_L("qwerty"));
	test(entryArray->At(0)->iDetails==_L("wibble"));

	test(entryArray->At(11)->iDetails==_L("abc"));
	test(entryArray->At(10)->iDetails==_L("abcdefgh"));
	test(entryArray->At(9)->iDetails==_L("Drafts"));
	test(entryArray->At(8)->iDetails==_L("Inbox"));
	test(entryArray->At(7)->iDetails==_L("Michelle"));
	test(entryArray->At(6)->iDetails==_L("Natascha"));
	test(entryArray->At(5)->iDetails==_L("Outbox"));
	test(entryArray->At(4)->iDetails==_L("Sent"));
	test(entryArray->At(3)->iDetails==_L("thing"));

	test(entryArray->At(13)->iDetails==_L("astronaut"));
	test(entryArray->At(12)->iDetails==_L("bludgeon"));


	
	test.Printf(_L("Sorting by description, with grouping\n"));
	ordering.SetSorting(EMsvSortByDescription);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
//	test(entryArray->At(0)->iDetails==_L("Inbox"));
//	test(entryArray->At(1)->iDetails==_L("Outbox"));
//	test(entryArray->At(2)->iDetails==_L("Drafts"));
//	test(entryArray->At(3)->iDetails==_L("Sent"));

	test(entryArray->At(4)->iDescription==_L("a"));
	test(entryArray->At(5)->iDescription==_L("billy"));
	test(entryArray->At(6)->iDescription==_L("goat"));
	test(entryArray->At(7)->iDescription==_L("was"));
	test(entryArray->At(8)->iDescription==_L("young"));

	test(entryArray->At(9)->iDescription==_L("a"));
	test(entryArray->At(10)->iDescription==_L("once"));
	test(entryArray->At(11)->iDescription==_L("there"));
	test(entryArray->At(12)->iDescription==_L("time"));
	test(entryArray->At(13)->iDescription==_L("upon"));



	test.Printf(_L("Sorting by description reverse, no grouping\n"));
	ordering.SetSorting(EMsvSortByDescriptionReverse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
//	test(entryArray->At(3)->iDetails==_L("Inbox"));
//	test(entryArray->At(2)->iDetails==_L("Outbox"));
//	test(entryArray->At(1)->iDetails==_L("Drafts"));
//	test(entryArray->At(0)->iDetails==_L("Sent"));

	test(entryArray->At(8)->iDescription==_L("a"));
	test(entryArray->At(7)->iDescription==_L("billy"));
	test(entryArray->At(6)->iDescription==_L("goat"));
	test(entryArray->At(5)->iDescription==_L("was"));
	test(entryArray->At(4)->iDescription==_L("young"));

	test(entryArray->At(13)->iDescription==_L("a"));
	test(entryArray->At(12)->iDescription==_L("once"));
	test(entryArray->At(11)->iDescription==_L("there"));
	test(entryArray->At(10)->iDescription==_L("time"));
	test(entryArray->At(9)->iDescription==_L("upon"));



	
	test.Printf(_L("Sorting by date, no grouping\n"));
	ordering.SetSorting(EMsvSortByDate);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==idA);
	test(entryArray->At(1)->Id()==idB);
	test(entryArray->At(2)->Id()==idC);
	test(entryArray->At(3)->Id()==idD);
	test(entryArray->At(4)->Id()==id1);
	test(entryArray->At(5)->Id()==id6);
	test(entryArray->At(6)->Id()==id8);
	test(entryArray->At(7)->Id()==id4);
	test(entryArray->At(8)->Id()==id7);
	test(entryArray->At(9)->Id()==id5);
	test(entryArray->At(10)->Id()==id2);
	test(entryArray->At(11)->Id()==id10);
	test(entryArray->At(12)->Id()==id9);
	test(entryArray->At(13)->Id()==id3);



	test.Printf(_L("Sorting by date reverse, no grouping\n"));
	ordering.SetSorting(EMsvSortByDateReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(13)->Id()==idA);
	test(entryArray->At(12)->Id()==idB);
	test(entryArray->At(11)->Id()==idC);
	test(entryArray->At(10)->Id()==idD);
	test(entryArray->At(0)->Id()==id3);
	test(entryArray->At(1)->Id()==id9);
	test(entryArray->At(2)->Id()==id10);
	test(entryArray->At(3)->Id()==id2);
	test(entryArray->At(4)->Id()==id5);
	test(entryArray->At(5)->Id()==id7);
	test(entryArray->At(6)->Id()==id4);
	test(entryArray->At(7)->Id()==id8);
	test(entryArray->At(8)->Id()==id6);
	test(entryArray->At(9)->Id()==id1);



	test.Printf(_L("Sorting by date, grouping by entry type\n"));
	ordering.SetSorting(EMsvSortByDate);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);

	test(entryArray->At(4)->Id()==id6);
	test(entryArray->At(5)->Id()==id8);
	test(entryArray->At(6)->Id()==id7);
	test(entryArray->At(7)->Id()==id10);
	test(entryArray->At(8)->Id()==id9);

	test(entryArray->At(9)->Id()==id1);
	test(entryArray->At(10)->Id()==id4);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(12)->Id()==id2);
	test(entryArray->At(13)->Id()==id3);



	test.Printf(_L("Sorting by date reverse, grouping by entry type\n"));
	ordering.SetSorting(EMsvSortByDateReverse);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);

	test(entryArray->At(8)->Id()==id6);
	test(entryArray->At(7)->Id()==id8);
	test(entryArray->At(6)->Id()==id7);
	test(entryArray->At(5)->Id()==id10);
	test(entryArray->At(4)->Id()==id9);

	test(entryArray->At(13)->Id()==id1);
	test(entryArray->At(12)->Id()==id4);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(10)->Id()==id2);
	test(entryArray->At(9)->Id()==id3);



	test.Printf(_L("Sorting by date, grouping by priority\n"));
	ordering.SetSorting(EMsvSortByDate);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==id1); // high
	test(entryArray->At(1)->Id()==id4);
	test(entryArray->At(2)->Id()==id7);

	test(entryArray->At(3)->Id()==idA); // medium // CHANGE while fail sometime after 1999
	test(entryArray->At(4)->Id()==idB);
	test(entryArray->At(5)->Id()==idC);
	test(entryArray->At(6)->Id()==idD);
	test(entryArray->At(7)->Id()==id6); 
	test(entryArray->At(8)->Id()==id5);
	test(entryArray->At(9)->Id()==id2);
	test(entryArray->At(10)->Id()==id10);
	test(entryArray->At(11)->Id()==id3);

	test(entryArray->At(12)->Id()==id8); // low
	test(entryArray->At(13)->Id()==id9);



	test.Printf(_L("Sorting by date reverse, grouping by priority\n"));
	ordering.SetSorting(EMsvSortByDateReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==id7); // high
	test(entryArray->At(1)->Id()==id4);
	test(entryArray->At(2)->Id()==id1);

	test(entryArray->At(11)->Id()==idA); // medium // CHANGE while fail sometime after 1999
	test(entryArray->At(10)->Id()==idB);
	test(entryArray->At(9)->Id()==idC);
	test(entryArray->At(8)->Id()==idD);
	test(entryArray->At(7)->Id()==id6); 
	test(entryArray->At(6)->Id()==id5);
	test(entryArray->At(5)->Id()==id2);
	test(entryArray->At(4)->Id()==id10);
	test(entryArray->At(3)->Id()==id3);

	test(entryArray->At(13)->Id()==id8); // low
	test(entryArray->At(12)->Id()==id9);



	test.Printf(_L("Sorting by date, grouping by entry type & priority\n"));
	ordering.SetSorting(EMsvSortByDate);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);

	test(entryArray->At(4)->Id()==id7);
	test(entryArray->At(5)->Id()==id6);
	test(entryArray->At(6)->Id()==id10);
	test(entryArray->At(7)->Id()==id8);
	test(entryArray->At(8)->Id()==id9);

	test(entryArray->At(9)->Id()==id1);
	test(entryArray->At(10)->Id()==id4);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(12)->Id()==id2);
	test(entryArray->At(13)->Id()==id3);



	test.Printf(_L("Sorting by date reverse, grouping by entry type & priority\n"));
	ordering.SetSorting(EMsvSortByDateReverse);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);

	test(entryArray->At(4)->Id()==id7);
	test(entryArray->At(5)->Id()==id10);
	test(entryArray->At(6)->Id()==id6);
	test(entryArray->At(7)->Id()==id9);
	test(entryArray->At(8)->Id()==id8);

	test(entryArray->At(9)->Id()==id4);
	test(entryArray->At(10)->Id()==id1);
	test(entryArray->At(11)->Id()==id3);
	test(entryArray->At(12)->Id()==id2);
	test(entryArray->At(13)->Id()==id5);



	TMsvId xx0,xx1,xx2,xx3,xx4,xx5,xx6,xx7,/*xx8,*/xx9,xx10,xx11,xx12,xx13;

	test.Printf(_L("Sorting by size, no grouping\n"));
	ordering.SetSorting(EMsvSortBySize);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	xx0=entryArray->At(0)->Id();
	xx1=entryArray->At(1)->Id();
	xx2=entryArray->At(2)->Id();
	xx3=entryArray->At(3)->Id();
	test (xx0!=xx1 && xx0!=xx2 && xx0!=xx3 && xx1!=xx2 && xx1!=xx3 && xx2!=xx3);
	test(xx0==idA || xx0==idB || xx0==idC || xx0==idD);
	test(xx1==idA || xx1==idB || xx1==idC || xx1==idD);
	test(xx2==idA || xx2==idB || xx2==idC || xx2==idD);
	test(xx3==idA || xx3==idB || xx3==idC || xx3==idD);
	test(entryArray->At(4)->Id()==id1);
	test(entryArray->At(5)->Id()==id4);
	test(entryArray->At(6)->Id()==id5);
	test(entryArray->At(7)->Id()==id2);
	test(entryArray->At(8)->Id()==id6);
	xx9=entryArray->At(9)->Id();  // these 3 all same size so order could be anything
	xx10=entryArray->At(11)->Id(); 
	xx11=entryArray->At(10)->Id();
	test (xx9!=xx10 && xx9!=xx11 && xx10!=xx11);
	test (xx9==id7 || xx9==id9 || xx9==id10);
	test (xx10==id7 || xx10==id9 || xx10==id10);
	test (xx11==id7 || xx11==id9 || xx11==id10);
	test(entryArray->At(12)->Id()==id8);
	test(entryArray->At(13)->Id()==id3);



	test.Printf(_L("Sorting by size reverse, no grouping\n"));
	ordering.SetSorting(EMsvSortBySizeReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);
	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	xx13=entryArray->At(13)->Id();
	xx12=entryArray->At(12)->Id();
	xx11=entryArray->At(11)->Id();
	xx10=entryArray->At(10)->Id();
	test (xx10!=xx11 && xx10!=xx12 && xx10!=xx13 && xx11!=xx12 && xx11!=xx13 && xx12!=xx13);
	test(xx10==idA || xx10==idB || 
			xx10==idC || xx10==idD);
	test(xx11==idA || xx11==idB || 
			xx11==idC || xx11==idD);
	test(xx12==idA || xx12==idB || 
			xx12==idC || xx12==idD);
	test(xx13==idA || xx13==idB || 
			xx13==idC || xx13==idD);
	test(entryArray->At(9)->Id()==id1);
	test(entryArray->At(8)->Id()==id4);
	test(entryArray->At(7)->Id()==id5);
	test(entryArray->At(6)->Id()==id2);
	test(entryArray->At(5)->Id()==id6);
	xx2=entryArray->At(2)->Id();  // these 3 all same size so order could be anything
	xx3=entryArray->At(3)->Id(); 
	xx4=entryArray->At(4)->Id();
	test (xx2!=xx3 && xx2!=xx4 && xx3!=xx4);
	test (xx2==id7 || xx2==id9 || xx2==id10);
	test (xx3==id7 || xx3==id9 || xx3==id10);
	test (xx4==id7 || xx4==id9 || xx4==id10);
	test(entryArray->At(1)->Id()==id8);
	test(entryArray->At(0)->Id()==id3);



	test.Printf(_L("Sorting by size, grouping by type\n"));
	ordering.SetSorting(EMsvSortBySize);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
/*
	xx0=entryArray->At(0)->Id();
	xx1=entryArray->At(1)->Id();
	xx2=entryArray->At(2)->Id();
	xx3=entryArray->At(3)->Id();
	test (xx0!=xx1 && xx0!=xx2 && xx0!=xx3 && xx1!=xx2 && xx1!=xx3 && xx2!=xx3);
	test(xx0==idA || xx0==idB || 
			xx0==idC || xx0==idD);
	test(xx1==idA || xx1==idB || 
			xx1==idC || xx1==idD);
	test(xx2==idA || xx2==idB || 
			xx2==idC || xx2==idD);
	test(xx3==idA || xx3==idB || 
			xx3==idC || xx3==idD);
*/
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);
	test(entryArray->At(4)->Id()==id6);
	xx5=entryArray->At(5)->Id();  // these 3 all same size so order could be anything
	xx6=entryArray->At(6)->Id(); 
	xx7=entryArray->At(7)->Id();
	test (xx5!=xx6 && xx5!=xx7 && xx6!=xx7); // check they're all different
	test (xx5==id7 || xx5==id9 || xx5==id10); // in any order
	test (xx6==id7 || xx6==id9 || xx6==id10);
	test (xx7==id7 || xx7==id9 || xx7==id10);
	test(entryArray->At(8)->Id()==id8);
	test(entryArray->At(9)->Id()==id1);
	test(entryArray->At(10)->Id()==id4);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(12)->Id()==id2);
	test(entryArray->At(13)->Id()==id3);



	test.Printf(_L("Sorting by size reverse, grouping by type\n"));
	ordering.SetSorting(EMsvSortBySizeReverse);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
/*	xx0=entryArray->At(0)->Id();
	xx1=entryArray->At(1)->Id();
	xx2=entryArray->At(2)->Id();
	xx3=entryArray->At(3)->Id();
	test (xx0!=xx1 && xx0!=xx2 && xx0!=xx3 && xx1!=xx2 && xx1!=xx3 && xx2!=xx3);
	test(xx0==idA || xx0==idB || 
			xx0==idC || xx0==idD);
	test(xx1==idA || xx1==idB || 
			xx1==idC || xx1==idD);
	test(xx2==idA || xx2==idB || 
			xx2==idC || xx2==idD);
	test(xx3==idA || xx3==idB || 
			xx3==idC || xx3==idD);
*/	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);
	
	test(entryArray->At(4)->Id()==id8);
	xx5=entryArray->At(5)->Id();  // these 3 all same size so order could be anything
	xx6=entryArray->At(6)->Id(); 
	xx7=entryArray->At(7)->Id();
	test (xx5!=xx6 && xx5!=xx7 && xx6!=xx7); // check they're all different
	test (xx5==id7 || xx5==id9 || xx5==id10); // in any order
	test (xx6==id7 || xx6==id9 || xx6==id10);
	test (xx7==id7 || xx7==id9 || xx7==id10);
	test(entryArray->At(8)->Id()==id6);
	test(entryArray->At(9)->Id()==id3);
	test(entryArray->At(10)->Id()==id2);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(12)->Id()==id4);
	test(entryArray->At(13)->Id()==id1);



	test.Printf(_L("Sorting by size, grouping by priority\n"));
	ordering.SetSorting(EMsvSortBySize);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==id1);	// high
	test(entryArray->At(1)->Id()==id4);
	test(entryArray->At(2)->Id()==id7);
/* CHANGE
	test(entryArray->At(3)->Id()==id5);  // medium
	xx4=entryArray->At(4)->Id();
	xx5=entryArray->At(5)->Id();
	test(xx4!=xx5);
	test((xx4==id2 || xx4==id6) && (xx5==id2 || xx5==id6)); 
	test(entryArray->At(6)->Id()==id10);
	test(entryArray->At(7)->Id()==id3);

	xx8=entryArray->At(8)->Id(); // low
	xx9=entryArray->At(9)->Id();
	xx10=entryArray->At(10)->Id();
	xx11=entryArray->At(11)->Id();
	test (xx8!=xx9 && xx8!=xx10 && xx8!=xx11 && xx9!=xx10 && xx9!=xx11 && xx10!=xx11);
	test(xx8==idA || xx8==idB || 
			xx8==idC || xx8==idD);
	test(xx9==idA || xx9==idB || 
			xx9==idC || xx9==idD);
	test(xx10==idA || xx10==idB || 
			xx10==idC || xx10==idD);
	test(xx11==idA || xx11==idB || 
			xx11==idC || xx11==idD);
	test(entryArray->At(12)->Id()==id9);
	test(entryArray->At(13)->Id()==id8);
*/



	test.Printf(_L("Sorting by size reverse, grouping by priority\n"));
	ordering.SetSorting(EMsvSortBySizeReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
	test(entryArray->At(0)->Id()==id7);	// high
	test(entryArray->At(1)->Id()==id4);
	test(entryArray->At(2)->Id()==id1);
/* CHANGE
	test(entryArray->At(3)->Id()==id3);
	test(entryArray->At(4)->Id()==id10); // medium
	xx5=entryArray->At(5)->Id();
	xx6=entryArray->At(6)->Id();
	test(xx5!=xx6);
	test((xx5==id2 || xx5==id6) && (xx6==id2 || xx6==id6)); 
	test(entryArray->At(7)->Id()==id5);

	test(entryArray->At(8)->Id()==id8);
	test(entryArray->At(9)->Id()==id9);
	xx10=entryArray->At(10)->Id(); // low
	xx11=entryArray->At(11)->Id();
	xx12=entryArray->At(12)->Id();
	xx13=entryArray->At(13)->Id();
	test (xx10!=xx11 && xx10!=xx12 && xx10!=xx13 && xx11!=xx12 && xx11!=xx13 && xx12!=xx13);
	test(xx10==idA || xx10==idB || 
			xx10==idC || xx10==idD);
	test(xx11==idA || xx11==idB || 
			xx11==idC || xx11==idD);
	test(xx12==idA || xx12==idB || 
			xx12==idC || xx12==idD);
	test(xx13==idA || xx13==idB || 
			xx13==idC || xx13==idD);
*/



	test.Printf(_L("Sorting by size, grouping by type and priority\n"));
	ordering.SetSorting(EMsvSortBySize);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
/* CHANGE
	xx0=entryArray->At(0)->Id();
	xx1=entryArray->At(1)->Id();
	xx2=entryArray->At(2)->Id();
	xx3=entryArray->At(3)->Id();
	test (xx0!=xx1 && xx0!=xx2 && xx0!=xx3 && xx1!=xx2 && xx1!=xx3 && xx2!=xx3);
	test(xx0==idA || xx0==idB || 
			xx0==idC || xx0==idD);
	test(xx1==idA || xx1==idB || 
			xx1==idC || xx1==idD);
	test(xx2==idA || xx2==idB || 
			xx2==idC || xx2==idD);
	test(xx3==idA || xx3==idB || 
			xx3==idC || xx3==idD);
*/
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);

	test(entryArray->At(4)->Id()==id7); // messages
	test(entryArray->At(5)->Id()==id6);
	test(entryArray->At(6)->Id()==id10);
	test(entryArray->At(7)->Id()==id9);
	test(entryArray->At(8)->Id()==id8);

	test(entryArray->At(9)->Id()==id1); // attachments
	test(entryArray->At(10)->Id()==id4);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(12)->Id()==id2);
	test(entryArray->At(13)->Id()==id3);



	test.Printf(_L("Sorting by size reverse, grouping by type and priority\n"));
	ordering.SetSorting(EMsvSortBySizeReverse);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); // 10 entries added + 4 local folders created on start up
/* CHANGE
	xx0=entryArray->At(0)->Id();
	xx1=entryArray->At(1)->Id();
	xx2=entryArray->At(2)->Id();
	xx3=entryArray->At(3)->Id();
	test (xx0!=xx1 && xx0!=xx2 && xx0!=xx3 && xx1!=xx2 && xx1!=xx3 && xx2!=xx3);
	test(xx0==idA || xx0==idB || 
			xx0==idC || xx0==idD);
	test(xx1==idA || xx1==idB || 
			xx1==idC || xx1==idD);
	test(xx2==idA || xx2==idB || 
			xx2==idC || xx2==idD);
	test(xx3==idA || xx3==idB || 
			xx3==idC || xx3==idD);
*/
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);

	test(entryArray->At(4)->Id()==id7); // messages
	test(entryArray->At(5)->Id()==id10);
	test(entryArray->At(6)->Id()==id6);
	test(entryArray->At(7)->Id()==id8);
	test(entryArray->At(8)->Id()==id9);

	test(entryArray->At(9)->Id()==id4); // attachments
	test(entryArray->At(10)->Id()==id1);
	test(entryArray->At(11)->Id()==id3);
	test(entryArray->At(12)->Id()==id2);
	test(entryArray->At(13)->Id()==id5);



	test.Printf(_L("Sorting by date reverse, grouping by MTM\n"));
	ordering.SetSorting(EMsvSortByDateReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); 
	test(entryArray->At(6)->Id()==idA);
	test(entryArray->At(5)->Id()==idB);
	test(entryArray->At(4)->Id()==idC);
	test(entryArray->At(3)->Id()==idD);
	test(entryArray->At(2)->Id()==id6);
	test(entryArray->At(1)->Id()==id4);
	test(entryArray->At(0)->Id()==id3);
	//
	test(entryArray->At(9)->Id()==id1);
	test(entryArray->At(8)->Id()==id7);
	test(entryArray->At(7)->Id()==id2);
	//
	test(entryArray->At(12)->Id()==id8);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(10)->Id()==id10);
	//
	test(entryArray->At(13)->Id()==id9);


	test.Printf(_L("Sorting by date, grouping by MTM\n"));
	ordering.SetSorting(EMsvSortByDate);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); 
	test(entryArray->At(0)->Id()==idA);
	test(entryArray->At(1)->Id()==idB);
	test(entryArray->At(2)->Id()==idC);
	test(entryArray->At(3)->Id()==idD);
	test(entryArray->At(4)->Id()==id6);
	test(entryArray->At(5)->Id()==id4);
	test(entryArray->At(6)->Id()==id3);
	//
	test(entryArray->At(7)->Id()==id1);
	test(entryArray->At(8)->Id()==id7);
	test(entryArray->At(9)->Id()==id2);
	//
	test(entryArray->At(10)->Id()==id8);
	test(entryArray->At(11)->Id()==id5);
	test(entryArray->At(12)->Id()==id10);
	//
	test(entryArray->At(13)->Id()==id9);


	test.Printf(_L("Sorting by date, grouping by Type and MTM\n"));
	ordering.SetSorting(EMsvSortByDate);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); 
	// Folders
	test(entryArray->At(0)->Id()==idC);
	test(entryArray->At(1)->Id()==idA);
	test(entryArray->At(2)->Id()==idB);
	test(entryArray->At(3)->Id()==idD);
	// Messages
	test(entryArray->At(4)->Id()==id6);
	//
	test(entryArray->At(5)->Id()==id7);
	//
	test(entryArray->At(6)->Id()==id8);
	test(entryArray->At(7)->Id()==id10);
	//
	test(entryArray->At(8)->Id()==id9);
	// Attachments
	test(entryArray->At(9)->Id()==id4);
	test(entryArray->At(10)->Id()==id3);
	//
	test(entryArray->At(11)->Id()==id1);
	test(entryArray->At(12)->Id()==id2);
	//
	test(entryArray->At(13)->Id()==id5);

	test.Printf(_L("Sorting by id\n"));
	ordering.SetSorting(EMsvSortById);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(EFalse);
	entryArray->SortL(ordering);
	
	test(entryArray->Count()==14); 
	test(entryArray->At(0)->Id()==idA);
	test(entryArray->At(1)->Id()==idB);
	test(entryArray->At(2)->Id()==idC);
	test(entryArray->At(3)->Id()==idD);
	test(entryArray->At(4)->Id()==id1);
	test(entryArray->At(5)->Id()==id2);
	test(entryArray->At(6)->Id()==id3);
	test(entryArray->At(7)->Id()==id4);
	test(entryArray->At(8)->Id()==id5);
	test(entryArray->At(9)->Id()==id6);
	test(entryArray->At(10)->Id()==id7);
	test(entryArray->At(11)->Id()==id8);
	test(entryArray->At(12)->Id()==id9);
	test(entryArray->At(13)->Id()==id10);


	test.Printf(_L("Reverse sort by Id\n"));
	ordering.SetSorting(EMsvSortByIdReverse);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(EFalse);
	entryArray->SortL(ordering);
	
	test(entryArray->Count()==14); 
	test(entryArray->At(13)->Id()==idA);
	test(entryArray->At(12)->Id()==idB);
	test(entryArray->At(11)->Id()==idC);
	test(entryArray->At(10)->Id()==idD);
	test(entryArray->At(9)->Id()==id1);
	test(entryArray->At(8)->Id()==id2);
	test(entryArray->At(7)->Id()==id3);
	test(entryArray->At(6)->Id()==id4);
	test(entryArray->At(5)->Id()==id5);
	test(entryArray->At(4)->Id()==id6);
	test(entryArray->At(3)->Id()==id7);
	test(entryArray->At(2)->Id()==id8);
	test(entryArray->At(1)->Id()==id9);
	test(entryArray->At(0)->Id()==id10);


	test.Printf(_L("Sorting by Id, grouping by MTM\n"));
	ordering.SetSorting(EMsvSortById);
	ordering.SetGroupByType(EFalse);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); 
	test(entryArray->At(0)->Id()==idA);
	test(entryArray->At(1)->Id()==idB);
	test(entryArray->At(2)->Id()==idC);
	test(entryArray->At(3)->Id()==idD);
	test(entryArray->At(4)->Id()==id3);
	test(entryArray->At(5)->Id()==id4);
	test(entryArray->At(6)->Id()==id6);
	//
	test(entryArray->At(7)->Id()==id1);
	test(entryArray->At(8)->Id()==id2);
	test(entryArray->At(9)->Id()==id7);
	//
	test(entryArray->At(10)->Id()==id5);
	test(entryArray->At(11)->Id()==id8);
	test(entryArray->At(12)->Id()==id10);
	//
	test(entryArray->At(13)->Id()==id9);


	test.Printf(_L("Sorting by Reverse Id, grouping by Type\n"));
	ordering.SetSorting(EMsvSortById);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupByPriority(EFalse);
	ordering.SetGroupByMtm(EFalse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==14); 

	// NB the code allways sorts the standard folders by details
	// whatever you ask it to do, at least when grouped by type
	// its coded this way, not sure why but it seems to be intentional.

//	test(entryArray->At(3)->Id()==idD);
//	test(entryArray->At(2)->Id()==idC);
//	test(entryArray->At(1)->Id()==idB);
//	test(entryArray->At(0)->Id()==idA);
	//
	test(entryArray->At(8)->Id()==id10);
	test(entryArray->At(7)->Id()==id9);
	test(entryArray->At(6)->Id()==id8);
	test(entryArray->At(5)->Id()==id7);
	test(entryArray->At(4)->Id()==id6);
	//
	test(entryArray->At(13)->Id()==id5);
	test(entryArray->At(12)->Id()==id4);
	test(entryArray->At(11)->Id()==id3);
	test(entryArray->At(10)->Id()==id2);
	test(entryArray->At(9)->Id()==id1);



	TInt count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));

	entryArray->Reset();
	CleanupStack::PopAndDestroy();		
	}



LOCAL_C void TestDetailSortL()
	{
	theMtmList->Reset();
	theMtmList->AppendL(uid1);
	theMtmList->AppendL(uid2);
	theMtmList->AppendL(uid3);
	theMtmList->AppendL(uid4);
	
	TMsvSelectionOrdering ordering;
	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(*theMtmList);

	TMsvId id1=AddEntryL(entryArray,_L("a@b"),_L(""),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),100);
	TMsvId id2=AddEntryL(entryArray,_L("a@b"),_L(""),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,29,0,0)),100);
	TMsvId id3=AddEntryL(entryArray,_L("a@b"),_L(""),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,28,0,0)),100);
	TMsvId id4=AddEntryL(entryArray,_L("c@d"),_L(""),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),100);
	TMsvId id5=AddEntryL(entryArray,_L("c@d"),_L(""),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,29,0,0)),100);
	TMsvId id6=AddEntryL(entryArray,_L("c@d"),_L(""),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,28,0,0)),100);

	test.Printf(_L("Sorting by details, no grouping\n"));
	ordering.SetSorting(EMsvSortByDetails);
	entryArray->SortL(ordering);

	test(entryArray->Count()==6); // 6 entries added
	test(entryArray->At(0)->Id()==id1);
	test(entryArray->At(1)->Id()==id2);
	test(entryArray->At(2)->Id()==id3);
	test(entryArray->At(3)->Id()==id4);
	test(entryArray->At(4)->Id()==id5);
	test(entryArray->At(5)->Id()==id6);


	test.Printf(_L("Sorting by details reverse, no grouping\n"));
	ordering.SetSorting(EMsvSortByDetailsReverse);
	entryArray->SortL(ordering);

	test(entryArray->At(0)->Id()==id4);
	test(entryArray->At(1)->Id()==id5);
	test(entryArray->At(2)->Id()==id6);
	test(entryArray->At(3)->Id()==id1);
	test(entryArray->At(4)->Id()==id2);
	test(entryArray->At(5)->Id()==id3);


	TInt count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));

	entryArray->Reset();
	CleanupStack::PopAndDestroy(entryArray);
	}



LOCAL_C void TestSortingSpeedL(CMsvEntryArray* aArray, TMsvSelectionOrdering aOrdering,TInt aRepeat,TSpeedTestSort aSort)
//
//	Test speed of sort
//
	{
	CMsvEntryArray* localArray = CMsvEntryArray::NewLC(*theMtmList);

	TTime starttime,endtime;
	
	starttime.HomeTime();
	for (TInt i=0;i<aRepeat;++i)
		{
		localArray->Reset();
		TInt count=aArray->Count();
		while (count--)
			localArray->AppendL(aArray->At(count));
		localArray->SortL(aOrdering);
		}
	endtime.HomeTime();
	TTimeIntervalMicroSeconds elaspedtime = endtime.MicroSecondsFrom(starttime);
	
	TBuf<100> buffer;
	switch (aSort)
		{
	case ENoSorting:
		buffer.Format(_L("    sorting 90 entries %d times with nothing set took %ds\n"),
			aRepeat,((TInt64&)elaspedtime)/1000000);
		break;
	case ESortingByDescriptionReverseNoGrouping:
		buffer.Format(_L("   sorting 90 entries %d times by details reverse no grouping took %ds\n"),
			aRepeat,((TInt64&)elaspedtime)/1000000);
		break;
	case ESortingByDescriptionReverseAllGrouping:
		buffer.Format(_L("   sorting 90 entries %d times by details reverse all grouping took %ds\n"),
			aRepeat,((TInt64&)elaspedtime)/1000000);
		break;
	case ESortingByDescriptionNoGrouping:
		buffer.Format(_L("   sorting 90 entries %d times by details no grouping took %ds\n"),
			aRepeat,((TInt64&)elaspedtime)/1000000);
		break;
	case ESortingByDescriptionAllGrouping:
		buffer.Format(_L("   sorting 90 entries %d times by details all grouping took %ds\n"),
			aRepeat,((TInt64&)elaspedtime)/1000000);
		break;
	case ESortingByDescriptionGroupingByPriority:
		buffer.Format(_L("   sorting 90 entries %d times by details grouping by priority took %ds\n"),
			aRepeat,((TInt64&)elaspedtime)/1000000);
		break;
	case ESortingByDescriptionGroupingByMessageType:
		buffer.Format(_L("   sorting 90 entries %d times by details grouping by type took %ds\n"),
			aRepeat,((TInt64&)elaspedtime)/1000000);
		break;
		}
	test.Printf(buffer);
	CleanupStack::PopAndDestroy();
	}

LOCAL_C void SpeedTestL(TInt aRepeat,TSpeedTestSort aSort)
//
//	Test speed of sort
//
	{
	test.Printf(_L("Checking sorting speed\n"));

	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(*theMtmList);

	AddEntryL(entryArray,_L("nothing"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),100);
	AddEntryL(entryArray,_L("abc"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),23);
	AddEntryL(entryArray,_L("abcdefgh"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),45);
	AddEntryL(entryArray,_L("qwerty"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),87);
	AddEntryL(entryArray,_L("Michelle"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),2);

	AddEntryL(entryArray,_L("thing"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),1);
	AddEntryL(entryArray,_L("wibble"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),1);
	AddEntryL(entryArray,_L("astronaut"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),1);
	AddEntryL(entryArray,_L("bludgeon"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),2);
	AddEntryL(entryArray,_L("Natascha"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),4);

	AddEntryL(entryArray,_L("a"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),34);
	AddEntryL(entryArray,_L("aa"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),7);
	AddEntryL(entryArray,_L("aaa"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),25);
	AddEntryL(entryArray,_L("aba"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),87);
	AddEntryL(entryArray,_L("ii"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),100);

	AddEntryL(entryArray,_L("qq"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),2);
	AddEntryL(entryArray,_L("ww"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),1);
	AddEntryL(entryArray,_L("ee"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),1);
	AddEntryL(entryArray,_L("rr"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),2);
	AddEntryL(entryArray,_L("tt"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),3);

	AddEntryL(entryArray,_L("asa"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),45);
	AddEntryL(entryArray,_L("qwr"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),2);
	AddEntryL(entryArray,_L("hghgd"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),9);
	AddEntryL(entryArray,_L("eqre"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),10);
	AddEntryL(entryArray,_L("zvczv"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),54);

	AddEntryL(entryArray,_L("yuyu"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),33);
	AddEntryL(entryArray,_L("iutu"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),22);
	AddEntryL(entryArray,_L("oioi"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),22);
	AddEntryL(entryArray,_L("ye"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),11);
	AddEntryL(entryArray,_L("ojg"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),10);

 	AddEntryL(entryArray,_L("adsf"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),9);
	AddEntryL(entryArray,_L("rre"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),2);
	AddEntryL(entryArray,_L("erqer"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),3);
	AddEntryL(entryArray,_L("rtre"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),4);
	AddEntryL(entryArray,_L("ytdsf"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),3);

	AddEntryL(entryArray,_L("utiu"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),11);
	AddEntryL(entryArray,_L("tuyiut"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),44);
	AddEntryL(entryArray,_L("iouyoi"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),55);
	AddEntryL(entryArray,_L("3243"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),5);
	AddEntryL(entryArray,_L("safd"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),6);

	AddEntryL(entryArray,_L("qre"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),1);
	AddEntryL(entryArray,_L("afdv"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),1);
	AddEntryL(entryArray,_L("uiui"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),1);
	AddEntryL(entryArray,_L("ioioert"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),1);
	AddEntryL(entryArray,_L("qwerec"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),8);

	AddEntryL(entryArray,_L("adasdf"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),7);
	AddEntryL(entryArray,_L("wqr"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),5);
	AddEntryL(entryArray,_L("jhkjg"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),12);
	AddEntryL(entryArray,_L("uiui"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),19);
	AddEntryL(entryArray,_L("teyteq"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),20);

	AddEntryL(entryArray,_L("djdue"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),122);
	AddEntryL(entryArray,_L("oeior"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),12);
	AddEntryL(entryArray,_L("clcjue"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),11);
	AddEntryL(entryArray,_L("oeproy"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),4);
	AddEntryL(entryArray,_L("peoreu"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),3);

	AddEntryL(entryArray,_L("qreqr"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),1);
	AddEntryL(entryArray,_L("dddaa"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),3);
	AddEntryL(entryArray,_L("eeerr"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),5);

 	AddEntryL(entryArray,_L("ewur"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),89);
	AddEntryL(entryArray,_L("qerdgy"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),45);
	AddEntryL(entryArray,_L("der"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),20);
	AddEntryL(entryArray,_L("aqwe"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),18);
	AddEntryL(entryArray,_L("afe"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),13);
						   
	AddEntryL(entryArray,_L("kfjue"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),9);
	AddEntryL(entryArray,_L("ewoiuioreuojk"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),5);
	AddEntryL(entryArray,_L("erqiueriqup"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),4);
	AddEntryL(entryArray,_L("afiaei"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),2);
	AddEntryL(entryArray,_L("euyuioyo"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),2);

	AddEntryL(entryArray,_L("AUIYEIO"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),1);
	AddEntryL(entryArray,_L("ERUIEYIO"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),3);
	AddEntryL(entryArray,_L("AFDA"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),40);
	AddEntryL(entryArray,_L("ERQRE"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),9);
	AddEntryL(entryArray,_L("FHGHJYU"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),3);

	AddEntryL(entryArray,_L("IOUYIOPO"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),1);
	AddEntryL(entryArray,_L("JHDK8U"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),11);
	AddEntryL(entryArray,_L("EQREQQ"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),20);
	AddEntryL(entryArray,_L("djhflkd"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),50);
	AddEntryL(entryArray,_L("tyeav"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),5);

	AddEntryL(entryArray,_L("oyoitpy"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),8);
	AddEntryL(entryArray,_L("eiuoip"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),4);
	AddEntryL(entryArray,_L("yueior"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),1);
	AddEntryL(entryArray,_L("ewureup"),KUidMsvAttachmentEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),1);
	AddEntryL(entryArray,_L("eruyeo"),KUidMsvAttachmentEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),1);

	AddEntryL(entryArray,_L("nvhjfdh"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),210);
	AddEntryL(entryArray,_L("wuyrio"),KUidMsvMessageEntry,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),200);
	AddEntryL(entryArray,_L("fhjciuov"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),10);
	AddEntryL(entryArray,_L("ereruio"),KUidMsvMessageEntry,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),9);
	AddEntryL(entryArray,_L("eruiyo"),KUidMsvMessageEntry,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),4);
	
	TMsvSelectionOrdering ordering=TMsvSelectionOrdering();
	switch (aSort)
		{
	case ENoSorting:
		ordering.SetSorting(EMsvSortByNone);
		ordering.SetGroupByType(EFalse);
		ordering.SetGroupByPriority(EFalse);
		ordering.SetGroupByMtm(EFalse);
		break;
	case ESortingByDescriptionReverseNoGrouping:
		ordering.SetSorting(EMsvSortByDetailsReverse);
		ordering.SetGroupByType(EFalse);
		ordering.SetGroupByPriority(EFalse);
		ordering.SetGroupByMtm(EFalse);
		break;
	case ESortingByDescriptionReverseAllGrouping:
		ordering.SetSorting(EMsvSortByDetailsReverse);
		ordering.SetGroupByType(ETrue);
		ordering.SetGroupByPriority(ETrue);
		ordering.SetGroupByMtm(EFalse);
		break;
	case ESortingByDescriptionNoGrouping:
		ordering.SetSorting(EMsvSortByDetails);
		ordering.SetGroupByType(EFalse);
		ordering.SetGroupByPriority(EFalse);
		ordering.SetGroupByMtm(EFalse);
		break;
	case ESortingByDescriptionAllGrouping:
		ordering.SetSorting(EMsvSortByDetails);
		ordering.SetGroupByType(ETrue);
		ordering.SetGroupByPriority(ETrue);
		ordering.SetGroupByMtm(EFalse);
		break;
	case ESortingByDescriptionGroupingByPriority:
		ordering.SetSorting(EMsvSortByDetails);
		ordering.SetGroupByType(EFalse);
		ordering.SetGroupByPriority(ETrue);
		ordering.SetGroupByMtm(EFalse);
		break;
	case ESortingByDescriptionGroupingByMessageType:
		ordering.SetSorting(EMsvSortByDetails);
		ordering.SetGroupByType(ETrue);
		ordering.SetGroupByPriority(EFalse);
		ordering.SetGroupByMtm(EFalse);
		break;
		}

	TestSortingSpeedL(entryArray,ordering,aRepeat,aSort);

	TInt count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));

	entryArray->Reset();
	CleanupStack::PopAndDestroy();		
	} 



LOCAL_C CMsvEntryArray* CreateMtmSortArrayL(const CArrayFixFlat<TUid>& aMtmList)
	{
	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(aMtmList);

	AddEntryL(entryArray,_L("Inbox"),_L(""),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0);
	AddEntryL(entryArray,_L("Outbox"),_L("ff"),KUidMsvFolderEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0);
	AddEntryL(entryArray,_L("Drafts"),_L(""),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0);
	AddEntryL(entryArray,_L("Sent"),_L(""),KUidMsvFolderEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	
	AddEntryL(entryArray,_L("nothing"),_L("once"),KUidMsvAttachmentEntry,uid3,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,1,8,30,0,0)),1);
	AddEntryL(entryArray,_L("abc"),_L("upon"),KUidMsvAttachmentEntry,uid4,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,12,8,30,0,0)),87);
	AddEntryL(entryArray,_L("abcdefgh"),_L("a"),KUidMsvAttachmentEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,30,8,30,0,0)),900);
	AddEntryL(entryArray,_L("qwerty"),_L("time"),KUidMsvAttachmentEntry,uid5,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,4,8,30,0,0)),7);
	AddEntryL(entryArray,_L("Michelle"),_L("there"),KUidMsvAttachmentEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,9,8,30,0,0)),34);

	AddEntryL(entryArray,_L("thing"),_L("was"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),88);
	AddEntryL(entryArray,_L("wibble"),_L("a"),KUidMsvMessageEntry,uid6,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),100);
	AddEntryL(entryArray,_L("astronaut"),_L("young"),KUidMsvMessageEntry,uid1,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),110);
	AddEntryL(entryArray,_L("bludgeon"),_L("billy"),KUidMsvMessageEntry,uid4,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),100);
	AddEntryL(entryArray,_L("Natascha"),_L("goat"),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),100);

	return entryArray;
	}

LOCAL_C void DeleteMtmSortArrayL(CMsvEntryArray* aEntryArray)
	{
	TInt count = aEntryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, aEntryArray->At(count));
	aEntryArray->Reset();
	CleanupStack::PopAndDestroy(); // entry array
	}

LOCAL_C void TestMtmSortL()
	{
	test.Printf(_L("Testing MTM grouping\n"));
	CMsvEntryArray* entryArray;
	CArrayFixFlat<TUid>* mtmList = new(ELeave)CArrayFixFlat<TUid>(8);

	// noraml order, all present
	mtmList->Reset();
	mtmList->AppendL(uid1);
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid3);
	mtmList->AppendL(uid4);
	mtmList->AppendL(uid5);
	mtmList->AppendL(uid6);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid1);	
	test(entryArray->At(1)->iMtm==uid1);	
	test(entryArray->At(2)->iMtm==uid1);	
	test(entryArray->At(3)->iMtm==uid1);	
	test(entryArray->At(4)->iMtm==uid1);	
	test(entryArray->At(5)->iMtm==uid2);	
	test(entryArray->At(6)->iMtm==uid2);	
	test(entryArray->At(7)->iMtm==uid2);	
	test(entryArray->At(8)->iMtm==uid3);	
	test(entryArray->At(9)->iMtm==uid3);	
	test(entryArray->At(10)->iMtm==uid4);	
	test(entryArray->At(11)->iMtm==uid4);	
	test(entryArray->At(12)->iMtm==uid5);	
	test(entryArray->At(13)->iMtm==uid6);	
	DeleteMtmSortArrayL(entryArray);


	// reverse order, all present
	mtmList->Reset();
	mtmList->AppendL(uid6);
	mtmList->AppendL(uid5);
	mtmList->AppendL(uid4);
	mtmList->AppendL(uid3);
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid1);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(13)->iMtm==uid1);	
	test(entryArray->At(12)->iMtm==uid1);	
	test(entryArray->At(11)->iMtm==uid1);	
	test(entryArray->At(10)->iMtm==uid1);	
	test(entryArray->At(9)->iMtm==uid1);	
	test(entryArray->At(8)->iMtm==uid2);	
	test(entryArray->At(7)->iMtm==uid2);	
	test(entryArray->At(6)->iMtm==uid2);	
	test(entryArray->At(5)->iMtm==uid3);	
	test(entryArray->At(4)->iMtm==uid3);	
	test(entryArray->At(3)->iMtm==uid4);	
	test(entryArray->At(2)->iMtm==uid4);	
	test(entryArray->At(1)->iMtm==uid5);	
	test(entryArray->At(0)->iMtm==uid6);	
	DeleteMtmSortArrayL(entryArray);


	// random order, all present
	mtmList->Reset();
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid4);
	mtmList->AppendL(uid3);
	mtmList->AppendL(uid1);
	mtmList->AppendL(uid6);
	mtmList->AppendL(uid5);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid2);	
	test(entryArray->At(1)->iMtm==uid2);	
	test(entryArray->At(2)->iMtm==uid2);	
	test(entryArray->At(3)->iMtm==uid4);	
	test(entryArray->At(4)->iMtm==uid4);	
	test(entryArray->At(5)->iMtm==uid3);	
	test(entryArray->At(6)->iMtm==uid3);	
	test(entryArray->At(7)->iMtm==uid1);	
	test(entryArray->At(8)->iMtm==uid1);	
	test(entryArray->At(9)->iMtm==uid1);	
	test(entryArray->At(10)->iMtm==uid1);	
	test(entryArray->At(11)->iMtm==uid1);	
	test(entryArray->At(12)->iMtm==uid6);	
	test(entryArray->At(13)->iMtm==uid5);	
	DeleteMtmSortArrayL(entryArray);

	// random order, 1 missing
	mtmList->Reset();
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid4);
	mtmList->AppendL(uid3);
	mtmList->AppendL(uid6);
	mtmList->AppendL(uid5);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid2);	
	test(entryArray->At(1)->iMtm==uid2);	
	test(entryArray->At(2)->iMtm==uid2);	
	test(entryArray->At(3)->iMtm==uid4);	
	test(entryArray->At(4)->iMtm==uid4);	
	test(entryArray->At(5)->iMtm==uid3);	
	test(entryArray->At(6)->iMtm==uid3);	
	test(entryArray->At(7)->iMtm==uid6);	
	test(entryArray->At(8)->iMtm==uid5);	
	test(entryArray->At(9)->iMtm==uid1);	
	test(entryArray->At(10)->iMtm==uid1);	
	test(entryArray->At(11)->iMtm==uid1);	
	test(entryArray->At(12)->iMtm==uid1);	
	test(entryArray->At(13)->iMtm==uid1);	
	DeleteMtmSortArrayL(entryArray);

	// random order, 1 3 missing
	mtmList->Reset();
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid4);
	mtmList->AppendL(uid6);
	mtmList->AppendL(uid5);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid2);	
	test(entryArray->At(1)->iMtm==uid2);	
	test(entryArray->At(2)->iMtm==uid2);	
	test(entryArray->At(3)->iMtm==uid4);	
	test(entryArray->At(4)->iMtm==uid4);	
	test(entryArray->At(5)->iMtm==uid6);	
	test(entryArray->At(6)->iMtm==uid5);	
	test(entryArray->At(7)->iMtm==uid1);	
	test(entryArray->At(8)->iMtm==uid1);	
	test(entryArray->At(9)->iMtm==uid1);	
	test(entryArray->At(10)->iMtm==uid1);	
	test(entryArray->At(11)->iMtm==uid1);	
	test(entryArray->At(12)->iMtm==uid3);	
	test(entryArray->At(13)->iMtm==uid3);	
	DeleteMtmSortArrayL(entryArray);

	// random order, 1 3 5 missing
	mtmList->Reset();
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid4);
	mtmList->AppendL(uid6);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid2);	
	test(entryArray->At(1)->iMtm==uid2);	
	test(entryArray->At(2)->iMtm==uid2);	
	test(entryArray->At(3)->iMtm==uid4);	
	test(entryArray->At(4)->iMtm==uid4);	
	test(entryArray->At(5)->iMtm==uid6);	
	test(entryArray->At(6)->iMtm==uid1);	
	test(entryArray->At(7)->iMtm==uid1);	
	test(entryArray->At(8)->iMtm==uid1);	
	test(entryArray->At(9)->iMtm==uid1);	
	test(entryArray->At(10)->iMtm==uid1);	
	test(entryArray->At(11)->iMtm==uid3);	
	test(entryArray->At(12)->iMtm==uid3);	
	test(entryArray->At(13)->iMtm==uid5);	
	DeleteMtmSortArrayL(entryArray);

	// random order, 1 3 5 6 missing
	mtmList->Reset();
	mtmList->AppendL(uid2);
	mtmList->AppendL(uid4);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid2);	
	test(entryArray->At(1)->iMtm==uid2);	
	test(entryArray->At(2)->iMtm==uid2);	
	test(entryArray->At(3)->iMtm==uid4);	
	test(entryArray->At(4)->iMtm==uid4);	
	test(entryArray->At(5)->iMtm==uid1);	
	test(entryArray->At(6)->iMtm==uid1);	
	test(entryArray->At(7)->iMtm==uid1);	
	test(entryArray->At(8)->iMtm==uid1);	
	test(entryArray->At(9)->iMtm==uid1);	
	test(entryArray->At(10)->iMtm==uid3);	
	test(entryArray->At(11)->iMtm==uid3);	
	test(entryArray->At(12)->iMtm==uid5);	
	test(entryArray->At(13)->iMtm==uid6);	
	DeleteMtmSortArrayL(entryArray);

	// random order, 1 2 3 5 6 missing
	mtmList->Reset();
	mtmList->AppendL(uid4);
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid4);	
	test(entryArray->At(1)->iMtm==uid4);	
	test(entryArray->At(2)->iMtm==uid1);	
	test(entryArray->At(3)->iMtm==uid1);	
	test(entryArray->At(4)->iMtm==uid1);	
	test(entryArray->At(5)->iMtm==uid1);	
	test(entryArray->At(6)->iMtm==uid1);	
	test(entryArray->At(7)->iMtm==uid2);	
	test(entryArray->At(8)->iMtm==uid2);	
	test(entryArray->At(9)->iMtm==uid2);	
	test(entryArray->At(10)->iMtm==uid3);	
	test(entryArray->At(11)->iMtm==uid3);	
	test(entryArray->At(12)->iMtm==uid5);	
	test(entryArray->At(13)->iMtm==uid6);	
	DeleteMtmSortArrayL(entryArray);

	// all missing
	mtmList->Reset();
	entryArray = CreateMtmSortArrayL(*mtmList);
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByNone));
	test(entryArray->At(0)->iMtm==uid1);	
	test(entryArray->At(1)->iMtm==uid1);	
	test(entryArray->At(2)->iMtm==uid1);	
	test(entryArray->At(3)->iMtm==uid1);	
	test(entryArray->At(4)->iMtm==uid1);	
	test(entryArray->At(5)->iMtm==uid2);	
	test(entryArray->At(6)->iMtm==uid2);	
	test(entryArray->At(7)->iMtm==uid2);	
	test(entryArray->At(8)->iMtm==uid3);	
	test(entryArray->At(9)->iMtm==uid3);	
	test(entryArray->At(10)->iMtm==uid4);	
	test(entryArray->At(11)->iMtm==uid4);	
	test(entryArray->At(12)->iMtm==uid5);	
	test(entryArray->At(13)->iMtm==uid6);	
	DeleteMtmSortArrayL(entryArray);


	delete mtmList;
	}
	
	
LOCAL_C void TestStandardFoldersL()
	{
	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(*theMtmList);

	TMsvId idA=AddEntryL(entryArray,_L("Inbox"),_L(""),KUidMsvFolderEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0,ETrue);
	TMsvId idB=AddEntryL(entryArray,_L("Outbox"),_L("ff"),KUidMsvFolderEntry,uid1,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0,ETrue);
	TMsvId idC=AddEntryL(entryArray,_L("Drafts"),_L(""),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0);
	TMsvId idD=AddEntryL(entryArray,_L("Sent"),_L(""),KUidMsvFolderEntry,uid2,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	
	TMsvId id1=AddEntryL(entryArray,_L("thing"),_L("was"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,2,8,30,0,0)),88);
	TMsvId id2=AddEntryL(entryArray,_L("wibble"),_L("a"),KUidMsvMessageEntry,uid4,EMsvHighPriority,TTime(TDateTime(1999,EJanuary,5,8,30,0,0)),100);
	TMsvId id3=AddEntryL(entryArray,_L("astronaut"),_L("young"),KUidMsvMessageEntry,uid1,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,3,8,30,0,0)),110);
	TMsvId id4=AddEntryL(entryArray,_L("bludgeon"),_L("billy"),KUidMsvMessageEntry,uid4,EMsvLowPriority,TTime(TDateTime(1999,EJanuary,22,8,30,0,0)),100);
	TMsvId id5=AddEntryL(entryArray,_L("Natascha"),_L("goat"),KUidMsvMessageEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1999,EJanuary,13,8,30,0,0)),100);

	test.Printf(_L("Testing standard folders\n"));

	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByNone));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idB);	
	test(entryArray->At(2)->Id()==idC);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==id1);	
	test(entryArray->At(5)->Id()==id2);	
	test(entryArray->At(6)->Id()==id3);	
	test(entryArray->At(7)->Id()==id4);	
	test(entryArray->At(8)->Id()==id5);	
	
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByDetails));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idB);	
	test(entryArray->At(2)->Id()==idC);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==id3);	
	test(entryArray->At(5)->Id()==id4);	
	test(entryArray->At(6)->Id()==id5);	
	test(entryArray->At(7)->Id()==id1);	
	test(entryArray->At(8)->Id()==id2);	

	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByDetailsReverse));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idB);	
	test(entryArray->At(2)->Id()==idC);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==id2);	
	test(entryArray->At(5)->Id()==id1);	
	test(entryArray->At(6)->Id()==id5);	
	test(entryArray->At(7)->Id()==id4);	
	test(entryArray->At(8)->Id()==id3);	

	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders|KMsvGroupByPriority, EMsvSortByDetails));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idB);	
	test(entryArray->At(2)->Id()==idC);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==id2);	
	test(entryArray->At(5)->Id()==id5);	
	test(entryArray->At(6)->Id()==id1);	
	test(entryArray->At(7)->Id()==id3);	
	test(entryArray->At(8)->Id()==id4);	

	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders|KMsvGroupByPriority|KMsvGroupByMtm, EMsvSortByDetails));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idB);	
	test(entryArray->At(2)->Id()==idC);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==id2);	
	test(entryArray->At(5)->Id()==id1);	
	test(entryArray->At(6)->Id()==id5);	
	test(entryArray->At(7)->Id()==id3);	
	test(entryArray->At(8)->Id()==id4);	

	TInt count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));
	entryArray->Reset();
	CleanupStack::PopAndDestroy(); // entry array
	}
	


LOCAL_C void TestServicesL()
	{
	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(*theMtmList);

	TMsvId idA=AddEntryL(entryArray,_L("Inbox"),_L(""),KUidMsvServiceEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0,ETrue);
	TMsvId idB=AddEntryL(entryArray,_L("Outbox"),_L(""),KUidMsvServiceEntry,uid1,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0);
	TMsvId idC=AddEntryL(entryArray,_L("Drafts"),_L(""),KUidMsvServiceEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0,ETrue);
	TMsvId idD=AddEntryL(entryArray,_L("Sent"),_L(""),KUidMsvServiceEntry,uid2,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	TMsvId idE=AddEntryL(entryArray,_L("My service"),_L(""),KUidMsvServiceEntry,uid2,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	TMsvId idF=AddEntryL(entryArray,_L("Your service"),_L(""),KUidMsvServiceEntry,uid1,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	

	test.Printf(_L("Testing services\n"));

	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByNone));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idC);	
	test(entryArray->At(2)->Id()==idB);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==idE);	
	test(entryArray->At(5)->Id()==idF);	
	
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByDetails));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idC);	
	test(entryArray->At(2)->Id()==idE);	
	test(entryArray->At(3)->Id()==idB);	
	test(entryArray->At(4)->Id()==idD);	
	test(entryArray->At(5)->Id()==idF);	

	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByDetailsReverse));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idC);	
	test(entryArray->At(2)->Id()==idF);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==idB);	
	test(entryArray->At(5)->Id()==idE);	


	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders|KMsvGroupByMtm, EMsvSortByDetails));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idC);	
	test(entryArray->At(2)->Id()==idB);	
	test(entryArray->At(3)->Id()==idF);	
	test(entryArray->At(4)->Id()==idE);	
	test(entryArray->At(5)->Id()==idD);	

	TInt count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));
	entryArray->Reset();
	CleanupStack::PopAndDestroy(); // entry array
	}


LOCAL_C void TestUnknownEntryTypesL()
	{
	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(*theMtmList);

	TMsvId idA=AddEntryL(entryArray,_L("Sercice"),_L(""),KUidMsvServiceEntry,uid2,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0,ETrue);
	TMsvId idE=AddEntryL(entryArray,_L("Unknown"),_L(""),uid2,uid2,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	TMsvId idC=AddEntryL(entryArray,_L("Message"),_L(""),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0,ETrue);
	TMsvId idB=AddEntryL(entryArray,_L("Folder"),_L(""),KUidMsvFolderEntry,uid1,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0);
	TMsvId idD=AddEntryL(entryArray,_L("Attachemnt"),_L(""),KUidMsvAttachmentEntry,uid2,EMsvHighPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,3)),0);
	
	entryArray->SortL(TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone));
	entryArray->SortL(TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByDate));
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByMtm, EMsvSortByDate));
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByPriority, EMsvSortByDate));

	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByType, EMsvSortByDate));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idB);	
	test(entryArray->At(2)->Id()==idC);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==idE);
	
	entryArray->SortL(TMsvSelectionOrdering(KMsvGroupByStandardFolders, EMsvSortByDate));
	test(entryArray->At(0)->Id()==idA);	
	test(entryArray->At(1)->Id()==idC);	
	test(entryArray->At(2)->Id()==idB);	
	test(entryArray->At(3)->Id()==idD);	
	test(entryArray->At(4)->Id()==idE);

	TInt count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));
	entryArray->Reset();
	CleanupStack::PopAndDestroy(); // entry array
	}


/**
@SYMTestCaseID MSG-IMAP-1058
@SYMTestType UT
@SYMTestPriority Critical
@SYMPREQ 1058
@SYMTestCaseDesc Tests sorting with a different subject skip string
@SYMTestActions  Create message entries and sort them (subject based sorting). Check if order after sorting is as expected. Do this for differnt subject skip strings
@SYMTestExpectedResults After sorting the order of the messages should be as expected.
*/
LOCAL_C void TestSubjectSortingL()
	{
	TMsvSelectionOrdering ordering;
	CMsvEntryArray* entryArray = CMsvEntryArray::NewLC(*theMtmList);

	// First test with the default skip string (colon + space)
	TMsvId idA=AddEntryL(entryArray,_L("Hello1"),_L("Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,0)),0);
	TMsvId idB=AddEntryL(entryArray,_L("Hello2"),_L("Re: Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,1)),0);
	TMsvId idC=AddEntryL(entryArray,_L("Hello3"),_L("Re: Re: Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,2)),0);
	TMsvId idD=AddEntryL(entryArray,_L("Hello4"),_L("Fwd: Re: Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,3)),0);
	
	TMsvId idE=AddEntryL(entryArray,_L("Goodbye1"),_L("Goodbye"),KUidMsvMessageEntry,uid2,EMsvHighPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,0)),1);
	TMsvId idF=AddEntryL(entryArray,_L("Goodbye2"),_L("Re: Goodbye"),KUidMsvMessageEntry,uid2,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,12,8,30,0,0)),87);
	TMsvId idG=AddEntryL(entryArray,_L("Goodbye3"),_L("Re: Re: Goodbye"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,30,8,30,0,0)),900);
	TMsvId idH=AddEntryL(entryArray,_L("Goodbye4"),_L("Fwd: Re: Goodbye"),KUidMsvMessageEntry,uid1,EMsvHighPriority,TTime(TDateTime(2005,EJanuary,4,8,30,0,0)),7);
	TMsvId idI=AddEntryL(entryArray,_L("Goodbye5"),_L("Fwd: Re: Re: Goodbye"),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,9,8,30,0,0)),34);

	TMsvId idJ=AddEntryL(entryArray,_L("Bathtub1"),_L("Bathtub"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,2,8,30,0,0)),88);
	TMsvId idK=AddEntryL(entryArray,_L("Bathtub2"),_L("Re: Bathtub"),KUidMsvMessageEntry,uid2,EMsvHighPriority,TTime(TDateTime(2005,EJanuary,5,8,30,0,0)),100);
	TMsvId idL=AddEntryL(entryArray,_L("Bathtub3"),_L("Re: Re: Bathtub"),KUidMsvMessageEntry,uid3,EMsvLowPriority,TTime(TDateTime(2005,EJanuary,3,8,30,0,0)),110);
	TMsvId idM=AddEntryL(entryArray,_L("Bathtub4"),_L("Fwd: Re: Bathtub"),KUidMsvMessageEntry,uid4,EMsvLowPriority,TTime(TDateTime(2005,EJanuary,22,8,30,0,0)),100);
	TMsvId idN=AddEntryL(entryArray,_L("Bathtub5"),_L("Fwd: Bathtub"),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,13,8,30,0,0)),100);
	
	// these are to make sure entries which end with the skip string, these should end up first
	TMsvId idO=AddEntryL(entryArray,_L("Whatsit1"),_L("Whatsit: "),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,1,0)),0);
	TMsvId idP=AddEntryL(entryArray,_L("Whatsit2"),_L("Re: Whatsit: "),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,2,0)),0);
	TMsvId idQ=AddEntryL(entryArray,_L("Whatsit3"),_L("Re: Re: Whatsit: "),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,3,0)),0);
	TMsvId idR=AddEntryL(entryArray,_L("Whatsit4"),_L("Fwd: Re: Whatsit: "),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,4,0)),0);

	
	test.Printf(_L("Sorting by subject\n"));
	ordering.SetSorting(EMsvSortByDescription);
	entryArray->SortL(ordering);
	
	test(entryArray->Count()==18); // 18 entries added
	test(entryArray->At(0)->iDetails==_L("Whatsit4"));
	test(entryArray->At(1)->iDetails==_L("Whatsit3"));
	test(entryArray->At(2)->iDetails==_L("Whatsit2"));
	test(entryArray->At(3)->iDetails==_L("Whatsit1"));
	test(entryArray->At(4)->iDetails==_L("Bathtub4"));
	test(entryArray->At(5)->iDetails==_L("Bathtub5"));
	test(entryArray->At(6)->iDetails==_L("Bathtub2"));
	test(entryArray->At(7)->iDetails==_L("Bathtub3"));
	test(entryArray->At(8)->iDetails==_L("Bathtub1"));
	test(entryArray->At(9)->iDetails==_L("Goodbye3"));
	test(entryArray->At(10)->iDetails==_L("Goodbye2"));
	test(entryArray->At(11)->iDetails==_L("Goodbye5"));
	test(entryArray->At(12)->iDetails==_L("Goodbye4"));
	test(entryArray->At(13)->iDetails==_L("Goodbye1"));
	test(entryArray->At(14)->iDetails==_L("Hello4"));
	test(entryArray->At(15)->iDetails==_L("Hello3"));
	test(entryArray->At(16)->iDetails==_L("Hello2"));
	test(entryArray->At(17)->iDetails==_L("Hello1"));
	
	test.Printf(_L("Reverse sorting by subject\n"));
	ordering.SetSorting(EMsvSortByDescriptionReverse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==18); // 18 entries added
	test(entryArray->At(0)->iDetails==_L("Hello4"));
	test(entryArray->At(1)->iDetails==_L("Hello3"));
	test(entryArray->At(2)->iDetails==_L("Hello2"));
	test(entryArray->At(3)->iDetails==_L("Hello1"));
	test(entryArray->At(4)->iDetails==_L("Goodbye3"));
	test(entryArray->At(5)->iDetails==_L("Goodbye2"));
	test(entryArray->At(6)->iDetails==_L("Goodbye5"));
	test(entryArray->At(7)->iDetails==_L("Goodbye4"));
	test(entryArray->At(8)->iDetails==_L("Goodbye1"));
	test(entryArray->At(9)->iDetails==_L("Bathtub4"));
	test(entryArray->At(10)->iDetails==_L("Bathtub5"));
	test(entryArray->At(11)->iDetails==_L("Bathtub2"));
	test(entryArray->At(12)->iDetails==_L("Bathtub3"));
	test(entryArray->At(13)->iDetails==_L("Bathtub1"));
	test(entryArray->At(14)->iDetails==_L("Whatsit4"));
	test(entryArray->At(15)->iDetails==_L("Whatsit3"));
	test(entryArray->At(16)->iDetails==_L("Whatsit2"));
	test(entryArray->At(17)->iDetails==_L("Whatsit1"));

	test.Printf(_L("Sorting by subject - Group by type and standard folders\n"));
	ordering.SetSorting(EMsvSortByDescription);
	ordering.SetGroupByType(ETrue);
	ordering.SetGroupStandardFolders(ETrue);
	entryArray->SortL(ordering);

	test(entryArray->Count()==18); // 18 entries added
	test(entryArray->At(0)->iDetails==_L("Whatsit4"));
	test(entryArray->At(1)->iDetails==_L("Whatsit3"));
	test(entryArray->At(2)->iDetails==_L("Whatsit2"));
	test(entryArray->At(3)->iDetails==_L("Whatsit1"));
	test(entryArray->At(4)->iDetails==_L("Bathtub4"));
	test(entryArray->At(5)->iDetails==_L("Bathtub5"));
	test(entryArray->At(6)->iDetails==_L("Bathtub2"));
	test(entryArray->At(7)->iDetails==_L("Bathtub3"));
	test(entryArray->At(8)->iDetails==_L("Bathtub1"));
	test(entryArray->At(9)->iDetails==_L("Goodbye3"));
	test(entryArray->At(10)->iDetails==_L("Goodbye2"));
	test(entryArray->At(11)->iDetails==_L("Goodbye5"));
	test(entryArray->At(12)->iDetails==_L("Goodbye4"));
	test(entryArray->At(13)->iDetails==_L("Goodbye1"));
	test(entryArray->At(14)->iDetails==_L("Hello4"));
	test(entryArray->At(15)->iDetails==_L("Hello3"));
	test(entryArray->At(16)->iDetails==_L("Hello2"));
	test(entryArray->At(17)->iDetails==_L("Hello1"));

	ordering.SetGroupByType(EFalse);
	ordering.SetGroupStandardFolders(EFalse);

	//Sorting with empty SubjectSkipString
	ordering.SetSubjectSkipString(_L(""));

	test.Printf(_L("Sorting by subject, empty skip string\n"));
	ordering.SetSorting(EMsvSortByDescription);
	entryArray->SortL(ordering);
	
	test(entryArray->Count()==18); // 18 entries added
	test(entryArray->At(0)->iDetails==_L("Whatsit4"));
	test(entryArray->At(1)->iDetails==_L("Whatsit3"));
	test(entryArray->At(2)->iDetails==_L("Whatsit2"));
	test(entryArray->At(3)->iDetails==_L("Whatsit1"));
	test(entryArray->At(4)->iDetails==_L("Bathtub4"));
	test(entryArray->At(5)->iDetails==_L("Bathtub5"));
	test(entryArray->At(6)->iDetails==_L("Bathtub2"));
	test(entryArray->At(7)->iDetails==_L("Bathtub3"));
	test(entryArray->At(8)->iDetails==_L("Bathtub1"));
	test(entryArray->At(9)->iDetails==_L("Goodbye3"));
	test(entryArray->At(10)->iDetails==_L("Goodbye2"));
	test(entryArray->At(11)->iDetails==_L("Goodbye5"));
	test(entryArray->At(12)->iDetails==_L("Goodbye4"));
	test(entryArray->At(13)->iDetails==_L("Goodbye1"));
	test(entryArray->At(14)->iDetails==_L("Hello4"));
	test(entryArray->At(15)->iDetails==_L("Hello3"));
	test(entryArray->At(16)->iDetails==_L("Hello2"));
	test(entryArray->At(17)->iDetails==_L("Hello1"));
	
	TInt count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));

	entryArray->Reset();
	
	
	// Now test with different skip string set
	ordering.SetSubjectSkipString(_L("!>"));

	idA=AddEntryL(entryArray,_L("Hello1"),_L("Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,0)),0);
	idB=AddEntryL(entryArray,_L("Hello2"),_L("Re!>Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,1)),0);
	idC=AddEntryL(entryArray,_L("Hello3"),_L("Re!>Re!>Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,2)),0);
	idD=AddEntryL(entryArray,_L("Hello4"),_L("Fwd!>Re!>Hello"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,3)),0);
	
	idE=AddEntryL(entryArray,_L("Goodbye1"),_L("Goodbye"),KUidMsvMessageEntry,uid2,EMsvHighPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,0)),1);
	idF=AddEntryL(entryArray,_L("Goodbye2"),_L("Re!>Goodbye"),KUidMsvMessageEntry,uid2,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,12,8,30,0,0)),87);
	idG=AddEntryL(entryArray,_L("Goodbye3"),_L("Re!>Re!>Goodbye"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,30,8,30,0,0)),900);
	idH=AddEntryL(entryArray,_L("Goodbye4"),_L("Fwd!>Re!>Goodbye"),KUidMsvMessageEntry,uid1,EMsvHighPriority,TTime(TDateTime(2005,EJanuary,4,8,30,0,0)),7);
	idI=AddEntryL(entryArray,_L("Goodbye5"),_L("Fwd!>Re!>Re!>Goodbye"),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,9,8,30,0,0)),34);

	idJ=AddEntryL(entryArray,_L("Bathtub1"),_L("Bathtub"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,2,8,30,0,0)),88);
	idK=AddEntryL(entryArray,_L("Bathtub2"),_L("Re!>Bathtub"),KUidMsvMessageEntry,uid2,EMsvHighPriority,TTime(TDateTime(2005,EJanuary,5,8,30,0,0)),100);
	idL=AddEntryL(entryArray,_L("Bathtub3"),_L("Re!>Re!>Bathtub"),KUidMsvMessageEntry,uid3,EMsvLowPriority,TTime(TDateTime(2005,EJanuary,3,8,30,0,0)),110);
	idM=AddEntryL(entryArray,_L("Bathtub4"),_L("Fwd!>Re!>Bathtub"),KUidMsvMessageEntry,uid4,EMsvLowPriority,TTime(TDateTime(2005,EJanuary,22,8,30,0,0)),100);
	idN=AddEntryL(entryArray,_L("Bathtub5"),_L("Fwd!>Bathtub"),KUidMsvMessageEntry,uid3,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,13,8,30,0,0)),100);
	
	// these are to make sure entries which end with the skip string, these should end up first
	idO=AddEntryL(entryArray,_L("Whatsit1"),_L("Whatsit!>"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,0,0)),0);
	idP=AddEntryL(entryArray,_L("Whatsit2"),_L("Re!>Whatsit!>"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,1,0)),0);
	idQ=AddEntryL(entryArray,_L("Whatsit3"),_L("Re!>Re!>Whatsit!>"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,2,0)),0);
	idR=AddEntryL(entryArray,_L("Whatsit4"),_L("Fwd!>Re!>Whatsit!>"),KUidMsvMessageEntry,uid1,EMsvMediumPriority,TTime(TDateTime(2005,EJanuary,1,8,30,3,0)),0);

	test.Printf(_L("Sorting by subject, different skip string\n"));
	ordering.SetSorting(EMsvSortByDescription);
	entryArray->SortL(ordering);
	
	test(entryArray->Count()==18); // 18 entries added
	test(entryArray->At(0)->iDetails==_L("Whatsit4"));
	test(entryArray->At(1)->iDetails==_L("Whatsit3"));
	test(entryArray->At(2)->iDetails==_L("Whatsit2"));
	test(entryArray->At(3)->iDetails==_L("Whatsit1"));
	test(entryArray->At(4)->iDetails==_L("Bathtub4"));
	test(entryArray->At(5)->iDetails==_L("Bathtub5"));
	test(entryArray->At(6)->iDetails==_L("Bathtub2"));
	test(entryArray->At(7)->iDetails==_L("Bathtub3"));
	test(entryArray->At(8)->iDetails==_L("Bathtub1"));
	test(entryArray->At(9)->iDetails==_L("Goodbye3"));
	test(entryArray->At(10)->iDetails==_L("Goodbye2"));
	test(entryArray->At(11)->iDetails==_L("Goodbye5"));
	test(entryArray->At(12)->iDetails==_L("Goodbye4"));
	test(entryArray->At(13)->iDetails==_L("Goodbye1"));
	test(entryArray->At(14)->iDetails==_L("Hello4"));
	test(entryArray->At(15)->iDetails==_L("Hello3"));
	test(entryArray->At(16)->iDetails==_L("Hello2"));
	test(entryArray->At(17)->iDetails==_L("Hello1"));
	
	
	test.Printf(_L("Reverse sorting by subject, different skip string\n"));
	ordering.SetSorting(EMsvSortByDescriptionReverse);
	entryArray->SortL(ordering);

	test(entryArray->Count()==18); // 18 entries added
	test(entryArray->At(0)->iDetails==_L("Hello4"));
	test(entryArray->At(1)->iDetails==_L("Hello3"));
	test(entryArray->At(2)->iDetails==_L("Hello2"));
	test(entryArray->At(3)->iDetails==_L("Hello1"));
	test(entryArray->At(4)->iDetails==_L("Goodbye3"));
	test(entryArray->At(5)->iDetails==_L("Goodbye2"));
	test(entryArray->At(6)->iDetails==_L("Goodbye5"));
	test(entryArray->At(7)->iDetails==_L("Goodbye4"));
	test(entryArray->At(8)->iDetails==_L("Goodbye1"));
	test(entryArray->At(9)->iDetails==_L("Bathtub4"));
	test(entryArray->At(10)->iDetails==_L("Bathtub5"));
	test(entryArray->At(11)->iDetails==_L("Bathtub2"));
	test(entryArray->At(12)->iDetails==_L("Bathtub3"));
	test(entryArray->At(13)->iDetails==_L("Bathtub1"));
	test(entryArray->At(14)->iDetails==_L("Whatsit4"));
	test(entryArray->At(15)->iDetails==_L("Whatsit3"));
	test(entryArray->At(16)->iDetails==_L("Whatsit2"));
	test(entryArray->At(17)->iDetails==_L("Whatsit1"));
	
	count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));

	entryArray->Reset();
	
	
	// Now test to make sure only message entries are affected by subject sorting
	// folders, attachments and service entries should be sorted the normal way
	ordering.SetSubjectSkipString(_L(": "));
	
	idA=AddEntryL(entryArray,_L("folder1"),_L("First"),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0);
	idB=AddEntryL(entryArray,_L("service1"),_L("Re: First"),KUidMsvServiceEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0);
	idC=AddEntryL(entryArray,_L("attachment1"),_L("Re: Re: First"),KUidMsvAttachmentEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0);
	idD=AddEntryL(entryArray,_L("folder2"),_L("Second"),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0);
	idE=AddEntryL(entryArray,_L("service2"),_L("Re: Second"),KUidMsvServiceEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0);
	idF=AddEntryL(entryArray,_L("attachment2"),_L("Re: Re: Second"),KUidMsvAttachmentEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0);
	idG=AddEntryL(entryArray,_L("folder3"),_L("Third"),KUidMsvFolderEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,0)),0);
	idH=AddEntryL(entryArray,_L("service3"),_L("Re: Third"),KUidMsvServiceEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,1)),0);
	idI=AddEntryL(entryArray,_L("attachment3"),_L("Re: Re: Third"),KUidMsvAttachmentEntry,uid1,EMsvMediumPriority,TTime(TDateTime(1998,EJanuary,1,8,30,0,2)),0);
	
	test.Printf(_L("Sorting non message entries by description\n"));
	ordering.SetSorting(EMsvSortByDescription);
	entryArray->SortL(ordering);
	
	test(entryArray->Count()==9); // 9 entries added
	test(entryArray->At(0)->iDetails==_L("folder1"));
	test(entryArray->At(1)->iDetails==_L("service1"));
	test(entryArray->At(2)->iDetails==_L("attachment1"));
	test(entryArray->At(3)->iDetails==_L("attachment2"));
	test(entryArray->At(4)->iDetails==_L("attachment3"));
	test(entryArray->At(5)->iDetails==_L("service2"));
	test(entryArray->At(6)->iDetails==_L("service3"));
	test(entryArray->At(7)->iDetails==_L("folder2"));
	test(entryArray->At(8)->iDetails==_L("folder3"));
	
	count = entryArray->Count();
	while (count--)
		delete CONST_CAST(TMsvEntry*, entryArray->At(count));

	entryArray->Reset();
	
	CleanupStack::PopAndDestroy();
	}

LOCAL_C void doTestsL()
	{
	id = 100000;

	theBufferArray = new(ELeave) CDesCArraySeg(32);
	CleanupStack::PushL(theBufferArray);

	theMtmList = new(ELeave)CArrayFixFlat<TUid>(8);
	CleanupStack::PushL(theMtmList);

	CMsgsTestUtils::Start(_L("Sorting entries"));
	TestSortL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Start(_L("Sorting detail entries"));
	TestDetailSortL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Mtm Sorting"));
	TestMtmSortL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Standard Folder Sorting"));
	TestStandardFoldersL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Mtm Sorting"));
	TestServicesL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("No sorting"));
	SpeedTestL(250,ENoSorting);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Sorting by description no grouping"));
	SpeedTestL(250,ESortingByDescriptionNoGrouping);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Sorting by description reverse no grouping"));
	SpeedTestL(250,ESortingByDescriptionReverseNoGrouping);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Sorting by description grouping by priority"));
	SpeedTestL(250,ESortingByDescriptionGroupingByPriority);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Sorting by description grouping by message type"));
	SpeedTestL(250,ESortingByDescriptionGroupingByMessageType);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Sorting by description reverse all grouping"));
	SpeedTestL(250,ESortingByDescriptionReverseAllGrouping);
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Sorting by description all grouping"));
	SpeedTestL(250,ESortingByDescriptionAllGrouping);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Unknown entry types"));
	TestUnknownEntryTypesL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Subject sorting"));
	TestSubjectSortingL();
	CMsgsTestUtils::Complete();

	CleanupStack::PopAndDestroy(2); // theMtmList, theBufferArray
	}

#pragma warning( default : 4710 ) // class TMsvEntry const * &  __thiscall CArrayFix<class TMsvEntry const *>::At(int)' not expanded
