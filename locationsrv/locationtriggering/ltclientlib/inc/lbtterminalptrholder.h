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


#ifndef __LBTTERMINALPTRHOLDER_H__
#define __LBTTERMINALPTRHOLDER_H__

#include <e32base.h>
#include <lbscommon.h>

/*
 * Holds the TPtr8 and TPtrC8 arrays for all RPositionServer requests.
 *
 * internal
 */
class CLbtSubSessnPtrHolder : public CBase
	{
public:
    /**
     * Allocates and constructs a <code>CLbtSubSessnPtrHolder</code> object.
     *
     * The function leaves if there is insufficient memory.
     *
     * @publishedAll
     * @param aNumberOfPtrs is the number of modifiable pointer descriptors to
     * create. These are used for asynchronous 'get' requests.
     * @param aNumberOfPtrCs is the number of read-only pointer descriptors to
     * create. These are used for asynchronous 'set' requests.
     * @return a newly created CLbtSubSessnPtrHolder object.
     */
	IMPORT_C static CLbtSubSessnPtrHolder* NewL(TInt aNumberOfPtrs, 
	                                            TInt aNumberOfPtrCs);
	
	/**
	 * Destructor
	 */
 	IMPORT_C ~CLbtSubSessnPtrHolder();
	
	/**
     * An accessor for the modifiable pointer descriptors.
     *
     * @publishedAll
     * @param aIndex specifies the descriptor to return.
     * @return a reference to one of our contained pointer descriptors.
     */
	IMPORT_C TPtr8& Ptr(TInt aIndex);
	
	/**
     * An accessor for the read-only pointer descriptors.
     *
     * @publishedAll
     * @param aIndex specifies the descriptor to return.
     * @return a reference to one of our contained pointer descriptors.
     */
	IMPORT_C TPtrC8& PtrC(TInt aIndex);
	
protected:
    /**
     * Leaving constructor for CLbtSubSessnPtrHolder.
     *
     * @publishedAll
     * @param aNumberOfPtrs is the number of modifiable pointer descriptors to
     * create. These are used for asynchronous 'get' requests.
     * @param aNumberOfPtrCs is the number of read-only pointer descriptors to
     * create. These are used for asynchronous 'set' requests.
     */
	IMPORT_C void ConstructL(TInt aNumberOfPtrs, TInt aNumberOfPtrCs);
private:
    /**
     * TPtr8 pointer array.
     */
	RArray<TPtr8> iPtrArray;
	
	/**
     * TPtrC8 pointer array.
     */
	RArray<TPtrC8> iPtrCArray;
	};

#endif // __LbtTERMINALPTRHOLDER_H__
