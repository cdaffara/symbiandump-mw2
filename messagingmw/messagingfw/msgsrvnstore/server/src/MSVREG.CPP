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
// MSVREG.CPP
//

#include "MSVREG.H"
#include "MSVRUIDS.H"
#include "MSVPANIC.H"
#include "MsvSecurityCapabilitySet.h"

#include <f32file.h>
#include <s32strm.h>
#include <e32uid.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"				
#endif

/** Creates a new CMtmDllInfo and initialises it with values describing an MTM 
component. 

@param aHumanReadableName Descriptor holding a descriptive name for the MTM 
component 
@param aUidType Group of UIDs for the MTM. The UIDs should be as follows: UID1: 
always KDynamicLibraryUid UID2: identifies whether the MTM component is a 
Client-side MTM, User Interface MTM, UI Data MTM, or Server-side MTM. UID3: 
identifies this concrete MTM uniquely 
@param aFilename A full filename (including drive and path) of the MTM dll
@param aEntryPointOrdinalNumber Ordinal of factory function for the MTM 
@param aVersion Version information for the MTM component 
@leave KErrNoMemory A memory allocation failed 
@return New CMtmDllInfo initialised with passed values */
EXPORT_C CMtmDllInfo* CMtmDllInfo::NewL(const TDesC& aHumanReadableName,const TUidType& aUidType,const TDesC& aFilename,TInt aEntryPointOrdinalNumber,const TVersion aVersion)
	{
	CMtmDllInfo* mtmdllinfo=new(ELeave) CMtmDllInfo(aUidType,aEntryPointOrdinalNumber,aVersion);
	CleanupStack::PushL(mtmdllinfo);
	mtmdllinfo->ConstructL(aHumanReadableName, aFilename);
	CleanupStack::Pop(mtmdllinfo);
	return mtmdllinfo;	
	}

/** Creates a new CMtmDllInfo initialised from another CMtmDllInfo object. 

@param aMtmDllInfo A CMtmDllInfo object from which to initialise this
@leave KErrNoMemory A memory allocation failed 
@return New CMtmDllInfo initialised with passed CMtmDllInfo */
EXPORT_C CMtmDllInfo* CMtmDllInfo::NewL(const CMtmDllInfo& aMtmDllInfo)
	{
	CMtmDllInfo* mtmdllinfo = new(ELeave) CMtmDllInfo(aMtmDllInfo);
	CleanupStack::PushL(mtmdllinfo);
	mtmdllinfo->ConstructL(*aMtmDllInfo.iHumanReadableName, *aMtmDllInfo.iFilename);
	CleanupStack::Pop(mtmdllinfo);
	return mtmdllinfo;
	}
	

/** Creates a new CMtmDllInfo and initialises it with values read from a stream. 

The contents of the stream to read will have been created by CMtmDllInfo::ExternalizeL().

@param aStream Stream to read from 
@leave KErrNoMemory A memory allocation failed
@return New CMtmDllInfo */
EXPORT_C CMtmDllInfo* CMtmDllInfo::NewL(RReadStream& aStream)
	{
	CMtmDllInfo* mtmdllinfo=new(ELeave) CMtmDllInfo();
	CleanupStack::PushL(mtmdllinfo);
	mtmdllinfo->InternalizeL(aStream);
	CleanupStack::Pop();
	return mtmdllinfo;	
	}

/** Destructor. */
EXPORT_C CMtmDllInfo::~CMtmDllInfo()
	{
	delete iHumanReadableName;
	delete iFilename;
	}

EXPORT_C void CMtmDllInfo::SetHumanReadableNameL(const TDesC& aHumanReadableName)
/** Sets the descriptive name of the MTM component for which the object holds registration 
data.

@param aHumanReadableName Descriptor holding a descriptive name for the MTM 
component */
	{
	__ASSERT_DEBUG(aHumanReadableName.Length()<=KHumanReadableNameLength,PanicServer(EMsvHumanReadableNameTooLong));
	HBufC* humanreadablename=HBufC::NewL(aHumanReadableName.Length());
	delete iHumanReadableName;
	iHumanReadableName=humanreadablename;
	iHumanReadableName->Des().Copy(aHumanReadableName);
	}

