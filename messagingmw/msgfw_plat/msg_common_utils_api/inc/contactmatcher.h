/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   class definition of CContactMatcher
*
*/



#ifndef CONTACTMATCHER_H
#define CONTACTMATCHER_H

// System includes
#include <e32base.h>
#include <e32std.h>
#include <bamdesca.h>

#include <MVPbkSingleContactOperationObserver.h>
#include <MVPbkContactStoreListObserver.h>
#include <MVPbkContactFindObserver.h>
#include <MVPbkFieldType.h>
#include <CVPbkPhoneNumberMatchStrategy.h>
#include <AiwContactSelectionDataTypes.h>
#include <MVPbkContactFindFromStoresObserver.h>
#include <barsc.h>

// Classes referenced
class RFs;
class CActiveSchedulerWait;
class CVPbkContactManager;
class MVPbkContactLink;
class CVPbkContactLinkArray;
class CVPbkContactStoreUriArray;
class MVPbkContactOperationBase;
class MVPbkFieldType;
class MVPbkFieldTypeList;
class MVPbkStoreContact;
class MVPbkStoreContactField;
class CVPbkFieldTypeRefsList;
class MVPbkStoreContactFieldCollection;
class MVPbkContactFieldSelector;

class CPbk2SortOrderManager;
class MPbk2ContactNameFormatter;
/**
* CContactMatcher class is an API for contact matching.
*
* Before a method starts executing a Virtual Phonebook operation,
* it checks if there's an older asynchronous operation already in execution.
* If there is, synchronous methods leave with error code KErrInUse.
* Asynchronous methods complete the request with same error code.
*
* If phone backup/restore has been detected, using methods which
* require Virtual Phonebook is not possible normally. 
* Instead those methods leave with error code KErrAccessDenied.
* Once EStoreBackupRestoreCompleted event has been received,
* methods can be called normally.
*
* @lib VPbkEngine.lib
* @since 3.1
*/

