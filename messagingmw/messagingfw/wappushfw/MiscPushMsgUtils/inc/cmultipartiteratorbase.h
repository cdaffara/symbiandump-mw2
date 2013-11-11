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

#ifndef __CMULTIPARTITERATORBASE_H__
#define __CMULTIPARTITERATORBASE_H__

/** @file
@internalTechnology
@released
*/

#include <e32std.h>
#include <e32base.h>

#include <push/pushmessage.h>

_LIT8(KBoundaryParam, "boundary");
_LIT8(KBoundaryStartEnd, "--");
_LIT8(KCRLF, "\r\n");
_LIT8(KHdrSeparator, "\r\n\r\n");


// panics
#ifdef _DEBUG
_LIT(KPushInvalidContentType,"Invalid Content-Type");
_LIT(KPushPartIndexOutOfRange,"Part Index Out of range!");
#endif


class CMultipartIteratorBase : public CBase
/** WAP Push multipart message splitter base class. 

This class defines an abstract interface for iterating through multipart messages 
to return the message parts. 
@internalTechnology
@released
*/
	{
public:
	/** Moves the iterator to the next message part.
	
	@return ETrue if there is a next part, EFalse otherwise */
	virtual TBool NextL() = 0;
	/** Resets the iterator the beginning of the message. */
	virtual void FirstL() = 0;
	/** Gets the current message part.
	
	@return Message part */
	virtual CPushMessage* PartL() = 0;

protected:
	CMultipartIteratorBase(CPushMessage& aPushMessage);

protected:
	CPushMessage& iPushMessage;
	TUint iCurrentPartStart;
	};

#endif // __CMULTIPARTITERATORBASE_H__

