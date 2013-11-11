/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef CAOCOMMANDPARSER_H
#define CAOCOMMANDPARSER_H

// INCLUDES
#include <e32base.h>

#include <AlwaysOnlineManagerCommon.h>

// CONSTANTS
enum TAOParserErrors
    {
    EAOParserNoError = 0,
    EAOParserNoUid,
    EAOParserNoCmd,
    EAOParserNoResult,
    EAOParserUnknownFormat
    };
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS
// CLASS DECLARATION

/**
* Utility class to parse received messages from 
* CAOServerCommandHandler::FinalProgress()
*  @since S60 3.1
*/
class CAOCommandParser : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Creates object from CAOCommandParser and leaves it to cleanup stack
        * @since S60 3.1
        * @param aProgress, final progress
        * @return, Constructed object
        */
        static CAOCommandParser* NewLC( const TDesC8& aProgress );

        /**
        * Creates object from CAOCommandParser
        * @since S60 3.1
        * @param aProgress, final progress
        * @return, Constructed object
        */
        static CAOCommandParser* NewL( const TDesC8& aProgress );

        /**
        * Destructor
        * @since S60 3.1
        */
        virtual ~CAOCommandParser();

    public: // New functions
        /**
        * Parses the received message.
        * @since S60 3.1
        */
        TAOParserErrors Parse();
        
        /**
        * Gets the UID from received message.
        * @since S60 3.1
        * @return, plugin's UID
        */
        const TUid& Uid();
        
        /**
        * Gets the command from received message.
        * @since S60 3.1
        * @return, command issued
        */
        TInt Command();
        
        /**
        * Gets the result from received message.
        * @since S60 3.1
        * @return, plugin's result
        */
        const TDes8& Result();
        
    protected:  // Constructors

        /**
        * Default constructor for classCAOCommandParser
        * @since S60 3.1
        * @return, Constructed object
        */
        CAOCommandParser();

        /**
        * Symbian 2-phase constructor
        * @since S60 3.1
        * @param aProgress, final progress
        */
        void ConstructL( const TDesC8& aProgress );

    private:  // Functions from base classes
        void ParseUID( TDesC8& aEndBuf );
        void ParseCommand( TDesC8& aEndBuf );
        void ParseResult( TDesC8& aEndBuf );

    private:    // Data
        // Final progress
        HBufC8*                         iProgress;
        // Parsed uid
        TUid                            iUid;
        // Parsed command
        TInt                            iCommand;
        // Parsed result
        TBuf8<KAOFinalProgressLength>   iResult;
    };

#endif //  CAOCOMMANDPARSER_H

// End of File
