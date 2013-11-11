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
// __ACTION_INFO_BEGIN__
// [Action Name]
// CompareSmsMessages
// [Action Parameters]
// TMsvId		FolderId1   <input>             : Value of Folder Id to be compared with.
// TMsvId		FolderId2   <input>             : Value of Folder Id to be compared
// CMsvSession  session     <input>             : Reference to the session.
// TInt         Result      <output-initiation> : Value of enumerated constants if not
// identical(zero if one folder has
// corresponding same message in other folder).
// [Action Description]
// Compare and check the whether the messages in one folder has corresponding same message
// in other folder. The output of the Test action should be zero if all parts of
// the message in both folders are identical, otherwise the corresponding
// enumerated value of the non-identical part of the message.
// [APIs Used]
// CMsvEntrySelection::ChildrenL
// CMsvEntrySelection::Count
// CMsvSession::GetEntryL
// CMsvEntry::Entry
// CMsvStore::RestoreBodyTextL
// CSmsHeader::RestoreL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

// system includes
#include <smut.h>
#include <msvapi.h>
#include <txtrich.h>
#include <smuthdr.h>


// user includes
#include "CmtfTestActionCompareSmsMessages.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionCompareSmsMessages::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareSmsMessages* self = new (ELeave) CMtfTestActionCompareSmsMessages(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}


CMtfTestActionCompareSmsMessages::CMtfTestActionCompareSmsMessages(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCompareSmsMessages::~CMtfTestActionCompareSmsMessages()
	{
	}

void CMtfTestActionCompareSmsMessages::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareSmsMessages);
	TMsvId       paramFolderId1 = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	TMsvId       paramFolderId2 = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	iParamSession   = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* folderEntry1 = CMsvEntry::NewL(*iParamSession,paramFolderId1,TMsvSelectionOrdering());
	CleanupStack::PushL(folderEntry1);
	CMsvEntrySelection* folder1Selection =folderEntry1->ChildrenWithMtmL(KUidMsgTypeSMS);
	CleanupStack::PushL(folder1Selection);
	folderEntry1->SetEntryL(paramFolderId2);
	CMsvEntrySelection* folder2Selection =folderEntry1->ChildrenWithMtmL(KUidMsgTypeSMS);
    CleanupStack::PushL(folder2Selection);
 	TInt count1 = folder1Selection->Count();
	TInt resCount=0;
	TInt enumVal;
	TInt folder1Count=count1;
	TInt count2 = folder2Selection->Count();
	if((count1 <= 0)||(count2 < count1))
		{
		User::Leave(KErrGeneral);
		}
	TMsvId msgId1 = KMsvRootIndexEntryId;
	CMsvEntry* f1Entry = iParamSession->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(f1Entry);
	TMsvId msgId2 = KMsvRootIndexEntryId;
	CMsvEntry* f2Entry = iParamSession->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(f2Entry);

	while (count1--)
		{
		msgId1 = (*folder1Selection)[count1];
		f1Entry->SetEntryL(msgId1);
		TMsvEntry msgEntry1 = f1Entry->Entry();
		while (count2--)
			{
			msgId2 = (*folder2Selection)[count2];
			f2Entry->SetEntryL(msgId2);
			TMsvEntry msgEntry2 = f2Entry->Entry();

			if (IsMatchHeaderL(*f1Entry, *f2Entry))
				{
				enumVal = EFailedToMatchHeader;
				}
			else if (!IsMatchL(*f1Entry, *f2Entry))
				{
				enumVal = EFailedToMatchBody;
				}
			else
				{
				resCount++;
				if(resCount == folder1Count)
					{
					enumVal = EAllMatched;
					}
				break;
				}
			}
		}
	StoreParameterL<TInt>(TestCase(),enumVal,ActionParameters().Parameter(3));
	CleanupStack::PopAndDestroy(f2Entry);
	CleanupStack::PopAndDestroy(f1Entry);
	CleanupStack::PopAndDestroy(folder2Selection);
	CleanupStack::PopAndDestroy(folder1Selection);
	CleanupStack::PopAndDestroy(folderEntry1);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareSmsMessages);
	TestCase().ActionCompletedL(*this);
	}



  TBool CMtfTestActionCompareSmsMessages::IsMatchL(CMsvEntry& aFolder1Msg, CMsvEntry& aFolder2Msg)
