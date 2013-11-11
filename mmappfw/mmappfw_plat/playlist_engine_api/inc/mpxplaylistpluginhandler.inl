/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline implementation of playlist plugin handler
*
*/


// ----------------------------------------------------------------------------
// Return current playlist plugin id
// ----------------------------------------------------------------------------
//
inline TUid CMPXPlaylistPluginHandler::PluginUid() const
    {
    return iPluginUid;
    }   
    
// ----------------------------------------------------------------------------
// Return current playlist plugin type
// ----------------------------------------------------------------------------
//
inline TUint32 CMPXPlaylistPluginHandler::PluginType() const
    {
    return iPluginType;
    }   
    
// ----------------------------------------------------------------------------
// Return playlist plugin name
// ----------------------------------------------------------------------------
//
inline const TDesC& CMPXPlaylistPluginHandler::PluginName() const
    {
    return iDisplayName ? static_cast<const TDesC&>( *iDisplayName ) : KNullDesC;
    }

// ----------------------------------------------------------------------------
// Check if the playlist plugin found or not
// ----------------------------------------------------------------------------
//
inline TBool CMPXPlaylistPluginHandler::PluginFound() const
    { 
    return iPlugin!=NULL;
    }

// ----------------------------------------------------------------------------
// Return current playlist plugin
// ----------------------------------------------------------------------------
//
inline CMPXPlaylistPlugin* CMPXPlaylistPluginHandler::Plugin() const
    {
    return iPlugin;
    }
    
    
    
    
