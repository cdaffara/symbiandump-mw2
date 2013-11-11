/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: AlwaysOnline server command parser
*
*/


// INCLUDE FILES
#include <e32base.h>

#include "AOCommandParser.h"
#include "AlwaysOnlineManagerLogging.h"

// EXTERNAL DATA STRUCTURES
// EXTERNAL FUNCTION PROTOTYPES
// CONSTANTS
_LIT8( KAOCmdEnd, ":AOCmdEnd" );
_LIT8( KColon, ":" );
_LIT8( KProgressFormat, "AOCmdStart:*:*:AOCmdEnd" );
const TInt KAOStartTagLength = 11;
const TInt KAOColonHex = 0x3A;
// MACROS
// LOCAL CONSTANTS AND MACROS
// MODULE DATA STRUCTURES
// LOCAL FUNCTION PROTOTYPES
// FORWARD DECLARATIONS

// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------------------------------
// CAOCommandParser::CAOCommandParser()
// ----------------------------------------------------------------------------
//
CAOCommandParser::CAOCommandParser()
    {
    KAOMANAGER_LOGGER_FN1("CAOCommandParser::CAOCommandParser()");
    KAOMANAGER_LOGGER_FN2("CAOCommandParser::CAOCommandParser()");
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::~CAOCommandParser()
// ----------------------------------------------------------------------------
//
CAOCommandParser::~CAOCommandParser()
    {
    delete iProgress;
    iProgress = NULL;
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::ConstructL()
// ----------------------------------------------------------------------------
//
void CAOCommandParser::ConstructL( const TDesC8& aProgress )
    {
    iProgress = aProgress.AllocL();
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::NewL()
// ----------------------------------------------------------------------------
//
CAOCommandParser* CAOCommandParser::NewL( const TDesC8& aProgress )
    {
    CAOCommandParser* self = NewLC( aProgress );
    CleanupStack::Pop( self );

    return self;
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::NewLC()
// ----------------------------------------------------------------------------
//
CAOCommandParser* CAOCommandParser::NewLC( const TDesC8& aProgress )
    {
    CAOCommandParser* self = new ( ELeave ) CAOCommandParser();
    CleanupStack::PushL( self );
    self->ConstructL( aProgress );

    return self;
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::Parse()
// ----------------------------------------------------------------------------
//
TAOParserErrors CAOCommandParser::Parse()
    {
    KAOMANAGER_LOGGER_FN1("CAOCommandParser::Parse()");
    
    TAOParserErrors error = EAOParserNoError;
    
    // Is the format correct?
    TInt idx = iProgress->Match( KProgressFormat );

    KAOMANAGER_LOGGER_WRITE_FORMAT8("CAlwaysOnlineManager::HandleOpCompletion() : result: %S", iProgress);
    
    if ( idx != KErrNotFound )  // Was format correct?
        {
        KAOMANAGER_LOGGER_WRITE("CAOCommandParser::Parse() Format is correct");
        
        // Received progress:
        // "AOCmdStart:<UID of plugin>:<command enumeration>:<result>:AOCmdEnd
        TInt endTagIdx = iProgress->Find( KAOCmdEnd );
        TPtrC8 endBuf( iProgress->Mid( 
            KAOStartTagLength, 
            endTagIdx - KAOStartTagLength ) );
        
        // Now it should be like:
        // <UID of plugin>:<command enumeration>:<result>
        ParseUID( endBuf );
        ParseCommand( endBuf );
        ParseResult( endBuf );
        }
    else
        {
        KAOMANAGER_LOGGER_WRITE("CAOCommandParser::Parse() Unknown format");
        error = EAOParserUnknownFormat;
        }
    
    KAOMANAGER_LOGGER_FN2("CAOCommandParser::CAOCommandParser()");
    
    return error;
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::ParseUID()
// ----------------------------------------------------------------------------
//
void CAOCommandParser::ParseUID( TDesC8& aEndBuf )
    {
    TInt idx = aEndBuf.Find( KColon );
    TPtrC8 uidBuf( aEndBuf.Left( idx ) );
        
    TLex8 uidLex( uidBuf );
    TInt id;
    uidLex.Val( id );
    iUid = TUid::Uid( id );
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::ParseCommand()
// ----------------------------------------------------------------------------
//
void CAOCommandParser::ParseCommand( TDesC8& aEndBuf )
    {
    TInt idx = aEndBuf.Find( KColon );
    TChar colonChar( KAOColonHex );
    TInt secondColonIdx = aEndBuf.LocateReverse( colonChar );
    // Jump over colon characters
    idx++;
    secondColonIdx++;
    TPtrC8 cmdBuf( aEndBuf.Mid( idx, secondColonIdx - idx ) );
    
    TLex8 cmdLex( cmdBuf );
    cmdLex.Val( iCommand );
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::ParseResul()
// ----------------------------------------------------------------------------
//
void CAOCommandParser::ParseResult( TDesC8& aEndBuf )
    {
    TInt idx = aEndBuf.Find( KColon );
    TChar colonChar( KAOColonHex );
    TInt secondColonIdx = aEndBuf.LocateReverse( colonChar );
    // Jump over colon characters
    idx++;
    secondColonIdx++;

    TPtrC8 resultBuf ( aEndBuf.Right( aEndBuf.Length() - secondColonIdx ) );
    iResult = resultBuf.Left( iResult.MaxLength() );
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::Uid()
// ----------------------------------------------------------------------------
//
const TUid& CAOCommandParser::Uid()
    {
    return iUid;
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::Command()
// ----------------------------------------------------------------------------
//
TInt CAOCommandParser::Command()
    {
    return iCommand;
    }

// ----------------------------------------------------------------------------
// CAOCommandParser::Result()
// ----------------------------------------------------------------------------
//
const TDes8& CAOCommandParser::Result( )
    {
    return iResult;
    }

// End of File
