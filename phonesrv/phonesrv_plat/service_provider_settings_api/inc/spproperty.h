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
* Description:  Header file of service provider settings property class
*
*/


#ifndef C_SPPROPERTY_H
#define C_SPPROPERTY_H

#include <e32base.h>

#include "spdefinitions.h"

/**
 *  Service provider settings property class
 *
 *  This class represent one property of Sevice Provider settings
 *
 *  @lib serviceprovidersettings.lib
 *  @since S60 V3.2
 */
NONSHARABLE_CLASS( CSPProperty ): public CBase
    {

    public: // Constructor and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPProperty* NewL();

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSPProperty* NewLC();

        /**
        * Destructors.
        */
        IMPORT_C virtual ~CSPProperty();

    public: // member functions
        /**
         * Returns the name of this property
         *
         * @since S60 V3.2
         * @return Name of property
         */
        IMPORT_C TServicePropertyName GetName() const;

        /**
         * Sets the name for this property
         *
         * @since S60 V3.2
         * @param aPropertyname new property name
         */
        IMPORT_C TInt SetName( TServicePropertyName aPropertyname );

        /**
         * Returns the type of this property
         *
         * @since S60 V3.2
         * @return Type of property
         */
        IMPORT_C TSPItemType GetPropertyType() const;

        /**
         * Sets the type for this property
         *
         * @since S60 V3.2
         * @param aPropertyType property type
         */
        TInt SetPropertyType( TSPItemType aPropertyType );


        /**
         * Returns the datatype of this property
         * No imported, set automatically.
         *
         * @since S60 V3.2
         * @return Datatype of property
         */
        IMPORT_C TPropertyDataType GetDataType() const;

        /**
         * Sets the datatype of this property
         *
         * @since S60 V3.2
         * @param aDataType new data type
         */
        void SetDataType( TPropertyDataType aDataType );

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
        IMPORT_C TInt GetValue( TDes& aValue ) const;

        /**
         * Sets the value of property. Value must match to datatype.
         *
         * @since S60 V3.2
         * @param aValue new descriptor
         * @return Error code
         */
        IMPORT_C TInt SetValue( const TDesC& aValue );


        /**
         * Returns value of property
         *
         * @since S60 V3.2
         * @param aValue on return, On/Off value
         * @return Error code
         */
        IMPORT_C TInt GetValue( TOnOff& aValue ) const;

        /**
         * Sets the value of property. Value must match to datatype.
         *
         * @since S60 V3.2
         * @param aValue new On/Off value
         * @return Error code
         */
        IMPORT_C TInt SetValue( TOnOff aValue );

        /**
         * Sets the value of this property without datatype check
         *
         * @since S60 V3.2
         * @param aValue new value
         * @return Error code
         */
        TInt SetValueNoDataTypeCheck( const TDesC& aValue );

        /**
         * Returns datatype of given property
         *
         * @since S60 V3.2
         * @param aProperty property name
         * @return Datatype of given property
         */
        IMPORT_C static TPropertyDataType DataType( TServicePropertyName aProperty );
        
        /**
         * Returns property type of given property
         *
         * @since S60 V3.2
         * @param aProperty property name
         * @return property type of given property
         */
        IMPORT_C static TSPItemType PropertyType( TServicePropertyName aProperty );

        /**
         * Copies data from source property // to be removed?
         *
         * @since S60 V3.2
         * @param aSource source property
         */
        IMPORT_C void CopyL( const CSPProperty& aSource );

        /**
         * Compares data of this property to another.
         *
         * @since S60 V3.2
         * @param aProperty property to be compared
         * @return ETrue if properties have same data
         */
        IMPORT_C TBool operator==( 
                    const CSPProperty& aProperty ) const;

    private:

        /**
        * C++ default constructor.
        */
        CSPProperty();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
         * Sets default value of property if defined
         *
         * @since S60 V3.2
         * @param aProperty property name
         * @return An error code
         */
        TInt SetDefaultValue( TServicePropertyName aProperty );

    private: // data

        /**
         * Descriptor of property value
         * Own.
         */
        RBuf               iPropertyValue;

        /**
         * Name of property
         */
        TServicePropertyName        iPropertyName;

        /**
         * Datatype of property
         */
        TPropertyDataType        iDataType;
        
        /**
         * Property type
         */
        TSPItemType             iPropertyType;
    
    };


#endif // C_SPPROPERTY_H
