// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file
 @internalTechnology
 @released
*/

#ifndef __CMULTIPARTBINITERATOR_H__
#define __CMULTIPARTBINITERATOR_H__

#include <push/wapdecoder.h>
#include <push/cmultipartiteratorbase.h>

//##ModelId=3B659EE10005
class CMultipartBinIterator : public CMultipartIteratorBase
/** Splits a WAP binary multipart message into its parts. 

The class takes a WAP binary formatted multipart message (application/vnd.wap.mulitpart.*) and can pull 
 out the parts.

It implements the abstract multipart iterator interface defined in CMultipartIteratorBase.
@internalTechnology
@released */
	{
public:
	//##ModelId=3B659EE10037
	IMPORT_C static CMultipartBinIterator* NewL(CPushMessage& aPushMessage);

	// from CMultipartIteratorBase
	//##ModelId=3B659EE1002F
	IMPORT_C virtual TBool NextL();
	//##ModelId=3B659EE1002E
	IMPORT_C virtual void FirstL();
	//##ModelId=3B659EE1002D
	IMPORT_C virtual CPushMessage* PartL();
private:
	//##ModelId=3B659EE10024
	CMultipartBinIterator(CPushMessage& aPushMessage);
	//##ModelId=3B659EE10023
	void ConstructL();
private:
	//##ModelId=3B659EE1001B
	TWapBinCodex::TUintvar iMultiNumEntries;
	//##ModelId=3B659EE10012
	TInt iCurrentPart;
	};

#endif // __CMULTIPARTBINITERATOR_H__
