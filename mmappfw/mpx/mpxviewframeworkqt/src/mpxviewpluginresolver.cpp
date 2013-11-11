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
* Description: 
*
*/

#include "mpxviewpluginresolver.h"	// MpxViewPluginResolver
#include "mpxviewpluginresolver_p.h"  // MpxViewPluginResolverPrivate

// -----------------------------------------------------------------------------
// MpxViewPluginResolver()
// -----------------------------------------------------------------------------
//
MpxViewPluginResolver::MpxViewPluginResolver( const QList<int>& requestedPlugins )
:
impl ( new MpxViewPluginResolverPrivate( requestedPlugins ) )
{}

// -----------------------------------------------------------------------------
// ~MpxViewPluginResolver()
// -----------------------------------------------------------------------------
//
MpxViewPluginResolver::~MpxViewPluginResolver()
{
    if( impl ) {
        delete impl;
    }
}
// -----------------------------------------------------------------------------
// resolvePlugin()
// -----------------------------------------------------------------------------
//
MpxViewPlugin* MpxViewPluginResolver::resolvePlugin()
{
    return impl ? impl->resolvePlugin() : 0;
}
