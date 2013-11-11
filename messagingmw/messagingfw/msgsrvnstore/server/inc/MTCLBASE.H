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
// MTCLBASE.H
//
/**
 * @file 
 * @publishedAll
 * @released
 */

#ifndef __MTCLBASE_H__
#define __MTCLBASE_H__

#include <e32base.h>		
#include <badesca.h>
#include <msvapi.h>

#include <cmsvattachment.h>
#include <msvstd.hrh>
#include <cmsvrecipientlist.h>

class CMsvAttachmentWaiter;

/** Specifies one or more parts of a message in message-forwarding, message-reply, 
and message-validation functions.

Standard message parts are defined by the constants that can be added (or 
bitwise-or'ed) together to specify multiple parts. Some message parts may 
not be meaningful for some MTMs.

Specifies find attributes which modify the behaviour of the search text utility 
provided by the Global Find API. This does not apply to v5.1 nor to v5. */
typedef TUint TMsvPartList;

class CParaFormatLayer;
class CCharFormatLayer;


//
// Prototype of expected factory function

typedef CBaseMtm* MtmFactoryFunctionL(CRegisteredMtmDll&, CMsvSession&);


/** Specifies function IDs for standard MTM functions.

The function IDs are specified such that they do not coincide
with the free area defined for MTM commands.

If MTMs also offer their own equivalents of these functions,
they may implement these function IDs by considering the
two IDs to be the same in their client MTM.

MTMs that support SendAs must support SendMessage.
*/
enum TMtmStandardAsyncCommands
	{
	KMTMStandardFunctionsSendMessage = KMtmFirstFreeStandardMtmFunctionId
	};


