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
* Description:  GetParitialObject operation helper function
*
*/
  

#include <mtp/mtpdatatypeconstants.h>

#include "ttypeflatbuf.h"

static const TMTPTypeFlatBase::TElementInfo KMTPTypeFlatBufElementInfo = { EMTPTypeFlat, 0, -1 };

// ----------------------------------------------------------------------------- 
// TMTPTypeFlatBuf::TMTPTypeFlatBuf
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
TMTPTypeFlatBuf::TMTPTypeFlatBuf( const TDes8& aBuffer )
    : TMTPTypeFlatBase()
    {
    SetBuffer( aBuffer );
    }

// ----------------------------------------------------------------------------- 
// TMTPTypeFlatBase::TElementInfo& TMTPTypeFlatBuf::ElementInfo
// Override to provide element info 
// -----------------------------------------------------------------------------
//
const TMTPTypeFlatBase::TElementInfo& TMTPTypeFlatBuf::ElementInfo( TInt /**aElementId*/ ) const
    {
    return KMTPTypeFlatBufElementInfo;
    }

// end of file

