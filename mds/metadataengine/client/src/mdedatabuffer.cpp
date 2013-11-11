/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Serializard data buffer container
*
*/

#include <mdedatabuffer.h>

#include "mdcserializationbuffer.h"

EXPORT_C RMdEDataBuffer::RMdEDataBuffer() : iBuffer( NULL )
	{
	}

EXPORT_C void RMdEDataBuffer::Close()
	{
	if( iBuffer )
		{
		delete iBuffer;
		iBuffer = NULL;
		}
	}

void RMdEDataBuffer::SetBufferL(CMdCSerializationBuffer* aBuffer)
	{
	if( !iBuffer )
		{
		iBuffer = aBuffer;
		}
	else
		{
		User::Leave( KErrCorrupt );
		}
	}

CMdCSerializationBuffer* RMdEDataBuffer::GetBufferLC()
	{
	CMdCSerializationBuffer* buffer = iBuffer;
	iBuffer = NULL; // cleanup stack handles deletion, if PushL leaves
	CleanupStack::PushL( buffer );
	return buffer;
	}

