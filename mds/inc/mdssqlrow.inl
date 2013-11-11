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

// -----------------------------------
// TColumn
// -----------------------------------

inline TColumn::TColumn( TColumnDataType aType ) :
	iType( aType )
    {
    iData.iInt32 = 0;
    }

inline TColumnDataType TColumn::Type() const
    {
    // the HBuf types are internal and hidden.
    switch ( iType )
	    {
	    case EColumnHBuf8:
	    	return EColumnDes8;
	    case EColumnHBuf16:
	    	return EColumnDes16;
		case EColumnNullDes8:
			return EColumnDes8;
		case EColumnNullDes16:
			return EColumnDes16;
		default:
			return iType;
	    }
    }

inline void TColumn::Free()
    {
    if ( iType == EColumnHBuf8 )
        {
        delete (HBufC8*)iData.iText.iPtr;
        iData.iText.iPtr = 0;
        }
    else if ( iType == EColumnHBuf16 )
        {
        delete (HBufC16*)iData.iText.iPtr;
        iData.iText.iPtr = 0;
        }
    }

inline void TColumn::Set( TInt32 aVal ) 
    {
    iType = EColumnInt32;
    iData.iInt32 = aVal;
    }

inline void TColumn::Set( TUint32 aVal ) 
    {
    iType = EColumnUint32;
    iData.iUint32 = aVal;
    }

inline void TColumn::Set( TBool aVal )
    {
    iType = EColumnBool;
    iData.iInt32 = aVal;
    }

inline void TColumn::Set( TInt64 aVal )
    {
    iType = EColumnInt64;
    iData.iInt64 = aVal;
    }

inline void TColumn::Set( TReal32 aVal ) 
    {
    iType = EColumnReal32;
    iData.iReal32 = aVal;
    }

inline void TColumn::Set( TReal64 aVal ) 
    {
    iType = EColumnReal64;
    iData.iReal64 = aVal;
    }

inline void TColumn::Set( TTime aVal )
    {
    iType = EColumnTime;
    iData.iInt64 = aVal.Int64();
    }

inline void TColumn::Set( const TDesC8& aVal )
    {
    iType = EColumnDes8;
    iData.iText.iPtr = (TAny*)aVal.Ptr();
    iData.iText.iLen = aVal.Length();
    }

inline void TColumn::Set( const TDesC16& aVal )
    {
    iType = EColumnDes16;
    iData.iText.iPtr = (TAny*)aVal.Ptr();
    iData.iText.iLen = aVal.Length();
    }

inline void TColumn::Set( const HBufC8* aVal )
    {
    iType = ( aVal ? EColumnHBuf8 : EColumnNullDes8 );
    iData.iText.iPtr = (TAny*)aVal;
    }

inline void TColumn::Set( const HBufC16* aVal )
    {
    iType = ( aVal ? EColumnHBuf16 : EColumnNullDes16 );
    iData.iText.iPtr = (TAny*)aVal;
    }

inline void TColumn::NotUsed()
    {
    iType = EColumnNotUsed;
    iData.iText.iPtr = 0;
    }


inline void TColumn::Get( TInt32& aValue ) const
    {
    if( EColumnNullDes16 == iType )
    	{
    	aValue = 0;
    	return;
    	}

    __ASSERT_DEBUG( iType == EColumnInt32, MMdCCommon::Panic( EDatabaseFault ) );
    aValue = iData.iInt32;
    }

inline void TColumn::Get( TUint32& aValue ) const
    {
    if( EColumnNullDes16 == iType )
    	{
    	aValue = 0;
    	return;
    	}

	__ASSERT_DEBUG( iType == EColumnUint32, MMdCCommon::Panic( EDatabaseFault ) );
    aValue = iData.iUint32;
    }

inline void TColumn::Get( TBool& aValue ) const 
    {
    if( EColumnNullDes16 == iType )
    	{
    	aValue = EFalse;
    	return;
    	}

	__ASSERT_DEBUG( iType == EColumnBool, MMdCCommon::Panic( EDatabaseFault ) );
    aValue = iData.iInt32;
    }

inline void TColumn::Get( TInt64& aValue ) const
    {
    if( EColumnNullDes16 == iType )
    	{
    	aValue = 0;
    	return;
    	}

	__ASSERT_DEBUG( iType == EColumnInt64, MMdCCommon::Panic( EDatabaseFault ) );
    aValue = iData.iInt64;
    }

