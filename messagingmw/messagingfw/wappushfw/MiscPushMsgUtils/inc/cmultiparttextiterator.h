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

#ifndef __CMULTIPARTTEXTITERATOR_H__
#define __CMULTIPARTTEXTITERATOR_H__


#include <e32std.h>
#include <e32base.h>

#include <push/cmultipartiteratorbase.h>
#include <push/pushdispatcher.h>


_LIT(KMultipartText, "multipart/");
_LIT(KMultipartBin, "application/vnd.wap.multipart");


//##ModelId=3B659EE4024E
class CMultipartTextIterator : public CMultipartIteratorBase
/** Splits a text multipart MIME message into its parts. 

This text formatted multipart is in RFC 2061 format (multipart/). The class can 
determine the boundaries and pull out the parts.

It implements the abstract multipart iterator interface defined in CMultipartIteratorBase. 
@internalTechnology
@released
*/
	{
public:
	//##ModelId=3B659EE4028A
	IMPORT_C static CMultipartTextIterator* NewL(CPushMessage& aPushMessage);
	//##ModelId=3B659EE40282
	IMPORT_C virtual ~CMultipartTextIterator();
		
	// from CMultipartIteratorBase
	//##ModelId=3B659EE40281
	IMPORT_C virtual TBool NextL();
	//##ModelId=3B659EE40280
	IMPORT_C virtual void FirstL();
	//##ModelId=3B659EE40279
	IMPORT_C virtual CPushMessage* PartL();
private:
	//##ModelId=3B659EE40277
	CMultipartTextIterator(CPushMessage& aPushMessage);
	//##ModelId=3B659EE40276
	void BoundaryParamL();
	//##ModelId=3B659EE4026E
	void ConstructL();
private:
	//##ModelId=3B659EE40264
	HBufC8*	iBoundary;
	//##ModelId=3B659EE4025B
	TPtrC8 iIndexPtr;
	};

#endif // __CMULTIPARTTEXTITERATOR_H__
