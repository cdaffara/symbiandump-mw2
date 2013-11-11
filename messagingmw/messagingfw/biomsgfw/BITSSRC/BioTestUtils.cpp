// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// BioTestUtils.h
//

#include "biotestutils.h"

#include <e32test.h>
#include <f32fsys.h>
#include <s32file.h>
#include <e32uid.h>
#include <txtrich.h>
#include <txtfmlyr.h>
#include <msvids.h>
#include <msvuids.h>
#include <msvruids.h>
#include <e32std.h>
#include <e32math.h>
#include <mtclbase.h>
#include <mtclreg.h>
#include <msvapi.h>
#include <mtmuids.h>

#include <smuthdr.h>
#include <biouids.h>
#include <biodb.h>
#include <biouids.h>
#include <regpsdll.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include <tmsvsmsentry.h>
#include <bifchangeobserver.h>
#include <biomessageuids.h>
#include "tmsvbioinfo.h"
#endif

#define KCharSpace          ' '

EXPORT_C CBioTestUtils::~CBioTestUtils()
	{
	Reset();
	delete iBioDb;
	}

EXPORT_C CBioTestUtils* CBioTestUtils::NewL(RTest& aRTest)
	{
	CBioTestUtils* self = new (ELeave) CBioTestUtils(aRTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CBioTestUtils* CBioTestUtils::NewLC(RTest& aRTest)
	{
	CBioTestUtils* self = new(ELeave) CBioTestUtils(aRTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CBioTestUtils* CBioTestUtils::NewL(RTest& aRTest, TInt aCreationFlags)
	{
	CBioTestUtils* self = NewLC(aRTest, aCreationFlags);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CBioTestUtils* CBioTestUtils::NewLC(RTest& aRTest, TInt aCreationFlags)
	{
	CBioTestUtils* self = new(ELeave) CBioTestUtils(aRTest);
	CleanupStack::PushL(self);
	self->ConstructL(aCreationFlags);
	return self;
	}

void CBioTestUtils::Reset()
	{
	if(iTestParsedFieldArray)
		{
		iTestParsedFieldArray->ResetAndDestroy();
		delete iTestParsedFieldArray;
		iTestParsedFieldArray=NULL;
		}
	if(iMessageBody)
		{
		delete iMessageBody;
		iMessageBody = NULL;
		}
	if(iDir)
		{
		delete iDir;
		iDir = NULL;
		}
	if(iRegisteredParserDll)
		{
		delete iRegisteredParserDll;
		iRegisteredParserDll = NULL;
		}
	if (iBioClientMtm)
		{
		delete iBioClientMtm;
		iBioClientMtm=NULL;
		}
	if (iBioServerMtm)
		{
		delete iBioServerMtm;
		iBioServerMtm = NULL;
		iServerEntry = NULL;
		}

	iNumFiles = 0;
	iFilesProcessed = 0;
	iMessagesCreated = 0;

	CMsvTestUtils::Reset();
	}

//
// Register a Sms Mtm server DLL
//
EXPORT_C void CBioTestUtils::CreateBioServerMtmRegL()
	{
	CreateServerMtmRegL(KUidMsgTypeSmartMessage, _L("BIO MTM"),KBioServerMtmFileName(), KBioClientMtmFileName(), TMsvTestDllInfo(KBioUiMtmFileName,2), TMsvTestDllInfo(KBioUiMtmFileName,1), KUidMsgTypeSmartMessage, KDataComponentFileName);
	}

CBioTestUtils::CBioTestUtils(RTest& aRTest)
: CMsvTestUtils(aRTest)
	{
	iBioServiceId = KMsvNullIndexEntryId;
	}

void CBioTestUtils::ConstructL()
	{
	TInt flags=NULL; // Settings flags for the base ConstructL() function

	//	Loading the DLLs
	iRTest.Next(_L("Create Data Component FileStores"));

	CMsvTestUtils::ConstructL(flags);

	iBioDb = CBIODatabase::NewL(iFs);

	FileSession().SetSessionPath(_L("c:\\"));
	}

// Overloaded constructor - allows developer to decide what components to create
void CBioTestUtils::ConstructL(TInt aCreationFlags)
	{
	iRTest.Next(_L("Initialising Data Components"));
	// 
	CMsvTestUtils::ConstructL(aCreationFlags);

	iBioDb = CBIODatabase::NewL(iFs);
	FileSession().SetSessionPath(_L("c:\\"));
	}


void CBioTestUtils::Panic(TBioTestUtilsPanic aPanic)
	{
	Panic((TInt) aPanic);
	}

void CBioTestUtils::Panic(TInt aPanic)
	{
	User::Panic(BIOTEST_PANIC, aPanic);
	}

EXPORT_C TMsvId CBioTestUtils::CreateBIOEntryL(TDesC& aText, TBIOMessageType aMessageType )
	{
	// Ensure that we have a valid service ID to work with:
	if (iBioServiceId==0)
		iBioServiceId = SetBIOServiceIdL();

	// First take the descriptor reference and create a new descriptor to manipulate.
	// Then convert '\n' to the paragraph delimiters used in proper SMS.
	HBufC* localBuffer = aText.AllocL();
	CleanupStack::PushL(localBuffer);

	TPtr messDes = localBuffer->Des();

	if (aMessageType != EBiovCardMessage && aMessageType != EBiovCalenderMessage)
		{
		// convert \r\n to \n since this is what is expected from SMS when not vCard data
		for (TInt i=0; i<messDes.Length(); i++)
			{
			if (messDes[i]==(TText)'\r' && i<messDes.Length()-1 && messDes[i+1]==(TText)'\n')
				messDes.Delete(i,1); 
			} 
		}

	//	Create and fill a CRichText object for the jobbie:
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* richText = CRichText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(richText);

	TInt pos = richText->DocumentLength();	// Ought to be 0.
	richText->InsertL(pos, messDes);
	
	TMsvEntry newBioEntry;
	newBioEntry.SetNew(ETrue);
	newBioEntry.SetComplete(EFalse);
	newBioEntry.SetUnread(ETrue);
	newBioEntry.SetVisible(ETrue);
	newBioEntry.SetReadOnly(EFalse);
	newBioEntry.SetFailed(EFalse);
	newBioEntry.SetOperation(EFalse);
	newBioEntry.SetMultipleRecipients(EFalse);
	newBioEntry.SetAttachment(EFalse);
	newBioEntry.iMtm= KUidBIOMessageTypeMtm;
	newBioEntry.iType=KUidMsvMessageEntry;
	newBioEntry.iServiceId=iBioServiceId;
	TTime now;
	now.UniversalTime();
	newBioEntry.iDate = now;

	newBioEntry.iDescription.Set(richText->Read(0, richText->DocumentLength()));
	TBufC<KTelephoneNumberMaxLength> telNumber;
	HBufC* tempNumber = CreateDummyPhoneNumberL();
	telNumber = tempNumber->Des();
	newBioEntry.iDetails.Set(telNumber);
	// Finished with telNumber, so free up the memory:
	delete tempNumber; 

	// Change the iServiceId, iMtm, iBioType  for message:
	SetForMtmTypeL(newBioEntry, aMessageType);
	// Update the size to that of the attachment length  - correct? 
	// Otherwise just get 68
	newBioEntry.iSize = richText->DocumentLength();// msgSize;
	iClientServer == EServerSide ? CreateBioEntryServerSideL(newBioEntry,*richText) : CreateBioEntryClientSideL(newBioEntry,*richText);

	CleanupStack::PopAndDestroy(4); // richText, charFormatLayer, paraFormatLayer, localBuffer
	return newBioEntry.Id(); 
	}

void CBioTestUtils::SetForMtmTypeL(TMsvEntry& aEntry, TBIOMessageType aType)
	{
	TInt index;
	// Should have a match for the Message Type as the message is created with the SMS id's.
	switch(aType)
		{
		case ENoMessage:
			// Just to keep compiler happy!
			break;
		case EBioIapSettingsMessage:
			aEntry.iBioType  = KUidBIOInternetAccessPointMsg;
			break;
		case EBioEnpMessage:
			aEntry.iBioType =KUidBIOEmailNotificationMsg;
			break;
		case EBioRingTonesMessage:
			aEntry.iBioType =KUidBIORingingTonesMsg;
			break;
		case EBioOpLogoMessage:
			aEntry.iBioType =KUidBIOOperatorLogoMsg;
			break;
		case EBioCompBusCardMessage:
			aEntry.iBioType =KUidBIOBusinessCardMsg;
			break;
		case EBiovCardMessage:
			aEntry.iBioType =KUidBIOVCardMsg;
			break;
		case EBiovCalenderMessage:
			aEntry.iBioType =KUidBIOVCalenderMsg;
			break;
		case EBioWAPSettingsMessage:
			aEntry.iBioType =KUidBIOWAPAccessPointMsg;
			break;
		case EBioWPRVSettingsMessage:
			aEntry.iBioType =KUidBIOWPRVMsg;
			break;
		}
	// Set other TMsvEntry fields if we have a BIO message type
	if (aType != ENoMessage)
		{
		aEntry.SetMtmData2(0);
		aEntry.SetMtmData3(0);
		}

	// set description from BIF file
	iBioDb->GetBioIndexWithMsgIDL(TUid::Uid(aEntry.iBioType),index);
	if(index==KErrNotFound)
		User::Leave(KErrNotFound);
	aEntry.iDescription.Set(iBioDb->BifReader(index).Description());
	}

EXPORT_C void CBioTestUtils::LogCurrentMessageTypeL(TMsvId aMsvId)
	{
	// assume we're already pointing at a BIO message
	TMsvEntry entry;
	TMsvId oldId;
	if(iClientServer == EServerSide)
		{
		oldId = iServerEntry->Entry().Id();
		User::LeaveIfError(iServerEntry->SetEntry(aMsvId));
		entry = iServerEntry->Entry();
		}
	else
		{
		oldId = iMsvEntry->Entry().Id();
		iMsvEntry->SetEntryL(aMsvId);
		entry = iMsvEntry->Entry();
		}

	TInt biotype = entry.iBioType;
	if(biotype == KUidBIOInternetAccessPointMsg)
		{
		switch(entry.MtmData1())
			{
		case 1:
			WriteComment(_L("IACP basic IAP message"));
			break;
		case 2:
			WriteComment(_L("IACP basic mail message"));
			break;
		case 3:
			WriteComment(_L("IACP basic IAP & mail message"));
			break;
		case 7:
			WriteComment(_L("IACP Internet script message"));
			break;
		case 8:
			WriteComment(_L("IACP SMS settings message"));
			break;
		case 15:
			WriteComment(_L("Telephone settings message"));
			break;
		default:
			WriteComment(_L("Unsupported IACP type"));
			break;
			}
		}
	else if(biotype == KUidBIOEmailNotificationMsg)
			WriteComment(_L("Email notification message"));
	else if(biotype == KUidBIORingingTonesMsg)
			WriteComment(_L("Ringing tones message"));
	else if(biotype == KUidBIOOperatorLogoMsg)
			WriteComment(_L("Operator logo message"));
	else if(biotype == KUidBIOBusinessCardMsg)
			WriteComment(_L("Compact Business Card message"));
	else if(biotype == KUidBIOVCardMsg)
			WriteComment(_L("vCard message"));
	else if(biotype == KUidBIOVCalenderMsg)
			WriteComment(_L("vCalendar"));
	else if(biotype == KUidBIOWAPAccessPointMsg)
			WriteComment(_L("WAP settings message"));
	else
		WriteComment(_L("Unknown type"));

	if(iClientServer == EServerSide)
		{
		User::LeaveIfError(iServerEntry->SetEntry(oldId));
		}
	else
		{
		iMsvEntry->SetEntryL(oldId);
		}
	}

EXPORT_C TMsvId CBioTestUtils::CreateBIOEntryFromFileL(const TDesC& aFilename, TBIOMessageType aMessageType)
	{
	//First use the filename to get the message body, 
	// then convert '\n' to the paragraph delimiters used in proper SMS	
	HBufC* tempBuffer;
	TMsvId newEntryId;
	tempBuffer = ReadFromFileL(aFilename);
	CleanupStack::PushL(tempBuffer);

	newEntryId = CreateBIOEntryL(*tempBuffer, aMessageType );

	// Finished with our local descriptors - free up some memory
	CleanupStack::PopAndDestroy();  
	return newEntryId;
	}


EXPORT_C void CBioTestUtils::EmptyInboxMessagesL()
	{
	// Function gets the Inbox Entry, and then all the children of this entry.
	// Then we iterate through the selection deleting each entry.
	CMsvEntrySelection* inboxMessageSel;
	if(iClientServer == EServerSide)
		{
		inboxMessageSel = new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(inboxMessageSel);
		// Create a local Server entry, and get the children of the Global Inbox:
		User::LeaveIfError(iServerEntry->SetEntry(KMsvGlobalInBoxIndexEntryId));	
		iServerEntry->GetChildren(*inboxMessageSel ); // children of parent, ie global inbox
		if (inboxMessageSel->Count() != 0)
			{
			iServerEntry->DeleteEntries(*inboxMessageSel);
    		}
		//	Point back at something safe before we return:
		User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
		}
	else
		{
		iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
		inboxMessageSel = iMsvEntry->ChildrenL();
		CleanupStack::PushL(inboxMessageSel);
		if (inboxMessageSel->Count() != 0)
			{
			TInt count;
			TMsvId currentMsgId;
			for (count = 0; count < inboxMessageSel->Count(); count++)
				{
				currentMsgId= inboxMessageSel->At(count);
				// Delete this entry
				DeleteLocalEntrySynchronouslyL(*iMsvEntry, currentMsgId);
				}
    		}
		iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
		}
	CleanupStack::PopAndDestroy();	// inboxMessageSel
	}

void CBioTestUtils::DeleteLocalEntrySynchronouslyL(CMsvEntry& aParentEntry, TMsvId anId)
	{ 
	//  Creates an entry synchronously by handling the asynchrous parts locally
 
	const TMsvSelectionOrdering& originalSort=aParentEntry.SortType();
	TMsvSelectionOrdering sort(originalSort);
	sort.SetShowInvisibleEntries(ETrue);
	aParentEntry.SetSortTypeL(sort);
	CMsvOperationWait* myDog=CMsvOperationWait::NewLC();
	CMsvOperation* op=aParentEntry.DeleteL(anId, myDog->iStatus);
	myDog->Start();
	CActiveScheduler::Start();
	const TInt err=op->iStatus.Int();
	TMsvLocalOperationProgress progress=McliUtils::GetLocalProgressL(*op);
	delete op;
	User::LeaveIfError(err);
	User::LeaveIfError(progress.iError);
	CleanupStack::PopAndDestroy();
	aParentEntry.SetSortTypeL(originalSort);
	}
//*************************************************************************
//
//		Getting BIO service ID
//
//*************************************************************************

// Get,or create, the default BIO service:
EXPORT_C TMsvId CBioTestUtils::SetBIOServiceIdL()
	{
	// If Bio service Id is none zero its already been set - just return the value:
	if (iBioServiceId)
		return iBioServiceId;

	if(iClientServer == EServerSide)
		SetBioServiceServerSideL();
	else
		SetBioServiceClientSideL();
	return iBioServiceId;
	}

void CBioTestUtils::SetBioServiceServerSideL()
	{
	// Set up the RootEntry sort order first:
	TMsvSelectionOrdering oldSortType = iServerEntry->Sort();
	TMsvSelectionOrdering sortType(KMsvGroupByType, EMsvSortByNone, ETrue);	
	iServerEntry->SetSort(sortType);

	// Search the root folder for any entries which are BIO Message services.
	// If none are found then create one.
	TMsvId localServiceId=KMsvNullIndexEntryId;
	localServiceId = DefaultServiceForMTML(KUidBIOMessageTypeMtm, ETrue);

	// Check that the Service id is valid, i.e. set iCurrentEntry context to service entry:
	TRAPD( error, iServerEntry->SetEntry(localServiceId) );
	if ( (localServiceId == KMsvUnknownServiceIndexEntryId)  || (error == KErrNotFound) )
		{
		// Id isn't a valid BIO service, so try and get the first BIO service:
		CMsvEntrySelection* sel = GetListOfAccountsWithMTMServerL(KUidBIOMessageTypeMtm);
		CleanupStack::PushL(sel);
		if (sel->Count())
			iBioServiceId=(*sel)[0];	// Select the first available service.
		CleanupStack::PopAndDestroy();	// sel
		if (localServiceId == KMsvUnknownServiceIndexEntryId || localServiceId == KMsvNullIndexEntryId)
			// Haven't found any valid BIO service, so try and create a BIO service:
			localServiceId=CreateDefaultBIOServiceL();
		}
		else
			User::LeaveIfError(error);  // An error other than no service occurred.

	__ASSERT_DEBUG( localServiceId != KMsvNullIndexEntryId, User::Panic(_L("BioGen"), KErrNotFound));

	//	Point back at something safe:
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
	iServerEntry->SetSort(oldSortType);

	iBioServiceId = localServiceId;
	}

void CBioTestUtils::SetBioServiceClientSideL()
	{
	iMsvEntry->SetSortTypeL(TMsvSelectionOrdering(KMsvGroupByType, EMsvSortByNone, ETrue));
#if defined (_DEBUG)
	if (iMsvEntry->EntryId() != KMsvRootIndexEntryId)  //iMsvEntry should be set to root
		User::Leave(KErrNotFound);	//replaces a panic - ENotAServiceEntry
#endif

	TMsvId localServiceId=KMsvNullIndexEntryId;
	localServiceId=DefaultServiceForMTML(KUidBIOMessageTypeMtm, ETrue);

	//Check if there is a valid entry for this ID
	TRAPD(ret, /*const TMsvEntry& ent=*/iMsvEntry->ChildDataL(localServiceId));

	if (localServiceId == KMsvUnknownServiceIndexEntryId || ret==KErrNotFound)
		{
		// Our Id isn't a valid BIO service, possibly because there isn't a default 
		// BIO service.  So try and get the first BIO service.
		CMsvEntrySelection* sel=GetListOfAccountsWithMTMClientL(KUidBIOMessageTypeMtm);
		if (sel->Count())
			localServiceId=(*sel)[0]; // select the first available service
		delete sel;
		if (localServiceId == KMsvUnknownServiceIndexEntryId || localServiceId == KMsvNullIndexEntryId)
			// No SMS service present, so let's create one 
			CreateServicesL();
		}
	else
		User::LeaveIfError(ret);  // an error other than no service occurred

#if defined (_DEBUG)
	if (localServiceId == KMsvNullIndexEntryId)
		User::Leave(KErrNotFound);	//replaces a panic - ENotAServiceEntry
#endif
	iBioServiceId = localServiceId;
	}

TMsvId CBioTestUtils::CreateDefaultBIOServiceL() 
	{
	//	Haven't found an entry so create a BIO Message service:
	TMsvEntry bioServiceEntry;
	bioServiceEntry.iMtm = KUidBIOMessageTypeMtm;
	bioServiceEntry.iType = KUidMsvServiceEntry;
	bioServiceEntry.SetVisible(EFalse);
	bioServiceEntry.iDate.UniversalTime();
	bioServiceEntry.iDescription.Set(_L("BIO Message Service "));	// Is there such a thing?
	bioServiceEntry.iDetails.Set(_L("BIO Message Service"));

//	no need to set default service for v2, CBaseMtm 'knows' that for services 
//	where there is a single service entry, it has to be the default
	TMsvId newBIOServiceId;
	if(iClientServer == EServerSide)
		{
		User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
		User::LeaveIfError(iServerEntry->CreateEntry(bioServiceEntry)); // Needs to be a child of the root!
		newBIOServiceId =bioServiceEntry.Id();

		}
	else
		{
		iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
		iMsvEntry->CreateL(bioServiceEntry); // Needs to be a child of the root!
		newBIOServiceId =bioServiceEntry.Id();

		}

	// No need to set Server Entry - still pointing at the root.
	return newBIOServiceId;
	}


TMsvId CBioTestUtils::DefaultServiceForMTML(TUid aMtmUid, TBool aFindFirstServiceIfNoDefault)
	{
	TMsvId serviceId=KMsvUnknownServiceIndexEntryId;
	if(iClientServer == EServerSide)
		serviceId = DefaultServiceServerL(aMtmUid,aFindFirstServiceIfNoDefault);
	else
		serviceId = DefaultServiceClientL(aMtmUid,aFindFirstServiceIfNoDefault);
	return serviceId;
	}

TMsvId CBioTestUtils::DefaultServiceServerL(TUid aMtmUid, TBool /*aFindFirstServiceIfNoDefault*/)
	{
	TMsvId serviceId=KMsvUnknownServiceIndexEntryId;
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));

		// Find first available service with the desired iMtm value:
		CMsvEntrySelection* sel=new (ELeave) CMsvEntrySelection;
		CleanupStack::PushL(sel);

		iServerEntry->GetChildren(*sel);
		
		for (TInt count = 0; serviceId != 0 && count < sel->Count(); count++)
			{
			User::LeaveIfError(iServerEntry->SetEntry(sel->At(count)));
			if (iServerEntry->Entry().iMtm == aMtmUid)
				serviceId = sel->At(count);
			}
		CleanupStack::PopAndDestroy(); //  sel

	//	Point back at something safe:
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));

	return serviceId;
	}

TMsvId CBioTestUtils::DefaultServiceClientL(TUid /*aMtmUid*/, TBool /*aFindFirstServiceIfNoDefault*/)
	{
	if (!iBioClientMtm)
		{
		InstantiateClientMtmsL();
		}
	return iBioClientMtm->DefaultServiceL();
	}

CMsvEntrySelection* CBioTestUtils::GetListOfAccountsWithMTMServerL(TUid aMtmUid)
	{
	// Gets a list of entries with the passed in MTM.
	CMsvEntrySelection*	 returnSelection =new(ELeave) CMsvEntrySelection();
	CleanupStack::PushL(returnSelection);

	CMsvEntrySelection*	 rootKin = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(rootKin);

	CMsvEntrySelection*	 entryKin = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(entryKin);
	
	TMsvSelectionOrdering  oldSortType = iServerEntry->Sort();
	TMsvSelectionOrdering	sortType(KMsvGroupByType|KMsvGroupByStandardFolders, EMsvSortByDetailsReverse, ETrue);

	// iCurrentEntry should be pointing towards the root, but make sure:
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryIdValue));
	iServerEntry->SetSort(sortType);
	iServerEntry->GetChildren(*rootKin);  // Get the child entries of the root.

	TBool visible=EFalse;
	TInt err;
	if ( rootKin->Count() !=0)
		{
		TMsvEntry tentry;
		for (TInt cc=rootKin->Count(); --cc>=0; ) // N.B. decrements prior to entering loop.
			{
			// Set the context of our Server Entry to the next child entry of the root:
			User::LeaveIfError(iServerEntry->SetEntry(rootKin->At(cc)) );
			// Get the entry and check its mtm type:
			tentry =iServerEntry->Entry();   
			if (tentry.iMtm==aMtmUid)
				{
				// Is it a proper service entry?
				if (tentry.iType.iUid==KUidMsvServiceEntryValue && tentry.Id()!=KMsvLocalServiceIndexEntryIdValue)
					{
					// If it's visible or it has no entry in the index - add to our return array:
					if (tentry.Visible() || tentry.iRelatedId==KMsvNullIndexEntryId)
						returnSelection->AppendL(tentry.Id());
					else if (tentry.iRelatedId!=tentry.Id())
						{
						// Check if there are any related entries:
						TRAP(err, iServerEntry->GetChildren(*entryKin) );
						TInt count = entryKin->Count();
						while (count--)
							{
							User::LeaveIfError(iServerEntry->SetEntry( entryKin->At(count) ) );
							if (iServerEntry->Entry().Id() == tentry.iRelatedId)
								break;
							}
						visible=iServerEntry->Entry().Visible();
						if (err==KErrNone && !visible && returnSelection->Find(tentry.iRelatedId)!=KErrNone)
							returnSelection->AppendL(tentry.Id());
						}
					}
				}
			}
		}		

	//	Point back at something safe:
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
	iServerEntry->SetSort(oldSortType);
	CleanupStack::PopAndDestroy(2); // entryKin, rootKin, 	
	CleanupStack::Pop(); // returnSelection 

	return returnSelection;
	}

