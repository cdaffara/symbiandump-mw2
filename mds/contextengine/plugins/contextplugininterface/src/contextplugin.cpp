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
* Description:  
*
*/


#include <ecom.h>
#include "contextplugin.h"
	
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CContextPlugin* CContextPlugin::NewL( const TUid& aUid )
	{
	TAny* contextPlugin = REComSession::CreateImplementationL(
		aUid, _FOFF( CContextPlugin, iDtor_ID_Key ) );
	return ( reinterpret_cast<CContextPlugin*>(contextPlugin) );
	}

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CContextPlugin::ListImplementationsL( RImplInfoPtrArray& aImplInfoArray )
	{
	REComSession::ListImplementationsL( KCContextPluginInterfaceUid, aImplInfoArray );
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CContextPlugin::~CContextPlugin()
	{
	// Destroy any instance variables and then
	// inform the framework that this specific
	// instance of the interface has been destroyed.
	REComSession::DestroyedImplementation( iDtor_ID_Key );
	}

// ---------------------------------------------------------------------------
// Main function of the DLL.
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Dll()
	{
	return( KErrNone );
	}
