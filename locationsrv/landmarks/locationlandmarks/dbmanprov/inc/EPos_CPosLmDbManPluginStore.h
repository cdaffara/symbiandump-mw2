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
* Description: This class manages the collection of protocol-specific database manager
*  implementations.
*
*
*/


#ifndef CPOSLMDBMANPLUGINSTORE_H
#define CPOSLMDBMANPLUGINSTORE_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CPosLmDatabaseManagerPluginBase;
class CPosLmDbManPluginInfo;

// CLASS DECLARATION

/**
*  This class manages the collection of protocol-specific database manager
*  implementations. It loads all available implementations when constructed,
*  and deletes implementations when destructed. It also contains a method
*  for retrieval of implementation based on protocol id and traversing
*  the colection.
*
*  @lib eposlmdbmanprov.lib
*  @since S60 3.0
*  @version $Revision: 1.4 $, $Date: 2005/07/07 13:40:31 $
*/
class CPosLmDbManPluginStore : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        *
        * @since S60 3.0
        */
        static CPosLmDbManPluginStore* NewL(TAny* aConstructionParams);

        /**
        * Destructor.
        *
        * @since S60 3.0
        */
        virtual ~CPosLmDbManPluginStore();

    public: // New functions

        /**
        * Retrieves an instance of the provider implementation that supports
        * the given protocol. Client does NOT take ownership of returned object.
        *
        * @param aProtocol the protocol that should be supported by the returned
        * instance.
        * @return the implementation instance supporting the given protocol, or
        * NULL if no matching implementatio could be found.
        */
        CPosLmDatabaseManagerPluginBase* GetInstanceL(
        /* IN  */ const TDesC& aProtocol
        );

        /**
        * Returns the number of provider implementations.
        *
        * @return The number of provider implementations.
        */
        TInt NumberOfInstances() const;

        /**
        * Returns a provider implementation instance.
        * Client does NOT take ownership of returned object.
        *
        * @param aIndex The list index of the implementation instance (starts
        * at 0).
        * @return The implementation instance.
        */
        CPosLmDatabaseManagerPluginBase* GetInstanceAt(
        /* IN  */       TInt aIndex
        );


    protected:

        /**
        * C++ default constructor.
        */
        CPosLmDbManPluginStore();

    private:

       /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(TAny* aConstructionParams);


        // By default, prohibit copy constructor
        CPosLmDbManPluginStore( const CPosLmDbManPluginStore& );
        // Prohibit assigment operator
        CPosLmDbManPluginStore& operator= ( const CPosLmDbManPluginStore& );

    private:    // Data

        RPointerArray<CPosLmDbManPluginInfo> iPlugins;

    };

#endif      // CPOSLMDBMANPLUGINSTORE_H

// End of File