CMsvEntrySelection* CBioTestUtils::GetListOfAccountsWithMTMClientL(TUid aMtmUid)
	{
	// Gets a list of entries with the passed in MTM
	CMsvEntrySelection*	 returnSelection =new(ELeave) CMsvEntrySelection();
	CleanupStack::PushL(returnSelection);

	CMsvEntrySelection*	 rootKin;
	CMsvEntrySelection*	 entryKin;

	TMsvSelectionOrdering  oldSortType = iMsvEntry->SortType();
	TMsvSelectionOrdering	sortType(KMsvGroupByType|KMsvGroupByStandardFolders, EMsvSortByDetailsReverse, ETrue);

	// iCurrentEntry should be pointing towards the root, but make sure
	iMsvEntry->SetEntryL(KMsvRootIndexEntryIdValue);
	iMsvEntry->SetSortTypeL(sortType);
	rootKin=iMsvEntry->ChildrenL();  // Get the child entries of the root
	CleanupStack::PushL(rootKin);

	TBool visible=EFalse;

	if ( rootKin->Count() !=0)
		{
		TMsvEntry tentry;
		for (TInt cc=rootKin->Count(); --cc>=0; ) // NB decrements prior to entering loop
			{
			// Set the context of our Server Entry to the next child entry of the root
			iMsvEntry->SetEntryL(rootKin->At(cc)) ;
			// get the entry and check it's mtm type
			tentry =iMsvEntry->Entry();   
			if (tentry.iMtm==aMtmUid)
				{
				// Is it a proper service entry
				if (tentry.iType.iUid==KUidMsvServiceEntryValue && tentry.Id()!=KMsvLocalServiceIndexEntryIdValue)
					{
					// if it's visible or it has no entry in the index - add to our return array
					if (tentry.Visible() || tentry.iRelatedId==KMsvNullIndexEntryId)
						returnSelection->AppendL(tentry.Id());
					else if (tentry.iRelatedId!=tentry.Id())
						{
						// Check if there are any related entries
						entryKin=iMsvEntry->ChildrenL() ;
						CleanupStack::PushL(entryKin);

						TInt count = entryKin->Count();
						while (count--)
							{
							iMsvEntry->SetEntryL( entryKin->At(count) );
							if (iMsvEntry->Entry().Id() == tentry.iRelatedId)
								break;
							}
						visible=iMsvEntry->Entry().Visible();
						if (!visible && returnSelection->Find(tentry.iRelatedId)!=KErrNone)
							returnSelection->AppendL(tentry.Id());
						CleanupStack::PopAndDestroy(); // entryKin 	
						}
					}
				}
			}
		}		

	//	Point back at something safe.....
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
	iMsvEntry->SetSortTypeL(oldSortType);
	CleanupStack::PopAndDestroy(); // entryKin, rootKin, 	
	CleanupStack::Pop(); //returnSelection 

	return returnSelection;
	}