inline void TColumn::Get( TReal32& aValue ) const
    {
    if( EColumnNullDes16 == iType )
    	{
    	aValue = 0.0f;
    	return;
    	}

	__ASSERT_DEBUG( iType == EColumnReal32, MMdCCommon::Panic( EDatabaseFault ) );
    aValue = iData.iReal32;
    }

inline void TColumn::Get( TReal64& aValue ) const
    {
    if( EColumnNullDes16 == iType )
    	{
    	aValue = 0.0f;
    	return;
    	}

	__ASSERT_DEBUG( iType == EColumnReal64, MMdCCommon::Panic( EDatabaseFault ) );
    aValue = iData.iReal64;
    }

inline void TColumn::Get( TTime& aValue ) const
    {
    if( EColumnNullDes16 == iType )
    	{
    	aValue = TTime( 0 );
    	return;
    	}
    
	__ASSERT_DEBUG( iType == EColumnTime, MMdCCommon::Panic( EDatabaseFault ) );
    aValue = TTime(iData.iInt64);
    }

inline void TColumn::Get( TPtrC8& aValue ) const
    {
    if ( iType == EColumnDes8 )
        {
        aValue.Set(
            (TUint8*)iData.iText.iPtr,
            iData.iText.iLen );
        }
    else if ( iType == EColumnHBuf8 )
        {
        aValue.Set(
            ((HBufC8*)iData.iText.iPtr)->Des() );
        }
    else if ( iType == EColumnNullDes8 ||
        iType == EColumnNullDes16 )
        {
        aValue.Set( KNullDesC8().Ptr(), 0 );
        }
    else MMdCCommon::Panic( EDatabaseFault );
    }

inline void TColumn::Get( TPtrC16& aValue ) const
    {
    if ( iType == EColumnDes16 )
        {
        aValue.Set(
            (TUint16*)iData.iText.iPtr,
            iData.iText.iLen );
        }
    else if ( iType == EColumnHBuf16 )
        {
        aValue.Set(
            ((HBufC16*)iData.iText.iPtr)->Des() );
        }
    else if ( iType == EColumnNullDes16 )
        {
        aValue.Set( KNullDesC16().Ptr(), 0 );
        }
    else MMdCCommon::Panic( EDatabaseFault );
    }

inline TBool TColumn::IsNull()
    {
    return ( iType == EColumnNullDes8 ||
        iType == EColumnNullDes16  ||
        ( iData.iText.iPtr == NULL &&
          ( iType == EColumnDes8 || iType == EColumnDes16 ||
            iType == EColumnHBuf8 || iType == EColumnHBuf16 ) ) );
    }

// -----------------------------------
// RRowData
// -----------------------------------


inline RRowData::RRowData()
    : iColumns( 16 )
    {
    }

inline void RRowData::Close()
    {
    Free();
    iColumns.Close();
    }

inline void RRowData::Free()
    {
    const TInt count( Size() );
    for ( TInt i=0; i<count; ++i )
        {
        iColumns[i].Free();
        }
    }

inline TInt RRowData::Size() const
    {
    return iColumns.Count();
    }

inline TColumn& RRowData::Column( TInt aIndex )
    {
    return iColumns[aIndex];
    }

inline const TColumn& RRowData::Column( TInt aIndex ) const
    {
    return iColumns[aIndex];
    }

inline TColumn& RRowData::AppendL( const TColumn& aColumn )
    {
    iColumns.AppendL( aColumn );
    return iColumns[ Size()-1 ];
    }

inline void RRowData::Reset()
    {
    iColumns.Reset();
    }

inline void RRowData::AppendColumnTypesL( RRowData& aColumnTypeRow )
	{
	Free();
	Reset();
	
	const TInt columnCount = aColumnTypeRow.Size();
	ReserveL( columnCount );

	for( TInt i = 0; i < columnCount; i++ )
		{
	    iColumns.Append( aColumnTypeRow.Column( i ).Type() ); 
		}
	}

inline void RRowData::ReserveL( TInt aColumnCount )
	{
	iColumns.ReserveL( aColumnCount );
	}
