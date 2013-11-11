/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Identifies all implementations for an interface.
*
*/

#include "epos_comasuplecomhandler.h"
#include <ecom/ecom.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Named constructor
// ---------------------------------------------------------------------------
//
 COMASuplEComHandler* COMASuplEComHandler::NewL(const TInt aInterfaceUid)
	{
	COMASuplEComHandler* self = new(ELeave)COMASuplEComHandler(aInterfaceUid);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void COMASuplEComHandler::ConstructL()
	{
	InitializeImplementationListL();
	}

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplEComHandler::COMASuplEComHandler(const TInt aInterfaceUid)
	{
	iInterfaceUid = TUid::Uid(aInterfaceUid);
	iDtorIdKey = TUid::Null();
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//	
COMASuplEComHandler::~COMASuplEComHandler()
	{
	UnLoadImplementation();
	iImplementationList.ResetAndDestroy();
	iImplementationList.Close();
	}

// ---------------------------------------------------------------------------
// returns iImplementationList which contain implementations installed for
// a particular interface Uid.
// ---------------------------------------------------------------------------
//	
 const RImplInfoPtrArray& COMASuplEComHandler::GetImplementationList()
	{
	return iImplementationList;
	}

// ---------------------------------------------------------------------------
// Fills iImplementationList with implementations installed for
// a particular interface Uid.
// ---------------------------------------------------------------------------
//
void COMASuplEComHandler::InitializeImplementationListL()
	{
	REComSession::ListImplementationsL(iInterfaceUid, iImplementationList);
	}

// ---------------------------------------------------------------------------
// REComSession may be closed and garbage plug-ins will be unloaded.
// ---------------------------------------------------------------------------
//
void COMASuplEComHandler::UnLoadImplementation()
	{
	REComSession::FinalClose();
	}
