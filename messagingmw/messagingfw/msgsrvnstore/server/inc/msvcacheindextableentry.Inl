// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 
 IsInRange()
 @param TMsvId : TMsvId of the entry
 @return TBool : ETrue if the TMsvId is in the range of the block, EFalse otherwise
 
 The function checks if the passed TMsvId falls in the range of the block.
 
*/
inline TBool CMsvCacheIndexTableEntry::IsInRange(TMsvId aId) const
	{
	if (aId >= iMinMsvId && aId <= iMaxMsvId)
		{
		return(ETrue);
		}
	else 
		{
		return(EFalse);
		}
	}






/**
 * IsDirty()
 * @param None
 * @return TBool : status of the dirty flag
 * 
 * The function return ETrue if the dirty flag is set, EFalse if otherwise.
 */
inline TBool CMsvCacheIndexTableEntry::IsDirty() const
	{
	if(iFlags & EMsvCacheIndexTableSetDirtyFlag)
		{
		return(ETrue);
		}
	else 
		{
		return (EFalse);
		}
	}






/**
 * SetDirty()
 * @param TBool: set or reset
 * @return None.
 *
 * The function sets or resets the dirty flag depending on the aFlag variable
 * passed.
 */
inline void CMsvCacheIndexTableEntry::SetDirty()
	{
	iFlags |= EMsvCacheIndexTableSetDirtyFlag;
	}





/**
 * ClearDirty()
 * @param TBool: set or reset
 * @return None.
 *
 * The function sets or resets the dirty flag depending on the aFlag variable
 * passed.
 */
inline void CMsvCacheIndexTableEntry::ClearDirty()
	{
	iFlags &= EMsvCacheIndexTableClearDirtyFlag;
	}
		
		
		
		
/**
 * SetMinMsvIdRange()
 * @param TMsvId : the TMsvId representing the lower part of the range
 * @return None.
 * 
 * The function sets the iMinMsvId to the TMsvId passed.
 */
inline void CMsvCacheIndexTableEntry::SetMinMsvIdRange(TMsvId aId)
	{
	iMinMsvId = aId;
	}






/**
 * SetMaxMsvIdRange()
 * @param TMsvId : the TMsvId representing the upper part of the range
 * @return None.
 * 
 * The function sets the iMaxMsvId to the TMsvId passed.
 */
inline void CMsvCacheIndexTableEntry::SetMaxMsvIdRange(TMsvId aId)
	{
	iMaxMsvId = aId;
	}






/**
 * GetMinMsvIdRange()
 * @param None.
 * @return TMsvId : the lower part of the block's range
 * 
 * The function returns the iMinMsvId to the TMsvId passed.
 */
inline TMsvId CMsvCacheIndexTableEntry::GetMinMsvIdRange() const
	{
	return(iMinMsvId);
	}






/**
 * GetMaxMsvIdRange()
 * @param None.
 * @return TMsvId : the upper part of the block's range
 * 
 * The function returns the iMaxMsvId to the TMsvId passed.
 */
inline TMsvId CMsvCacheIndexTableEntry::GetMaxMsvIdRange() const
	{
	return(iMaxMsvId);
	}






/**
 * Size()
 * @param None.
 * @return TInt : the number of entries currently in the block
 * 
 * The function returns the number of entries currently present in the block.
 */
inline TInt CMsvCacheIndexTableEntry::Size() const
	{
	if(iBlockPtr == NULL)
		{
		return 0;
		}
	else
		{
		return(iBlockPtr->Count());
		}
	}






/**
 * ClearFlags()
 * @param None.
 * @return None.
 * 
 * The function clears all the flags.
 */
inline void CMsvCacheIndexTableEntry::ClearFlags()
	{
	iFlags &= 0X0000;
	}






/**
 * SetGrandChildPresent()
 * @param TBool : set or reset
 * @return None.
 * 
 * The function sets or resets the flag depending on whether grandchildren entries are present in the block.
 */
inline void CMsvCacheIndexTableEntry::SetGrandChildPresent()
	{
	iFlags |= EMsvCacheIndexTableEntrySetGrandChildPresent;
	}





/**
 * SetGrandChildPresent()
 * @param TBool : set or reset
 * @return None.
 * 
 * The function sets or resets the flag depending on whether grandchildren entries are present in the block.
 */
inline void CMsvCacheIndexTableEntry::ClearGrandChildPresent()
	{
	iFlags &= EMsvCacheIndexTableEntryClearGrandChildPresent;
	}





/**
 * IsGrandChildPresent()
 * @param None.
 * @return TBool : value of the EMsvCacheIndexTableEntrySetGrandChildPresent flag
 * 
 * The function returns ETrue if there are grandchildren present in the block, EFalse if otherwise.
 */
inline TBool CMsvCacheIndexTableEntry::IsGrandChildPresent() const
	{
	if(iFlags & EMsvCacheIndexTableEntrySetGrandChildPresent)
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}




/**
 * BlockPtr()
 * @param None.
 * @return RPointerArray<CMsvCacheEntry>* : handle to the block.
 * 
 * The function returns a handle to the block.
 */
inline RPointerArray<CMsvCacheEntry>* CMsvCacheIndexTableEntry::BlockPtr()
	{
	return iBlockPtr;
	}




/**
 * AccessTime()
 * @param None.
 * @return TTime : the last accessed time of the block.
 * 
 * The function the last accessed time of the block.
 */
inline const TTime CMsvCacheIndexTableEntry::AccessTime() const
	{
	return iTimeStamp;
	}



/**
 * SetAccessTime()
 * @param TTime : New time.
 * @return void.
 *
 * The function set the new timestamp for the block.
 */
inline void CMsvCacheIndexTableEntry::SetAccessTime(TTime aNewTime)
	{
	iTimeStamp = aNewTime;
	}



