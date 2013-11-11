/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides two helper functions for the database: 
*                 read and write a boolean value into a stream.
*
*/



// INCLUDE FILES

#include "CbsStreamHelper.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CbsStreamHelper::ReadBoolL
// Reads a boolen value from a stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CbsStreamHelper::ReadBoolL( 
    RReadStream& aStream )
    {
    TInt8 integer;
    aStream >> integer;

    TBool result( ETrue );
    switch ( integer )
        {
        case 0:
            result = EFalse;
            break;

        case 1:
            result = ETrue;
            break;

        default:
            User::Leave( KErrCorrupt );
            break;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CbsStreamHelper::WriteBoolL
// Writes a boolen value to a stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CbsStreamHelper::WriteBoolL( 
    RWriteStream& aStream, 
    TBool aBool )
    {
    TInt8 integer;
    if ( aBool == EFalse ) 
        {
        integer = 0;
        }
    else 
        {
        integer = 1;
        }
    aStream << integer;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File
