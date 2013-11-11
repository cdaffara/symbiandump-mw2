// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// A revised version of the CRegisteredParserDll class.  This no longer uses a 
// reference to a CParserRegistrationData object.  Instead a UidType is passed in 
// the  constructor, or through a set method.  The class is used to find the file 
// which contains a parser, to load and maintain a reference count of the number of 
// objects using the dll, so that it can be unloaded when no longer required. 
// It will also unload the dll when a new parser is required.
// 
//

#include "REGPSDLL.H"
#include <f32file.h>
#include <s32strm.h>
#include <e32uid.h>
#include <biouids.h>
#include <msvreg.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include <biomessageuids.h>
#endif
    
EXPORT_C CRegisteredParserDll* CRegisteredParserDll::NewL(TDesC& aParserName)
/** Allocates and creates a new BIO parser counter.

@param aParserName Name of the BIO parser to handle
@return New BIO parser counter */
    {
    CRegisteredParserDll* self = new (ELeave) CRegisteredParserDll(aParserName);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CRegisteredParserDll::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }


CRegisteredParserDll::CRegisteredParserDll(const TDesC& aParserName)
: CTimer(EPriorityLow), iParserName(aParserName)
	{
	}

EXPORT_C CRegisteredParserDll::~CRegisteredParserDll()
/** Destructor. */
	{
	__ASSERT_DEBUG(iDllRefCount==0,User::Panic(_L("Dll in use"), KBIOMessageParserDllStillInUse));
    Cancel();
	iParserDllLibrary.Close();
	}

void CRegisteredParserDll::RunL()
    {
    iParserDllLibrary.Close();
    }

EXPORT_C TInt CRegisteredParserDll::GetLibrary(RFs& aFs,RLibrary& aDllLibrary)
/** Gets a handle to the parser DLL, and increments the reference count.

@param aFs File server handle
@param aDllLibrary On return, the parser DLL
@return Incremented reference count if successful; otherwise, a system wide 
error code */
	{
	TInt ret=KErrNone;
	if (iDllRefCount==0)
		TRAP(ret,LoadLibraryL(aFs));
	if (ret==KErrNone)
		{
		aDllLibrary = iParserDllLibrary;
		iDllRefCount++;
		}
	else
		iParserDllLibrary.Close();
	return ret;
	}

void CRegisteredParserDll::LoadLibraryL(RFs& /*aFs*/)   
	{
	// load parser using name - messaging api v2 style
	Cancel();
	if (iParserDllLibrary.Handle()==0)
		{
		User::LeaveIfError(iParserDllLibrary.Load(iParserName));
		}
	}

EXPORT_C void CRegisteredParserDll::ReleaseLibrary()
/** Releases the parser DLL, and decrements the reference count. */
	{
	iDllRefCount--;
	if(iDllRefCount==0)
		{
        After(KReleaseLibraryTimeout);
		}
	}

TInt CRegisteredParserDll::FindFullName(RFs& /*aFs*/,TDes& /*aFullName*/)
	{
	User::Invariant();
	return KErrNotSupported; // Only here to avoid warnings
	}
