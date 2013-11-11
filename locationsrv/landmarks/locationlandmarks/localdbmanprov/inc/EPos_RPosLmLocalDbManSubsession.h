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
* Description: This class is a subsession handle with operations related to
*  database management event listening.
*
*
*/


#ifndef RPOSLMLOCALDBMANSUBSESSION_H
#define RPOSLMLOCALDBMANSUBSESSION_H

//  INCLUDES
#include <e32base.h>
#include <epos_rposlandmarkserver.h>


/**
*  This class is a subsession handle with operations related to
*  database management event listening.
*
*  @lib eposlmdbmanprov.lib
*  @since S60 3.0
*  @version $Revision: 1.4 $, $Date: 2005/07/07 13:40:15 $
*/
class RPosLmLocalDbManSubsession : public RSubSessionBase
    {
    public:

        /**
        * C++ default constructor.
        *
        */

        RPosLmLocalDbManSubsession();

        /**
        * Creates a local database management subsession with the landmarks
        * server.
        *
        * @param aServer Is a connected session with the landmarks server.
        * @return @p KErrNone if successful, otherwise one of the system-wide
        * error codes.
        */

        TInt Open(
        /* IN  */ RPosLandmarkServer& aServer
        );

        /**
        * Closes the subsession.
        *
        */
        void Close();

        /**
        * Creates a local landmarks database.
        *
        * @param aDatabaseInfo The database that should be created.
        * Database URI, database drive and settings attributes are used.
        * @return @p KErrNone if the successful,
        *   otherwise a system wide error code.
        */
        TInt CreateDatabase(
        /* IN  */ HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * Deletes a local landmarks database.
        *
        * @param aDatabaseInfo The database that should be deleted.
        * Database URI and database drive attributes are used.
        * @return @p KErrNone if the successful,
        *   otherwise a system wide error code.
        */
        TInt DeleteDatabase(
        /* IN  */ HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * Copies a local landmarks database.
        *
        * @param aSourceUri URI of the database that should be copied.
        * @param aTargetUri URI of the new database copy.
        * @return @p KErrNone if the successful,
        *   otherwise a system wide error code.
        */
        TInt CopyDatabase(
        /* IN  */   const TDesC& aSourceUri,
        /* IN  */   const TDesC& aTargetUri
        );

    private:

        // By default, prohibit copy constructor
        RPosLmLocalDbManSubsession(
            const RPosLmLocalDbManSubsession&);
        // Prohibit assigment operator
        RPosLmLocalDbManSubsession& operator= (
            const RPosLmLocalDbManSubsession&);

    private:    // Data

    };

#endif      // RPOSLMLOCALDBMANSUBSESSION_H

// End of File
