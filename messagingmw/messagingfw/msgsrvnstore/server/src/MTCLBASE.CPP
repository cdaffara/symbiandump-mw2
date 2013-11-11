// Copyright (c) 1998-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// MTCLBASE.CPP
//
#include <e32std.h>
#include <txtrich.h>
#include <txtfmlyr.h>
#include <mtmuids.h>

#include "MSVIDS.H"
#include "MSVAPI.H"
#include "MTCLBASE.H"
#include "MTCLREG.H"
#include "MSVPANIC.H"
#include "MSVUIDS.H"

#include <mmsvattachmentmanager.h>
#include "CMsvAttachmentWaiter.h"
#include <cmsvmimeheaders.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

#if defined(_DEBUG)
_LIT(KMtclPanicString, "MSGS");
#endif


EXPORT_C CBaseMtm::CBaseMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: iRegisteredMtmDll(aRegisteredMtmDll), iSession(aSession)
//
//
//
/** Creates a CBaseMtm with member variables initialised from the passed arguments. 


Client applications do not use this function. It is relevant only to implementers 
of derived classes.

The value of aSession can be accessed through Session().

Derived classes can implement a constructor to perform any additional MTM-specific 
setup that can be safely carried out in a constructor. Such constructors must 
call the base class constructor function.

Derived classes also implement two-phase construction functions (NewL(), ConstructL()) 
to create a new instance of the object, in which any dynamic allocation should 
be performed. Client-side MTMs also implement a factory function by which 
a MTM registry can request an instance of the class.

@param aRegisteredMtmDll Registration data for the MTM DLL. 
@param aSession The CMsvSession of the client requesting the object */
	{}


EXPORT_C CBaseMtm::~CBaseMtm()
//
//
//
/** Cleans up the base class. 

CBaseMtm -derived objects must be deleted by client applications when they 
are no longer required. 

Derived classes can implement a destructor to do any additional clean up tasks 
that they require. */
	{
	DeleteEntry();
	delete iAddresseeList;
	delete iRichTextBody;
	delete iCharFormatLayer;
	delete iParaFormatLayer;
	iRegisteredMtmDll.ReleaseLibrary();
	delete iAttachmentWaiter;
	if(iExtensionData)
  		{
		delete iExtensionData;    
  		}	
	}


EXPORT_C TUid CBaseMtm::Type() const
/** Gets the Type UID of the message type associated with the Client-side MTM.

@return UID of the message type associated with the MTM */
	{
	return iRegisteredMtmDll.MtmTypeUid();
	}


EXPORT_C TInt CBaseMtm::QueryCapability(TUid /*aCapability*/, TInt& /*aResponse*/)
//
//
//
	/** Queries if the MTM supports a particular capability, specified by a UID. 
	
	For MTM-specific UIDs, see the documentation for the relevant MTM.
	
	Requirements:
	
	Implementations should check aCapability for the standard capability values, 
	plus any MTM-specific capabilities, and if recognised, return a suitable response 
	in aResponse. If aCapability is unknown, return KErrNotSupported.
	
	The default implementation returns KErrNotSupported. 
	
	@param aCapability UID of capability to be queried 
	@param aResponse Response value. The format of the response varies according 
	to the capability. 
	@return KErrNone: aCapability is a recognised value and a response is returned 
	KErrNotSupported: aCapability is not a recognised value */
	{
	return KErrNotSupported;
	}



EXPORT_C void CBaseMtm::SwitchCurrentEntryL(TMsvId aId)
//
//
//
/** Changes the entry on which later actions are performed to the entry with the 
specified TMsvId. 

@param aId The ID of the entry upon which all following actions will be performed 

@leave KErrNoMemory Insufficient memory 
@leave KErrNotFound The requested entry does not exist */
	{
	// create the new CMsvEntry
	CMsvEntry* newEntry = Session().GetEntryL(aId);
	SetCurrentEntryL(newEntry);
	}