/***********************************************
***********************************************/
class CBaseMtm : public CBase, public MMsvEntryObserver
/** Provides a high-level interface for accessing and manipulating a Message Server 
entry. 

Message client applications use the class to access such functionality polymorphically. 
MTM implementers implement a derived class to provide such functionality for 
their message protocol. 

The following are some significant groups of functions:

Context functions: the SetCurrentEntryL() and SwitchCurrentEntryL() functions 
change the context - the entry on which later actions are performed. After 
creating a new Client-side MTM object, a message client application should 
set an initial context before using other functions. Note that: any changes 
made to an existing context are not automatically saved: the message client 
application should ensure this itself by calling SaveMessageL(); no message 
data for the new context is retrieved from the Message Server, to retrieve 
entry data, call LoadMessageL() after setting the context; calling Body() 
immediately after setting the context returns an empty CRichText object: this 
is because the private cache of context body text that the base class maintains 
is re-initialised to an empty value. MTM implementers should note that the 
virtual ContextEntrySwitched() is called from these functions to allow derived 
classes to also clear any caches of MTM-specific entry data.

Store and restore entry data functions: the changes that a message client 
application makes to a message context through Client-side MTM functions, 
such as altering the body text obtained through Body(), are, for efficiency, 
cached in memory by a Client-side MTM. The message store and restore functions 
are concerned with transferring data between that cache and committed storage. 
Note that, unlike message contexts, message client applications are not expected 
to manipulate directly service contexts. Instead, the corresponding User Interface 
MTM will provide dialogs to allow the user to alter service settings, and 
call Client-side MTM functions to handle their retrieval and storage. Symbian 
OS v5 expects the base class functions to handle storage and retrieval for 
both message and service contexts, and their implementations must detect what 
the context is, and act accordingly. Post-v5, the API is clarified by restricting 
the base class functions to handle message contexts only. To handle service 
contexts, a Client-side MTM must define its own functions for the User Interface 
MTM to call.

Store and restore body text functions: the base class maintains a private 
CRichText object cache to store the body text for the current context. This 
can be accessed for reading and writing by message client applications through 
Body(). StoreBodyL() and RestoreBodyL() encapsulate for implementers of derived 
classes the retrieval and storage of this CRichText object to a CMsvStore.

Address list functions: the format and storage of message addresses is MTM-specific. 
AddresseeList(), AddAddresseeL(), and RemoveAddressee() are designed to allow 
clients with no MTM-specific knowledge to access address information in a 
generic way. The base class has a protected data member iAddresseeList, an 
array of descriptors, which these functions manipulate. Implementations should 
save the address information to the appropriate place in the message store 
when the message is stored.

MTM-specific functionality: MTM components can offer protocol-specific functionality 
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
	IMPORT_C ~CBaseMtm();
	IMPORT_C TUid Type() const;
	//
	// context related
	IMPORT_C void SetCurrentEntryL(CMsvEntry* aEntry);
	IMPORT_C void SwitchCurrentEntryL(TMsvId aId);
	IMPORT_C CMsvEntry& Entry() const;
	IMPORT_C TBool HasContext() const;
	//
	//
	/** Commits cached changes to the storage controlled by the Message Server.
	
	It can only be called on message contexts. It should be called in the following circumstances:
	
	1. to preserve changes when the context is changed, or when the Client-side MTM 
	object is deleted 
	
	2. to enable other parts of the Messaging to access the updated entry, as required, 
	for example, before sending a message 
	
	Requirements:
	
	An implementation must update the store and index entry relating to the message 
	context. Typically, the message store should be opened for editing with CMsvEntry::EditStoreL(). 
	It should be updated as follows:
	
	1. body text: call StoreBodyL() to update the store's body text stream
	
	2. address list: update the appropriate MTM-specific area of the store from the 
	data in iAddresseeList
	
	3. subject: if supported, update the appropriate MTM-specific area of the store 
	from the private cache set by SetSubjectL()
	
	Changes can then be committed to the store with CMsvStore::CommitL().
	
	The index entry should also be updated to reflect changes. Possible fields 
	that may need updating include: Description (for subject changes); Details 
	and Multiple Recipients (for recipient changes); and Size. Index entry changes 
	are committed using CMsvEntry::ChangeL().
	
	The function should panic for non-message contexts. */
	virtual void SaveMessageL()=0; 
	/** Loads the cache with the message data for the current context.
	
	It can only be called on message contexts. 
	
	It is typically used after the context has been set with SetCurrentEntryL() 
	or SwitchCurrentEntryL(). CBaseMtm functions to manipulate the entry can only 
	be called after this function has been called.
	
	Requirements:
	
	An implementation must restore the store and index entry relating to the message 
	context. Typically, the message store should be opened for reading with CMsvEntry::ReadStoreL(). 
	It should be then be read to set the following:
	
	1. body text: call RestoreBodyL() to update the cached body text
	
	2. address list: read the appropriate MTM-specific area of the store to update 
	iAddresseeList
	
	3. subject: if supported, read the appropriate MTM-specific area of the store 
	and update the cache with SetSubjectL()
	
	The function should panic for non-message contexts. */
	virtual void LoadMessageL()=0;
	/** Validates the current message context. 
	
	The precise validation performed is specific to the MTM, but, typically, checks 
	that the specified message parts are well-formed. 
	
	Requirements:
	
	Implementation of this function is highly protocol-specific. A minimum step 
	is to check that the current context is a message.
	
	@param aPartList Indicates the message parts for which validation is requested 
	
	@return If valid, KErrNone If invalid, identifies the invalid part(s). The 
	error value is the bitmask of the TMsvPartList IDs for each invalid part */
	virtual TMsvPartList ValidateMessage(TMsvPartList aPartList)=0;
	/** Searches the specified message part(s) for the plain-text version of the text 
	to be found. 
	
	If the specified part list indicates a part that is not supported, or is not 
	present in the current message, the function behaves as if the specified part 
	exists but does not contain the required text.
	
	Requirements:
	
	The parts of the entry for which searching is allowed is implementation specific. 
	If no searching is supported, always return 0.
	
	@param aTextToFind The plain-text version of the text to be found. 
	@param aPartList Indicates the message parts which should be searched. 
	@return If the text was not found, or searching is unsupported, 0. If the text 
	was found, a bitmask of the TMsvPartList IDs for each part in which the text 
	was present. */
	virtual TMsvPartList Find(const TDesC& aTextToFind, TMsvPartList aPartList)=0;
	//
	//
	/** Creates a reply message to the current message context. 
	
	Some MTMs may support inclusion of elements, specified by aPartlist, from 
	the original message in the reply. The parent for the new entry is specified 
	in aDestination. 
	
	The returned CMsvOperation object completes when creating the reply is complete. 
	On completion, the context is set to the reply message.
	
	Requirements:
	
	A typical implementation for this function would include the following steps: 
	
	1. create a new message in the specified destination by calling CMsvEntry::CreateL()
	
	2. set the entry index values as appropriate
	
	3. set the properties of the message as required. The normal minimum is to set 
	the address to the sender of the original message. An implementation may also 
	follow the options specified in aPartlist to set other properties, for example, 
	to include the original message text.
	
	4. set the context to the reply 
	
	5. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	If message replies are not supported, implementations should leave with KErrNotSupported.
	
	The implementation of this function may be similar to that of ForwardL(), 
	allowing opportunities for code sharing.
	
	@param aDestination The entry to which to assign the reply 
	@param aPartlist Defines the parts that are to be copied from the original 
	message into the reply 
	@param aCompletionStatus The request status to be completed when the operation 
	has finished 
	@leave KErrNotSupported The Client-side MTM does not support reply operations 
	@leave Other leave codes Dependent on implementation 
	@return If successful, this is an asynchronously completing reply operation. 
	If failed, this is a completed operation, with status set to the relevant 
	error code. */
	virtual CMsvOperation* ReplyL(TMsvId aDestination, TMsvPartList aPartlist, TRequestStatus& aCompletionStatus)=0;
	/** Creates a forwarded message from the current message context. 
	
	Some MTMs may 
	support inclusion of elements, specified by aPartlist, from the original message 
	in the forwarded message. The parent for the new entry is specified in aDestination. 
	The returned CMsvOperation object completes when editing the forwarded message 
	is complete. On completion, the context is set to the forwarded message.
	
	Requirements:
	
	A typical implementation for this function would include the following steps: 
	
	1. create a new message in the specified destination by calling CMsvEntry::CreateL()
	
	2. set the entry index values as appropriate
	
	3. set message content as required. The normal minimum is to include the text 
	of the original message. An implementation may also follow the options specified 
	in aPartlist to include other properties of the original message.
	
	4. set the context to the reply 
	
	5. return a CMsvOperation-derived object to provide asynchronous control and 
	monitoring of the operation
	
	If forwarded messages are not supported, implementations should leave with 
	KErrNotSupported.
	
	The implementation of this function may be similar to that of ReplyL(), allowing 
	opportunities for code sharing.
	
	@param aDestination The entry to which to assign the forwarded message 
	@param aPartList Defines the parts that are to be copied from the original 
	message into the forwarded message 
	@param aCompletionStatus The request status to be completed when the operation 
	has finished 
	@leave KErrNotSupported The Client-side MTM does not support creation of forwarded 
	messages 
	@leave Other leave codes Dependent on implementation 
	@return If successful, this is an asynchronously completing forward message 
	operation. If failed, this is a completed operation, with status set to the 
	relevant error code. */
	virtual CMsvOperation* ForwardL(TMsvId aDestination, TMsvPartList aPartList, TRequestStatus& aCompletionStatus)=0;
	//
	// addresssee list (used by objects with no MTM knowledge)
	IMPORT_C const CMsvRecipientList& AddresseeList() const;

	/** Adds an addressee for the current context.
	
	Addresses are not validated by checking their format by this function. Usually 
	that is performed by calling ValidateMessage().
	
	Requirements:
	
	Implementations should append the address to the object's address cache 
	in the protected iAddresseeList data member. Some implementations may also 
	wish to store addresses in an internal data structure appropriate for the 
	protocol, for example, a class holding message header information. 
	
	@param aRealAddress String representing an address to be added to the list 
	for the current message 
	@leave KErrNotSupported The message already has the maximum number of addressees 
	
	@leave Other leave codes Dependent on implementation */
	virtual void AddAddresseeL(const TDesC& aRealAddress)=0;
	/** Adds an addressee for the current context, and enables the client to specify 
	an alias, which may be useful for some protocols. For example, for fax, if 
	the address is the fax number, the alias could supply the recipient's name. 
	
	Addresses are not validated by checking their format by this function. Usually 
	that is performed by calling ValidateMessage().
	
	Requirements:
	
	Implementations should append the address to the object's address cache 
	in the protected iAddresseeList data member. Some implementations may also 
	wish to store addresses in an internal data structure appropriate for the 
	protocol, for example, a class holding message header information. 
	
	@param aRealAddress String representing an address to be added to the list 
	for the current message 
	@param aAlias Alias information 
	@leave KErrNotSupported The message already has the maximum number of addressees 
	
	@leave Other leave codes Dependent on implementation */
	virtual void AddAddresseeL(const TDesC& aRealAddress, const TDesC& aAlias)=0;
	
	IMPORT_C virtual void AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress);
	IMPORT_C virtual void AddAddresseeL(TMsvRecipientType aType, const TDesC& aRealAddress, const TDesC& aAlias);
	
	/** Removes an address from the current address list. The address is specified 
	by a zero-based index into the address list. If the index is not known, applications 
	can use AddresseeList() to retrieve the entire list to find the item.
	
	Requirements:
	
	Implementations should call iAddresseeList->Delete(aIndex) to remove the address 
	from in the address list protected data member.
	
	@param aIndex Index of address to be removed */
	virtual void RemoveAddressee(TInt aIndex)=0;
	//
	// standard data accessor/mutators
	IMPORT_C CRichText& Body();
	IMPORT_C const CRichText& Body() const;
	IMPORT_C virtual void SetSubjectL(const TDesC& aSubject); // default leaves with KErrNotSupported
	IMPORT_C virtual const TPtrC SubjectL() const; // default leaves with KErrNotSupported
	//
	// RTTI functions
	IMPORT_C virtual TInt QueryCapability(TUid aCapability, TInt& aResponse); // default returns KErrNotSupported
	/** Invokes synchronous protocol-specific operations. For asynchronous operations, 
	a similar function, InvokeAsyncFunctionL(), is available.
	
	aSelection and aParameter allow data to be passed to the operation. 
	
	Requirements:
	
	For functionality that requires message transport access, such as making a 
	connection, the implementation should pass the request onto the corresponding 
	Server-side MTM. This is done through calling CMsvSession::TransferCommandL(). 
	Implementations may also provide protocol-specific functions themselves if 
	this is useful.
	
	@param aFunctionId ID of the requested operation 
	@param aSelection Selection of message entries. This is used if the operation 
	requires message entries to work on. 
	@param aParameter Buffer containing input and output parameters. The format 
	of this is specific to the operation. 
	@leave KErrNotSupported aFunctionId is not a recognised operation ID 
	@leave Other leave codes Dependent on implementation */
	virtual void InvokeSyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter)=0;
	/** Invokes asynchronous protocol-specific operations. For synchronous operations, 
	a similar function, InvokeSyncFunctionL(), is available.
	
	aSelection and aParameter allow data to be passed to the operation. The TRequestStatus 
	and CMsvOperation objects are used as normal to control and monitor the operation.
	
	Requirements:
	
	For functionality that requires message transport access, such as making a 
	connection, the implementation should pass the request onto the corresponding 
	Server-side MTM. This is done through calling CMsvSession::TransferCommandL(). 
	Implementations may also provide protocol-specific functions themselves if 
	this is useful.
	
	InvokeAsyncFunctionL() should return a CMsvOperation-derived object to provide 
	asynchronous control and monitoring of the operation. If CMsvSession::TransferCommandL() 
	is called, this should be the CMsvOperation object returned by that function.
	
	@param aFunctionId ID of the requested operation 
	@param aSelection Selection of message entries. This is used if the operation 
	requires message entries to work on. 
	@param aParameter Buffer containing input and output parameters. The format 
	of this is specific to the operation. 
	@param aCompletionStatus The request status to be completed when the operation 
	has finished 
	@leave KErrNotSupported aFunctionId is not a recognised operation ID 
	@leave Other leave codes Dependent on implementation 
	@return If successful, this is an asynchronously completing operation. If failed, 
	this is a completed operation, with status set to the relevant error code. */
	virtual CMsvOperation* InvokeAsyncFunctionL(TInt aFunctionId,const CMsvEntrySelection& aSelection, TDes8& aParameter, TRequestStatus& aCompletionStatus)=0;
	//
	IMPORT_C CMsvSession& Session();

	// Attachment functions to support the SendAs API
	virtual inline void Filler1() {};
	virtual inline void Filler2() {};
	IMPORT_C virtual void AddAttachmentL(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus);
	IMPORT_C virtual void AddAttachmentL(RFile& aFile, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus);
	IMPORT_C virtual void AddLinkedAttachmentL(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus);
	IMPORT_C virtual void AddEntryAsAttachmentL(TMsvId aAttachmentId, TRequestStatus& aStatus);
	IMPORT_C virtual void CreateAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, const TDesC8& aMimeType, TUint aCharset, TRequestStatus& aStatus);
	IMPORT_C virtual void CancelAttachmentOperation();

	IMPORT_C virtual void CreateMessageL(TMsvId aServiceId);

	// BIO functions to support the SendAs API
	// Entry().Entry().iBioType will be set by SendAs if this function does not leave.
	// The default implementation in CBaseMtm is to do nothing.
	IMPORT_C virtual void BioTypeChangedL(TUid aBioTypeUid);
	
 	IMPORT_C virtual TMsvId DefaultServiceL() const;
 	IMPORT_C virtual void RemoveDefaultServiceL();
 	IMPORT_C virtual void ChangeDefaultServiceL(const TMsvId& aService);
 	
 	//For setting the character encoding value, options are 7-bit, 8-bit and 16-bit Unicode.
 	IMPORT_C TInt SetMessageCharacterSet(TUint aCharSet);
	IMPORT_C void SetExtensionData(TAny* aSortData);
	IMPORT_C TAny* GetExtensionData();
	
