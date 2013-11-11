/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility class to central repository database.
*
*/




#ifndef C_CENREPDATABASEUTIL_H
#define C_CENREPDATABASEUTIL_H

#include <e32base.h>
#include <cenrepdatabaseproperty.h>


class CRepository;

/**
 *  Utility class to accessing tables in central repository database
 * 
 *
 *  Support tables formatted in following way:
 * 
 *    //-----------------------------------------------------
 *    //| KColId  | KColProp1 | KColProp2 | ... | KColPropX |
 *    //|  (int)  |  (des)    |  (des)    |     |   (des)   |
 *    //-----------------------------------------------------
 *    //|   1     |  "Prop a" |  "Prop x" | ... |  "aaa"    |
 *    //|   7     |  "Prop b" |  "Prop y" | ... |  "bbb"    |
 *    //|   15    |  "Prop c" |  "Prop z" | ... |  "ccc"    |
 *    //|   0     |    ""     |    ""     | ... |    ""     |
 *    //|   0     |    ""     |    ""     | ... |    ""     |
 *    //|   ...   |    ...    |    ...    | ... |    ...    |
 *   //------------------------------------------------------
 *
 *
 *  @code
 *  @endcode
 *
 *  @lib cenrepdatabase.lib
 *  @since S60 v3.2 
 */
