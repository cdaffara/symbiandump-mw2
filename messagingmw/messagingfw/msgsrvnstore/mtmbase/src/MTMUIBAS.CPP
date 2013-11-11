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
// MODULE - Message Type Module
// 
//

// Generic includes
#include <coemain.h>	// CCoeEnv
#include <bautils.h>	// BaflUtils
#include <basched.h>	// KErrExtended

// Messaging includes
#include <msvreg.h>
#include <msvuids.h>
#include <msvruids.h>
#include "MSVUTILS.H"

// Specific includes
#include "MTMUIBAS.H"
#include "MTUIPAN.H"


//
// CBaseMtmUi - MTMUi base API (public methods)  //
//
EXPORT_C CBaseMtmUi::~CBaseMtmUi()
/** Destructor.

Cleans up the base class. CBaseMtmUi-derived objects must be deleted by client 
applications when they are no longer required. 

Derived classes can implement a destructor to do any additional clean up tasks 
that they require. */
	{
	iRegisteredMtmDll.ReleaseLibrary();
	if (iResourceFileOffset)
		iCoeEnv->DeleteResourceFile(iResourceFileOffset);
	}

EXPORT_C void CBaseMtmUi::SetPreferences(TUint aFlags)
/** Sets flags that communicate the MTM's preferences to the application UI.

@param aFlags Bitmask of preference flags */
	{
	iFlags = aFlags;
	}

EXPORT_C TUint CBaseMtmUi::Preferences() const
/** Gets flags that communicate the MTM's preferences to the application UI.

@return Bitmask of preference flags */
	{
	return iFlags;
	}

EXPORT_C TUid CBaseMtmUi::Type() const
/** Gets the Type UID of the message type associated with the User Interface MTM.

@return UID of the message type associated with the MTM */
	{
	return iBaseMtm.Type();
	}

EXPORT_C CBaseMtm& CBaseMtmUi::BaseMtm() const
/** Gets a reference to the Client-side MTM that requested this object.

@return Client-side MTM that requested this object */
	{
	return iBaseMtm;
	}

