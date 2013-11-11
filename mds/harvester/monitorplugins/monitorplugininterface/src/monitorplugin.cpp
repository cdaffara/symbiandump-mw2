/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class for monitor plug-ins.*
*/


#include "monitorplugin.h"

EXPORT_C CMonitorPlugin* CMonitorPlugin::NewL(const TUid& aUid)
	{
	TAny* monitorPlugin = REComSession::CreateImplementationL(
			aUid, _FOFF( CMonitorPlugin, iDtor_ID_Key ) );
	return (reinterpret_cast<CMonitorPlugin*>(monitorPlugin));
	}

EXPORT_C void CMonitorPlugin::ListImplementationsL(RImplInfoPtrArray& aImplInfoArray)
	{
	REComSession::ListImplementationsL(
			KMonitorPluginInterfaceUid, aImplInfoArray );
	}
	
// Destructor
EXPORT_C CMonitorPlugin::~CMonitorPlugin()
	{
	REComSession::DestroyedImplementation( iDtor_ID_Key );
	}

GLDEF_C TInt E32Dll()
	{
	return(KErrNone);
	}

