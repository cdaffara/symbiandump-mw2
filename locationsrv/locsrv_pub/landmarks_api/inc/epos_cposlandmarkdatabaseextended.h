/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SDK/S60, Landmarks API
*
*/


#ifndef EPOS_CPOSLANDMARKDATABASEEXTENDED_H_
#define EPOS_CPOSLANDMARKDATABASEEXTENDED_H_

#include "EPos_CPosLandmarkDatabase.h"

class CPosLmCategoryManager;

/**
*  Handle to a landmark database.
*
*  This class defines extensions to the functionality of @ref CPosLandmarkDatabase class.
*
*  @see CPosLandmarkDatabase
*  @since S60 3.2
*  @lib eposlandmarks.lib.
*/
class CPosLandmarkDatabaseExtended :  public CPosLandmarkDatabase
    {
    public:
        
        /**
         * Opens the default landmark database.
         *
         * This method is the same as @ref CPosLandmarkDatabase::OpenL().
         *
         * The client takes ownership of the returned database handle.
         *
         * The database may have to be initialized before it can be used, see
         * @ref IsInitializingNeeded and @ref InitializeL.
         *
         * This function requires @p ReadUserData capability.
         *
         * @leave KErrNotSupported This extended interface is not supported
         *                         for default database.
         * @returns A handle to the open database.
         * @see CPosLandmarkDatabase::OpenL()
         */
        IMPORT_C static CPosLandmarkDatabaseExtended* OpenL();

        /**
         * Opens a specific landmark database.
         *
         * This method is the same as @ref CPosLandmarkDatabase::OpenL(const TDesC&).
         *
         * The client refers to a database by URI. The URI consists of a
         * protocol specifier and the database location: "protocol://location".
         * If the client does not specify a protocol, "file://" will be assumed.
         *
         * For local landmark databases, the URI consists of the drive and the
         * database file name, e.g. "c:landmarks.ldb". The path cannot be
         * specified by the client. The extension of the database file name must
         * be "ldb" otherwise the client will get the error @p KErrArgument.
         *
         * If the client specifies a local database and does not specify the
         * drive letter, e.g. "landmarks.ldb", default database drive will be assumed.
         *
         * The client takes ownership of the returned database handle.
         *
         * The database may have to be initialized before it can be used, see
         * @ref IsInitializingNeeded and @ref InitializeL.
         *
         * This function requires @p ReadUserData capability.
         *
         * @param[in] aDatabaseUri The URI of the database to open.
         * @returns A handle to the open database.
         *
         * @leave KErrArgument Extension of the local database name is not "ldb".
         * @leave KErrNotSupported This extended interface is not supported
         *                         for specified protocol.
         * @see CPosLandmarkDatabase::OpenL(const TDesC&)
         */
        IMPORT_C static CPosLandmarkDatabaseExtended* OpenL( const TDesC&  aDatabaseUri );

        /**
         * Destructor.
         */
        IMPORT_C virtual ~CPosLandmarkDatabaseExtended();

    public:

        /** Returns total amount of landmarks in the database.
         *  @return Number of landmarks in the database or negative error code:
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        IMPORT_C TInt LandmarksCount();

        /** Returns total amount of landmark categories in the database.
         *  @return Number of landmark categories in the database or negative error code.
         *      - KErrNotSupported if not supported in current database.
         *      - KErrPosLmNotInitialized if database is not yet initialized. */
        IMPORT_C TInt CategoriesCount();

    public:
        
        /** Returns reference to category manager instance for this database. */
        virtual CPosLmCategoryManager& CategoryManager() = 0;

    protected:

        /** Returns pointer to extension interface. Type of the 
         *  requested extension interface is supplied by caller in aExtensionId.
         *
         *  Ownership of returned object is not transferred to caller, it is kept by
         *  implementation.
         *  
		 *  @param[in] aExtensionUid Type of requested extension
		 *  @return Pointer to extension implementation, or NULL if not supported.
		 */
        virtual void* GetExtension( TUid aExtensionUid ) = 0;

    protected:

        // C++ constructor.
        IMPORT_C CPosLandmarkDatabaseExtended();

    private:

        // Prohibit copy constructor
        CPosLandmarkDatabaseExtended( const CPosLandmarkDatabaseExtended& );
        // Prohibit assigment operator
        CPosLandmarkDatabaseExtended& operator= ( const CPosLandmarkDatabaseExtended& );
        
    private:
        TInt8 reserved[32];    
    };


#endif /*EPOS_CPOSLANDMARKDATABASEEXTENDED_H_*/
