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
* Description:   HPosLmDatabaseInfo class
*
*/



#ifndef HPOSLMDATABASEINFO_H
#define HPOSLMDATABASEINFO_H

#include <e32base.h>
#include <f32file.h>
#include "EPos_TPosLmDatabaseSettings.h"

/**
*  @ref HPosLmDatabaseInfo encapsulates information about a landmark database.
*
*  @lib eposlmdbmanlib.lib
*  @since S60 3.0
*/
class HPosLmDatabaseInfo
    {
    public:

        /**
        * Two-phased constructor.
        *
        * @param[in] aDatabaseUri The URI of the landmark database.
        * @return A new instance of this class.
        */
        IMPORT_C static HPosLmDatabaseInfo* NewLC( const TDesC& aDatabaseUri );

        /**
        * Two-phased constructor.
        *
        * @param[in] aDatabaseUri The URI of the landmark database.
        * @return A new instance of this class.
        */
        IMPORT_C static HPosLmDatabaseInfo* NewL( const TDesC& aDatabaseUri );

        /**
        * Two-phased copy constructor.
        *
        * @param[in] aDatabaseInfo The information instance to copy.
        * @return A new instance of this class.
        */
        IMPORT_C static HPosLmDatabaseInfo* NewLC(
            const HPosLmDatabaseInfo& aDatabaseInfo
        );

        /**
        * Two-phased copy constructor.
        *
        * @param[in] aDatabaseInfo The information instance to copy.
        * @return A new instance of this class.
        */
        IMPORT_C static HPosLmDatabaseInfo* NewL(
            const HPosLmDatabaseInfo& aDatabaseInfo
        );


    public:

        /**
        * Retrieves the database URI.
        *
        * @return A pointer to the URI descriptor. This pointer is valid until
        *   the @ref HPosLmDatabaseInfo object is destroyed.
        */
        IMPORT_C TPtrC DatabaseUri() const;

        /**
        * Returns the protocol part from the URI which is set.
        *
        * Example: If the URI is "file://c:landmarks.ldb" then the protocol is
        * "file".
        *
        * If no protocol is specified, an empty descriptor is returned. This
        * implies "file" protocol.
        *
        * @return A pointer to the protocol descriptor. This pointer is valid
        *   until the @ref HPosLmDatabaseInfo object is destroyed.
        */
        IMPORT_C TPtrC Protocol() const;

        /**
        * Returns whether the database is the default database.
        *
        * Note: that this attribute is only set if the @ref HPosLmDatabaseInfo
        * instance has been returned from a @ref CPosLmDatabaseManager
        * function, e.g. @ref CPosLmDatabaseManager::ListDatabasesL,
        * @ref CPosLmDatabaseManager::GetDatabaseInfoL,
        * @ref CPosLmDatabaseManager::RegisterDatabaseL,
        * @ref CPosLmDatabaseManager::CreateDatabaseL or
        * @ref CPosLmDatabaseManager::ModifyDatabaseSettingsL. If not, this
        * function returns @p EFalse.
        *
        * @return @p ETrue if the database is the default one, otherwise @p EFalse.
        */
        IMPORT_C TBool IsDefault() const;

        /**
        * Returns which storage media the database resides in.
        *
        * Note: this attribute is only set if the @ref HPosLmDatabaseInfo
        * instance has been returned from a @ref CPosLmDatabaseManager
        * function, e.g. @ref CPosLmDatabaseManager::ListDatabasesL,
        * @ref CPosLmDatabaseManager::GetDatabaseInfoL,
        * @ref CPosLmDatabaseManager::RegisterDatabaseL,
        * @ref CPosLmDatabaseManager::CreateDatabaseL or
        * @ref CPosLmDatabaseManager::ModifyDatabaseSettingsL. If not, this
        * function returns @p EMediaUnknown.
        *
        * @return The storage media the database resides in.
        */
        IMPORT_C TMediaType DatabaseMedia() const;

        /**
        * Returns which database drive the database resides in.
        *
        * Note: that this attribute is only set if the @ref HPosLmDatabaseInfo
        * instance has been returned from a @ref CPosLmDatabaseManager
        * function, e.g. @ref CPosLmDatabaseManager::ListDatabasesL,
        * @ref CPosLmDatabaseManager::GetDatabaseInfoL,
        * @ref CPosLmDatabaseManager::RegisterDatabaseL,
        * @ref CPosLmDatabaseManager::CreateDatabaseL or
        * @ref CPosLmDatabaseManager::ModifyDatabaseSettingsL. If not, this
        * function returns 0.
        *
        * If database drive is not applicable for the database, e.g. the
        * database is remote, this function returns 0.
        *
        * @return The drive letter for the drive where the database resides, or
        *   0 if the letter is not set.
        */
        IMPORT_C TChar DatabaseDrive() const;

        /**
        * Retrieve a const reference to the database settings.
        *
        * The const reference can be used to read the database settings.
        *
        * @returns Const reference to the database settings
        */
        IMPORT_C const TPosLmDatabaseSettings& Settings() const;

        /**
        * Retrieve a reference to the database settings.
        *
        * The reference can be used to read and write to the database settings.
        *
        * @returns Reference to the database settings
        */
        IMPORT_C TPosLmDatabaseSettings& Settings();

        /**
        * Returns the size in bytes of this object.
        *
        * @returns The size of this object.
        */
        IMPORT_C TInt Size() const;

        /**
        * @internal */
        /*
        * Sets the default database indicator flag.
        * This flag is used to indicate if the database described by the
        * object is the default database.
        *
        * @param[in] aIsDefault @p ETrue if the database is the default database,
        * otherwise @p ETrue.
        */
        void SetDefault( TBool aIsDefault );

        /**
        * @internal */
        /*
        * Sets the database media type.
        *
        * @param[in] aMediaType The media type.
        */
        void SetMediaType( TMediaType aMediaType );

        /**
        * @internal */
        /*
        * Sets the database drive.
        *
        * @param[in] aDatabaseDrive The database drive letter.
        */
        void SetDatabaseDrive( TChar aDatabaseDrive );

   private:

        // C++ constructor.
        HPosLmDatabaseInfo(const TDesC& aDatabaseUri);

        // C++ copy constructor.
        HPosLmDatabaseInfo(const HPosLmDatabaseInfo& aDatabaseInfo);

        // Allocates memory for URI descriptor during construction.
        static TAny* AllocateL(const TDesC& aDatabaseUri);

        // Sets URI descriptor during construction.
        void SetDatabaseUri(const TDesC& aDatabaseUri);

        // Calculates buffer offset so that buffer is 4-byte aligned
        static TInt BufferOffset();

        // Prohibit assigment operator
        HPosLmDatabaseInfo& operator= ( const HPosLmDatabaseInfo& );

    private:

        TPosLmDatabaseSettings iSettings;
        TMediaType iDatabaseMedia;
        TChar iDatabaseDrive;
        TBool iIsDefault;
        TUint8 iBuffer[1];  // Must be at end of class

    };

#endif      // HPOSLMDATABASEINFO_H


