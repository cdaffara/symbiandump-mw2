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
* Description:Class containing static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*/


#ifndef POSLMDBINFOEXTENSION_H
#define POSLMDBINFOEXTENSION_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class HPosLmDatabaseInfo;
class TPosLmDatabaseSettings;

// CLASS DECLARATION

/**
*  Class containing static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*  This class is not meant for Landmarks API clients.
*
*  @version $Revision: 1.3 $, $Date: 2005/03/09 09:30:52 $
*/
class PosLmDbInfoExtension
    {
    public:  // New functions

        /**
        * Sets the default database indicator flag.
        * This flag is used to indicate if the database described by the
        * object is the default database.
        *
        * @param aDatabaseInfo The object in which the flag should be set.
        * @param aIsDefault @p ETrue if the database is the default database,
        * otherwise @p ETrue.
        */
        IMPORT_C static void SetDefault(
        /* IN/OUT  */   HPosLmDatabaseInfo &aDatabaseInfo,
        /* IN  */       TBool aIsDefault
        );

        /**
        * Sets the database media type.
        *
        * @param aDatabaseInfo The object in which the parameter should be set.
        * @param aMediaType The media type.
        */
        IMPORT_C static void SetMediaType(
        /* IN/OUT  */   HPosLmDatabaseInfo &aDatabaseInfo,
        /* IN  */       TMediaType aMediaType
        );

        /**
        * Sets the database drive.
        *
        * @param aDatabaseInfo The object in which the parameter should be set.
        * @param aDatabaseDrive The database drive letter.
        */
        IMPORT_C static void SetDatabaseDrive(
        /* IN/OUT  */   HPosLmDatabaseInfo &aDatabaseInfo,
        /* IN  */       TChar aDatabaseDrive
        );


    };

#endif      // POSLMDBINFOEXTENSION_H

// End of File