/** Internalises the object from a stream. 

The contents of the stream to read will have been created by CMtmDllInfo::ExternalizeL().

@param aStream Stream to read from 
@leave Error Standard streaming errors
*/
EXPORT_C void CMtmDllInfo::InternalizeL(RReadStream& aStream)
	{
	HBufC* humanreadablename=HBufC::NewL(aStream,KHumanReadableNameLength);
	delete iHumanReadableName;
	iHumanReadableName=humanreadablename;
	iMessagingCapability = aStream.ReadInt8L();
	iSendBodyCapability = aStream.ReadInt8L();
	iCapabilitiesAvailable = aStream.ReadInt8L();
	TUid uid1,uid2,uid3;
	aStream >> uid1;
	aStream >> uid2;
	aStream >> uid3;
	TUidType uidtype(uid1,uid2,uid3);
	iUidType=uidtype;                 
	iEntryPointOrdinalNumber=aStream.ReadInt32L();
	iVersion.iMajor=aStream.ReadInt8L();
	iVersion.iMinor=aStream.ReadInt8L();
	iVersion.iBuild=aStream.ReadInt16L();
	HBufC* temp;
	if (uid3.iUid == KUidMtmDefaultSpecificVal)
		{
		// We can assume we are reading data from an updated stream.
		// The version number should be <= 2.0.
		__ASSERT_DEBUG((iVersion.iMajor < KMtmComponentCurrentMajorVersionNumber) || (iVersion.iMajor == KMtmComponentCurrentMajorVersionNumber&& iVersion.iMinor == KMtmComponentCurrentMinorVersionNumber), PanicServer(EMsvBadMtmVersionNumber));
		
		temp = HBufC::NewL(aStream, KMaxFileName);
		}
	else
		{
		// Just create a zero length filename for old versions
		temp = KNullDesC().AllocL();
		}
	delete iFilename;
	iFilename = temp;
	}


// no longer used but as it is exported it has been kept
/** Externalises the object to a stream. 

@param aStream Stream to write to 
@leave Error Standard streaming errors
*/
EXPORT_C void CMtmDllInfo::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << *iHumanReadableName;
	aStream.WriteInt8L(iMessagingCapability);
	aStream.WriteInt8L(iSendBodyCapability);
	aStream.WriteInt8L(iCapabilitiesAvailable);
	aStream << iUidType[0];                 
	aStream << iUidType[1];                 
	aStream << iUidType[2];                 
	aStream.WriteInt32L(iEntryPointOrdinalNumber);
	aStream.WriteInt8L(iVersion.iMajor);
	aStream.WriteInt8L(iVersion.iMinor);
	aStream.WriteInt16L(iVersion.iBuild);
	if (iUidType[2].iUid == KUidMtmDefaultSpecificVal)
		{
		// The version number should be <= 2.0.
		__ASSERT_DEBUG((iVersion.iMajor < KMtmComponentCurrentMajorVersionNumber) || (iVersion.iMajor == KMtmComponentCurrentMajorVersionNumber && iVersion.iMinor == KMtmComponentCurrentMinorVersionNumber), PanicServer(EMsvBadMtmVersionNumber));

		aStream << FileName();
		}
	}

/** Overloaded equality operator.
@param aMtmDllInfo Object to compare
@return True if the iFileName member and the UIDs (iUidType member) are the same in both objects,
otherwise false
*/

EXPORT_C TBool CMtmDllInfo::operator==(const CMtmDllInfo& aMtmDllInfo) const
	{
	return (FileName().CompareF(aMtmDllInfo.FileName()) == 0 && (iUidType==aMtmDllInfo.iUidType));
	}

