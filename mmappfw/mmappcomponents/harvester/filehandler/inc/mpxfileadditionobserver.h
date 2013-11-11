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
* Description:  File addition Observer
*
*/


#ifndef MMPXFILEADDITIONOBSERVER_H
#define MMPXFILEADDITIONOBSERVER_H

/**
 *  Abstract interface for file addition
 *
 *  @lib mpxfilehandler.lib
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( MMPXFileAdditionObserver )
    {

public:

    /**
     * Indicates that a new media file has 
     * been detected by the folder scanner
     *
     * @param aFileName file name
     * @param aColIndex mime type look up index
     * @param aPlaylist is this item a playlist
     */
    virtual void HandleFileAdditionL( const TDesC& aFileName, 
                                      TInt aColIndex,
                                      TBool aPlaylist = EFalse ) = 0; //lint !e1735
    /**
    * Callback from scanner that it is moving onto next folder
    * @param aDrive a drive to open
    * @param aFolder folder that we are opening
    */
    virtual void HandleOpenDriveL( TDriveNumber aDrive, const TDesC& aFolder ) = 0;
      
    /**
     * Checks whether a file is a media file
     * @param aFile file to check
     * @return ETrue if it is a media file
     */
    virtual TInt IsMediaFileL( const TDesC& aFile ) = 0;
    
    /**
     * Checks whether a file is a playlist file
     * @param aFile, file to check
     * @return ETrue if this is a playlist file.
     */
    virtual TInt IsPlaylistFileL( const TDesC& aFile ) = 0;
    
    /**
    * Checks if a certain path is blocked
    * @param aPath, path to check
    * @return ETrue if the path is on the blocked list
    */
    virtual TBool IsPathBlockedL( const TDesC& aPath ) = 0;
    };


#endif // MMPXFILEADDITIONOBSERVER_H
