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
// BIOCMTM.CPP
//

#include "BIOCMTM.H"
#include "BIOSCMDS.H"

#include <biouids.h>
#include <f32file.h>
#include "REGPSDLL.H"   // Parser Registry - used to load the parser
#include <biodb.h>
#include "BIOOP.H"
#include <msvstd.h>

#include <mtmuids.h>
#include <msvreg.h>
#include <mtmdef.h>
#include <msvftext.h>

#include <txtrich.h>
#include <txtfmlyr.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#include <biomessageuids.h>
#include <bifchangeobserver.h>
#include <mtmuidsdef.hrh>
#endif

#include <msvenhancesearchsortutil.h>


EXPORT_C CBIOClientMtm* CBIOClientMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
/** Allocates and constructs a new BIO client MTM object.

@param aRegisteredMtmDll Registration data for the MTM DLL
@param aSession The CMsvSession of the client requesting the object
@return New BIO client MTM object */
	{
	CBIOClientMtm* self=new(ELeave) CBIOClientMtm(aRegisteredMtmDll, aSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CBIOClientMtm::~CBIOClientMtm()
/** Destructor. */
	{
	delete iBioDatabase;
	iFs.Close();
	}

void CBIOClientMtm::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	// construct a Bio database
	iBioDatabase = CBIODatabase::NewL(iFs);
	}

void CBIOClientMtm::HandleEntryEvent(TMsvEntryEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
/** Does nothing.

@param aEvent Unused
@param aArg1 Unused
@param aArg2 Unused
@param aArg3 Unused */
	{
	}

void CBIOClientMtm::SaveMessageL()
/** Commits cached changes to the current context's body text to storage. */
	{
	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	StoreBodyL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy();
	}

void CBIOClientMtm::LoadMessageL()
/** Loads the current context's message body from storage. */
	{
	CMsvStore* store = iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	

	// Get a reference to TMsvEnhanceSearchSortUtil  instance set by CMsvSearchsortOpOnHeaderBody class
	// If advanced search and sort is being performed than do not load the body here
	// For API's other than CMsvSearchsortOpOnHeaderBody-> FindInHeaderBodyL(), a call to LoadMessageL()
	// loads the body.
	TMsvEnhanceSearchSortUtil* searchsortutil = (TMsvEnhanceSearchSortUtil*)(GetExtensionData());
	
	
	if ( searchsortutil == NULL )
		{
		if (store->HasBodyTextL())
			{
			RestoreBodyL(*store);
			}
		}
			
	CleanupStack::PopAndDestroy();
	}

CMsvOperation* CBIOClientMtm::ReplyL  (TMsvId /*aDestination*/, TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
/** Leaves with KErrNotSupported.

@param aDestination Unused
@param aPartlist Unused
@param aCompletionStatus Unused
@return Unused */
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}

CMsvOperation* CBIOClientMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& /*aCompletionStatus*/)
/** Leaves with KErrNotSupported.

@param aDestination Unused
@param aPartList Unused
@param aCompletionStatus Unused
@return Unused */
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}

TMsvPartList CBIOClientMtm::ValidateMessage(TMsvPartList /*aPartList*/)
/** Does nothing.

@param aPartList Unused
@return Always 0 */
	{
	return 0;
	}

