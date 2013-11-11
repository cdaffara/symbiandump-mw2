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
// MSVREG.H
//
/**
 * @file 
 * @publishedAll
 * @released
 */

#if !defined(__MSVREG_H__)
#define __MSVREG_H__
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__F32FILE_H__)
#include <f32file.h>
#endif

const TInt KHumanReadableNameLength=50;
typedef TBuf<KHumanReadableNameLength> THumanReadableName;   //  could be used client side

#define KMsvDefaultTimeoutMicroSeconds32 30000000

// forward declarations
class RWriteStream;
class RReadStream;
class RFs;


class CMtmDllInfo : public CBase
/** Encapsulates the registration data for a single concrete MTM component. 

It is used in the creation of registration data for an MTM group. Typically, 
an application gathers a number of CMtmDllInfo objects into an CMtmDllInfoArray. 
 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMtmDllInfo* NewL(const TDesC& aHumanReadableName,const TUidType& aUidType,const TDesC& aFilename,TInt aEntryPointOrdinalNumber,const TVersion aVersion);
	IMPORT_C static CMtmDllInfo* NewL(const CMtmDllInfo& aMtmDllInfo);
	IMPORT_C static CMtmDllInfo* NewL(RReadStream& aStream);
	IMPORT_C ~CMtmDllInfo();
	IMPORT_C void SetHumanReadableNameL(const TDesC& aHumanReadableName);
	inline TPtrC HumanReadableName() const;
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;  // not used but as it is exported it has been kept
	IMPORT_C TBool operator==(const CMtmDllInfo& aMtmDllInfo) const; 
	void SetMessagingCapability(TBool aCapability);
	void SetSendBodyCapability(TBool aCapability);
	void SetCapabilitiesAvailable(TBool aBool);
	IMPORT_C TBool MessagingCapability() const;
	IMPORT_C TBool SendBodyCapability() const;
	IMPORT_C TBool CapabilitiesAvailable() const;
	TPtrC FileName() const;
private:
	CMtmDllInfo();
	CMtmDllInfo(const TUidType& aUidType,TInt aEntryPointOrdinalNumber,const TVersion aVersion);
	CMtmDllInfo(const CMtmDllInfo& aMtmDllInfo);
	void ConstructL(const TDesC& aHumanReadableName, const TDesC& aFilename);
private:
	HBufC* iHumanReadableName;         //  Should be less than KHumanReadableNameLength
public:
	/** Group of UIDs for the MTM. See NewL() for details. */
	TUidType iUidType;                 //  Three uids
	/** Ordinal of factory function for the MTM component */
	TInt iEntryPointOrdinalNumber;
	/** Version information for the MTM component */
	TVersion iVersion;

	/** Flag that indicates if the MTM can send messages */
	TBool iMessagingCapability;
	/** Flag that indicates if the MTM can handle body text */
	TBool iSendBodyCapability;
	/** Flag that indicates if settings have been made for the 
	MessagingCapability() and SendBodyCapability() flags */	
	TBool iCapabilitiesAvailable;
private:
	HBufC* iFilename;
	};


class CMtmDllInfoArray : public CArrayPtrFlat<CMtmDllInfo>
/** Collects the registration data for concrete MTM components, as encapsulated 
in CMtmDllInfo objects, into an array.

Basic array functionality is provided by the base class CArrayPtrFlat<CMtmDllInfo>.

It is used in the creation of registration data for an MTM group: see CMtmGroupData. 

@publishedAll
@released
*/
	{
public:
	IMPORT_C CMtmDllInfoArray();
	IMPORT_C ~CMtmDllInfoArray();
	IMPORT_C void AddMtmDllInfoL(CMtmDllInfo* aMtmDllInfo);
	};

class TCapabilitySet;

class CMtmGroupData : public CBase
/** Encapsulates the registration data for an MTM group. 

It is used to access and write the registration data file for an MTM group.
 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMtmGroupData* NewL(TUid aMtmTypeUid, TUid aTechnologyTypeUid,CMtmDllInfoArray* aMtmDllInfoArray, const TCapabilitySet& aMtmRequiredCaps);
	IMPORT_C static CMtmGroupData* NewL(const CMtmGroupData& aMtmGroupData);
	IMPORT_C static CMtmGroupData* NewL(RReadStream& aStream);
	IMPORT_C ~CMtmGroupData();
	IMPORT_C void InternalizeL(RReadStream& aStream);
	IMPORT_C void ExternalizeL(RWriteStream& aStream) const;  // not used but as it is exported it has been kept
	inline TUid MtmTypeUid() const;
	inline TUid TechnologyTypeUid() const;
	IMPORT_C const CMtmDllInfoArray& MtmDllInfoArray() const;
	IMPORT_C const TCapabilitySet& GetMtmRequiredCapabilities() const;
	IMPORT_C TBool operator==(const CMtmGroupData& aMtmGroupData) const;
private:
	CMtmGroupData(TUid aMtmTypeUid, TUid aTechnologyTypeUid, CMtmDllInfoArray* aMtmDllInfoArray, const TCapabilitySet& aMtmRequiredCaps);
	void ConstructL(const CMtmGroupData& aMtmGroupData);
	void ConstructL();
	void AppendMtmDllInfoArrayL(const CMtmDllInfoArray& aMtmDllInfoArray);
	CMtmDllInfoArray& MtmDllInfoArrayPrivate();
	CMtmGroupData(TUid aMtmTypeUid=KNullUid,TUid aTechnologyTypeUid=KNullUid);
	void AppendMtmDllInfoL(CMtmDllInfo* aMtmDllInfo);  // Leaves if second uid wrong
private:
	TUid iMtmTypeUid;
	TUid iTechnologyTypeUid;
	CMtmDllInfoArray* iMtmDllInfoArray;
	TCapabilitySet iMtmRequiredCaps;
	};


class MRegisteredMtmDllObserver
/** 
@publishedAll
@released
*/
	{
public:
	virtual TInt UseMtmGroup(TUid aMtmTypeUid)=0;
	virtual TInt ReleaseMtmGroup(TUid aMtmTypeUid)=0;
	};


