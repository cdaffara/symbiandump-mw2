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
* Description:   TPosLmDatabaseSettings class
*
*/



#ifndef TPOSLMDATABASESETTINGS_H
#define TPOSLMDATABASESETTINGS_H

#include <e32std.h>

const TInt KPosLmDatabaseName = 64;
#define KPOSLMDATABASESETTINGSRESERVEDSIZE 31

typedef TBuf<KPosLmDatabaseName> TPosLmDatabaseName;

/**
*  @ref TPosLmDatabaseSettings encapsulates the attributes that can be set for
*  a landmark database.
*
*  The only available attribute is the displayable name.
*
*  To set a new display name, create a @ref HPosLmDatabaseInfo object
*  containing the URI of the database. Call @ref SetDatabaseName on the
*  @ref TPosLmDatabaseSettings member in @ref HPosLmDatabaseInfo and then
*  pass the @ref HPosLmDatabaseInfo to
*  @ref CPosLmDatabaseManager::ModifyDatabaseSettingsL.
*
*  When retrieving settings for a database, @ref IsAttributeSet can be used to
*  find out whether display name is set for the database or not.
*
*  @lib eposlmdbmanlib.lib
*  @since S60 3.0
*/
class TPosLmDatabaseSettings
    {
    public:

        /**
        *  Enumeration of the attributes that can be set for a landmark
        *  database.
        */
        enum TAttribute
            {
            EName = 0x01                 /**<
                Displayable name for the landmark database */
            };

    public:

        /**
        * Default constructor.
        */
        IMPORT_C TPosLmDatabaseSettings();


    public:

        /**
        * Checks whether a database attribute is set in this instance.
        *
        * @param aDbAttribute The database attribute to check.
        * @return @p ETrue if the attribute is set, otherwise @p EFalse.
        */
        IMPORT_C TBool IsAttributeSet( TAttribute aDbAttribute ) const;

        /**
        * Unset a database attribute.
        *
        * If an attribute is not set in this instance and the instance is
        * passed as input to a function for modifying the settings for a
        * database, e.g.  @ref CPosLmDatabaseManager::ModifyDatabaseSettingsL,
        * the attribute will be removed from the database.
        *
        * @param aDbAttribute The database attribute to unset.
        */
        IMPORT_C void UnsetAttribute( TAttribute aDbAttribute );

        /**
        * Retrieves the displayable name for the database.
        *
        * If the @p EName attribute is not set, an empty descriptor is returned.
        *
        * @return A pointer to the name descriptor. This pointer is valid until
        *   the @ref TPosLmDatabaseSettings instance is destroyed.
        */
        IMPORT_C TPtrC DatabaseName() const;

        /**
        * Set a displayable name for the database.
        *
        * If an empty descriptor is set, the database display name will be
        * set to an empty string.
        *
        * @param[in] aDatabaseName The new name for the database.
        */
        IMPORT_C void SetDatabaseName( const TPosLmDatabaseName& aDatabaseName );

    private:

        TUint8 iAttributes;
        TPosLmDatabaseName iName;
        TUint8 iReserved[KPOSLMDATABASESETTINGSRESERVEDSIZE];

    };

#endif      // TPOSLMDATABASESETTINGS_H


