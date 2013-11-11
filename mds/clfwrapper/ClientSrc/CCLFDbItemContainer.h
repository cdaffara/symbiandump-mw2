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


#ifndef CCLFDBITEMCONTAINER_H
#define CCLFDBITEMCONTAINER_H

//  INCLUDES
#include <e32base.h>
#include <CLFContentListing.h>

// FORWARD DECLARATIONS

class CMdEObject;
class CCLFContainerItem;

// CLASS DECLARATION

/**
*  Container item.
*
*  @lib ContentListingFramework.lib
*  @since S60 3.0
*/
NONSHARABLE_CLASS( CCLFContainerItem ) : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        */    
        static CCLFContainerItem* NewLC( const CMdEObject* aObject );

        /**
        * Destructor
        */
        virtual ~CCLFContainerItem();

    public:

        /**
        * Increase reference count of item
        * @since S60 3.0
        */
        inline void IncreaseReferenceCount();

        /**
        *
        * @since S60 3.0
        * @return ref. to DB item
        */
        inline const CMdEObject& DbItem() const;

    protected:

        /**
        * For find in order
        */
        static TInt Compare( const CCLFContainerItem& aFirst,
                             const CCLFContainerItem& aSecond );

        /**
        * For find in order
        */
        inline void SetItemId( TCLFItemId aItemId );

    private:

        /**
        * Constructors
        */    
        CCLFContainerItem( const CMdEObject* aObject );

        CCLFContainerItem();

    private: // data

        // friend classes
        friend class CCLFDbItemContainer;   // CSI: 36 #  36:Use of friends
        
        TInt iReferenceCount;
        const CMdEObject* iMdEObject; //own
        TBool iDeprecated;
        TCLFItemId iItemId;
    };

/**
*  DBItem container.
*
*  @lib ContentListingFramework.lib
*  @since S60 3.0
*/
NONSHARABLE_CLASS( CCLFDbItemContainer ) : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFDbItemContainer* NewL();

        /**
        * Destructor.
        */
        virtual ~CCLFDbItemContainer();

    public:

        /**
        * @since S60 3.0
        * @param aItemId
        * @return Container item by CLF item id, if item is not found in
        *         conter then NULL pointer is returned
        */
        CCLFContainerItem* ItemById( const TCLFItemId aItemId ) const;

        /**
        * Decrease referency count for item and deletes container item and DB item
        * @since S60 3.0
        * @param aItem
        */
        void ReleaseItem( CCLFContainerItem* aItem );

        /**
        * Add container item to container.
        * @since S60 3.0
        * @param aItem
        */
        inline void AddL( CCLFContainerItem* aItem );

        /**
        *
        * @since S60 3.0
        * @param aItemIdArray
        */
        void SetDeprecatedItems( const RArray<TCLFItemId>& aItemIdArray );

    protected:

        TInt FindItem( TCLFItemId aItemId, CCLFContainerItem*& aItem ) const;
        TInt NextItem( CCLFContainerItem*& aItem ) const;

    private:

        /**
        * C++ default constructor.
        */
        CCLFDbItemContainer();

    private:
    
        class TFindHelper
            {
            public:
                TFindHelper();
                void Reset();
                void Set( TCLFItemId aItemId, TInt aIndex );

            public:    
                TCLFItemId iItemId;
                TInt iIndex;
            };

    private:    // Data

        RPointerArray<CCLFContainerItem> iItemArray;

        TLinearOrder<CCLFContainerItem> iArraySorter;
        mutable CCLFContainerItem iFindItem;
        mutable TFindHelper iFindHelper;
    };

// inline functions
#include "CCLFDbItemContainer.inl"

#endif      // CCLFDBITEMCONTAINER_H

// End of File
