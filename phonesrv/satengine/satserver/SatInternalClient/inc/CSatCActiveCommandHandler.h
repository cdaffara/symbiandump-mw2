/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the handler for the SIM Application Toolkit
*                active commands.
*
*/


#ifndef CSATCACTIVECOMMANDHANDLER_H
#define CSATCACTIVECOMMANDHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include "MSatUiAdapter.h"

// FORWARD DECLARATIONS

class RSatUiSession;

// CLASS DECLARATION

/**
 *  This class handles end user -initiated commands.
 *  It is derived from an abstract interface (MSatUiAdapter) so that
 *  the implementation is not exposed to the user of this class.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.6
 */
class CSatCActiveCommandHandler : public CBase, public MSatUiAdapter
    {
    public: // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        static CSatCActiveCommandHandler* NewL( RSatUiSession* aSat );

        /**
         * Destructor.
         */
        virtual ~CSatCActiveCommandHandler();

    public: // New functions

        /**
         * From MSatUiAdapter
         *
         * Menu Selection active command handling.
         * @param aMenuItem The index of the selected menu item (starts from 0).
         * @param aHelpRequested A flag indicating whether help was req'd.
         */
        void MenuSelection(
            TInt aMenuItem,
            TBool aHelpRequested );

        /**
         * From MSatUiAdapter
         *
         * Session terminanation active command handling.
         * @param aType The type of termination
         */
        void SessionTerminated( TInt8 aType );

    private: // Constructors

        /**
         * C++ default constructor.
         * @param aSat A pointer to a session (does not take ownership).
         */
        CSatCActiveCommandHandler( RSatUiSession* aSat );

    private: // Data

        /**
         * Reference to the Sat API
         */
        RSatUiSession* iSession;

        /**
         * The data structure for the command data
         */        
        RSat::TMenuSelectionV1 iMenuSelectionData;

        /**
         * The data package for IPC
         */          
        RSat::TMenuSelectionV1Pckg iMenuSelectionPckg;

    };

#endif // CSATCACTIVECOMMANDHANDLER_H

// End of File
