/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ECOM Interface defination for LBT Server Logic. The server 
*				 logic has to implement this interface.
*
*/


#include <ecom.h>
#include "lbtserverlogicbase.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtServerLogicBase::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtServerLogicBase* CLbtServerLogicBase::NewL(TUid aImplementaionUid)
	{
	TAny* ptr = REComSession::CreateImplementationL(aImplementaionUid,
													_FOFF(CLbtServerLogicBase,iDtor_ID_Key));
	return reinterpret_cast<CLbtServerLogicBase*>(ptr);
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicBase::CLbtServerLogicBase
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtServerLogicBase::CLbtServerLogicBase()
	{
	iDtor_ID_Key = TUid::Null();
	}

// ---------------------------------------------------------------------------
// CLbtServerLogicBase::~CLbtServerLogicBase
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtServerLogicBase::~CLbtServerLogicBase()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

//end of file
