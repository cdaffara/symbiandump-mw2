/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Handle to a local landmark database.
*
*
*/


#ifndef CPOSLMLOCALDATABASE_H
#define CPOSLMLOCALDATABASE_H

#include <e32base.h>
#include <epos_cposlandmarkdatabaseextended.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <epos_cposlmdiskutilities.h>
#include <epos_mposlmdatabasefastcounters.h>

class CPosLmLocalDbAccess;
class CPosLandmark;
class RPosLandmarkServer;
class RPosLmLocalNameIndex;
class RPosLmLocalAccessSubsession;
class CPosLmDiskUtilities;
class CPosLmLocalImportOp;
class CPosLmLocalCategoryManager;
class CPosLmLocalDbFastCounters;

/**
* Handle to a local landmark database.
*
* This class contains functions for creating, iterating, reading, modifying
* and deleting landmarks.
*
* @lib eposlmlocalaccess.lib
*/
class CPosLmLocalDatabase : public CPosLandmarkDatabaseExtended
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * Opens a local landmark database specified by the database URI.
        *
        * @param[in] aDatabaseURI Database URI here represented as a path in the
        *   filesystem. If the URI is NULL the default landmark database will
        *   be loaded. The TAny pointer must be a HBufC pointer (a cast will be
        *   made). This class is instantiated by the ECom framework.
        * @return A new instance of this class.
        */
        IMPORT_C static CPosLmLocalDatabase* OpenL( TAny* aDatabaseUri );

        /**
        * Destructor.
        */
        IMPORT_C ~CPosLmLocalDatabase();

    public:  // Data types

        /** Specifies the type of database lock. */
        enum TLockType
            {
            EUnlocked = 0,
            EReadLock,
            EWriteLock
            };

        /** Specifies the transaction operation */
        enum TTransactionOp
            {
            EStart,
            ECommit,
            ERollback
            };

    public: // New functions

        /**
        * Copies partial settings from aFromSetting to aToSetting.
        *
        * @param aFromSetting The source partial read parameter settings.
        * @param aToSetting The destination partial read parameter settings.
        */
        void CopyPartialSettingsL(
            const CPosLmPartialReadParameters& aFromSetting,
            CPosLmPartialReadParameters& aToSetting );

        /**
        * Returns a handle to a database access object.
        * @return A database access object.
        */
        IMPORT_C CPosLmLocalDbAccess* DatabaseAccess();

        /**
        * Leaves with KErrPosLmNotInitialized if initialization is needed.
        */
        IMPORT_C void LeaveIfInitializingIsNeededL();

        /**
        * Returns the drive unit for current database.
        * @return A drive unit character.
        */
        TChar DatabaseDrive();

        /**
        * Returs a handle to the Landmarks Server.
        * @return Handle to the server.
        */
        RPosLmLocalAccessSubsession& Session();

        /**
        * Registers an event in the Landmarks Server.
        * @param aEventType Type of event.
        * @param aId Optional landmarks id.
        * @return KErrNone if successful. Otherwise any system-wide error code.
        */
        TInt RegisterEvent(
            TPosLmEventType aEventType,
            TPosLmItemId aId = KPosLmNullItemId );

        /**
        * Sets a lock on the database and puts a release lock function on the
        * clean-up stack.
        *
        * Leaves with KErrLocked if the aquired lock is not compatible with
        * previous locks on the database.
        *
        * @param aLockType The type of lock to set.
        */
        IMPORT_C void AquireLockLC( TLockType aLockType );

        /**
        * Sets a lock on the database. Does not put a release lock function on
        * the clean-up stack. Useful for asynchronous operations.
        *
        * Leaves with KErrLocked if the aquired lock is not compatible with
        * previous locks on the database.
        * 
        * @param aLockType The type of lock to set.
        */
        IMPORT_C void AquireLockL( TLockType aLockType );

        /**
        * Releases a lock on the database. Useful for asynchronous operations
        * that cannot put a release lock function on the clean-up stack.
        * 
        * @param aLockType The type of lock to release.
        */
        IMPORT_C TInt ReleaseLock( TLockType aLockType );
        
        /**
        * Returs a handle to the Landmarks Server.
        * @return Handle to the server.
        */
        IMPORT_C RPosLmLocalNameIndex& NameIndex();

        /**
        * Returs a handle to the Landmarks Server.
        * @return Handle to the server.
        */
        IMPORT_C const RPosLmLocalNameIndex& NameIndex() const;

        /** Starts new transaction for server data. 
         *  Puts rollback action to cleanup stack.
         *  Used in landmark change transactions to ensure updating
         *  lm name index on server side */
        IMPORT_C void BeginServerTransactionLC();
        
        /** Starts new transaction for server data. 
         *  Used in landmark change transactions to ensure updating
         *  lm name index on server side */
        IMPORT_C void BeginServerTransactionL();

        /** Commits current server data transaction and removes rollback from cleanup */
        IMPORT_C void CommitServerTransactionLX();
        
        /** Commits current server data transaction */
        IMPORT_C void CommitServerTransactionL();

        /** Cancels current transaction */
        IMPORT_C void RollbackServerTransaction();

    public: // From CPosLandmarkDatabase.

        /** @returns @p ETrue if the database is in need of initialization,
        *   otherwise @p EFalse. */
        TBool IsInitializingNeeded() const;

        /** @returns A handle to the operation. */
        CPosLmOperation* InitializeL();

        /** @param aLandmarkId The ID of the landmark to read.
        *   @returns The requested landmark. The landmark object is put on the
        *   cleanup stack. */
        CPosLandmark* ReadLandmarkLC( TPosLmItemId aLandmarkId );

        /** @return The landmark iterator. */
        CPosLmItemIterator* LandmarkIteratorL();

        /** @param[in] aSortPref A sort preference object.
        *   @return The landmark iterator. */
        CPosLmItemIterator* LandmarkIteratorL( const TPosLmSortPref& aSortPref );

        /** @param[in,out] aLandmark The landmark to add.
        *   @return The ID of the new landmark. */
        TPosLmItemId AddLandmarkL( CPosLandmark& aLandmark );

        /** @param[in] aLandmark The new landmark data. */
        void UpdateLandmarkL( const CPosLandmark& aLandmark );

        /** @param aLandmarkId The ID of the landmark to remove.*/
        void RemoveLandmarkL( TPosLmItemId aLandmarkId );

        /** @param aLandmarkIdArray The IDs of the landmarks to remove.
        *   @returns A handle to the operation. */
        CPosLmOperation* RemoveLandmarksL(
            const RArray<TPosLmItemId>& aLandmarkIdArray );

        /** @returns A handle to the operation. */
        CPosLmOperation* RemoveAllLandmarksL();

        /** @returns The current partial read parameters. */
        CPosLmPartialReadParameters* PartialReadParametersLC();

        /** @param aPartialSettings The new partial read parameters. */
        void SetPartialReadParametersL(
            const CPosLmPartialReadParameters& aPartialSettings );

        /** @param The ID of the landmark to read.
        *   @returns The requested landmark. The landmark object will only
        *   contain the attributes defined in the partial read attributes. The
        *   landmark object is put on the cleanup stack. */
        CPosLandmark* ReadPartialLandmarkLC( TPosLmItemId aLandmarkId );

        /** @param aLandmarkIdArray An array with IDs of the landmarks to read.
        *   @returns A handle to the operation. */
        CPosLmOperation* PreparePartialLandmarksL(
            const RArray<TPosLmItemId>& aLandmarkIdArray );

        /** @param aPreparePartialLandmarkOperation The operation object returned
        *   by the @ref PreparePartialLandmarksL function.
        *   @returns An array containing the prepared partial landmark objects. */
        CArrayPtr<CPosLandmark>* TakePreparedPartialLandmarksL(
            CPosLmOperation* aPreparePartialLandmarkOperation );

        /** @returns The database URI. The URI is put on the cleanup stack. */
        HBufC* DatabaseUriLC();

        /** @returns Size information for the database. */
        TSize SizeL();

        /** @returns A handle to the operation. */
        CPosLmOperation* CompactL();

        /** @param[out] aEvent Will contain the event information when an event occurs.
        *   @param[out] aStatus Will be completed with @p KErrNone if an event
        *   occurs and an error code if some error was encountered. */
        void NotifyDatabaseEvent(
            TPosLmEvent& aEvent,
            TRequestStatus& aStatus );

        /** @return @p KErrNone if the request was successfully cancelled,
        *   otherwise a system wide error code. */
        TInt CancelNotifyDatabaseEvent();

        /** @param[in] aLandmarkEncoder A landmark encoder object.
        *   @param[in] aLandmarkIdArray The landmarks which should be exported.
        *   @param[in] aTransferOptions A bitmap representing the options for the
        *   export operation. The bitmap values are defined by
        *   @ref _TTransferOptions.
        *   @returns A handle to the operation. */
        CPosLmOperation* ExportLandmarksL(
            CPosLandmarkEncoder&  aLandmarkEncoder,
            const RArray<TPosLmItemId>&  aLandmarkIdArray,
            TTransferOptions  aTransferOptions );

        /** @param[in] aLandmarkParser An object which can parse landmark content.
        *   @param aTransferOptions A bitmap representing the options for the
        *       import operation. The bitmap values are defined by
        *   @ref _TTransferOptions.
        *   @returns A handle to the operation. */
        CPosLmOperation* ImportLandmarksL(
            CPosLandmarkParser&  aLandmarkParser,
            TTransferOptions  aTransferOptions );

        /** @param[in] aLandmarkParser An object which can parse landmark content.
        *   @param[in] aLandmarkSelection An array defining which of the parsed
        *   landmarks to import. The array items refer to the order in which
        *   @ref CPosLandmarkParser accesses the landmarks. 0 means the first
        *   parsed landmark, 1 means the second, etc. If the parser supports
        *   indexing, these numbers correspond to the index values used to
        *   access the landmarks in @ref CPosLandmarkParser::LandmarkLC. The
        *   index values must be positive numbers and less than the number of
        *   landmarks accessed by the parser, otherwise this function will
        *   panic with code @p EPosInvalidIndex. @p Note: that the indexes
        *   can be used regardless of whether the parser supports indexing or
        *   not.
        * @param aTransferOptions A bitmap representing the options for the
        *   import operation. The bitmap values are defined by
        *   @ref _TTransferOptions.
        * @returns A handle to the operation. */
        CPosLmOperation* ImportLandmarksL(
            CPosLandmarkParser&  aLandmarkParser,
            const RArray<TUint>&  aLandmarkSelection,
            TTransferOptions  aTransferOptions );

        /** @param[in] aImportOperation This object is returned by
        *   @ref ImportLandmarksL methods.
        *   @returns An object for iterating the landmarks added in the latest
        *   import operation. */
        CPosLmItemIterator* ImportedLandmarksIteratorL(
            CPosLmOperation*  aImportOperation );

    protected: // from CPosLandmarkDatabaseExtended

        /** Returns reference to category manager instance for this database. */
        virtual CPosLmCategoryManager& CategoryManager();
        
        /** Returns pointer to extension interface. Type of the 
         *  requested extension interface is supplied by caller in aExtensionId.
         *
         *  Ownership of returned object is not transferred to caller, it is kept by
         *  implementation.
         *  
         *  @param[in] aExtensionUid Type of requested extension
         *  @return Pointer to extension implementation, or NULL if not supported.
         */
        virtual void* GetExtension( TUid aExtensionUid );
        
    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalDatabase();

        /**
        * @param aDatabaseUri Database URI.
        */
        void ConstructL( TAny* aDatabaseUri );

        /**
        * Reads a landmark with the specified id from the database.
        *
        * @param[in] aLandmarkId A landmark id.
        * @param[in] aIsPartial ETrue to read partial information. EFalse to read
        *   full information.
        * @return The landmark.
        */
        CPosLandmark* ReadLmLC(
            TPosLmItemId aLandmarkId,
            TBool aIsPartial );

        /**
        * Creates an import proxy operation. Caller takes ownership of the
        * operation.
        *
        * @param[in] aImportOp Import operation. Ownership is transferred.
        * @return Import proxy operation.
        */
        CPosLmOperation* CreateImportProxyL( CPosLmLocalImportOp* aImportOp );

        /**
        * Opens a landmark database.
        *
        * @param[in] aIsDefaultDb ETrue if the database is the default database.
        *   EFalse otherwise.
        */
        void OpenDatabaseL( TBool aIsDefaultDb );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalDatabase( const CPosLmLocalDatabase& );
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalDatabase& operator=( const CPosLmLocalDatabase& );

    private:    // Data

        // Database access
        CPosLmLocalDbAccess* iDbAccess;

        // Disk utilities
        CPosLmDiskUtilities* iDiskUtilities;

        // Database location
        HBufC* iDbUri;

        // Partial read settings
        CPosLmPartialReadParameters* iPartialSettings;

        // Database drive
        TChar  iDbDrive;

        // Session to Landmarks Server.
        RPosLandmarkServer* iServer;
        RPosLmLocalAccessSubsession* iSubSession;
        RPosLmLocalNameIndex* iNameIndex;

        // Instance of category manager, for client
        CPosLmLocalCategoryManager* iCategoryManager;

        // Fast counters extension
        CPosLmLocalDbFastCounters* iFastCounters;
    };

#endif      // CPOSLMLOCALDATABASE_H
