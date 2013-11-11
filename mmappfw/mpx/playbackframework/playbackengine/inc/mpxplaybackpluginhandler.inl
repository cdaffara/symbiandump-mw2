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
* Description:  Inline implementation of playback plugin handler
*
*/


// ----------------------------------------------------------------------------
// Set observer
// ----------------------------------------------------------------------------
//
inline void CMPXPlaybackPluginHandler::SetObserver(
    MMPXPlaybackPluginObserver& aObs)
    {
    iPluginObserver = &aObs;
    }

// ----------------------------------------------------------------------------
// Return current plugin
// ----------------------------------------------------------------------------
//
inline CMPXPlaybackPlugin* CMPXPlaybackPluginHandler::Plugin()
    {
    return iPlugin;
    }

// ----------------------------------------------------------------------------
// Return current plugin id
// ----------------------------------------------------------------------------
//
inline TUid CMPXPlaybackPluginHandler::PluginUid() const
    {
    return iPluginUid;
    }

// ----------------------------------------------------------------------------
// Return current plugin type
// ----------------------------------------------------------------------------
//
inline TMPXPlaybackPlayerType CMPXPlaybackPluginHandler::PluginType() const
    {
    return iPluginType;
    }

// ----------------------------------------------------------------------------
// Return player name
// ----------------------------------------------------------------------------
//
inline const TDesC& CMPXPlaybackPluginHandler::PlayerName() const
    {
    return *iDisplayName;
    }

// ----------------------------------------------------------------------------
// Return index of the subplayer
// ----------------------------------------------------------------------------
//
inline TInt CMPXPlaybackPluginHandler::SubPlayer() const
    {
    return iPlugin->SubPlayerIndex();
    }

// ----------------------------------------------------------------------------
// Check if the player found or not
// ----------------------------------------------------------------------------
//
inline TBool CMPXPlaybackPluginHandler::PlayerFound() const
    {
    return iPlugin!=NULL;
    }
