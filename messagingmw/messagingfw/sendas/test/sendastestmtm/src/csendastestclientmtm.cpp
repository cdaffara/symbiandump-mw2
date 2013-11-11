// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// csendastestclientmtm.cpp
//
#include "csendastestclientmtm.h"

#include <msvids.h>
#include <msvuids.h>
#include <txtrich.h>

#include "csendastestaddress.h"
#include "csendastestsendoperation.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

const TInt KSendAsTestMtmMaxBodySize	= 0x100;
const TInt KSendAsTestMtmMaxMessageSize	= 0x200;

EXPORT_C CSendAsTestClientMtm* CSendAsTestClientMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
	{
	CSendAsTestClientMtm* self = new (ELeave) CSendAsTestClientMtm(aRegisteredMtmDll, aSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CSendAsTestClientMtm::~CSendAsTestClientMtm()
	{
	delete iSendAsTestMessage;
	}

CSendAsTestClientMtm::CSendAsTestClientMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: CBaseMtm(aRegisteredMtmDll, aSession)
	{
	}
	
void CSendAsTestClientMtm::ConstructL()
	{
	// Find the default service...
    TMsvSelectionOrdering ordering(KMsvNoGrouping, EMsvSortByNone, ETrue);
    CMsvEntry* entry = CMsvEntry::NewL(Session(), KMsvRootIndexEntryId, ordering);
    CleanupStack::PushL(entry);
	
 	CMsvEntrySelection* sel = entry->ChildrenWithMtmL(KUidMsgTypeSendAsTest);
	CleanupStack::PushL(sel);
	if( sel->Count() == 0 )
		{
		// There is no service - create it.
		TMsvEntry service;
		service.iMtm = KUidMsgTypeSendAsTest;
		service.iType = KUidMsvServiceEntry;
		service.SetReadOnly(EFalse);
		service.SetVisible(ETrue);
		
		entry->SetEntryL(KMsvRootIndexEntryId);
		entry->CreateL(service);
		
		iServiceId = service.Id();
		}
	else
		{
		iServiceId = sel->At(0);		
		}
	CleanupStack::PopAndDestroy(2, entry);	// entry, sel
	
	// Switch the context - this will initialise various objects in the base.	
	SwitchCurrentEntryL(KMsvRootIndexEntryId);
	
	iSendAsTestMessage = CSendAsTestMessage::NewL();
	}
	
void CSendAsTestClientMtm::DoAddAddresseeL(const TDesC& aAddress, const TDesC& aAlias, CSendAsTestMessage::TSendAsTestAddressType aType)
	{
	// Need to ensure that the message address list and the addressee list stay
	// in sync. First create the address object that will be added to the message
	// address list.	
	CSendAsTestAddress* address = CSendAsTestAddress::NewL();
	CleanupStack::PushL(address);
	address->SetAddressL(aAddress);
	address->SetAliasL(aAlias);
	address->SetAddressType(aType);
	
	// Add the address info to the addressee list - if this fails then ok as
	// message list has not been added to.
	AddToAddresseeListL(aAddress, aAlias);
	
	// Now add to the message address list...
	RPointerArray<CSendAsTestAddress>& array = iSendAsTestMessage->Addresses();
	TInt err = array.Append(address); 
	if( err != KErrNone )
		{
		// Could not add address to the message address list - remove the last
		// added address from the addressee list.
		iAddresseeList->Delete(iAddresseeList->Count()-1);
		User::Leave(err);
		}
	else
		{
		// The address was successfully added - pop off the cleanup stack.
		CleanupStack::Pop(address);		
		}
	}
	
void CSendAsTestClientMtm::AddToAddresseeListL(const TDesC& aAddress, const TDesC& aAlias)
	{
	// Append address to addressee list...
	if( aAlias.Length() > 0 )
		{
		HBufC* buf = HBufC::NewLC(aAlias.Length() + aAddress.Length() + 3);
		TPtr ptr = buf->Des();
		ptr.Copy(aAlias);
		ptr.Append(' ');
		ptr.Append('<');
		ptr.Append(aAddress);
		ptr.Append('>');
		iAddresseeList->AppendL(ptr);
		CleanupStack::PopAndDestroy(buf);		
		}
	else
		{
		iAddresseeList->AppendL(aAddress);
		}	
	}

void CSendAsTestClientMtm::Reset()
	{
	// Reset the two address lists.
	if( iAddresseeList != NULL )
		{
		iAddresseeList->Reset();
		}
	if( iSendAsTestMessage != NULL )		
		{
		iSendAsTestMessage->Addresses().ResetAndDestroy();	
		}	
	}

/*
 * from CBaseMtm
 */
 
void CSendAsTestClientMtm::SaveMessageL()
	{
	__ASSERT_DEBUG( iMsvEntry != NULL, User::Invariant() );

	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	switch( iMsvEntry->Entry().iType.iUid )
		{
	case KUidMsvServiceEntryValue:
	case KUidMsvFolderEntryValue:
		break;
	case KUidMsvMessageEntryValue:
		{
		iSendAsTestMessage->StoreL(*store);
		StoreBodyL(*store);
		TMsvEntry entry(iMsvEntry->Entry());
		if (iSendAsTestMessage->BioType() != KNullUid)
			{
			entry.iBioType = iSendAsTestMessage->BioType().iUid;
			}
		
		iMsvEntry->ChangeL(entry);

		} break;
	default:
		User::Invariant();
		}	
	store->CommitL();

 	// Set the iSize member of the TMsvEntry
 	if( iMsvEntry->HasStoreL() )
 		{
 		const TInt size = store->SizeL();
		TMsvEntry entry(iMsvEntry->Entry());
 		if( entry.iSize != size )
 			{
 			entry.iSize = size;
 			iMsvEntry->ChangeL(entry);
 			}
 		}
	CleanupStack::PopAndDestroy(store);
	}
	
void CSendAsTestClientMtm::LoadMessageL()
	{
	__ASSERT_DEBUG( iMsvEntry != NULL, User::Invariant() );

	Reset();
	Body().Reset();

	CMsvStore* store = iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	
	switch( iMsvEntry->Entry().iType.iUid )
		{
	case KUidMsvServiceEntryValue:
	case KUidMsvFolderEntryValue:
		break;
	case KUidMsvMessageEntryValue:
		{
		CSendAsTestMessage* message = CSendAsTestMessage::NewL();
		CleanupStack::PushL(message);

		// Restore the message from the store and populate the addressee list.
		message->RestoreL(*store);
		
		const RPointerArray<CSendAsTestAddress>& addresses = message->Addresses();
		TInt count = addresses.Count();
	
		for( TInt i=0; i<count; ++i )
			{
			const CSendAsTestAddress& address = *addresses[i];
			AddToAddresseeListL(address.Address(), address.Alias());
			}

		store->RestoreBodyTextL(Body());

		CleanupStack::Pop(message); 
		delete iSendAsTestMessage;
		iSendAsTestMessage = message;
		} break;
	default:
		User::Invariant();
		}
	CleanupStack::PopAndDestroy(store);
	}

TMsvPartList CSendAsTestClientMtm::ValidateMessage(TMsvPartList /*aPartList*/)
	{
	TMsvPartList retList = 0;
	return retList;
	}
	
TMsvPartList CSendAsTestClientMtm::Find(const TDesC& /*aTextToFind*/, TMsvPartList /*aPartList*/)
	{
	TMsvPartList retList = 0;
	return retList;
	}

CMsvOperation* CSendAsTestClientMtm::ReplyL(TMsvId /*aDestination*/, TMsvPartList /*aPartlist*/, TRequestStatus& aCompletionStatus)
	{
	TMsvEntry entry = iMsvEntry->Entry();
	TPckg<TMsvId> prog(entry.Id());
	return CMsvCompletedOperation::NewL(Session(), KUidMsgTypeSendAsTest, prog, entry.iServiceId, aCompletionStatus, KErrNotSupported);
	}
	
CMsvOperation* CSendAsTestClientMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& aCompletionStatus)
	{
	TMsvEntry entry = iMsvEntry->Entry();
	TPckg<TMsvId> prog(entry.Id());
	return CMsvCompletedOperation::NewL(Session(), KUidMsgTypeSendAsTest, prog, entry.iServiceId, aCompletionStatus, KErrNotSupported);
	}

void CSendAsTestClientMtm::AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress)
	{
	DoAddAddresseeL(aRealAddress, KNullDesC(), (CSendAsTestMessage::TSendAsTestAddressType)aType);
	}
	
