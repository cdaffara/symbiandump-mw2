// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CSIContentHandler..inl
// WAP Push Message Inline functions file for the Handler classes
// attribute methods
// 
//

/**
 
 Flag whether message has an action attribute
 @param aAction
 boolean to set flag to - ETrue for action attrib present
 EFalse for no action attrib
 
*/
inline void CSIContentHandler::SetActionFlag(TBool aAction)      
	{
	if (aAction)
		iAttributes |= EAction;
	else
		iAttributes &=~ EAction;
	}

/**
 *	Check status of action flag to see if message has an action attribute.
 *	@return TBool
 *		ETrue - action attribute present in message
 *		EFalse - no action attribute
 */
inline TBool CSIContentHandler::ActionFlag() const
	{
	return iAttributes & EAction;
	}

/** 
 *	Flag whether message has a created date attribute
 *	@param aCreated
 *		boolean to set flag to - ETrue for si-created attrib present
 *								 EFalse for no si-created attrib
 */ 
inline void CSIContentHandler::SetCreatedFlag(TBool aCreated)      
	{
	if (aCreated)
		iAttributes |= ECreated;
	else
		iAttributes &=~ ECreated;
	}

/**
 *	Check status of created flag to see if message has a si-created attribute.
 *	@return TBool
 *		ETrue - si-created attribute present in message
 *		EFalse - no si-created attribute
 */
inline TBool CSIContentHandler::CreatedFlag() const
	{
	return iAttributes & ECreated;
	}

/** 
 *	Flag whether message has a Href attribute
 *	@param aHref
 *		boolean to set flag to - ETrue for Href attrib present
 *								 EFalse for no href attrib
 */ 
inline void CSIContentHandler::SetHrefFlag(TBool aHref)      
	{
	if (aHref)
		iAttributes |= EHref;
	else
		iAttributes &=~ EHref;
	}


/**
 *	Check status of href flag to see if message has an href attribute.
 *	@return TBool
 *		ETrue - href attribute present in message
 *		EFalse - no href attribute
 */
inline TBool CSIContentHandler::HrefFlag() const
	{
	return iAttributes & EHref;
	}

/** 
 *	Flag whether message has an expiry date attribute (si-expires)
 *	@param aExpires
 *		boolean to set flag to - ETrue for si-expires present
 *								 EFalse for no si-expires date
 */ 
inline void CSIContentHandler::SetExpiresFlag(TBool aExpires)      
	{
	if (aExpires)
		iAttributes |= EExpires;
	else
		iAttributes &=~ EExpires;
	}

/**
 *	Check status of flag to see if message has an expiry date attribute.
 *	@return TBool
 *		ETrue - si-expires attribute present in message
 *		EFalse - no si-expires attribute
 */
inline TBool CSIContentHandler::ExpiresFlag() const
	{
	return iAttributes & EExpires;
	}

/** 
 *	Flag whether message has a si-id attribute
 *	@param aSiId
 *		boolean to set flag to - ETrue for si-id attrib present
 *								 EFalse for no si-id attrib
 */ 
inline void CSIContentHandler::SetSiIdFlag(TBool aSiId)      
	{
	if (aSiId)
		iAttributes |= ESiId;
	else
		iAttributes &=~ ESiId;
	}

/**
 *	Check status of flag to see if message has an si-id attribute.
 *	@return TBool
 *		ETrue - si-id attribute present in message
 *		EFalse - no si-id attribute
 */
inline TBool CSIContentHandler::SiIdFlag() const
	{
	return iAttributes & ESiId;
	}

/** 
 *	Flag whether message has a Data attribute (text)
 *	@param aData
 *		boolean to set flag to - ETrue for data attrib present
 *								 EFalse for no data attrib
 */ 
inline void CSIContentHandler::SetDataFlag(TBool aData)      
	{
	if (aData)
		iAttributes |= EData;
	else
		iAttributes &=~ EData;
	}

/**
 *	Check status of data flag to see if message has any data.
 *	@return TBool
 *		ETrue - data attribute present in message
 *		EFalse - no data attribute
 */
inline TBool CSIContentHandler::DataFlag() const
	{
	return iAttributes & EData;
	}

