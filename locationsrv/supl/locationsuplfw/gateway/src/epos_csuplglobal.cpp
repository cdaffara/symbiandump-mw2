/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Server global functions
*
*/



// INCLUDE FILES
#include <e32base.h>
//#ifdef _DEBUG
#include "epos_csupltrace.h"
//#endif
#include "epos_suplterminalerrors.h"
#include "epos_csuplglobal.h"

// ================= LOCAL FUNCTIONS =======================

TInt DataSizeL(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam)
    {
    TInt dataSize = aMessage.GetDesLength(aClientBufferParam);
    if (dataSize < 0)
        {
        aMessage.Panic(KSuplClientFault, ESuplBadDescriptor);
        User::Leave(KErrBadDescriptor);
        }
    return dataSize;
    }

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// SuplGlobal::Read
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt SuplGlobal::Read(const RMessage2& aMessage,
                  const TInt aParam,
                  TDes8& aDes,
                  TInt aOffset)
	{
    TInt ret = aMessage.Read(aParam, aDes, aOffset);
    if (ret != KErrNone)
        {
        aMessage.Panic(KSuplClientFault, ESuplBadDescriptor);
        }
    return ret;
    }

// ---------------------------------------------------------
// SuplGlobal::Read
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt SuplGlobal::Read(const RMessage2& aMessage,
                  const TInt aParam,
                  TDes16& aDes,
                  TInt aOffset)
    {
	TInt ret = aMessage.Read(aParam, aDes, aOffset);
    if (ret != KErrNone)
        {
        aMessage.Panic(KSuplClientFault, ESuplBadDescriptor);
        }
    return ret;
    }

// ---------------------------------------------------------
// SuplGlobal::Write
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt SuplGlobal::Write(const RMessage2& aMessage,
                   const TInt aParam,
                   const TDesC8& aDes,
                   TInt aOffset)
	{
	TInt ret = aMessage.Write(aParam, aDes, aOffset);
	if (ret != KErrNone)
        {
        aMessage.Panic(KSuplClientFault, ESuplBadDescriptor);
        }
    return ret;
	}

// ---------------------------------------------------------
// SuplGlobal::CopyClientBuffer8LC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC8* SuplGlobal::CopyClientBuffer8LC(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam)
    {
    // Create an empty server side buffer that will contain client data
    TInt dataSize = DataSizeL(aMessage, aClientBufferParam);
    HBufC8* buffer = HBufC8::NewLC(dataSize);

    // Populate server side buffer with client data
    TPtr8 ptrToBuf = buffer->Des();
    User::LeaveIfError(SuplGlobal::Read(aMessage, aClientBufferParam, ptrToBuf));
    return buffer;
    }

// ---------------------------------------------------------
// SuplGlobal::CopyClientBufferLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC* SuplGlobal::CopyClientBufferLC(
    const RMessage2& aMessage, 
    const TInt aClientBufferParam)
    {
    // Create an empty server side buffer that will contain client data
    TInt dataSize = DataSizeL(aMessage, aClientBufferParam);
    HBufC* buffer = HBufC::NewLC(dataSize);

    // Populate server side buffer with client data
    TPtr ptrToBuf = buffer->Des();
    User::LeaveIfError(SuplGlobal::Read(aMessage, aClientBufferParam, ptrToBuf));
    return buffer;
    }

// ---------------------------------------------------------
// SuplGlobal::RequestComplete
// ---------------------------------------------------------
//
void SuplGlobal::RequestComplete(const RMessage2& aMessage, TInt aError)
	{
	if(!aMessage.IsNull())	
		{
		// Complete the IPC message only if it is a valid message.
		aMessage.Complete(aError);
		}
	}

//#ifdef _DEBUG
CSuplTrace* traceLog = NULL;

void SuplGlobal::SetupTraceL()
    {
    if (!traceLog)
        {
        traceLog = CSuplTrace::NewL();
        }
    }

void SuplGlobal::TearDownTrace()
    {
    delete traceLog;
    traceLog = NULL;
    }

void SuplGlobal::Trace(const TDesC& aDescription,
                   const TDesC& aFilename,
                   const TInt aLineNumber)
    {
    if (traceLog)
        {
        // to epos log
        TInt err;
        TRAP(err, traceLog->TraceL(aDescription, aFilename, aLineNumber));
        
#ifdef _DEBUG
        // same to epocwind.out
        _LIT(KSuplTraceLogFormat , "SUPL::EPosSuplGateway: %S");
        RDebug::Print(KSuplTraceLogFormat, &aDescription);
#endif
        }
        
    }
//#endif

//  End of File  
