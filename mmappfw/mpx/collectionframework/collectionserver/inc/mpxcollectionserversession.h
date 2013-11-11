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
* Description:  Collection server session
*
*/


#ifndef CMPXCOLLECTIONSESSION_H
#define CMPXCOLLECTIONSESSION_H

#include <e32base.h>
#include <e32cons.h>
#include <f32file.h>
#include <s32mem.h>
#include <mpxcommand.h>
#include "mpxcollectionengineobserver.h"
#include "mpxcollectionserverdefs.h"

class CMPXCollectionClientContext;
class CMPXCollectionEngine;
class CMPXMessageQueue;
class CMPXMediaArray;
class CMPXCollectionPath;

/**
* Music collection server session
*/
class CMPXCollectionSession : public CSession2,
                              public MMPXCollectionEngineObserver
    {
public:

    /**
    *  Two-phased constructor. Leaves on failure.
    *
    *  @param aEngine engine object
    *  @return The constructed object
    */
    static CMPXCollectionSession* NewL(CMPXCollectionEngine& aEngine);

    /**
    *  C++ constructor
    *
    *  @param aEngine engine object
    */
    CMPXCollectionSession(CMPXCollectionEngine& aEngine);

    /**
    *  Destructor
    */
    ~CMPXCollectionSession();

private: // From base classes

    /**
    *  From MMPXCollectionEngineObserver
    *  Handles the collection entries/item being opened. Typically called
    *  when client has Open()'d a folder or an item
    *
    *  @param aMedia media to hold collection entries
    *  @param aIndex focused entry
    *  @param aComplete ETrue no more entries. EFalse more entries
    *                   expected
    *  @param aError error code
    */
    void HandleOpen(CMPXMedia* aMedia, TInt aIndex,
                    TBool aComplete,TInt aError);

    /**
    * From MMPXCollectionEngineObserver
    * Handles the completion of an async operation
    *
    * @param aError error code of completion
    */
    void HandleOpComplete(TInt aError);

    /**
    *  From MMPXCollectionEngineObserver
    *  Handle media properties
    *
    *  @param aMedia media
    *  @param aError error code
    */
    void HandleMedia(CMPXMedia* aMedia, TInt aError);

    /**
     *  From MMPXCollectionEngineObserver
     *  Handle completion of a asynchronous command
     *
     *  @param aCommandResult result of the command, NULL if error
     *  @param aError error code
     */
     void HandleCommandComplete(CMPXCommand* aCommandResult,
                                TInt aError);

    /**
    *  From MMPXCollectionEngineObserver
    *  Handles remove by collection path
    *
    * @param aUriArray, contains a list of file paths to be deleted
    * @param aError error code
    */
    void HandleRemove(const CDesCArray& aUriArray, TInt aError);

    /**
    *  From MMPXCollectionEngineObserver
    *  Handles FindAllL results
    *
    *  @param aMedia media item(s)
    *  @param aError error code
    */
    void HandleFindAll(CMPXMedia* aMedia, TInt aError);

    /**
    *  From CSession2
    *  Service request
    *  @param aMessage message object
    */
    void ServiceL(const RMessage2& aMessage);

private:
    /**
    *  Dispatch message
    *
    *  @param aMessage message object
    *  @return message completion code
    */
    TInt DispatchMessageL(const RMessage2& aMessage);

    /**
    *  Set collection mode
    *
    *  @param aMessage message object
    */
    void SetModeL(const RMessage2& aMessage);

    /**
    *  Set client id
    *
    *  @param aMessage message object
    */
    void SetClientIdL(const RMessage2& aMessage);

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
    void SetAsyncL(const RMessage2& aMessage);

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
                       TInt aSlot2 = 0,const TDesC8* aVal2 = NULL,
                       TInt aSlot3 = 0,const TDesC8* aVal3 = NULL);

    /**
    *  Writes data from the specified source descriptor to the specified
    *  message slot
    *
    *  @param aSlot1 message slot 1
    *  @param aVal1 value in the slot 1
    *  @param aSlot2 message slot 2
    *  @param aVal2 value in the slot 2
    *  @return KErrNone, if successful, otherwise system error code
    */
    TInt DoWriteData(TInt aSlot1 = 0, const TDesC8* aVal1 = NULL,
                     TInt aSlot2 = 0, const TDesC8* aVal2 = NULL,
                     TInt aSlot3 = 0, const TDesC8* aVal3 = NULL);

    /**
    *  Get current selection
    *
    *  @param aMessage message object
    */
    void GetSelectionL(const RMessage2& aMessage);

private:

    /**
    *  2nd phase constructor
    */
    void ConstructL();

private:
    CMPXCollectionEngine& iEngine;
    CMPXCollectionClientContext* iContext; // Not owned

    RMessage2 iMessage;
    TBool iCompleteRequest;

    //
    // Data for client, for current request
    //
    CBufBase* iAsyncBuffer; // buffer for async request
    CBufBase* iSyncBuffer; // buffer for sync request
    CMPXMedia* iSyncMedia; // Keep reference to result media of sync request
    CMPXMedia* iAsyncMedia; // Keep reference to result media of async request
    CMPXMessageQueue* iMessageQueue; // Message queue
    CMPXMediaArray* iRootMediaArray; // media array for media request of multi selection at root
    CMPXCommand* iMediaCommand; // a copy of media request command
    CMPXCollectionPath* iMediaPath; // path for media request
    };

#endif // CMPXCOLLECTIONSESSION_H

