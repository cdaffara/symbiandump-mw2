// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// WAP Push Message Inline functions file for the Utility class
// 
//

/**
 @file
 @publishedPartner
 @released
 
 Gets the transmission date for Push Messages other than SI. 
 
 For SI message the user should call CSISLPushMsgUtils::GetCreationDateL() as this function panics 
 if the message type is SI.
 
 @param	aId 
 The ID of the message to get Transmission Date.
 
 @return 
 Date of transmission of message.
 
 @leave	KErrNotFound Message Id is not valid and entry does not exist.
 @leave	CMsvEntry::SetEntryL
 @leave	CMsvEntry::HasStoreL
 
 @panic	CSISLPushMsgUtils 0
 If the message type is SI.
*/
inline TTime CSISLPushMsgUtils::GetTransmissionDateL(TMsvId aId)
	{
	// Must be a Push message, shouldn't be an SI message - use GetCreationDate
	__ASSERT_ALWAYS( (iMsvEntry->Entry().iMtm == KUidMtmWapPush)
					&& iMsvEntry->Entry().iBioType == KUidWapPushMsgSL.iUid, 
						User::Panic(KSISLPushMsgUtilsPanicTitle,ESISLPushMsgUtilsNotValidPushMsg));
	return GetDateL(aId, KUidWapPushMsgSL);
	}


/**
Gets the Creation date for SI Push Messages.

For any other types of message the user should call CSISLPushMsgUtils::GetTransmissionDateL() 
as this function panics if the message type is not SI.
 
@param	aId 
The ID of the message to get Creation Date.

@return 
Date of creation of message.

@leave	KErrNotFound Message Id is not valid and entry does not exist.
@leave	CMsvEntry::SetEntryL
@leave	CMsvEntry::HasStoreL

@panic	CSISLPushMsgUtils 1
If the message type is not SI.
*/ 
inline TTime CSISLPushMsgUtils::GetCreationDateL(TMsvId aId)
	{
	__ASSERT_ALWAYS(iMsvEntry->Entry().iBioType == KUidWapPushMsgSI.iUid, 
						User::Panic(KSISLPushMsgUtilsPanicTitle,ESISLPushMsgUtilsNotSiMsg));
	return GetDateL(aId, KUidWapPushMsgSI);
	}