//*****************************************************************************
//
// File Read and Message Creation functions
//
//*****************************************************************************
EXPORT_C void CBioTestUtils::SetSessionPath(const TDesC& aSessionPath)
	{
    iFs.SetSessionPath(aSessionPath);
	}

HBufC* CBioTestUtils::ReadFromFileL(const TDesC& aFile)
	{
	// Reads in data from a file and loads it into a buffer which is later used
	// to create an SMS message in the  Inbox.  This function differs from the others
	// in that it also gets any From: field by locating the token in the file. 
	// NOTE: There is an inherent assumption that the From: token and the field will
	// be on a single line in the text file (i.e. less than 1024 characters.
	RFile           file;
	TBuf8<1024>     lineBuffer;   // Buffer for Read function.
	TInt			err=KErrNone;

	err = file.Open(iFs,aFile,EFileStreamText|EFileRead|EFileShareAny);

    if(err != KErrNone)  // Didn't find the file, so leave - should only get valid filenames!
        {
        User::Leave(KErrNotFound);
		}

	// In real life this is set by the Socket Port Observer, or the SMS server.
	// So,in this test code we must do it by creating a random  telephone number.
	//	iMessageDetails = CreateDummyPhoneNumberL();

    HBufC*	bioBuf=HBufC::NewLC(65535); // Create a new descriptor on the heap.
	HBufC*	copyBuffer=HBufC::NewLC(1024);

    do // Read in the text from file, and also check if there is a name field:
		{
		err = file.Read(lineBuffer);// Read upto 256 chars, '\n' and all...
		//err = ReadLineL(file,lineBuffer);
		if (err==KErrNone)				// Made a valid read,
			if (lineBuffer.Length()==0) // but read 0 chars
				err=KErrEof;			// so set err value to end processing

		if(err == KErrNone)
			{
			copyBuffer->Des().Copy(lineBuffer);// Copy, and overwrite existing text
			if( (bioBuf->Length() + copyBuffer->Length()) > bioBuf->Des().MaxLength() )
					{
					bioBuf = bioBuf->ReAllocL(bioBuf->Length() + copyBuffer->Length());
					}
			bioBuf->Des().Append(*copyBuffer);
            //bioBuf->Des().Append(_L("\n"));
			}
		}
    while(err != KErrEof);

	CleanupStack::PopAndDestroy(); // Destroy the copyBuffer.
	CleanupStack::Pop();// Remove the bioBuf.
    
	file.Close();
	return bioBuf;  // Transfer ownership and responsibility of the buffer.
	}