//
// --- Functions NOT dependent on the current context ---
//
EXPORT_C CMsvOperation* CBaseMtmUi::CreateL(const TMsvEntry& aEntry, CMsvEntry& aParent, TRequestStatus& aStatus)
// Default implementation simply delegates to the message server
/** Creates a new entry as a child. 

Typically, after creating the entry, the function would launch a suitable interface, 
such as a message editor, for the user to edit the new entry.

The returned CMsvOperation object completes when creation is complete (for 
example, when the message editor application is closed).

Requirements:

The default implementation simply calls aParent.CreateL() to create the entry. 
If creating entries is not supported, implementations should leave with KErrNotSupported. 
Otherwise, implementations should:

1. check aEntry.iType.iUid to ensure that they can create the requested type 
of entry and leave with code KErrNotSupported if they cannot

2. return a CMsvOperation-derived object to provide asynchronous control and 
monitoring of the operation

3. for messages, launch a suitable editor application developed for the message 
type.

@param aEntry The data to be copied into the new entry 
@param aParent The parent of the new entry 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNotSupported The requested type of entry cannot be created 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing create operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	return aParent.CreateL(aEntry, aStatus);
	}

//
// --- Deletion ---
// --- Deletes entries from the current context, which must be a folder or service of the relevant MTM ---
//
EXPORT_C CMsvOperation* CBaseMtmUi::DeleteFromL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
/** Deletes a selection of entries owned by the current context. 

The current context must be a folder or service of the relevant MTM. The returned CMsvOperation 
object completes when deleting is complete. 

Requirements:

The default implementation simply calls CMsvEntry::DeleteL() to delete the 
entries. Implementations can override this to provide any additional checking 
or user interaction. If deleting entries is not supported, implementations 
should leave with KErrNotSupported.

Where this function is implemented, it should return a CMsvOperation-derived 
object to provide asynchronous control and monitoring of the operation.

@param aSelection Selection of message entries 
@param aStatus The request status to be completed when the operation has finished 
@leave KErrNotSupported The User Interface MTM does not support delete operations, 
or deleting is inappropriate to the current context 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing delete operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	__ASSERT_DEBUG(aSelection.Count(), Panic(EMtuiEmptySelection));

//	const TMsvEntry& context=BaseMtm().Entry().Entry();
//@	__ASSERT_ALWAYS(context.iType.iUid==KUidMsvServiceEntryValue || context.iType.iUid==KUidMsvFolderEntryValue, Panic(EMtuiContextNotFolderOrService));

	return BaseMtm().Entry().DeleteL(aSelection, aStatus);
	}

EXPORT_C CMsvOperation* CBaseMtmUi::UnDeleteFromL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& /*aStatus*/)
/** Undeletes the specified selection.

The default implementation leaves with KErrNotSupported.

@param aSelection Selection of message entries 
@param aStatus The request status to be completed when the operation has finished 
@leave KErrNotSupported The User Interface MTM does not support undelete operations, 
or undeleting is inappropriate to the current context 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing undelete operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
//
// --- Deletes service, which need not be the current context ---
EXPORT_C CMsvOperation* CBaseMtmUi::DeleteServiceL(const TMsvEntry& aService, TRequestStatus& aStatus)
/** Deletes a specified service entry. 

This entry does not have to be the current context. The usual behaviour is to remove the 
service settings, disabling further use of that service. The returned CMsvOperation 
object completes when deleting is complete.

Requirements:

The default implementation calls CMsvEntry::DeleteL() to delete the entry. 
Implementations can override this to provide any additional checking or user 
interaction. If deleting services is not supported, implementations should 
leave with KErrNotSupported.

Where this function is implemented, it should return a CMsvOperation-derived 
object to provide asynchronous control and monitoring of the operation.

@param aService Service to delete 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNotSupported This User Interface MTM does not support delete service 
operations 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing delete operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	__ASSERT_DEBUG(aService.iType.iUid==KUidMsvServiceEntryValue, Panic(EMtuiEntryNotService));

	CMsvEntry* parent=Session().GetEntryL(aService.Parent());
	CleanupStack::PushL(parent);
	CMsvOperation* deleteoperation=parent->DeleteL(aService.Id(), aStatus);
	CleanupStack::PopAndDestroy();// parent
	
	return deleteoperation;
	}


//
// --- Copy and move functions ---
// --- Context should be set to folder or entry of this MTM ---
// --- Default implementations simply call the relevant CMsvEntry functions ---
//
EXPORT_C CMsvOperation* CBaseMtmUi::CopyToL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
// Copies entries in that selection to the current context
// All entries in the selection must have the same parent
/** Copies an entry selection to a remote service. 

All entries in the selection must have the same parent. The current context must be the MTM folder or service 
to which to copy. The returned CMsvOperation object completes when copying 
the entries is complete.

The usual result of this function is to send messages. The exact behaviour varies 
between MTMs: for example, a Fax MTM would transmit a fax when asked to copy a fax 
from a local folder to a fax service. 

Requirements:

The default implementation calls CMsvEntry::CopyL() to do the copy. If message 
copying is not supported, implementations should leave with KErrNotSupported.

Where this function is implemented, it should return a CMsvOperation-derived 
object to provide asynchronous control and monitoring of the operation.

@param aSelection Selection of message entries 
@param aStatus The request status to be completed when the operation has finished 
@leave KErrNotSupported The User Interface MTM does not support copy to operations 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing copy operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	__ASSERT_DEBUG(aSelection.Count(), Panic(EMtuiEmptySelection));

	const TMsvEntry& context=BaseMtm().Entry().Entry();
	__ASSERT_ALWAYS(context.iType.iUid==KUidMsvServiceEntryValue || context.iType.iUid==KUidMsvFolderEntryValue, Panic(EMtuiContextNotFolderOrService));

	CMsvEntry* sourceEntry=Session().GetEntryL(aSelection.At(0));
	CleanupStack::PushL(sourceEntry);

	CMsvOperation* op=sourceEntry->CopyL(aSelection, context.Id(), aStatus);

	CleanupStack::PopAndDestroy();// sourceEntry

	return op;
	}

EXPORT_C CMsvOperation* CBaseMtmUi::MoveToL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
/** Moves an entry selection to a remote service. 

All entries in the selection must have the same parent. The current context must be the 
MTM folder or service to which to move the entries. The returned CMsvOperation object completes 
when moving the entries is complete.

The usual result of this function is to send messages, in a similar way to 
CopyToL(). The difference is that CopyToL() does not remove the original entries. 

The exact behaviour varies between MTMs, as described for CopyToL().

Requirements:

The default implementation calls CMsvEntry::MoveL() to do the move. Implementations 
are likely to be similar to those for CopyToL(), and may indeed share code.

If an MTM does not support message moving, then it should leave with value 
KErrNotSupported.

Where this function is implemented, it should return a CMsvOperation-derived 
object to provide asynchronous control and monitoring of the operation.

@param aSelection Selection of message entries 
@param aStatus The request status to be completed when the operation has finished 
@leave KErrNotSupported The User Interface MTM does not support move to operations 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing move operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	__ASSERT_DEBUG(aSelection.Count(), Panic(EMtuiEmptySelection));

	const TMsvEntry& context=BaseMtm().Entry().Entry();
	__ASSERT_ALWAYS(context.iType.iUid==KUidMsvServiceEntryValue || context.iType.iUid==KUidMsvFolderEntryValue, Panic(EMtuiContextNotFolderOrService));

	CMsvEntry* sourceEntry=Session().GetEntryL(aSelection.At(0));
	CleanupStack::PushL(sourceEntry);

	CMsvOperation* op=sourceEntry->MoveL(aSelection, context.Id(), aStatus);

	CleanupStack::PopAndDestroy();// sourceEntry

	return op;
	}

EXPORT_C CMsvOperation* CBaseMtmUi::CopyFromL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus)
/** Copies an entry selection from a remote service. 

All entries in the selection 
must have the same parent. The current context must be the MTM folder or service 
from which to copy. The returned CMsvOperation object completes when copying 
the entries is complete.

The usual result of this function is to get messages. It is called when the 
user copies and pastes messages from a remote service. The exact behaviour 
varies between MTMs. 

Requirements:

The default implementation calls CMsvEntry::CopyL() to do the copy. If message 
copying is not supported, implementations should leave with KErrNotSupported.

Where this function is implemented, it should return a CMsvOperation-derived 
object to provide asynchronous control and monitoring of the operation.

@param aSelection Selection of message entries 
@param aTargetId The ID of the entry to own the copies 
@param aStatus The request status to be completed when the operation has finished 
@leave KErrNotSupported The User Interface MTM does not support copy from operations 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing copy operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	__ASSERT_DEBUG(aSelection.Count(), Panic(EMtuiEmptySelection));
	return BaseMtm().Entry().CopyL(aSelection, aTargetId, aStatus);
	}

EXPORT_C CMsvOperation* CBaseMtmUi::MoveFromL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus)
/** Moves an entry selection from a remote service. 

All entries in the selection 
must have the same parent. The current context must be the MTM folder or service 
from which to move. The returned CMsvOperation object completes when moving 
the entries is complete.

The usual result of this function is to get messages, in a similar way to 
CopyFromL(). The difference is that CopyFromL() does not remove the original 
entries. 

Requirements:

The default implementation calls CMsvEntry::MoveL() to do the move. Implementations 
are likely to be similar to those for CopyToL(), and may indeed share code.

If an MTM does not support message moving, then it should leave with value 
KErrNotSupported.

Where this function is implemented, it should return a CMsvOperation-derived 
object to provide asynchronous control and monitoring of the operation.

@param aSelection Selection of message entries 
@param aTargetId The entry ID of the remote service 
@param aStatus The request status to be completed when the operation has finished 
@leave KErrNotSupported The User Interface MTM does not support get operations 
@leave Other Dependent on implementation 
@return If successful, this is an asynchronously completing move operation. 
If failed, this is a completed operation, with status set to the relevant 
error code. */
	{
	__ASSERT_DEBUG(aSelection.Count(), Panic(EMtuiEmptySelection));
	return BaseMtm().Entry().MoveL(aSelection, aTargetId, aStatus);
	}

