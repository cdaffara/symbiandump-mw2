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
* Description: Declaration of Client API for the landmark server.
*
*
*/


#ifndef RPOSLANDMARKSERVER_H
#define RPOSLANDMARKSERVER_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include "EPos_TPosLmDatabaseEvent.h"
#include "EPos_TPosLmDatabaseSettings.h"
#include "EPos_HPosLmDatabaseInfo.h"

// FORWARD DECLARATIONS
class CPosLandmarkServerExtension;

// CLASS DECLARATION

/**
*  Client API for the landmark server.
*
*  @lib eposlmservlib.lib
*  @since S60 3.0
*  @version $Revision: 1.4 $, $Date: 2005/07/07 13:40:03 $
*/
class RPosLandmarkServer : public RSessionBase
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        IMPORT_C RPosLandmarkServer();

    public: // New functions


        /**
        * Connect to the landmark server.
        *
        * The server is started if it is not already running.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since S60 3.0
        * @return @p KErrNone if successful, otherwise a system-wide error
        *   code.
        */
        IMPORT_C TInt Connect();

        /**
        * Closes the session to the server
        *
        * @since S60 3.0
        */
        IMPORT_C void Close();

        /**
        * Lists all URIs in the server's database registry for a certain
        * protocol.
        *
        * The client specifies a protocol string as input to this function.
        * The function only returns a list of the databases which are
        * accessed through this protocol.
        *
        * The client takes ownership of the returned array.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since S60 3.0
        * @param aProtocol The protocol for which database URIs should be
        *   listed.
        * @return The list of database URIs.
        */
        IMPORT_C CDesCArray* ListDatabasesLC(
        /* IN  */       const TDesC& aProtocol
        );

        /**
        * Lists information about each landmark database in the
        * server's database registry.
        *
        * The client specifies a protocol string as input to this function.
        * The function only returns information about the databases
        * which are accessed through this protocol.
        *
        * The client specifies an array which is populated by this function.
        * The client takes ownership of all information objects in the array.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since S60 3.0
        * @param aDatabaseInfoArray On return, contains information about
        *   the landmark databases. Any objects which are in the array when
        *   it is passed to this function will be not be removed.
        * @param aProtocol The protocol for which database URIs should be
        *   listed.
        */
        IMPORT_C void ListDatabasesL(
        /* OUT */       RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
        /* IN  */       const TDesC& aProtocol = KNullDesC
        );

        /**
        * Registers a landmark database.
        *
        * The landmark database is then returned when listing landmark
        * databases.
        *
        * The client supplies an information object containing the URI of the
        * database to register. The information object can also contain
        * database settings, e.g. a display name for the database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since S60 3.0
        * @param aDatabaseInfo Information about the landmark database to
        *   register.
        * @return @p KErrNone if successful.\n
        *   @p KErrArgument if the URI is incorrect.\n
        *   @p KErrAlreadyExists if the database has already been registered.
        */
        IMPORT_C TInt RegisterDatabase(
        /* IN/OUT */       HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * Unregisters a landmark database.
        *
        * After this, the landmark database will not be returned when listing
        * landmark databases.
        *
        * If the database has not been registered, nothing happens.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since S60 3.0
        * @param aDatabaseUri The URI of the database to unregister.
        * @param aDriveLetter The drive where the database resides. If
        *   drive is not applicable, 0 should be specified.
        * @return @p KErrNone if successful.\n
        *   @p KErrArgument if the URI is incorrect.
        */
        IMPORT_C TInt UnregisterDatabase(
        /* IN  */       const TDesC& aDatabaseUri,
        /* IN  */       TChar aDriveLetter = 0
        );

        /**
        * Unregisters all landmark databases which are accessed through a
        * certain protocol.
        *
        * After this, the landmark databases will not be returned when listing
        * landmark databases.
        *
        * If the protocol is not known or if there are no databases for
        * the protocol, nothing happens.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since S60 3.0
        * @param aProtocol The protocol to unregister all databases for.
        * @return @p KErrNone if successful, otherwise a system-wide error
        *   code.
        */
        IMPORT_C TInt UnregisterAllDatabases(
        /* IN  */       const TDesC& aProtocol
        );

        /**
        * Modifies the settings for a landmark database.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since S60 3.0
        * @param aDatabaseUri The URI of the database to modify settings for.
        * @param aDatabaseSettings The new settings for the database.
        * @param aDriveLetter The drive where the database resides. If
        *   drive is not applicable, 0 should be specified.
        * @return @p KErrNone if successful.\n
        *   @p KErrArgument if the URI is incorrect.\n
        *   @p KErrNotFound if the URI does not exist in the registry.
        */
        IMPORT_C TInt ModifyDatabaseSettings(
        /* IN  */       const TDesC& aDatabaseUri,
        /* IN  */       const TPosLmDatabaseSettings& aDatabaseSettings,
        /* IN  */       TChar aDriveLetter = 0
        );

        /**
        * Retrieve information about a landmark database.
        *
        * This function requires @p ReadUserData capability.
        *
        * @since S60 3.0
        * @param aDatabaseInfo An information object containing the URI of the
        *   landmark database. On return, the object contains information about
        *   the landmark database, including any database settings.
        * @return @p KErrNone if successful.\n
        *   @p KErrArgument if the URI is incorrect.\n
        *   @p KErrNotFound if the URI does not exist in the registry.
        */
        IMPORT_C TInt GetDatabaseInfo(
        /* IN/OUT */    HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * Reports a database management event.
        *
        * The event is distributed to all listening clients.
        *
        * A database URI can be specified to indicate which database the
        * event is about.
        *
        * This function requires @p ReadUserData and @p WriteUserData
        * capabilities.
        *
        * @since S60 3.0
        * @param aEvent Event information
        * @param aDatabaseUri The URI of the database which the event is about.
        * @return @p KErrNone if successful, otherwise a system-wide error
        *   code.
        */
        IMPORT_C TInt ReportEvent(
        /* IN  */       const TPosLmDatabaseEvent& aEvent,
        /* IN  */       const TDesC& aDatabaseUri = KNullDesC
        );

    private:

        HBufC8* SendMessageLC(
        /* IN  */       TInt aFunction,
        /* IN  */       const TDesC& aDes,
        /* IN  */       TInt aLength,
        /* IN/OUT */    TInt& aAllocLength
        );

        void PopulateArrayL(
        /* OUT */       RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
        /* IN  */       HBufC8* aBuffer
        );

        void PopulateArrayL(
        /* OUT */       CDesCArray* aArray,
        /* IN  */       HBufC8* aBuffer
        );

        void FreeResources();

        static void FreeRsc(TAny* aParam);
        /**
        * Starts the server.
        *
        * @return KErrNone on success, otherwise any system wide error.
        */
        TInt StartServer();

    private:    // Data

        CPosLandmarkServerExtension* iExtension;
    };

#endif      // RPOSLANDMARKSERVER_H

// End of File
