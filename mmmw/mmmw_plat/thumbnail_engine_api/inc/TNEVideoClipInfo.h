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
* Description:
*
*/


#ifndef __TNEVIDEOCLIPINFO_H__
#define __TNEVIDEOCLIPINFO_H__

#include "TNECommon.h"

#include <gdi.h>

#define KBestThumbIndex (-1) // search for best possible thumbnail from video

/*
 *  Forward declarations.
 */
class CFbsBitmap;  
class CTNEVideoClipInfo;

/**
 * Observer for notifying that video clip info
 * is ready for reading.
 *
 * @see  CTNEVideoClipInfo
 */
class MTNEVideoClipInfoObserver 
    {
public:
    /**
     * Called to notify that video clip info is ready
     * for reading.
     *
     * Possible error codes:
     *  - <code>KErrNotFound</code> if there is no file with the specified name
     *    in the specified directory (but the directory exists)
     *  - <code>KErrPathNotFound</code> if the specified directory
     *    does not exist
     *  - <code>KErrUnknown</code> if the specified file is of unknown format
     *
     * @param aInfo   video clip info
     * @param aError  <code>KErrNone</code> if info is ready
     *                for reading; one of the system wide
     *                error codes if reading file failed
     */
    virtual void NotifyVideoClipInfoReady(CTNEVideoClipInfo& aInfo, 
                                          TInt aError) = 0;
    };


/**
 * Observer for notifying that video clip thumb has been completed.
 *
 * @see  CTNEVideoClipInfo
 */
class MTNEVideoClipThumbObserver
    {
public:
    /**
     * Called to notify that video clip thumbnail generation 
     * has been completed. 
     * 
     * @param aInfo   video clip info
     * @param aError  <code>KErrNone</code> if frame was
     *                completed successfully; one of the system wide
     *                error codes if generating frame failed
     * @param aThumb  pointer to thumb if it was generated successfully;
     *                <code>NULL</code> if generating frame failed
     */
    virtual void NotifyVideoClipThumbCompleted(CTNEVideoClipInfo& aInfo, 
                                               TInt aError, 
                                               CFbsBitmap* aThumb) = 0;
    };

/**
 * Utility class for getting information about video clip.
 */
class CTNEVideoClipInfo : public CBase
    {
public:

    /* Constructors & destructor. */

    /**
     * Constructs a new CTNEVideoClipInfo object to get information
     * about the specified video clip file. The specified observer
     * is notified when info is ready for reading. This method
     * may leave if no resources are available to construct 
     * a new object.
     * The file will be opened in EFileShareReadersOnly mode by default, 
     * and the same mode should be used by the client too if it need to open
     * the file at the same time.
     * 
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *
     * @param aFileName  name of video clip file
     * @param aObserver  observer to notify when info is ready for reading
     *
     * @return  pointer to a new CTNEVideoClipInfo instance
     */
    IMPORT_C static CTNEVideoClipInfo* NewL(const TDesC& aFileName,
                                            MTNEVideoClipInfoObserver& aObserver);

    IMPORT_C static CTNEVideoClipInfo* NewL(const RFile& aFileName,
                                            MTNEVideoClipInfoObserver& aObserver);

    /**
     * Constructs a new CTNEVideoClipInfo object to get information
     * about the specified video clip file. The constructed object
     * is left in the cleanup stack. The specified observer
     * is notified when info is ready for reading. This method
     * may leave if no resources are available to construct a new
     * object.
     * The file will be opened in EFileShareReadersOnly mode by default, 
     * and the same mode should be used by the client too if it need to open
     * the file at the same time.
     * 
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *
     * @param aFileName  name of video clip file
     * @param aObserver  observer to notify when info is ready for reading
     *
     * @return  pointer to a new CTNEVideoClipInfo instance
     */
    IMPORT_C static CTNEVideoClipInfo* NewLC(const TDesC& aFileName,
                                             MTNEVideoClipInfoObserver& aObserver);

    IMPORT_C static CTNEVideoClipInfo* NewLC(const RFile& aFileName,
                                             MTNEVideoClipInfoObserver& aObserver);

    /* General property methods. */
    /**
     * Returns the file name of the clip. Panics if there is no file 
     * associated with this clip or info is not yet ready for reading.
     * 
     * @return  file name of the clip
     */
    virtual TPtrC FileName() const = 0;
    
    /* Video frame property methods. */

    /**
     * Returns the number of video frames in this clip. Panics if info
     * is not yet ready for reading.
     * 
     * @return  number of video frames in this clip
     */
    virtual TInt VideoFrameCount() const = 0;

    /* Thumbnail methods. */

    /**
     * Generates a bitmap of the given frame from video clip.
     * The frame bitmap is scaled to the specified resolution and converted
     * to the specified display mode. This method is asynchronous. The frame
     * is generated in background and the observer is notified when the frame
     * is complete. This method panics if info is not yet ready for reading or 
     * the resolution is illegal.
     * 
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *  - <code>KErrNotSupported</code>, if the specified combination of 
     *                                   parameters is not supported
     *
     * @param aObserver    observer to be notified when the frame is completed
     * @param aIndex       index of frame, or KFrameIndexBestThumb to look for
     *                     most suitable thumbnail frame.
     * @param aResolution  resolution of the desired frame bitmap, or
     *                     <code>NULL</code> if the frame should be
     *                     in the original resolution
     * @param aDisplayMode desired display mode; or <code>ENone</code> if 
     *                     any display mode is acceptable
     * @param aEnhance     apply image enhancement algorithms to improve
     *                     frame quality; note that this may considerably
     *                     increase the processing time needed to prepare
     *                     the frame
     * @param aPriority    priority of the frame generation
     */
    virtual void GetThumbL(MTNEVideoClipThumbObserver& aObserver,
                            TInt aIndex = KBestThumbIndex,
                            TSize* const aResolution = 0,
                            TDisplayMode aDisplayMode = ENone,
                            TBool aEnhance = EFalse,
                            TInt aPriority = CActive::EPriorityIdle) = 0;
    
    /**
     * Cancels frame generation. If no frame is currently being 
     * generated, the function does nothing.
     */
    virtual void CancelThumb() = 0;

    };



#endif // __TNEVIDEOCLIPINFO_H__