protected:
	IMPORT_C CBaseMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession);
	//
	IMPORT_C void StoreBodyL(CMsvStore& aStore);
	IMPORT_C void RestoreBodyL(CMsvStore& aStore);
	//
	/** Called by the base class functions SwitchCurrentEntryL() and SetCurrentEntryL() 
	when the context is changed to another entry. 
	
	Client applications do not use this function. It is relevant only to implementers 
	of derived classes.
	
	Requirements: 
	
	An implementation should clear:
	
	1. address data stored in iAddresseeList
	
	2. any caches of MTM-specific entry data relating to a previous context. For 
	example, if the implementation has a private buffer storing a message subject, 
	for access through Subject(), this buffer should be cleared. */
	virtual void ContextEntrySwitched()=0; // called after the context of this instance has been changed to another entry
	//
	IMPORT_C void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

	// Method used for extension: called by non virtual methods that need 
	// to have a polymorphic behaviour.
	IMPORT_C virtual TAny* GetInterface(TUid aUid);
	
	// From CBase
	IMPORT_C virtual TInt Extension_(TUint aExtensionId, TAny*& a0, TAny* a1);

private:
	void DeleteEntry();
	
	void AddFilePathAttachmentL(const TDesC& aFilePath, const TDesC8& aMimeType, TUint aCharset, CMsvAttachment::TMsvAttachmentType aType, TRequestStatus& aStatus);
	


protected:
	/** The current context. */
	CMsvEntry*		iMsvEntry;
	/** The address list for the current context. */
	CMsvRecipientList* iAddresseeList;

	/** Paragraph formatting applied to the CRichText object for the body text, as 
	returned by Body(). This is set to an empty CParaFormatLayer instance whenever 
	the context is set.
	
	Implementations can modify this if they wish to apply particular formatting 
	to body text. */
	CParaFormatLayer* iParaFormatLayer;
	/** Character formatting applied to the CRichText object for the body text, as 
	returned by Body().
	
	Implementations can modify this if they wish to apply particular formatting 
	to body text. */
	CCharFormatLayer* iCharFormatLayer;


	
private:
	TMsvId	iEntryId;
	CRichText* iRichTextBody;
	CRegisteredMtmDll& iRegisteredMtmDll;
	CMsvSession& iSession;
	CMsvAttachmentWaiter* iAttachmentWaiter;

	// Extra data member to allow for future extensions
	TAny* iExtensionData;
	};


#endif // __MTCLBASE_H__