//*****************************************************************************
//
// Creating a dummy phone number for use in messages
//
//*****************************************************************************



HBufC* CBioTestUtils::CreateDummyPhoneNumberL()
	{
	HBufC* number= HBufC::NewL(32); // Note: function doesn't own this, up to the caller to delete
	TInt64 phoneNo;
	TInt64 seed;
	TTime tempTime;
	tempTime.UniversalTime();
	seed=tempTime.Int64();
	phoneNo= Math::Rand(seed);
	if (phoneNo <0)
		phoneNo = -phoneNo;
	number->Des().Num( phoneNo); 
	number->Des().Insert(0,_L("+44"));// Add a leading '+' to make if an international numbers
	return number;
	}


void CBioTestUtils::CreateBioEntryServerSideL(TMsvEntry& aEntry, CRichText& aBody)
	{
	//	Get the global inbox:
	User::LeaveIfError(iServerEntry->SetEntry(KMsvGlobalInBoxIndexEntryId));
	// Entry Context already set, create our entry:
	User::LeaveIfError(iServerEntry->CreateEntry(aEntry)); 
	// Set our context to the the new entry so we can create a store:
	User::LeaveIfError(iServerEntry->SetEntry(aEntry.Id()) );
	// Save all the changes:
	CMsvStore* store;
	store=iServerEntry->EditStoreL();
	CleanupStack::PushL(store);

	if (store->HasBodyTextL())
		{
		store->DeleteBodyTextL();
		}
	store->StoreBodyTextL(aBody);
	store->CommitL();
	CleanupStack::PopAndDestroy(); // Close and destroy the store.
	// Set context to the new entry to allow updating of  the entry:
	aEntry.SetComplete(ETrue);
	iServerEntry->ChangeEntry(aEntry);
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
	}

