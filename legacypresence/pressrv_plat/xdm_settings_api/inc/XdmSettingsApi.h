/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   XDM Settings API
*
*/




#ifndef __XDMSETTINGSAPI__
#define __XDMSETTINGSAPI__

#include <badesca.h>
#include "XdmEngineDomainCRKeys.h"

/**
* The list of configurable XDM Settings
**/
enum TXdmSettingsProperty
    {
    EXdmPropSettingsId      = KXdmSettingsId,
    EXdmPropAppId           = KXDMAppId,
    EXdmPropName            = KXDMAppName,
    EXdmPropProvId          = KXDMProviderId,
    EXdmPropToNapId         = KXDMNAPID,
    EXdmPropToAppRef        = KXDMSipReference,
    EXdmPropUri             = KXDMXcapRootURI,
    EXdmPropAuthName        = KXDMUserName,
    EXdmPropAuthSecret      = KXDMPassword,
    EXdmPropAuthType        = KXDMAuthType,
    EXdmPropNotFound        = KErrNotFound  // for initialization
    };

//The authentication types = possible values for EXdmPropAuthType
_LIT( KXdmAuthGaa,                             "GAA" );
_LIT( KXdmAuthEarlyIms,                        "EARLY-IMS" );
_LIT( KXdmAuthHttpDigest,                      "HTTP-DIGEST" );

//Logging constants
_LIT( KXdmSettingsLogDir,                       "XDM" );
_LIT( KXdmSettingsLogFile,                      "SettingsApi.txt" );
const TInt KXdmSettingsLogBufferMaxSize		    = 2000;

//Forward declarations
class CRepository;
class CXdmSettingsCollection;
    
//CLASS DECLARATION
class TXdmSettingsApi
    {
    public:
        
        /**
        * Return the name of each settings collection
        * stored in the repository
        *
        * @return CDesCArray* Names of all settings collections
        */
        IMPORT_C static CDesCArray* CollectionNamesLC( RArray<TInt>& aSettingIds );
        
        /**
        * Set the XDM properties as a collection of settings
        *
        * @param CXdmSettingsCollection A collection of properties
        * @return TInt The ID of the newly created collection
        */
        IMPORT_C static TInt CreateCollectionL( const CXdmSettingsCollection& aCollection );
        
        /**
        * Remove a collection from the Central Repository
        *
        * NOTE:
        *
        * - Leaves with KErrNotFound if a collection of properties
        *   by the specified ID does not exist in the repository.
        *
        * @param TInt ID of the collection to be removed
        * @return void
        */
        IMPORT_C static void RemoveCollectionL( TInt aSettingsId );
        
        /**
        * Create an individual property to the specified
        * collection of properties.
        *
        * NOTE:
        *
        * - Leaves with KErrAlreadyExists, if the property to be
        *   created already exists in the specified collection.
        *
        * - Leaves with KErrNotFound, if a property set by the
        *   specified ID cannot be found from the repository
        *
        * @param TInt ID of the target property set
        * @param TDesC& Value for the property to be created
        * @param TXdmSettingsProperty Name of the property
        * @return void
        */
        IMPORT_C static void CreatePropertyL( TInt aSettingsId,
                                              const TDesC& aPropertyValue,
                                              TXdmSettingsProperty aPropertyName );
                                              
        /**
        * Remove an individual property from the specified
        * collection of properties.
        *
        * NOTE:
        *
        * - Leaves with KErrNotFound, if a property set by the
        *   specified ID cannot be found from the repository
        *
        * @param TInt Name of the target property set
        * @param TXdmSettingsProperty Name of the property to remove
        * @return void
        */
        IMPORT_C static void RemovePropertyL( TInt aSettingsId,
                                              TXdmSettingsProperty aPropertyName );
                                          
        /**
        * Update an individual property in a named property set
        *
        * NOTE:
        *
        * - Leaves with KErrNotFound, if a property set by the
        *   specified ID cannot be found
        *
        * - Also note that an attempt to update a non-existent
        *   property effectively creates a new one.
        *
        * @param TInt ID of the target property set
        * @param TDesC& New value for the property to be updated
        * @param TXdmSettingsProperty Name of the property
        * @return void
        */
        IMPORT_C static void UpdatePropertyL( TInt aSettingsId,
                                              const TDesC& aPropertyValue,
                                              TXdmSettingsProperty aPropertyName );
                                         
        /**
        * Return an XDM property set/collection
        *
        * NOTE:
        *
        * - Leaves with KErrNotFound, if a property set by the
        *   specified ID cannot be found from the repository.
        *
        * @param TInt ID of the property set
        * @return CDesCArray& Colletion of properties
        */
        IMPORT_C static CXdmSettingsCollection* SettingsCollectionL( TInt aSettingsId );
        
        /**
        * Return an individual XDM settings property
        *
        * NOTE:
        *
        * - Leaves with KErrNotFound, if a property set by the
        *   specified ID cannot be found from the repository.
        *
        * @param TInt ID of the target property set
        * @param TXdmSettingsProperty Name of the property
        * @return HBufC* Value of the property on top of CleanupStack 
        */
        IMPORT_C static HBufC* PropertyL( TInt aSettingsId,
                                          TXdmSettingsProperty aSingleProp );
        
    private:

        /**
        * Write to the log file
        *
        * @param TRefByValue<const TDesC> Log string
        * @return void
        */
        static void WriteToLog( TRefByValue<const TDesC8> aFmt,... );
        
        /**
        * Find keys by their ID
        *
        * @param TInt ID of the desired property set
        * @param CRepository* An opened repository
        */
        static RArray<TUint32> FindByIdL( TInt aSettingsId, CRepository* aRepository );
                                            
        
        /**
        * Find keys by their type
        *
        * @param CRepository* An opened repository
        * @param TXdmSettingsProperty Type of the desired property set
        */                  
        static RArray<TUint32> FindByTypeL( CRepository* aRepository,
                                            TXdmSettingsProperty aSingleProp );
        
        /**
        * Create a unique ID for a collection of settings
        *
        * @param CRepository* An opened repository
        * @param TInt Number of the target row
        * @return TInt32 A new Central Repository key
        */
        static TInt CreateUniqueIdL( CRepository* aRepository, TInt aRow );
        
        /**
        * Find the row for the specified collection of settings
        *
        * @param TInt the ID of the desired property set
        * @param CRepository* An opened repository
        * @return TInt Row number for the target property set
        */
        static TInt FindRowL( TInt aSettingsId, CRepository* aRepository );
                                
        /**
        * Return the last row from the repository
        *
        * @param CRepository* An opened repository
        * @param TInt32 Column
        * @return TInt Row number for the last property set
        */
        static TInt LastRow( CRepository* aRepository );
    };

#endif      //__XDMSETTINGSAPI__
            
// End of File