CMtmDllInfo::CMtmDllInfo(const TUidType& aUidType,TInt aEntryPointOrdinalNumber,const TVersion aVersion):
	iUidType(aUidType),                
	iEntryPointOrdinalNumber(aEntryPointOrdinalNumber),
	iVersion(aVersion)
	{
	__DECLARE_NAME(_S("CMtmDllInfo"));
	}


CMtmDllInfo::CMtmDllInfo()
	{
	}

CMtmDllInfo::CMtmDllInfo(const CMtmDllInfo& aMtmDllInfo):
	iUidType(aMtmDllInfo.iUidType),
	iEntryPointOrdinalNumber(aMtmDllInfo.iEntryPointOrdinalNumber),
	iVersion(aMtmDllInfo.iVersion),
	iMessagingCapability(aMtmDllInfo.iMessagingCapability),
	iSendBodyCapability(aMtmDllInfo.iSendBodyCapability),
	iCapabilitiesAvailable(aMtmDllInfo.iCapabilitiesAvailable)
	{
	}

void CMtmDllInfo::ConstructL(const TDesC& aHumanReadableName, const TDesC& aFilename)
	{
	iHumanReadableName = aHumanReadableName.AllocL();
	iFilename = aFilename.AllocL();
	}

TPtrC CMtmDllInfo::FileName() const
	{
	return *iFilename;
	}


/** Sets a flag to indicate that the MTM can send messages. 

@param aCapability True to set the flag, false to clear it
*/
void CMtmDllInfo::SetMessagingCapability(TBool aCapability)
	{
	iMessagingCapability = aCapability;
	}
 

/** Sets a flag to indicate that the MTM can handle body text. 

@param aCapability True to set the flag, false to clear it
*/
void CMtmDllInfo::SetSendBodyCapability(TBool aCapability)
	{
	iSendBodyCapability = aCapability;
	}

/** Sets a flag to indicate that settings have been made for 
the MessagingCapability() and SendBodyCapability() flags.

These settings are optional, so may not have been made 
for all MTMs.  
@param aCapability True if the settings exist; otherwise false
*/
void CMtmDllInfo::SetCapabilitiesAvailable(TBool aCapability)
	{
	iCapabilitiesAvailable = aCapability;
	}

/** Tests if the flag that indicates that the MTM can send 
messages has been set. 

@return True if the flag has been set; otherwise false
*/
EXPORT_C TBool CMtmDllInfo::MessagingCapability() const
	{
	return iMessagingCapability;
	}

/** Tests if the flag that indicates that the MTM can handle  
body text has been set. 

@return True if the flag has been set; otherwise false
*/
EXPORT_C TBool CMtmDllInfo::SendBodyCapability() const
	{
	return iSendBodyCapability;
	}

/** Tests if settings have been made for the MessagingCapability() 
and SendBodyCapability() flags.

These settings are optional, so may not have been made 
for all MTMs.  
@return True if the settings exist; otherwise false
*/
EXPORT_C TBool CMtmDllInfo::CapabilitiesAvailable() const
	{
	return iCapabilitiesAvailable;
	}

/** Default constructor. */
EXPORT_C CMtmDllInfoArray::CMtmDllInfoArray():
	CArrayPtrFlat<CMtmDllInfo>(8)
	{
	__DECLARE_NAME(_S("CMtmDllInfoArray"));
	}

/** Destructor. */
EXPORT_C CMtmDllInfoArray::~CMtmDllInfoArray()
	{
	ResetAndDestroy();
	}

EXPORT_C void CMtmDllInfoArray::AddMtmDllInfoL(CMtmDllInfo* aMtmDllInfo)
/** Appends a CMtmDllInfo to the array.

@param aMtmDllInfo CMtmDllInfo to append 
@leave KErrNoMemory A memory allocation failed */
	{
	CleanupStack::PushL(aMtmDllInfo);
	AppendL(aMtmDllInfo);
	CleanupStack::Pop();
	}



