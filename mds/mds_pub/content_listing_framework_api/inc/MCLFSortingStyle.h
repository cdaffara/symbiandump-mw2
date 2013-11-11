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


#ifndef MCLFSORTINGSTYLE_H
#define MCLFSORTINGSTYLE_H

//  INCLUDES
#include <CLFContentListing.hrh>
#include <CLFContentListing.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class MCLFSortingStyleExt;

// CLASS DECLARATION

/**
*  Sorting style for Content Listing Framework.
*  This class can be used to define primary and secondary sorting styles for
*  Content Listing Framework list model. Sorting styles are used to sort the
*  list model.<br><br>
*  Secondary sorting style is used to sort items that could not be sorted by
*  the primary sorting style. In other words, items that do not have the
*  field defined by primary or any previous secondary sorting style, the items
*  are sorted using the next secondary sorting style. When a secondary sorting
*  style is appended, it becomes the least significant sorting style until a
*  new one is added after it. Items with undefined fields are placed before or
*  after items with defined fields, depending on the undefined field position
*  setting in MCLFSortingStyle.
*  <br><br>
*  Usage:
*  @code
*  // Create a sorting style instance
*  MCLFSortingStyle* sortingStyle = ContentListingFactory::NewSortingStyleLC();
*
*  // Set sort ordering
*  sortingStyle->SetOrdering( ECLFOrderingDescending );
*
*  // Set undefied item position. Undefined items are items that doesn't
*  // have field that is defined in sorting style fields
*  sortingStyle->SetUndefinedItemPosition( ECLFSortingStyleUndefinedEnd );
*
*  // All sorting style parameter fields are string type
*  sortingStyle->SetSortingDataType( ECLFItemDataTypeDesC );
*
*  // Set default sorting parameter
*  sortingStyle->AddFieldL( ECLFFieldIdSongName );
*
*  // Set secondary sorting parameter
*  sortingStyle->AddFieldL( ECLFFieldIdFileName );
*
*  // If item doesn't have ECLFFieldIdSongName field then
*  // ECLFFieldIdFileName field is used to sort item.
*  @endcode
*
*  @lib ContentListingFramework.lib
*  @since S60 3.1
*/
class MCLFSortingStyle
    {
    public:  // Destructor

        /**
        * Destructor.
        */
        virtual ~MCLFSortingStyle() {}

    public: // New functions

        /**
        * Reset settings of the sorting style to default.
        * @since S60 3.1
        */
        virtual void ResetL() = 0;

        /**
        * Set sort ordering. Ordering can be ascending or descending. See
        * TCLFSortingStyleOrdering
        * @since S60 3.1
        * @param aOrdering Sort ordering to be set
        */
        virtual void SetOrdering( TCLFSortingStyleOrdering aOrdering ) = 0;

        /**
        * Get current sort ordering of the sorting style.
        * @since S60 3.1
        * @return Current sort ordering
        */
        virtual TCLFSortingStyleOrdering Ordering() const = 0;

        /**
        * Set data type of sorting parameter fields. Data type can be text,
        * time or number. See TCLFItemDataType
        * @since S60 3.1
        * @param aDataType Data type to be set
        */
        virtual void SetSortingDataType( TCLFItemDataType aDataType ) = 0;

        /**
        * Get current sorting data type of fields.
        * @since S60 3.1
        * @return Current data type sorting of sorting fields
        */
        virtual TCLFItemDataType SortingDataType() const = 0;

        /**
        * Set undefined item position. Items not having information for defined
        * sorting parameter fields are placed to top or bottom of the model. See
        * TCLFUndefinedItemPosition
        * @since S60 3.1
        * @param aUndefinedItemPosition Position for undefiened items
        */
        virtual void SetUndefinedItemPosition(
                    TCLFUndefinedItemPosition aUndefinedItemPosition ) = 0;

        /**
        * Get current position of undefined items.
        * @since S60 3.1
        * @return Current position for undefined items
        */
        virtual TCLFUndefinedItemPosition UndefinedItemPosition() const = 0;

        /**
        * Add new field to the sorting style. First added field will be the
        * default field. Remember to set type of field(s) with
        * SetSortingDataType. Fields that are some other type will be ignored.
        * See TCLFDefaultFieldId in CLFContentListing.hrh
        * @since S60 3.1
        * @param aFieldId Id of the sorting parameter field
        */
        virtual void AddFieldL( TCLFFieldId aFieldId ) = 0;

        /**
        * Get IDs of fields that have been set to the sorting style.
        * @since S60 3.1
        * @param aArray Array of field IDs
        */
        virtual void GetFieldsL( RArray<TCLFFieldId>& aArray ) const = 0;

    private: // Extension interface

        virtual MCLFSortingStyleExt* Extension() { return NULL; }

    };

#endif      // MCLFSORTINGSTYLE_H

// End of File
