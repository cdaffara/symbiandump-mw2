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
* Description:  CPosLmDisplayData class
*
*/


#ifndef CPOSLMDISPLAYDATA_H
#define CPOSLMDISPLAYDATA_H

#include <e32base.h>

const TInt KPosLmNoNewItems = -1;

class CPosLmDisplayItem;
class CPosLmPartialReadParameters;

/**
* Displayable data collection.
*
* A displayable data collection consists of displayable items
* (@ref CPosLmDisplayItem). A displayable data collection either
* consists of landmark items (if a landmark search has been started)
* or category items (if a category search has been started).
* Items of different types cannot be mixed in the collection.
*
* Displayable data is used in @ref CPosLandmarkSearch and
* @ref CPosLmMultiDbSearch to hold search results. The collection is
* populated with new results every time the next search step is executed.
* Displayable items contain full or partial 
* (see @ref CPosLandmarkDatabase::SetPartialReadParameters)
* landmark data or full category data, and can be used to display search
* results already during the search and also after it has completed.
*
* Note: The single class instance may only be used by one search
* instance at a time.
*
*  @lib eposlmsearchlib.lib
*  @since S60 3.0
*/
class CPosLmDisplayData : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        IMPORT_C static CPosLmDisplayData* NewL();

        /**
        * Destructor.
        */
        virtual ~CPosLmDisplayData();

    public:

        /**
        * Returns the index of the next match found during current asynchronous
        * search operation. If the search is executed synchronously, then all
        * indexes of the matches found during the search are returned one
        * by one.
        *
        * After each search step, the new item indexes are returned in
        * ascending order according to the specified sort preference in
        * the search.
        *
        * @return The item index or @p KPosLmNoNewItems when no new items
        *   are available.
        */
        IMPORT_C TInt NewItemIndex();

        /**
        * Returns the number of items in the collection.
        *
        * @return The number of items.
        */
        IMPORT_C TInt Count() const;

        /**
        * Resets the collection and deletes all contained items.
        */
        IMPORT_C void Reset();

        /**
        * Returns the displayable item specified by index.
        * The index must be strictly less than @ref Count and not less than 0.
        *
        * @param[in] aItemIndex A displayable item index.
        * @return A displayable item.
        *
        * @panic "Landmarks Client"-EPosInvalidIndex
        *   Item index is beyond valid range.
        */
        IMPORT_C CPosLmDisplayItem& DisplayItem( TInt aItemIndex ) const;

        /**
        * Sets the partial read parameters for this display data.
        *
        * Partial read parameters are used to define which landmark data should
        * be read during a landmark search. If no partial read parameters are
        * set, the whole landmark will be read.
        *
        * This function only affects the searches which are started after
        * it is called. The current search is not affected.
        *
        * If landmarks are sorted by name, the name will always be a part of
        * the landmark in the display data, even if it is not requested.
        *
        * Note: Partial read parameters are only used for landmark searches.
        *
        * @param[in] aPartialSettings The partial read parameters.
        */
        IMPORT_C void SetPartialReadParametersL(
            const CPosLmPartialReadParameters& aPartialSettings
        );

        /**
        * Unsets the partial read parameters for this display data.
        *
        * This means that from now on all landmarks added to this display data
        * instance will contain all information.
        *
        * To have any affect, this function must be called before a search is
        * started. If it is called during a search, it will only affect
        * the next search.
        *
        * Note: Partial read parameters will only have effect on landmarks
        * searches.
        */
        IMPORT_C void UnsetPartialReadParameters();

        /** @internal */
        /* Returns the partial read parameters set for this display data.
        *
        * @return Partial read parameters or NULL if not set.
        */
        CPosLmPartialReadParameters* PartialReadParameters() const;

        /** @internal */
        /* Returns a reference to all the displayable items.
        * This array owns its displayable items.
        *
        * @return The items array.
        */
        RPointerArray<CPosLmDisplayItem>& DisplayItems();

        /** @internal */
        /* Returns a reference to the new displayable items.
        * This array does not own its displayable items.
        * New displayable items are always a subset of all displayable items.
        *
        * @return The new item indexes array.
        */
        RPointerArray<CPosLmDisplayItem>& NewDisplayItems();

    private:

        // C++ constructor.
        CPosLmDisplayData();

    private:

        RPointerArray<CPosLmDisplayItem> iDisplayItems;
        RPointerArray<CPosLmDisplayItem> iNewDisplayItems;
        CPosLmPartialReadParameters* iPartialParameters;

    };

#endif      // CPOSLMDISPLAYDATA_H


