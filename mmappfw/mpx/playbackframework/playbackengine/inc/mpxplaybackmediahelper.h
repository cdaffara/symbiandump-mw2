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
* Description:  Playback engine's media helper
*
*/



#ifndef CMPXPLAYBACKMEDIAHELPER_H
#define CMPXPLAYBACKMEDIAHELPER_H

#include <e32base.h>
#include <mpxcollectionpath.h>
#include <mpxtaskqueueobserver.h>
#include <mpxcollectionobserver.h>

// FORWARD DECLARATIONS
class CMPXPlaybackEngine;
class CMPXActiveTaskQueue;
class MMPXPlaybackEngineObserver;
class MMPXCollectionUtility;
class CMPXClientList;

/**
*  Helper class to retrieve media for clients
*/
NONSHARABLE_CLASS( CMPXPlaybackMediaHelper ) : public CBase,
                                               public MMPXTaskQueueObserver,
                                               public MMPXCollectionObserver
    {
public:

    /**
    * Two-phased constructor
    */
    static CMPXPlaybackMediaHelper* NewL( CMPXPlaybackEngine& aEngine );

    /**
    * Destructor.
    */
    virtual ~CMPXPlaybackMediaHelper();

private: // constructors

    /**
    * Constructor.
    */
    CMPXPlaybackMediaHelper( CMPXPlaybackEngine& aEngine );

    /**
    * Symbian OS constructor.
    */
    void ConstructL();

public: // New Methods

    /**
    * Retrieves the media given the current path, and upon return,
    * either calls back the observer, or broadcasts the message to
    * the given client list
    *
    * @param aPath Collection path to retrieve media.
    * @param aCmd CMPXCommand containing the attributes and attribute spec
    *             Ownership of aAttrs is passed to this method
    * @param aObserver Callback to return media to
    * @param aBroadcast Whether or not to broadcast media message
    * @param aClientlist Clientlist to broadcast message to
    */
    void MediaL( const CMPXCollectionPath& aPath,
                 CMPXCommand* aCmd,
                 MMPXPlaybackEngineObserver* aObserver,
                 TBool aBroadcast = EFalse,
                 CMPXClientList* aClientList = NULL );

    /**
    * Cancels all outstanding requests
    */
    void CancelRequests();

private: // From base classes

    /**
    *  From MMPXTaskQueueObserver
    */
    void ExecuteTask( TInt aTask,
                      TInt aParamData,
                      TAny* aPtrData,
                      const CBufBase& aBuf,
                      TAny* aCallback,
                      CBase* aCObject1,
                      CBase* aCObject2);

    /**
    * @see MMPXTaskQueueObserver
    */
    void HandleTaskError(TInt aTask,
                         TAny* aPtrData,
                         TAny* aCallback,
                         TInt aError);

	/**
    *  From MMPXCollectionObserver
    */
    void HandleCollectionMessage(CMPXMessage* aMsg, TInt aErr);

    /**
    *  From MMPXCollectionObserver
    */
    void HandleOpenL( const CMPXMedia& aEntries,
                      TInt aIndex,
                      TBool aComplete,
                      TInt aError );

    /**
    *  From MMPXCollectionObserver
    */
    void HandleOpenL( const CMPXCollectionPlaylist& aPlaylist,
                      TInt aError );

    /**
     *  From MMPXCollectionObserver
     */
    void HandleCommandComplete( CMPXCommand* aCommandResult,
                                TInt aError );

    /**
    *  From MMPXCollectionMediaObserver
    */
    void HandleCollectionMediaL( const CMPXMedia& aMedia,
                                 TInt aError);

private:  // New functions

    /**
    * Executes task, leaving method
    */
    void DoExecuteTaskL(
            TInt aTask,
            TInt aParamData,
            TAny* aPtrData,
            const CBufBase& aBuf,
            TAny* aCallback,
            CBase* aCObject1,
            CBase* aCObject2);

private: // data
    CMPXPlaybackEngine& iEngine;
    MMPXCollectionUtility* iCollectionUtility;
    CMPXActiveTaskQueue* iTaskQueue;
    };

#endif // CMPXPLAYBACKMEDIAHELPER_H

//End of file