EXPORT_C void CBaseMtm::SetCurrentEntryL(CMsvEntry* aEntry)
//
//
//
/** Sets the entry on which later actions are performed to the specified CMsvEntry. 


@param aEntry The entry on which all following actions will be performed 
@leave KErrNoMemory Insufficient memory */
	{
	CleanupStack::PushL(aEntry);

	if (iAddresseeList==NULL)
		iAddresseeList = CMsvRecipientList::NewL();
	if (iParaFormatLayer==NULL)
		iParaFormatLayer = CParaFormatLayer::NewL();
	if (iCharFormatLayer==NULL)
		iCharFormatLayer = CCharFormatLayer::NewL();
	if (iRichTextBody==NULL)
		iRichTextBody = CRichText::NewL(iParaFormatLayer, iCharFormatLayer);
	else
		iRichTextBody->Reset();

	aEntry->AddObserverL(*this);

	CleanupStack::Pop(); // aEntry

	// swap the old and new context
	DeleteEntry();
	iMsvEntry = aEntry;
	iEntryId = iMsvEntry->EntryId();

	// If waiting for attachment additons to be commited, delete without commiting
	// the changed
	delete iAttachmentWaiter;
	iAttachmentWaiter = NULL;

	// inform concrete implementation that the context has switched (or set in the first case)
	ContextEntrySwitched();
	}


EXPORT_C CMsvEntry& CBaseMtm::Entry() const
/** Gets a CMsvEntry for the current context. The message client application can 
then use this to manipulate the entry.

Implementers should note that this function returns the value of the protected 
data member iMsvEntry.

@return CMsvEntry for the current context */
	{
	__ASSERT_DEBUG(iMsvEntry!=NULL, User::Panic(KMtclPanicString, 275));
	return *iMsvEntry;
	};


EXPORT_C TBool CBaseMtm::HasContext() const
/** Tests if an MTM context has been set.

A Client-side MTM has no context until one is set through SwitchCurrentEntryL() 
or SetCurrentEntryL() . 

@return ETrue: context has been set EFalse: context has not been set */
	{
	return (iMsvEntry!=NULL);
	}


void CBaseMtm::DeleteEntry()
//
//
//
	{
	if (iMsvEntry!=NULL)
		{
		iMsvEntry->RemoveObserver(*this);
		delete iMsvEntry;
		iMsvEntry=NULL;
		}
	}

EXPORT_C void CBaseMtm::StoreBodyL(CMsvStore& aStore)
//
//
//
/** Stores and commits the current cached CRichText body text to the appropriate 
stream in the specified message store. Client applications do not use this 
function. It is relevant only to implementers of derived classes.

The function overwrites any existing data in that stream. The implementation 
must have write access to aStore. 

A typical use of this function is from SaveMessageL().

@param aStore Message store in which to store the body text 
@leave KErrAccessDenied The store was opened for read only 
@leave Other Standard stream-related leave codes */
	{
	__ASSERT_ALWAYS(iMsvEntry!=NULL, PanicServer(EMtclMsvEntryNotSet));
	__ASSERT_ALWAYS(iMsvEntry->EntryId()==iEntryId, PanicServer(EMtclContextChangedByOwner));
	
	aStore.StoreBodyTextL(*iRichTextBody);	
	}

EXPORT_C void CBaseMtm::RestoreBodyL(CMsvStore& aStore)
//
//
//
/** Gets the current cached CRichText from the appropriate stream in the specified 
message store. Client applications do not use this function. It is relevant 
only to implementers of derived classes.

A typical use of this function is from LoadMessageL().

@param aStore Message store from which to read the body text */
	{
	__ASSERT_ALWAYS(iMsvEntry!=NULL, PanicServer(EMtclMsvEntryNotSet));
	__ASSERT_ALWAYS(iMsvEntry->EntryId()==iEntryId, PanicServer(EMtclContextChangedByOwner));

	aStore.RestoreBodyTextL(*iRichTextBody);
	}


EXPORT_C CRichText& CBaseMtm::Body()
/** Gets the body text of the context, that must be a message. For non-message 
contexts, an empty CRichText is returned. 

@return Body text of the context 
@see StoreBodyL()
@see RestoreBodyL() */
	{
	__ASSERT_DEBUG(iMsvEntry!=NULL, User::Panic(KMtclPanicString, 275)); 
	return *iRichTextBody;
	}


EXPORT_C const CRichText& CBaseMtm::Body() const
/** Gets the read-only body text of the context, that must be a message. For non-message 
contexts, an empty CRichText is returned. 

@return Read-only body text of the context 
@see StoreBodyL()
@see RestoreBodyL() */
	{
	__ASSERT_DEBUG(iMsvEntry!=NULL, User::Panic(KMtclPanicString, 275));
	return *iRichTextBody;
	}


