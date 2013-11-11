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

_LIT(KPushSIContentTypeDesC, "text/vnd.wap.si");


/** 
Gets the SI ID.

As a server can send many different versions of the same SI Push Message, only the 
latest of which is valid. The ID allows the UI and WAP Push architecture to locate any 
previous instances of the message.

If SI ID does not exist, a null descriptor is returned.

@return 
SI ID of the SI message.
*/
inline const TDesC& CSIPushMsgEntry::Id() const
	{
	if (iMsgID)
		return *iMsgID;
	else
		return KNullDesC;
	}


/** 
Gets the time that the Push Message was created.

@return 
Creation time.
*/
inline const TTime& CSIPushMsgEntry::Created() const
	{
	return iCreatedDate;
	}


/** 
Sets the time that the Push Message was created.

@param aCreated
Creation time.
*/
inline void CSIPushMsgEntry::SetCreated(const TTime& aCreated)
	{
	iCreatedDate=aCreated;
	}


/** 
Gets the expiry date and time for the Push Message. 

The value is set by either SetExpires(), or by RetrieveL() on a previously 
saved message entry.

@return 
Expiry date and time.
*/
inline const TTime& CSIPushMsgEntry:: Expires() const 
	{
	return iExpiryDate;
	}
 

/** 
Sets the Expires time. 

@param aExpires 
Expiry date and time of the SI message. 
*/
inline void CSIPushMsgEntry::SetExpires(const TTime& aExpires)
	{
	iExpiryDate=aExpires;
	}


/** 
Gets the URL of the service to be accessed.

If there is no URL in the message, then a NULL descriptor will be returned.

@return 
URL of the service to be accessed. 
*/
inline const TDesC& CSIPushMsgEntry::Url() const
	{
	if (iUrl)
		return *iUrl;
	else
		return KNullDesC;
	}


/**
Gets the Content Type of the message. 

This content type is defined for both SI and SIC Push Messages. This is because the SI content 
handler deals with both types, decoding the WBXML versions prior to parsing, so there is 
effectively no difference.

@return 
The Content Type of the message.
*/
inline const TDesC& CSIPushMsgEntry::ContentType() const
	{
	// Generalise -group si & sic together!
	return KPushSIContentTypeDesC;
	}