/** Creates a new CMtmGroupData and initialise it with registration data for an 
MTM group. 

@param aMtmTypeUid UID that uniquely identifies the MTM group 
@param aTechnologyTypeUid UID that can be used to indicate the messaging technology 
which the MTM group implements 
@param aMtmDllInfoArray Array of registration data for the MTM components in 
the group. This function takes immediate responsibility for aMtmDllInfoArray,
so it should not be on the cleanup stack prior to the call. If successful, 
the newly created CMtmGroupData takes ownership of the object
@param aMtmRequiredCaps The required security capabilities for this MTM group
@leave KErrNoMemory A memory allocation failed 
@return New initialised CMtmGroupData */

EXPORT_C CMtmGroupData* CMtmGroupData::NewL(TUid aMtmTypeUid, TUid aTechnologyTypeUid, CMtmDllInfoArray* aMtmDllInfoArray, const TCapabilitySet& aMtmRequiredCaps)
	{
	__ASSERT_DEBUG(aMtmDllInfoArray != NULL, PanicServer(EMsvConstructWithNullDllInfoArray));
	CleanupStack::PushL(aMtmDllInfoArray);	
	CMtmGroupData* self = new(ELeave) CMtmGroupData(aMtmTypeUid, aTechnologyTypeUid, aMtmDllInfoArray, aMtmRequiredCaps);
	CleanupStack::Pop(aMtmDllInfoArray); 
	return self;	
	}

/** Creates a new CMtmGroupData and initialises it with another CMtmGroupData

@param aMtmTypeGroupData Another CMtmGroupData from which to copy
@leave KErrNoMemory A memory allocation failed 
@return New initialised CMtmGroupData */

EXPORT_C CMtmGroupData* CMtmGroupData::NewL(const CMtmGroupData& aMtmGroupData)
	{
	CMtmGroupData* self = new(ELeave) CMtmGroupData(aMtmGroupData.iMtmTypeUid, aMtmGroupData.iTechnologyTypeUid, NULL, aMtmGroupData.iMtmRequiredCaps);
	CleanupStack::PushL(self);
	self->ConstructL(aMtmGroupData);
	CleanupStack::Pop(self);
	return self;
	}

CMtmGroupData::CMtmGroupData(TUid aMtmTypeUid, TUid aTechnologyTypeUid, CMtmDllInfoArray* aMtmDllInfoArray,const TCapabilitySet& aMtmRequiredCaps):
	iMtmTypeUid(aMtmTypeUid),
	iTechnologyTypeUid(aTechnologyTypeUid),
	iMtmDllInfoArray(aMtmDllInfoArray),
	iMtmRequiredCaps(aMtmRequiredCaps)
	{
	}
		
void CMtmGroupData::AppendMtmDllInfoArrayL(const CMtmDllInfoArray& aMtmDllInfoArray)
	{
	TInt count=aMtmDllInfoArray.Count();
	for (TInt ii=0; ii<count; ++ii)
		{
		CMtmDllInfo* mtmdllinfo = CMtmDllInfo::NewL(*aMtmDllInfoArray[ii]);
		AppendMtmDllInfoL(mtmdllinfo);
		}
	}
	
void CMtmGroupData::ConstructL(const CMtmGroupData& aMtmGroupData)
	{
	iMtmDllInfoArray = new(ELeave) CMtmDllInfoArray();
	AppendMtmDllInfoArrayL(aMtmGroupData.MtmDllInfoArray());
	}
	
void CMtmGroupData::ConstructL()
	{
	iMtmDllInfoArray = new(ELeave) CMtmDllInfoArray();
	}


