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
// MTSR.H
//
/**
 @file
 @publishedAll
 @released
*/
#ifndef __MTSR_H__
#define __MTSR_H__

#include <e32base.h>
#include <badesca.h>
#include <msvstd.h>
#include <msvreg.h>
#include <tnonoperationmtmdata.h>
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "cinstalledmtmgroup.h"
#endif

// forward declarations
class RWriteStream;
class RReadStream;
class RFs;
class CDictionaryFileStore;
class CInstalledMtmGroup;
class CMsvServerEntry;
class TMsvSystemProgress;
class CInstalledMtmGroupArray;

#if (defined SYMBIAN_USER_PROMPT_SERVICE)
const TUint KUIDMsgClientThreadInfo = 0x10283090;
#endif

class CBaseServerMtm : public CActive
/** Base class for Server-side MTM components. Server-side MTMs provide all message 
transport functionality for a particular messaging protocol. 

MTM implementers implement a derived class to provide such functionality for 
their message protocol. Writers of message client applications are never concerned 
with this class and its sub-classes, as these are only accessed by the Message 
Server. 

Each MTM interprets the generic commands in different ways. For example, a 
Fax MTM would transmit a fax when asked to copy a fax from a local folder 
to a fax service. For the same function, an IMAP MTM would create a copy of 
the message on the remote server and update the message index to show the 
copy of the message on the remote server. An important initial design task 
is to the map the functions to the functionality provided by the protocol.

Server-side MTM functions are called by the Message Server as a result of 
a client request that requires some remote operation with the MTM's protocol. 
The following steps give a simplified view of the usual sequence of events:

1. the Message Server instantiates a Server-side MTM object through the factory 
function

2. the Message Server calls the appropriate asynchronous function on the Server-side 
MTM interface, passing a TRequestStatus argument

3. the Server-side MTM function typically starts whatever asynchronous communications 
it requires and returns

4. the Server-side MTM is signalled when the asynchronous communications complete, 
and handles the result

5. the Server-side MTM signals the Message Server, through the TRequestStatus 
passed earlier

6. the Message Server deletes the Server-side MTM object

To qualify this somewhat:

1. it is up to the Server-side MTM implementation to decide how to translate 
data back and forth between the formats used by Message Server (index entry, 
message store, binary files), and that required by the protocol; this is another 
important design task 

2. depending on the protocol being used, the communications sequence can be of 
considerable complexity; typically it requires division into a number of asynchronous 
steps

3. for greater efficiency where further commands are shortly expected, deletion 
of the Server-side MTM object can be prevented

For asynchronous requests, a Server-side MTM should always complete the TRequestStatus 
with KErrNone. Any errors should be returned in the progress information.

Note the following significant groups of functions:

1. Copy and move from remote functions: CopyToLocalL() and MoveToLocalL() are 
called by the Message Server to get a selection of entries from a remote location. 
For many protocols, this should be interpreted as message retrieval. For protocols 
where messages exist on a remote server, this function is typically used to 
download specific messages, after an initial connection has downloaded message 
headers. 

2. Copy and move to remote functions: CopyFromLocalL() and MoveFromLocalL() are 
called by the Message Server to copy/move a selection of entries to a remote 
location. For many protocols, this should be interpreted as message sending. 

3. Copy and move within remote functions: CopyWithinServiceL() and MoveWithinServiceL() 
are called by the Message Server to copy a selection of entries within a remote 
service. An example of their use might be for a user rearranging messages 
within remote folders. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C ~CBaseServerMtm();
	//
	/** Copies a selection of entries from a remote location to a local location. This 
	will only be meaningful for some protocols.
	
	Requirements:
	
	Implementations should provide this function if the messaging protocol supports 
	retrieval of remote entries. If this is not supported, implementations should 
	leave with KErrNotSupported. 
	
	Implementations of this function have three fundamental steps:
	
	1. doing the transfer operation using the appropriate communications protocols 
	
	2. converting protocol-specific data into the three-part storage format (index 
	entry, message store, binary files) required by the Message Server
	
	3. updating entries in the Message Server
	
	@param aSelection The collection of message index entries for which the copy/moving 
	is required. 
	@param aDestination The entry ID to which the selection is to be copied 
	@param aStatus Asynchronous completion word for the operation 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void CopyToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)=0;
	/** Copies a selection of entries from a local location to a remote location. 
	
	Requirements:
	
	Implementations should provide this function if the messaging protocol supports 
	retrieval of remote entries. If this is not supported, implementations should 
	leave with KErrNotSupported. 
	
	Implementations of this function have three fundamental steps:
	
	1. reading entry data
	
	2. converting entry data from the Message Server format into that required by 
	the protocol
	
	3. doing the transfer operation using the appropriate communications protocols 
	
	@param aSelection The collection of message index entries for which the copy 
	is required 
	@param aDestination The entry ID of the service by which the entries should 
	be transferred 
	@param aStatus Asynchronous completion word for the operation 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void CopyFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)=0;
	/** Copies a selection of entries within a remote location. 
	
	Requirements:
	
	Implementations should provide this function if the messaging protocol supports 
	the ability to copy entries within a remote service. If this is not supported, 
	implementations should leave with KErrNotSupported. 
	
	@param aSelection The collection of message index entries for which the copy 
	is required 
	@param aDestination The server entry ID to which the selection is to be copied 
	@param aStatus Asynchronous completion word for the operation 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void CopyWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)=0;
	/** Deletes each entry in the supplied selection when called by the message Server. 
	If any of the entries in the selection is a parent entry, then all its children 
	should also be deleted, recursively to the bottom of the ownership tree.
	
	Implementations should provide this function if the messaging protocol supports 
	deletion of remote entries. If this is not supported, implementations should 
	leave with KErrNotSupported. 
	
	@param aSelection The collection of entries that are to be deleted. 
	@param aStatus Asynchronous completion object. 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void DeleteAllL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)=0;
	/** Creates a new remote entry with relevant data when called by the Message Server.
	 
	Implementations should provide this function if the messaging protocol supports 
	creation of remote entries. If this is not supported, implementations should 
	leave with KErrNotSupported.
	
	As with ChangeL(), the Server-side MTM implementation must decide what information 
	in the TMsvEntry is relevant to the remote entry, and translate it appropriately 
	for the specific protocol. Most of the data contained in the TMsvEntry is 
	specific to the Message Server, and would probably have no direct correlation 
	with the protocol's own storage format. For example, for a folder, probably 
	only the name and parent are needed, so if the protocol supports creation 
	of remote folders, the implementation could:
	
	1. check for a folder type entry
	
	2. get the folder name and parent details from aNewEntry
	
	3. initiate a protocol-specific action to create the remote folder
	
	@param aNewEntry Data by which to create entry 
	@param aStatus Asynchronous completion word for the operation. 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void CreateL(TMsvEntry aNewEntry, TRequestStatus& aStatus)=0;
	/** Updates a remote entry with relevant data when called by the Message Server.
	 
	Implementations should provide this function if the messaging protocol supports 
	updating of remote entries. If this is not supported, implementations should 
	leave with KErrNotSupported. 
	
	The Server-side MTM implementation must decide what information in the TMsvEntry 
	is relevant to the remote entry, and translate it appropriately for the specific 
	protocol. Most of the data contained in the TMsvEntry is specific to the Symbian 
	OS Message Server, and would probably have no direct correlation with the 
	protocol's own storage format. Some entry data may however be useful. For 
	example, if the protocol supports remote renaming of folders, the implementation 
	could:
	
	1. check for a folder type entry
	
	2. extract the folder name from aNewEntry.iDetails
	
	3. check if the folder name has changed by comparing the new name with iDetails 
	in the index entry currently; if not, complete with KErrNone
	
	4. initiate a protocol-specific action to rename the remote folder
	
	The implementation should also always update the local Message Server index 
	through CMsvServerEntry::ChangeL().
	
	@param aNewEntry Data by which to update entry 
	@param aStatus Asynchronous completion word for the operation. 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void ChangeL(TMsvEntry aNewEntry, TRequestStatus& aStatus)=0;
	//
	/** Executes an MTM-specific operation on a selection of entries when called by 
	the Message Server.
	
	The call is made as a response to a client program invoking an MTM-specific 
	operation through CBaseMtm::InvokeSyncFunctionL()/InvokeAsyncFunctionL(). 
	The aSelection, aCommand, and aParameter arguments pass the values of the 
	original aSelection, aFunctionId, and aParameter respectively arguments from 
	such a call. The use (if any) of the aSelection and aParameter arguments by 
	the function depends on the command.
	
	@param aSelection A selection of message entries on which the command is to 
	be executed 
	@param aCommand The MTM-specific command to be carried out 
	@param aParameter Command-specific parameters 
	@param aStatus Asynchronous completion word for the operation */
	virtual void StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus)=0;
	//
	/** Tests if the Server-side MTM object should be deleted when called by the Message 
	Server
	
	It is useful to stop the MTM being deleted when more commands are expected 
	shortly. This would be the case, for example, after receiving a command to 
	go online. 
	
	If there are no more commands expected by the Server-side MTM object, then 
	the function should return EFalse, and the Message Server will delete it.
	
	@return ETrue: the MTM object should not be deleted EFalse: the MTM object 
	can be deleted */
	virtual TBool CommandExpected()=0;
	//
	/** This function is called by the Message Server to get progress information for 
	the current asynchronous operation. 
	
	The call is made as a response to a client program requesting progress information 
	through CMsvOperation::ProgressL(). The packing format used in the TDesC8 
	is MTM-specific. Only the implementation of the User Interface MTM progress 
	information functions need to understand the format.
	
	The progress buffer should have a maximum size of 256 bytes.
	
	@return Progress information on current asynchronous operation 
	@see CBaseMtmUi::DisplayProgressSummary()
	@see CBaseMtmUi::GetProgress() */
	virtual const TDesC8& Progress()=0;
	//
	/** Moves a selection of entries from a remote location to a local location. 
	
	Requirements:
	
	Implementations should provide this function if the messaging protocol supports 
	retrieval of remote entries. If this is not supported, implementations should 
	leave with KErrNotSupported. 
	
	Implementations of this function have three fundamental steps:
	
	1. doing the transfer operation using the appropriate communications protocols 
	
	2. converting protocol-specific data into the three-part storage format (index 
	entry, message store, binary files) required by the Message Server
	
	3. updating entries in the Message Server
	
	MoveToLocalL() should differ from CopyToLocalL() in additionally deleting 
	the original remote data. 
	
	@param aSelection The collection of message index entries for which the moving 
	is required. 
	@param aDestination The entry ID to which the selection is to be copied/moved 
	@param aStatus Asynchronous completion word for the operation 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void MoveToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)=0;
	/** Moves a selection of entries from a local location to a remote location. 
	
	Requirements:
	
	Implementations should provide this function if the messaging protocol supports 
	retrieval of remote entries. If this is not supported, implementations should 
	leave with KErrNotSupported. 
	
	Implementations of this function have three fundamental steps:
	
	1. reading entry data
	
	2. converting entry data from the Message Server format into that required by 
	the protocol
	
	3. doing the transfer operation using the appropriate communications protocols 
	
	The implementation of MoveFromLocalL() should differ from CopyFromLocalL() 
	in additionally deleting the original local data. 
	
	@param aSelection The collection of message index entries for which the move 
	is required 
	@param aDestination The entry ID of the service by which the entries should 
	be transferred 
	@param aStatus Asynchronous completion word for the operation 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void MoveFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)=0;
	/** Moves a selection of entries within a remote location. 
	
	Requirements:
	
	Implementations should provide this function if the messaging protocol supports 
	the ability to move entries within a remote service. If this is not supported, 
	implementations should leave with KErrNotSupported. 
	
	The implementation of MoveWithinServiceL() should differ from CopyWithinServiceL() 
	in additionally deleting the original data. 
	
	@param aSelection The collection of message index entries for which the move 
	is required 
	@param aDestination The server entry ID to which the selection is to be moved 
	@param aStatus Asynchronous completion word for the operation 
	@leave KErrNotSupported The Server-side MTM does not support this operation 
	@leave Other leave codes Dependent on implementation */
	virtual void MoveWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus)=0;

	IMPORT_C TInt SystemProgress(TMsvSystemProgress& aOutSysProg);
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
	TInt ClientThreadInfo(TThreadId aClientInfo, TBool aCapabilityCheck);
