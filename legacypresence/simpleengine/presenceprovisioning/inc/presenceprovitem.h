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
* Description:    Provisioning item for parsing and saving Presence settings
*
*/




#ifndef __PRESPROVITEM_H__
#define __PRESPROVITEM_H__

// INCLUDES
#include <e32base.h>
#include <CWPAdapter.h>

// FORWARD DECLARATIONS
class CWPCharacteristic;
class CWPParameter;
class TPresSettingsSet;

// Constants
_LIT( KPresProvisioningAppId,    "ap0002" );
_LIT( KPresProvisioningAppRef,    "PRS-APPREF");
_LIT( KPresProvOpenBrack, "(" );
_LIT( KPresProvCloseBrack, ")" );

_LIT(KPresIAppRef, "APPREF");
_LIT(KPresIToAppRef, "TO-APPREF");
_LIT(KPresProvClientObjDataLim, "CLIENT-OBJ-DATA-LIMIT");
_LIT(KPresProvContentSrvUri, "CONTENT-SERVER-URI");
_LIT(KPresProvSrcThPublish, "SOURCE-THROTTLE-PUBLISH");
_LIT(KPresProvMaxPresSubs, "MAX-NUMBER-OF-PRESENCE-SUBSCRIPTIONS");
_LIT(KPresProvMaxPresSubsInLists, "MAX-NUMBER-OF-SUBSCRIPTIONS-IN-PRESENCE-LIST");
_LIT(KPresProvServiceUriTemp, "SERVICE-URI-TEMPLATE");

/**
* Provisioning item for parsing and saving Presence settings. Derived from
* MWPVisitor as recommended by provisioning framework
*
* @since S60 3.2
*/
class CPresProvItem : public CBase, public MWPVisitor
    {
    public: 
        
        /**
        * Two-phased constructor.
        */
        static CPresProvItem* NewL();

        /**
        * Destructor.
        */
        virtual ~CPresProvItem();                


    public: // From MWPVisitor

        void VisitL( CWPCharacteristic& aCharacteristic );
        
        void VisitL( CWPParameter& aParameter );
        
        void VisitLinkL( CWPCharacteristic& aLink );
            
    public: 
            
        /**
        * Validates the provisioning item for mandatory fields
        *
        * @since S60 3.2
        * @return ETrue if correct item
        */
        TBool Validate() const;
        
        /**
        * Saves the provisioned settings
        *
        * @since S60 3.2
        * @return KErrNone if successfull
        */
        TInt SaveL();
        
        /**
        * Returns name of the provisioning item
        *
        * @since S60 3.2
        * @return Name or KNullDesC
        */
        const TDesC& Name() const;
        
        /**
        * Returns application id of the provisioning item
        *
        * @since S60 3.2
        * @return ApplicationId or KNullDesC
        */
        const TDesC& ApplicationId() const;
        
        /**
        * Returns id of the created settings
        *
        * @since S60 3.2
        * @return The id or KErrNotFound if not yet saved
        */
        TInt PresSetId() const;
        
        /**
        * Returns value of the APPREF of the provisioning item
        *
        * @since S60 3.2
        * @return iAppRef or KNullDesC
        */
        const TDesC& AppReference() const;
        
        /**
        * Returns the application references from this item to other
        * applications
        *
        * @since S60 3.2
        * @return Array of references
        */
        const RArray<TPtrC>& ToAppReferences() const;
                  
        /**
        * Sets SIP settings id
        *
        * @since S60 3.2
        * @param aSipId Settings id
        */
        void SetSipSetIdL(TInt aSipId);
        
        /**
        * Sets XDM settings id
        *
        * @since S60 3.2
        * @param aXdmId Settings id
        */
        void SetXDMSetIdL(TInt aXdmId);
        
                
    private:
    
        /**
        * C++ default constructor.
        */
        CPresProvItem();
        
        /**
        * Determine and save named parameter
        *
        * @since S60 3.2
        * @param aName Name of the parameter
        * @param aValue Value of the parameter
        */
        void DetermineNamedParameter( const TDesC& aName, const TDesC& aValue );
        
        /**
        * Check presence settings duplicate names, provide alternate name
        * if necessary.
        *
        * @since S60 3.2
        * @param aSetId presence id, accepts KErrNotFound for new sets
        * @param aSetName presence set name, suggested name returns here
        * @return none
        */  
        void CheckDuplicateNameL(TDes& aSetName, const TInt& aSetId);
        
        /**
        * Des to Int
        *
        * @since S60 3.2
        * @param aSourceDes Descriptor to convert
        * @return TInt
        */  
        TInt DesToInt(const TDesC& aSourceDes) const;
        
        /** 
        * Checks if there is ordinal numbering at the end of
        * descriptor and rips it off if there is.
        *
        * @param aName pointer to source descriptor
        * @return HBufC* containing the name ready for the numbering
        */
        HBufC* GetPrefixL(HBufC* aName);
        

    private:

        
        // The allocated id for the saved settings 
        TInt iPresSetId;
        
        // Just used for validation, not to save
        TPtrC iAppId;
        
        // Set Items to be saved
        TPtrC iSetName;        
        TInt iSipProfile;
        TInt iXDMSetting;
        TInt iObjectSize;
        TPtrC iContSrvrAdd;
        TInt iPublicationInt;
        TInt iMaxSubscriptions;
        TInt iMaxContactsInList;
        TPtrC iDomainSyntax;
        TPtrC iProviderID;
        
           
        // TO-APPREF
        TPtrC  iAppRef;
        
        // TO-XDM
        TPtrC  iToXdmSet;
        
        // TO-SIP
        TPtrC  iToSipProfile;
        
        // Flag indicating wheather settings are saved
        TBool iSettingsSaved;
        
        // Application references
        RArray<TPtrC> iToAppReferences;
        
    };

#endif // __PRESPROVITEM_H__   
            
// End of File