void CSendAsTestClientMtm::AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress, const TDesC& aAlias)
	{
	DoAddAddresseeL(aRealAddress, aAlias, (CSendAsTestMessage::TSendAsTestAddressType)aType);
	}

void CSendAsTestClientMtm::AddAddresseeL(const TDesC& aRealAddress)
	{
	DoAddAddresseeL(aRealAddress, KNullDesC(), CSendAsTestMessage::ESendAsTestAddressTypeTo);
	}

void CSendAsTestClientMtm::AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias)
	{
	DoAddAddresseeL(aRealAddress, aAlias, CSendAsTestMessage::ESendAsTestAddressTypeTo);
	}

void CSendAsTestClientMtm::RemoveAddressee(TInt aIndex)
	{
	__ASSERT_DEBUG( iSendAsTestMessage->Addresses().Count() == iAddresseeList->Count(), User::Invariant() );
	
	// Remove the address from the addressee list and message address list.
	iAddresseeList->Delete(aIndex);
	iSendAsTestMessage->Addresses().Remove(aIndex);
	}
	
void CSendAsTestClientMtm::SetSubjectL(const TDesC& aSubject)
	{
	iSendAsTestMessage->SetSubjectL(aSubject);
	}
	
void CSendAsTestClientMtm::BioTypeChangedL(TUid aBioTypeUid)
	{
	iSendAsTestMessage->SetBioTypeL(aBioTypeUid);
	}

