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
* Description: 
*     See class description below
*
*/


#ifndef POSLMSQLQUERYHELPER_H
#define POSLMSQLQUERYHELPER_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  This is a helper class for creating sql queries.
*
*  @version $Revision: 1.2 $, $Date: 2005/02/22 13:43:30 $
*/
class PosLmSqlQueryHelper
    {
    public: // New functions

        /**
        * Appends a WHERE clause to a SQL query.
        * The SQL query will search for landmark whose latitude and longitude
        * coordinates resides between the specified limits.
        *
        * @param aDbQuery The sql query.
        * @param aSouthLatitude The southern latitude border of the search area.
        * @param aNorthLatitude The northern latitude border of the search area.
        * @param aWestLongitude The western longitude border of the search area.
        * @param aEastLongitude The eastern longitude border of the search area.
        */
        static void AppendAreaCriteriaToQueryL(
        /* IN/OUT  */   HBufC* aDbQuery,
        /* IN  */       TReal64 aSouthLatitude,
        /* IN  */       TReal64 aNorthLatitude,
        /* IN  */       TReal64 aWestLongitude,
        /* IN  */       TReal64 aEastLongitude
        );

    };

#endif  // POSLMSQLQUERYHELPER_H

// End of File
