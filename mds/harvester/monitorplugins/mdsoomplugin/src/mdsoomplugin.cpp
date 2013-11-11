/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Low memory observer for havester server
*
*/


#include "mdsoomplugin.h"
#include "harvesterlog.h"
#include "harvestercommon.h"


CMdSOomPlugin::CMdSOomPlugin()
	{
	WRITELOG("CMdSOomPlugin::CMdSOomPlugin()");
	}


CMdSOomPlugin::~CMdSOomPlugin()
	{
	iOomMsgQueue.Close();
	}

CMdSOomPlugin* CMdSOomPlugin::NewL()
	{
	CMdSOomPlugin* self = new (ELeave) CMdSOomPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMdSOomPlugin::ConstructL()
	{
	COomMonitorPlugin::ConstructL();
	
	User::LeaveIfError( iOomMsgQueue.CreateGlobal(KHarvesterOomQueue, 32) );
	}


void CMdSOomPlugin::FreeRam( TInt /*aBytesToFree*/ )
	{
	WRITELOG("CMdSOomPlugin::FreeRam() - start");
	
	iOomMsgQueue.Send( EMdsOomFreeRam );
	
	WRITELOG("CMdSOomPlugin::FreeRam() - end");
	}

void CMdSOomPlugin::MemoryGood()
	{
	WRITELOG("CMdSOomPlugin::MemoryGood - start");
	
	iOomMsgQueue.Send( EMdsOomMemoryGood );	
	
	WRITELOG("CMdSOomPlugin::MemoryGood - end ");
	}

void CMdSOomPlugin::ExtensionInterface(TUid aInterfaceId, TAny*& aImplementation)
	{
	COomMonitorPlugin::ExtensionInterface(aInterfaceId, aImplementation);
	}