void CBioTestUtils::CreateBioEntryClientSideL(TMsvEntry& aEntry, CRichText& aBody)
	{
		//	Get the global inbox.	
	iMsvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	iMsvEntry->CreateL(aEntry); 
	iMsvEntry->SetEntryL(aEntry.Id());

	// Save all the changes
	CMsvStore* store;
	store=iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);

	if (store->HasBodyTextL())
		{
		store->DeleteBodyTextL();
		}
	store->StoreBodyTextL(aBody);
	store->CommitL();

	CleanupStack::PopAndDestroy(); //store - close the store

	aEntry.SetComplete(ETrue);
	// Update the entry
	iMsvEntry->ChangeL(aEntry);
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
	}

void CBioTestUtils::InstantiateClientMtmsL()
	{
	// client side
	__ASSERT_ALWAYS(iMsvSession, User::Panic(BIOTEST_PANIC, KErrBIONotOnClientSide));
	if(iClientMtmRegistry)
		delete iClientMtmRegistry;
	iClientMtmRegistry = CClientMtmRegistry::NewL(*iMsvSession);
	iBioClientMtm = (CBIOClientMtm*) iClientMtmRegistry->NewMtmL(KUidMsgTypeSmartMessage);
	}

void CBioTestUtils::InstantiateServerMtmsL()
	{
	iBioServerMtm = (CBIOServerMtm*) InstantiateServerMtmL(KUidBIOMessageTypeMtm, iBioServiceId);
	const CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection();
	TRequestStatus st;
	TMsvId s =0;
	TRAP_IGNORE(iBioServerMtm->CopyFromLocalL(*selection,s, st));
	TRAP_IGNORE(iBioServerMtm->CopyToLocalL(*selection,s, st));
	TRAP_IGNORE(iBioServerMtm->CopyWithinServiceL(*selection,s, st));
	TRAP_IGNORE(iBioServerMtm->MoveFromLocalL(*selection,s, st));
	TRAP_IGNORE(iBioServerMtm->MoveToLocalL(*selection,s, st));
	TRAP_IGNORE(iBioServerMtm->MoveWithinServiceL(*selection,s, st));
	TRAP_IGNORE(iBioServerMtm->DeleteAllL(*selection,st));
	
	TInt intw =0;
		
	TRAP_IGNORE(iBioServerMtm->Cancel());
	iBioServerMtm->CommandExpected();
	TMsvEntry entry;
	
	TRAP_IGNORE(iBioServerMtm->ChangeL(entry,st));
	TRAP_IGNORE(iBioServerMtm->CreateL(entry,st));
	delete selection;
	}

