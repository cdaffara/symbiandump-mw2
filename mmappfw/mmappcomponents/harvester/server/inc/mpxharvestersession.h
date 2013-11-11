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
* Description:  Harvester Server Session
*
*/


#ifndef CMPXHARVESTERSESSION_H
#define CMPXHARVESTERSESSION_H

#include <e32base.h>
#include <e32cons.h>
#include <f32file.h>
#include <s32mem.h>

#include "mpxharvesterengineobserver.h"
#include "mpxhvsmsg.h"

// FORWARD DECLARATIONS
class CMPXHarvesterServer;
class CMPXHarvesterEngine;
class CMPXMedia;

/**
* Harvester server session
*/
NONSHARABLE_CLASS( CMPXHarvesterSession ) : public CSession2,
                                            public MMPXHarvesterEngineObserver
    {
public:

    /**
    *  Two-phased constructor. Leaves on failure.
    *
    *  @param aServer server object
    *  @param aEngine engine object
    *  @return The constructed object
    */
    static CMPXHarvesterSession* NewL(CMPXHarvesterServer& aServer,
                                      CMPXHarvesterEngine& aEngine);

    /**
    *  C++ constructor
    *
    *  @param aServer server object
    *  @param aEngine engine object
    */
    CMPXHarvesterSession(CMPXHarvesterServer& aServer,
                          CMPXHarvesterEngine& aEngine);

    /**
    *  Destructor
    */
    ~CMPXHarvesterSession();

private: // From base classes

    /**
    *  From CSession2
    *  Service request
    *  @param aMessage message object
    */
    void ServiceL(const RMessage2& aMessage);

    /**
    * From MMPXHarvesterEngineObserver
    * Handles the completion of an import event
    */
    void HandleImportCompleteL( CMPXMedia* aMedia, TInt aErr,
                                RHvsMsg* aMsg );

    /**
    * From MMPXHarvesterEngineObserver
    * Handles the completion of an import event
    */
    void HandleExportCompleteL( CMPXMedia* aMedia, TInt aErr,
                                RHvsMsg* aMsg );

    /**
    * From MMPXHarvesterEngineObserver
    * Handles the completion of an async msg with error
    */
    void CompleteTask( RHvsMsg* aMsg, TInt aErr );

private:
    /**
    *  Dispatch message
    *
    *  @param aMessage message object
    */
    void DispatchMessageL(const RMessage2& aMessage);

    /**
    * Scan the FS for files
    */
    void DoScanL();

    /**
    * Cancel the scan (if executing)
    */
    void DoCancelScan();

    /**
    * Shutdown the harvester (for testing memory leaks)
    * Testing ONLY
    */
    void DoShutdown();

    /**
    * Handles addition of a file (iDes) to the harvester
    * @param aMsg message containing the item to add
    */
    void DoAddFileL( RHvsMsg* aMsg );

    /**
    * Decode the IPC message to add a media
    * @param aMessage, message to decode
    * @return, collection ID of the media
    */
    TInt DoAddMediaPropL( const RMessage2& aMessage );

    /**
    * Handles media creation for a file
    * @param aMsg message containing the item
    */
    void DoGetMediaL( RHvsMsg* aMsg );

    /**
    * Decode the IPC message to get a collection id
    * @param aMessage, message to decode
    * @return, collection ID of the file
    */
    TInt DoGetColUidL( const RMessage2& aMessage );

    /**
    * Removes multiple songs from the harvester db
    * @param aMessage message to decode
    */
    void DoRemoveMultipleFilesL( const RMessage2& aMessage );

    /**
    * Update a file according to new information
    * @param aMessage message to decode
    */
    void DoUpdateFileL( const RMessage2& aMessage );

    /**
    * Rename a file according to new information
    * @param aMessage message to decode
    */
    void DoRenameFileL( const RMessage2& aMessage );

    /**
    * Set the message as async and add it to the message list
    * @param aMessage message to decode
    * @param aOp operation id
    */
    RHvsMsg* SetAsyncL( const RMessage2& aMessage, TMPXHarvesterServerOp aOp );

    /**
    * Complete and remove an RHvsMsg
    * @param aMessage message to decode
    * @param aErr error code to complete operation
    */
    void CompleteMessage( RHvsMsg*& aMessage, TInt aErr );

    /*
    *  Helper methods for transferring variable sized arrays and objects
    */

    /**
    *  Reads a descriptor from the message, allocated into iDes
    *
    *  @param aMessage message object
    *  @param aMsgSlot message slot
    *  @return the result descriptor
    */
    const TDesC& ReadDesL(const RMessage2& aMessage,TInt aMsgSlot);

private:

    /**
    *  2nd phase constructor
    */
    void ConstructL();

private:

    CMPXHarvesterServer&  iServer;  // not owned
    CMPXHarvesterEngine&  iEngine;  // not owned

    HBufC* iDes;
    HBufC8* iDes8;
    CBufBase* iBuffer;

    RPointerArray<RHvsMsg> iMessages;
    CMPXMedia* iMedia;
    };

#endif // CMPXHARVESTERSESSION_H

