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
//

/**
 @file
 @publishedPartner
 @released
*/

#include <msvapi.h>


/** 
Gets the Content Type of the message.

@return 
The Content Type of the message. 
*/
inline const TDesC& CUnknownPushMsgEntry::ContentType() const
	{
	if (iContentType)
		return *iContentType;
	else
		return KNullDesC;
	}


/**
Sets the time that the message was sent. 

@param aTime
The time the message was sent.
*/
inline void	CUnknownPushMsgEntry::SetTimeSent(const TTime& aTime)
	{
	iTimeSent=aTime;
	}


/**
Gets the time that the message was sent. 

@return 
The time the message was sent
*/
inline const TTime& CUnknownPushMsgEntry::TimeSent()  const
	{
	return iTimeSent;
	}