NONSHARABLE_CLASS( CCenRepDatabaseUtil ) : public CBase
    {
    public:

        /**
         * Two-phased constructor.
         */
        IMPORT_C static CCenRepDatabaseUtil* NewL( TUid aUid, 
                                                 TUint32 aStartKey, 
                                                 TUint32 aColIncrement, 
                                                 TUint32 aColMask,
                                                 TUint32 aIdCounterKey,
                                                 TInt aColCount );
        /**
         * Two-phased constructor.
         */
        IMPORT_C static CCenRepDatabaseUtil* NewLC( TUid aUid, 
                                                  TUint32 aStartKey, 
                                                  TUint32 aColIncrement, 
                                                  TUint32 aColMask,
                                                  TUint32 aIdCounterKey,
                                                  TInt aColCount );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CCenRepDatabaseUtil();
        
    public: 
        /**
         * Start transaction in repository. 
         * Note. Function leaves two cleanup items to cleanupstack and those
         * are freed when CommitTransaction or RollbackTransaction is called.
         *
         * @since S60 v3.2
         */
        IMPORT_C void BeginTransactionL();

        /**
         * Commit changes in repository
         *
         * @since S60 v3.2
         * @return An error code
         */
        IMPORT_C TInt CommitTransaction();
        
        /**
         * Rollback changes in repository
         *
         * @since S60 v3.2
         */
        IMPORT_C void RollbackTransaction();
        
        /**
         * Store new entry to table.
         *
         * @since S60 v3.2
         * @param aEntryId Contains new ID when returns.
         * @param aArray Container for properties of this entry.
         */
        IMPORT_C void AddEntryL(  TInt& aEntryId, const RIpAppPropArray& aArray );

        /**
         * Search entry by entry ID. 
         *
         * @since S60 v3.2
         * @param aEntryId Id of requested entry
         * @param aArray on return, property array.
         * @return error code, KErrNotFound if unknown entry.
         */
        IMPORT_C TInt FindEntryL( TInt aEntryId, RIpAppPropArray& aArray );

        /**
         * Updates entry to central repository
         *
         * @since S60 v3.2
         * @param aEntryId Id of requested entry
         * @param aArray Updated properties of entry
         * @return error code, KErrNotFound if unknown entry.
         */
        IMPORT_C TInt UpdateEntryL( TInt aEntryId, 
                                    const RIpAppPropArray& aArray );

        /**
         * Deletes settings entry from storage by entry ID.
         *
         * @since S60 v3.2
         * @param aEntryId Id of entry which is going to be deleted
         * @return error code, KErrNone if delete succeeded, otherwise system wide error code
         */
        IMPORT_C TInt DeleteEntryL( TInt aEntryId );

        /**
         * Search property of entry from storage.
         *
         * @since S60 v3.2
         * @param aEntryId The entry ID of requested entry
         * @param aPropertyName property name to be found
         * @param aProperty on return, requested property
         * @return error code, KErrNone if succeeded, otherwise system wide error code
         */
        IMPORT_C TInt FindPropertyL( TInt aEntryId, 
                                     TUint32 aPropertyName,
                                     CCenRepDatabaseProperty& aProperty );

        /**
         * Add or update properties of entry in table.
         * Values of existing properties are overwritten.
         *
         * @since S60 v3.2
         * @param aEntryId The ID of updated entry
         * @param aArray new properties to be added or updated
         * @return error code, KErrNotFound if unknown entry.
         */
        IMPORT_C TInt AddOrUpdatePropertiesL( TInt aEntryId, 
                                              const RIpAppPropArray& aArray );


        /**
         * Delete properties of entry in table.
         *
         * @since S60 v3.2
         * @param aEntryId The entry ID of requested entry
         * @param aNameArray name of properties to be deleted
         * @return error code, KErrNotFound if unknown entry.
         */
        IMPORT_C TInt DeletePropertiesL( TInt aEntryId,
                                         const RArray<TUint32>& aNameArray );

        /**
         * Return count of entries in table.
         *
         * @since S60 v3.2
         * @return Count of stored settings.
         */
        IMPORT_C TInt EntryCountL( TInt& aCount );

        /**
         * Search all entry IDs from storage
         *
         * @since S60 v3.2
         * @param aEntryIds An array containing returned entry IDs
         * @return error code
         */
        IMPORT_C TInt FindEntryIdsL( RArray<TInt>& aEntryIds );
                                   
        /**
         * Find entry IDs with same properties
         *
         * @since S60 v3.2
         * @param aPropertyArray property array
         * @param aEntryIds on return, entry Ids with same properties
         * @return error code
         */
        IMPORT_C TInt FindEntryIdsFromPropertiesL( const RIpAppPropArray& aPropertyArray, 
                                                   RArray<TInt>& aEntryIds );
        
        
    private:

        /**
         * C++ default constructor.
         */
        CCenRepDatabaseUtil( TUint32 aStartKey, 
                           TUint32 aColIncrement, 
                           TUint32 aColMask,
                           TUint32 aIdCounterKey,
                           TInt aColCount );

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL( TUid aUid );

        /**
         * Release semaphore, this function is used in custom cleanup
         * 
         * @since S60 v3.2
         * @param aPtr Pointer to an object which is the target of the cleanup operation
         */
        static void ReleaseSemaphore( TAny* aPtr );

        /**
         * Release semaphore
         * 
         * @since S60 v3.2
         */
        void DoReleaseSemaphore();

        /**
         * Set new value for given key. Creates the key if it doesn't exists.
         *
         * @since S60 v3.2
         * @param aKey
         * @param aValue
         */
        template<class T> void SetOrCreateKeyL( TUint32 aKey, 
                                                const T& aValue );
                                                
        /**
         * Create new ID for entry.
         *
         * @since S60 v3.2
         * @param aNewId Contains new ID when return.
         */
        void CreateEntryIdL( TInt& aNewId );
        
        /**
         * Defines next available key for ID.
         *
         * @since S60 v3.2
         * @return New key for ID
         */
        TUint32 GetNewIdKeyL( );
        
        /**
         * Update given properties of entry.
         *
         * @since S60 v3.2
         * @param aIdKey
         * @param Array Contains updated properties and values
         */
        void UpdatePropertiesL( TUint32 aIdKey, const RIpAppPropArray& aArray );
        
        /**
         * Check if entry has given properties.
         *
         * @since S60 v3.2
         * @param aEntryId Id of entry.
         * @param aPropertyArray Properties to be checked.
             * @param aOk Result
         */
        void CheckEntryHasPropertiesL( TInt aEntryId, const RIpAppPropArray& aPropertyArray, TBool& aOk );
        
        /**
         * Custom cleanup for array.
         *
         * @since S60 v3.2
         * @param aPointer
         */
        static void CleanupPointerArray( TAny* aPointer );
        
        
    private: // data
    
        /**
         * Central Repository object.
         * Own.
         */
        CRepository* iRepository;
        
        /**
         * RSemaphore object.
         * Own.
         */
        RSemaphore iSemaphore;
        
        /**
         * Start key in first column( Id column )
         */
        TUint32 iStartKey;

        /**
         * Column increment
         */
        TUint32 iColIncrement;

        /**
         * Column mask
         */
        TUint32 iColMask;
        
        /**
         * Id counter key
         */
        TUint32 iIdCounterKey;
        
        /**
         * Column count
         */
        TInt iColCount;

    private: // For testing
        #ifdef TEST_EUNIT
            friend class UT_CCenRepDatabaseUtil;
        #endif

    };


#endif // C_CENREPDATABASEUTIL_H