EXPORT_C CMtmGroupData* CMtmGroupData::NewL(RReadStream& aStream)
/** Creates a new CMtmGroupData and initialises it from the specified stream.

@param aStream Stream from which to read previously externalised CMtmGroupData 

@leave KErrNoMemory A memory allocation failed 
@return New initialised CMtmGroupData */
	{
	CMtmGroupData* mtmgroupdata=new(ELeave) CMtmGroupData();
	CleanupStack::PushL(mtmgroupdata);
	mtmgroupdata->ConstructL();
	mtmgroupdata->InternalizeL(aStream);
	CleanupStack::Pop();
	return mtmgroupdata;	
	}

/** Destructor. */
EXPORT_C CMtmGroupData::~CMtmGroupData()
	{
	delete iMtmDllInfoArray;
	}

EXPORT_C void CMtmGroupData::InternalizeL(RReadStream& aStream)
/** Internalises group registration data. 

@param aStream Stream from which to internalise object */
	{
	MtmDllInfoArrayPrivate().ResetAndDestroy();
	aStream >> iMtmTypeUid;
	aStream >> iTechnologyTypeUid;
	TInt count=aStream.ReadInt32L();
	for (TInt i=0; i<count; i++)
		{
		CMtmDllInfo* mtmdllinfo=CMtmDllInfo::NewL(aStream);
		AppendMtmDllInfoL(mtmdllinfo);
		}
	MsvSecurityCapabilitySetUtils::InternalizeL(aStream,iMtmRequiredCaps);
	}


/** Returns a constant reference to the array of MTM dll information objects owned by the CMtmGroupData.

@return A constant reference to the MTM dll info array*/

EXPORT_C const CMtmDllInfoArray& CMtmGroupData::MtmDllInfoArray() const
	{
	// Return a const reference to the dll info array
	__ASSERT_DEBUG(iMtmDllInfoArray != NULL, PanicServer(EMsvAttemptToUseNullDllInfoArray));
	return *iMtmDllInfoArray;
	}
	
CMtmDllInfoArray& CMtmGroupData::MtmDllInfoArrayPrivate()
	{
	// Return a non-const reference to the dll info array. Only for use within
	// this class
	__ASSERT_DEBUG(iMtmDllInfoArray != NULL, PanicServer(EMsvAttemptToUseNullDllInfoArray));
	return *iMtmDllInfoArray;
	}
		
EXPORT_C const TCapabilitySet& CMtmGroupData::GetMtmRequiredCapabilities() const
	{
	return iMtmRequiredCaps;
	}
	

 // not used but as it is exported it has been kept
EXPORT_C void CMtmGroupData::ExternalizeL(RWriteStream& aStream) const
/** Externalises group registration data. 

This is the method by which registration data is written to a MTM registration 
data file.

@param aStream Stream to which to externalise object */
	{
	aStream << iMtmTypeUid;
	aStream << iTechnologyTypeUid;
	const CMtmDllInfoArray& mtmDllInfoArray = MtmDllInfoArray();
	TInt count=mtmDllInfoArray.Count();
	aStream.WriteInt32L(count);
	for (TInt ii=0; ii<count; ++ii)
		aStream << *mtmDllInfoArray[ii];

	MsvSecurityCapabilitySetUtils::ExternalizeL(aStream,iMtmRequiredCaps);
	}

EXPORT_C TBool CMtmGroupData::operator==(const CMtmGroupData& aMtmGroupData) const
/** Tests for equality with another CMtmGroupData object.

@param aMtmGroupData CMtmGroupData object with which to compare 
@return ETrue: equal, EFalse: unequal.  */
	{
	TBool isequal=((iMtmTypeUid==aMtmGroupData.iMtmTypeUid) && (iTechnologyTypeUid==aMtmGroupData.iTechnologyTypeUid));
	if (isequal)
		{
		const CMtmDllInfoArray& mtmDllInfoArray1 = MtmDllInfoArray();
		const CMtmDllInfoArray& mtmDllInfoArray2 = aMtmGroupData.MtmDllInfoArray();
		TInt count1=mtmDllInfoArray1.Count();
		TInt count2=mtmDllInfoArray2.Count();
		TBool isequal=(count1==count2);
		for (TInt ii=0; (ii<count1) && isequal; ++ii)
			isequal=(mtmDllInfoArray1[ii]==mtmDllInfoArray2[ii]);
		}
	if (isequal)
		{
		isequal = (iMtmRequiredCaps.HasCapabilities(aMtmGroupData.iMtmRequiredCaps)
			&& aMtmGroupData.iMtmRequiredCaps.HasCapabilities(iMtmRequiredCaps));
		}
	return isequal;
	}

