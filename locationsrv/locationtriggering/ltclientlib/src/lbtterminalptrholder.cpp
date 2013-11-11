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
* Description:  Pointer holder for client's data
*
*/


#include "lbtterminalptrholder.h" 

// ---------------------------------------------------------------------------
// CLbtSubSessnPtrHolder::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CLbtSubSessnPtrHolder* CLbtSubSessnPtrHolder::NewL(TInt aNumberOfPtrs,
                                                            TInt aNumberOfPtrCs)
	{
	CLbtSubSessnPtrHolder* self = new (ELeave) CLbtSubSessnPtrHolder;
	CleanupStack::PushL(self);
	self->ConstructL(aNumberOfPtrs, aNumberOfPtrCs);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CLbtSubSessnPtrHolder::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
// 
EXPORT_C void CLbtSubSessnPtrHolder::ConstructL(TInt aNumberOfPtrs,
                                                TInt aNumberOfPtrCs)
	{
	TPtr8 ptr(NULL, 100);
	TInt i;
	for (i = 0; i < aNumberOfPtrs; i++)
		{
		User::LeaveIfError(iPtrArray.Append(ptr));
		}

	TPtrC8 ptrC(NULL, 100);
	for (i = 0; i < aNumberOfPtrCs; i++)
		{
		User::LeaveIfError(iPtrCArray.Append(ptrC));
		}
	}

// ---------------------------------------------------------------------------
// CLbtSubSessnPtrHolder::~CLbtSubSessnPtrHolder()
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtSubSessnPtrHolder::~CLbtSubSessnPtrHolder()
	{
	iPtrCArray.Close();
	iPtrArray.Close();
	}

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::Ptr()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TPtr8& CLbtSubSessnPtrHolder::Ptr(TInt aIndex)
	{
	return iPtrArray[aIndex];
	}

// ---------------------------------------------------------------------------
// CLbtTriggerConditionArea::PtrC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC8& CLbtSubSessnPtrHolder::PtrC(TInt aIndex)
	{
	return iPtrCArray[aIndex];
	}


