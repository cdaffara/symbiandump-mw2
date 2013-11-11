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
//

#include <push/pluginkiller.h>
#include <push/cpushhandlerbase.h>


/**
Constructor.

@param aPushPlugin The plugin owner to be deleted

@internalComponent
*/
CPluginKiller::CPluginKiller(CPushHandlerBase* aPushPlugin)
: CActive(EPriorityStandard), iPushPlugin(aPushPlugin), iDeletePushPlugin(EFalse)
	{
	CActiveScheduler::Add(this);
	}


/** 
Destructor. 
*/
CPluginKiller::~CPluginKiller()
	{
	// Responsible for Deleting Plugin Owners.
	Cancel();
	__ASSERT_DEBUG(iDeletePushPlugin, User::Invariant());
	delete iPushPlugin;
	}


/** 
Deletes the plug-in.
*/
EXPORT_C void CPluginKiller::KillPushPlugin()
	{
	//Starts a completed asynchronous request to delete the plug-in.
	//When RunL() runs, it deletes the plug-in and the plug-in killer object itself. 

	TRequestStatus* pS=&iStatus;
	User::RequestComplete(pS,KErrNone);
	SetActive();
	}

/**
Cancel the request 
*/
void CPluginKiller::DoCancel()
	{
	}


/**
Handles the completion of an asynchronous request. RunL() will only be called by the plug-in
killer completing its own request in KillPushPlugin(). This method sets a flag to indicate
that it is safe to delete the plugin owner in the destructor. This ensures that the plug-in
killer object is only deleted if the push plugin is deleted.
*/
void CPluginKiller::RunL()
	{
	iDeletePushPlugin = ETrue;
	delete this;
	}
