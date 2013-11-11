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
* Description:    XDM OMA Provisioning Adapter class
*
*/




#ifndef __XDMPROVADAPTER_H__
#define __XDMPROVADAPTER_H__

//  INCLUDES
#include <CWPAdapter.h>

// FORWARD DECLARATIONS
class CXdmProvItem;
class CWPCharacteristic;

// CONSTANTS
#ifdef _DEBUG
_LIT( KXdmProvAdapterLogDir,                          "Xdm" );
_LIT( KXdmProvAdapterLogFile,                         "XdmProvAdapter.txt" );
const TInt KXdmProvAdapterBufferMaxSize               = 2000;
#endif

const TInt KXdmProvIntMaxLength = 10; // max length of 32bit integer

_LIT8( KXdmProvAppId8,          "ap0003" );
_LIT8( KXdmProvSipIdentifier,   "w9010" );

// CLASS DECLARATION

class CXdmProvAdapter : public CWPAdapter
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CXdmProvAdapter* NewL();

        /**
        * Destructor.
        */
        ~CXdmProvAdapter();
        
#ifdef _DEBUG        
        /**
        * Writes logs to file.
        */
        static void WriteToLog( TRefByValue<const TDesC8> aFmt,... );
#endif
        
    private: // from CWPAdapter
        
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
        CXdmProvAdapter();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        
    private: 

        // Default name for settings
        HBufC* iDefaultName;

        // Title for XDM settings
        HBufC* iTitle;
        
        // XdmItems
        RPointerArray<CXdmProvItem> iProvItems;
       
        
    };

#endif // __XDMPROVADAPTER_H__   
            
// End of File
