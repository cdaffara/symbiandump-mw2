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


#ifndef MCLFITEM_H
#define MCLFITEM_H

//  INCLUDES
#include <CLFContentListing.hrh>
#include <CLFContentListing.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class MCLFItemExt;

// CLASS DECLARATION

/**
*  Content Listing Framework item.
*  All items that are fetched from server have got an Item ID number.
*  Item ID is defined in runtime so do not store Item IDs permanently.
*  One item can contain multiple fields. The field consists of a Field ID and
*  field data. Type of the field is defined with Field ID: see
*  TCLFDefaultFieldId in CLFContentListing.hrh.
*  For example, a music file could contain these two fields:
*  ECLFFieldIdSongName (data: title of the song) and ECLFFieldIdArtist
*  (data: name of the artist).<br><br>
*  Usage:
*  @code
*  // Get file names and sizes of all items
*  TInt count( listModel->ItemCount() );
*  for( TInt i = 0 ; i < count ; ++i )
*      {
*      const MCLFItem& myItem = listModel->Item( i );
*
*      // Get file name of an item (string data)
*      TPtrC fileName;
*      TInt error( myItem.GetField( ECLFFieldIdFileName, fileName ) );
*
*      ...
*
*      // Get file size of an item (integer data)
*      TInt32 size( 0 );
*      error = myItem.GetField( ECLFFieldIdFileSize, size );
*
*      ...
*
*      }
*  @endcode
*
*  @lib ContentListingFramwork.lib
*  @since S60 3.1
*/
class MCLFItem
    {
    public:  // Destructor

        /**
        * Destructor.
        */
        virtual ~MCLFItem() {}

    public: // New functions

        /**
        * Get Item ID of the item.
        * @since S60 3.1
        * @return Item ID of the item
        */
        virtual TCLFItemId ItemId() const = 0;

        /**
        * Get data type of the field. Data type can be text, time or integer.
        * @since S60 3.1
        * @param aFieldId ID of the field (TCLFDefaultFieldId)
        * @return Data type of the field.
        *         ECLFItemDataTypeNull, if the field does not exist in the item.
        */
        virtual TCLFItemDataType DataType( TCLFFieldId aFieldId ) const = 0;

        /**
        * Get field data that is a string.
        * @since S60 3.1
        * @param aFieldId ID of the field (TCLFDefaultFieldId)
        * @param aData Data of the field
        * @return System wide error code.
        *         KErrNone if field exist,
        *         KErrNotFound if field doesn't exist,
        *         KErrNotSupported if field type doesn't match
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TPtrC& aData ) const = 0;

        /**
        * Get field data that is an integer.
        * @since S60 3.1
        * @param aFieldId ID of the field (TCLFDefaultFieldId)
        * @param aData Data of the field
        * @return System wide error code.
        *         KErrNone if field exist,
        *         KErrNotFound if field doesn't exist,
        *         KErrNotSupported if field type doesn't match
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TInt32& aData ) const = 0;

        /**
        * Get field data that is a time data.
        * @since S60 3.1
        * @param aFieldId ID of the field (TCLFDefaultFieldId)
        * @param aData Data of the field
        * @return System wide error code.
        *         KErrNone if field exist,
        *         KErrNotFound if field doesn't exist,
        *         KErrNotSupported if field type doesn't match
        */
        virtual TInt GetField( TCLFFieldId aFieldId, TTime& aData ) const = 0;

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MCLFItemExt* Extension() { return NULL; }

    };

#endif      // MCLFITEM_H

// End of File
