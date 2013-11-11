/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLandmarkCategory class
*
*/


#ifndef CPOSLANDMARKCATEGORY_H
#define CPOSLANDMARKCATEGORY_H

#include <e32base.h>
#include "EPos_Landmarks.h"


/**
*  Landmark category.
*
*  A landmark database can contain a number of categories which can be
*  assigned to the landmarks in the database. A landmark can be associated
*  with multiple categories, e.g. a landmark can be a "Restaurant" and a "Pub".
*  Categories also enable filtered searches, e.g. a client could search for
*  nearby restaurants.
*
*  This class specifies the landmark category attributes, i.e. name, icon and
*  a global category identifier. Global categories enable a uniform
*  categorization across multiple databases.
*
*  A landmark category is created, updated or deleted by using functions in
*  @ref CPosLmCategoryManager.
*
*  @lib eposlandmarks.lib
*  @since S60 3.0
*/
class CPosLandmarkCategory : public CBase
    {
    public:

        /**
        *  Bitmap for specifying a group of category attributes. Bit values are
        *  defined by @ref _TAttributes.
        */
        typedef TUint32 TAttributes;

        /**
        *  Specifies a category attribute, such as category name or category icon.
        */
        enum _TAttributes
            {
            ENoAttribute        = 0x0000  /**<
                No attribute specified. */,
            ECategoryName       = 0x0001  /**<
                The name of the landmark category. */,
            EIcon               = 0x0010  /**<
                The icon that represents the category in a UI. */,
            EAllAttributes      = 0xFFFF  /**<
                All category attributes. */
            };

    public:

        /**
        * Two-phased constructor.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLandmarkCategory* NewLC();

        /**
        * Two-phased constructor.
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLandmarkCategory* NewL();

        /**
        * Two-phased copy constructor.
        *
        * @param[in] aCategory The category to copy.
        * @returns A copy of the specified category object.
        */
        IMPORT_C static CPosLandmarkCategory* NewLC(
               const CPosLandmarkCategory&  aCategory
        );

        /**
        * Two-phased copy constructor.
        *
        * @param[in] aCategory The category to copy.
        * @returns A copy of the specified category object.
        */
        IMPORT_C static CPosLandmarkCategory* NewL(
               const CPosLandmarkCategory&  aCategory
        );

        /**
        * Destructor.
        */
        virtual ~CPosLandmarkCategory();

    public:

        /**
        * Reads the ID of the category entry in the database.
        *
        * @returns The ID of the category entry in the database, or
        *   @p KPosLmNullItemId if the category has not been added to the
        *   database yet.
        */
        IMPORT_C TPosLmItemId CategoryId() const;

        /**
        * Retrieves global category ID.
        *
        * Global categories enable a uniform categorization across multiple
        * databases.
        *
        * @returns The global category ID or @p KPosLmNullGlobalCategory if the
        *   category is not global.
        */
        IMPORT_C TPosLmGlobalCategory GlobalCategory() const;

        /**
        * @internal */
        /*
        * Sets the category to be global (i.e. not only defined in one
        * database).
        *
        * Global categories enable a uniform categorization across multiple
        * databases.
        *
        * @param[in] aGlobalCategory The global category ID or
        *   @p KPosLmNullGlobalCategory if the category should not be global.
        */
        void SetGlobalCategory(
               TPosLmGlobalCategory  aGlobalCategory
        );

        /**
        * Reads the name of the landmark category.
        *
        * @param[out] aCategoryName Will contain the category name.
        * @returns @p KErrNone if successful, @p KErrNotFound if the category
        *   name is not set.
        */
        IMPORT_C TInt GetCategoryName(
               TPtrC&  aCategoryName
        ) const;

        /**
        * Sets the name of the landmark category.
        *
        * @param[in] aCategoryName The category name.
        *
        * @leave KErrArgument Client passed an empty descriptor or landmark name 
        *   is longer than @p KPosLmMaxCategoryNameLength.
        */
        IMPORT_C void SetCategoryNameL(
               const TDesC&  aCategoryName
        );

        /**
        * Associates the landmark category with an icon.
        *
        * Icons are found in icon files. To set an icon, the client
        * must specify the name of the icon file and the index of the
        * icon within the file.
        *
        * The landmark is not affected if the icon file is changed or
        * removed. It only contains a link to the icon.
        *
        * @param[in] aIconFileName The full icon file name.
        * @param[in] aIconIndex The index of the icon within the icon file.
        * @param[in] aIconMaskIndex The index of the icon mask within the
        *   icon file.
        *
        * @leave KErrArgument The icon file name is longer than @p KMaxFileName.
        *
        * @panic "Landmarks Client"-EPosLmInvalidArgument The icon index is negative or
        *   icon mask index is negative and not equal to @p KPosLmIconMaskNotUsed.
        */
        IMPORT_C void SetIconL(
               const TDesC&  aIconFileName,
               TInt  aIconIndex,
               TInt  aIconMaskIndex
        );

        /**
        * Returns the a link to the icon associated with the landmark category.
        *
        * Icons are found in icon files. It is referenced by the name of
        * the icon file and the index of the icon within the file.
        *
        * The landmark category is not affected if the icon file is
        * changed or removed. It only contains a link to the icon. This means
        * that the link could be invalid.
        *
        * @param[out] aIconFileName The full icon file name.
        * @param[out] aIconIndex The index of the icon within the icon file.
        * @param[out] aIconMaskIndex The index of the icon mask within the
        *   icon file. If no icon mask index is defined @p KPosLmIconMaskNotUsed
        *   is returned.
        *
        * @returns @p KErrNone if successful, @p KErrNotFound if the icon is
        *   not set.
        */
        IMPORT_C TInt GetIcon(
               TPtrC& aIconFileName,
               TInt& aIconIndex,
               TInt& aIconMaskIndex
        ) const;

        /**
        * Removes category attributes from the landmark category.
        *
        * @param[in] aAttributes A bitmap specifying which category attributes to
        *   remove.
        */
        IMPORT_C void RemoveCategoryAttributes(
               TAttributes aAttributes
        );

        /**
        * @internal */
        /*
        * Sets the category ID to the landmark category.
        *
        * @param[in] aId The category ID to set.
        */
        void SetCategoryIdL(
               TPosLmItemId aId
        );

    private:

        // C++ constructor
        CPosLandmarkCategory();

        void ConstructL();
        void ConstructL(const CPosLandmarkCategory& aCategory);

        // Prohibit copy constructor
        CPosLandmarkCategory(const CPosLandmarkCategory&);

        // Prohibit assigment operator
        CPosLandmarkCategory& operator= (const CPosLandmarkCategory&);

    private:

        // Category ID
        TPosLmItemId iId;

        // Global category
        TPosLmGlobalCategory iGlobalCategory;

        // Category label
        HBufC* iCategoryName;

        // Category icon filename
        HBufC* iIconFileName;

        // Category icon index
        TInt iIconIndex;

        // Category icon mask index
        TInt iIconMaskIndex;
    };

#endif      // CPOSLANDMARKCATEGORY_H


