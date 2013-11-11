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
* Description:  Inline implementation of playback plugin constructor/destructor
*
*/


// ======================= INLINE MEMBER FUNCTIONS ============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
inline CMPXPlaybackPlugin* CMPXPlaybackPlugin::NewL(TUid aUid, 
                                          MMPXPlaybackPluginObserver& aObs)
    {
    CMPXPlaybackPlugin* self = reinterpret_cast<CMPXPlaybackPlugin*>(
           REComSession::CreateImplementationL(aUid,
                                          _FOFF(CMPXPlaybackPlugin,iDtorKey)));
    self->iUid = aUid;
    self->SetObserver( aObs );
    return self; 
    }

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
inline CMPXPlaybackPlugin* CMPXPlaybackPlugin::NewL(TUid aUid)
    {
    CMPXPlaybackPlugin* self = reinterpret_cast<CMPXPlaybackPlugin*>(
           REComSession::CreateImplementationL(aUid,
                                          _FOFF(CMPXPlaybackPlugin,iDtorKey)));
    self->iUid = aUid;
    return self; 
    }

// ----------------------------------------------------------------------------
// plugin destruction
// ----------------------------------------------------------------------------
//
inline CMPXPlaybackPlugin::~CMPXPlaybackPlugin()
    { 
    REComSession::DestroyedImplementation(iDtorKey);
    }

// ----------------------------------------------------------------------------
// Returns implemenation uid of the plugin
// ----------------------------------------------------------------------------
//
inline const TUid& CMPXPlaybackPlugin::Uid()
    {
    return iUid;
    }

// ----------------------------------------------------------------------------
// Set observer
// ----------------------------------------------------------------------------
//
inline void CMPXPlaybackPlugin::SetObserver(MMPXPlaybackPluginObserver& aObs)
    {
    iObs = &aObs;
    }
