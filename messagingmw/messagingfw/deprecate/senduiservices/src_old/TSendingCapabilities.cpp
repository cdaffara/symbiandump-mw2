/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Sending capabilities for sending services. Used by SendUi to
*                filter sending services from "Send" pop-up list query.
*
*/




// INCLUDE FILES
#include <TSendingCapabilities.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TSendingCapabilities::TSendingCapabilities
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C TSendingCapabilities::TSendingCapabilities()
        :
    iBodySize(0),
    iMessageSize(0),
    iFlags(0)
    {
    }

// ---------------------------------------------------------
// TSendingCapabilities::TSendingCapabilities
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------
//
EXPORT_C TSendingCapabilities::TSendingCapabilities(
    TInt aBodySize,
    TInt aMessageSize,
    TInt aFlags)
        :
    iBodySize(aBodySize),
    iMessageSize(aMessageSize),
    iFlags(aFlags)
    {
    }

// ---------------------------------------------------------
// TSendingCapabilities::TSendingCapabilities
// Overloaded equal operator
// might leave.
// ---------------------------------------------------------
//
EXPORT_C TBool TSendingCapabilities::operator==(
    const TSendingCapabilities& aSendingCapabilities ) const
    {
    TBool ret = EFalse;

    if ( iBodySize == aSendingCapabilities.iBodySize &&
         iMessageSize == aSendingCapabilities.iMessageSize &&
         iFlags == aSendingCapabilities.iFlags )
        {
        ret = ETrue;
        }

    return ret;
    }

// ---------------------------------------------------------
// TSendingCapabilities::TSendingCapabilities
// Overloaded not equal operator
// might leave.
// ---------------------------------------------------------
//
EXPORT_C TBool TSendingCapabilities::operator!=(
    const TSendingCapabilities& aSendingCapabilities ) const
    {
    return !( operator==( aSendingCapabilities ) );
    }

//  End of File 
