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
// This file contains the declaration of the PushAppDispatcher which Handles the push message using the
// extracted app ID. Other class is PushContentTypeDispatcher for creating appropiate handler to handle the push message
// 
//

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __PUSHDISPATCHER_H__
#define __PUSHDISPATCHER_H__

#include <e32std.h>

//Forward declarations
class CPluginOwner;
class CPluginKiller;
class MWapPushLog;
class MConnManObserver;
class CPushHandlerBase;
class CContentHandlerBase;

/** Handle for default user agent. */
const TInt KUserAgentAppHandler = 0;

/** Creates an appropriate App Handler for the given AppId or AppURI.
	
	Looks up the application Id passed in and uses the WAP Plugin Server
	to determine the correct handler which is then created. If there is no
	recognised application handler to deal with the Message App Type then the
	default behaviour is to create an Unknown AppHandler to process message.

	@param aAppId Application ID extracted from Push Message
	@param aLog Reference to logging object 
	@param aManager Reference to Connection Manager Observer Mixin class
	@param aAppURI Application URI used instead of Application UID

	@return Reference to the application plugin handler base class 
*/
//##ModelId=3B659EE80221
class PushAppDispatcher
/**
@publishedPartner
@released
*/
	{
public:
	//##ModelId=3B659E6E0145
	IMPORT_C static CPushHandlerBase& GetHandlerL(TInt aAppId, MWapPushLog& aLog, MConnManObserver& aManager);
	//##ModelId=3B659E6E014D
	IMPORT_C static CPushHandlerBase& GetHandlerL(const TDesC8& aAppURI, MWapPushLog& aLog, MConnManObserver& aManager);

private:
	//##ModelId=3B659E6E0140
	static CPushHandlerBase* CreatePluginFromDataL(const TDesC& aData, const TUid& aType, MWapPushLog& aLog, MConnManObserver& aManager);

	friend class PushContentTypeDispatcher;
	};


/** Creates an appropriate Content Type handler for the given CPushMessage.

	The content dispatcher uses the WAP plugin server to determine the correct handler
	given the Content Type retrieved from the CPushMessage. It will then instantiate
	an instance of the correct handler and pass back a pointer to this content
	handler. If the Content Type is not recognised and no appropriate handler
	exists, a Unknown Content Handler will be created to deal with the message.

	@param aContentType Content type extracted from Push Message
	@param aLog Reference to logging object 
	@param aManager Reference to Connection Manager Observer Mixin class

	@return  Reference to the content plugin handler base class 
*/
//##ModelId=3B659EE902E1
class PushContentTypeDispatcher 
/**
@publishedPartner
@released
*/
	{
public:
	//##ModelId=3B659E6E030C
	IMPORT_C static CContentHandlerBase& GetHandlerL(const TDesC16& aContentType, MWapPushLog& aLog, MConnManObserver& aManager);
	};


#endif
