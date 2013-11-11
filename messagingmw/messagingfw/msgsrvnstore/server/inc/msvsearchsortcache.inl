// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Contain all inline functions of serach sort cache.
// 
//

/**
 *******************************
 SetSortQuery() :This flag is set only for Sort query. 
 If client query is a Sort Query , it is 1 else (for search) query it is 0.
 @param None
 @return  None
 @internalComponent
 @released
 ********************************
*/
inline void CMsvSearchSortCacheEntry::DoSortQuery()
	{
	iQueryOptions |= ESortQuery;	
	}

/***************************
 * SetCaseSensetive() : This flag is set only for CaseSensetive option. 
 * @param None
 * @return  None
 * @internalComponent
 * @released
 ***************************/ 
inline void CMsvSearchSortCacheEntry::DoCaseSensetive()
	{
	iQueryOptions |= ECaseSensitive;
	} 
/***************************
 * SetWholeWord(): This flag is set only for Whole Word option.
 * @param None
 * @return  None
 * @internalComponent
 * @released
 *************************/ 
inline void CMsvSearchSortCacheEntry::DoWholeWord()
	{
	iQueryOptions |= EWholeWord;
	} 
/***************************
 * SetAscendingSort() :For Ascending Sort option bit is set. 
 * @param None
 * @return  None
 * @internalComponent
 * @released
***************************/ 
inline void CMsvSearchSortCacheEntry::DoAscendingSort()
	{
	iQueryOptions |= EAscendingSort;
	}
	
/************************
* SetWildCharacter() : This flag is set only for KWildCharacter option.
* @param None
* @return  None
* @internalComponent
* @released
***************************/
inline void CMsvSearchSortCacheEntry::DoWildCharacter() //set Wild Character option .
	{
	iQueryOptions |= EWildCharacter;
	}	

/************************************
* SetExplicitSortReq() : This flag is set only for ExplicitSortReq option.
* @param None
* @return  None
* @internalComponent
* @released
***********************************/
inline void CMsvSearchSortCacheEntry::DoExplicitSortReq() // Set the flag for Explicit sorting .
	{
	iQueryOptions |= EExplicitSort;
	} 
/**********************************
* IsSortQuery() : Return TRUE if it is a Sort Query else FALSE.
* @param None
* @return  TRUE or FALSE
* @internalComponent
* @released
**********************************/
inline TBool CMsvSearchSortCacheEntry::IsSortQuery() const 
	{
	return (iQueryOptions & ESortQuery);	
	}

/**********************************
* IsCaseSensetive() : Return TRUE if CaseSensetive option is set else FALSE.
* @param None
* @return  TRUE or FALSE
* @internalComponent
* @released
**********************************/
inline TBool CMsvSearchSortCacheEntry::IsCaseSensetive() const
	{
	return (iQueryOptions & ECaseSensitive );
	} 
/**********************************
* IsWholeWord() : Return TRUE if KWholeWord option is set else FALSE.
* @param None
* @return  TRUE or FALSE
* @internalComponent
* @released
**********************************/
inline TBool CMsvSearchSortCacheEntry::IsWholeWord() const
	{
	return (iQueryOptions & EWholeWord);
	}
/**********************************
* IsAscendingSort() : Return TRUE if EAscendingSort is set else FALSE.
* @param None
* @return  TRUE or FALSE
* @internalComponent
* @released
**********************************/
inline TBool CMsvSearchSortCacheEntry::IsAscendingSort() const
	{
	return (iQueryOptions & EAscendingSort);
	} 
/**********************************
* IsWildCharacter() : Return TRUE if EWildCharacter is set else FALSE.
* @param None
* @return  TRUE or FALSE
* @internalComponent
* @released
**********************************/
inline TBool CMsvSearchSortCacheEntry::IsWildCharacter() const //set Wild Character option .
	{
	return (iQueryOptions & EWildCharacter);
	}	 
/**********************************
* IsExplicitSortReq() : Return TRUE if EExplicitSort is set else FALSE.
* @param None
* @return  TRUE or FALSE
* @internalComponent
* @released
**********************************/
inline TBool CMsvSearchSortCacheEntry::IsExplicitSortReq() const // Set the flag for Explicit sorting .
	{
	return (iQueryOptions & EExplicitSort);
	} 
/*******************************
 * GetResultType() : Return Result Type.
 * @param None.
 * @return TSearchSortResultType& : Return Search Sort result type as reference.
 * @internalComponent
 * @released
 * ******************************/
 TMsvSearchSortResultType& CMsvSearchSortCacheEntry::GetResultType()
 	{
 	return iResultType;
 	}
 	
TMsvId& CMsvSearchSortCacheEntry::ReturnParentId()
	{
	return iParentId;
	}

