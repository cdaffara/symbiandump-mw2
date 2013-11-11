// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <tmsvpackednotifierrequest.h>
#include "MSVPANIC.H"

const TInt KTSecInfoNumTInts = sizeof(TSecurityInfo)/sizeof(TInt);


EXPORT_C TMsvPackedNotifierRequest::TMsvPackedNotifierRequest(HBufC8*& aBuffer)
: iBuffer(aBuffer)
	{
	}

EXPORT_C TInt TMsvPackedNotifierRequest::Pack(const CMsvEntrySelection& aSelection, const TSecurityInfo& aSecurityInfo)
	{
	// check the buffer is large enough for the CMsvSelection array and the TSecurityInfo object
	// note the extra +1 is to package the CMsvSelection array count
	TInt requiredSize = ((aSelection.Count()+KTSecInfoNumTInts+1)*sizeof(TInt));
	if (requiredSize>iBuffer->Des().MaxSize())
		{
		return KErrOverflow;
		}

	// set the buffer (contents) length
	iBuffer->Des().SetLength(requiredSize);

	// get pointer to start of iBuffer data 
	TInt* ptr = (TInt*) const_cast<TUint8*> (iBuffer->Ptr());
	
	// pack MSvSelection array size and contents
	*ptr++ = aSelection.Count();
	for (TInt count=0; count<aSelection.Count(); ++count)
		{
		*ptr++ = aSelection.At(count);
		}
		
	// get pointer to security info object
	TInt* secPtr = (TInt*) const_cast<TSecurityInfo*> (&aSecurityInfo);

	// pack TSecurityInfo size and copy into buffer
	for (TInt count=0; count<KTSecInfoNumTInts; ++count)
		{
		*ptr++ = *secPtr++;
		}

	return KErrNone;
	}


EXPORT_C void TMsvPackedNotifierRequest::UnpackL(const TDesC8& aBuffer, CMsvEntrySelection& aSelection, TSecurityInfo& aSecurityInfo)
	{
    __ASSERT_DEBUG(aSelection.Count()==0, PanicServer(EMsvOperationUnpackSelectionNotEmpty));	
	TInt* ptr = (TInt*) const_cast<TUint8*> (aBuffer.Ptr());

	// extract the CMsvEntrySelection object
	TInt count = *ptr++;
	while (count--)
		{
		aSelection.AppendL(*ptr++);
		}

	// extract the TSecurityInfo object
	TInt* secPtr = (TInt*) const_cast<TSecurityInfo*> (&aSecurityInfo);
	count = KTSecInfoNumTInts;
	while (count--)
		{
		*secPtr++ = *ptr++;
		}
	}
