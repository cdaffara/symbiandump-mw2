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
* Description: Provides functions for reading and changing
* settings in the Landmarks settings.
*
*/


#ifndef CPOSLMSETTINGSHANDLER_H
#define CPOSLMSETTINGSHANDLER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CRepository;

// CLASS DECLARATION

/**
* Provides functions for reading and changing
* settings in the Landmarks settings.
* A client can also listen to landmark settings events.
*
*  @version $Revision: 1.7 $, $Date: 2005/03/31 08:14:12 $
*/
class CPosLmSettingsHandler : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Creates a settings handler.
        *
        * @return A settings handler.
        */
        IMPORT_C static CPosLmSettingsHandler* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmSettingsHandler();

    public:  // New functions

        /**
        * Gets the default database URI from the Landmarks settings.
        * The client takes ownership of the descriptor object.
        *
        * @return The URI of the default landmark database.
        */
        IMPORT_C HBufC* GetDefaultDatabaseUriL();

        /**
        * Sets the default database URI in the Landmarks settings.
        *
        * @param aDatabaseUri The URI of the database which
        * should be set as default
        */
        IMPORT_C void SetDefaultDatabaseUriL(
        /* IN  */       const TDesC& aDatabaseUri
        );

        /**
        * Notifies a client when landmark settings have changed.
        *
        * @param aStatus The listener's request status.
        * @return KErrNone if successful, otherwise a system wide
        * error code.
        */
        IMPORT_C TInt NotifySettingsChanged(
        /* IN  */       TRequestStatus& aStatus
        );

        /**
        * Cancels a call to NotifySettingsChanged.
        *
        * @return KErrNone if the request was successfully cancelled,
        * otherwise a system wide error code.
        */
        IMPORT_C TInt CancelNotifySettingsChanged();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmSettingsHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CPosLmSettingsHandler( const CPosLmSettingsHandler& );
        // Prohibit assigment operator
        CPosLmSettingsHandler& operator= ( const CPosLmSettingsHandler& );

    private:    // Data

        CRepository* iRepository;

    };

#endif      // CPOSLMSETTINGSHANDLER_H

// End of File