EXPORT_C void CBaseMtm::SetSubjectL(const TDesC& /*aSubject*/)
//
//
//
	/** Sets the subject text of the context, which must be a message. 
	
	Some Client-side MTMs may not support subject text, in which case the function 
	leaves with KErrNotSupported.
	
	Requirements:
	
	If the message protocol supports subject text, implementations should maintain 
	a private buffer to store this information, settable through this function.
	
	Implementations should save the subject text to the appropriate place in the 
	message store when the message is stored.
	
	The default implementation leaves with KErrNotSupported.
	
	@param aSubject Message subject text 
	@leave KErrNotSupported The Client-side MTMs does not support subject text 
	@see StoreBodyL()
	@see RestoreBodyL() */
	{
	User::Leave(KErrNotSupported);
	}


/** Gets the list of intended recipients for the current context, which must be 
a message. 

In the case of protocols that allow different classes of recipient (such as 
To, Cc and Bcc), the list returned is whatever the protocol defines as the 
default recipient list. 

Requirements:

The default implementation simply returns the value of the protected data 
member iAddresseeList. As a consequence, Client-side MTM implementations should 
update this member whenever the address list is modified.

@return Array of recipients
*/
EXPORT_C const CMsvRecipientList& CBaseMtm::AddresseeList() const
	{
	__ASSERT_DEBUG(iMsvEntry!=NULL, User::Panic(KMtclPanicString, 275));
	return *iAddresseeList;
	}


/** Adds addressee to list.

The default implementation treats To: and Cc: type addressees as if the addressee
had no type.

The default implementation does not support Bcc type addressees.

@param	aType
The addressee type.

@param	aRealAddress
The addressee address.

@leave	KErrNotSupprted
The addressee type was Bcc which is not supported in the default implementation.
*/
EXPORT_C void CBaseMtm::AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress)
	{
	if (aType == EMsvRecipientBcc)
		{
		User::Leave(KErrNotSupported);
		}
	AddAddresseeL(aRealAddress);
	}

	
/** Adds addressee to list.

The default implementation treats To: and Cc: type addressees as if the addressee
had no type.

The default implementation does not support Bcc type addressees.

@param	aType
The addressee type.

@param	aRealAddress
The addressee address.

@param	aAlias
The alias for the addressee.

@leave	KErrNotSupprted
The addressee type was Bcc which is not supported in the default implementation.
*/
EXPORT_C void CBaseMtm::AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress, const TDesC& aAlias)
	{
	if (aType == EMsvRecipientBcc)
		{
		User::Leave(KErrNotSupported);
		}
	AddAddresseeL(aRealAddress, aAlias);
	}


EXPORT_C const TPtrC CBaseMtm::SubjectL() const
//
//
//
/** Gets the subject text of the context, which must be a message. 

Some Client-side MTMs may not support subject text, in which case the function 
leaves with KErrNotSupported.

Requirements:

If the message protocol supports subject text, implementations should maintain 
a private buffer to store this information, gettable through this function.

The default implementation leaves with KErrNotSupported.

@leave KErrNotSupported The Client-side MTMs does not support subject text 
@return Message subject text */
	{
	User::Leave(KErrNotSupported);
	return TPtrC();
	}


