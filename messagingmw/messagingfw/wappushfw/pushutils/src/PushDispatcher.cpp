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
// Local includes
// 
//

// System includes
//
#include "PushDispatcher.h"
#include <push/pluginkiller.h>
#include <push/cpushhandlerbase.h>
#include <push/ccontenthandlerbase.h>
#include <push/pushlog.h>

/** Loads the Wap Push Application or Content Handler Plugin.
 *
 * The plug-in returned is responsible for its own destruction.
 * This behaviour is provided using the plug-in killer, which deletes
 * the plug-in owner on request when the plugin is ready to kill itself.
 * 
 * @param aData Data identifying specific handlers
 * @param aType Uid of Wap Push App Handling Plugin
 * @return Plugin Owner
 * @leave EWapErrPluginNotFound (-10018) if the plug-in server was unable to
 * find the requested plug-in. 
 */
CPushHandlerBase* PushAppDispatcher::CreatePluginFromDataL(const TDesC& aData, const TUid& aType, MWapPushLog& aLog, MConnManObserver& aManager)
	{
	CPushHandlerBase* pushHandler = CPushHandlerBase::NewL(aData, aType);
	CleanupStack::PushL(pushHandler);

	CPluginKiller* pluginKiller = new (ELeave) CPluginKiller(pushHandler);
	CleanupStack::Pop(pushHandler);
	
	// Set some data
	pushHandler->SetKiller(*pluginKiller);
	pushHandler->SetLogger(aLog);
	pushHandler->SetManager(aManager);

	return pushHandler;
	}


/** Loads the Application Handler Plugin using App ID.
 *
 *  @param aAppId Uid for identifying specific Handlers in the App Plugin
 *  @param aLog Logging Utility Mixin class
 *  @param aManager Connection Manger Observer Mixin Class
 *  @return Reference to Push Handler Plugin Base
 */
EXPORT_C CPushHandlerBase& PushAppDispatcher::GetHandlerL(TInt aAppId, MWapPushLog& aLog, MConnManObserver& aManager) 
	{
	__LOG_PAR_DEBUG("PushAppDispatcher called");
	const TInt KAppIdLength =10;
	TBuf<KAppIdLength> appIdBuf;
	_LIT(KAppIdBufFormat, "0x%08x");
	appIdBuf.Format(KAppIdBufFormat, aAppId);

	CPushHandlerBase* pushHandler = CreatePluginFromDataL(appIdBuf, KUidPushHandlerBase, aLog, aManager);

	return *pushHandler;
	}


/** Loads the Application Handler Plugin using App URI.
 * 
 *  The AppURI string needs to be widened from 8 bit to 16 bit 
 *  Unicode text; this is done by the descriptor Copy() method.
 *
 *	@param aAppURI URI for identifying specific Handlers in the App Plugin
 * 	@param aLog	Logging Utility Mixin class
 *  @param aManager Connection Manger Observer Mixin Class
 *  @return Reference to the Push Handler Plugin Base
 */
EXPORT_C CPushHandlerBase& PushAppDispatcher::GetHandlerL(const TDesC8& aAppURI, MWapPushLog& aLog, MConnManObserver& aManager) 
	{
	__LOG_PAR_DEBUG("PushAppDispatcher called");
	HBufC16* applicationURI = HBufC16::NewL(aAppURI.Length());
	CleanupStack::PushL(applicationURI);
	applicationURI->Des().Copy(aAppURI);
	
	CPushHandlerBase* pushHandler = CreatePluginFromDataL(*applicationURI, KUidPushHandlerBase, aLog, aManager);
	CleanupStack::PopAndDestroy(applicationURI);

	return *pushHandler;
	}


/** Loads the Content-type Handler Plugin using content type string.
 *
 *	@param aContentType Content Type requested
 *  @param aLog	Logging Utility Mixin class
 *  @param aManager Connection Manger Observer Mixin Class
 *	@return Reference to the Push Handler Plugin Base
 */
EXPORT_C CContentHandlerBase& PushContentTypeDispatcher::GetHandlerL(const TDesC16& aContentType, MWapPushLog& aLog, MConnManObserver& aManager) 
	{
	__LOG_PAR_DEBUG("Push Content Type Dispatcher called")

	CPushHandlerBase* pushHandler = PushAppDispatcher::CreatePluginFromDataL(aContentType, 
																			 KUidPushContentHandlerBase,
																			 aLog,
																			 aManager);
	// This line was added instead of returning directly explicitly to remove a warning under armi
	CContentHandlerBase* handlerBase = REINTERPRET_CAST(CContentHandlerBase*, pushHandler);

	return *handlerBase;
	}
