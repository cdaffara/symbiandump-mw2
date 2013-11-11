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
* Description:   Pointer holder for client's data
*
*/



#include "epos_suplterminalptrholder.h"

//-----------------------------------------------------------------------------
// CSuplSubSessnPtrHolder
//-----------------------------------------------------------------------------

/**
 * Allocates and constructs a <code>CSuplSubSessnPtrHolder</code> object.
 *
 * The function leaves if there is insufficient memory.
 *
 * @publishedAll
 * @param aNumberOfPtrs is the number of modifiable pointer descriptors to
 * create. These are used for asynchronous 'get' requests.
 * @param aNumberOfPtrCs is the number of read-only pointer descriptors to
 * create. These are used for asynchronous 'set' requests.
 * @return a newly created CSuplSubSessnPtrHolder object.
 */
EXPORT_C CSuplSubSessnPtrHolder* CSuplSubSessnPtrHolder::NewL(TInt aNumberOfPtrs,
                                                            TInt aNumberOfPtrCs)
	{
	CSuplSubSessnPtrHolder* self = new (ELeave) CSuplSubSessnPtrHolder;
	CleanupStack::PushL(self);
	self->ConstructL(aNumberOfPtrs, aNumberOfPtrCs);
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Leaving constructor for CSuplSubSessnPtrHolder.
 *
 * @publishedAll
 * @param aNumberOfPtrs is the number of modifiable pointer descriptors to
 * create. These are used for asynchronous 'get' requests.
 * @param aNumberOfPtrCs is the number of read-only pointer descriptors to
 * create. These are used for asynchronous 'set' requests.
 */
EXPORT_C void CSuplSubSessnPtrHolder::ConstructL(TInt aNumberOfPtrs,
                                                TInt aNumberOfPtrCs)
	{
	TPtr8 ptr(NULL, 0);
	TInt i;
	for (i = 0; i < aNumberOfPtrs; i++)
		{
		User::LeaveIfError(iPtrArray.Append(ptr));
		}

	TPtrC8 ptrC(NULL, 0);
	for (i = 0; i < aNumberOfPtrCs; i++)
		{
		User::LeaveIfError(iPtrCArray.Append(ptrC));
		}
	}

/**
 * Destructor for CSuplSubSessnPtrHolder.
 *
 * @publishedAll
 */
EXPORT_C CSuplSubSessnPtrHolder::~CSuplSubSessnPtrHolder()
	{
	iPtrCArray.Close();
	iPtrArray.Close();
	}

/**
 * An accessor for the modifiable pointer descriptors.
 *
 * @publishedAll
 * @param aIndex specifies the descriptor to return.
 * @return a reference to one of our contained pointer descriptors.
 */
EXPORT_C TPtr8& CSuplSubSessnPtrHolder::Ptr(TInt aIndex)
	{
	return iPtrArray[aIndex];
	}

/**
 * An accessor for the read-only pointer descriptors.
 *
 * @publishedAll
 * @param aIndex specifies the descriptor to return.
 * @return a reference to one of our contained pointer descriptors.
 */
EXPORT_C TPtrC8& CSuplSubSessnPtrHolder::PtrC(TInt aIndex)
	{
	return iPtrCArray[aIndex];
	}