//  Real non-derivable class

class CRegisteredMtmDll : public CTimer
/** Allows an MTM object to access registry information about itself.
 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CRegisteredMtmDll* NewL(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,const TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver);
	IMPORT_C ~CRegisteredMtmDll();

	inline TUid MtmTypeUid() const;
	inline TUid TechnologyTypeUid() const;
	inline const CMtmDllInfo& MtmDllInfo() const;

	inline TInt MtmDllRefCount() const;

	IMPORT_C TInt GetLibrary(RFs& aFs,RLibrary& aMtmDllLibrary);   // Increments reference count if successful or returns error 
	IMPORT_C void ReleaseLibrary();                       // Decrements access count
	                                                      // Should be called from destructor of any objects created from the library 
private:
	CRegisteredMtmDll(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver);
	void ConstructL(const CMtmDllInfo& aMtmDllInfo);
	void LoadLibraryL(RFs& aFs);   

	void RunL();

private:
	TUid iMtmTypeUid;
	TUid iTechnologyTypeUid;
	CMtmDllInfo* iMtmDllInfo;
	RLibrary iMtmDllLibrary;
	TInt iMtmDllRefCount;
	TTimeIntervalMicroSeconds32 iTimeoutMicroSeconds32;
	MRegisteredMtmDllObserver& iRegisteredMtmDllObserver;
	};



class CRegisteredMtmDllArray : public CArrayPtrFlat<CRegisteredMtmDll>
/**
@publishedAll
@released
*/
	{
public:
	IMPORT_C CRegisteredMtmDllArray();
	IMPORT_C ~CRegisteredMtmDllArray();
	IMPORT_C void AddRegisteredMtmDllL(CRegisteredMtmDll* aRegisteredMtmDll);
	};

//  Base class

class CMtmDllRegistry : public CBase
/** Accesses the various MTM component registries. 

The derived classes used by 
clients are CClientMtmRegistry, CMtmUiDataRegistry, and CMtmUiRegistry. It 
provides functions to discover the MTMs registered in such a registry. The 
registered MTMs can be regarded as existing in a list: you can use NumRegisteredMtmDlls() 
to get the number in the list, and MtmTypeUid() to index into it.
 
@publishedAll
@released
*/
	{
public:
	IMPORT_C ~CMtmDllRegistry();

	inline TInt NumRegisteredMtmDlls() const;
	IMPORT_C TUid MtmTypeUid(TInt anIndex) const;                                

	inline TBool IsPresent(TUid aMtmTypeUid) const;
	IMPORT_C TUid TechnologyTypeUid(TUid aMtmTypeUid) const;
	IMPORT_C const CMtmDllInfo& RegisteredMtmDllInfo(TUid aMtmTypeUid) const;                                
	IMPORT_C TBool IsInUse(TUid aMtmTypeUid) const;        // returns true if specified DLL in use 
	IMPORT_C TBool IsInUse() const;                        // returns true if any DLL in use 

protected:
	IMPORT_C CMtmDllRegistry(RFs& aFs,TUid aMtmDllTypeUid,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32=KMsvDefaultTimeoutMicroSeconds32); 

	IMPORT_C TInt MtmTypeUidToIndex(TUid anMtmTypeUid) const;

	IMPORT_C TInt AddRegisteredMtmDll(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver); // returns error
	IMPORT_C void RemoveRegisteredMtmDll(TUid aMtmTypeUid);         
	IMPORT_C void RemoveAllRegisteredMtmDlls();                     

private:
	void DoAddRegisteredMtmDllL(TUid aMtmTypeUid,TUid aTechnologyTypeUid,const CMtmDllInfo& aMtmDllInfo,MRegisteredMtmDllObserver& aRegisteredMtmDllObserver); // returns error

protected:
	RFs& iFs;
	TUid iMtmDllTypeUid;  //  Second Uid in all Dlls
	CRegisteredMtmDllArray iRegisteredMtmDllArray;  // Inline CArray, OK because no NewL();
	TTimeIntervalMicroSeconds32 iTimeoutMicroSeconds32;
	};


#include <msvreg.inl>

#endif
