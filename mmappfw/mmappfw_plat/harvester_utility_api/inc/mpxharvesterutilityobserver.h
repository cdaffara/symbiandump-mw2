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
* Description:  Harvester Utility observer
*
*/


#ifndef MMPXHARVESTERUTILITYOBSERVER_H
#define MMPXHARVESTERUTILITYOBSERVER_H

/**
 *  Observer to the harvester utility, for async calls
 *
 *  @lib mpxharvesterutility.lib
 */
NONSHARABLE_CLASS(MMPXHarvesterUtilityObserver)
    {

public:

    /**
     * Callback from method MMPXHarvesterUtility::ExportPlaylistL().
     * Handles completion of playlist export. observer assumed
     * ownership of aMedia.
     *
     * @since S60 3.0
     * @param aMedia media object containing the playlist
     *        aMedia is NULL if aErr != KErrNone
     * @param aErr error code for the operation
     */
    virtual void HandlePlaylistExportCompletedL( CMPXMedia* aMedia, TInt aErr ) = 0;

    /**
     * Callback from method MMPXHarvesterUtility::ImportPlaylist().
     * Handles completion of playlist import.
     *
     * @since S60 3.0
     * @param aMedia media parsed from the playlist file, observer
     *        assumes ownership of this media
     *        aMedia is NULL if the aErr != KErrNone
     * @param aErr  error code for the operation
     */
    virtual void HandlePlaylistImportCompletedL( CMPXMedia* aMedia,
                                                 TInt aErr ) = 0;

    /**
     * Callback from method MMPXHarvesterUtility:: AddFileL().
     * Handle asynchronous file addition by file name, observer
     * assumes ownership of aMedia.
     *
     * @since S60 3.0
     * @param aMedia media object containing the media details of the file
     *        aMedia is NULL if aErr != KErrNone
     * @param aErr error code for the operation
     */
    virtual void HandleFileAddCompletedL( CMPXMedia* aMedia,
                                          TInt aErr ) = 0;

    /**
     * Callback from method MMPXHarvesterUtility::DeleteFilesL().
     * Handle file delete completion.
     *
     * @since S60 3.0
     * @param aErr error code for the operation
     */
    virtual void HandleDeleteCompletedL( TInt aErr ) = 0;

    /**
     * Callback from method MMPXHarvesterUtility::ImportFileL().
     * Handle file import completion.
     *
     * @since S60 3.0
     * @param aMedia media object containing the media details of the file
     *        aMedia is NULL if aErr != KErrNone
     * @param aErr error code for the operation
     */
    virtual void HandleFileImportCompletedL( CMPXMedia* aMedia, TInt aErr  ) = 0;

    /**
     * Callback from method MMPXHarvesterUtility::GetMediaForFileL().
     * Handle asynchronous media creation by file name, observer
     * assumes ownership of aMedia.
     *
     * @since S60 3.0
     * @param aMedia media object containing the media details of the file
     *        aMedia is NULL if aErr != KErrNone
     * @param aErr error code for the operation
     */
    virtual void HandleFileGetMediaCompletedL( CMPXMedia* aMedia, TInt aErr ) = 0;

    };


#endif // MMPXHARVESTERUTILITYOBSERVER_H
