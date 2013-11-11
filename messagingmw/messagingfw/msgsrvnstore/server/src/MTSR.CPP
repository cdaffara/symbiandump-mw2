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
// MTSR.CPP
//

#include "MSVRUIDS.H"
#include "MTSR.H"
#include "MTSRUT.H"
#include "MSVPANIC.H"
#include "MSVENTRY.H"
#include "MsvSecurityCapabilitySet.h"
#include <tmsvsystemprogress.h>

#include <e32uid.h>
#include <s32file.h>
#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "cinstalledmtmgroup.h"
#include "msvconsts.h"				
#endif

_LIT(KDefaultRegistryFileStoreName, "\\private\\1000484b\\Mtm Registry v2");


const TUid KUidRegistryFileStore={0x10003C6A};
const TUid KUidRegistryRootStream={268439422};

const TInt KMtmInfoFileResourceId = 1;
const TInt KMtmCapabilitiesResourceId = 2;
const TInt KMtmSecurityCapabilitySetResourceId = 3;

EXPORT_C CBaseServerMtm::~CBaseServerMtm()
/** Destructor.

This cleans up the base class. CBaseServerMtm-derived objects are deleted by the 
Message Server when they are no longer required. 

Derived classes can implement a destructor to do any additional clean up tasks 
that they require. */
	{
	iRegisteredMtmDll.ReleaseLibrary();
	delete iServerEntry;
	}

/** Constructor. 

The function is passed a CMsvServerEntry object in aServerEntry. This object is used to access
and change Message Server entries. Its context is initially set either to the
parent of the entry or selection being operated on, or the entry itself. The
constructor stores it in the protected iServerEntry member.

Derived classes can implement a constructor to perform any additional MTM-specific 
setup that can be safely carried out in a constructor. Such constructors must call the 
base class constructor function.

@param aRegisteredMtmDll Registration data for the MTM DLL
@param aServerEntry Context on which to operate
*/
EXPORT_C CBaseServerMtm::CBaseServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry):
	CActive(EPriorityStandard),	iServerEntry(aServerEntry), iRegisteredMtmDll(aRegisteredMtmDll)
	{
	}

EXPORT_C void CBaseServerMtm::RunL()
/** Provides a simple implementation of CActive::RunL() that calls the derived 
class's DoRunL() function. If that function leaves, then the leave is trapped, 
and DoComplete() is called to handle the error.

This implementation ensures that derived classes handle leave errors in RunL(), 
rather than the default of the error being passed to the active scheduler. */
	{
	DoRunL();
	}


