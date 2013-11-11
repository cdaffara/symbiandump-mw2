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



#ifndef CCLFDEFAULTOPERATION_H
#define CCLFDEFAULTOPERATION_H

//  INCLUDES
#include <MCLFCustomSorter.h>
#include <MCLFPostFilter.h>
#include <MCLFCustomGrouper.h>
#include <CLFContentListing.hrh>
#include <CLFContentListing.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MCLFSortingStyle;
class MCLFModifiableItem;

// CLASS DECLARATION

/**
*  This class implements operations from
*  MCLFCustomSorter, MCLFPostFilter and MCLFCustomGrouper.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CCLFDefaultOperation ) : public CBase,
                                            public MCLFCustomSorter,
                                            public MCLFPostFilter,
                                            public MCLFCustomGrouper
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFDefaultOperation* NewL();

        /**
        * Destructor.
        */
        virtual ~CCLFDefaultOperation();

    public: // New functions

        /**
        * Set sorting style.
        * @since Series 60 3.0
        * @param aSortingStyle Ownership is not transfered
        */
        void SetSortingStyle( MCLFSortingStyle* aSortingStyle );

        /**
        * Append sorting style.
        * @since Series 60 3.0
        * @param aSortingStyle Ownership is not transfered
        */
        void AppendSortingStyleL( MCLFSortingStyle& aSortingStyle );

        /**
        * Set item grouping.
        * @since Series 60 3.0
        * @param aGrouping Grouping style
        */
        void SetGrouping( TCLFGrouping aGrouping );

    protected:  // New functions

        void DoMusicAlbumGroupingL( const TArray<MCLFItem*>& aSourceList,
                                    RPointerArray<MCLFItem>& aGroupedList );

    protected:  // Functions from base classes

        /**
        * From MCLFCustomSorter
        */
        virtual void SortItemsL( RPointerArray<MCLFItem>& aItemArray );

        /**
        * From MCLFCustomGrouper
        */
        virtual void GroupItemsL( const TArray<MCLFItem*>& aSourceList,
                                  RPointerArray<MCLFItem>& aGroupedList );

        /**
        * From MCLFPostFilter
        */
        virtual void FilterItemsL( const TArray<MCLFItem*>& aItemList,
                                   RPointerArray<MCLFItem>& aFilteredItemList );

    private:

        /**
        * C++ default constructor.
        */
        CCLFDefaultOperation();

    private:    // Data
        // Ref.
        MCLFSortingStyle* iSortingStyle;
        // Ref.
        RPointerArray<MCLFSortingStyle> iSortingStyleArray;
        // Grouping style
        TCLFGrouping iGrouping;
        // Own. this class owns grouped items
        RPointerArray<MCLFModifiableItem> iGroupedItemList;

    };

#endif      // CCLFDEFAULTOPERATION_H

// End of File
