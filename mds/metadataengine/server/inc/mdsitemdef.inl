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
* Description:  Class to hold description about one item definition
*
*/


/**
 * Get object name
 * @return object name
 */
inline const TDesC& CMdsItemDef::GetName() const
	{
	return *iName;
	}

/**
 * Get object Id
 * @return object Id
 */
inline TDefId CMdsItemDef::GetId() const
	{
	return iId;
	}

inline void CMdsItemDef::SetId(TDefId aId)
	{
	iId = aId;
	}	

inline void CMdsItemDef::SetFlag( TStoredFlags aFlag )
	{
	iStoredFlags |= aFlag;
	}

inline TBool CMdsItemDef::GetFlag( TStoredFlags aFlag ) const
	{
	return iStoredFlags & aFlag;
	}

inline void CMdsItemDef::SetStoredInDB()
	{
	iStoredFlags |= EStoredFlagsDB;
	}

inline void CMdsItemDef::SetAllNotStoredInDB()
	{
	iStoredFlags = EStoredFlagsNone;
	}

inline void CMdsItemDef::SetTableStoredInDB()
	{
	iStoredFlags |= EStoredFlagsTable;
	}

inline TBool CMdsItemDef::GetStoredInDB() const
	{
	return iStoredFlags & EStoredFlagsDB;
	}

inline TBool CMdsItemDef::GetTableStoredInDB() const
	{
	return iStoredFlags & EStoredFlagsTable;
	}

inline void CMdsItemDef::CheckAlphabetL( TUint16 aChar )
	{	
	if( !( ( 0x0041 <= aChar && aChar <= 0x005a ) || 
		   ( 0x0061 <= aChar && aChar <= 0x007a ) ) )
		{
		User::Leave( KErrGeneral );
		}
	}

inline void CMdsItemDef::CheckAlphabetOrNumberL( TUint16 aChar )
	{
	if( !( ( 0x0041 <= aChar && aChar <= 0x005a ) || 
		   ( 0x0061 <= aChar && aChar <= 0x007a ) || 
		   ( 0x0030 <= aChar && aChar <= 0x0039 ) ) )
		{
		User::Leave( KErrGeneral );
		}	
	}