/** Handles the cases where RunL is leaving.
*/
EXPORT_C TInt CBaseServerMtm::RunError(TInt aError)
	{
	DoComplete(aError);
	return KErrNone;
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
EXPORT_C TAny* CBaseServerMtm::GetInterface(TUid /*aUid*/)
 	{
	return NULL;
 	}

/** 
This call leads to calling one of the server Mtms to populate the 
TMsvSystemProgress structure.
@param aOutSysProg The TMsvSystemProgress structure to be populated by the server
@return the error of the Extension_ method call
*/
EXPORT_C TInt CBaseServerMtm::SystemProgress(TMsvSystemProgress& aOutSysProg)
	{
	TAny* ptrNull = NULL;
	return Extension_(KUIDMsgMsvSystemProgress, ptrNull, &aOutSysProg);
	}

/**
Call to this method leads to call Extension_ method of server MTMs, to which
the thread handle of the client application is passed.
*/
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
TInt CBaseServerMtm::ClientThreadInfo(TThreadId aClientInfo, TBool aCapabilityCheck)
	{
	TAny* ptrNull = &aCapabilityCheck;
	return Extension_(KUIDMsgClientThreadInfo, ptrNull, &aClientInfo);
	}
#endif

TInt CBaseServerMtm::GetNonOperationMtmData(TNonOperationMtmDataType& aMtmDataType, TPtrC8& aResultBuffer)
	{
	TAny* mtmDataTypePtr = &aMtmDataType;
	return Extension_(KUidMsgNonOperationMtmData, mtmDataTypePtr, &aResultBuffer);
	}

/** The extension method provides a polymorphic behaviour to call the correct
SystemProgress function.
@param aExtensionId The Uid passed in as KUIDMsgMsvSystemProgress to obtain the
System Progress.
@return KErrExtensionNotSupported retuned if no Extension_ function is overriddden
by the derived class.
*/	
EXPORT_C TInt CBaseServerMtm::Extension_(TUint aExtensionId, TAny *&a0, TAny *a1)	
	{
	TInt ret = KErrNone;
	switch(aExtensionId)
		{
		case KUIDMsgMsvSystemProgress:
		case KUidMsgNonOperationMtmData:
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
		case KUIDMsgClientThreadInfo:
#endif		
			{
			ret = KErrExtensionNotSupported;
			}
			break;

		default:
			{
		// Chain to base class
			ret = CActive::Extension_(aExtensionId, a0, a1);
			}
			break;
		}
	return ret;
	}
// Empty declaration
EXPORT_C TInt CBaseServerMtm::ChangeEntriesL( const CMsvEntrySelection& /*aSelection*/,TInt /*aMark*/, TRequestStatus& aStatus )
    {
    //Empty declaration. Respective MTM implementation should be call .
    TRequestStatus aEmptyStatus;
    aEmptyStatus = aStatus ;
    
    return KErrNotSupported; // Specific to MTM. Base return Not supported.  
    }

EXPORT_C CServerMtmDllRegistry* CServerMtmDllRegistry::NewL(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
	{
	return new(ELeave) CServerMtmDllRegistry(aFs,aTimeoutMicroSeconds32); 
	}

EXPORT_C CServerMtmDllRegistry::~CServerMtmDllRegistry()
	{
	}

EXPORT_C CBaseServerMtm* CServerMtmDllRegistry::NewServerMtmL(TUid aMtmTypeUid, CMsvServerEntry* aInitialEntry)
	{
	CleanupStack::PushL(aInitialEntry); // Take ownership of the server entry

	if(!IsPresent(aMtmTypeUid))
		User::Leave(KErrNotFound);

	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	CRegisteredMtmDll* registeredmtmdll=iRegisteredMtmDllArray[index];
	RLibrary mtmdlllibrary;
	User::LeaveIfError(registeredmtmdll->GetLibrary(iFs, mtmdlllibrary));
	
	TInt refcount = registeredmtmdll->MtmDllRefCount();
	CBaseServerMtm* servermtm = NULL;
	CleanupStack::Pop(); // aInitialEntry - function takes ownership of this

	TRAPD(ret, servermtm = NewMtmL(mtmdlllibrary, aInitialEntry, *registeredmtmdll));

	if ((ret!=KErrNone) && (registeredmtmdll->MtmDllRefCount()==refcount))  //  Library not released in mtm destructor
		registeredmtmdll->ReleaseLibrary();
	
	User::LeaveIfError(ret);
	return servermtm;
	}

CBaseServerMtm* CServerMtmDllRegistry::NewMtmL(const RLibrary& aLib, CMsvServerEntry* aServerEntry, CRegisteredMtmDll& aReg) const
	{
	CleanupStack::PushL(aServerEntry); // Take ownership of the server entry
	typedef CBaseServerMtm*(*NewServerMtmL)(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry);

	TInt ordinal = aReg.MtmDllInfo().iEntryPointOrdinalNumber;
	TLibraryFunction libFunc = aLib.Lookup(ordinal);
	if (!libFunc)
		User::Leave(KErrBadLibraryEntryPoint);

	NewServerMtmL pFunc = (NewServerMtmL)libFunc;

	CleanupStack::Pop(); // aServerEntry - mtm should take ownership of this
	return (*pFunc)(aReg, aServerEntry);
	}
	
CServerMtmDllRegistry::CServerMtmDllRegistry(RFs& aFs,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32):
	CMtmDllRegistry(aFs,KUidMtmServerComponent,aTimeoutMicroSeconds32)
	{
	__DECLARE_NAME(_S("CServerMtmDllRegistry"));
	}

CInstalledMtmGroupArray::CInstalledMtmGroupArray():
	CArrayPtrFlat<CInstalledMtmGroup>(8)
	{
	}

CInstalledMtmGroupArray::~CInstalledMtmGroupArray()
	{
	ResetAndDestroy();
	}

void CInstalledMtmGroupArray::AddInstalledMtmGroupL(CInstalledMtmGroup* aInstalledMtmGroup)
	{
	CleanupStack::PushL(aInstalledMtmGroup);
	AppendL(aInstalledMtmGroup);
	CleanupStack::Pop();
	}


EXPORT_C CMtmRegistryControl* CMtmRegistryControl::NewL(RFs& anFs,CServerMtmDllRegistry& aServerMtmDllRegistry)
	{
	CMtmRegistryControl* mtmregistrycontrol=new(ELeave) CMtmRegistryControl(anFs,aServerMtmDllRegistry);
	CleanupStack::PushL(mtmregistrycontrol);
	mtmregistrycontrol->ConstructL();
	CleanupStack::Pop();
	return mtmregistrycontrol;	
	}

EXPORT_C CMtmRegistryControl::~CMtmRegistryControl()
	{
	delete iInstalledMtmGroupArray;
	}

EXPORT_C TInt CMtmRegistryControl::InstallMtmGroup(const TDesC& aFullName,TUid& aMtmTypeUid)
	{
	aMtmTypeUid=KNullUid;

	// Install the mtm
	TRAPD(ret, DoInstallMtmGroupL(aFullName, aMtmTypeUid));
	if ((ret!=KErrNone) && (aMtmTypeUid!=KNullUid))
		RemoveInstalledMtmGroup(aMtmTypeUid);
	return ret;
	}

EXPORT_C TInt CMtmRegistryControl::FullNameToMtmTypeUid(const TDesC& aFullName,TUid& aMtmTypeUid) const
	{
	aMtmTypeUid=KNullUid;
	TRAPD(ret, aMtmTypeUid = DoFindMtmTypeUidL(aFullName));
	if (ret==KErrNone)
		{
		TUidType uidtype(KPermanentFileStoreLayoutUid, KUidMsvDataComponent, aMtmTypeUid);
		TInt index=UidTypeToIndex(uidtype);
		if (index==iInstalledMtmGroupArray->Count())
			ret=KErrNotFound;
		else 
			aMtmTypeUid=iInstalledMtmGroupArray->At(index)->iMtmGroupData->MtmTypeUid();
		}
	return ret;
	}

TBool CMtmRegistryControl::IsResFileL(const TDesC& aFullName) const
	{
	// Check the extension to see if it's a resource file
	TParse parse;
	User::LeaveIfError(parse.Set(aFullName, NULL, NULL));

	// If first alpha character of extension is 'r' assume we're dealing with a resource file
	TPtrC ext(parse.Ext());
	return ext.Length() > 1 && TCharF(ext[1]) == TCharF('r');
	}

TUid CMtmRegistryControl::DoFindMtmTypeUidL(const TDesC& aFullName) const
	{
	TUid mtmTypeUid = KNullUid;

	// Is the file a resource file?
	if (!IsResFileL(aFullName))
		{
		// Get the third Uid
		TEntry entry;
		User::LeaveIfError(iFs.Entry(aFullName, entry));
		mtmTypeUid = entry[2];
		}
	else
		{
		// Open the resource file
		RResourceFile file;
		file.OpenL(iFs, aFullName);
		CleanupClosePushL(file);
		HBufC8* res = file.AllocReadLC(1);
		
		// Interpret the resource buffer
		TResourceReader reader;
		reader.SetBuffer(res);

		// Get the mtm type
		mtmTypeUid = TUid::Uid(reader.ReadInt32());
		CleanupStack::PopAndDestroy(2); // res, file
		}

	// Return the Uid Type
	return mtmTypeUid;
	}

EXPORT_C TInt CMtmRegistryControl::DeInstallMtmGroup(TUid aMtmTypeUid)
	{
	TRAPD(ret,DoDeInstallMtmGroupL(aMtmTypeUid));
	if (ret!=KErrNone)
		{
		TInt index=MtmTypeUidToIndex(aMtmTypeUid);
		iInstalledMtmGroupArray->At(index)->iIsInstalled=ETrue;
		}
	return ret;
	}

EXPORT_C TInt CMtmRegistryControl::UseMtmGroup(TUid aMtmTypeUid)
	{
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	__ASSERT_DEBUG(index<iInstalledMtmGroupArray->Count(),PanicServer(EMtsrInstalledMtmGroupNotPresent));
	iInstalledMtmGroupArray->At(index)->iClientUsageCount++;	
	return KErrNone;
	}

EXPORT_C TInt CMtmRegistryControl::ReleaseMtmGroup(TUid aMtmTypeUid)  
	{
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	__ASSERT_DEBUG(index<iInstalledMtmGroupArray->Count(),PanicServer(EMtsrInstalledMtmGroupNotPresent));
	iInstalledMtmGroupArray->At(index)->iClientUsageCount--;	
	return KErrNone;
	}

EXPORT_C TBool CMtmRegistryControl::IsInUse(TUid aMtmTypeUid) const
	{
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	__ASSERT_DEBUG(index<iInstalledMtmGroupArray->Count(),PanicServer(EMtsrInstalledMtmGroupNotPresent));
	return (*iInstalledMtmGroupArray->At(index)).iClientUsageCount>0;
	}

EXPORT_C TInt CMtmRegistryControl::FillRegisteredMtmDllArray(TUid aMtmDllTypeUid,CRegisteredMtmDllArray& aRegisteredMtmDllArray,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)  // Fill array with Dlls whose second uid is aMtmDllTypeUid
	{
	aRegisteredMtmDllArray.ResetAndDestroy();  //  Trash the array
	TUid mtmdlltypeuid[KMsvNumMtmDllTypes];  //  There must be an easier way to construct the array
	mtmdlltypeuid[EMtsrServerComponentIndex]	=KUidMtmServerComponent;
	mtmdlltypeuid[EMtsrClientComponentIndex]	=KUidMtmClientComponent;
	mtmdlltypeuid[EMtsrUiComponentIndex]		=KUidMtmUiComponent;
	mtmdlltypeuid[EMtsrUiDataComponentIndex]	=KUidMtmUiDataComponent;
/*
	mtmdlltypeuid[EMtsrDllTupe1Index]=KMsvDllType1Uid;
	mtmdlltypeuid[EMtsrDllType2Index]=KMsvDllType2Uid;
	mtmdlltypeuid[EMtsrDllType3Index]=KMsvDllType3Uid;
	mtmdlltypeuid[EMtsrDllType4Index]=KMsvDllType4Uid;
	mtmdlltypeuid[EMtsrDllType5Index]=KMsvDllType5Uid;
	mtmdlltypeuid[EMtsrDllType6Index]=KMsvDllType6Uid;
*/
	TInt index=0;
	for (; (index<KMsvNumMtmDllTypes) && (aMtmDllTypeUid!=mtmdlltypeuid[index]); index++)
		{
		}
	if (index==KMsvNumMtmDllTypes)
		return KErrNotSupported;
	TInt ret=KErrNone;
	TInt count1=iInstalledMtmGroupArray->Count();
	for (TInt i=0; i<count1; i++)
		{
		CMtmGroupData* mtmgroupdata=iInstalledMtmGroupArray->At(i)->iMtmGroupData;
		TInt count2=mtmgroupdata->MtmDllInfoArray().Count();
		if (index<count2)
			{
			CMtmDllInfo* mtmdllinfo=mtmgroupdata->MtmDllInfoArray()[index];
			if (mtmdllinfo->iUidType[2]!=KNullUid)
				{
				TRAP(ret,
					{	
					CRegisteredMtmDll* registeredmtmdll=CRegisteredMtmDll::NewL(mtmgroupdata->MtmTypeUid(),mtmgroupdata->TechnologyTypeUid(),*mtmdllinfo,aTimeoutMicroSeconds32,*this);
					
					// The following takes ownership of registeredmtmdll
					aRegisteredMtmDllArray.AddRegisteredMtmDllL(registeredmtmdll);
					});

				if (ret!=KErrNone)
					{
					aRegisteredMtmDllArray.ResetAndDestroy();
					break;
					}
				}
			}			
		}
	return ret;
	}


const CMtmGroupData& CMtmRegistryControl::GetMtmGroupDataReferenceL(TUid aMtmTypeUid) const
	{
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	if (index==iInstalledMtmGroupArray->Count())
		User::Leave(KErrNotFound);
	
	return *(iInstalledMtmGroupArray->At(index)->iMtmGroupData);
	}

EXPORT_C CMtmGroupData* CMtmRegistryControl::GetMtmGroupDataL(TUid aMtmTypeUid) const
	{
	return CMtmGroupData::NewL(GetMtmGroupDataReferenceL(aMtmTypeUid));
	}

EXPORT_C void CMtmRegistryControl::StoreRegistryL() const
	{
	TParse parsedname;
	User::LeaveIfError(parsedname.Set(iPathName,NULL,NULL));
	TInt ret=iFs.MkDirAll(parsedname.DriveAndPath());
	if (ret!=KErrAlreadyExists)
		User::LeaveIfError(ret);
	CDictionaryFileStore* registryfilestore = CDictionaryFileStore::OpenLC(iFs,parsedname.FullName(),KUidRegistryFileStore);
	RDictionaryWriteStream writestream;
	writestream.AssignLC(*registryfilestore,KUidRegistryRootStream);
	ExternalizeL(writestream);
	writestream.CommitL();
	writestream.Close();
	registryfilestore->CommitL();
	CleanupStack::PopAndDestroy(2);	// writestream and registryfilestore
	}

EXPORT_C void CMtmRegistryControl::RestoreRegistryL()
	{
	TParse parsedname;
	User::LeaveIfError(parsedname.Set(iPathName,NULL,NULL));
	TEntry entry;
	User::LeaveIfError(iFs.Entry(parsedname.FullName(),entry));
	CDictionaryFileStore* registryfilestore = CDictionaryFileStore::OpenLC(iFs,parsedname.FullName(),KUidRegistryFileStore);
	RDictionaryReadStream readstream;
	readstream.OpenLC(*registryfilestore,KUidRegistryRootStream);
	InternalizeL(readstream);
	readstream.Close();
	CleanupStack::PopAndDestroy();// readstream
	CleanupStack::PopAndDestroy();	// registryfilestore
	}

EXPORT_C void CMtmRegistryControl::InternalizeL(RReadStream& aStream)
	{
	__ASSERT_DEBUG(iInstalledMtmGroupArray->Count()==0,PanicServer(EMtsrRegistryControlStillInUse));
	TRAPD(ret,DoInternalizeL(aStream))
	if (ret!=KErrNone)
		{
		iInstalledMtmGroupArray->ResetAndDestroy();
		iServerMtmDllRegistry.RemoveAllRegisteredMtmDlls();
		User::Leave(ret);
		}
	}

EXPORT_C void CMtmRegistryControl::ExternalizeL(RWriteStream& aStream) const
	{
	TInt count1=iInstalledMtmGroupArray->Count(),count2=0;
	TInt i=0;
	for (; i<count1; i++)
		if ((*iInstalledMtmGroupArray->At(i)).iIsInstalled)
			count2++;
	aStream.WriteInt32L(count2);
	for (i=0; i<count1; i++)
		if ((*iInstalledMtmGroupArray->At(i)).iIsInstalled)
			aStream << *(iInstalledMtmGroupArray->At(i)->iFilename);
	}

CMtmRegistryControl::CMtmRegistryControl(RFs& anFs,CServerMtmDllRegistry& aServerMtmDllRegistry):
	iFs(anFs),
	iInstalledMtmGroupArray(),
	iServerMtmDllRegistry(aServerMtmDllRegistry)
	{
	__DECLARE_NAME(_S("CMtmRegistryControl"));
	}

void CMtmRegistryControl::ConstructL()
	{
	iInstalledMtmGroupArray = new(ELeave) CInstalledMtmGroupArray();

	TChar iDriveChar= iFs.GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(iDriveChar);
	systemDrive.Append(KDriveDelimiter);
	iPathName=systemDrive;
	iPathName.Append(KDefaultRegistryFileStoreName);
	
	TRAPD(ret,RestoreRegistryL());  
	//should not delete registry if in use
	if ((ret!=KErrNone) && (ret!=KErrInUse))
		{
		//ignore error if we can't delete a corrupt file; StoreRegistryL will leave
		iFs.SetAtt(iPathName,0,KEntryAttReadOnly|KEntryAttHidden|KEntryAttSystem);
		iFs.Delete(iPathName);
		StoreRegistryL();
		}
	}

TInt CMtmRegistryControl::MtmTypeUidToIndex(TUid aMtmTypeUid) const
	{
	TInt count=iInstalledMtmGroupArray->Count();
	TInt index=0;
	for (; (index<count) && (iInstalledMtmGroupArray->At(index)->iMtmGroupData->MtmTypeUid()!=aMtmTypeUid); index++)
		{
		}
	return index;
	}

TInt CMtmRegistryControl::UidTypeToIndex(TUidType aUidType) const
	{
	TInt count=iInstalledMtmGroupArray->Count();
	TInt index=0;
	for (; (index<count) && (iInstalledMtmGroupArray->At(index)->iUidType!=aUidType); index++)
		{
		}
	return index;
	}

CMtmGroupData *CMtmRegistryControl::LoadMTMFileL(const TDesC& aFullName, TUid &aUid)
	{
	if(IsResFileL(aFullName)==EFalse)
		return(LoadDatFileL(aFullName, aUid));
	else
		return(LoadResFileL(aFullName, aUid));
	}

CMtmGroupData *CMtmRegistryControl::LoadDatFileL(const TDesC& aFullName, TUid &aUid)
	{
	TEntry entry;
	User::LeaveIfError(iFs.Entry(aFullName,entry));  //  Check file exists
	if ((entry[0]!=KPermanentFileStoreLayoutUid) || (entry[1]!=KUidMsvDataComponent))
		User::Leave(KErrNotSupported);
	aUid=entry[2];
	CMtmGroupData* mtmgroupdata=ReadDataFileStoreL(aFullName);
	return(mtmgroupdata);
	}


CMtmGroupData *CMtmRegistryControl::LoadResFileL(const TDesC& aFullName, TUid &aUid)
	{
	TFileName fileName(aFullName);
	_LIT(KRssFileExtension, ".rsc");

	TParsePtrC filenamePPtr(fileName);
	TPtrC ext =filenamePPtr.Ext();
	if(ext!=(KRssFileExtension)) 
		{ //if ext is not .rsc, replace with rsc.  Always pass *.rsc to the BaflUtils::NearestLanguageFile
		TInt pos=fileName.FindF(ext);
		fileName.Replace(pos,fileName.Length()-pos,KRssFileExtension);
		}
	BaflUtils::NearestLanguageFile(iFs, fileName);

	// Open the resource file	
	RResourceFile file;
	file.OpenL(iFs, fileName);
	CleanupClosePushL(file);
	HBufC8* res = file.AllocReadLC(KMtmInfoFileResourceId);

	// Attempt to get the capabilities resource
	HBufC8* cap = NULL;

	TRAPD(error,cap = file.AllocReadL(KMtmCapabilitiesResourceId));
	TBool capAvailable = EFalse;
	TBool capSend = EFalse;
	TBool capBody = EFalse;
	if (error == KErrNone)
		{
		CleanupStack::PushL(cap);
		capAvailable = ETrue;
		TResourceReader capReader;
		capReader.SetBuffer(cap);
		// Get the send capability
		capSend = capReader.ReadInt8();
		// Get the body capability
		capBody = capReader.ReadInt8();

		CleanupStack::PopAndDestroy(cap);
		}

	// Interpret the resource buffer
	TResourceReader reader;
	reader.SetBuffer(res);
	
	// Get the mtm type
	TUid mtmTypeUid = { reader.ReadInt32() };
	aUid=mtmTypeUid;
		
	CMtmDllInfoArray* mtmDllInfoArray = new(ELeave) CMtmDllInfoArray();
	CleanupStack::PushL(mtmDllInfoArray);
	
	// Get technology type
	TUid technologyTypeUid = { reader.ReadInt32() };
	
	// Get number of mtm components
	TInt components(reader.ReadInt16());
	while(components--)
		{
		// Get the name of the component
		TPtrC name(reader.ReadTPtrC());
		
		// Get the type of component
		TUid componentUid = { reader.ReadInt32() };
		
		// Get specific Uid of the Dll
		TUid specificUid = { reader.ReadInt32() };
		
		// Get the Dll entry point
		TInt entryPoint(reader.ReadInt16());
		
		// Get version number
		TInt major = reader.ReadInt16();
		TInt minor = reader.ReadInt16();
		TInt build = reader.ReadInt16();
		TVersion version(major, minor, build);
		
		// Generate uid type
		TUidType componentUidType(KDynamicLibraryUid, componentUid, specificUid);
		
		TPtrC mtmFilename;
		if (specificUid.iUid == KUidMtmDefaultSpecificVal)
			{
			// We can assume the MTM resource file has been updated.
			// The version number should be <= 2.0.
			__ASSERT_DEBUG((version.iMajor < KMtmComponentCurrentMajorVersionNumber) || (version.iMajor == KMtmComponentCurrentMajorVersionNumber && version.iMinor == KMtmComponentCurrentMinorVersionNumber), PanicServer(EMsvBadMtmVersionNumber));

			// Get filename
			mtmFilename.Set(reader.ReadTPtrC());
			}
		// Add component info
		CMtmDllInfo* mtmDllInfo = CMtmDllInfo::NewL(name, componentUidType, mtmFilename, entryPoint, version);
		mtmDllInfo->SetMessagingCapability(capSend);
		mtmDllInfo->SetSendBodyCapability(capBody);
		mtmDllInfo->SetCapabilitiesAvailable(capAvailable);
		mtmDllInfoArray->AddMtmDllInfoL(mtmDllInfo);
		}
	
	// Are there any components?
	if (mtmDllInfoArray->Count() == 0)
		User::Leave(KErrNotFound);
	

	// The resource file *must* contain a security capability set once __SUPPORT_MESSAGING_API_V1__ is no longer defined
	__ASSERT_DEBUG(file.OwnsResourceId(KMtmSecurityCapabilitySetResourceId), PanicServer(EMsvSecurityCapabilitySetResourceIsNotPresent));
	HBufC8* securityResBuf = file.AllocReadLC(KMtmSecurityCapabilitySetResourceId);
	reader.SetBuffer(securityResBuf);
	TCapabilitySet mtmRequiredCaps;
	MsvSecurityCapabilitySetUtils::ReadFromResourceL(reader,mtmRequiredCaps);
	CleanupStack::PopAndDestroy(securityResBuf);
	CleanupStack::Pop(mtmDllInfoArray);
	// Generate the group data - transfer ownership of mtmDllInfoArray and mtmRequiredCaps
	CMtmGroupData* mtmgroupdata = CMtmGroupData::NewL(mtmTypeUid, technologyTypeUid, mtmDllInfoArray, mtmRequiredCaps);



	CleanupStack::PopAndDestroy(res);
	CleanupStack::PopAndDestroy(&file);
	return(mtmgroupdata);
	}

void CMtmRegistryControl::DoInstallMtmGroupL(const TDesC& aFullName,TUid& aMtmTypeUid)
	{
	aMtmTypeUid=KNullUid;
	TUid uid;
	CMtmGroupData* mtmgroupdata = LoadMTMFileL(aFullName,uid);
	TInt index=MtmTypeUidToIndex(mtmgroupdata->MtmTypeUid());
	if (index<iInstalledMtmGroupArray->Count())
		{
		delete mtmgroupdata;
		User::Leave(KErrAlreadyExists);
		}

	// The following takes ownership of the group data
	TUidType uidtype(KPermanentFileStoreLayoutUid, KUidMsvDataComponent, uid);
	CInstalledMtmGroup* installedmtmgroup=CInstalledMtmGroup::NewL(uidtype,mtmgroupdata,aFullName);
	aMtmTypeUid=mtmgroupdata->MtmTypeUid();

	// Install the Mtm
	AddInstalledMtmGroupL(installedmtmgroup);

	//  Attempt to store registry
	StoreRegistryL();
	}

CMtmGroupData* CMtmRegistryControl::ReadDataFileStoreL(const TDesC& aFullName) const
	{
	CFileStore* filestore = CPermanentFileStore::OpenLC(iFs,aFullName,EFileStream|EFileRead|EFileShareExclusive);
	TStreamId streamid=filestore->Root();
	RStoreReadStream readstream;
	readstream.OpenLC(*filestore,streamid);
	CMtmGroupData* mtmgroupdata=CMtmGroupData::NewL(readstream);
	CleanupStack::PopAndDestroy(2);	// readstream, filestore
	return mtmgroupdata;
	}

void CMtmRegistryControl::DoDeInstallMtmGroupL(TUid aMtmTypeUid)
	{
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	__ASSERT_DEBUG(index<iInstalledMtmGroupArray->Count(),PanicServer(EMtsrInstalledMtmGroupNotPresent));
	__ASSERT_DEBUG(!IsInUse(aMtmTypeUid),PanicServer(EMtsrInstalledMtmGroupStillInUse));
	CInstalledMtmGroup* installedmtmgroup=iInstalledMtmGroupArray->At(index);
	installedmtmgroup->iIsInstalled=EFalse;
	StoreRegistryL();
	RemoveInstalledMtmGroup(aMtmTypeUid);
	}

void CMtmRegistryControl::DoInternalizeL(RReadStream& aStream)
	{
	iInstalledMtmGroupArray->ResetAndDestroy();
	iServerMtmDllRegistry.RemoveAllRegisteredMtmDlls();
	TInt count=aStream.ReadInt32L();
	for (TInt i=0; i<count; i++)
		{
		HBufC *filename=HBufC::NewLC(aStream,KMaxFileName);
		TUid uid=KNullUid;
		CMtmGroupData *mtmgroupdata=NULL;
		TRAPD(error,mtmgroupdata=LoadMTMFileL(*filename,uid));
		if(error==KErrNone)
			{
			TUidType uidtype(KPermanentFileStoreLayoutUid, KUidMsvDataComponent, uid);
			CInstalledMtmGroup* installedmtmgroup=CInstalledMtmGroup::NewL(uidtype,mtmgroupdata,*filename);
			AddInstalledMtmGroupL(installedmtmgroup);
			}
		else if(error!=KErrNotFound)
			{
			User::Leave(error);
			}
		CleanupStack::PopAndDestroy(filename);		
		}
	User::LeaveIfError(FillRegisteredMtmDllArray(iServerMtmDllRegistry.iMtmDllTypeUid,iServerMtmDllRegistry.iRegisteredMtmDllArray,iServerMtmDllRegistry.iTimeoutMicroSeconds32));
	}

void CMtmRegistryControl::AddInstalledMtmGroupL(CInstalledMtmGroup* aInstalledMtmGroup)
	{
	iInstalledMtmGroupArray->AddInstalledMtmGroupL(aInstalledMtmGroup);
	CMtmDllInfo* mtmdllinfo=aInstalledMtmGroup->iMtmGroupData->MtmDllInfoArray()[EMtsrServerComponentIndex];
	if (mtmdllinfo->FileName().Length() > 0)
		User::LeaveIfError(iServerMtmDllRegistry.AddRegisteredMtmDll(aInstalledMtmGroup->iMtmGroupData->MtmTypeUid(),aInstalledMtmGroup->iMtmGroupData->TechnologyTypeUid(),*mtmdllinfo,*this));	
	}


void CMtmRegistryControl::RemoveInstalledMtmGroup(TUid aMtmTypeUid)
	{
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	if (index<iInstalledMtmGroupArray->Count())
		{

		delete iInstalledMtmGroupArray->At(index);
		iInstalledMtmGroupArray->Delete(index);
		}
	if (iServerMtmDllRegistry.IsPresent(aMtmTypeUid))
		iServerMtmDllRegistry.RemoveRegisteredMtmDll(aMtmTypeUid);
	}