CMtmGroupData::CMtmGroupData(TUid aMtmTypeUid,TUid aTechnologyTypeUid):
	iMtmTypeUid(aMtmTypeUid),
	iTechnologyTypeUid(aTechnologyTypeUid)
	{
	__DECLARE_NAME(_S("CMtmGroupData"));
	}


void CMtmGroupData::AppendMtmDllInfoL(CMtmDllInfo* aMtmDllInfo)
	{
	CMtmDllInfoArray& mtmDllInfoArray = MtmDllInfoArrayPrivate();
	TInt index = mtmDllInfoArray.Count();
	CleanupStack::PushL(aMtmDllInfo);
	if (index==KMsvNumMtmDllTypes)
		User::Leave(KErrNotSupported);  //  There may be a better error value
	if (aMtmDllInfo->iUidType[0]!=KDynamicLibraryUid)  
		User::Leave(KErrNotSupported);  

	TUid mtmdlltypeuid[KMsvNumMtmDllTypes] = { KUidMtmServerComponentVal, KUidMtmClientComponentVal, KUidMtmUiComponentVal, KUidMtmUiDataComponentVal };
	__ASSERT_DEBUG(aMtmDllInfo->iUidType[1]==mtmdlltypeuid[index],PanicServer(EMsvMtmDllInfoSecondUidIncorrect));
	if (aMtmDllInfo->iUidType[1]!=mtmdlltypeuid[index])  
		User::Leave(KErrNotSupported);
	mtmDllInfoArray.AppendL(aMtmDllInfo);
	CleanupStack::Pop(aMtmDllInfo);
	}

/**
@internalComponent
*/
EXPORT_C CRegisteredMtmDll* CRegisteredMtmDll::NewL(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,const TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver)
	{
	CRegisteredMtmDll* registereredmtmdll=new(ELeave) CRegisteredMtmDll(aMtmTypeUid,aTechnologyTypeUid,aTimeoutMicroSeconds32,aRegisteredMtmDllObserver);
	CleanupStack::PushL(registereredmtmdll);
	registereredmtmdll->ConstructL(aMtmDllInfo);
	CleanupStack::Pop();
	return registereredmtmdll;	
	}

EXPORT_C CRegisteredMtmDll::~CRegisteredMtmDll()
	{
	__ASSERT_DEBUG(iMtmDllRefCount==0,PanicServer(EMsvRegisteredMtmDllStillInUse));
	Cancel();
	delete iMtmDllInfo;
	iMtmDllLibrary.Close();
	}

/**
@internalComponent
*/
EXPORT_C TInt CRegisteredMtmDll::GetLibrary(RFs& aFs,RLibrary& aMtmDllLibrary)
	{
	TInt ret=KErrNone;
	if (iMtmDllRefCount==0)
		TRAP(ret,LoadLibraryL(aFs));
	if (ret==KErrNone)
		{
		aMtmDllLibrary=iMtmDllLibrary;
		iMtmDllRefCount++;
		}
	else
		iMtmDllLibrary.Close();
	return ret;
	}

/**
@internalComponent
*/
EXPORT_C void CRegisteredMtmDll::ReleaseLibrary()
	{
	__ASSERT_DEBUG(iMtmDllRefCount>0,PanicServer(EMsvRegisteredMtmDllRefCountZero));
	iMtmDllRefCount--;
	if (iMtmDllRefCount==0)
		{
		iRegisteredMtmDllObserver.ReleaseMtmGroup(iMtmTypeUid);  //  Ignore error returned, this is safe client side
		After(iTimeoutMicroSeconds32);
		}
	}

