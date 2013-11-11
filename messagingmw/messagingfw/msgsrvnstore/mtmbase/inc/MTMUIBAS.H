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

#if !defined(__MTMUIBAS_H__)
#define __MTMUIBAS_H__

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif
#if !defined(__MTCLBASE_H__)
#include <mtclbase.h>
#endif

// Forward references
class CMsvSession;
class CMsvOperation;
class CCoeEnv;

//
// CBaseMtmUi - MTMUi base API   //
//
class CBaseMtmUi : public CBase
/** Provides MTM-specific user interaction, such as editing, viewing, deleting, 
and copying of message entries. 

Message client applications use the class to access such functionality polymorphically. 
MTM implementers implement a derived class to provide such functionality for 
their message protocol.

The base class is largely an interface definition: it implements little important 
functionality itself. For implementers, this documentation defines conditions 
that any implementation must fulfil, plus suggested semantics for behaviour. 
This still leaves many decisions in the hands of the implementer. A key design 
time activity is to define how the particular characteristics of the implemented 
protocol are best mapped to the interface. As this class is concerned with 
user interfaces, implementers should be sensitive to the look and feel of 
the target phone.

Many functions are required to return an instance of a CMsvOperation-derived 
class to provide asynchronous control and monitoring to the caller of the 
operation being performed. 

The following are some significant groups of functions:

Entry manipulation functions: OpenL(), CloseL(), EditL(), and ViewL() are 
all available in two overloaded versions. The first version operates on the 
current context. The second version operates upon groups of entries specified 
by a CMsvEntrySelection argument. All entries must be in same folder and all 
of the correct MTM type. The context may change after calling these functions. 
Concrete User Interface MTMs can legally ignore any of the entries within 
the selection - many may, for example, simply open the first entry.

Copying and moving functions: the CopyToL(), CopyFromL(), MoveToL(), and MoveFromL() 
functions are concerned with copying and moving entries to and from remote 
servers. MTMs can implement these functions to provide any user interaction 
needed before copying or moving can proceed, such as dialogs to confirm settings; 
and, any protocol-specific setup steps that can be done at this stage: for 
example, a fax MTM could render messages into the required image format.

Progress information functions: DisplayProgressSummary() and GetProgress() 
convert MTM-specific progress information about some asynchronous operation, 
such as message sending, to a human-readable form. The progress information 
to pass into these functions is, in general, obtained from an on-going CMsvOperation 
by calling CMsvOperation::Progress(). The nature of any information provided 
is MTM-specific. 

MTM-specific UI functions: MTM components can offer protocol-specific functionality 
not provided by base class interface functions. MTM components define IDs 
that correspond to each protocol-specific operation offered, and implement 
the InvokeSyncFunctionL() and InvokeAsyncFunctionL() functions to allow clients 
to access these operations by passing in the appropriate ID. Two functions 
are provided to allow the MTM component to offer both synchronous and asynchronous 
functionality. Message client applications can dynamically add user-interface 
features for these operations using CBaseMtmUiData::MtmSpecificFunctions(). 
MTM developers should document the IDs if they wish to make the operations 
available to clients. 
@publishedAll
@released
*/
	{
public:
	//  --- enums ---
	/** Progress information buffer length. */
	enum {
	/** Defines the maximum buffer length used in GetProgress() to hold progress information. */
	EProgressStringMaxLen=255};
	//
	// --- Set up functionality ---
	IMPORT_C void SetPreferences(TUint aFlags);
	IMPORT_C TUint Preferences() const;
	//
	// --- Destruction ---
	IMPORT_C virtual ~CBaseMtmUi();
	//
	// --- Accessor functions ---
	IMPORT_C TUid Type() const;
	IMPORT_C CBaseMtm& BaseMtm() const;
	//
	// --- Functions NOT dependent on the current context ---
	IMPORT_C virtual CMsvOperation* CreateL(const TMsvEntry& aEntry, CMsvEntry& aParent, TRequestStatus& aStatus);
	//
	// --- Functions dependent on the current context ---
	/** Opens an entry. 
	
	The behaviour that this implies is dependent on the current context:
	
	1. for message contexts, OpenL() is the equivalent of EditL(), or for read-only 
	messages, ViewL(). Whether opening of remote messages is allowed is MTM-specific.
	
	2. for service contexts, the function may initiate a connection to obtain the 
	service contents from the server, completing asynchronously
	
	3. for remote folder contexts, dependent on implementation, the function may 
	initiate a connection to obtain the folder contents from the server, completing 
	asynchronously
	
	The returned CMsvOperation object completes when opening is complete.
	
	Requirements:
	
	If opening entries is not supported, implementations should leave with KErrNotSupported. 
	Otherwise, implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can open the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	3. for remote messages, typically, copy the messages to the Inbox
	
	@param aStatus The request status to be completed when the operation has finished 
	
	@leave KErrNotSupported The User Interface MTM does not support open operations, 
	or opening is inappropriate to the current context 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing open operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* OpenL(TRequestStatus& aStatus) = 0; 
	/** Closes an entry. The behaviour that this implies is dependent on the current 
	context:
	
	1. for message contexts, this usually closes any launched editors or viewers
	
	2. for folder contexts, leaves with KErrNotSupported
	
	3. for service contexts, causes closure of a connection to a server
	
	The returned CMsvOperation object completes when closing is complete.
	
	Requirements:
	
	Implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can close the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	@param aStatus The request status to be completed when the operation has finished 
	
	@leave KErrNotSupported The User Interface MTM does not support close operations, 
	or closing is inappropriate to the current context 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing close operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* CloseL(TRequestStatus& aStatus) = 0;
	/** Edits an entry. 
	
	The behaviour that this implies is dependent on the current 
	context:
	
	1. for message contexts, this usually launches the appropriate message editor, 
	or returns status KErrReadOnly if the message is not editable
	
	2. for remote folder contexts, launches a settings dialog, for example to rename 
	the folder, completing asynchronously
	
	3. for service contexts, launches a service settings dialog, completing asynchronously
	
	The returned CMsvOperation object completes when editing is complete.
	
	Requirements:
	
	Implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can edit the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	3. for remote messages, typically, copy the messages to the Inbox
	
	4. follow the leave protocols described in Leave considerations below
	
	@param aStatus The request status to be completed when the operation has finished 
	@leave KErrNotSupported The User Interface MTM does not support edit operations, 
	or editing is inappropriate to the current context 
	@leave KErrReadOnly Context is read-only 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing edit operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* EditL(TRequestStatus& aStatus) = 0;// Launches editor/settings dialog as appropriate  
	/** Views an entry. 
	
	For message contexts, this usually launches the appropriate 
	message viewer. Other context types leave with KErrNotSupported. The returned 
	CMsvOperation object completes when viewing is complete.
	
	Requirements:
	
	Implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can view the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	@param aStatus The request status to be completed when the operation has finished 
	@leave KErrNotSupported The User Interface MTM does not support view operations, 
	or viewing is inappropriate to the current context 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing view operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* ViewL(TRequestStatus& aStatus) = 0;// Launches viewer/settings dialog as appropriate  
	//
	// --- Actions upon message selections ---
	// --- Selections must be in same folder and all of the correct MTM type ---
	// --- Context may change after calling these functions ---
	/** Opens a selection of entries. 
	
	The behaviour that this implies is dependent on the current context:
	
	1. for message contexts, OpenL() is the equivalent of EditL(), or for read-only 
	messages, ViewL(). Whether opening of remote messages is allowed is MTM-specific.
	
	2. for service contexts, the function may initiate a connection to obtain the 
	service contents from the server, completing asynchronously
	
	3. for remote folder contexts, dependent on implementation, the function may 
	initiate a connection to obtain the folder contents from the server, completing 
	asynchronously
	
	The returned CMsvOperation object completes when opening is complete.
	
	Requirements:
	
	If opening entries is not supported, implementations should leave with KErrNotSupported. 
	Otherwise, implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can open the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	3. for remote messages, typically, copy the messages to the Inbox
	
	@param aStatus The request status to be completed when the operation has finished 
	@param aSelection Selection of message entries 
	@leave KErrNotSupported The User Interface MTM does not support open operations, 
	or opening is inappropriate to the current context 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing open operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* OpenL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection) = 0; 
	/** Closes a selection of entries. 
	
	The behaviour that this implies is dependent on the current context:
	
	1. for message contexts, this usually closes any launched editors or viewers
	
	2. for folder contexts, leaves with KErrNotSupported
	
	3. for service contexts, causes closure of a connection to a server
	
	The returned CMsvOperation object completes when closing is complete.
	
	Requirements:
	
	Implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can close the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	@param aStatus The request status to be completed when the operation has finished 
	@param aSelection Selection of message entries 
	@leave KErrNotSupported The User Interface MTM does not support close operations, 
	or closing is inappropriate to the current context 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing close operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* CloseL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection) = 0;
	/** Edits a selection of entries. 
	
	The behaviour that this implies is dependent on the current context:
	
	1. for message contexts, this usually launches the appropriate message editor, 
	or returns status KErrReadOnly if the message is not editable
	
	2. for remote folder contexts, launches a settings dialog, for example to rename 
	the folder, completing asynchronously
	
	3. for service contexts, launches a service settings dialog, completing asynchronously
	
	The returned CMsvOperation object completes when editing is complete.
	
	Requirements:
	
	Implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can edit the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	3. for remote messages, typically, copy the messages to the Inbox
	
	4. follow the leave protocols described in Leave considerations below
	
	@param aStatus The request status to be completed when the operation has finished 
	@param aSelection Selection of message entries 
	@leave KErrNotSupported The User Interface MTM does not support edit operations, 
	or editing is inappropriate to the current context 
	@leave KErrReadOnly Context is read-only 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing edit operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* EditL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection) = 0;// Launches editor/settings dialog as appropriate  
	/** Views a selection of entries. 
	
	For message contexts, this usually launches the appropriate 
	message viewer. Other context types leave with KErrNotSupported. The returned 
	CMsvOperation object completes when viewing is complete.
	
	Requirements:
	
	Implementations should:
	
	1. check aEntry.iType.iUid to ensure that they can view the requested type of 
	entry and leave with code KErrNotSupported if they cannot
	
	2. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	@param aStatus The request status to be completed when the operation has finished 
	@param aSelection Selection of message entries 
	@leave KErrNotSupported The User Interface MTM does not support view operations, 
	or viewing is inappropriate to the current context 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing view operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* ViewL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection) = 0;// Launches viewer/settings dialog as appropriate  
	/** Cancels the sending of specified messages.
	
	@param aStatus The request status to be completed when the operation has finished 	
	@param aSelection Selection of message entries 
	@leave KErrNotSupported The User Interface MTM does not support cancel operations, 
	or cancelling is inappropriate to the current context 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing cancel operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* CancelL(TRequestStatus& aStatus, const CMsvEntrySelection& aSelection) = 0;
	//
	// --- Deletion ---
	// --- Deletes entries from the current context, which must be a folder or service of the relevant MTM ---
	IMPORT_C virtual CMsvOperation* DeleteFromL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);//Deletes selection from current context
	// --- Un-deletes entries from the current context, which must be a folder or service of the relevant MTM ---
	IMPORT_C virtual CMsvOperation* UnDeleteFromL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);//Un-deletes selection from current context
	// --- Deletes service, which need not be the current context ---
	IMPORT_C virtual CMsvOperation* DeleteServiceL(const TMsvEntry& aService, TRequestStatus& aStatus); // default implementation simply delegates to the server
	//
	// --- Message responding ---
	/** Creates a reply message to the current message context, and allows the user 
	to edit it through a message-type specific editor. 
	
	Some MTMs may support inclusion of elements, specified by aPartlist, from the original message in the reply. 
	The parent for the new entry is specified in aDestination. The returned CMsvOperation 
	object completes when editing the reply is complete. On completion, the context 
	is set to the reply message.
	
	Requirements:
	
	A typical implementation for this function would include the following steps: 
	
	1. create a new reply entry by calling CBaseMtm::ReplyL()
	
	2. call EditL() allow the user to edit the reply
	
	3. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	If message replies are not supported, implementations should leave with KErrNotSupported.
	
	@param aDestination The entry to which to assign the reply 
	@param aPartlist Defines the parts which are to be copied from the original 
	message into the reply. See CBaseMtm::ReplyL() for details. 
	@param aCompletionStatus The request status to be completed when the operation 
	has finished 
	@leave KErrNotSupported The User Interface MTM does not support reply operations 
	@leave Other Dependent on implementation 
	@return If successful, this is an asynchronously completing reply operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* ReplyL(TMsvId aDestination, TMsvPartList aPartlist, TRequestStatus& aCompletionStatus) = 0;
	/**
	Creates a forwarded message from the current message context, and allows the user to edit it through a message-type 
	specific editor. 
	
	Some MTMs may support inclusion of elements, specified by aPartlist, from the original message in the 
	forwarded message. The parent for the new entry is specified in aDestination. The returned CMsvOperation 
	object completes when editing the forwarded message is complete. On completion, the context is set to the forwarded message.

	Requirements:

	A typical implementation for this function would include the following steps:

	1. create a new forwarded message entry by calling CBaseMtm::ForwardL()

	2. call EditL() allow the user to edit the forwarded message

	3. return a CMsvOperation-derived object to provide asynchronous control and monitoring of the operation

	If forwarded messages are not supported, implementations should leave with KErrNotSupported.

	@return	If successful, this is an asynchronously completing forward message operation. If failed, 
	this is a completed operation, with status set to the relevant error code. 	
	@param aDestination The entry to which to assign the forwarded message 
	@param aPartList Defines the parts which are to be copied from the original message into the forwarded message 
	@param aCompletionStatus The request status to be completed when the operation has finished 
	@leave KErrNotSupported The User Interface MTM does not support message forwarding operations 
	@leave Other Dependent on implementation 
	*/
	virtual CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus) = 0;

	//
	// --- Copy and move functions ---
	// --- Context should be set to folder or entry of this MTM ---
	// --- Default implementations imply call the relevant CMsvEntry functions ---
	IMPORT_C virtual CMsvOperation* CopyToL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);// Context should be MTM folder/service to copy to
	IMPORT_C virtual CMsvOperation* MoveToL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);// Context should be MTM folder/service to move to
	IMPORT_C virtual CMsvOperation* CopyFromL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus);// Context should be MTM folder/service to copy from
	IMPORT_C virtual CMsvOperation* MoveFromL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus);// Context should be MTM folder/service to move from
	//
	// --- Interpret transfer progress ---
	IMPORT_C virtual TInt DisplayProgressSummary(const TDesC8& aProgress) const;// Used for display operation summary (usually completed operations). Default implementation returns KErrNotSupported. 
	IMPORT_C virtual TInt GetProgress(const TDesC8& aProgress, TBuf<EProgressStringMaxLen>& aReturnString, TInt& aTotalEntryCount, TInt& aEntriesDone,
		TInt& aCurrentEntrySize, TInt& aCurrentBytesTrans) const;// Default implementation returns KErrNotSupported
	//	
	// --- RTTI functions ---
	IMPORT_C virtual TInt QueryCapability(TUid aCapability, TInt& aResponse);
	IMPORT_C virtual void InvokeSyncFunctionL(TInt aFunctionId, const CMsvEntrySelection& aSelection, TDes8& aParameter);
	IMPORT_C virtual CMsvOperation* InvokeAsyncFunctionL(TInt aFunctionId, const CMsvEntrySelection& aSelection, TRequestStatus& aCompletionStatus, 
		TDes8& aParameter);

