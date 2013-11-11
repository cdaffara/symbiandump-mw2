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
* Description:  Observer class to the playlist scanner object
*
*/


#ifndef MMPXPLAYLISTSCANOBSERVER_H
#define MMPXPLAYLISTSCANOBSERVER_H

class CMPXMediaArray;

/**
 *  MMPXPlaylistScanObserver
 *
 *  Abstract interface for handling playlist scanning
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
class MMPXPlaylistScanObserver
    {

public:

    /**
    * Notification of a list of extracted playlist media properties
    * @param aMediaArray, list of playlist objects
    */
    virtual void AddPlaylistToCollectionL( CMPXMediaArray& aMediaArray ) = 0;
    
    /**
    * Notification of a list of extracted playlist media properties
    * @param aMediaArray, list of playlist objects
    */
    virtual void UpdatePlaylistToCollectionL( CMPXMediaArray& aMediaArray ) = 0;
    
    };


#endif // MMPXPLAYLISTSCANOBSERVER_H