class CContactMatcher : public CBase,
	public MVPbkSingleContactOperationObserver,
	public MVPbkContactStoreListObserver,
	public MVPbkContactFindObserver, 
	public MVPbkContactFindFromStoresObserver 
    {
public: // Construct & destruct

    /**
    * Two-phase constructor for CContactMatcher class.
    *
    * @param  aFsSession File server session.
    *
    * @return CContactMatcher* Pointer to newly created instance.
    */
    IMPORT_C static CContactMatcher* NewL( RFs*  aFsSession );

    /**
    * Two-phase constructor for CContactMatcher class.
    * Like NewL(), but also places instance on cleanup stack.
    *
    * @return CContactMatcher* Pointer to newly created instance.
    */
    IMPORT_C static CContactMatcher* CContactMatcher::NewLC( RFs* aFsSession );

    /**
    * C++ destructor.
    */
    IMPORT_C ~CContactMatcher();

private: // Internal construct

    /**
    * C++ Constructor.
    */
    CContactMatcher( RFs* aFsSession );

    /**
    * Second phase constructor
    */
    void ConstructL();

public: // API methods

    /**
    * Opens one or more contact stores for future match operations.
    *
    * @param aUriArray  Array of contact store URIs to be opened.
    *
    * @exception KErrNotSupported when none of the stores opens
    * @exception KErrGeneral if some stores are already open
    */
    IMPORT_C void OpenStoreL( const CVPbkContactStoreUriArray& aUriArray );

    /**
    * Opens one or more contact stores for future match operations.
    * Asynchronous version.
    *
    * @param aUriArray  Array of contact store URIs to be opened.
    * @param aStatus  Completion status of the request.
    *                 KErrNone - at least one store opened successfully
    *                 KErrNotSupported - all stores failed to open
    *                 KErrGeneral - if some stores are already open
    */		    
    IMPORT_C void OpenStoreL( const CVPbkContactStoreUriArray& aUriArray,
        TRequestStatus& aStatus );			    

    /**
    * Opens all contact stores for future match operations.
    * NOTE: does not open OwnNumber store.
    *
    * @exception KErrNotSupported when none of the stores opens
    * @exception KErrGeneral if some stores are already open
    */
    IMPORT_C void OpenAllStoresL();

    /**
    * Opens all contact stores for future match operations.
    * Asynchronous version.
    * NOTE: does not open OwnNumber store.
    *
    * @param aStatus  Completion status of the request.
    *                 KErrNone - at least one store opened successfully
    *                 KErrNotSupported - all stores failed to open
    *                 KErrGeneral - if some stores are already open
    */
    IMPORT_C void OpenAllStoresL( TRequestStatus& aStatus );			

    /**
    * Opens default contact stores for future match operations.
    * Used to find local aliases to phone numbers and email addresses
    * NOTE: does not open OwnNumber store or fixed dialing store
    * Opens the stores in the default preference order
    *
    * @exception KErrNotSupported when none of the stores opens
    * @exception KErrGeneral if some stores are already open
    */
    IMPORT_C void OpenDefaultMatchStoresL();

    /**
    * Opens default contact stores for future match operations.
    * Used to find local aliases to phone numbers and email addresses
    * Asynchronous version.
    * NOTE: does not open OwnNumber store or fixed dialing store
    * Opens the stores in the default preference order
    *
    * @param aStatus  Completion status of the request.
    *                 KErrNone - at least one store opened successfully
    *                 KErrNotSupported - all stores failed to open
    *                 KErrGeneral - if some stores are already open
    */
    IMPORT_C void OpenDefaultMatchStoresL( TRequestStatus& aStatus );

    /**
    * Opens all OwnNumber stores for future match operations.
    *
    * @exception KErrNotSupported when none of the stores opens
    * @exception KErrGeneral if some stores are already open
    */
    IMPORT_C void OpenOwnNumberStoresL();

    /**
    * Opens all OwnNumber stores for future match operations.       
    *
    * @param aStatus  Completion status of the request.
    *                 KErrNone - at least one store opened successfully
    *                 KErrNotSupported - all stores failed to open
    *                 KErrGeneral - if some stores are already open
    */	
    IMPORT_C void OpenOwnNumberStoresL( TRequestStatus& aStatus );	

    /**
    * Closes all currently open contact stores
    * including OwnNumber stores.
    *
    * @return Leaves on error.
    */
    IMPORT_C void CloseStoresL();

    /**
    * Determines whether a phone number is OwnNumber. Synchronous version.
    * The method searches for the number from currently open stores.
    * In order to use SIM's ISDN store, it should be the only open store.
    * Can be opened with OpenOwnNumberStoresL().
    *
    * @param aSearch  Phone number to search for
    * @param aResult  Boolean result.
    */
    IMPORT_C void IsOwnNumberL( const TDesC& aSearch, TBool& aResult );

    /**
    * Determines whether a phone number is OwnNumber. Asynchronous version.
    * The method searches for the number from currently open stores.
    * In order to use SIM's ISDN store, it should be the only open store.
    * Can be opened with OpenOwnNumberStoresL().
    *
    * @param aSearch  Phone number to search for
    * @param aStatus  Request status containing the result and 
    *                 possible error code.
    *                 KErrNone     = TRUE,
    *                 KErrNotFound = FALSE,
    *                 other value  = system wide error code.        
    */
    IMPORT_C void IsOwnNumberL( const TDesC& aSearch, TRequestStatus& aStatus );

    /**
    * Finds contacts with phone numbers that match the search string.
    * Synchronous version.
    * Contacts are searched from all currently open contact stores.
    * Matching is done from the end of the phone number.
    * Note: If the search string is shorter than 7 digits, matching from
    * Phone memory contact store works ONLY if the phone number is
    * exactly the same as the search string.
    * Example:
    * Search string "567" would find phone number "567", but not "1234567".
    * From SIM stores both numbers would be found.
    *
    * @param aSearch  Search string
    * @param aDigits  Number of digits to match from the end of phone number.
    * @param aFlags   Search all or stop at first match.
    *                 See enum values from CVPbkPhoneNumberMatchStrategy.h
    * @param aLinkArray  Links to matching contacts are returned in this
    *                 array that method caller provides.
    *
    * @return KErrNone = success, otherwise an error code. ,,list of codes
    */
    IMPORT_C void MatchPhoneNumberL( const TDesC& aSearch, TInt aDigits,
        CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aFlags,
        CVPbkContactLinkArray& aLinkArray );

    /**
    * Finds contacts with phone numbers that match the search string.
    * Asynchronous version.
    * Searching is done according to parameters like in the synchronous version,
    * but status code is returned in aStatus.
    *
    * @param aStatus  Completion status of the request.
    *                 Values: KErrNone = success, otherwise an error code.
    */
    IMPORT_C void MatchPhoneNumberL( const TDesC& aSearch, TInt aDigits,
        CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aFlags,
        CVPbkContactLinkArray& aLinkArray, TRequestStatus& aStatus );

    /**
    * Returns the global list of possible field types.
    *
    * @return List of field types
    */
    IMPORT_C const MVPbkFieldTypeList& FieldTypes() const;
                
    /**
    * Gets a store contact from a contact link.
    * @param aContactLink  The link from which store contact should be returned.
    * @param aStoreContact Pointer to store contact.
    *                      Client must take the ownership immediately.
    */
    IMPORT_C void GetStoreContactL( const MVPbkContactLink& aContactLink,
        MVPbkStoreContact** aStoreContact );
        
    /**
    * Gets a store contact from a contact link.
    * Asynchronous version.
    * Parameters like in the synchronous version,
    * but status code is returned in aStatus.
    *
    * @param aStatus  Completion status of the request.
    *                 Values: KErrNone = success, otherwise an error code.
    */
    IMPORT_C void GetStoreContactL( const MVPbkContactLink& aContactLink,
        MVPbkStoreContact** aStoreContact, TRequestStatus& aStatus );

    /**
    * Returns a pointer to contact's field data of given field type.
    * If the field type isn't found from contact, return value is KNullDesC.
    * NOTE: this works only for field types of storage type 
    * EVPbkFieldStorageTypeText.
    * For other types, leaves with error code KErrArgument.
    *
    * @param  aStoreContact The contact from which field data should be returned.
    * @param  aFieldType    Field's type.
    * @return TPtrC pointer to field's data.
    */
    IMPORT_C TPtrC GetFieldDataTextL( const MVPbkStoreContact& aStoreContact,
        const MVPbkFieldType& aFieldType ) const; 

    /**
    * Returns a pointer to contact's field data of given field type.
    * If the field type isn't found from contact, return value is
    * "1.1.1111". (d.m.yyyy).
    * NOTE: this works only for field types of storage type 
    * EVPbkFieldStorageTypeDateTime.
    * For other types, leaves with error code KErrArgument.
    *
    * @param  aStoreContact The contact from which field data should be returned.
    * @param  aFieldType    Field's type.
    * @return TTime pointer to field's data.
    */
    IMPORT_C TTime GetFieldDataDateTimeL( const MVPbkStoreContact& aStoreContact,
        const MVPbkFieldType& aFieldType ) const; 

    /**
    * Returns a pointer to contact's field data of given field type.
    * If the field type isn't found from contact, return value is KNullDesC8.
    * NOTE: this works only for field types of storage type 
    * EVPbkFieldStorageTypeBinary.
    * For other types, leaves with error code KErrArgument.
    *
    * @param  aStoreContact The contact from which field data should be returned.
    * @param  aFieldType    Field's type.
    * @return TPtrC8 pointer to field's data.
    */
    IMPORT_C TPtrC8 GetFieldDataBinaryL(  const MVPbkStoreContact& aStoreContact,
        const MVPbkFieldType& aFieldType ) const; 		    		    

    /**
    * Cancels asynchronous operation.
    */	
    IMPORT_C void CancelOperation();

    /**
    * Returns the used contact store list, needed for e.g. aiw fetch
    * @return The used contact store list
    */
    IMPORT_C MVPbkContactStoreList& GetContactStoresL();

    /**
    * Returns the name of the contact in the same format as MPbk2ContactNameFormatter
    * @param  aFieldCollection The fieldcollection of the contact
    * @return HBufC* the name of the contact or null
    */
    IMPORT_C HBufC* GetNameL(
        MVPbkStoreContactFieldCollection& aFieldCollection );

    /**
    * Finds out if the contact has the right field, support for phonenumber, email and mms
    * @param aAddressSelectType is the address type looked for
    *        aContact is the contact from which the data is searched from
    * @return TInt KErrNotFound in case data is not found
    */
    IMPORT_C TInt ContactHasFieldOfTypeL( TAiwAddressSelectType aAddressSelectType,
        const MVPbkStoreContact& aContact );

    /**
    * Returns reference to the contactmanager
    * @return reference to the contact manager
    */
    IMPORT_C CVPbkContactManager& GetContactManager();
    
    /**
    * Finds contacts with field data that match the search string.
    * Contacts are searched from all currently open contact stores.
    * Matching is done from field types given by the method caller.
    * The list of all possible field types can be fetched from the wrapper
    * using FieldTypes().
    *
    * @param aSearch  Search string
    * @param aFieldTypes  List of field types included in matching.
    * @param aLinkArray  Links to matching contacts are returned 
    *                    in this array that method caller provides.
    *
    * @return KErrNone = success, otherwise an error code. ,,list of codes
    */
    IMPORT_C void MatchDataL( const TDesC& aSearch, const MVPbkFieldTypeList& aFieldTypes, 		
        CVPbkContactLinkArray& aLinkArray);

    /**
    * Finds contacts with field data that match the search string.
    * Contacts are searched from all currently open contact stores.
    * Matching is done from field types given by the method caller.
    * The list of all possible field types can be fetched from the wrapper
    * using FieldTypes().
    *
    * @param aSearchStrings  Search strings
    * @param aFieldTypes  List of field types included in matching.
    * @param aLinkArray  Links to matching contacts are returned
    *                    in this array that method caller provides.
    * @param aWordParserCallBack is the callback function to the parser
    *
    * @return KErrNone = success, otherwise an error code.
    */
    IMPORT_C void MatchDataL( const MDesC16Array& aSearchStrings, 
        const MVPbkFieldTypeList& aFieldTypes,
        CVPbkContactLinkArray& aLinkArray,
        const TCallBack& aWordParserCallBack );		   

    /**
    * Finds contacts with field data that match the search string.
    * Asynchronous version.
    * Searching is done according to parameters like in the synchronous version,
    * but status code is returned in aStatus.
    *
    * @param aStatus  Completion status of the request.
    *                 Values: KErrNone = success, otherwise an error code.
    */
    IMPORT_C void MatchDataL( const TDesC& aSearch, const MVPbkFieldTypeList& aFieldTypes, 		
        CVPbkContactLinkArray& aLinkArray, TRequestStatus& aStatus );
        
     /**
    * Returns reference to the contact name formatter. Object is created if not used earlier.
    * @return reference to the contact name formatter
    */
    IMPORT_C MPbk2ContactNameFormatter& ContactNameFormatterL();
            
private: // from MVPbkContactStoreListObserver, MVPbkContactStoreObserver

    /**
    * Called when the opening process is complete, ie. all stores have
    * been reported either failed or successfully opened.
    */
    void OpenComplete();

    /**
    * Called when a contact store is ready for use.
    */
    void StoreReady( MVPbkContactStore& aContactStore );

    /**
    * Called when a contact store becomes unavailable.
    * Client may inspect the reason of the unavailability and decide whether or not
    * it will keep the store opened (ie. listen to the store events).
    * @param aContactStore The store that became unavailable.
    * @param aReason The reason why the store is unavailable.
    *                This is one of the system wide error codes.
    */
    void StoreUnavailable( MVPbkContactStore& aContactStore, TInt aReason );

    /**
    * Called when changes occur in the contact store.
    * @see TVPbkContactStoreEvent, MVPbkContactStoreObserver.h
    *
    * @param aContactStore
    * @param aStoreEvent Event that has occured.
    */
    void HandleStoreEventL( MVPbkContactStore& aContactStore,
        TVPbkContactStoreEvent aStoreEvent );

private: // from MVPbkContactFindFromStoresObserver
    /**
    * Called when find is complete on a single store. Callee takes
    * ownership of the results. In case of an error during find,
    * the aResultsFromStore may contain only partial results of the find.
    *
    * @param aStore is the store from which the contacts were searched from
    *
    * @param aResultsFromStore Array of contact links that matched the find.
    *                 Callee must take ownership of this object in
    *                 the end of the function, ie. in case the function
    *                 does not leave.
    */
    void FindFromStoreSucceededL( MVPbkContactStore& aStore,
        MVPbkContactLinkArray* aResultsFromStore );
                        
    /**
    * This function is called if/when there were errors while searching
    * from a store.
    * @param aStore is the store from which the search was done.
    * @param aError is the error code.
    */
    void FindFromStoreFailed( MVPbkContactStore& aStore, TInt aError );

    /**
    * Called when find is complete.
    */
    void FindFromStoresOperationComplete();

private: // from MVPbkContactFindObserver

    /**
    * Called when find is complete. Callee takes ownership of the results.
    * In case of an error during find, the aResults may contain only
    * partial results of the find.
    *
    * @param aResults Array of contact links that matched the find.
    *                 Callee must take ownership of this object in
    *                 the end of the function, ie. in case the function
    *                 does not leave.
    */
    void FindCompleteL( MVPbkContactLinkArray* aResults );

    /**
    * Called in case the find fails for some reason.
    * 
    * @param aError One of the system wide error codes.
    */
    void FindFailed( TInt aError );

private: // from MVPbkSingleContactOperationObserver

    /**
    * Called when operation is completed.
    *
    * @param aOperation the completed operation.
    * @param aContact  the contact returned by the operation.
    *                  Client must take the ownership immediately.
    */
    void VPbkSingleContactOperationComplete(
        MVPbkContactOperationBase& aOperation, MVPbkStoreContact* aContact );

    /**
    * Called if the operation fails.
    *
    * @param aOperation    the failed operation.
    * @param aError        error code of the failure.
    */
    void VPbkSingleContactOperationFailed(
        MVPbkContactOperationBase& aOperation, TInt aError );        

private: // Constants

    // Wrapper method IDs for calls that have an asynchronous version
    enum TMethodId
        {
        ENoMethod = 0,
        EMatchPhoneNumber,
        EMatchData,
        EGetStoreContact,
        EOpenStore, 
        ECloseStores 
        };

    // Wrapper method processing status.
    enum TMethodStatus
        {
        EIdle = 0,
        EExecuting,
        EFinished
        };

private: // Methods
    MVPbkContactFieldSelector* CreateFieldTypeSelectorLC( TInt aResId );
    const MVPbkStoreContactField* FindField( const MVPbkStoreContact& aContact,
        const MVPbkFieldType& aFType ) const;
    static CVPbkContactStoreUriArray* GetStoreArrayLC(
        const TDesC& (* const aFuncPtrs[])() );
    void CContactMatcher::OpenStoreL( const TDesC& (* const aFuncPtrs[])() );
    void CContactMatcher::OpenStoreL( const TDesC& (* const aFuncPtrs[])(),
        TRequestStatus&  aStatus );
    void FreeOldOperation();

    // Copies the entries to the existing recipient array
    void CopyFindResultsL( MVPbkContactLinkArray* aResults );

    // Open store, code common to sync/async versions.
    void OpenStoreCommonL( const CVPbkContactStoreUriArray& aUriArray );

    // Match phonenumber, code common to sync/async versions.
    void MatchPhoneNumberCommonL( const TDesC& aData, TInt aDigits,
        CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aFlags );

    void InitOperationL( TMethodId aApiMethod );
    void InitOperationL( TMethodId aApiMethod, TRequestStatus* aStatus );
    void InitOperation( TRequestStatus* aStatus );
    void OperationComplete( TInt ErrorCode = KErrNone );
    void OperationFailed( TInt aError );
    void CleanupNumberMatch();
    void RemoveSimilarEmailAddressesL( const TDesC& aData, CVPbkContactLinkArray& aLinkArray, const MVPbkFieldTypeList& aFieldTypes );

private: // Data

    // Used members

    // File server session for ContactManager. Can also be NULL.
    RFs* iFsSession;  
    MVPbkStoreContact** iResultStoreContact; // result of GetStoreContact
    CVPbkContactLinkArray* iResultContactLinkArray; // result of matching operations
    TInt iResultContactLinkCnt; // number of links found in matching operations
    TRequestStatus* iClientStatus; // request status used in asynch calls

    // Own members
    CVPbkContactManager* iContactManager;
    MVPbkContactOperationBase* iOperation;  // CM operation being processed
    CActiveSchedulerWait iASchedulerWait;   // used in asynch calls
    CVPbkContactStoreUriArray* iStoreUris;  // used in matching
    CVPbkPhoneNumberMatchStrategy* iMatchStrategy; // used in matching
    CVPbkPhoneNumberMatchStrategy::TConfig* iStratConfig;  // used in matching
    TBool iSync;  // is this wrapper call Synchronous (1) or Asynchronous (0) 
    TInt iError;  // error code used while processing VPbk-operations
    TBool iBackup;// contact store backup/restore in progress
    // API method ID. Needed for cleanup after method finishes.
    TMethodId iApiMethod;
    // API method status. Needed to know processing has finished.
    TMethodStatus iApiMethodStatus;

    CPbk2SortOrderManager* iSortOrderManager;
    MPbk2ContactNameFormatter* iNameFormatter;
    TBool iResourceFileInitialized;
    RResourceFile iResourceFile ;
    };

/**
* Panic codes used in CContactMatcher.
*
* @since 3.1u
*/
class ContactMatcherPanics
    {
public: // API
        
    /**
    * Panic codes
    */
    enum TPanic
        {
        EPanNullPointer = 0,
        EPanInvalidParam,
        EPanInvalidOp
        };

public:

    /**
    * Panic
    */
    static void Panic( TPanic aPanic );
    };

#endif

// End of File
