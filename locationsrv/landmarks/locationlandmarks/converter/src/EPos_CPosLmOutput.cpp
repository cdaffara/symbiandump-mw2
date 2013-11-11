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
* Description: Abstract base class for encoder output.
*
*
*/


// INCLUDE FILES
#include    <s32strm.h>
#include    <utf.h>
#include    "EPos_CPosLmOutput.h"
#include    "EPos_PosLmConverterUtils.h"

// CONSTANTS
const TInt KPosUtf8StringLength = 40;
const TInt KPosTUintMaxStringLength = 10;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmOutput::CPosLmOutput
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmOutput::CPosLmOutput()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmOutput::BaseConstructL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmOutput::BaseConstructL(RWriteStream& aWriteStream)
    {
    iWriteStream = &aWriteStream;
    }

// Destructor
CPosLmOutput::~CPosLmOutput()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmOutput::WriteL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmOutput::WriteL(const TDesC16& aDes)
    {
    CheckLowDiskL(aDes);

    TBuf8<KPosUtf8StringLength> utf8;
    TPtrC16 unicode(aDes);

    while (unicode.Length() != 0)
        {
        TInt returnValue =
            CnvUtfConverter::ConvertFromUnicodeToUtf8(utf8, unicode);
        User::LeaveIfError(returnValue);

        iWriteStream->WriteL(utf8);

        // Remove the converted source text part
        unicode.Set(unicode.Right(returnValue));
        }
    }

// -----------------------------------------------------------------------------
// CPosLmOutput::WriteAndDeleteL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmOutput::WriteAndDeleteL(TDesC16* aDes)
    {
    WriteL(*aDes);
    CleanupStack::PopAndDestroy(aDes);
    }

// -----------------------------------------------------------------------------
// CPosLmOutput::WriteTRealL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmOutput::WriteTRealL(TReal aValue)
    {
    TBuf<KDefaultRealWidth> buf;
    PosLmConverterUtils::RealToDesL(aValue, buf);

    WriteL(buf);
    }

// -----------------------------------------------------------------------------
// CPosLmOutput::WriteTUintL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmOutput::WriteTUintL(TUint aValue)
    {
    TBuf<KPosTUintMaxStringLength> buf;
    buf.Num(aValue);

    WriteL(buf);
    }

// -----------------------------------------------------------------------------
// CPosLmOutput::CommitL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmOutput::CommitL()
    {
    iWriteStream->CommitL();
    }

//  End of File
