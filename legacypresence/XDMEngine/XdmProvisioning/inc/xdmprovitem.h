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
* Description:    Provisioning item for parsing and saving XDM settings
*
*/




#ifndef __XDMPROVITEM_H__
#define __XDMPROVITEM_H__

// INCLUDES
#include <CWPAdapter.h>

// FORWARD DECLARATIONS
class CWPCharacteristic;
class CWPParameter;

_LIT( KXdmProvAppId,            "ap0003" );
_LIT( KXdmProvAppRef,           "APPREF" );
_LIT( KXdmProvToAppRef,         "TO-APPREF" );
_LIT( KXdmProvStartBracket,     "(" );
_LIT( KXdmProvCloseBracket,     ")" );

const TInt KXdmProvIdLength     = 10;
const TInt KXdmProvNameLength   = 40;
const TInt KXdmProvBufLength    = 512;

/**
* Xdm settings item for provisioning.
*
* @since S60 3.2
*/
class CXdmProvItem : public CBase, public MWPVisitor
    {
    public: 
        
        /**
        * Two-phased constructor.
        */
        static CXdmProvItem* NewL();

        /**
        * Destructor.
        */
        virtual ~CXdmProvItem();                


    private: // From MWPVisitor

        void VisitL( CWPCharacteristic& aCharacteristic );
        
        void VisitL( CWPParameter& aParameter );
        
        void VisitLinkL( CWPCharacteristic& aLink );
            
    public: 
            
        /**
        * Validates the provisioning item for mandatory fields
        * @return ETrue if correct item
        */
        TBool Validate() const;
        
        /**
        * Saves the provisioned settings
        * @return KErrNone if successfull
        */
        TInt SaveL();
        
        /**
        * Returns name of the provisioning item
        * @return Name or KNullDesC
        */
        const TDesC& Name() const;
        
        /**
        * Returns application id of the provisioning item
        * @return ApplicationId or KNullDesC
        */
        const TDesC& ApplicationId() const;
        
        /**
        * Returns id of the created settings
        * @return The id or KErrNotFound if not yet saved
        */
        TInt XdmSettingsId() const;
        
        /**
        * Returns value of the APPREF of the provisioning item
        * @return iAppRef or KNullDesC
        */
        const TDesC& AppReference() const;
        
        /**
        * Returns the application references from this item to other
        * applications
        * @return Array of references
        */
        const RArray<TPtrC>& ToAppReferences() const;
                  
                
        /**
        * Set SIP settings id
        * @param aStorageIdValue Settings id
        */
        void SetSipSettingsIdL( const TDesC8& aStorageIdValue );
        
        /**
        * Set name of the access point
        * @param aLink
        */
       void SetNapDefL ( CWPCharacteristic& aLink );      
                  
    private:
    

        /**
        * C++ default constructor.
        */
        CXdmProvItem();
        
        /**
        * Determines and saves named parameter
        * @param aName Name of the parameter
        * @param aValue Value of the parameter
        */
        void DetermineNamedParameter( const TDesC& aName, 
                                      const TDesC& aValue );
        
        /**
        * Finds the access point id from CommsDb
        * based on access poin name
        */
        void FindAccessPointIdL ();
                
        /**
        * Checks that the settings name is not in use.
        * Creates new name with index if needed.
        */
        void CheckExistingNamesL( );

    private:

        
        // The allocated id for the saved settings 
        TInt iXdmSettingsId;    
        
        // APPID
        TPtrC  iAppId;
        
        // APPNAME
        TPtrC   iAppName;
        
        // PROVIDER-ID
        TPtrC  iProviderId;
        
        // TO-APPREF
        HBufC*  iSipReference;
        
        // TO-NAPID
        TPtrC  iToNapId;
        
        // Access point id
        HBufC*  iNapId;
        
        // Name of the access point
        TPtrC  iNapName;
        
        // APPREF
        TPtrC  iAppRef;

        // URI
        TPtrC  iXcapRootUri;
        
        // AAUTHNAME
        TPtrC  iUserName;
        
        // AAUTHSECRET
        TPtrC iPassword;
    
        // AAUTHTYPE
        TPtrC iAuthType;
        
        // Flag indicating wheather settings are saved
        TBool iSettingsSaved;
        
        // Application references
        RArray<TPtrC> iToAppReferences;
        

    };

#endif // __XDMPROVITEM_H__   
            
// End of File




