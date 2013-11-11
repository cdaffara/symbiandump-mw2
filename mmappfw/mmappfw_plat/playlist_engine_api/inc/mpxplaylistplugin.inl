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
* Description:  Inline implementation of playlist plugin constructor/destructor
*
*/


// ----------------------------------------------------------------------------
// Factory method to instantiate a playlist plugin with the matching UID
// ----------------------------------------------------------------------------
//
inline CMPXPlaylistPlugin* CMPXPlaylistPlugin::NewL(
    TUid aUid, 
    MMPXPlaylistPluginObserver& aObserver,
    RFs& aFs,
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aTopCharacterSet,
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aAvailableCharacterSet)
    {
    CMPXPlaylistPlugin* self = 
        reinterpret_cast<CMPXPlaylistPlugin*>(
            REComSession::CreateImplementationL(
                aUid,
                _FOFF(CMPXPlaylistPlugin,iDtorKey)));

    self->iObserver = &aObserver;
    self->iFs = &aFs;
    self->iTopCharacterSet = &aTopCharacterSet;
    self->iAvailableCharacterSet = &aAvailableCharacterSet;

    return self;
    }

// ----------------------------------------------------------------------------
// plugin destruction
// ----------------------------------------------------------------------------
//
inline CMPXPlaylistPlugin::~CMPXPlaylistPlugin()
    { 
    REComSession::DestroyedImplementation(iDtorKey);
    }

