/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Abstract SQL connection class*
*/

#ifndef __MDSSQLCONNECTION_H__
#define __MDSSQLCONNECTION_H__

#include <e32base.h>
#include "mdccommon.pan"

#include "mdscommoninternal.h"


// types of columns
enum TColumnDataType
    {
    EColumnNotUsed,
    EColumnBool,
    EColumnInt32,
    EColumnUint32,
    EColumnInt64,
    EColumnReal32,
    EColumnReal64,
    EColumnTime,
    EColumnDes8,
    EColumnDes16,  // could be used as Unknown, because it checks the
                   // actual type from sqlite
    EColumnHBuf8,  // local pointer responsibility
    EColumnHBuf16, // local pointer responsibility
    EColumnNullDes8,
    EColumnNullDes16, // used when db column is null 
    //EColumnUnknown
    
    EColumnTItemId = EColumnUint32, // should be changed according to TItemId definition
    EColumnTDefId = EColumnUint32, // should be changed according to TDefId definition
    };

/**
* A class representing a database column.
* The column contains a type and an untyped pointer, which can
* be cast to represent various types.
*/
NONSHARABLE_CLASS(TColumn)
    {
    public:
        /** Returns column type. */
        inline TColumnDataType Type() const;

        /** Constructs a column with data type only. Use for queries. */
        inline TColumn( TColumnDataType aType = EColumnNotUsed );

        /** frees allocated memory in this column */
        inline void Free();

        // constructors

        TColumn( TInt32 aVal ) { Set( aVal ); }
        TColumn( TUint32 aVal ) { Set( aVal ); }
        TColumn( TBool aVal ) { Set( aVal ); }
        TColumn( TInt64 aVal ) { Set( aVal ); }
        TColumn( TReal32 aVal ) { Set( aVal ); }
        TColumn( TReal64 aVal ) { Set( aVal ); }
        TColumn( TTime aVal ) { Set( aVal ); }
        TColumn( const TDesC8& aVal ) { Set( aVal ); }
        TColumn( const TDesC16& aVal ) { Set( aVal ); }
        TColumn( const HBufC8* aVal ) { Set( aVal ); }
        TColumn( const HBufC16* aVal ) { Set( aVal ); }

        // data setters & getters
        
        inline void Set( TInt32 aVal );

        inline void Set( TUint32 aVal );

        inline void Set( TBool aVal );

        inline void Set( TInt64 aVal );

        inline void Set( TReal32 aVal );

        inline void Set( TReal64 aVal );

        inline void Set( TTime aVal );

        inline void Set( const TDesC8& aVal );

        inline void Set( const TDesC16& aVal );

        inline void Set( const HBufC8* aVal );

        inline void Set( const HBufC16* aVal );

        inline void NotUsed();

        // data getters

        inline void Get( TInt32& aValue ) const;

        inline void Get( TUint32& aValue ) const;

        inline void Get( TBool& aValue ) const;

        inline void Get( TInt64& aValue ) const;

        inline void Get( TReal32& aValue ) const;

        inline void Get( TReal64& aValue ) const;

        inline void Get( TTime& aValue ) const;

        inline void Get( TPtrC8& aValue ) const;

        inline void Get( TPtrC16& aValue ) const;

        inline TBool IsNull();

        /**
        * type of data in this column
        */
        TColumnDataType iType;

        /**
        * a structure to store multi-typed data
        */
        union TMultiTypeData
            {
            /*struct
                {
                TUint32 iLow;
                TUint32 iHigh;
                } iInt64;*/
            TInt64 iInt64;
            
            TReal32 iReal32;
            TReal64 iReal64;
            TInt32 iInt32;
            TUint32 iUint32;
            struct
                {
                TAny* iPtr;
                TInt iLen;
                } iText;
            };

        /**
        * multi-use data in this column
        */
        TMultiTypeData iData;
    };

/**
* A class representing a database row.
* The class describes each column type and the data in the column.
* where column data is located. The class is provided during
* query and is used when each row is read. Note that the
* buffer class MAY be modified by the user during the query.
* The same class is also used when replacing wildcards with
* variable data.
*/
class RRowData
    {
    public:

        /**
        * constructor
        */
        inline RRowData();

        /** Closes all the resources used by this object */
        inline void Close();

        /** frees allocated memory in this row */
        inline void Free();

        /** number of columns or variables */
        inline TInt Size() const;

        /** returns column by index */
        inline TColumn& Column( TInt aIndex );

        /** returns column by index */
        inline const TColumn& Column( TInt aIndex ) const;

        /** Appends a new column in row */
        inline TColumn& AppendL( const TColumn& aColumn );

        /** Resets the row */
        inline void Reset();

		/** Copy all columns */
		void AppendColumnTypesL( RRowData& aColumnTypeRow );

		/** Receive space to row */
		void ReserveL( TInt aColumnCount );
		
    protected:

        /** The array of columns */
        RArray<TColumn> iColumns;
    };


#include "mdssqlrow.inl" // inline methods

#endif	// __MDSSQLCONNECTION_H__


// End of File
