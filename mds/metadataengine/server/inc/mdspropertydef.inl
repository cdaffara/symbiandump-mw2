/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class to hold description about one property
*
*/


inline TBool CMdsPropertyDef::CheckMinMaxValue( TInt32 aValue ) const
	{
	return ( aValue >= iMinValue.iInt32 && aValue <= iMaxValue.iInt32 );
	}

inline TBool CMdsPropertyDef::CheckMinMaxValue( TUint32 aValue ) const
	{
	return ( aValue >= iMinValue.iUint32 && aValue <= iMaxValue.iUint32 );
	}

inline TBool CMdsPropertyDef::CheckMinMaxValue( const TInt64& aValue ) const
	{
	return ( aValue >= iMinValue.iInt64 && aValue <= iMaxValue.iInt64 );
	}

inline TBool CMdsPropertyDef::CheckMinMaxValue( const TReal& aValue ) const
	{
	return ( aValue >= iMinValue.iReal && aValue <= iMaxValue.iReal );
	}

/**
 * Get property type
 * @return property type
 */
inline TPropertyType CMdsPropertyDef::GetType() const
	{
	return iType;
	}
	
/**
 * Get property read-only
 * @return property read-only
 */
inline TBool CMdsPropertyDef::GetReadOnly() const
	{
	return iPropertyFlags & EPropertyReadOnly;
	}

/**
 * Get property mandatory
 * @return is property mandatory
 */
inline TBool CMdsPropertyDef::GetMandatory() const
	{
	return iPropertyFlags & EPropertyMandatory;
	}

/**
 * Get property indexed flag
 * @return is property indexed
 */
inline TBool CMdsPropertyDef::GetIndexed() const
	{
	return iIndexed;
	}

inline TBool CMdsPropertyDef::operator!=( const CMdsPropertyDef& aPropertyDef ) const
	{
	return !( *this == aPropertyDef );
	}
