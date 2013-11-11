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
* Description:  Collection playlist observer
*
*/


#ifndef MMPXCOLLECTIONPLAYLISTOBSERVER_H
#define MMPXCOLLECTIONPLAYLISTOBSERVER_H


/**
 *  Interface for colleciton playlist changes
 *
 *  @lib mpxcollectionutility.lib
 */
NONSHARABLE_CLASS(MMPXCollectionPlaylistObserver)
    {
public:
    /**
     *  Handle collection playlist change.
     *
     *  @since S60 3.2.3
     *  @param aError KErrNotFound - Playlist is updated, current item removed
     *                KErrNone - Playlist is updated, current item is valid
     *                KErrEof - Playlist is updated, current item removed and 
     *                          reached to the end of playlist
     */
    virtual void HandleCollectionPlaylistChange(TInt aError) = 0;
    
    };    
    
#endif // MMPXCOLLECTIONPLAYLISTOBSERVER_H
