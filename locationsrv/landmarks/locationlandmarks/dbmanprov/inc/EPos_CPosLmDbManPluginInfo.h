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
* Description: This class is a placeholder for a database manager plugin and its
*  associated destructor id key.
*
*
*/


#ifndef CPOSLMDBMANPLUGININFO_H
#define CPOSLMDBMANPLUGININFO_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CPosLmDatabaseManagerPluginBase;

// CLASS DECLARATION

/**
*  This class is a placeholder for a database manager plugin and its
*  associated destructor id key. It handles loading and unloading of the
*  ECom plugin.
*
*  @lib eposlmdbmanprov.lib
*  @since S60 3.0
*  @version $Revision: 1.4 $, $Date: 2005/07/07 13:40:31 $
*/
class CPosLmDbManPluginInfo : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        *
        * @param aDtorIdKey ECom destructor key.
        * @param aConstructionParams Construction parameters for the plugin.
        */
        static CPosLmDbManPluginInfo* NewLC(
        /* IN  */ TUid aDtorIdKey,
        /* IN  */ TAny* aConstructionParams);

        /**
        * Destructor.
        *
        */
        virtual ~CPosLmDbManPluginInfo();

    public: // New functions

        /**
        * Retrieves the plugin instance.
        *
        * @return The plugin instance.
        */
        CPosLmDatabaseManagerPluginBase* Instance();

    protected:

        /**
        * C++ default constructor.
        */
        CPosLmDbManPluginInfo();

    private:

       /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aDtorIdKey ECom destructor key.
        * @param aConstructionParams Construction parameters for the plugin.
        */
        void ConstructL(
        /* IN  */ TUid aDtorIdKey,
        /* IN  */ TAny* aConstructionParams);


        // By default, prohibit copy constructor
        CPosLmDbManPluginInfo(const CPosLmDbManPluginInfo&);
        // Prohibit assigment operator
        CPosLmDbManPluginInfo& operator= (const CPosLmDbManPluginInfo&);

    private:    // Data

        CPosLmDatabaseManagerPluginBase* iPlugin;
        TUid iDtorIdKey;

    };

#endif      // CPOSLMDBMANPLUGININFO_H

// End of File
