// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CWAPPUSHMSGUTILS_INL_
#define __CWAPPUSHMSGUTILS_INL_


/**
 * Returns a reference to the CMsvSession member of the class. 
 * Used by other objects when they need to pass a reference to another 
 * function, usually Push Entry class for Save or Retrieve. 
 * 
 * @return Reference to the iMsvSession class member. 
 */
inline CMsvSession& CWapPushMsgUtils::Session() const 
	{
	return *iMsvSession;
	}


/** 
 * Inherited function that must be implemented. 
 * Does not do anything as we do not need to respond to session events. 
 * If there are any problems, calls to the session will leave. Things will be 
 * tidied up then.
 * @param TMsvSessionEvent enumeration indicating event that has just occured
 * @param TAny* specific data
 */
inline void CWapPushMsgUtils::HandleSessionEventL(TMsvSessionEvent , TAny* , TAny* , TAny* )
	{
	}



/**
 * Static function that determines whether an entry has been flagged 
 * as Deleted. Hides the implementation of the deletion mechanism, and 
 * avoids other classes having to perform bitwise operations.
 * 
 * @param aEntry Reference to an entry in the message server index
 * @return TBool ETrue if the entry status is set to Deleted, otherwise EFalse.
 */
inline TBool CWapPushMsgUtils::IsDeleted(const TMsvEntry& aEntry)
	{
	// Must always be a Push Msg Entry, otherwise operation is pointless
	__ASSERT_ALWAYS(aEntry.iMtm == KUidMtmWapPush, 
				User::Panic(KMsgUtilsPanicTitle,EPushMsgUtilsNotValidPushMsg));
	return ( (aEntry.MtmData1() & KPushMaskOnlyStatus) 
					== CPushMsgEntryBase::EPushMsgStatusDeleted);
	}


#endif