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

#ifndef _MDEDATABUFFER_H_
#define _MDEDATABUFFER_H_

#include <e32def.h>

// forward declaration
class CMdCSerializationBuffer;

class RMdEDataBuffer
{
public:
	IMPORT_C RMdEDataBuffer();

	IMPORT_C void Close();

	/**
	 * Set buffer to data buffer. Data buffer takes the ownership of the 
	 * buffer.
	 * 
	 * @param aBuffer serialization buffer
	 */
	void SetBufferL(CMdCSerializationBuffer* aBuffer);
	
	CMdCSerializationBuffer* GetBufferLC();

private:
	RMdEDataBuffer(const RMdEDataBuffer&);
	
private:
	CMdCSerializationBuffer* iBuffer;
};

#endif /*_MDEDATABUFFER_H_*/

