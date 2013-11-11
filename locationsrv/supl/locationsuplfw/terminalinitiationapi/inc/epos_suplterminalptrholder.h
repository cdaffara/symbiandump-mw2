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



#ifndef __SUPLTERMINALPTRHOLDER_H__
#define __SUPLTERMINALPTRHOLDER_H__

#include <e32base.h>
#include <lbscommon.h>

class CSuplSubSessnPtrHolder : public CBase
/*
 * Holds the TPtr8 and TPtrC8 arrays for all RPositionServer requests.
 *
 * \internal
 */
	{
public:
	IMPORT_C static CSuplSubSessnPtrHolder* NewL(TInt aNumberOfPtrs,
	                                            TInt aNumberOfPtrCs);
	IMPORT_C ~CSuplSubSessnPtrHolder();
	IMPORT_C TPtr8& Ptr(TInt aIndex);
	IMPORT_C TPtrC8& PtrC(TInt aIndex);
protected:
	IMPORT_C void ConstructL(TInt aNumberOfPtrs, TInt aNumberOfPtrCs);
private:
	RArray<TPtr8> iPtrArray;
	RArray<TPtrC8> iPtrCArray;
	};

#endif // __SUPLTERMINALPTRHOLDER_H__