void CBioTestUtils::DeleteServicesL()
	{
	DeleteServiceL(KUidBIOMessageTypeMtm);
	}

void CBioTestUtils::CreateServicesL()
	{
	iBioServiceId = CreateServiceL(KUidMsgTypeSmartMessage);
	}

void CBioTestUtils::FindExistingServicesL()
	{
	iBioServiceId = 0;
	TRAPD(err,ServiceIdL(KUidMsgTypeSmartMessage, iBioServiceId));
	if (err)
		Printf(_L("No existing SMS services found!\n"));
	}

void CBioTestUtils::InstallMtmGroupsL()
	{
	InstallMtmGroupL(KDataComponentFileName);
	}

void CBioTestUtils::CreateServerMtmRegsL()
	{
	CreateBioServerMtmRegL();
	}

//
// stuff to generate test BIO messages
//

EXPORT_C CMsvEntrySelection* CBioTestUtils::GenerateMessagesL()
	{
	return GenerateMessagesL(KBIOTxtFilePath);
	}

EXPORT_C CMsvEntrySelection*  CBioTestUtils::GenerateMessagesL(const TDesC& aFileDirectory, TBool aLogCreationDetails)
	{
	TBufC<KMaxFileName> currentFile;
	TMsvId messageId;
	TBIOMessageType currentMsgType;

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection();
	CleanupStack::PushL(selection);

	TInt err = iFs.GetDir(aFileDirectory, KEntryAttMatchMask, ESortByName, iDir);
	if (err == KErrPathNotFound)
		{
		TInt makeDirErr = iFs.MkDirAll(aFileDirectory);	
		makeDirErr==KErrNone ? User::Leave(KErrNotFound): User::Leave(makeDirErr);
		}
	else if (err!=KErrNone)
		{
		User::Leave(err);
		}

	// Set the session path for the RFs
	SetSessionPath(aFileDirectory);
	if (iDir) 
		delete iDir;
	User::LeaveIfError(iFs.GetDir(_L("*.txt"), KEntryAttNormal, ESortByName, iDir));
    TInt count = iDir->Count();
    if(count == 0)
        {
        User::Leave(KErrNotFound);   // No files to process
        }

	TBuf<60> outputBuf;

	for(TInt loop = 0; loop < count; loop++)
		{
		currentFile=( (*iDir)[iFilesProcessed].iName );
		// Not processed all the messages - so keep the current state
		iFilesProcessed++;   // Here because need to update the counter promptly
		currentMsgType = SetMessageType(currentFile);
		if (currentMsgType!=ENoMessage) // skip any dodgy filenames
			{
			messageId = CreateBIOEntryFromFileL( currentFile,currentMsgType);
			selection->AppendL(messageId);
			TPtrC tempPtr = (currentFile.Des() );
			if (aLogCreationDetails)
				{
				outputBuf.Format(_L("TMsvId  %d   -    %S"), messageId, &tempPtr);
				WriteComment(outputBuf);
				}
			}
		}
	CleanupStack::Pop(); // selection
	return selection;
    }

