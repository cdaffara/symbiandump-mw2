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
* Description:    PresSettingsAPI deals with the Sets of all
*               presence settings.
*
*/




#ifndef __PRESSETTINGSAPI__
#define __PRESSETTINGSAPI__

//includes
#include <badesca.h>

#include "simpleenginedomaincrkeys.h"
#include "pressettingsset.h"

// Forward declarations
class CRepository;

// FOR INTERNAL USE    
// consts related to hex calc.
const TInt KLastRow = 0x10000;
const TUint32 KOnlyRowInfo = 0xFFFFFF00;
const TInt KOnlyColumnInfo = 0x000000FF;
const TUint KFullValue = 0xFFFFFFFF;
const TInt KUnitRow = 0x100;
const TInt KLastUseableRow = KLastRow - 1;

// FOR CLIENTS

/**
* The list of configurable Presence Settings
* If change in future match them with pressetttingsapiprivatecrkeys.h
**/
enum TPresSettingsProperty
    {
    EPresPropSetID =        KSimpleSetId,               // int, not for client use         
    EPresStatusOfProps  =   KSimpleStatusofProps,       // Integer
    EPresPropSetName =      KSimpleSetName,             // Descriptor
    EPresPropSIPProfile =   KSimpleSipProfile,          // Integer
    EPresPropXDMSet =       KSimpleXDMSet,              // Integer
    EPresPropObjectSize =   KSimpleClientObjDataLimit,  // Integer
    EPresPropContSrvAdd =   KSimpleContentSvrAdd,       // Descriptor
    EPresPropPubInterval =  KSimpleSourceThrottlePublish,   // Integer
    EPresPropMaxSubs =      KSimpleMaxNbrPresenceSubs,      // Integer
    EPresPropMaxCntInList = KSimpleMaxNbrSubsInList,        // Integer
    EPresPropDomainSyntax = KSimpleServiceUriTemplate,      // Descriptor
    EPresPropProviderID   = KSimpleProviderID,              // Descriptor
    EPresPropResrv2,                        // Resevered 2
    };
    
/**
* These are the open/locked/hidden values for each settings item.
* Masked in a single integer, each set item takes 2 bits.
**/
enum TPresPropStatus
    {
    EPresSettingOpened =     0,      
    EPresSettingLocked =   1,
    EPresSettingHidden =   2
    };
    
    
/**
* This is the class which clients need to use for manipulating presence settings.
**/
class PresSettingsApi
    {
    public:
        
        /**
        * Returns the name of all setting sets stored in the repository.
        * This function is useful for displaying a list of available sets,
        * elements of RArray can be used to access each set seperately.
        * Make sure that CDesCArray* didnt allocated any memory before
        * this method. Caller may also want to reset aSetIds before
        * calling this method.
        *
        * @param RArray<TInt>& a list of available sets Ids
        * @return CDesCArray* Names of all settings sets
        */
        IMPORT_C static CDesCArray* GetAllSetsNamesLC( RArray<TInt>& aSetIds );

        /**
        * Creates a new set in the repository. This method may leave if some
        * system error occur while creating new set.
        *
        * @param TPresSettingsSet A Set of properties
        * @return TInt The ID of the newly created Set
        */
        IMPORT_C static TInt CreateSetL(TPresSettingsSet aSet );
        
        /**
        * Updats a set in the repository
        *
        * @param TPresSettingsSet A Set of properties
        *   TInt aSetId, ID of the Set to be updated
        * @return error code, KErrNotFound if id not found
        */
        IMPORT_C static TInt UpdateSetL(TPresSettingsSet aSet, TInt aSetId );
        
        /**
        * Removes a settings Set from the Repository
        *
        * @param TInt aSetId, ID of the Set to be removed
        * @return error code, KErrNotFound if set doesnt found.
        */
        IMPORT_C static TInt RemoveSetL( TInt aSetId );
        
        /**
        * Updates a single integer property in a settings set.
        * @param TPresSettingsProperty Name of the property 
        * @param TInt aSetId ID of the target property set
        * @param TInt aPropValueInt Value of the property
        * @return error code, KErrNotFound if set doesnt found.
        */
        IMPORT_C static TInt UpdatePropertyL( TInt aSetId, 
                                            TPresSettingsProperty aPropertyName,
                                            TInt aPropValueInt
                                            );
                                            
        /**
        * Updates a single descriptor property in a settings set.
        * @param TPresSettingsProperty Name of the property 
        * @param TInt aSetId ID of the target property set
        * @param TDesC& Value for the property to be created
        * @return error code, KErrNotFound if set doesnt found.
        */
        IMPORT_C static TInt UpdatePropertyL( TInt aSetId, 
                                            TPresSettingsProperty aPropertyName,
                                            const TDesC& aPropValueDes
                                            );

        /**
        * Returns a Pres settings set
        *
        * @param TInt aSetID, ID of the property set
        *  TPresSettingsSet& aSet, set returns here as reference 
        * @return error code, KErrNotFound if set doesnt found.
        */
        IMPORT_C static TInt SettingsSetL(TInt aSetID, 
                                                        TPresSettingsSet& aSet);
                                                        
        /**
        * Returns TPresPropStatus for a given property from the given integer.
        * Use aPropertyName=EPresStatusOfProps to get the status of a set as a whole
        *
        * @param TInt aStatusOfProps, integer to decode from
        *  TPresSettingsProperty Name of the property 
        * @return TPresPropStatus for the given property
        */
        IMPORT_C static TPresPropStatus DecodePropertyStatus(TInt aStatusOfProps,
                                              TPresSettingsProperty aPropertyName);
                                              
        /**
        * Encodes the given TPresPropStatus to the given integer.
        * Use aPropertyName=EPresStatusOfProps to set the status of a set as a whole
        *
        * @param TInt aStatusOfProps, reference integer to be encoded
        *  TPresPropStatus status to be encoded to given integer
        *  TPresSettingsProperty Name of the property
        * @return none
        */
        IMPORT_C static void EncodePropertyStatus(TInt& aStatusOfProps,
                                              TPresPropStatus aPropertyStatus,
                                           TPresSettingsProperty aPropertyName);
                                           
        /**
        * Find whether the given property is descriptor or not
        *
        * @param TPresSettingsProperty aProperty a settings property
        * @return TBool, ETrue if its an string
        */
        IMPORT_C static TBool IsStr( TPresSettingsProperty aProperty );
        
    private:
    
        /**
        * Create a unique ID for a set
        *
        * @param CRepository* An opened repository
        * @param TUint32 aRowInHex, a fresh row
        * @return TInt created unique ID
        */
        static TInt CreateUniqueIdL( CRepository* aRepository, TUint32 aRowInHex );
        
        /**
        * Get a fresh (empty) row, which is used for creating a new set
        *
        * @param CRepository* An opened repository
        * @return TUint32 aRowInHex, a fresh row
        */
        static TUint32 GetFreshRawInHex( CRepository* aRepository );
        
        /**
        * Find the row having given set ID
        *
        * @param CRepository* An opened repository
        *   TInt aSetID, ID of the property set
        * @return TUint32, the row in case found, otherwise this is error code (-ve)
        */
        static TUint32 GetRawInHexL( CRepository* aRepository, TInt aSetID );
        
    };

#endif      //__PRESSETTINGSAPI__
            
// End of File