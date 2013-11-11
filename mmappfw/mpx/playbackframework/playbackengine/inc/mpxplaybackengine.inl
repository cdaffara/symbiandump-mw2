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
* Description:  Inline implementation of playback engine
*
*/


#include <mpxlog.h>
// ============================ LOCAL FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Check if a pointer is NULL
// ----------------------------------------------------------------------------
//
#include <mpxmessage2.h>
#include <mpxcollectionplaylist.h>
#include <mpxmediacollectiondetaildefs.h>
#include <mpxplaybackmessagedefs.h>

inline void CheckPtrL(TAny* aPtr)
    {
    if (!aPtr)
        {
        User::Leave(KErrNotReady);
        }
    }
    
// ----------------------------------------------------------------------------
// Check if a pointer is NULL
// ----------------------------------------------------------------------------
//
inline void CheckPtrL(const TAny* aPtr)
    {
    CheckPtrL(const_cast<TAny*>(aPtr));
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Return plugin interface
// ----------------------------------------------------------------------------
//
inline CMPXPlaybackPlugin* CMPXPlaybackEngine::PluginL() const
    {
    CheckPtrL(iPluginHandler->Plugin());
    return iPluginHandler->Plugin();
    }

// ----------------------------------------------------------------------------
// Return the list of clients attached to this player
// ----------------------------------------------------------------------------
//
inline CMPXClientList* CMPXPlaybackEngine::ClientList()
    {
    return iClientList;
    }

// ----------------------------------------------------------------------------
// Return plugin handler
// ----------------------------------------------------------------------------
//
inline CMPXPlaybackPluginHandler* CMPXPlaybackEngine::PluginHandler()
    {
    return iPluginHandler;
    }    
    
// ----------------------------------------------------------------------------
// Does this player have access to audio resource
// ----------------------------------------------------------------------------
//
inline TBool CMPXPlaybackEngine::IsPlayerActive() const
    {
    return iPlayerActive;
    }
    
// ----------------------------------------------------------------------------
// Return mode id of the player engine
// ----------------------------------------------------------------------------
//
inline const TUid& CMPXPlaybackEngine::ModeId() const
    {
    return iModeId;
    }
	

// ----------------------------------------------------------------------------
// Return category of the player engine
// ----------------------------------------------------------------------------
//
inline const TInt CMPXPlaybackEngine::Category() const
    {
    return iCategory;
    }

// ----------------------------------------------------------------------------
// Current state
// ----------------------------------------------------------------------------
//
inline TMPXPlaybackState CMPXPlaybackEngine::State() const
    {
    MPX_DEBUG3("CMPXPlaybackEngine::State(): this = 0x%08x, iState = %d", this, iState);
    return iState;
    }   
          
// ----------------------------------------------------------------------------
// Current collection path, or KNullDesC if none (i.e. not from collection)
// ----------------------------------------------------------------------------
//
inline const CMPXCollectionPlaylist* CMPXPlaybackEngine::Playlist() const
    {
    CMPXCollectionPlaylist* pl = NULL;
    if (iPlaylist && iPlaySource==EPlayFromCollection)
        {
        pl = iPlaylist;
        }    
    return pl;
    }
    
// ----------------------------------------------------------------------------
// Current uri, or KNullDesC if none (i.e. uri not supplied)
// ----------------------------------------------------------------------------
//
inline const TDesC& CMPXPlaybackEngine::Uri() const
    { 
    return (iUri && iPlaySource == EPlayFromUri) ? 
            static_cast<const TDesC&>(*iUri) : KNullDesC;
    }    

// ----------------------------------------------------------------------------
// Current file handle
// ----------------------------------------------------------------------------
//
inline const RFile& CMPXPlaybackEngine::File() const
    {
    return iFile;
    }

// ----------------------------------------------------------------------------
// Return last active process id
// ----------------------------------------------------------------------------
//
inline TProcessId CMPXPlaybackEngine::LastActiveProcessId() const
    {
    return iLastActiveProcess;
    }

// ----------------------------------------------------------------------------
// CPlaybackCallbackReflector constructor
// ----------------------------------------------------------------------------
//
inline CMPXPlaybackEngine::TCallbackReflector::TCallbackReflector(
                                                   CMPXPlaybackEngine& aEngine)
    : iEngine(aEngine)
    {
    }

// ----------------------------------------------------------------------------
// Handle playback property
// Called by CMPXPlaybackEngine::HandleProperty which is called by plugin
// ----------------------------------------------------------------------------
//  
void CMPXPlaybackEngine::TCallbackReflector::HandleProperty(
    TMPXPlaybackProperty aProperty,
    TInt aValue,
    TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::TCallbackReflector::HandleProperty");
    MPX_DEBUG5("HandleProperty eninge 0x%08x prop %d val %d err %d", 
                &iEngine, aProperty, aValue, aError);
    ASSERT(iEngine.iTaskQueue->Task() == EProperty);
    if (KErrNone == aError)
        {
        iEngine.iProperties[aProperty]=aValue;
        TRAP_IGNORE(
            iEngine.iClientList->SendMsgL(TMPXPlaybackMessage(
                                    TMPXPlaybackMessage::EPropertyChanged,
                                    aProperty, aValue)));
        }
    // Task will be completed in the CMPXPlaybackEngine::HandleProperty
    }
    
// ----------------------------------------------------------------------------
// Method is called continously until aComplete=ETrue, signifying that 
// it is done and there will be no more callbacks
// ----------------------------------------------------------------------------
//  
void CMPXPlaybackEngine::TCallbackReflector::HandleSubPlayerNames(
                                               TUid /*aPlayer*/,
                                               const MDesCArray* /*aSubPlayers*/,
                                               TBool /*aComplete*/,
                                               TInt  /*aError*/)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::TCallbackReflector::HandleSubPlayerNames");
    }
    
// ----------------------------------------------------------------------------
// Callback of media request
// ----------------------------------------------------------------------------
//  
void CMPXPlaybackEngine::TCallbackReflector::HandleMedia(
                                    CMPXMedia* /*aMedia*/,
                                    TInt /*aError*/)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::TCallbackReflector::HandleMedia");
    }

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
// ----------------------------------------------------------------------------
// Current file handle
// ----------------------------------------------------------------------------
//
inline const RFile64& CMPXPlaybackEngine::File64() const
    {
    MPX_FUNC("CMPXPlaybackEngine::File64()");
    return iFile64;
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

//End of file