EXPORT_C TBIOMessageType CBioTestUtils::SetMessageType(const TDesC& aFileName)
	{

	// Check each file prefix in turn with the filename passed as a parameter. 
	// If cannot find the correct type set to ENoMessage to indicate an error. 
	// GenerateNextMessage will then skip the file!

	if (aFileName.MatchF(KBIOIapPrefix)==0)  // File starts with "iap"
		{
		return EBioIapSettingsMessage;
		}
	if(aFileName.MatchF(KBIOEnpPrefix)==0) // File name starts "enp"
		{
		return EBioEnpMessage;
		}
	if (aFileName.MatchF(KBIORingTonePrefix)==0)//Filename begins "rtone"
		{
		return EBioRingTonesMessage;
		}
	if (aFileName.MatchF(KBIOOpLogoPrefix)==0)	// Filename begins "oplogo"
		{
		return EBioOpLogoMessage;
		}
	if (aFileName.MatchF(KBIOcBusinessCardPrefix)==0)// Filename begins "cbc"
		{
		return EBioCompBusCardMessage;
		}
	if (aFileName.MatchF(KBIOvCardPrefix)==0)
		{
		return EBiovCardMessage;
		}
	if (aFileName.MatchF(KBIOvCalenderPrefix)==0)
		{
		return EBiovCalenderMessage;
		}
	if (aFileName.MatchF(KBIOWappPrefix)==0)
		{
		return EBioWAPSettingsMessage;
		}
	// if we've reached this point it's an unknown filename 
	return ENoMessage;
	}

EXPORT_C void CBioTestUtils::DoAppendVariantName(TDes& aFileName)
	{
#if defined(__WINS__) && defined(__WINSCW__)
	aFileName.Append(_L(".WINSCW."));
#elif(__WINS__)
	aFileName.Append(_L(".WINS."));
#endif
#if (defined(__THUMB__) || defined(__MARM_THUMB__))
	aFileName.Append(_L(".THUMB."));
#endif
#if (defined(__ARMI__) || defined(__MARM_ARMI__))
	aFileName.Append(_L(".ARMI."));
#endif
#if (defined(__ARM4__) || defined(__MISA__) || defined(__MARM_ARM4__))
	aFileName.Append(_L(".ARM4."));
#endif

#if defined(_DEBUG)
	aFileName.Append(_L("DEB."));
#else
	aFileName.Append(_L("REL."));
#endif

	aFileName.Append(_L("LOG"));
	}

//
// Create a return the appropriate parser for the biomsg with id 
//
EXPORT_C CBaseScriptParser2* CBioTestUtils::CreateParserL(TMsvId aBioMsgId)
	{
	TMsvEntry myBioEntry;

	if(iClientServer==EClientSide)
		{
		iMsvEntry->SetEntryL(aBioMsgId);
		myBioEntry = iMsvEntry->Entry();
		}
	else
		{
		User::Leave(KErrNotSupported);
		}

	// check its the right type
	if(myBioEntry.iMtm.iUid != KUidMsgTypeSmartMessage.iUid)
		User::Leave(KBspInvalidMessage);

	return CreateParserTypeL(myBioEntry.iBioType);
	}

EXPORT_C CBaseScriptParser2* CBioTestUtils::CreateParserTypeL(TInt32 aBioMsgType)
	{
	// get the bio type and create the parser
	TUid messageUid = TUid::Uid(aBioMsgType);

	// look up identification for parser
	TFileName parserDllName(iBioDb->GetBioParserNameL(messageUid));

	if (iRegisteredParserDll)  // Already have an old RegParserDll, delete it to stop memory leak.
		{
		delete iRegisteredParserDll;
		iRegisteredParserDll = NULL;
		}

    iRegisteredParserDll = CRegisteredParserDll::NewL(parserDllName); // Create a new RegParserDll

    RLibrary parserlibrary;
    User::LeaveIfError(iRegisteredParserDll->GetLibrary(iFs, parserlibrary));

    typedef CBaseScriptParser2* (*NewParserL)(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);

    TInt entrypointordinalnumber=1; // The one and only entry point
    TLibraryFunction libFunc=parserlibrary.Lookup(entrypointordinalnumber);
    if (libFunc==NULL)
        User::Leave(KErrBadLibraryEntryPoint);
    NewParserL pFunc=(NewParserL) libFunc;
    TInt refcount=iRegisteredParserDll->DllRefCount();
    CBaseScriptParser2* parser=NULL;
    TRAPD(ret,parser=((*pFunc)(*iRegisteredParserDll, *iMsvEntry, iFs)));
    if ((ret!=KErrNone) && (iRegisteredParserDll->DllRefCount()==refcount))
        iRegisteredParserDll->ReleaseLibrary();

	User::LeaveIfError(ret);
    return parser;
	}