CRegisteredMtmDll::CRegisteredMtmDll(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver):
	CTimer(EPriorityLow),
	iMtmTypeUid(aMtmTypeUid),
	iTechnologyTypeUid(aTechnologyTypeUid),
	iTimeoutMicroSeconds32(aTimeoutMicroSeconds32),
	iRegisteredMtmDllObserver(aRegisteredMtmDllObserver)
	{
	__DECLARE_NAME(_S("CRegisteredMtmDll"));
	CActiveScheduler::Add(this);
	}

void CRegisteredMtmDll::ConstructL(const CMtmDllInfo& aMtmDllInfo)
	{
	CTimer::ConstructL();
	iMtmDllInfo = CMtmDllInfo::NewL(aMtmDllInfo);
	}

void CRegisteredMtmDll::LoadLibraryL(RFs& /*aFs*/)  
	{
	__ASSERT_DEBUG(iMtmDllRefCount==0,PanicServer(EMsvRegisteredMtmDllRefCountNonZero));
	Cancel();  //  Cancel timer
	if (iMtmDllLibrary.Handle()==0)
		{
		// Secure API and old UID support removed - load by filename
		User::LeaveIfError(iMtmDllLibrary.Load(iMtmDllInfo->FileName()));
		}
	User::LeaveIfError(iRegisteredMtmDllObserver.UseMtmGroup(iMtmTypeUid));
	}


void CRegisteredMtmDll::RunL()
	{
	iMtmDllLibrary.Close();
	}


EXPORT_C CRegisteredMtmDllArray::CRegisteredMtmDllArray():
	CArrayPtrFlat<CRegisteredMtmDll>(8)
	{
	__DECLARE_NAME(_S("CRegisteredMtmDllArray"));
	}

EXPORT_C CRegisteredMtmDllArray::~CRegisteredMtmDllArray()
	{
	ResetAndDestroy();
	}

EXPORT_C void CRegisteredMtmDllArray::AddRegisteredMtmDllL(CRegisteredMtmDll* aRegisteredMtmDll)
	{
	CleanupStack::PushL(aRegisteredMtmDll);
	AppendL(aRegisteredMtmDll);
	CleanupStack::Pop();
	}

EXPORT_C CMtmDllRegistry::~CMtmDllRegistry()
	{
	}

EXPORT_C TUid CMtmDllRegistry::MtmTypeUid(TInt anIndex) const                                
/** Gets the MTM UID of a registered MTM using its index.

@param anIndex Indexed of registered MTM 
@return MTM UID */
	{
	__ASSERT_DEBUG((0<=anIndex) && (anIndex<iRegisteredMtmDllArray.Count()),PanicServer(EMsvRegisteredMtmDllIndexOutRange));
	return iRegisteredMtmDllArray[anIndex]->MtmTypeUid();
	}

EXPORT_C TUid CMtmDllRegistry::TechnologyTypeUid(TUid aMtmTypeUid) const
/** Gets the technology type UID for the specified MTM.

@param aMtmTypeUid UID of MTM 
@return Technology type UID */
	{
	__ASSERT_DEBUG(IsPresent(aMtmTypeUid),PanicServer(EMsvRegisteredMtmDllNotFound));
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	return iRegisteredMtmDllArray[index]->TechnologyTypeUid();
	}

EXPORT_C const CMtmDllInfo& CMtmDllRegistry::RegisteredMtmDllInfo(TUid aMtmTypeUid) const                                
/** Gets the registration data for the specified MTM.

@param aMtmTypeUid UID of MTM 
@return Registration data for MTM */
	{
	__ASSERT_DEBUG(IsPresent(aMtmTypeUid),PanicServer(EMsvRegisteredMtmDllNotFound));
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	return iRegisteredMtmDllArray[index]->MtmDllInfo();
	}