//
// --- Interpret transfer progress ---
//
EXPORT_C TInt CBaseMtmUi::DisplayProgressSummary(const TDesC8& /*aProgress*/) const
/** Displays a message describing the progress of an operation. 

Requirements:

The default implementation simply returns KErrNotSupported. 

Implementations should unpack the passed aProgress buffer, construct from 
this a suitable message, and display it. The implementation of this function 
should share an understanding of the format of the buffer with the implementations 
of CMsvOperation-derived classes.

@param aProgress Progress information obtained from a call to CMsvOperation::Progress() 
@return A system error code */
	{// Default implementation
	return KErrNotSupported;
	}

EXPORT_C TInt CBaseMtmUi::GetProgress(const TDesC8& /*aProgress*/, TBuf<EProgressStringMaxLen>& /*aReturnString*/, TInt& /*aTotalEntryCount*/,
							 TInt& /*aEntriesDone*/, TInt& /*aCurrentEntrySize*/, TInt& /*aCurrentBytesTrans*/) const
/** Obtains progress information description and statistics. A message client application 
can then display this information to the user. 

Requirements:

The default implementation returns KErrNotSupported.

Implementations should unpack the passed aProgress buffer, and fill in the 
return values appropriately. The implementation of this function should share 
an understanding of the format of the buffer with the implementations of CMsvOperation-derived 
classes.

@param aProgress MTM-specific transfer progress information 
@param aReturnString On return, a string describing the progress status 
@param aTotalEntryCount On return, the total number of entries the operation 
is acting upon 
@param aEntriesDone On return, the number of entries which the operation has 
completed on 
@param aCurrentEntrySize On return, the size of the current entry the operation 
is acting upon 
@param aCurrentBytesTrans On return, the number of bytes of the current entry 
which have been completed 
@return A system error code */
	{// Default implementation
	return KErrNotSupported;
	}

