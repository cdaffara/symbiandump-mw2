/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Ecom methods implementation
*
*
*/


// INCLUDE FILES
#include <ecom/implementationproxy.h>
#include "EPos_CPosLmLmxEncoder.h"
#include "EPos_CPosLmLmxParser.h"
#include "EPos_CPosLmGpxParser.h"
#include "EPos_CPosLmKmlParser.h"
#include "EPos_LmConverterUids.hrh"
#include "epos_cposlmurlencoder.h"
#include "epos_cposlmurlparser.h"

// ==================== LOCAL FUNCTIONS ====================

// Define the interface UIDs
const TImplementationProxy KPosLmConverterImplTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KPosLmConverterEncoderImplUid, CPosLmLmxEncoder::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KPosLmConverterParserImplUid, CPosLmLmxParser::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KPosLmGpxParserImplUid, CPosLmGpxParser::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KPosLmKmlParserImplUid, CPosLmKmlParser::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KPosLmKmzParserImplUid, CPosLmKmlParser::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KPosLmUrlEncoderImplUid, CPosLmUrlEncoder::NewL ),
    IMPLEMENTATION_PROXY_ENTRY( KPosLmUrlParserImplUid, CPosLmUrlParser::NewL )
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount =
        sizeof(KPosLmConverterImplTable) / sizeof(TImplementationProxy);

    return KPosLmConverterImplTable;
    }