/**
 * Compares the body text of Folder1 and Folder2
 *
 * @return Returns ETrue if the body text matches
 */
  {
	HBufC* folder1MsgBody = GetBodyTextL(aFolder1Msg);
	HBufC* folder2MsgBody = GetBodyTextL(aFolder2Msg);
	TBool retVal = (*folder1MsgBody == *folder2MsgBody);
	return retVal;
	}

  TBool CMtfTestActionCompareSmsMessages::IsMatchHeaderL(CMsvEntry& aFolder1Msg, CMsvEntry& aFolder2Msg)
/**
 * Compares the Message Header of  Folder1 and Folder2
 *
 * @return Returns ETrue if the body text matches
 */
  {
	CSmsHeader* folder1MsgHeader = GetSubmitHeaderL(aFolder1Msg);
	CleanupStack::PushL(folder1MsgHeader);
	CSmsHeader* folder2MsgHeader = GetDeliverHeaderL(aFolder2Msg);
	CleanupStack::PushL(folder2MsgHeader);
	
	CArrayPtrFlat<CSmsNumber>& f1Recipients = folder1MsgHeader->Recipients();
	CArrayPtrFlat<CSmsNumber>& f2Recipients = folder2MsgHeader->Recipients();

	TInt f1Count = f1Recipients.Count();
	TInt f2Count = f2Recipients.Count();
    TBool retVal = ETrue;
	if((f1Count == f2Count)&&(folder1MsgHeader->Type()==folder2MsgHeader->Type()))
		{
        for(TInt i=0;i<f1Count;i++)
			{
			retVal = (f1Recipients[i] == f2Recipients[i]);
			if((retVal==EFalse))
			break;
			}
		}
	else
		{
		retVal = EFalse;
		}
	CleanupStack::PopAndDestroy(folder2MsgHeader);
	CleanupStack::PopAndDestroy(folder1MsgHeader);
	return retVal;
	}

  CSmsHeader* CMtfTestActionCompareSmsMessages::GetSubmitHeaderL(CMsvEntry& aMessage)
/**
 * Returns the CSmsHeader restored from message aMessage.
 */
	{
	CSmsHeader* header = CSmsHeader::NewL(CSmsMessage::NewL(iParamSession->FileSession(), CSmsPDU::ESmsSubmit, CSmsBuffer::NewL()));
	CleanupStack::PushL(header);

	CMsvStore* store = aMessage.ReadStoreL();
	CleanupStack::PushL(store);
	header->RestoreL(*store);
	CleanupStack::PopAndDestroy(store);

	CleanupStack::Pop(header);
	return header;
	}

  CSmsHeader* CMtfTestActionCompareSmsMessages::GetDeliverHeaderL(CMsvEntry& aMessage)
  /**
   * Returns the CSmsHeader restored from message aMessage.
   */
  	{
  	CSmsHeader* header = CSmsHeader::NewL(CSmsMessage::NewL(iParamSession->FileSession(), CSmsPDU::ESmsDeliver, CSmsBuffer::NewL()));
  	CleanupStack::PushL(header);

 	CMsvStore* store = aMessage.ReadStoreL();
  	CleanupStack::PushL(store);
  	header->RestoreL(*store);
  	CleanupStack::PopAndDestroy(store);

	CleanupStack::Pop(header);
  	return header;
  	}


HBufC* CMtfTestActionCompareSmsMessages::GetBodyTextL(CMsvEntry& aMessage)
/**
 * @return The body text of message aMessage
 */
	{
	CMsvStore* store = aMessage.ReadStoreL();
	CleanupStack::PushL(store);

	CRichText* richText;
	CParaFormatLayer* paraFormatLayer;
    CCharFormatLayer* charFormatLayer;
    paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
    charFormatLayer=CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);

    richText=CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);
	store->RestoreBodyTextL(*richText);
	const TInt len =richText->DocumentLength();
	HBufC* buf = HBufC::NewL(len);
	TPtr ptr(buf->Des());
	richText->Extract(ptr);
	CleanupStack::PopAndDestroy(richText);
	CleanupStack::PopAndDestroy(charFormatLayer);
	CleanupStack::PopAndDestroy(paraFormatLayer);
	CleanupStack::PopAndDestroy(store);
	return buf;
	}
