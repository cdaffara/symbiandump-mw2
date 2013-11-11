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
* Description: Class containing static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*/


#ifndef POSLMIMPLEXTENSION_H
#define POSLMIMPLEXTENSION_H

//  INCLUDES
#include <e32base.h>

#include "EPos_Landmarks.h"

// FORWARD DECLARATIONS
class CPosLandmarkDatabase;
class CPosLandmark;
class CPosLandmarkCategory;

// CLASS DECLARATION

/**
*  Class containing static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*  This class is not meant for Landmarks API clients.
*
*  @version $Revision: 1.4 $, $Date: 2005/02/15 13:36:27 $
*/
class PosLmImplExtension
    {
    public:  // New functions

        /*
        * Returns the database access implementation which is used.
        *
        * @param aDatabase The landmark database to fetch ID from.
        * @returns The @ref CPosLandmarkDatabase implementation ID.
        */
        IMPORT_C static TUid ImplementationId(
        /* IN/OUT  */   const CPosLandmarkDatabase&  aDatabase
        );

        /*
        * Sets the partial update flag to a specified landmark.
        * This flag is used to indicate if only partial information is included
        * in the landmark object.
        *
        * @param aLandmark The landmark to set flag for.
        * @param aPartial @p EFalse if the landmark contains all the landmark
        *   information, otherwise @p ETrue.
        */
        IMPORT_C static void SetPartialL(
        /* IN/OUT  */   CPosLandmark &aLandmark,
        /* IN  */       TBool aPartial
        );

        /*
        * Sets the landmark id to a specified landmark.
        *
        * @param aLandmark The landmark to set id for.
        * @param aId The landmark id to set.
        */
        IMPORT_C static void SetLandmarkIdL(
        /* IN/OUT  */   CPosLandmark &aLandmark,
        /* IN  */       TPosLmItemId aId
        );

        /*
        * Sets the category id to a specified landmark category.
        *
        * @param aCategory The category to set id for.
        * @param aId The category id to set.
        */
        IMPORT_C static void SetCategoryIdL(
        /* IN/OUT  */   CPosLandmarkCategory &aCategory,
        /* IN  */       TPosLmItemId aId
        );

        /**
        * Sets a specified landmark category to be global (i.e. not only
        * defined in one database).
        *
        * Global categories enable a uniform categorisation across multiple
        * databases.
        *
        * @param aCategory The category to set global category for.
        * @param aGlobalCategory The global category ID or
        *   @p KPosLmNullGlobalCategory if the category should not be global.
        */
        IMPORT_C static void SetGlobalCategory(
        /* IN/OUT  */   CPosLandmarkCategory& aCategory,
        /* IN  */       TPosLmGlobalCategory aGlobalCategory
        );
    };

#endif      // POSLMIMPLEXTENSION_H

// End of File