TMsvPartList CBIOClientMtm::Find(const TDesC& aTextToFind, TMsvPartList aPartList)
/** Searches the specified message part(s) for the plain-text version of the text 
to be found.

Supported messages parts are KMsvMessagePartBody, KMsvMessagePartOriginator, 
and KMsvMessagePartDescription.

@param aTextToFind The plain-text version of the text to be found. 
@param aPartList Indicates the message parts which should be searched. 
@return 0, if the text was not found, or searching is unsupported. If the text 
was found, a bitmask of the TMsvPartList IDs for each part in which the text 
was present. */
	{
	TMsvPartList retList=KMsvMessagePartNone;
	
  	// Enhance search and sort
  	// The setting variables are accessed by getting an instance to TMsvEnhanceSearchSortUtil  class
  	// This is accessed here by using GetExtensionData() defined in CBaseMtm which refers 
  	// to iExtensionData member variable
  	
 	TMsvEnhanceSearchSortUtil* searchsortutil = (TMsvEnhanceSearchSortUtil*)(GetExtensionData());
	
	// For callees other than CMsvSearchsortOpOnHeaderBody class, searchsortutil pointer will be NULL
	if(searchsortutil != NULL)
		{
		// Get the setting values from TMsvEnhanceSearchSortUtil iPartList variable
		TInt32 searchsortsetting=searchsortutil->GetSearchSortSetting();
		
		if (aPartList & KMsvMessagePartFrom) // From search
			{
			TRAPD(ret,FindInOriginatorL(aTextToFind,aPartList,retList));
			if (ret != KErrNone)
				{
				User::LeaveIfError(ret);
				}
			}
		else if (aPartList & KMsvMessagePartSubject) // Subject search	
			{
			TRAPD(ret,FindInDescriptionL(aTextToFind,aPartList,retList));
			if (ret != KErrNone)
				{
				User::LeaveIfError(ret);
				}	
			}
		// Load the body if it wasnt previously loaded
		else if ((aPartList & KMsvMessagePartBody) && !(searchsortsetting & EMessagePartBodyLoaded))
			{
			// Load the message body
			CMsvStore* store = iMsvEntry->ReadStoreL();
			CleanupStack::PushL(store);
			if (store->HasBodyTextL())
				{
				RestoreBodyL(*store);
				}
				
			// The message body is loaded.Set the setting variable to specify that the body is loaded
			// If the next search is also on body, than it wont be loaded next time.
			
			searchsortutil->SetSearchSortSetting(EMessagePartBodyLoaded); 
			
			TRAPD(ret,FindInBodyL(aTextToFind,aPartList,retList)); // Body search
			
			if (ret != KErrNone)
				{
				User::LeaveIfError(ret);
				}
			CleanupStack::PopAndDestroy(); // store
			}
		// Copying the Sort data for advanced search and sort.
		// This is done by setting the iExtensionData member variable of CBaseMtm class point 
		// to the field being sorted
		if ((searchsortsetting & EMessagePartSort ) || ((searchsortsetting & EMessagePartSearchSort) && (searchsortsetting & EMessagePartLastQueryOption) && (retList)))
			{
			if(searchsortsetting & EMessagePartFromSort)
	   		    {
	   		    SetExtensionData((TAny*)&iMsvEntry->Entry().iDetails);	
	   		    }
	  		else if(searchsortsetting & EMessagePartSubjectSort)
   				{
   				SetExtensionData((TAny*)&iMsvEntry->Entry().iDescription);		
   				}
   			else 
   				{
   				searchsortutil->SetSearchSortSetting(EMessagePartInvalidSortField);
   				}
			}
		}
	else
		{

		//Normal search. Implementation prior to PREQ1667 (Advanced search and sort on message store)
		if (aPartList&KMsvMessagePartBody)
			{
			TRAPD(ret,FindInBodyL(aTextToFind,aPartList,retList));
			if (ret != KErrNone)
				{
				User::LeaveIfError(ret);
				}
			}
		else if (aPartList&KMsvMessagePartOriginator) 
			{
			TRAPD(ret,FindInOriginatorL(aTextToFind,aPartList,retList));
			if (ret != KErrNone)
				{
				User::LeaveIfError(ret);
				}
			}
		else if (aPartList&KMsvMessagePartDescription)	
			{
			TRAPD(ret,FindInDescriptionL(aTextToFind,aPartList,retList));
			if (ret != KErrNone)
				{
				User::LeaveIfError(ret);
				}
			}
		}
	return retList;
	}

void CBIOClientMtm::FindInDescriptionL(const TDesC& aTextToFind, TMsvPartList aPartList,TMsvPartList& aFoundList)
	{
	CMsvFindText* text=CMsvFindText::NewL();
	CleanupStack::PushL(text);

	if (text->FindTextL(aTextToFind,iMsvEntry->Entry().iDescription,aPartList))
		aFoundList|=KMsvMessagePartDescription;

	CleanupStack::PopAndDestroy();// text
	}

void CBIOClientMtm::FindInOriginatorL(const TDesC& aTextToFind, TMsvPartList aPartList,TMsvPartList& aFoundList)
	{
	CMsvFindText* text=CMsvFindText::NewL();
	CleanupStack::PushL(text);
	
	if (text->FindTextL(aTextToFind,iMsvEntry->Entry().iDetails,aPartList))
		aFoundList|=KMsvMessagePartBody;

	CleanupStack::PopAndDestroy();// text
	}

