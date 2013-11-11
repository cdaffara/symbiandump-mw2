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
* Description: Provides static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*/


#ifndef POSLMSEARCHIMPLEXTENSION_H
#define POSLMSEARCHIMPLEXTENSION_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CPosLmDisplayData;
class CPosLmDisplayItem;
class CPosLmPartialReadParameters;

// CLASS DECLARATION

/*
*  Class containing static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*  This class is not meant for Landmarks Search API clients.
*
*  @version $Revision: 1.5 $, $Date: 2005/02/15 13:36:54 $
*/
class PosLmSearchImplExtension
    {
    public:  // New functions

        /*
        * Returns the landmark partial read parameters for a specified
        * display data.
        *
        * Ownership is not transferred, i.e. it is not allowed to delete the
        * returned partial read parameters.
        *
        * @param aDisplayData A display data.
        * @return Partial read parameters or NULL if not set.
        */
        IMPORT_C static CPosLmPartialReadParameters* PartialReadParameters(
        /* IN */        const CPosLmDisplayData& aDisplayData
        );

        /*
        * Sets the database index of this displayable item for a
        * displayable item.
        *
        * @param aDisplayItem A displayable item.
        * @param aDatabaseIndex The database index.
        */
        IMPORT_C static void SetDatabaseIndex(
        /* IN/OUT */    CPosLmDisplayItem& aDisplayItem,
        /* IN */        TUint aDatabaseIndex
        );

        /*
        * Sets the distance to the position specified in the nearest criteria
        * for a displayable item.
        *
        * @param aDisplayItem A displayable item.
        * @param aDistance Distance to the position.
        */
        IMPORT_C static void SetDistance(
        /* IN/OUT */    CPosLmDisplayItem& aDisplayItem,
        /* IN */        TReal32 aDistance
        );

    };

#endif      // POSLMSEARCHIMPLEXTENSION_H

// End of File
