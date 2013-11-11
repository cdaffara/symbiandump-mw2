/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class that provides access to landmark databases.
*
*
*/


#ifndef CPOSLMLOCALDBACCESS_H
#define CPOSLMLOCALDBACCESS_H

#include <d32dbms.h>
#include <EPos_CPosLandmarkDatabase.h> // For TSize

enum TPosLmInitParams
    {
    ENoInit     = 0x0000,   // No initialization needed.
    ECreated    = 0x0001,   // Database has been created.
    ELanguage   = 0x0002,   // Language has changed.
    EIndexes    = 0x0004    // Database missing some indexes
    };

/**
*  Class that provides access to landmark databases.
*  Responsible for creating and opening landmark databases. It is also
*  responsible for transaction handling and for providing a handle to the
*  database.
*
*  @lib eposlmlocalaccess.lib
*/
class CPosLmLocalDbAccess : public CBase
    {
    public:  // Data types

        /**
        * Preparation type for PrepareViewLC.
        */
        enum TPreparationType
            {
            EAddPreparation = 0,
            EUpdatablePreparation
            };

        /**
        * Compact level type for NewL.
        */
        enum TCompactLevelType
            {
            ELocalCompactLevel = 0,
            EServerCompactLevel
            };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aCompactLevel The compact level type to compare with when
        *   deciding if compact is needed.
        */
        IMPORT_C static CPosLmLocalDbAccess* NewL(
        /* IN  */   TCompactLevelType aCompactLevel = ELocalCompactLevel
        );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmLocalDbAccess();

    public: // New functions

        /**
        * Begins a transaction and puts a rollback function on the clean-up
        * stack.
        */
        IMPORT_C void BeginTransactionLC();

        /**
        * Commits a transaction and pops the rollback function from the
        * clean-up stack. Rollback function is not popped if commit fails.
        */
        IMPORT_C void CommitTransactionAndPopL();

        /**
        * Returns a handle to the database. Do NOT close the RDbNamedDatabase
        * object when done with it. This will close the database for the client.
        * The ownership still belongs to this class. The handle is used for
        * issuing sql queries on the database.
        * @param aDatabase A handle to the database.
        */
        IMPORT_C void GetDatabase(
        /* OUT */   RDbNamedDatabase& aDatabase
        );

        /**
        * Opens a landmark database with the specified URI.
        * @param aUri A database URI.
        * @return KErrNone if successful. KErrNotFound, KErrPathNotFound,
        *   KErrArgument or KErrAlreadyExists if the database does not exist.
        */
        IMPORT_C TInt OpenDatabaseL(
        /* IN */    const TDesC& aUri
        );

        /**
        * Begins a transaction. Does not put a rollback on the clean-up stack.
        * Useful for asynchronous operations.
        * @return KErrNone for client side access to the database or, if this
        *   is a shared database and the client has successfully locked it.
        *   KErrLocked if this is a shared database and another client already
        *   has an exclusive write-lock on it. Otherwise, one of the other
        *   system error codes.
        */
        IMPORT_C TInt BeginTransaction();

        /**
        * Commits a transaction. Useful for asynchronous operations that cannot
        * put a rollback on the clean-up stack.
        * @return KErrNone if successful, otherwise one of the other system
        *   wide error codes including the DBMS database error codes.
        */
        IMPORT_C TInt CommitTransaction();

        /**
        * Rolls back the current transaction. Useful when a rollback function
        * cannot be placed on the clean-up stack, i.e. in asynchronous
        * operations.
        */
        IMPORT_C void RollbackTransaction();

        /*
        * Tests if there is an outstanding transaction on the database.
        * @return ETrue, if a transaction is in progress; EFalse, otherwise.
        */
        IMPORT_C TBool InTransaction() const;

        /*
        * Returns the init flag which tells whether a database must be
        * initialized or not.
        *
        * @return Init flag.
        */
        IMPORT_C TUint InitFlag() const;

        /*
        * Returns a pointer to the init flag which tells whether a database
        * must be initialized or not.
        *
        * @return Pointer to init flag.
        */
        IMPORT_C TUint* InitFlagPtr();

        /*
        * Resets the init flag which tells whether a database must be
        * initialized or not.
        */
        IMPORT_C void ResetInitFlag();

        /**
        * Checks if database is damaged. That is, it needs to be recovered.
        *
        * @return Returns ETrue if the database is damaged. Otherwise EFalse.
        */
        IMPORT_C TBool IsDamaged();

        /**
        * Executes an SQL query.
        *
        * @param aQuery SQL query.
        */
        IMPORT_C void ExecuteL(
        /* IN */        const TDesC& aQuery
        );

        /**
        * Prepares and evalutes a database view. The view is put on the cleanup
        * stack.
        * @param aType The type of preparation (add/read).
        * @param aView A database view to prepare and evaluate.
        * @param aSql A SQL query.
        * @param aComparison A comparison type.
        */
        IMPORT_C void PrepareViewLC(
        /* IN */        TPreparationType aType,
        /* IN/OUT */    RDbView& aView,
        /* IN */        const TDesC& aSql,
        /* IN */        TDbTextComparison aComparison = EDbCompareNormal
        );

        /**
        * Returns size information for the database.
        *
        * @returns Size information for the database.
        */
        IMPORT_C CPosLandmarkDatabase::TSize SizeL();

        /**
        * Checks whether a database needs to be initialized or not.
        *
        * @param aIsNeeded ETrue if the database needs to be initialized,
        *   EFalse otherwise.
        * @return KErrCorrupt if the settings table cannot be found.
        *   KErrNone otherwise.
        */
        IMPORT_C TInt IsInitializeNeededL(
        /* OUT */       TBool& aIsNeeded
        );

        /**
        * Sets a flag in the database that initialization is done.
        */
        IMPORT_C void SetInitializeDoneL();

        /**
        * Checks if the language must be changed for a landmark database.
        * That is if the language in the database does not match the language
        * of the phone.
        *
        * @param aIsNeeded ETrue if language must be changed, EFalse otherwise.
        * @return KErrCorrupt if the settings table cannot be found.
        *   KErrNone otherwise.
        */
        IMPORT_C TInt IsLanguageChangeNeededL(
        /* OUT */       TBool& aIsNeeded
        );

        /**
        * Sets the language of the database to the specified language.
        *
        * @param aLanguage A language to set.
        */
        IMPORT_C void SetLanguageL(
        /* IN */        TLanguage aLanguage
        );

        /**
        * Checks whether compact is needed or not.
        *
        * @return ETrue if compact is needed. EFalse otherwise.
        */
        IMPORT_C TBool IsCompactNeededL();

        /** Checks if new indexes exist in the database */
        IMPORT_C TBool IsIndexCreationNeededL() const;
        
    private:

        /**
        * C++ default constructor.
        */
        CPosLmLocalDbAccess(
        /* IN  */   TCompactLevelType aCompactLevel
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Checks whether the database needs to be initialized or not.
        * @return KErrCorrupt if impossible to read from settings table.
        *   Otherwise KErrNone.
        */
        TInt VerifyDatabaseNeedInitializationL();

        /**
        * Reads the internal compact database level from a resource file.
        */
        void ReadCompactDbLevelL();

        /**
        * Either checks if initialize or a language switch is needed. A boolean
        * specifies which type of check to perform.
        *
        * @param aIsInitCheck ETrue if init check. EFalse if language check.
        * @param aIsNeeded ETrue if the change is needed. EFalse otherwise.
        * @return KErrCorrupt if the settings table cannot be found.
        *   KErrNone otherwise.
        */
        TInt PerformCheckL(
        /* IN */        TBool aIsInitCheck,
        /* OUT */       TBool& aIsNeeded
        );

        /**
        * Sets a flag in the database telling that either language switch or
        * initialization has been performed.
        *
        * @param aSetLanguage ETrue if setting language, EFalse if setting
        *   init flag.
        * @param aLanguage If setting language this will hold the actual
        *   language.
        */
        void SetFlagL(
        /* IN */        TBool aSetLanguage,
        /* IN */        TLanguage aLanguage
        );
        
        /** Checks whether given index exists on given table */
        TBool IndexExistsL( const TDesC& aTable, const TDesC& aIndex ) const;

    private:    // Data

        // Database session
        RDbs iDbSession;

        // Database
        RDbNamedDatabase iDb;

        // Flag used for remembering if initialization must be done.
        TUint iInit;

        // Level for internal compact.
        TCompactLevelType iLevelType;
        TReal32 iRelativeCompactLevel;
        TInt32 iAbsoluteCompactLevel;
    };

#endif      // CPOSLMLOCALDBACCESS_H

