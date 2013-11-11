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
// WAP Push Message Inline functions  File for the Utility class
// 
//

/**
 @file
 @publishedPartner
 @released
*/

_LIT(KPushSLContentTypeDesC, "text/vnd.wap.sl");


/** 
Gets the URL of the service to be loaded.

If there is no URL in the message, then a NULL descriptor will be returned.

@return 
URL of the service to be loaded 
*/
inline const TDesC& CSLPushMsgEntry::Url() const
	{
	if (iUrl)
		return *iUrl;
	else
		return KNullDesC;
	}


/**
Sets the time that the message was sent. 

@param aTime
The time that the message was sent.
*/
inline void	CSLPushMsgEntry::SetTimeSent(const TTime& aTime)
	{
	iTimeSent=aTime;
	}


/**
Gets the time that the message was sent. 

@return 
Time of sending
*/
inline const TTime& CSLPushMsgEntry::TimeSent() const 
	{
	return iTimeSent;
	}


/**
Gets the Content Type of the message. 

This content type is defined for both SL and SLC Push Messages. This is because the SL content 
handler deals with both types, decoding the WBXML versions prior to parsing, so there is 
effectively no difference.

@return 
Content type of the Push message.
*/
inline const TDesC& CSLPushMsgEntry::ContentType() const
	{
	// Generalise -group si & sic together!
	return KPushSLContentTypeDesC;
	}

