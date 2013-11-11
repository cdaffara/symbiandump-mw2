/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Header for main Adapter class
*
*/




#ifndef __PRESPROVADAPTER_H__
#define __PRESPROVADAPTER_H__

//  INCLUDES
#include <CWPAdapter.h>

// FORWARD DECLARATIONS
class CPresProvItem;
class CWPCharacteristic;

// CONSTANTS

_LIT8( KPresProvisioningAppId8, "ap0002" );
_LIT8( KPresProvisioningXdmId,  "ap0003" );
_LIT8( KPresProvisioningSipId,  "w9010" );

/**
* Main Adapter class. Derived from CWPAdapter as recommended by provisioning 
* framework
*
* @since S60 3.2
*/
class CPresProvAdapter : public CWPAdapter
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPresProvAdapter* NewL();

        /**
        * Destructor.
        */
        ~CPresProvAdapter();
        
    public: // from CWPAdapter
        
        TInt ItemCount() const;
        const TDesC16& SummaryTitle( TInt aIndex ) const;
        const TDesC16& SummaryText( TInt aIndex ) const;
        void SaveL( TInt aItem );
        TBool CanSetAsDefault( TInt aItem ) const;
        void SetAsDefaultL( TInt aItem );
        TInt DetailsL( TInt aItem, MWPPairVisitor& aVisitor );
        void VisitL( CWPCharacteristic& aCharacteristic);       
        void VisitL( CWPParameter& aParameter );        
        void VisitLinkL( CWPCharacteristic& aLink );
        void SettingsSavedL ( const TDesC8& aAppIdOfSavingItem,
                              const TDesC8& aAppRef, 
                              const TDesC8& aStorageIdValue);
        void SavingFinalizedL();
        
        void GetSavingInfoL( TInt aIndex,
                                      RPointerArray<HBufC8>& aSavingInfo );

    private:

        /**
        * C++ default constructor.
        */
        CPresProvAdapter();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Des to Int
		*
		* @since S60 3.2
        * @param aSrcDes Descriptor to convert
        * @return TInt
        */  
        TInt DesToInt( const TDesC8& aSrcDes ) const; 
        
        
    private: 

        // Default name for settings
        HBufC* iDefaultName;

        // Title for Presence settings
        HBufC* iTitle;
        
        // Presence items
        RPointerArray<CPresProvItem> iProvItems;

    };

#endif // __PRESPROVADAPTER_H__   
            
// End of File
