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
// User Interface registry
// 
//

// Messaging includes
#include <msvapi.h>
#include <msvruids.h>
#include <mtclbase.h>

// Specific includes
#include "MTUIREG.H"
#include "MTUIPAN.H"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"				
#endif	

//			 
//  CMtmUiRegistry  //
//
EXPORT_C CMtmUiRegistry* CMtmUiRegistry::NewL(CMsvSession& aMsvSession,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
/** Constructs and allocates a CMtmUiRegistry object. 

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
	CMtmUiRegistry* self = new (ELeave) CMtmUiRegistry(aMsvSession,aTimeoutMicroSeconds32);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	};

					 
CMtmUiRegistry::~CMtmUiRegistry()
/** Destructor. */
	{
	}


EXPORT_C CBaseMtmUi* CMtmUiRegistry::NewMtmUiL(CBaseMtm& aMtm)
/** Creates a User Interface MTM object for the specified MTM. 

The client should delete the returned object when it is no longer required. 

@return User Interface MTM object for specified MTM 
@param aMtm Client MTM for which to get the associated UI MTM
@leave KErrNotFound MTM type could not be found
@leave KErrNoMemory A memory allocation failed 
@leave KErrBadLibraryEntryPoint  Malformed MTM: a library entry point was not of the required type 
@leave Other DLL loading error codes if the DLL that provides the MTM cannot be loaded 
*/
	{
	CBaseMtmUi* newMtm = NULL;
	if(!IsPresent(aMtm.Type()))
		User::Leave(KErrNotFound);

	TInt index=MtmTypeUidToIndex(aMtm.Type());
	CRegisteredMtmDll* registeredmtmdll=iRegisteredMtmDllArray[index];
	RLibrary mtmdlllibrary;
	User::LeaveIfError(registeredmtmdll->GetLibrary(iFs,mtmdlllibrary));

	TInt refcount=registeredmtmdll->MtmDllRefCount();
	TRAPD(ret, newMtm = NewMtmL(mtmdlllibrary, aMtm, *registeredmtmdll));

	if ((ret!=KErrNone) && (registeredmtmdll->MtmDllRefCount()==refcount))  //  Library not released in mtm destructor
		registeredmtmdll->ReleaseLibrary();

	User::LeaveIfError(ret);
	return newMtm;
	}

CBaseMtmUi* CMtmUiRegistry::NewMtmL(const RLibrary& aLib, CBaseMtm& aMtm, CRegisteredMtmDll& aReg) const
	{
	TInt ordinal = aReg.MtmDllInfo().iEntryPointOrdinalNumber;
	TLibraryFunction libFunc = aLib.Lookup(ordinal);
	if (!libFunc)
		User::Leave(KErrBadLibraryEntryPoint);

	MtmUiFactoryFunctionL* pFunc = (MtmUiFactoryFunctionL*)libFunc;
	return (*pFunc)(aMtm, aReg);	
	}

// protected constructor
CMtmUiRegistry::CMtmUiRegistry(CMsvSession& aMsvSession,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
	: CObserverRegistry(aMsvSession, KUidMtmUiComponent, aTimeoutMicroSeconds32)
	{
	__DECLARE_NAME(_S("CMtmUiRegistry"));
	}