EXPORT_C TPtrC CBioTestUtils::MessageBodyL(TMsvId aBioMsgId)
	{
	CMsvStore* store = NULL;
	if(iClientServer==EServerSide)
		{
		User::LeaveIfError(iServerEntry->SetEntry(aBioMsgId));
		if(iServerEntry->Entry().iMtm.iUid != KUidMsgTypeSmartMessage.iUid)
			User::Leave(KBspInvalidMessage);
		store =  iServerEntry->ReadStoreL();
		}
	else
		{
		iMsvEntry->SetEntryL(aBioMsgId);
		if(iMsvEntry->Entry().iMtm.iUid != KUidMsgTypeSmartMessage.iUid)
			User::Leave(KBspInvalidMessage);
		store = iMsvEntry->ReadStoreL();
		}
    //  Extract the body text or leave false if not
	CleanupStack::PushL(store);
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
    CleanupStack::PushL(paraFormatLayer);
    CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
    CleanupStack::PushL(charFormatLayer);
    CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
    CleanupStack::PushL(richText);

	store->HasBodyTextL() ? store->RestoreBodyTextL(*richText) : User::Leave( KErrNotFound);
	
	TInt messageLength = richText->DocumentLength();

	delete iMessageBody;
	iMessageBody = HBufC::NewL(messageLength);
	TPtr messDes = iMessageBody->Des();

	TInt length = messDes.Length();
	while(length < messageLength)
		{
		TPtrC desc = richText->Read(length, messageLength-length);
		messDes.Append(desc);
		length+=desc.Length();
		}

	CleanupStack::PopAndDestroy(4);	// store, text, charFormatLayer, paraFormatLayer 
	return messDes;
	}

EXPORT_C void CBioTestUtils::LogExtractedFieldsL(TMsvId aMessage)
	{
	// make sure we've got the right entry
	CMsvStore* store = NULL;
	if(iClientServer==EServerSide)
		{
		User::LeaveIfError(iServerEntry->SetEntry(aMessage));
		if(iServerEntry->Entry().iMtm.iUid != KUidMsgTypeSmartMessage.iUid)
			User::Leave(KBspInvalidMessage);
		store =  iServerEntry->ReadStoreL();
		}
	else
		{
		iMsvEntry->SetEntryL(aMessage);
		if(iMsvEntry->Entry().iMtm.iUid != KUidMsgTypeSmartMessage.iUid)
			User::Leave(KBspInvalidMessage);
		store = iMsvEntry->ReadStoreL();
		}
    //  Extract the body text or leave false if not
	CleanupStack::PushL(store);

	// try to get our array of parsed fields
	RMsvReadStream in;
	in.OpenLC( *store, KUidMsvBIODataStream );
	InternalizeL(in);
	CleanupStack::PopAndDestroy();

	HBufC* fieldBuf  = NULL;
	TInt length = 0;
	WriteComment(_L("Extracted fields"));
	for(TInt loop = 0; loop < iTestParsedFieldArray->Count(); loop++)
		{
		CParsedField& field = *(*iTestParsedFieldArray)[loop];
		length = (field.FieldName().Length() + field.FieldValue().Length() + 1);

		delete fieldBuf;
		fieldBuf = HBufC::NewL(length);
		TPtr des = fieldBuf->Des();
		des.Append(field.FieldName());
		des.Append(KCharSpace);
		des.Append(field.FieldValue());
		
		//FieldValue may be bigger than '0x100' i.e logging script
		//this may exeede max buffer, so we need to break it into chuncks
		//of 64 chars and log the chuncks in sequence.
		TInt desLength = des.Length();
		TInt BufSize=64;
		if (desLength > BufSize)
			{
			TBuf<64> tempBuf;
			TInt count=0;
			while(count <= desLength)
				{
				if(desLength-count > BufSize )
					tempBuf= des.Mid(count,BufSize);
				else
					tempBuf= des.Mid(count,desLength-count);

				WriteComment(tempBuf);
				count+=BufSize;
				}
			}
		else
			WriteComment(des);

		}
	delete fieldBuf;
	CleanupStack::PopAndDestroy();
	}


void CBioTestUtils::InternalizeL(RMsvReadStream& aReadStream)
	{
	if(iTestParsedFieldArray)
		{
		iTestParsedFieldArray->ResetAndDestroy();
		delete iTestParsedFieldArray;
		iTestParsedFieldArray=NULL;
		}

	iTestParsedFieldArray = new(ELeave) CArrayPtrSeg<CParsedField>(16);

	CParsedField* parsedField = NULL;
	TInt count = aReadStream.ReadUint8L();
	for (TInt i=0; i < count; i++)
		{
		parsedField = new (ELeave) CParsedField();
		TRAPD(err, parsedField->InternalizeL(aReadStream))
		if(err)
			{
			delete parsedField; //deletes the last allocated object, privious ones will be deleted by iParsedFieldArray->ResetAndDestroy()
			User::Leave(err);
			}
		iTestParsedFieldArray->AppendL(parsedField);
		}
	}

EXPORT_C CArrayPtrSeg<CParsedField>& CBioTestUtils::ParsedFieldArray()
	{
	return *iTestParsedFieldArray;
	}