void CBIOClientMtm::FindInBodyL(const TDesC& aTextToFind, TMsvPartList aPartList,TMsvPartList& aFoundList)
	{
	CMsvFindText* text=CMsvFindText::NewL();
	CleanupStack::PushL(text);

	if (text->FindRichTextL(aTextToFind,Body(),aPartList))
		aFoundList|=KMsvMessagePartBody;

	CleanupStack::PopAndDestroy();// text
	}
	
void CBIOClientMtm::AddAddresseeL(const TDesC& /*aRealAddress*/)
/** Leaves with KErrNotSupported.

@param aRealAddress Unused */
	{
	User::Leave(KErrNotSupported);
	}

void CBIOClientMtm::AddAddresseeL(const TDesC& /*aRealAddress*/, const TDesC& /*aAlias*/)
/** Leaves with KErrNotSupported.

@param aRealAddress Unused
@param aAlias Unused */
	{
	User::Leave(KErrNotSupported);
	}

void CBIOClientMtm::RemoveAddressee(TInt /*aIndex*/)
/** Does nothing.

@param aIndex Unused */
	{
	}

void CBIOClientMtm::ContextEntrySwitched()
	{
	}

void CBIOClientMtm::InvokeSyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
/** Leaves with KErrNotSupported.

@param aFunctionId Unused
@param aSelection Unused
@param aParameter Unused */
	{
	User::Leave(KErrNotSupported);
	}

CMsvOperation* CBIOClientMtm::InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& /*aParameter*/, TRequestStatus& aCompletionStatus)
/** Requests a BIO message to be parsed or processed.

The behaviour resulting from the request is specific to the BIO message type. 
Typically, parsing causes an interpretation of the raw message body, and storage 
of the results. Processing causes some system action to take place according 
to the instructions in the message.

@param aFunctionId ID of the requested operation: this must be a TBiosCmds 
value.
@param aSelection Message entry to parse or process.
@param aParameter Unused
@param aCompletionStatus The request status to be completed when the operation 
has finished
@return If successful, this is an asynchronously completing operation. If failed, 
this is a completed operation, with status set to the relevant error code. */
	{
	if (aFunctionId != KBiosMtmParse && 
		aFunctionId != KBiosMtmParseThenProcess && 
		aFunctionId != KBiosMtmProcess)
		User::Leave(KErrNotSupported);
	
	__ASSERT_DEBUG(aSelection.Count()>=1, User::Panic(_L("BIOC"), KBIOMessageNotFound));
	// Messaging API V2
	CBIOOperation* parseOp = CBIOOperation::NewL(iFs, Session(), iBioDatabase, aCompletionStatus);
	parseOp->StartCommand(aSelection, aFunctionId, aCompletionStatus);
	return parseOp;
	}

// --- RTTI functions ---
TInt CBIOClientMtm::QueryCapability(TUid aCapability, TInt& aResponse)
/** Queries if the MTM supports a particular capability, specified by a UID.

It supports KUidMtmQueryMaxBodySize, KUidMtmQueryMaxTotalMsgSize, KUidMsvMtmQueryEditorUid, 
KUidMtmQuerySupportedBody, and KUidMtmQueryCanReceiveMsg.

@param aCapability UID of capability to be queried
@param aResponse Response value. The format of the response varies according 
to the capability. 
@return KErrNone: aCapability is a recognised value and a response is returned 
KErrNotSupported: aCapability is not a recognised value */
	{
	TInt error = KErrNone;
	// Supported
	switch (aCapability.iUid)
		{
	case KUidMtmQueryMaxBodySizeValue:
	case KUidMtmQueryMaxTotalMsgSizeValue:
		aResponse = KMaxTInt;
		break;
	case KUidMsvMtmQueryEditorUidValue:
		aResponse = KUidBIOMessageViewerApp.iUid;
		break;
	case KUidMtmQuerySupportedBodyValue:
		aResponse = KMtm7BitBody + KMtm8BitBody + KMtm16BitBody + KMtmBinaryBody;
		break;
	case KUidMtmQueryCanReceiveMsgValue:		
		break;
	// All others - Not Supported:
	default:
		error = KErrNotSupported;
		}
	return error;
	}


//
// constructor
//
CBIOClientMtm::CBIOClientMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: CBaseMtm(aRegisteredMtmDll, aSession)
	{
	}

