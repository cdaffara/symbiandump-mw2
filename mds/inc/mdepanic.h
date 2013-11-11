/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Panics the current thread with the
*                Nokia Metadata Engine Client DLL panic category 
*
*/


#ifndef __MDEPANIC_H__
#define __MDEPANIC_H__


#include <e32base.h>


/**
 * Panic utility class.
 */
NONSHARABLE_CLASS(TMdEPanic)
	{
public:

	/**
	 * Panic numbers.
	 */
	enum TReason
		{
		EInternal = 0,  // Internal error (that is, a bug in the Metadata Engine Client DLL)

		ESessionOpenInProgress,                             // 1
		ESessionDuplicateSchemaObserver,                    // 2
		ESessionUnknownSchemaObserver,                      // 3
		EItemDoesNotBelongToSession,                        // 4
		EItemNotInDatabase,                                 // 5
		ESchemaItemInvalidNameSpaceUri,                     // 6
		ESchemaItemSetDuplicateSchemaItem,                  // 7
		EQuantifiedPropertyDefSetDuplicatePropertyDef,      // 8
		EQuantifiedPropertyDefSetIllegalPropertyDefCount,   // 9
        EPropertyDefInvalidSize,                            // 10
		EIntPropertyDefInvalidMinAndMaxValues,              // 11
        ERealPropertyDefInvalidMinAndMaxValues,             // 12
		EObjectInvalidUri,                                  // 13
		EObjectInvalidPropertyDefType,                      // 14
		EObjectPropertyBelongsToAnotherObject,              // 15
		EPropertyInvalidSourceUri,                          // 16
		EPropertyReadOnly,                                  // 17
		ENumberPropertyIllegalTypeSize,                     // 18
		EEventInvalidSourceUri,                             // 19
		EEventInvalidParticipantUri,                        // 20
		EQueryStateIllegalOperation,                        // 21
        EQueryDoesntOwnResult,                              // 22
		EQueryDuplicateObserver,                            // 23
		EQueryUnknownObserver,                              // 24
		EConditionLocked,                                   // 25
        ESchemaItemEmptyName,                               // 26

/*		ESessionOpenFailed,
		EPropertyReadOnly,
		EPropertyInvalidOriginUri,
		EIntPropertyInvalidValue
*/		};

public:

	/**
	 * Panics the current thread with the Nokia Metadata Engine Client DLL panic category 
	 * and the specified panic number.
	 *
	 * @param aReason  panic number
	 */
	static void Panic(TReason aReason);
	};


#endif  // __MDEPANIC_H__
