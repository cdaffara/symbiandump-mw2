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
// BIOSVRMTM.CPP
// 
//

#include <e32uid.h>   // uids for Epoc32
#include <bioscmds.h>
#include "BIOSMTM.H"

#include <biodb.h>	// bio database

#include <biouids.h>	// contains panic codes
#include "REGPSDLL.H"   // Parser Registry - used to load the parser
#include "BSP.H"			// CBaseParser

// CRichText etc. includes
#include <txtrich.h>
#include <txtfmlyr.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <bifchangeobserver.h>
#include <biomessageuids.h>
#include "tmsvbioinfo.h"
#endif

EXPORT_C CBIOServerMtm* CBIOServerMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aEntry)
	{
	CBIOServerMtm* mysvrmtm=new(ELeave) CBIOServerMtm(aRegisteredMtmDll, aEntry);
	CleanupStack::PushL(mysvrmtm);
	mysvrmtm->ConstructL();
	CleanupStack::Pop();
	return mysvrmtm;
	}

CBIOServerMtm::~CBIOServerMtm()
	{
	Cancel();
	}

//
// Unsupported MTM commands
//
void CBIOServerMtm::CopyToLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::MoveToLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::DeleteAllL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::CreateL(TMsvEntry /*aNewEntry*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

void CBIOServerMtm::ChangeL(TMsvEntry /*aNewEntry*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

//
// BIOServer specific commands
//
void CBIOServerMtm::StartCommandL(CMsvEntrySelection& /*aSelection*/, TInt /*aCommand*/, const TDesC8& /*aParameter*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

//
// 
//
TBool CBIOServerMtm::CommandExpected()
	{
	return EFalse;
	}
//
//
//
const TDesC8& CBIOServerMtm::Progress()
	{
	User::Panic(_L("BIOS"), KBIOMessageSvrNotSupported);
	return KNullDesC8;
	}

//
// 
//
void CBIOServerMtm::DoCancel()
	{
	}

//
//
//
void CBIOServerMtm::DoRunL()
	{
	User::Leave(KErrNotSupported);
	}

//
//
//
void CBIOServerMtm::DoComplete(TInt /*aError*/)
	{
	}


//
// Constructor
//
CBIOServerMtm::CBIOServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aEntry):
CBaseServerMtm(aRegisteredMtmDll, aEntry)
	{
	}
		
//
// 2nd phase construction
//
void CBIOServerMtm::ConstructL()
	{

	CActiveScheduler::Add(this);
	}


