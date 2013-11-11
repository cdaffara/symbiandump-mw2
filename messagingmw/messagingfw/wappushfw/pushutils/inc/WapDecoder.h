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

/**
 @file
 @internalTechnology
 @released
*/

#ifndef __WAPDECODER_H__
#define __WAPDECODER_H__

#include <e32base.h>

//##ModelId=3B659EDE01E1
class TWapBinCodex
/** WAP binary codex utility.

This class contains static functions for decoding WAP binary data. 
@internalTechnology
@released
*/
	{
public:
	
	class TUintvar 
	/** Represents the WAP variable length integer type.

	This is used for decoding and indexing through WAP Binary data.

	The format is defined in WAP WSP Section 8.1.2. It can be up to 5 (8-bit) 
	bytes long, with the first bit indicating a carryover. */
		{
		public:
			/** Constructor.
			
			Data members are initialised to zero. */
			TUintvar() : iValue(0), iOctetSize(0) {};
			/** Integer value. */
			TInt	iValue;
			/** Value size in number of bytes. */
			TInt	iOctetSize;
		};

	//##ModelId=3B659EDE01F7
	IMPORT_C static void ExtractUIntvarL(const TDesC8& aSource, TInt aStartIndex, TWapBinCodex::TUintvar& aMultiByte);
	//##ModelId=3B659EDE01EB
	IMPORT_C static void ExtractMultiOctetInteger(const TDesC8& aSource, TInt& aInt);
	IMPORT_C static TInt ParseMultiOctetInteger(const TDesC8& aSource, TInt& aInt);
	};

#endif