//	
// --- RTTI functions ---
//
EXPORT_C TInt CBaseMtmUi::QueryCapability(TUid aCapability, TInt& aResponse)
/** Queries if the MTM supports a particular capability, specified by a UID. 

For MTM-specific UIDs, see the documentation for the relevant MTM.

Requirements:

The default implementation calls iBaseMtm->QueryCapability() (i.e. the corresponding 
function on the Client-side MTM) to do the query. This is sufficient for many 
implementations. 

However, it is expected that each MTM-specific function is indicated by a 
capability. For example, if the MTM supports a function with UID KMtmUiMessagingXYZ, 
then there might be a capability KMtmUiQueryMessagingXYZ. Therefore, if different 
MTM-specific functions are supported by a User Interface MTM and by the corresponding 
Client-side MTM, this function should be implemented to reflect this.

@param aCapability Capability for which to query 
@param aResponse On return, indicates whether the capability is supported, 
or some other property of the capability.. 
@return KErrNone if successful, KErrNotSupported: aCapability is not a recognised 
value 
@see InvokeAsyncFunctionL()
@see InvokeSyncFunctionL() */
	{
	return iBaseMtm.QueryCapability(aCapability, aResponse);
	}

EXPORT_C void CBaseMtmUi::InvokeSyncFunctionL(TInt aFunctionId, const CMsvEntrySelection& aSelection, TDes8& aParameter)
/** Invokes synchronous protocol-specific operations. For asynchronous operations, 
a similar function, InvokeAsyncFunctionL(), is available.

aSelection and aParameter allow data to be passed to the operation. 

Requirements:

The default implementation calls the corresponding function on the Client-side 
MTM (through iBaseMtm). This is sufficient for many implementations. However, 
there may be certain MTM-specific functions which require interaction with 
the user. These should be implemented here.

@param aFunctionId ID of the requested function 
@param aSelection Selection of message entries. This is used if the operation 
requires message entries to work on. 
@param aParameter Buffer containing input and output parameters. The format 
of this is specific to the operation. 
@leave KErrNotSupported aFunctionId is not a recognised function ID 
@leave Other Dependent on implementation */
	{
	iBaseMtm.InvokeSyncFunctionL(aFunctionId, aSelection, aParameter);
	}

