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
* Description:  Service Provider Settings API
*
*/


#ifndef C_SPSETTINGS_H
#define C_SPSETTINGS_H

#include <e32base.h>
#include <badesca.h>

#include "spdefinitions.h"

class CSPSettingsEngine;
class CSPEntry;
class CSPProperty;

/**
 *  Service provider settings API class
 *
 *  Main class of managing service provider settings
 *
 *  @code example code of how to add a new service entry to service table
 *  CSPEntry* entry = CSPEntry::NewL();
 *  TInt err = entry->SetServiceName( _L("somename" ) );
 *  CSPProperty* property = CSPProperty::NewL();
 *  err = property->SetName( EPropertyServiceAttributeMask );
 *  property->SetValue( 100 );
 *  err = entry->AddProperty( property );
 *  // iSettings is instance of CSPSettings
 *  TInt err = iSettings->AddEntryL( *entry );
 *  @endcode 
 *
 *  @lib serviceprovidersettings.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CSPSettings ): public CBase
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPSettings* NewL();

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPSettings* NewLC();

        /**
        * Destructors.
        */
        IMPORT_C virtual ~CSPSettings();

    public:

        /**
         * Stores new service provider settings entry, 
         * service Id is returned inside aEntry object
         *
         * @since S60 v3.2
         * @param aEntry New service provider settings entry, on return service Id is filled in
         * @return error code
         */
        IMPORT_C TInt AddEntryL( CSPEntry& aEntry );

        /**
         * Search service provider settings entry by service ID. 
         *
         * @since S60 v3.2
         * @param aServiceId Id of requested xSP settings entry
         * @param aEntry on return, SP settings entry corresponding to aServiceId.
         * @return error code, KErrNone if succeeded, otherwise system wide error code
         */
        IMPORT_C TInt FindEntryL( TServiceId aServiceId, CSPEntry& aEntry );


        /**
         * Updates service provider settings entry to settings storage.
         * Service ID must be set to entry before update.
         *
         * @since S60 v3.2
         * @param aEntry Updated service provider settings entry
         * @return error code, KErrNone if update succeeded, otherwise system wide error code
         */
        IMPORT_C TInt UpdateEntryL( const CSPEntry& aEntry );

        /**
         * Deletes service provider settings entry from storage by service ID.
         *
         * @since S60 v3.2
         * @param aEntryId Id of settings entry which is going to be deleted
         * @return error code, KErrNone if delete succeeded, otherwise system wide error code
         */
        IMPORT_C TInt DeleteEntryL( TServiceId aServiceId );

        /**
         * Search property of service provider settings entry from storage.
         *
         * @since S60 v3.2
         * @param aServiceId The service ID of requested service entry
         * @param aPropertyName property name to be found
         * @param aProperty on return, requested property
         * @return error code, KErrNone if succeeded, otherwise system wide error code
         */
        IMPORT_C TInt FindPropertyL( TServiceId aServiceId,
                                    TServicePropertyName aPropertyName,
                                    CSPProperty& aProperty );

        /**
         * Add or update properties of service provider settings entry to storage.
         * If property name exists, update property. If property doesn't exist,
         * add property
         *
         * @since S60 v3.2
         * @param aServiceId The ID of updated service entry
         * @param aPropertyArray new properties to be added or updated
         * @return error code, KErrNone if update succeeded, otherwise system wide error code
         */
        IMPORT_C TInt AddOrUpdatePropertiesL( TServiceId aServiceId, 
                                              const RPropertyArray& aPropertyArray );


        /**
         * Add or update one property of service provider settings entry to storage.
         * If property name exists, update property. If property doesn't exist,
         * add property
         *
         * @since S60 v3.2
         * @param aServiceId The ID of updated service entry
         * @param aProperty new property to be added or updated
         * @return error code, KErrNone if update succeeded, otherwise system wide error code
         */
        IMPORT_C TInt AddOrUpdatePropertyL( TServiceId aServiceId, 
                                            const CSPProperty& aProperty );

        /**
         * Return count of service provider settings entries in storage.
         *
         * @since S60 v3.2
         * @return Count of stored service provider settings.
         */
        IMPORT_C TInt SettingsCountL();

        /**
         * Search all service provider IDs from storage
         *
         * @since S60 v3.2
         * @param aServiceIds An array containing returned service IDs
         * @return error code
         */
        IMPORT_C TInt FindServiceIdsL( RIdArray& aServiceIds );


        /**
         * Search all service names from settings storage by given service IDs.
         * Removes ID from array if it is not found from settings storage.
         *
         * @since S60 v3.2
         * @param aServiceIds An array of service IDs
         * @param aServiceNames on return, array which saves found service names 
         * @return error code
         */
        IMPORT_C TInt FindServiceNamesL( RIdArray& aServiceIds,
                                        CDesCArray& aServiceNames );


       /**
         * Search subservice properties of requested service provider settings entry by ID 
         * and subservice type
         *
         * @since S60 v3.2
         * @param aServiceId The ID of requested service entry
         * @param aPropertyType property type of sub service
         * @param aPropertyArray on return,  array which saves found subservice properties
         * @return error code
         */
        IMPORT_C TInt FindSubServicePropertiesL( TServiceId aServiceId,
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
        IMPORT_C TInt DeleteServicePropertiesL( TServiceId aServiceId,
                                                const RPropertyNameArray& aNameArray );
                                                
        /**
         * Find service IDs with same properties
         *
         * @since S60 v3.2
         * @param aPropertyArray property array
         * @param on return, service Ids with same properties
         * @return error code
         */
        IMPORT_C TInt FindServiceIdsFromPropertiesL( const RPropertyArray& aPropertyArray, 
                                                     RIdArray& aServiceIds );

        /**
         * Get SIP VoIP software version.
         *
         * @since S60 v3.2
         * @return SIP VoIP software version
         */
        IMPORT_C static const TDesC& GetSIPVoIPSWVersion();
        
        /**
         * Check feature is supported or not
         *
         * @since S60 v3.2
         * @return SIP VoIP software version
         */
        IMPORT_C TBool IsFeatureSupported( TSPServiceFeature aFeature );

    private:

        /**
        * C++ default constructor.
        */
        CSPSettings();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Commit or rollback transaction
        * @since S60 v3.2
        * @param aError error code of CenRep operation
        * @param aServiceId changed service Id
        * @return error code
        */
        TInt CommitOrRollbackL( TInt aError, TServiceId aServiceId );

        /**
         * Check is VoIP Service Id and then are Properties VoIP related
         *
         * @since S60 v3.2.3
         * @return KErrNotSupported if VoIP not supported and checking 
         * of Service Id and Properties gives true
         */
        TInt CheckSupportedL( TServiceId aServiceId, const RPropertyNameArray& aNameArray );
        
        /**
         * Check is VoIP Service Id
         *
         * @since S60 v3.2.3
         * @return KErrNotSupported if VoIP not supported and checking 
         * of Service Id gives true
         */
        TInt CheckSupportedL( TServiceId aServiceId );

        /**
         * Check is SP item type VoIP related
         *
         * @since S60 v3.2.3
         * @return KErrNotSupported if VoIP not supported and checking 
         * of SP item type gives true
         */
        TInt CheckSupportedL( TSPItemType aPropertyType );
        
        /**
         * Check does an array have Properties VoIP related
         *
         * @since S60 v3.2.3
         * @return KErrNotSupported if VoIP not supported and checking 
         * of the array gives true
         */
        TInt CheckSupportedL( const RPropertyArray& aPropertyArray );

    private: // data

        /**
         * Settings engine
         * Own.
         */
        CSPSettingsEngine* iSettingsEngine;


        /**
         * Feature manager support VoIP
         * Own.
         */
        TBool iFeatureManagerSupportVoIP;
        /**
         * Writing settings allowed
         */
        TBool iSettingsWriteAllowed;

    };

#endif // C_SPSETTINGS_H
