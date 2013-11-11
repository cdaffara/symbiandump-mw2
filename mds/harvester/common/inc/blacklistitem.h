/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#ifndef __BLACKLISTITEM_H__
#define __BLACKLISTITEM_H__

// SYSTEM INCLUDE
#include <e32base.h>


/**
* A helper class (DTO) for data stored to the blacklist database and cache.
*/
NONSHARABLE_CLASS( CBlacklistItem ) : public CBase
    {
    public:
        /**
         * Standard NewL().
         * @param aModified  Item's last modification time.
         * @param aUri  Item's URI.
         * @param aMediaId  Item's media id.
         * @return  New item
         */
        static CBlacklistItem* NewL( const TInt64& aModified,
                const TDesC& aUri, const TUint32 aMediaId );
        
        /** Destructor. */
        virtual ~CBlacklistItem();

        /**
         * Check if given item matches given data.
         * @param aUri  Item's URI.
         * @param aMediaId  Item's media id.
         * @return ETrue, if matches.
         */
        TBool Compare( const TDesC& aUri, const TUint32 aMediaId );
        
        /**
         * Return item's last modification time.
         * @return Item's last modification time.
         */
        const TInt64& Modified();
    
        /**
         * Return item's MediaId.
         * @return item's MediaId.
         */
        const TUint32& MediaId();
 
 
        /**
         * Return item's Uri.
         * @return item's Uri.
         */
        const HBufC* Uri();
        
        /**
         * Return item's size for serialization.
         * @return item's size for serialization.
         */
        TInt RequiredBufferSize();
 
    private:

        /**
         * 2nd phase construction.
         * @param aUri  Item's URI.
         */
        void ConstructL( const TDesC& aUri );
        
        /**
         * Constructor.
         * @param aModified  Item's last modification time.
         * @param aMediaId  Item's media id.
         */
        CBlacklistItem( const TInt64& aModified,
                const TUint32 aMediaId );

    private: // data
    
        /** @var Item modification time */
        TInt64 iModified;
        
        /** @var Item URI */
        HBufC* iUri;
        
        /** Media Id of the item's location */
        TUint32 iMediaId;
    };


#endif
