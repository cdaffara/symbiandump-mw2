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



#ifndef CCLFSORTINGSTYLEIMPL_H
#define CCLFSORTINGSTYLEIMPL_H

//  INCLUDES
#include <MCLFSortingStyle.h>
#include <e32base.h>

class TResourceReader;

// CLASS DECLARATION

/**
*  This class implements MCLFSortingStyle interface.
*
*  @lib ContentListingFramework.lib
*  @since S60 3.0
*/
NONSHARABLE_CLASS( CCLFSortingStyleImpl ) : public CBase,
                                            public MCLFSortingStyle
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFSortingStyleImpl* NewLC();

        /**
        * Two-phased constructor.
        */
        static CCLFSortingStyleImpl* NewLC( TResourceReader& aResource );

        /**
        * Destructor.
        */
        virtual ~CCLFSortingStyleImpl();

    protected:  // Functions from base classes

        /**
        * From MCLFSortingStyle
        */
        void ResetL();

        /**
        * From MCLFSortingStyle
        */
        void AddFieldL( TCLFFieldId aFieldId );

        /**
        * From MCLFSortingStyle
        */
        void GetFieldsL( RArray<TCLFFieldId>& aArray ) const;

        /**
        * From MCLFSortingStyle
        */
        void SetOrdering( TCLFSortingStyleOrdering aOrdering );

        /**
        * From MCLFSortingStyle
        */
        TCLFSortingStyleOrdering Ordering() const;

        /**
        * From MCLFSortingStyle
        */
        void SetSortingDataType( TCLFItemDataType aDataType );

        /**
        * From MCLFSortingStyle
        */
        TCLFItemDataType SortingDataType() const;

        /**
        * From MCLFSortingStyle
        */
        void SetUndefinedItemPosition(
                    TCLFUndefinedItemPosition aUndefinedItemPosition );

        /**
        * From MCLFSortingStyle
        */
        TCLFUndefinedItemPosition UndefinedItemPosition() const;


    private:

        /**
        * C++ default constructor.
        */
        CCLFSortingStyleImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( TResourceReader& aResource );

    private:    // Data
        // Own: field ID array
        RArray<TCLFFieldId> iFieldIDArray;
        // Own: data type
        TCLFItemDataType iDataType;
        // Own: sorting ordering
        TCLFSortingStyleOrdering iOrdering;
        // Own:
        TCLFUndefinedItemPosition iUndefinedItemPosition;

    };

#endif      // CCLFSORTINGSTYLEIMPL_H

// End of File
