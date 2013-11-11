/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/


#ifndef MCLFITEMLISTMODEL_H
#define MCLFITEMLISTMODEL_H

//  INCLUDES
#include <CLFContentListing.hrh>
#include <bamdesca.h>

// DATA TYPES
/**
*  Content Listing Framework list model refresh type.
*/
enum TCLFRefreshTypeFlags
    {
    /// Post filter is refreshed
    ECLFRefreshPostFilter   = 0x1,
    /// Grouping is refreshed
    ECLFRefreshGrouping     = 0x2,
    /// Sorting is refreshed
    ECLFRefreshSorting      = 0x4,
    /// All filters are used
    ECLFRefreshAll          = 0xFFFFFFFF
    };

// FORWARD DECLARATIONS
class MCLFItemListModelExt;
class MCLFItem;
class MCLFPostFilter;
class MCLFCustomGrouper;
class MCLFPostFilter;
class MCLFSortingStyle;
class MCLFCustomSorter;
class TResourceReader;

// CLASS DECLARATION

/**
*  List Model for Content Listing Framework.
*  This model is used to list Content Listing items. These items provide
*  infomation of media files on the device and one item represents one media
*  file. The model can be manipulated by sorters, post filters and groupers.
*  List Model is created by using Content Listing Engine.<br><br>
*  Usage:
*  @code
*  // Create a new List Model instance
*  MCLFItemListModel* listModel = iEngine->CreateListModelLC( *myObserver );
*
*  // Append music-type to wanted types array to exclude
*  // all other files than music files from the list model
*  RArray<TCLFMediaType> myMediaTypes;
*  CleanupClosePushL( myMediaTypes );
*  myMediaTypes.AppendL( ECLFMediaTypeMusic );
*
*  // Set wanted types with SetWantedMimeTypesL or/and SetWantedMediaTypesL.
*  // You can add both if you want.
*  listModel->SetWantedMediaTypesL( myMediaTypes.Array() );
*  CleanupStack::PopAndDestroy( &myMediaTypes );
*
*  // Set sorting style (if you want to sort items)
*  listModel->SetSortingStyle( mySortingStyle );
*
*  // You can also add post filter, custom sorter and custom grouper to model.
*
*  // Refresh the List Model.
*  // The model will fetch items from server and use post filter to filter items,
*  // grouping style or custom grouper to group items and finally sort items
*  // with sorting style or custom sorter.
*  listModel->RefreshL();
*
*  // You can also refresh the post filter and sorting only. This call does not
*  // fetch items from the server. The model is only filtered and sorted.
*  listModel->RefreshL( ECLFRefreshPostFilter | ECLFRefreshSorting );
*  @endcode
*
*  @lib ContentListingFramework.lib
*  @since S60 3.1
*/
class MCLFItemListModel
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~MCLFItemListModel() {}

    public: // New functions

        /**
        * Get Content Listing Framework item from the List Model.
        * @since S60 3.1
        * @pre aIndex >= 0 && aIndex < ItemCount()
        * @param aIndex Index of the item
        * @return Content Listing Framework item
        */
        virtual const MCLFItem& Item( TInt aIndex ) const = 0;

        /**
        * Get number of items that are in the model.
        * @since S60 3.1
        * @return Count of items
        */
        virtual TInt ItemCount() const = 0;

        /**
        * Activate or remove Sorting style.
        * Setting a new sorting style will remove all old secondary sorters.
        * Removing the sorting will also remove all secondary sorters too.
        * @since S60 3.1
        * @param aSortingStyle Sorting style to activate,
        *        if the pointer is NULL then sorting styles are removed
        */
        virtual void SetSortingStyle( MCLFSortingStyle* aSortingStyle ) = 0;

        /**
        * Append a secondary sorting style to the list model.
        * If an item doesn't have the field defined by primary or
        * any previous secondary sorting style, the items are sorted using
        * the next secondary sorting style. When a sorting style is
        * appended using this method, it becomes the least significant
        * sorting style until a new one is added after it.
        * Items with undefined fields are placed before or after items
        * with defined fields, depending on the undefined field position
        * setting in MCLFSortingStyle.
        * @since S60 3.1
        * @param aSortingStyle Secondary sorting style to add.
        */
        virtual void AppendSecondarySortingStyleL(
                                    MCLFSortingStyle& aSortingStyle ) = 0;

        /**
        * Activate or remove custom sorter.
        * Custom sorter will overwrite sorting style (if there is a sorting
        * style activated). See MCLFCustomSorter for an example implementation
        * of a custom sorter.
        * @since S60 3.1
        * @param aCustomSorter Custom sorter to activate.
        *        If the pointer is NULL, sorting style is used (if there is one
        *        activated)
        */
        virtual void SetCustomSorter( MCLFCustomSorter* aCustomSorter ) = 0;

        /**
        * Activate a grouping style for the List Model. Use ECLFNoGrouping as
        * the grouping style if you want to remove grouping.
        * Default grouping style is ECLFNoGrouping.
        * @since S60 3.1
        * @param aGrouping Grouping style to activate
        */
        virtual void SetGroupingStyle( TCLFGrouping aGrouping ) = 0;

        /**
        * Activate or remove custom grouper. See MCLFCustomGrouper for an example
        * implementation of a custom grouper.
        * Custom grouper will overwrite grouping style.
        * @since S60 3.1
        * @param aCustomGrouper Custom grouper to activate,
        *        if pointer is NULL then then grouping style is used
        */
        virtual void SetCustomGrouper( MCLFCustomGrouper* aCustomGrouper ) = 0;

        /**
        * Activate or remove post filter. Post filter removes items from the
        * List Model. See MCLFPostFilter for an example implementation of a
        * post filter.
        * @since S60 3.1
        * @param aPostFilter Post filter to activate,
        *        if pointer is NULL then active post filter will be removed.
        */
        virtual void SetPostFilter( MCLFPostFilter* aPostFilter ) = 0;

        /**
        * Wanted mime types of media files that will be requested to the model.
        * Overwrites old mime types if they were set.
        * @since S60 3.1
        * @param aMimeTypes List of wanted mime types. Mime types can contain
        *                   wildcard characters "*" and "?", where "*" matches
        *                   zero or more consecutive occurrences of any character
        *                   and "?" matches a single occurrence of any character
        */
        virtual void SetWantedMimeTypesL( const MDesCArray& aMimeTypes ) = 0;

        /**
        * Wanted mime types of media files that will be requested to the model.
        * Overwrites old mime types if they were set.
        * @since S60 3.1
        * @param aResource Resource reader to mime type list resource.
        *                  Use resource struct CLF_MIME_TYPE_ARRAY.
        */
        virtual void SetWantedMimeTypesL( TResourceReader& aResource ) = 0;

        /**
        * Wanted media types of media files that will be requested to the model.
        * Overwrites old media types if they were set.
        * @since S60 3.1
        * @param aMediaTypes List of wanted media types
        */
        virtual void SetWantedMediaTypesL(
                            const TArray<TInt>& aMediaTypes ) = 0;


        /**
        * Wanted media types of media files that will be requested to the model.
        * Overwrites old media types if they were set.
        * @since S60 3.1
        * @param aResource Resource reader to media type list resource.
        *                  Use resource struct CLF_MEDIA_TYPE_ARRAY
        */
        virtual void SetWantedMediaTypesL( TResourceReader& aResource ) = 0;

        /**
        * Refresh the model. This function is asynchronous (non-blocking) and
        * MCLFOperationObserver is called when the refresh operation is
        * completed. <br>
        * <br>
        * Operations in refresh are executed in the following order:<br>
        * 1. Model gets wanted items from server.
        *    Use SetWantedMediaTypesL and/or SetWantedMimeTypesL to define
        *    wanted items.<br>
        * 2. Model uses post filter to filter items.<br>
        * 3. Model groups items if grouping is selected.<br>
        * 4. Model sorting items.<br>
        * @since S60 3.1
        */
        virtual void RefreshL() = 0;

        /**
        * Refresh the model. This function is synchronous (blocking). Use
        * parameter(s) to define the type of refresh. See TCLFRefreshTypeFlags
        * for refresh flags.
        * @since S60 3.1
        * @param aRefreshType Flag(s) to use for refreshing.
        */
        virtual void RefreshL( TInt32 aRefreshType ) = 0;

        /**
        * Cancel asynchronous refresh operation.
        * @since S60 3.1
        */
        virtual void CancelRefresh() = 0;

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MCLFItemListModelExt* Extension() { return NULL; }

    };

#endif      // MCLFITEMLISTMODEL_H

// End of File