#endif

	TInt GetNonOperationMtmData(TNonOperationMtmDataType& aMtmDataType, TPtrC8& aResultBuffer);
	
	IMPORT_C virtual TInt ChangeEntriesL( const CMsvEntrySelection& aSelection,TInt aMark, TRequestStatus& aStatus );
protected:
	IMPORT_C CBaseServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry);
	/** Handles the completion of any asynchronous requests that it makes. It is called 
	from the base class RunL() .
	
	Note that any leaves made by this function result in DoComplete() being called 
	with the leave code. */
	virtual void DoRunL()=0;
	/** Called by the base class RunL() if DoRunL() leaves. 
	
	It should be implemented to handle this error. For example, progress information 
	could be updated to reflect the problem.
	
	@param aError The leave code given by DoRunL(). */
	virtual void DoComplete(TInt aError)=0;
	//
	IMPORT_C TInt Extension_(TUint aExtensionId, TAny *&a0, TAny *a1);
	//
private:
	// from CActive
	IMPORT_C void RunL();
	IMPORT_C TInt RunError(TInt aError);
	//
protected:
	/** The entry on which to operate. It is set in the constructor.
	
	The destructor deletes this member. */
	CMsvServerEntry* iServerEntry;
	
	// Method used for extension: called by non virtual methods that need 
	// to have a polymorphic behaviour.
	IMPORT_C virtual TAny* GetInterface(TUid aUid);
	//
