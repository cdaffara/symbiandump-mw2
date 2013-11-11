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
// MTCLREG.H
// 
//

#include <e32std.h>
#include "MSVRUIDS.H"
#include "MSVAPI.H"
#include "MTCLBASE.H"
#include "MTCLREG.H"
#include "MSVPANIC.H"
			 
EXPORT_C CClientMtmRegistry* CClientMtmRegistry::NewL(CMsvSession& aMsvSession,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
/** Gets a CClientMtmRegistry object. The client should delete this object when 
it is no longer required.

The registry keeps a reference count of the number of instances of each MTM 
in use. When that reference count falls to zero, the DLL that provides the 
MTM is unloaded. However, this is not done immediately, but only after the 
time specified in aTimeoutMicroSeconds32. This increases efficiency in cases 
where the DLL is required again shortly.

@param aMsvSession The client's Message Server session 
@param aTimeoutMicroSeconds32 Time to wait before unloading MTM DLLs 
@leave KErrNoMemory A memory allocation failed 
@return A pointer to a newly allocated and initialised object */
	{
	CClientMtmRegistry* self = new (ELeave) CClientMtmRegistry(aMsvSession,aTimeoutMicroSeconds32);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

					 
void CClientMtmRegistry::ConstructL()
	{
	CObserverRegistry::ConstructL();
	}


/** Destructor.
*/
EXPORT_C CClientMtmRegistry::~CClientMtmRegistry()
	{}


EXPORT_C CBaseMtm* CClientMtmRegistry::NewMtmL(TUid aMtmTypeUid)
/** Creates a Client-side MTM object for the specified MTM UID. 

The client should delete the returned object when it is no longer required.

@param aMtmTypeUid UID of MTM to obtain 
@leave KErrNotFound aMtmTypeUid does not specify a registered MTM 
@leave KErrNoMemory A memory allocation failed 
@leave KErrBadLibraryEntryPoint Malformed MTM: a library entry point was not 
of the required type 
@leave DLL loading error codes The DLL that provides the MTM cannot be loaded 
@return Client-side MTM object for specified MTM */
	{
	if(!IsPresent(aMtmTypeUid))
		User::Leave(KErrNotFound);
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	CRegisteredMtmDll* registeredmtmdll=iRegisteredMtmDllArray[index];
	RLibrary lib;
	User::LeaveIfError(registeredmtmdll->GetLibrary(iFs,lib));

	CBaseMtm* newMtm = NULL;
	TInt refcount=registeredmtmdll->MtmDllRefCount();
	TRAPD(ret, newMtm = DoNewMtmL(lib, *registeredmtmdll));

	if ((ret!=KErrNone) && (registeredmtmdll->MtmDllRefCount()==refcount))  //  Library not released in mtm destructor
		registeredmtmdll->ReleaseLibrary();
	User::LeaveIfError(ret);	

	return newMtm;
	}

CBaseMtm* CClientMtmRegistry::DoNewMtmL(const RLibrary& aLib, CRegisteredMtmDll& aReg) const
	{
	TLibraryFunction libFunc = aLib.Lookup(aReg.MtmDllInfo().iEntryPointOrdinalNumber);
	if (!libFunc)
		User::Leave(KErrBadLibraryEntryPoint);

	MtmFactoryFunctionL* pFunc = (MtmFactoryFunctionL*) libFunc;
	return (*pFunc)(aReg, iMsvSession);
	}

CClientMtmRegistry::CClientMtmRegistry(CMsvSession& aMsvSession, TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
	: CObserverRegistry(aMsvSession, KUidMtmClientComponent, aTimeoutMicroSeconds32)
	{}


