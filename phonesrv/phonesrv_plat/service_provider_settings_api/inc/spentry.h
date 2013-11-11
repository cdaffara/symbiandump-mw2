/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings entry
*
*/


#ifndef C_SPENTRY_H
#define C_SPENTRY_H

#include <e32base.h>

#include "spdefinitions.h"

class CSPProperty;


/**
 *  Service Provider Settings entry. 
 *  Container class for service attributes and settings properties
 *
 *  @lib serviceprovidersettings.lib
 *  @since S60 V3.2
 */
NONSHARABLE_CLASS( CSPEntry ): public CBase
    {

    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPEntry* NewL();

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPEntry* NewLC();

        /**
        * Destructors.
        */
        IMPORT_C virtual ~CSPEntry();

    public: // Member functions
        /**
         * Returns service Id of this entry.
         *
         * @since S60 V3.2
         * @return Service ID
         */
        IMPORT_C TServiceId GetServiceId() const;

        /**
         * Sets unique service provider ID for this entry.
         * This ID is generated automatically when new entry is added.
         * Service ID is also required when user updates entry in service table
         *
         * @since S60 V3.2
         * @param aServiceId service ID
         */
        void SetServiceId( TServiceId aServiceId ); 

        /**
         * Returns service name of this entry.
         *
         * @since S60 V3.2
         * @return Service name 
         */
        IMPORT_C const TDesC& GetServiceName() const;

        /**
         * Sets service name for this entry.
         *
         * @since S60 V3.2
         * @param aServiceName service name
         */
        IMPORT_C TInt SetServiceName( const TDesC& aServiceName );

        /**
         * Adds new property to this entry.
         *
         * @since S60 V3.2
         * @param aProperty new property pointer
         * @return KErrNone if addition succeeded, 
         *         system wide error code otherwise.
         */
        IMPORT_C TInt AddPropertyL( const CSPProperty& aProperty );

        /**
         * Updates content of property in this entry.
         *
         * @since S60 V3.2
         * @param aPropertyName property name
         * @param aValue new TInt value
         * @return KErrNone if update succeeded.
         */
        IMPORT_C TInt UpdateProperty( TServicePropertyName aPropertyName, TInt aValue );

        /**
         * Updates content of property in this entry.
         *
         * @since S60 V3.2
         * @param aPropertyName property name
         * @param aValue new TDesC value
         * @return KErrNone if update succeeded.
         */
        IMPORT_C TInt UpdateProperty( TServicePropertyName aPropertyName, const TDesC& aValue );

        /**
         * Updates content of property in this entry.
         *
         * @since S60 V3.2
         * @param aPropertyName property name
         * @param aValue new TOnOff value
         * @return KErrNone if update succeeded.
         */
        IMPORT_C TInt UpdateProperty( TServicePropertyName aPropertyName, TOnOff aValue );

        /**
         * Returns property of this entry by property name.
         *
         * @since S60 V3.2
         * @param aProperty On return, reference to requested property
         * @param aPropertyName Name of requested property
         * @return error code
         */
        IMPORT_C TInt GetProperty( const CSPProperty*& aProperty, 
                                   TServicePropertyName aPropertyName ) const;

        /**
         * Returns property of this entry by property index.
         *
         * @since S60 V3.2
         * @param aProperty On return, reference to requested property
         * @param aIndex index of requested property
         * @return error code
         */
        IMPORT_C TInt GetProperty( const CSPProperty*& aProperty, 
                                    TInt aIndex ) const;

        /**
         * Deletes property from this entry by index.
         *
         * @since S60 V3.2
         * @param aIndex index of requested property
         * @return error code
         */
        IMPORT_C TInt DeleteProperty( TInt aIndex );

        /**
         * Deletes property from this entry by property name.
         *
         * @since S60 V3.2
         * @param aProperty property name
         * @return error code
         */
        IMPORT_C TInt DeleteProperty( TServicePropertyName aProperty );

        /**
         * Returns all properties of this entry.
         *
         * @since S60 V3.2
         * @return Reference to array of properties
         */
        IMPORT_C const RPropertyArray& GetAllProperties() const;

        /**
         * Returns count of properties in this entry
         *
         * @since S60 V3.2
         * @return Count of properties
         */
        IMPORT_C TInt PropertyCount() const;
        
        /**
         * reset service entry
         *
         * @since S60 V3.2
         * @return Count of properties
         */
         IMPORT_C void Reset();
        

    private:

        /**
        * C++ default constructor.
        */
        CSPEntry();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
         * Search index of requested property in iPropertyArray
         *
         * @since S60 ?S60_version
         * @param aProperty
         * @return Index of property or KErrNotFound
         */
        TInt SearchProperty( TServicePropertyName aProperty ) const;

    private: // data

        /**
         * Id of service settings entry ( unique )
         */
        TServiceId iServiceId;

        /**
         * Name of settings entry ( unique )
         * Own.
         */
        RBuf iServiceName;

        /**
         * Service settings properties owned by this entry
         * Own.
         */
        RPropertyArray iPropertyArray;

    };


#endif // C_SPENTRY_H
