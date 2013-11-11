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
* Description:  Collection engine common observer
*
*/

 

#ifndef MMPXCOLLECTIONENGINECOMMONOBSERVER_H
#define MMPXCOLLECTIONENGINECOMMONOBSERVER_H

#include <badesca.h>
class CMPXMedia;
#include <mpxcommand.h>

/**
 *  Interface for receiving data asynchronously 
 */
NONSHARABLE_CLASS(MMPXCollectionEngineCommonObserver)
    {
public:
    /**
     *  Handle media.
     *
     *  @since S60 3.2.3
     *  @param aMedia media object, NULL if error
     *  @param aError error code    
     */
    virtual void HandleMedia(CMPXMedia* aMedia, TInt aError) = 0;

    /**
     *  Handle completion of a asynchronous command.
     *
     *  @since S60 3.2.3
     *  @param aCommandResult result of the command, NULL if error
     *  @param aError error code    
     */
     virtual void HandleCommandComplete(CMPXCommand* aCommandResult, 
                                        TInt aError) = 0;

    /**
     * Handles removing a collection path.
     * Use case is if an item is removed by a collecton path, we need to 
     * know which files were actually removed to delete from FS.
     *
     * @since S60 3.2.3
     * @param aUriArray NULL if the plugin does not use concept of URI
     *                   else, contains the file path to delete a file
     * @param aError error code
     *
     */
    virtual void HandleRemove(const CDesCArray& aUriArray, TInt aError) = 0;
    
    /**
     * Handles FindAllL results.
     * After find results are passed to the engine, the media object
     * needs to be picked up by the client.
     *
     * @since S60 3.2.3
     * @param aMedia media item(s)
     * @param aError error for the operation, or size of the buffer to return
     */
    virtual void HandleFindAll(CMPXMedia* aMedia, TInt aError) = 0;
    
    };    
    
#endif // MMPXCOLLECTIONENGINECOMMONOBSERVER_H
