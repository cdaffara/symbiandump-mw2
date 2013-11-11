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
* Description:  Used for representing value ranges of various types.
*
*/


#ifndef __MDERANGE_H__
#define __MDERANGE_H__

#include <e32base.h>

class CMdCSerializationBuffer;
   
enum TMdERangeType
	{
	EMdERangeTypeAny = 0,
	EMdERangeTypeEqual,
	EMdERangeTypeNotEqual,
	EMdERangeTypeLess,
	EMdERangeTypeLessOrEqual,
	EMdERangeTypeGreater,
	EMdERangeTypeGreaterOrEqual,
	EMdERangeTypeBetween,
	EMdERangeTypeNotBetween
	};

class TMdESerializeRange
	{
	public:
		static void SerializeL( CMdCSerializationBuffer& aBuffer, TMdERangeType aType,
				void* aMin, void* aMax, TInt aValueSize );
		static TUint32 RequiredBufferSize( TInt aValueSize );
	};
	
#include "mderange.inl"

#endif  // __MDERANGE_H__

