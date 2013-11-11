/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Provides the interface between SUPL Gateway and SUPL Protocol Handler. 
				 This class provides the functionality of loading and unloading 
				 the Protocol handler ECom plug-in.
*
*/




#include <ecom/ecom.h>
#include <badesca.h>

#include "epos_csupltrace.h"

#include "epos_csuplprotocolmanagerbase.h"     // Our own base implementations for ECOM

_LIT(KTraceFileName,"SUPL_PH::epos_csuplprotocolmanagerbase.cpp");


//Resets & destroys array if leave occurs
void ResetAndDestroy(TAny* aArray)
    {
    	((RImplInfoPtrArray*)aArray)->ResetAndDestroy();
    }

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::CSUPLProtocolManagerBase
// Interface's (abstract base class's) constuctor
// 
// -----------------------------------------------------------------------------
// 
EXPORT_C CSUPLProtocolManagerBase::CSUPLProtocolManagerBase()
    {
    }

// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::~CSUPLProtocolManagerBase
// Interface's (abstract base class's) destructor
// 
// -----------------------------------------------------------------------------
// 
// Interface's (abstract base class's) destructor
EXPORT_C CSUPLProtocolManagerBase::~CSUPLProtocolManagerBase()
    {
    // Inform the ECOM framework that this specific instance of the 
    // interface has been destroyed.
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    }
    
// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::NewL
// Interface's (abstract base class's) static factory method implementation.
// Asks ECOM plugin framework to instantiate appropriate concret plugin 
// implementation.
// Returns instance of CSUPLProtocolManagerBase
// 
// -----------------------------------------------------------------------------
// 
EXPORT_C  CSUPLProtocolManagerBase* 
    CSUPLProtocolManagerBase::NewL(const TDesC8& /*aOperationName*/)
    {
    /*
 	CSuplTrace* trace = CSuplTrace::NewL();
 	CleanupStack::PushL(trace);
 	
	// using custom resolver
	TEComResolverParams resolverParams;
	resolverParams.SetDataType(aOperationName);
	resolverParams.SetWildcardMatch(ETrue);     // Allow wildcard matching
	
	trace->TraceL(_L("Calling REComSession::CreateImplementationL"), KTraceFileName, __LINE__); 
	
	//KProtocolManagerInterfaceDefinitionUid is the UID of our custom ECOM interface.
	TAny* ptr = REComSession::CreateImplementationL(
	KProtocolManagerInterfaceDefinitionUid,           
	_FOFF(CSUPLProtocolManagerBase,iDtor_ID_Key), 
	resolverParams);
	
	trace->TraceL(_L("Calling REComSession::ListImplementationsL"), KTraceFileName, __LINE__); 
	
	RImplInfoPtrArray implInfoArray;
	CleanupStack::PushL(TCleanupItem(*ResetAndDestroy, &implInfoArray));
	REComSession::ListImplementationsL(KProtocolManagerInterfaceDefinitionUid,resolverParams,
	implInfoArray);
	
	TUid implementationUid = implInfoArray[0]->ImplementationUid();
	
	CleanupStack::PopAndDestroy(&implInfoArray);

	CSUPLProtocolManagerBase* SuplProtocolManager = REINTERPRET_CAST(CSUPLProtocolManagerBase*, ptr);
	
	CleanupStack::PushL(SuplProtocolManager);
	SuplProtocolManager->ConstructL(implementationUid);	
	CleanupStack::Pop(SuplProtocolManager);
	
	trace->TraceL(_L("Returning CSUPLProtocolManagerBase*"), KTraceFileName, __LINE__); 
	
	CleanupStack::PopAndDestroy(trace);
	
	return SuplProtocolManager;*/
	return NULL;
    }       

// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::NewL
// Interface's (abstract base class's) static factory method implementation.
// Asks ECOM plugin framework to instantiate appropriate concret plugin 
// implementation.
// Returns instance of CSUPLProtocolManagerBase
// 
// -----------------------------------------------------------------------------
//
EXPORT_C  CSUPLProtocolManagerBase* 
    CSUPLProtocolManagerBase::NewL(const TUid &aImplementationUid)
    {
	//KProtocolManagerInterfaceDefinitionUid is the UID of our custom ECOM interface.

	CSuplTrace* trace = CSuplTrace::NewL();
	CleanupStack::PushL(trace);

	trace->TraceL(_L("Calling REComSession::CreateImplementationL"), KTraceFileName, __LINE__); 

	TAny* ptr = REComSession::CreateImplementationL(
	aImplementationUid,           
	_FOFF(CSUPLProtocolManagerBase,iDtor_ID_Key)); 

	CSUPLProtocolManagerBase* SuplProtocolManager = REINTERPRET_CAST(CSUPLProtocolManagerBase*, ptr);

	CleanupStack::PushL(SuplProtocolManager);
	SuplProtocolManager->ConstructL(aImplementationUid);	
	CleanupStack::Pop(SuplProtocolManager);

	trace->TraceL(_L("Returning CSUPLProtocolManagerBase*"), KTraceFileName, __LINE__); 
	CleanupStack::PopAndDestroy(trace);

	return SuplProtocolManager;	   
    }           
    
// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::ImplementationUid
// Returns implementation UID     
// 
// -----------------------------------------------------------------------------
//
EXPORT_C  TUid CSUPLProtocolManagerBase::ImplementationUid() const
    {
	return  iImplementationUid;
    }           

// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::Version
// Returns Version of implementation 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C  TInt CSUPLProtocolManagerBase::Version() const
    {
    /*TInt version = KErrNotFound;
    TRAP_IGNORE(version = GetVersionL());
	return version; */
	return 0;
    }           

// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::ConstructL
// 2 phase constructor
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CSUPLProtocolManagerBase::ConstructL(const TUid &aUid) 
    {
	iImplementationUid = aUid;
    }           

// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::GetVersionL
// Find out version of implementation
// 
// -----------------------------------------------------------------------------
//
TInt CSUPLProtocolManagerBase::GetVersionL() const
	{
	/*RImplInfoPtrArray implInfoArray;
	CleanupStack::PushL(TCleanupItem(*ResetAndDestroy, &implInfoArray));
	REComSession::ListImplementationsL(KProtocolManagerInterfaceDefinitionUid, implInfoArray);
	
	TInt version = KErrNotFound;
	TInt cnt = implInfoArray.Count();
	
	for (TInt index = 0; index < cnt; index++)
	{
		if (implInfoArray[index]->ImplementationUid() == iImplementationUid)
			{
				version = implInfoArray[index]->Version();
				break;
			}
	}

	CleanupStack::PopAndDestroy(&implInfoArray);
	return version;
	*/
	return 0;
	}
	
// -----------------------------------------------------------------------------
// CSUPLProtocolManagerBase::GetSUPLMessageVersionL
// Find out version of message
// 
// -----------------------------------------------------------------------------
//	
EXPORT_C TInt CSUPLProtocolManagerBase::GetSUPLMessageVersionL(TInt& /*aMajorVersion*/, const TDesC8& /*aReceivedMessage*/)
{
	return KErrGeneral;
}	

// end of file
