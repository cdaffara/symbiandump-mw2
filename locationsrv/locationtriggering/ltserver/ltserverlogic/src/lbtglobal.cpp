/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Global services for LBT server logic
*
*/


// INCLUDE FILES
#include <lbterrors.h>
#include "lbtglobal.h"
#include "lbtlogger.h"

// ================= LOCAL FUNCTIONS =======================

TInt DataSizeL(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam)
    {
    FUNC_ENTER("LbtGlobal::DataSizeL");
    TInt dataSize = aMessage.GetDesLength(aClientBufferParam);
    if (dataSize < 0)
        {
        User::Leave(KErrBadDescriptor);
        }
    return dataSize;
    }

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// LbtGlobal::Write
// ---------------------------------------------------------
//
TInt LbtGlobal::Write(const RMessage2& aMessage,
               	      const TInt aParam,
               	      const TDesC8& aDes,
               	      TInt aOffset)
	{
	FUNC_ENTER("LbtGlobal::Write");
	TInt ret = aMessage.Write(aParam, aDes, aOffset);
	if (ret != KErrNone)
        {
        aMessage.Panic(KLbtClientPanicCategory, KErrBadDescriptor);
        }
    return ret;
	}

// ---------------------------------------------------------
// LbtGlobal::Read
// ---------------------------------------------------------
//
TInt LbtGlobal::Read(const RMessage2& aMessage,
	                 const TInt aParam,
	                 TDes8& aDes,
	                 TInt aOffset)
    {
    FUNC_ENTER("LbtGlobal::Read");
	TInt ret = aMessage.Read(aParam, aDes, aOffset);
    if (ret != KErrNone)
        {
        aMessage.Panic(KLbtClientPanicCategory, KErrBadDescriptor);
        }
    return ret;
    }

// ---------------------------------------------------------
// LbtGlobal::RequestComplete
// ---------------------------------------------------------
//
void LbtGlobal::RequestComplete(const RMessage2& aMessage, TInt aError)
	{
	FUNC_ENTER("LbtGlobal::RequestComplete");	
	if(!aMessage.IsNull())	
		{
		// Complete the IPC message only if it is a valid message.
		aMessage.Complete(aError);
		}
	}

// ---------------------------------------------------------
// LbtGlobal::CopyClientBuffer8LC
// ---------------------------------------------------------
//
HBufC8* LbtGlobal::CopyClientBuffer8LC( const RMessage2& aMessage, 
									    const TInt aClientBufferParam)
    {
    FUNC_ENTER("LbtGlobal::CopyClientBuffer8LC");
    
    if( aMessage.IsNull() )	
		{
		return NULL;
		}

    // Create an empty server side buffer that will contain client data
    TInt dataSize = DataSizeL(aMessage, aClientBufferParam);
    HBufC8* buffer = HBufC8::NewLC(dataSize);

    // Populate server side buffer with client data
    TPtr8 ptrToBuf = buffer->Des();
    TInt retVal = LbtGlobal::Read(aMessage, aClientBufferParam, ptrToBuf);
    if(retVal != KErrNone)
    	{
    	User::Leave(KErrBadDescriptor);
    	}
    return buffer;
    }

// end of file 