TInt CSendAsTestClientMtm::QueryCapability(TUid aCapability, TInt& aResponse)
	{
	TInt error = KErrNone;
	aResponse = ETrue;
	switch( aCapability.iUid )
		{
		case KUidMtmQueryMaxBodySizeValue:
			{
			aResponse = KSendAsTestMtmMaxBodySize;
			} break;
		case KUidMtmQueryMaxTotalMsgSizeValue:
			{
			aResponse = KSendAsTestMtmMaxMessageSize;
			} break;
		case KUidMtmQuerySupportedBodyValue:
			{
			aResponse = KMtm7BitBody + KMtm8BitBody + KMtm16BitBody;
			break;
			}
		case KUidMtmQueryCanSendMsgValue:
		case KUidMtmQuerySendAsMessageSendSupportValue:
		case KUidMtmQuerySupportAttachmentsValue:
		case KUidMtmQuerySupportSubjectValue:
			break;
		case KUidMtmQueryMaxRecipientCountValue:
			{
			aResponse = KErrNotFound;	// not limited
			} break;
		case KUidMtmQueryCanReceiveMsgValue:
		case KUidMtmQuerySupportsBioMsgValue:
		case KUidMtmQuerySupportsSchedulingValue:	
		case KUidMsvMtmQueryEditorUidValue:
		case KUidMtmQueryOffLineAllowedValue:
		case KUidMtmQuerySendAsRequiresRenderedImageValue:
		case KUidMtmQuerySendAsRenderingUidValue:
		case KUidMtmQuerySupportsFolderValue:
		case KUidMtmQuerySupportsRecipientTypeValue:
		default:
			error = KErrNotSupported;
			break;
		}
	return error;
	}

void CSendAsTestClientMtm::InvokeSyncFunctionL(TInt /*FunctionId*/, const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
	{

	User::Invariant();
	}

CMsvOperation* CSendAsTestClientMtm::InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus)
	{
	CMsvOperation* op = NULL;
	
	switch( aFunctionId )
		{
	case KMTMStandardFunctionsSendMessage:
		{
		// perform a regular send with standardised progress information for SendAs2
		op =  CSendAsTestSendOperation::NewL(Session(), aSelection, aParameter, aCompletionStatus);		
		} break;
	default:
		op = Session().TransferCommandL(aSelection,aFunctionId,(TDesC8&)aParameter, aCompletionStatus);
		break;
		};
	return op;
	}

TMsvId CSendAsTestClientMtm::DefaultServiceL() const
	{
	return iServiceId;
	}

void CSendAsTestClientMtm::ContextEntrySwitched()
	{
	Reset();
	}

/*
 * from MMsvEntryObserver
 */
 
void CSendAsTestClientMtm::HandleEntryEvent(TMsvEntryEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	}