EXPORT_C void CBaseMtm::HandleEntryEventL(TMsvEntryEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
/** Indicates that an event has occurred. 

Client applications do not use this function. It is relevant only to implementers of derived classes.

The Client-side MTM object is automatically set as an observer, through CMsvEntry::AddObserverL(), 
for the context whenever the context changes (by SetCurrentEntryL() or SwitchCurrentEntryL()).

The default implementation is defined to do nothing.

Requirements:

Implementations can override this function to handle events concerning the current context. 

@param aEvent Indicates the event type 
@param aArg1 Event specific argument value 
@param aArg2 Event specific argument value
@param aArg3 Event specific argument value
*/
	{
	}


EXPORT_C CMsvSession& CBaseMtm::Session()
/** Gets a reference to the session object passed by the creator of the Client-side 
MTM.

@return Session object used by the Client-side MTM */
	{
	return iSession;
	}


/**
Adds a file attachment to the current message entry.

The attachment is referenced by its file path and is copied into the message store.
Only one asynchronous operation can be run at any one time.

@param aFilePath The full path specification of the attachment file.
@param aMimeType The mime type of the attachment file.
@param aCharset The mime charset. The value is a standard IANA value. The value 0 indicates
	   that no charset is provided.
@param aStatus The request status to complete when request has completed.
@leave System-wide error codes.
*/
EXPORT_C void CBaseMtm::AddAttachmentL(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus)
	{
	AddFilePathAttachmentL(aFilePath, aMimeType, aCharset, CMsvAttachment::EMsvFile, aStatus);
	}
	
/**
Adds a file attachment to the current message entry.

The attachment is referenced by an open file handle and is copied into the message store. 
Only one asynchronous operation can be run at any one time.

@param aFile An open file handle for the file attachment.
@param aMimeType The mime type of the attachment file.
@param aCharset The mime charset. The value is a standard IANA value. The value 0 indicates
	   that no charset is provided.
@param aStatus The request status to complete when request has completed.
@leave System-wide error codes.
*/
EXPORT_C void CBaseMtm::AddAttachmentL(RFile& aFile, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus)
	{
	if( iAttachmentWaiter == NULL )
		iAttachmentWaiter = CMsvAttachmentWaiter::NewL();

	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	
	// set the size
	TInt size = 0;
	User::LeaveIfError(aFile.Size(size));
	attachment->SetSize(size);
	
	// set the mime-type if provided
	if( aMimeType.Length() > 0 )
		{
		attachment->SetMimeTypeL(aMimeType);
		}
		
	TFileName fileName;
	User::LeaveIfError(aFile.Name(fileName));
	attachment->SetAttachmentNameL(fileName);
	
	if( aCharset!=0 )
		{
		CMsvMimeHeaders* headers = CMsvMimeHeaders::NewLC();
		headers->SetMimeCharset(aCharset);
		headers->StoreL(*attachment);
		CleanupStack::PopAndDestroy(headers);
		}	
	
	// attachment is initialised, pass to the attachment manager
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	manager.AddAttachmentL(aFile, attachment, iAttachmentWaiter->iStatus); 
	CleanupStack::Pop(attachment); // ownership passed to manager
	iAttachmentWaiter->StartWaitingL(aStatus, store, &manager);
	CleanupStack::Pop(store);      // ownership passed
	}

/**
Adds a file attachment to the current message entry as a linked file.

The attachment is referenced by its file path and is not copied into the message store. The attachment file is always used from
its original location on disk indicated by the aFilePath parameter. Only one asynchronous operation can be run at any one time.

@param aFilePath The full path specification of the attachment file.
@param aMimeType The mime type of the attachment file.
@param aCharset The mime charset. The value is a standard IANA value. The value 0 indicates
	   that no charset is provided.
@param aStatus The request status to complete when request has completed.
@leave System-wide error codes.
*/	
EXPORT_C void CBaseMtm::AddLinkedAttachmentL(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus)
	{
	AddFilePathAttachmentL(aFilePath, aMimeType, aCharset, CMsvAttachment::EMsvLinkedFile, aStatus);
	}

/**
Creates a new empty file attachment to the current message entry.

An empty file attachment is created with the suggested given name and if
completed successfully, the aAttachmentFile will be open on the new file.
Only one asynchronous operation can be run at any one time.

@param aFileName The suggested file name for the new attachment file.
@param aAttachmentFile If successful, this will be set to the open new file.
@param aMimeType The mime type of the attachment file.
@param aCharset The mime charset. The value is a standard IANA value. The value 0 indicates
	   that no charset is provided.
@param aStatus The request status to complete when request has completed.
@leave System-wide error codes.
*/
EXPORT_C void CBaseMtm::CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus)
	{
	if( iAttachmentWaiter == NULL )
		iAttachmentWaiter = CMsvAttachmentWaiter::NewL();

	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	CMsvAttachment* attachmentInfo = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachmentInfo);
	attachmentInfo->SetAttachmentNameL(aFileName);
	attachmentInfo->SetMimeTypeL(aMimeType);

	if( aCharset!=0 )
		{
		CMsvMimeHeaders* headers = CMsvMimeHeaders::NewLC();
		headers->SetMimeCharset(aCharset);
		headers->StoreL(*attachmentInfo);
		CleanupStack::PopAndDestroy(headers);
		}

	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	manager.CreateAttachmentL(aFileName, aAttachmentFile, attachmentInfo, iAttachmentWaiter->iStatus);
	CleanupStack::Pop(attachmentInfo); // ownership passed to manager
	iAttachmentWaiter->StartWaitingL(aStatus, store, &manager);
	CleanupStack::Pop(store); // ownership passed
	}