EXPORT_C TBool CMtmDllRegistry::IsInUse(TUid aMtmTypeUid) const
/** Tests if the MTM with the specified UID is in use. You should check that the 
MTM is registered, through IsPresent(), before calling this function.

@param aMtmTypeUid UID of MTM to check 
@return ETrue if the specified MTM in use, else EFalse */
	{
	__ASSERT_DEBUG(IsPresent(aMtmTypeUid),PanicServer(EMsvRegisteredMtmDllNotFound));
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	return iRegisteredMtmDllArray[index]->MtmDllRefCount()>0;
	}

EXPORT_C TBool CMtmDllRegistry::IsInUse() const
/** Tests if any registered MTM is in use. 

@return ETrue if any MTM in use, else EFalse */
	{
	TInt count=iRegisteredMtmDllArray.Count();
	TBool isinuse=EFalse;
	for (TInt i=0; (i<count) && (!isinuse); i++)
		isinuse=iRegisteredMtmDllArray[i]->MtmDllRefCount()>0;
	return isinuse;
	}

EXPORT_C CMtmDllRegistry::CMtmDllRegistry(RFs& aFs,TUid aMtmDllTypeUid,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32):
	iFs(aFs),
	iMtmDllTypeUid(aMtmDllTypeUid),
	iTimeoutMicroSeconds32(aTimeoutMicroSeconds32)
	{
	__DECLARE_NAME(_S("CMtmDllRegistry"));
	}

EXPORT_C TInt CMtmDllRegistry::MtmTypeUidToIndex(TUid anMtmTypeUid) const
	{
	TInt i=0, count=iRegisteredMtmDllArray.Count();
	for (;(i<count) && (iRegisteredMtmDllArray[i]->MtmTypeUid()!=anMtmTypeUid); i++)
		{
		}
	return i;
	}

EXPORT_C TInt CMtmDllRegistry::AddRegisteredMtmDll(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver)
	{
	__ASSERT_DEBUG(!IsPresent(aMtmTypeUid),PanicServer(EMsvRegisteredMtmDllHasSameMtmTypeUid));
	__ASSERT_DEBUG(aMtmDllInfo.iUidType[0]==KDynamicLibraryUid,PanicServer(EMsvRegisteredMtmDllHasSameMtmTypeUid));
	__ASSERT_DEBUG(aMtmDllInfo.iUidType[1]==iMtmDllTypeUid,PanicServer(EMsvRegisteredMtmDllHasSameMtmTypeUid));
	TRAPD(ret,DoAddRegisteredMtmDllL(aMtmTypeUid,aTechnologyTypeUid,aMtmDllInfo,aRegisteredMtmDllObserver));
	return ret;
	}

EXPORT_C void CMtmDllRegistry::RemoveRegisteredMtmDll(TUid aMtmTypeUid)
	{
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	__ASSERT_DEBUG(index<iRegisteredMtmDllArray.Count(),PanicServer(EMsvRegisteredMtmDllNotFound));
	__ASSERT_DEBUG(iRegisteredMtmDllArray[index]->MtmDllRefCount()==0,PanicServer(EMsvRegisteredMtmDllStillInUse));
	delete iRegisteredMtmDllArray[index];
	iRegisteredMtmDllArray.Delete(index);
	}

EXPORT_C void CMtmDllRegistry::RemoveAllRegisteredMtmDlls()
	{
	iRegisteredMtmDllArray.ResetAndDestroy();
	}

void CMtmDllRegistry::DoAddRegisteredMtmDllL(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver)
	{
	CRegisteredMtmDll* registeredmtmdll=CRegisteredMtmDll::NewL(aMtmTypeUid,aTechnologyTypeUid,aMtmDllInfo,iTimeoutMicroSeconds32,aRegisteredMtmDllObserver);
	iRegisteredMtmDllArray.AddRegisteredMtmDllL(registeredmtmdll);
	}



