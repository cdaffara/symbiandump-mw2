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
* Description:  Collection observer
*
*/


#ifndef MMPXCOLLECTIONOBSERVER_H
#define MMPXCOLLECTIONOBSERVER_H

#include<mpxcollectionframeworkdefs.h>
#include <mpxmessage2.h>
#include <mpxcommand.h>

class MDesCArray;
class TMPXCollectionMessage;
class CMPXCollectionPlaylist;
class MMPXCollectionFindObserver;

/********************************************************************************
*
*  Note:
* 
*  ** remove TMPXCollectionMessage and associated funtions
*
*  ** remove null implementations
*
*********************************************************************************/

NONSHARABLE_CLASS(MMPXCollectionMediaObserver)
    {
public:
    /**
     *  Handle extended media properties.
     *
     *  @since S60 3.2.3
     *  @param aMedia media 
     *  @param aError error code    
     */
    virtual void HandleCollectionMediaL(const CMPXMedia& aMedia, 
                                        TInt aError) = 0;
    };

/**
 *  Interface for receiving data asynchronously 
 */
NONSHARABLE_CLASS(MMPXCollectionObserver)  : public MMPXCollectionMediaObserver
    {
public:

    /**
     *   
     * 
     *  Handle collection message
     *  NOTE: only one of HandleCollectionMessage callback can be implemented
     * 
     *  @param aMessage collection message
     *  @deprecated please implement HandleCollectionMessage(CMPXMessage* aMsg, TInt aErr)
     */
    virtual void HandleCollectionMessageL(const CMPXMessage& /*aMsg*/) {/* */}

    
    /** 
     *  Handle collection message.
     *  NOTE: only one of HandleCollectionMessage callback can be implemented
     * 
     *  @since S60 3.2.3
     *  @param aMessage collection message, ownership not transferred. 
     *         Please check aMsg is not NULL before using it. If aErr is not 
     *         KErrNone, plugin might still call back with more info in the aMsg.
     *  @param aErr system error code
     */
    virtual void HandleCollectionMessage(CMPXMessage* aMsg, TInt /*aErr*/) 
        {
        // Note change into pure virtual when all clients updated
        // Temp solution, aMsg will be a dummy object if aErr
        // TO BE REMOVED when above HandlePlaybackMessage is removed.
        TRAP_IGNORE(HandleCollectionMessageL(*aMsg));
        }

    /**
     *  Handles the collection entries being opened. Typically called
     *  when client has Open()'d a folder.
     *
     *  @since S60 3.2.3
     *  @param aEntries collection entries opened
     *  @param aIndex focused entry
     *  @param aComplete ETrue no more entries. EFalse more entries
     *                   expected
     *  @param aError error code   
     */
    virtual void HandleOpenL(const CMPXMedia& aEntries,
                             TInt aIndex,TBool aComplete,TInt aError) = 0;
    
    /**
     *  Handles the item being opened. Typically called
     *  when client has Open()'d an item. Client typically responds by
     *  'playing' the item via the playlist.
     *
     *  @since S60 3.2.3
     *  @param aPlaylist collection playlist
     *  @param aError error code
     */                         
    virtual void HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError) = 0;  

    /**
     *  Handle completion of a asynchronous command.
     *  Note: All clients should implement this callback.
     *
     *  @since S60 3.2.3
     *  @param aCommandResult result of the command, NULL if error
     *  @param aError error code    
     */
    virtual void HandleCommandComplete(CMPXCommand* /*aCommandResult*/, 
                                       TInt /*aError*/) {};
    };    

NONSHARABLE_CLASS(MMPXCollectionFindObserver)
    {
public:
    /**
     *  Handle callback for "find" operation.
     *
     *  @since S60 3.2.3
     *  @param aEntries CMPXMedia to be returned
     *  @param aComplete ETrue no more entries. EFalse more entries
     *                   expected
     *  @param aError error code   
     */
    virtual void HandleFindAllL(const CMPXMedia& aResults,
                                TBool aComplete,TInt aError) = 0;   
    };  

NONSHARABLE_CLASS(MMPXCollectionRemoveObserver)
    {
public:
    /**
     * Handles removing a collection path.
     * Calls back with the list of file paths that were deleted.
     * This callback is only applicable for local file storage plugins.
     *
     * @since S60 3.2.3
     * @param aUriArray list of files that were deleted
     * @param aError error code
     */
    virtual void HandleRemoveL(MDesCArray& aUriArray, TInt aError) = 0;
    };      
    
#endif // MMPXCOLLECTIONOBSERVER_H
