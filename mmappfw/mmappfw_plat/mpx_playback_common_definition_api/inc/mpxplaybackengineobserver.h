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
* Description:  playback engine observer.
*
*/



#ifndef MMPXPLAYBACKENGINEOBSERVER_H
#define MMPXPLAYBACKENGINEOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <mpxplaybackframeworkdefs.h>
#include <mpxmessage2.h>
#include <mpxcommand.h>

// FORWARD DECLARATIONS
class CMPXMedia;

/**
*  Interface for handling active engine.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(MMPXPlaybackEngineObserver)
    {
public:
    /**
    *  Handle playback property.
    *
    *  @since S60 3.2.3
    *  @param aProperty the property
    *  @param aValue the value of the property
    *  @param aError error code
    */
    virtual void HandleProperty(TMPXPlaybackProperty aProperty,
                                TInt aValue, 
                                TInt aError) = 0;
    
    /**
    *  Method is called continously until aComplete=ETrue, signifying that 
    *  it is done and there will be no more callbacks.
    *  Only new items are passed each time.
    *
    *  @since S60 3.2.3
    *  @param aPlayer UID of the subplayer
    *  @param aSubPlayers a list of sub players
    *  @param aComplete ETrue no more sub players. EFalse more subplayer
    *                   expected
    *  @param aError error code
    */
    virtual void HandleSubPlayerNames(TUid aPlayer, 
                                      const MDesCArray* aSubPlayers,
                                      TBool aComplete,
                                      TInt aError) = 0;
    
    /** 
    * 
    *  Call back of media request.
    * 
    *  @since S60 3.2.3
    *  @param aMedia media 
    *  @param aError error code  
    *  
    *  @deprecated Use method MMPXPlaybackEngineObserver::HandleMedia(
    *         CMPXMedia* aMedia, TInt aError) instead.
    *
    */
    virtual void HandleMedia(const CMPXMedia& aMedia,
                             TInt aError)
        {
        HandleMedia(const_cast<CMPXMedia*>(&aMedia), aError);
        }

    /**  
    *  Call back of media request.
    *
    *  @since S60 3.2.3
    *  @param aMedia media object. Ownership not transferred.
    *  @param aError error code
    */
     virtual void HandleMedia(CMPXMedia* /*aMedia*/,
                              TInt /*aError*/)
         {
         // Note: change it into pure virtual after plugins update
         }
    
    /**
    *  Handle completion of a asynchronous command.
    *
    *  @since S60 3.2.3
    *  @param aCommandResult result of the command, NULL if error
    *  @param aError error code    
    */
    virtual void HandlePlaybackCommandComplete(CMPXCommand* /*aCommandResult*/, 
                                               TInt /*aError*/) {} 
    };
          
#endif // MMPXPLAYBACKENGINEOBSERVER_H
            
// End of File