/**
Adds a message entry as an attachment to the current message entry.

This method simply registers an existing message entry as an attachment to the current entry. Only one asynchronous
operation can be run at any one time.

@param aAttachmentId The message Id of the message entry to add as an attachment.
@param aStatus The request status to complete when request has completed.
@leave System-wide error codes.
*/	
EXPORT_C void CBaseMtm::AddEntryAsAttachmentL(TMsvId aAttachmentId, TRequestStatus& aStatus)
	{
	// Get the size of the message entry
	CMsvEntry* attachEntry = iSession.GetEntryL(aAttachmentId);
	TInt attachSize = attachEntry->Entry().iSize;
	delete attachEntry;
	attachEntry = NULL;
	
	if( iAttachmentWaiter == NULL )
		iAttachmentWaiter = CMsvAttachmentWaiter::NewL();

	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvMessageEntry);
	CleanupStack::PushL(attachment);
	attachment->SetSize(attachSize);
	
	// Register entry as attachment
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	manager.AddEntryAsAttachmentL(aAttachmentId, attachment, iAttachmentWaiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed to manager
	iAttachmentWaiter->StartWaitingL(aStatus, store, &manager);
	CleanupStack::Pop(store); // ownership passed
	}

void CBaseMtm::AddFilePathAttachmentL(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, CMsvAttachment::TMsvAttachmentType aType, TRequestStatus& aStatus)
	{
	__ASSERT_DEBUG(aType != CMsvAttachment::EMsvMessageEntry, User::Invariant());
	
	if( iAttachmentWaiter == NULL )
		iAttachmentWaiter = CMsvAttachmentWaiter::NewL();
	
	CMsvStore* store = iMsvEntry->EditStoreL();
	CleanupStack::PushL(store);

	CMsvAttachment* attachment = CMsvAttachment::NewL(aType);
	CleanupStack::PushL(attachment);
	
	// set the size
	TEntry fileEntry;
	RFs& fs = iSession.FileSession();
	User::LeaveIfError(fs.Entry(aFilePath, fileEntry));
	attachment->SetSize(fileEntry.iSize);
	
	// set the mime-type, if provided
	if( aMimeType.Length() > 0 )
		{
		attachment->SetMimeTypeL(aMimeType);
		}
		
	// set attachment name
	TParse fileNameParser;
	User::LeaveIfError(fileNameParser.Set(aFilePath, NULL, NULL));
	attachment->SetAttachmentNameL(fileNameParser.NameAndExt());
	
	if( aCharset!=0 )
		{
		CMsvMimeHeaders* headers = CMsvMimeHeaders::NewLC();
		headers->SetMimeCharset(aCharset);
		headers->StoreL(*attachment);
		CleanupStack::PopAndDestroy(headers);
		}
	
	// attachment is initialised, pass to the attachment manager
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	switch(aType)
		{
		case CMsvAttachment::EMsvFile:
			manager.AddAttachmentL(aFilePath, attachment, iAttachmentWaiter->iStatus);
			break;
		case CMsvAttachment::EMsvLinkedFile:
			manager.AddLinkedAttachmentL(aFilePath, attachment, iAttachmentWaiter->iStatus);
			break;
		default:
			break;
		}

	CleanupStack::Pop(attachment); // ownership passed to manager

	iAttachmentWaiter->StartWaitingL(aStatus, store, &manager);
	CleanupStack::Pop(store); // ownership passed
	}

/**
Cancels the current attachment operation.
*/	
EXPORT_C void CBaseMtm::CancelAttachmentOperation()
	{
	if(iAttachmentWaiter)
		{
		iAttachmentWaiter->Cancel();
		delete iAttachmentWaiter;
		iAttachmentWaiter = NULL;
		}
	}


// virtual
EXPORT_C void CBaseMtm::CreateMessageL(TMsvId aServiceId)
/** Creates a new message entry as a child of the current context.

The default implementation creates an empty entry with its visible flag set 
to false and its in-preparation flag set to true.

@param aServiceId ID of the service to own the entry. */
	{
	// create an invisible blank entry 
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iServiceId = aServiceId;
	entry.iMtm = Type();
	entry.SetVisible(EFalse);
	entry.SetInPreparation(ETrue);

	// store entry in folder
	iMsvEntry->CreateL(entry);
	iMsvEntry->SetEntryL(entry.Id());
	iEntryId = entry.Id();
	}

