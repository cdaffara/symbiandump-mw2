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
* Description:  Playback server session
*
*/


#ifndef CMPXPLAYBACKSESSION_H
#define CMPXPLAYBACKSESSION_H


#include <e32cons.h>
#include <f32file.h>
#include <s32mem.h>
#include <mpxplaybackengineobserver.h>
#include "mpxplaybackserverdefs.h"

class CMPXPlaybackEngine;
class CMPXMessageQueue;

/**
* Music playback server session
*/
class CMPXPlaybackSession : public CSession2,
                            public MMPXPlaybackEngineObserver
    {
public:

    /**
    *  Two-phased constructor. Leaves on failure.
    *
    *  @return The constructed object
    */
    static CMPXPlaybackSession* NewL();

    /**
    *  C++ constructor
    *
    */
    CMPXPlaybackSession();

    /**
    *  Destructor
    */
    ~CMPXPlaybackSession();
    
private:

    /**
    *  From CSession2
    *  Service request
    *  @param aMessage message object
    */
    void ServiceL(const RMessage2& aMessage);

    /**
    *  Dispatch message
    *
    *  @param aMessage message object
    *  @return message completion code
    */
    void DispatchMessageL( const RMessage2& aMessage, TInt& aMsgHandleResult );
    
    /**
    *  Set playback mode
    *
    *  @param aMessage message object
    */
    void SetModeL(const RMessage2& aMessage);

    /**
    *  Get client id from message
    *
    *  @param aMessage message object
    *  @reutrn client thread id
    */
    TThreadId ClientIdL(const RMessage2& aMessage);

    /**
    *  Init player from file 
    *
    *  @param aMessage message object
    */
    void InitFromFileL(const RMessage2& aMessage);

    /**
    *  Cancel outstanding requests
    */
    void CancelRequests();

    /**
    *  Set request as async
    *
    *  @param aMessage message object
    */
    void SetAsync(const RMessage2& aMessage);
    
    /**
    *  Complete async request
    *
    *  @param aErr error code
    *  @param aSlot1 message slot 1
    *  @param aVal1 value in the slot 1
    *  @param aSlot2 message slot 2
    *  @param aVal2 value in the slot 2
    */
    void CompleteAsync(TInt aErr,
                        TInt aSlot1 = 0, const TDesC8* aVal1 = NULL,
                        TInt aSlot2 = 0,const TDesC8* aVal2 = NULL);
                        

    /**
    *  Writes data from the specified source descriptor to the specified
    *  message slot
    *
    *  @param aErr error code
    *  @param aSlot1 message slot 1
    *  @param aVal1 value in the slot 1
    *  @param aSlot2 message slot 2
    *  @param aVal2 value in the slot 2
    *  @return KErrNone, if successful, otherwise system error code
    */
    TInt DoWriteData(TInt aSlot1 = 0, const TDesC8* aVal1 = NULL,
                     TInt aSlot2 = 0, const TDesC8* aVal2 = NULL);

    /**
    *  Get current selection
    *
    *  @param aMessage message object
    */
    void GetSelectionL(const RMessage2& aMessage);
    
    /**
    *  Helper methods for transferring variable sized arrays and objects
    */
    
    /**
    *  Create the buffer filled with player UIDs
    *
    *  @param aMessage message object
    *  @return the size of buffer
    */
    TInt CreatePlayerUidsBufferL(const RMessage2& aMessage);

    /**
    *  Check if player is valid
    */
    void CheckPlayerL();
    
private:  

    /**
    *  From MMPXPlaybackEngineObserver
    *  Handle playback property
    *
    *  @param aProperty the property
    *  @param aValue the value of the property
    *  @param aError error code
    */
    void HandleProperty(TMPXPlaybackProperty aProperty,
                               TInt aValue, 
                               TInt aError);

    /**
    *  From MMPXPlaybackEngineObserver
    *  Method is called continously until aComplete=ETrue, signifying that 
    *  it is done and there will be no more callbacks
    *  Only new items are passed each time
    *
    *  @param aPlayer UID of the subplayer
    *  @param aSubPlayers a list of sub players
    *  @param aComplete ETrue no more sub players. EFalse more subplayer
    *                   expected
    *  @param aError error code
    */
    void HandleSubPlayerNames(TUid aPlayer,
                              const MDesCArray* aSubPlayers,
                              TBool aComplete,
                              TInt aError);

    /**
    *  From MMPXPlaybackEngineObserver
    *  Handle media properties
    *
    *  @param aMedia media object
    *  @param aError error code
    */
    void HandleMedia(CMPXMedia* aMedia, TInt aError); 

    /**
     *  From MMPXPlaybackEngineObserver
     *  Handle completion of a asynchronous command
     *
     *  @param aCommandResult result of the command, NULL if error
     *  @param aError error code    
     */
     void HandlePlaybackCommandComplete(CMPXCommand* aCommandResult, 
                                        TInt aError);
    
private:

    /**
    *  2nd phase constructor
    */
    void ConstructL();
    
private:
    CMPXPlaybackEngine* iPlayer;
    RMessage2 iMessage;    
    TBool iCompleteRequest;
    //
    // Data for client, for current request
    //
    CBufBase* iSyncBuffer;
    CBufBase* iAsyncBuffer;
    CMPXMedia* iMedia;
    CMPXMessageQueue* iMessageQueue; // Message queue
    };
        
#endif // CMPXPLAYBACKSESSION_H

