// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TSENDASCLIENTPANIC_H__
#define __TSENDASCLIENTPANIC_H__

/**
Defines the SendAs client panic codes.

@publishedAll
@released
*/
enum TSendAsClientPanic
	{
	/**
	Indicates an invalid request was made.
	*/
	ESendAsClientPanicBadRequest				= 0,

	/**
	Indicates a request was made on a session with an invalid handle.
	*/
	ESendAsClientPanicBadSubSessionHandle		= 1,
	
	/**
	Indicates a request was made with invalid an argument.
	*/
	ESendAsClientPanicBadRequestArgument		= 2,
	
	/**
	Indicates a request while the session is already in use.
	*/
	ESendAsClientPanicSubsessionInUse			= 3,
	
	/**
	Indicates a request relating to a message was made where no message exists.
	*/
	ESendAsClientPanicNoCurrentMessage			= 4,

	/**
	Indicates a request deleting a non-existent message was made.
	*/
	ESendAsClientPanicMessageAlreadyDeleted		= 5,
	
	/**
	Indicates a request to send a message encountered a problem.
	*/
	ESendAsClientPanicSendingMessage			= 6,

	/**
	Indicates a request to add or create an attachment encountered a problem.
	*/
	ESendAsClientPanicAddingCreatingAttachment  = 7

	};

#endif	//  __TSENDASCLIENTPANIC_H__
