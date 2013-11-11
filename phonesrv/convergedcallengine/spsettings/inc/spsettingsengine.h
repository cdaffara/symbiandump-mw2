/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings API's engine class
*
*/


#ifndef C_SPSETTINGSENGINE_H
#define C_SPSETTINGSENGINE_H

#include <e32base.h>
#include <badesca.h>
#include "spdefinitions.h"
#include "cenrepdatabaseproperty.h"

class CRepository;
class CSPEntry;
class CSPProperty;
class CCenRepDatabaseUtil;

/**
 *  Engine class for Service Provider Settings API.
 *
 *  Do actual storage operations for service settings.
 *
 *  @lib serviceprovidersettings.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CSPSettingsEngine ): public CBase
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSPSettingsEngine* NewL();

        /**
        * Two-phased constructor.
        */
        static CSPSettingsEngine* NewLC();

        /**
        * Destructors.
        */
        virtual ~CSPSettingsEngine();

    public: // memeber functions
        /**
         * Starts transaction in repository
         *
         * @since S60 v3.2
         */
        TInt BeginTransactionLC();

        /**
         * Commits changes in repository
         *
         * @since S60 v3.2
         */
        TInt CommitTransaction();
        
        /**
         * Rollback changes in repository
         *
         * @since S60 v3.2
         */
        void RollbackTransaction();

        /**
         * Stores new service provider settings entry
         *
         * @since S60 v3.2
         * @param aEntry New service provider settings entry, on return service Id is filled in
         */
        void AddEntryL( CSPEntry& aEntry );

        /**
         * Search service provider settings entry by service ID. 
         *
         * @since S60 v3.2
         * @param aServiceId Id of requested xSP settings entry
         * @param aEntry on return, SP settings entry corresponding to aServiceId.
         * @return error code, KErrNone if succeeded, otherwise system wide error code
         */
        TInt FindEntryL( TServiceId aServiceId, CSPEntry& aEntry );

        /**
         * Updates service provider settings entry to settings storage.
         *
         * @since S60 v3.2
         * @param aEntry Updated service provider settings entry
         * @return error code, KErrNone if update succeeded, otherwise system wide error code
         */
        TInt UpdateEntryL( const CSPEntry& aEntry );

        /**
         * Deletes service provider settings entry from storage by service ID.
         *
         * @since S60 v3.2
         * @param aEntryId Id of settings entry which is going to be deleted
         * @return error code, KErrNone if delete succeeded, otherwise system wide error code
         */
        TInt DeleteEntryL( TServiceId aServiceId );

        /**
         * Search property of service provider settings entry from storage.
         *
         * @since S60 v3.2
         * @param aServiceId The service ID of requested service entry
         * @param aPropertyName property name to be found
         * @param aProperty on return, requested property
         * @return error code, KErrNone if succeeded, otherwise system wide error code
         */
        TInt FindPropertyL( TServiceId aServiceId, 
                            TServicePropertyName aPropertyName,
                            CSPProperty& aProperty );

        /**
         * Add or update properties of service provider settings entry to storage.
         * If property name exists, update property. If property doesn't exist,
         *
         * @since S60 v3.2
         * @param aServiceId The ID of updated service entry
         * @param aPropertyArray new properties to be added or updated
         * @return error code, KErrNone if update succeeded, otherwise system wide error code
         */
        TInt AddOrUpdatePropertiesL( TServiceId aServiceId, const RPropertyArray& aPropertyArray );

        /**
         * Return count of service provider settings entries in storage.
         *
         * @since S60 v3.2
         * @return Count of stored service provider settings.
         */
        TInt SettingsCountL();

        /**
         * Search all service provider IDs from storage
         *
         * @since S60 v3.2
         * @param aServiceIds An array containing returned service IDs
         * @return error code
         */
        TInt FindServiceIdsL( RIdArray& aServiceIds );

        /**
         * Search all service names from settings storage by given service IDs.
         * Removes service ID from given array if corresponding service name
         * is not found. 
         *
         * @since S60 v3.2
         * @param aServiceIds An array of service IDs
         * @param aServiceNames on return, array which saves found service names 
         */
        void FindServiceNamesL( RIdArray& aServiceIds, CDesCArray& aServiceNames );


       /**
         * Search subservice properties of requested service provider settings entry by ID.
         *
         * @since S60 v3.2
         * @param aServiceId The ID of requested service entry
         * @param aPropertyType property type of sub service
         * @param aPropertyArray on return,  array which saves found subservice properties
         * @return error code
         */
        TInt FindSubServicePropertiesL( TServiceId aServiceId,
                                        TSPItemType aPropertyType,
                                       RPropertyArray& aPropertyArray );

        /**
         * Deletes service properties by service ID and property name.
         *
         * @since S60 v3.2
         * @param aServiceId The service ID of requested service entry
         * @param aNameArray name of properties to be deleted
         * @return error code
         */
        TInt DeleteServicePropertiesL( TServiceId aServiceId,
                                       const RPropertyNameArray& aNameArray );
                                       
        /**
         * Find service IDs with same properties
         *
         * @since S60 v3.2
         * @param aPropertyArray property array
         * @param on return, service Ids with same properties
         * @return error code
         */
        void FindServiceIdsFromPropertiesL( const RPropertyArray& aPropertyArray, RIdArray& aServiceIds );
                                       
        /**
         * Check feature is supported or not
         *
         * @since S60 v3.2
         * @return SIP VoIP software version
         */
        TBool IsFeatureSupportedL( TSPServiceFeature aFeature, 
                                   TBool aWriteAllowed );
        
        
        /**
         * When service table is changed, update the P&S key
         *
         * @since S60 v3.2
         */
        void UpdateSupportFeaturePSKeyL();

        /**
         * Check if it is VoIP Service ID
         *
         * @since S60 v3.2.3
         * @return ETrue if yes
         */
        TBool IsVoIPServiceL( TServiceId aServiceId );

        /**     
         * Get Name array from Property array
         *
         * @since S60 v3.2.3
         */
        void NameArrayFromPropertyArrayL( RPropertyNameArray& aNameArray, const RPropertyArray& aPropertyArray );

        /**
         * Check if an array has call related property name in it
         *
         * @since S60 v3.2.3
         * @return ETrue if yes
         */
        TBool HasCallPropertyName( const RPropertyNameArray& aNameArray );
        
        /**
         * Check if an array has VoIP or Vmbx related property name in it
         *
         * @since S60 v3.2.3
         * @return ETrue if yes
         */
        TBool HasVoIPAndVmbxPropertyName(  const RPropertyNameArray& aNameArray );
        
    private:

        /**
        * C++ default constructor.
        */
        CSPSettingsEngine();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


        /**
         * Walk through all the sevices and check there is any service match 
         *
         * @since S60 v3.2
         * @param aMask attribute mask 
         * @param aValue value to be checked
         */
        TBool CheckFeatureL( TInt aMask, TInt aValue );
        
        
        void ConvertSpEntryToCenRepArrayL( const CSPEntry& aEntry, 
                                           RIpAppPropArray& aArray );
        
        void ConvertSpArrayToCenRepArrayL( const RPropertyArray& aSpArray,
                                           RIpAppPropArray& aCenRepArray );
                                           
        void PropertyNameArrayFromItemTypeL( TSPItemType aPropertyType, 
                                             RPropertyNameArray& aNameArray );
                                           
        static void CleanupPointerArray( TAny* aPointer );
        
        
    private: // data

        CCenRepDatabaseUtil* iCenRepUtils;
    };

#endif // C_SPSETTINGS_H