EXPORT_C CMsvOperation* CBaseMtmUi::InvokeAsyncFunctionL(TInt aFunctionId, const CMsvEntrySelection& aSelection, TRequestStatus& aCompletionStatus, 
						TDes8& aParameter)
/**
Invokes asynchronous protocol-specific operations. For synchronous operations, a 
similar function, InvokeSyncFunctionL(), is available.

aSelection and aParameter allow data to be passed to the operation. The TRequestStatus 
and CMsvOperation objects are used as normal to control and monitor the operation.

Requirements:

The default implementation calls the corresponding function on the Client-side MTM 
(through iBaseMtm). This is sufficient for many implementations. However, there may 
be certain MTM-specific functions which require interaction with the user. 
These should be implemented here.

InvokeAsyncFunctionL() should return a CMsvOperation-derived object to provide 
asynchronous control and monitoring of the operation.

@return	If successful, this is an asynchronously completing operation. If failed, 
this is a completed operation, with status set to the relevant error code. 		
@param aFunctionId ID of the requested function 
@param aSelection Selection of message entries. This is used if the operation 
requires message entries to work on. 
@param aParameter Buffer containing input and output parameters. The format 
of this is specific to the operation.
@param aCompletionStatus The request status to be completed when the operation has finished 
@leave KErrNotSupported aFunctionId is not a recognised function ID 
@leave Other Dependent on implementation */
	{
	return iBaseMtm.InvokeAsyncFunctionL(aFunctionId, aSelection, aParameter, aCompletionStatus);
	}

//
// CBaseMtmUi - MTMUi base (protected methods)   //
//
EXPORT_C CBaseMtmUi::CBaseMtmUi(CBaseMtm& aBaseMtm, CRegisteredMtmDll& aRegisteredMtmDll)
	:	iBaseMtm(aBaseMtm), 
		iCoeEnv(CCoeEnv::Static()),
		iRegisteredMtmDll(aRegisteredMtmDll)
/** Constructor, which initialises member variables from the passed arguments. 

Client applications do not use this function. It is relevant only to implementers of derived classes.

The value of aBaseMtm is stored in the protected data member iBaseMtm, so 
it can be accessed by derived classes.

Derived classes can implement a constructor to perform any additional MTM-specific 
setup that can be safely carried out in a constructor. Such constructors must 
call the base class constructor function.

@param aBaseMtm The CBaseMtm requesting the object 
@param aRegisteredMtmDll Registration data for the DLL */
	{
	__DECLARE_NAME(_S("CBaseMtmUi"));
	}

EXPORT_C void CBaseMtmUi::ConstructL()
/** Second-phase constructor.

Client applications do not use this function. It is relevant only to implementers 
of derived classes.

Requirements:

Derived classes implement two-phase construction functions (NewL(), ConstructL()) 
to create a new instance of the object, in which any dynamic allocation should 
be performed. ConstructL() should be called from the NewL() function of the 
derived class.

The default implementation of this function loads resources required by the 
base class. Derived classes can implement this function to perform any additional 
MTM-specific second stage construction tasks that they require. Implementations 
must call the base class ConstructL() function.

Concrete MTMs must also implement a factory function by which a MTM registry 
can request an instance of the class.

@see MtmUiFactoryFunctionL */
	{
	// Get resource file
	TFileName resourceFileName;
 	GetResourceFileName(resourceFileName);
	
	MsvUtils::AddPathAndExtensionToResFileL(resourceFileName);
 
	//Now look for the nearest filename--will search the system drive if specified
	BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(), resourceFileName);

	TRAPD(ret, iResourceFileOffset=iCoeEnv->AddResourceFileL(resourceFileName));
	User::LeaveIfError(ret);
	}

EXPORT_C CMsvSession& CBaseMtmUi::Session() const
/** Gets a reference to the session object used by the Client-side MTM that requested 
the User Interface MTM.

@return Session object used by the Client-side MTM */
	{ 
	return iBaseMtm.Session(); 
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
EXPORT_C TAny* CBaseMtmUi::GetInterface(TUid /*aUid*/)
 	{
	return NULL;
 	}
