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
// MTUDREG.H
//

// Standard includes
#include <e32std.h>
#include <e32uid.h>

// Messaging includes
#include <msvapi.h>
#include <mtclbase.h>
#include <msvruids.h>

// Specific includes
#include "MTUDCBAS.H"
#include "MTUDPAN.H"
#include "MTUDREG.H"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"				
#endif

//
//  CMtmUiDataRegistry  //
//
EXPORT_C CMtmUiDataRegistry* CMtmUiDataRegistry::NewL(CMsvSession& aMsvSession, TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
/** Gets a CMtmUiDataRegistry object. 

The client should delete this object when it is no longer required.

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
	CMtmUiDataRegistry* self = new (ELeave) CMtmUiDataRegistry(aMsvSession, aTimeoutMicroSeconds32);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CMtmUiDataRegistry::~CMtmUiDataRegistry()
	{
	}

EXPORT_C CBaseMtmUiData* CMtmUiDataRegistry::NewMtmUiDataLayerL(const TUid& aMtmTypeUid)
/** Creates a UI Data MTM object for the MTM specified by aMtmTypeUid.

The client should delete the returned object when it is no longer required.

@param aMtmTypeUid UID of MTM to obtain 
@leave KErrNotFound aMtmTypeUid does not specify a registered MTM 
@leave KErrNoMemory A memory allocation failed 
@leave KErrBadLibraryEntryPoint Malformed MTM: a library entry point was not 
of the required type 
@leave DLL loading error codes The DLL that provides the MTM cannot be loaded 
@return UI Data MTM object for specified MTM */
	{
	CBaseMtmUiData* newMtmUd = NULL;

	if(!IsPresent(aMtmTypeUid))
		User::Leave(KErrNotFound);
	TInt index=MtmTypeUidToIndex(aMtmTypeUid);
	CRegisteredMtmDll* registeredmtmdll=iRegisteredMtmDllArray[index];
	RLibrary mtmdlllibrary;
	User::LeaveIfError(registeredmtmdll->GetLibrary(iFs, mtmdlllibrary));

	TInt refcount = registeredmtmdll->MtmDllRefCount();
	TRAPD(ret, newMtmUd = NewMtmL(mtmdlllibrary, *registeredmtmdll));

	if ((ret!=KErrNone) && (registeredmtmdll->MtmDllRefCount()==refcount))  //  Library not released in mtm destructor
		registeredmtmdll->ReleaseLibrary();
	User::LeaveIfError(ret);	
	return newMtmUd;
	}

CBaseMtmUiData* CMtmUiDataRegistry::NewMtmL(const RLibrary& aLib, CRegisteredMtmDll& aReg) const
	{
	TInt ordinal = aReg.MtmDllInfo().iEntryPointOrdinalNumber;
	TLibraryFunction libFunc = aLib.Lookup(ordinal);
	if (!libFunc)
		User::Leave(KErrBadLibraryEntryPoint);

	MtmUiDataFactoryFunctionL* pFunc = (MtmUiDataFactoryFunctionL*)libFunc;
	return (*pFunc)(aReg);
	}

// protected constructor
CMtmUiDataRegistry::CMtmUiDataRegistry(CMsvSession& aMsvSession, TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
	: CObserverRegistry(aMsvSession, KUidMtmUiDataComponent, aTimeoutMicroSeconds32)
	{
	__DECLARE_NAME(_S("CMtmUiDataRegistry"));
	}

