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
* Description:  Class to hold description about one object and it's properties
*
*/

/**
 * How many properties have object
 * return properties.Count()
 */
inline TInt CMdsObjectDef::GetPropertiesCount() const
	{
	return iPropertyDefs.Count();
	}

inline void CMdsObjectDef::SetC2PStoredInDB()
	{
	SetFlag( EStoredFlagsC2P );
	}

inline TBool CMdsObjectDef::GetC2PStoredInDB() const
	{
	return GetFlag( EStoredFlagsC2P );
	}

/*
 * Gets parent of the object
 */
inline const CMdsObjectDef* CMdsObjectDef::GetParent() const
	{
	return iParent;
	}

/**
 * Get property column Id
 *
 * @param aPropertyNo number of property in property array
 * @return ColumnId
 */
inline const CMdsObjectDef::TMdsColumnOrder& CMdsObjectDef::GetPropertyColumnL( TInt aIndex ) const
	{
	if (aIndex < 0 || aIndex >= iCol2Prop.Count())
		{
		User::Leave( KErrNotFound );
		}
	return iCol2Prop[aIndex];
	}

inline void CMdsObjectDef::ResetFlags()
	{
	iFlags = EObjectDefFlagsNone;
	}

inline CMdsObjectDef::TObjectDefFlags CMdsObjectDef::GetFlags() const
	{
	return iFlags;
	}

inline void CMdsObjectDef::SetFlags(TObjectDefFlags aFlags)
	{
	iFlags = aFlags;
	}