EXPORT_C void CBaseMtm::BioTypeChangedL(TUid /*aBioTypeUid*/)
	/** Informs client-side MTM that the context's BIO field is being changed as a 
	result of a call to CSendAs::SetBioTypeL().
	
	CSendAs::SetBioTypeL() calls this function before setting the BIO field in 
	the context's index entry. This allows a client-side MTM to perform MTM specific 
	actions when the BIO type changes. 
	
	CSendAs will not change the BIO type if this function leaves.
	
	The default implementation is to do nothing. 
	
	@param aBioTypeUid New value for the BIO field
	@see CSendAs::SetBioTypeL() */
	{
	//The default implementation is to do nothing.
	}
 	
/**
Gets the default MTM service.
 	
The default implementation is to assume the MTM only supports one service so finds the
first service associated with this MTM and returns that.
 
@return
The default service
 
@leave 
KErrNotFound If no service has been created.
*/
EXPORT_C TMsvId CBaseMtm::DefaultServiceL() const
 	{
    // Create a new entry, showing invisible entries (because the service entry may be invisible)
    TMsvSelectionOrdering ordering(KMsvNoGrouping, EMsvSortByNone, ETrue);
    CMsvEntry* entry = CMsvEntry::NewL(iSession, KMsvRootIndexEntryId, ordering);
    CleanupStack::PushL(entry);
	
 	CMsvEntrySelection *sel=entry->ChildrenWithMtmL(iRegisteredMtmDll.MtmTypeUid());
	CleanupStack::PushL(sel);
	if(sel->Count() == 0)
	    {
	    User::Leave(KErrNotFound);
	    } 
	TMsvId service=sel->At(0);
 	CleanupStack::PopAndDestroy(2,entry);
	return service;
 	}
 
/**
Removes the default service setting. The default implementation of this function assumes
that the MTM only supports one service and therefore this does nothing.
*/
EXPORT_C void CBaseMtm::RemoveDefaultServiceL()
 	{
 	}
 
/**
Sets the default MTM service. The default implementation of this function assumes that
the MTM only supports one service and therefore this does nothing.
 
@param	aService
The default service
*/
EXPORT_C void CBaseMtm::ChangeDefaultServiceL(const TMsvId& /*aService*/)
 	{
 	}


/**
Returns a pointer to the interface with the specified Uid.

This method is the first part of an extension pattern to allow for 
more functionality to be supported without adding virtual methods 
to this base class.

The default implementation returns a NULL pointer.
 
@param	aUid  
Uid of the extension interface
@return
Pointer to the extension interface
*/
EXPORT_C TAny* CBaseMtm::GetInterface(TUid /*aUid*/)
 	{
	return NULL;
 	}

/** 
Sets the character encoding value. The character encoding value options are 7-bit,
8-bit and 16-Bit Unicode. By default the character set encoding is 7 bit encoding.
@param	aCharSet	Character encoding value may be 7-bit/8-bit/16-bit Unicode.
@return KErrNone	If charset is changed successfully in SMS settings.
*/
   
EXPORT_C TInt CBaseMtm::SetMessageCharacterSet(TUint aCharSet)
	{
	TAny* ptrNull=NULL;
	return Extension_(KUIDCharacterSet,ptrNull,&aCharSet);		
	}
	
/**
The default Extension service. The default implementation of this function assumes that
the new service for setting the charset encoding value for a SMS message is not 
supported. TAny* is equivalent to void*.
@param	a0				 			The collective parameters of TAny*
@param	a1				 			The collective parameters of TAny*,Charset encoding value is actually extracted from a1.
@param aExtensionId 	 			Uid of the extension interface
@return KErrExtensionNotSupported 	If the message is other than SMS.
@return Other 			 			Standard system-wide error codes.
*/

EXPORT_C TInt CBaseMtm::Extension_(TUint aExtensionId, TAny *&a0, TAny *a1)	
	{
	TInt ret = KErrNone;
	switch(aExtensionId)
		{
		case KUIDCharacterSet:
			{
			ret = KErrExtensionNotSupported;
			}
			break;
		default:
			{
			// Chain to base class
			ret = CBase::Extension_(aExtensionId,a0,a1);
			}
			break;
		}
	return ret;
	}

//if passed value is NULL, its as good as reset extention data
EXPORT_C void CBaseMtm::SetExtensionData(TAny* aSortData)
	{
	iExtensionData=aSortData;	
	}
	
EXPORT_C TAny* CBaseMtm::GetExtensionData()
	{
	return iExtensionData;
	}