private:
	CRegisteredMtmDll& iRegisteredMtmDll;

private:
	// Extra data member to allow for future extensions
	TAny* iExtensionData;
	};


class CServerMtmDllRegistry : public CMtmDllRegistry
/**
@publishedAll
@released
*/
	{
friend class CMtmRegistryControl;
public:
	IMPORT_C static CServerMtmDllRegistry* NewL(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32=KMsvDefaultTimeoutMicroSeconds32);
	IMPORT_C ~CServerMtmDllRegistry();
	IMPORT_C CBaseServerMtm* NewServerMtmL(TUid aMtmTypeUid, CMsvServerEntry* aInitialEntry);
	//
protected:
	CServerMtmDllRegistry(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32);
	//
private:
	CBaseServerMtm* NewMtmL(const RLibrary& aLib, CMsvServerEntry* aServerEntry, CRegisteredMtmDll& aReg) const;
	};

//**********************************
// CMsvMtmCache
//**********************************


//**********************************
// CMtmRegistryControl
//**********************************

class CMtmRegistryControl : public CBase, public MRegisteredMtmDllObserver
/**
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMtmRegistryControl* NewL(RFs& anFs,CServerMtmDllRegistry& aServerMtmDllRegistry);
	IMPORT_C ~CMtmRegistryControl();

	IMPORT_C TInt InstallMtmGroup(const TDesC& aFullName,TUid& aMtmTypeUid);
	IMPORT_C TInt FullNameToMtmTypeUid(const TDesC& aFullName,TUid& aMtmTypeUid) const;
	IMPORT_C TInt DeInstallMtmGroup(TUid aMtmTypeUid);  //  returns error on storing registry

	IMPORT_C TInt UseMtmGroup(TUid aMtmTypeUid);
	IMPORT_C TInt ReleaseMtmGroup(TUid aMtmTypeUid);  
	IMPORT_C TBool IsInUse(TUid aMtmTypeUid) const;

	IMPORT_C TInt FillRegisteredMtmDllArray(TUid aMtmDllTypeUid,CRegisteredMtmDllArray& aRegisteredMtmDllArray,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32=0);  // Fill array with Dlls whose second uid is aMtmDllTypeUid
	IMPORT_C CMtmGroupData* GetMtmGroupDataL(TUid aMtmTypeUid) const;  
	const CMtmGroupData& GetMtmGroupDataReferenceL(TUid aMtmTypeUid) const;  

	IMPORT_C void StoreRegistryL() const;
	IMPORT_C void RestoreRegistryL();

	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

private:
	CMtmRegistryControl(RFs& anFs,CServerMtmDllRegistry& aServerMtmDllRegistry);
	void ConstructL();
	TInt MtmTypeUidToIndex(TUid aMtmTypeUid) const;
	TInt UidTypeToIndex(TUidType aUidType) const;
	void DoInstallMtmGroupL(const TDesC& aFullName,TUid& aMtmTypeUid);
	CMtmGroupData* ReadDataFileStoreL(const TDesC& aFullName) const;
	void DoDeInstallMtmGroupL(TUid aMtmTypeUid);
	void DoInternalizeL(RReadStream& aStream);
	void AddInstalledMtmGroupL(CInstalledMtmGroup* aInstalledMtmGroup);
	void RemoveInstalledMtmGroup(TUid aMtmTypeUid);
	TBool IsResFileL(const TDesC& aFullName) const;
	TUid DoFindMtmTypeUidL(const TDesC& aFullName) const;
	
	CMtmGroupData *LoadMTMFileL(const TDesC& aFullName, TUid &aUid);
	CMtmGroupData *LoadDatFileL(const TDesC& aFullName, TUid &aUid);
	CMtmGroupData *LoadResFileL(const TDesC& aFullName, TUid &aUid);	
private:
	RFs& iFs;
	CInstalledMtmGroupArray* iInstalledMtmGroupArray;
	CServerMtmDllRegistry& iServerMtmDllRegistry;
	TPath iPathName;
	};

#endif	// __MTSR_H__