protected:
	// --- Construction ---
	IMPORT_C CBaseMtmUi(CBaseMtm& aBaseMtm, CRegisteredMtmDll& aRegisteredMtmDll);
	IMPORT_C virtual void ConstructL();
	/** Sets the full path and name of the resource file associated with the User Interface MTM. 
	
	Client applications do not use this function. It is relevant only to implementers of derived classes.
	
	Requirements:
	
	The function is called by CBaseMtmUi::ConstructL(), which then loads the resource 
	file.
	
	@param aFileName Filename buffer to be filled with the resource file path 
	and name */
	virtual void GetResourceFileName(TFileName& aFileName) const = 0;
	//
	// --- Utility functions ---
	IMPORT_C CMsvSession& Session() const;

	// Method used for extension: called by non virtual methods that need 
	// to have a polymorphic behaviour.
	IMPORT_C virtual TAny* GetInterface(TUid aUid);

protected:
	// --- Data ---
	/** The CBaseMtm-derived object (Client-side MTM) that requested the User Interface 
	MTM. It is set by the constructor. */
	CBaseMtm&			iBaseMtm;
	/** Control environment object. It is set by the constructor. */
	CCoeEnv*			iCoeEnv;
	/** This flag records the preferences as set by SetPreferences(). */
	TUint				iFlags;

private:
	// --- Data ---
	CRegisteredMtmDll&	iRegisteredMtmDll;
	TInt				iResourceFileOffset;
	// Extra data member to allow for future extensions
	TAny* iExtensionData;
	};


#endif // __MTMUIBAS_H__
