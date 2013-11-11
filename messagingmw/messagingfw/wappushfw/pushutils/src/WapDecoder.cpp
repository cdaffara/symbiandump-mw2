// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "WapDecoder.h"

const TInt KMaxLongIntSize = 0x1E;	// Long-integer can have maximum 30 OCTETS (WSP Spec)

/**
Extracts a WAP variable length integer from a binary data buffer.

@param	aSource 
Input binary data buffer.

@param	aStartIndex
Position in aSource from which to extract the integer.

@param	aMultiByte
On return, the extracted variable length integer.

@leave	KErrCorrupt
A valid variable length integer was not found in the buffer.
*/
EXPORT_C void TWapBinCodex::ExtractUIntvarL(const TDesC8& aSource, TInt aStartIndex, TWapBinCodex::TUintvar& aMultiByte)
	{
	TUint8 byte = 0, topThreeBits = 0;
	aMultiByte.iValue = 0;
	aMultiByte.iOctetSize = 0;
	TInt lenLeft = aSource.Length() - aStartIndex;

	if (lenLeft <= 0)
		User::Leave(KErrCorrupt);
	
	// get the first octet
	byte = aSource[aStartIndex];

	// if the topThreeBits are set and we are using all 5 octets, this is corrupt data
	topThreeBits |= byte & 0x70; 
	
	// copy over the top byte
	aMultiByte.iValue |= byte & ~0x80;
	++aMultiByte.iOctetSize;

	// while the top 'continue' byte is set and we have more data
	while ((byte & 0x80) && (aMultiByte.iOctetSize < 5) && (--lenLeft > 0))
	{
		aMultiByte.iValue <<= 7;
		byte = aSource[aStartIndex + aMultiByte.iOctetSize++];
		aMultiByte.iValue |= byte & ~0x80;
	} 

	// last octet had continue bit set ... NOT allowed or aSource wasn't complete
	if (byte & 0x80)
		User::Leave(KErrCorrupt);

	// this was encoded wrong and the topThreeBits are about to shift off ... NOT allowed
	if (aMultiByte.iOctetSize == 5 && topThreeBits)
		User::Leave(KErrCorrupt);

	}

/**
Extracts a WAP-encoded multi-octet integer from a binary data buffer.

The function will panic if the binary data is corrupt.

@param	aSource
The binary data buffer containing the integer.

@param	aInt
The output argument with the parsed integer.

@panic
The binary data buffer is corrupted (debug only).

@deprecated TWapBinCodex::ParseMultiOctetInteger function.
Use the new 
*/
EXPORT_C void TWapBinCodex::ExtractMultiOctetInteger(const TDesC8& aSource, TInt& aInt)
/* 
 * NOTE - It is a straight copy from CHttpResponse as the function isn't exported, and it is  
 * not possible to change the interface to the Wapplugins dll. 
 */
	{
	// Get num bytes encoding the integer - 
	// we are positioned at that location in the source descriptor
	TUint8 numBytes = aSource[0];
	aInt = 0;
	__ASSERT_DEBUG(numBytes < aSource.Length(), User::Invariant());
	// Loop over the source, taking each byte and shifting it in to the count.  
    for (TInt count = 1; (count <= numBytes); count++)
        aInt = (aInt << 8) + aSource[count];
	}

/**
Extracts a WAP-encoded multi-octet integer from a binary data buffer.

If the integer could not be parsed, then the output argument aInt is not valid.

@param
aSource	The binary data buffer containing the integer.

@param
aInt	The output argument with the parsed integer.

@return
The number of bytes parsed or an error code. A value of KErrCorrupt indicates 
that the data buffer contained a corrupted multi-octet integer.
*/
EXPORT_C TInt TWapBinCodex::ParseMultiOctetInteger(const TDesC8& aSource, TInt& aInt)
	{
	// Get num bytes encoding the integer.
	TUint8 numBytes = aSource[0];
	aInt = 0;
	
	if( numBytes > aSource.Length() - 1 || numBytes > KMaxLongIntSize )
		return KErrCorrupt;
	
	// Loop over the source, taking each byte and shifting it in to the count.  
    for (TInt count = 1; (count <= numBytes); ++count)
        aInt = (aInt << 8) + aSource[count];

	return numBytes + 1;
	}
	
