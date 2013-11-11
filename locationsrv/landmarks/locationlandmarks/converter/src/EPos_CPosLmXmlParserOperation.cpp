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
* Description: Operation for parsing a XML file with Landmark Content Format.
*
*
*/


// INCLUDE FILES
#include <EPos_LandmarksErrors.h>
#include "EPos_CPosLmXmlParserOperation.h"
#include "EPos_CPosLmXmlParser.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmXmlParserOperation::CPosLmXmlParserOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmXmlParserOperation::CPosLmXmlParserOperation(CPosLmXmlParser& aLmXmlParser) 
 :  CPosLmConverterOperation(),
    iLmXmlParser(&aLmXmlParser),
    iStatusFlag(KPosLmOperationNotComplete)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmXmlParserOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmXmlParserOperation::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CPosLmXmlParserOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmXmlParserOperation* CPosLmXmlParserOperation::NewL(CPosLmXmlParser& aParser)
    {
    CPosLmXmlParserOperation* self =
        new(ELeave) CPosLmXmlParserOperation(aParser);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmXmlParserOperation::~CPosLmXmlParserOperation()
    {
    if (iLmXmlParser)
        {
        iLmXmlParser->OperationDestroyed();
        iLmXmlParser = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmXmlParserOperation::ParserDestroyed
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmXmlParserOperation::DisconnectParser()
    {
    iLmXmlParser = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmXmlParserOperation::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmXmlParserOperation::NextStepL(
    TReal32& aProgress)
    {
    __ASSERT_ALWAYS(iLmXmlParser,
        Panic(KPosLandmarksClientPanic, EPosLmProtocolBreak));

    iStatusFlag = iLmXmlParser->ParseNextPartL(aProgress);
    return iStatusFlag;
    }

// -----------------------------------------------------------------------------
// CPosLmXmlParserOperation::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmXmlParserOperation::HandleError(
    TInt& /*aError*/)
    {
    if (iLmXmlParser)
        {
        iLmXmlParser->ResetParserOnError();
        }
    }

//  End of File
