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
* Description:  CPosLmDisplayItem class
*
*/


#ifndef CPOSLMDISPLAYITEM_H
#define CPOSLMDISPLAYITEM_H

#include <e32base.h>

class CPosLandmark;
class CPosLandmarkCategory;


/**
* Displayable item.
*
* A displayable item consists of a landmark or category and its database
* index. Database index is 0 if running single database search, and it is
* in the range [0, @ref CPosLmMultiDbSearch::NumOfDatabasesInSearch - 1] in
* case of multiple database search. Thus, this class is a link between a
* landmark or a category and its database.
*
* The class is usually not instantiated by client applications.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmDisplayItem : public CBase
    {

    public:

    /**
    * Display item type.
    */
    enum TDisplayItemType
        {
        ELandmarkItem = 0       /**< Landmark display item. This indicates
                                * that the item contains a landmark and the
                                * @ref Landmark function can be called to
                                * access it.
                                */,
        ECategoryItem           /**< Category display item. This indicates
                                * that the item contains a category and the
                                * @ref Category function can be called to
                                * access it.
                                */
        };

    public:

        /**
        * Two-phased constructor.
        *
        * @param[in] aLandmark A landmark.
        * @param[in] aDatabaseIndex A database index.
        *
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmDisplayItem* NewL(
            CPosLandmark* aLandmark,
            TUint aDatabaseIndex = 0
        );

        /**
        * Two-phased constructor.
        *
        * @param[in] aCategory A category.
        * @param[in] aDatabaseIndex A database index.
        *
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmDisplayItem* NewL(
            CPosLandmarkCategory* aCategory,
            TUint aDatabaseIndex = 0
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmDisplayItem();

    public:

        /**
        * Returns the type of the display item.
        *
        * @return The display item type.
        */
        IMPORT_C TDisplayItemType DisplayItemType() const;

        /**
        * Returns the index of the database, which contained item belongs to.
        *
        * The database index is associated with a database URI from the list
        * of databases specified in @ref CPosLmMultiDbSearch.
        *
        * If display data is used in @ref CPosLandmarkSearch, this
        * function always returns 0.
        *
        * @return The database index of this displayable item.
        */
        IMPORT_C TUint DatabaseIndex() const;

        /**
        * Returns the category contained in the displayable item.
        *
        * @return The category.
        *
        * @panic "Landmarks Client"-EPosInvalidItemType
        *   The item type is not a category displayable item. See @ref DisplayItemType.
        */
        IMPORT_C const CPosLandmarkCategory& Category() const;

        /**
        * Returns the landmark contained in the displayable item.
        *
        * @return The landmark.
        *
        * @panic "Landmarks Client"-EPosInvalidItemType
        *   The item type is not a landmark displayable item. See @ref DisplayItemType.
        */
        IMPORT_C const CPosLandmark& Landmark() const;

        /**
        * Returns the distance to a position specified in the
        * @ref CPosLmNearestCriteria. The distance data is only used when
        * searching with this criteria.
        *
        * @param[out] aDistance Distance to the position specified in a
        *   @ref CPosLmNearestCriteria.
        * @return KErrNone if distance is available. KErrNotFound otherwise.
        */
        IMPORT_C TInt GetDistance( TReal32& aDistance ) const;

        /** @internal */
        /* 
        * Sets the database index of this displayable item.
        *
        * @param aDatabaseIndex The database index.
        */
        void SetDatabaseIndex( TUint aDatabaseIndex );

        /** @internal */
        /*
        * Sets the distance to the position specified in the nearest criterion.
        *
        * @param aDistance Distance to the position.
        */
        void SetDistance( TReal32 aDistance );

    private:

        /**
        * C++ constructor.
        *
        * @param[in] aLandmark A landmark.
        * @param[in] aDatabaseIndex Database index.
        */
        CPosLmDisplayItem(
            CPosLandmark* aLandmark,
            TUint aDatabaseIndex
        );

        /**
        * C++ constructor.
        *
        * @param[in] aCategory A category.
        * @param[in] aDatabaseIndex Database index.
        */
        CPosLmDisplayItem(
            CPosLandmarkCategory* aCategory,
            TUint aDatabaseIndex
        );

    private:

        TDisplayItemType iDisplayItemType;

        CPosLandmark* iLandmark;

        CPosLandmarkCategory* iCategory;

        TUint iDatabaseIndex;

        TReal32 iDistance;

        TBool iHasDistance;

    };

#endif      // CPOSLMDISPLAYITEM_H


