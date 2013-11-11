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
* Description:  Implementation file for MetaDataWriteCustomCommandParser
*
*/


// INCLUDES
#include "MetaDataWrite.h"
#include "MetaDataWrite.hrh"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataWriteCustomCommandParser::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMetaDataWriteCustomCommandParser* CMetaDataWriteCustomCommandParser::NewL(MMetaDataWriteCustomCommandImplementor& aImplementor)
{
    CMetaDataWriteCustomCommandParser* self = new(ELeave) CMetaDataWriteCustomCommandParser(aImplementor);
    return self;
}

// -----------------------------------------------------------------------------
// CMetaDataWriteCustomCommandParser::CMetaDataWriteCustomCommandParser
// C++ default constructor can NOT contain any code that might leave.
// -----------------------------------------------------------------------------
//
CMetaDataWriteCustomCommandParser::CMetaDataWriteCustomCommandParser(MMetaDataWriteCustomCommandImplementor& aImplementor) :
    CMMFCustomCommandParserBase(KUidMetaDataWriteCustomCommand),
    iImplementor(aImplementor)
{
    // do nothing
}

// -----------------------------------------------------------------------------
// CMetaDataWriteCustomCommandParser::CMetaDataWriteCustomCommandParser
// Decode and handle the message from the client
// -----------------------------------------------------------------------------
//
void CMetaDataWriteCustomCommandParser::HandleRequest(TMMFMessage& aMessage)
{
    if (aMessage.Destination().InterfaceId() == KUidMetaDataWriteCustomCommand)
    {
        TInt err = KErrNone;

        switch (aMessage.Function())
        {
            case KMetaDataWriteEnableDelayWrite:
                err = iImplementor.MmdwcEnableDelayWrite();
                break;
            case KMetaDataWriteCommitMetaData:
                err = iImplementor.MmdwcCommitMetaData();
                break;
            default:
                err = KErrNotSupported;
                break;
        }
        
        aMessage.Complete(err);
    }
    else
    {
        aMessage.Complete(KErrNotSupported);
    }
}

// End of file
