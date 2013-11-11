/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file of central repository database property class
*
*/


#ifndef C_CENREPDATABASEPROPERTY_H
#define C_CENREPDATABASEPROPERTY_H

#include <e32base.h>

/**
 *  central repository property class
 *
 *  This class represent one property in central repository
 *
 *  @lib cenrepdatabase.lib
 *  @since S60 V3.2
 */
NONSHARABLE_CLASS( CCenRepDatabaseProperty ): public CBase
    {
    public: // Constructor and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CCenRepDatabaseProperty* NewL();

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CCenRepDatabaseProperty* NewLC();

        /**
        * Destructors.
        */
        IMPORT_C virtual ~CCenRepDatabaseProperty();

    public: // member functions
        /**
         * Returns the name of this property
         *
         * @since S60 V3.2
         * @return Name of property
         */
        IMPORT_C TUint32 GetName() const;

        /**
         * Sets the name for this property
         *
         * @since S60 V3.2
         * @param aPropertyname new property name
         */
        IMPORT_C void SetName( TUint32 aPropertyname );

        /**
         * Returns value of property
         *
         * @since S60 V3.2
         * @param aValue on return, TInt value
         * @return Error code
         */
        IMPORT_C TInt GetValue( TInt& aValue ) const;

        /**
         * Sets value of property. Value must match to datatype.
         *
         * @since S60 V3.2
         * @param aValue new TInt value to be set
         * @return Error code
         */
        IMPORT_C TInt SetValue( TInt aValue );

        /**
         * Returns value of property
         *
         * @since S60 V3.2
         * @param aValue on return, descriptor value
         * @return Error code
         */
        IMPORT_C const TDesC& GetDesValue() const;

        /**
         * Sets the value of property. Value must match to datatype.
         *
         * @since S60 V3.2
         * @param aValue new descriptor
         * @return Error code
         */
        IMPORT_C TInt SetValue( const TDesC& aValue );


        /**
         * Compares data of this property to another.
         *
         * @since S60 V3.2
         * @param aProperty property to be compared
         * @return ETrue if properties have same data
         */
        IMPORT_C TBool CCenRepDatabaseProperty::operator==( 
                    const CCenRepDatabaseProperty& aProperty ) const;

    private:

        /**
        * C++ default constructor.
        */
        CCenRepDatabaseProperty();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // data

        /**
         * Descriptor of property value
         * Own.
         */
        RBuf iPropertyValue;

        /**
         * Name of property
         */
        TUint32 iPropertyName;


    private: // For testing
        #ifdef TEST_EUNIT
            friend class UT_CCenRepDatabaseProperty;
        #endif

    };
    
typedef RPointerArray<CCenRepDatabaseProperty> RIpAppPropArray;    
typedef RArray<TUint32> RKeyArray;

const TInt KCenRepMaxDesLength = 512;


#endif // C_CENREPDATABASEPROPERTY_H
