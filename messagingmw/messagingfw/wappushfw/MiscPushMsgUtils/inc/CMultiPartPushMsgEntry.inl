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

If Content Type does not exist, a null descriptor is returned.

@return 
The Content Type of the message.
*/
inline const TDesC& CMultiPartPushMsgEntry::ContentType() const
	{
	if (iContentType)
		return *iContentType;
	else
		return KNullDesC;
	}


/**
Sets the time that the message was sent.

@param aTime
The time that the message was sent.
*/
inline void	 CMultiPartPushMsgEntry::SetTimeSent(const TTime& aTime)
	{
	iTimeSent=aTime;
	}


/**
Gets the time that the message was sent.

@return 
The time that the message was sent
*/
inline const TTime& CMultiPartPushMsgEntry::TimeSent() const 
	{
	return iTimeSent;
	}

