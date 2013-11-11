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
* Description: Class for executing the parsing of URL operation.
*
*
*/


// INCLUDE FILES
#include <EPos_LandmarksErrors.h>
#include "epos_cposlmurlparseroperation.h"
#include "epos_cposlmurlparser.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmUrlParserOperation::CPosLmUrlParserOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmUrlParserOperation::CPosLmUrlParserOperation( CPosLmUrlParser& aLmUrlParser ) 
 :  CPosLmConverterOperation(),
    iUrlParser( &aLmUrlParser ),
    iStatus( KPosLmOperationNotComplete )
    {
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParserOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmUrlParserOperation::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParserOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmUrlParserOperation* CPosLmUrlParserOperation::NewL( CPosLmUrlParser& aUrlParser )
    {
    CPosLmUrlParserOperation* self =
        new( ELeave ) CPosLmUrlParserOperation( aUrlParser );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParserOperation::~CPosLmUrlParserOperation
// Destructor
// -----------------------------------------------------------------------------
//
CPosLmUrlParserOperation::~CPosLmUrlParserOperation()
    {
    if ( iUrlParser )
        {
        iUrlParser->UrlOperationDestroyed();
        iUrlParser = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParserOperation::DisconnectUrlParser
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmUrlParserOperation::DisconnectUrlParser()
    {
    iUrlParser = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParserOperation::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmUrlParserOperation::NextStepL(
    TReal32& /*aProgress*/ )
    {
    __ASSERT_ALWAYS(iUrlParser,
        Panic(KPosLandmarksClientPanic, EPosLmProtocolBreak));
    // Start URL parsing
    iStatus = iUrlParser->ParseUrlL();
    return iStatus;
    }

// -----------------------------------------------------------------------------
// CPosLmUrlParserOperation::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmUrlParserOperation::HandleError(
    TInt& /*aError*/)
    {
    if (iUrlParser)
        {
        iUrlParser->ResetUrlParser();
        }
    }


//  End of File
