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
* Description:  Playback observer
*
*/

 

#ifndef MMPXPLAYBACKOBSERVER_H
#define MMPXPLAYBACKOBSERVER_H

#include <bamdesca.h>
#include <mpxmessage2.h>
#include <mpxcommand.h>
#include<mpxplaybackframeworkdefs.h>

class TMPXPlaybackMessage;
class CMPXMedia;

/**
*  Interface for receiving events.
*
*  @lib mpxcommon.lib
*/ 
NONSHARABLE_CLASS(MMPXPlaybackObserver)
    {
public: 
    /**  
     *  Handle playback message.
     *
     *  @since S60 3.2.3
     *  @param aMsg playback message
     *  @deprecated Use method MMPXPlaybackObserver::HandlePlaybackMessage
     *       (CMPXMessage* aMsg, TInt aErr) instead. 
     *        Because if system error happens, e.g. KErrNoMemory, aMsg object
     *        can not be created. 
     *  NOTE: only one of HandlePlaybackMessage callback can be implemented.
     */
    virtual void HandlePlaybackMessage(const CMPXMessage& /*aMsg*/)
         {
         }
    
    /**
    *  Handle playback message.
    *  Note: only one of HandlePlaybackMessage callback can be implemented.
    *  Note: change into pure virtual when all clients updated.
    *
    *  @since S60 3.2.3
    *  @param aMsg playback message, ownership not transferred. 
    *         Please check aMsg is not NULL before using it. If aErr is not 
    *         KErrNone, plugin might still call back with more info in the aMsg.
    *  @param aErr system error code.
    */
    virtual void HandlePlaybackMessage(CMPXMessage* aMsg, TInt /*aErr*/)
        {// Note: change into pure virtual when all clients updated.
        // Temp solution, aMsg will be a dummy object if aErr.
        
        // TO BE REMOVED when above HandlePlaybackMessage is removed.
        HandlePlaybackMessage(*aMsg);
        }
    };

/**
*  Interface for receiving property values asynchronously.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(MMPXPlaybackCallback)
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
    virtual void HandlePropertyL(TMPXPlaybackProperty aProperty,
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
    virtual void HandleSubPlayerNamesL(TUid aPlayer, 
                                       const MDesCArray* aSubPlayers,
                                       TBool aComplete,
                                       TInt aError) = 0;
    
    /**
    *  Call back of media request.
    *
    *  @since S60 3.2.3
    *  @param aMedia media 
    *  @param aError error code    
    */
    virtual void HandleMediaL(const CMPXMedia& aProperties,
                              TInt aError) = 0;
    
    /**
     *  Handle completion of a asynchronous command.
     *  Note: All clients should implement this callback.
     *
     *  @since S60 3.2.3
     *  @param aCommandResult result of the command, NULL if error
     *  @param aError error code    
     */
    virtual void HandlePlaybackCommandComplete(CMPXCommand* /*aCommandResult*/, 
                                               TInt /*aError*/) {}
    
    };
    
#endif // MMPXPLAYBACKOBSERVER_H
