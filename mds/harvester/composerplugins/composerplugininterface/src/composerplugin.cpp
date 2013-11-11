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
* Description:  Composer Plug-in ECom interface
*
*/


#include "composerplugin.h"

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
EXPORT_C CComposerPlugin* CComposerPlugin::NewL( const TUid& aUid )
	{
	TAny* composerPlugin = REComSession::CreateImplementationL(
			aUid, _FOFF( CComposerPlugin, iDtor_ID_Key ) );
	return ( reinterpret_cast<CComposerPlugin*>(composerPlugin) );
	}

// ---------------------------------------------------------------------------
// ListImplementationsL
// ---------------------------------------------------------------------------
//
EXPORT_C void CComposerPlugin::ListImplementationsL( RImplInfoPtrArray& aImplInfoArray )
	{
	REComSession::ListImplementationsL(
			KCComposerPluginInterfaceUid, aImplInfoArray );
	}

// ---------------------------------------------------------------------------
// Destructor, virtual
// ---------------------------------------------------------------------------
//
EXPORT_C CComposerPlugin::~CComposerPlugin()
	{
	REComSession::DestroyedImplementation( iDtor_ID_Key );
	}

// ---------------------------------------------------------------------------
// SetSession
// ---------------------------------------------------------------------------
//
EXPORT_C void CComposerPlugin::SetSession( CMdESession& aSession )
	{
	iSession = &aSession;
	
	SetObservers();
	}

// ---------------------------------------------------------------------------
// RemoveSession
// ---------------------------------------------------------------------------
//
EXPORT_C void CComposerPlugin::RemoveSession()
	{
	RemoveObservers();
	
	iSession = NULL;
	}

// ---------------------------------------------------------------------------
// E32Dll
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Dll()
	{
	return KErrNone;
	}
