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
* Description:   TNE Video Clip Info Implementation
*
*/




#ifndef __TNEVIDEOCLIPINFOIMP_H__
#define __TNEVIDEOCLIPINFOIMP_H__


#include <e32base.h>

#include "TNEProcessor.h"

#include <TNEVideoClipInfo.h>


/*
 *  Forward declarations.
 */
// cannot include bitmap.h since video processor includes
// this file and fails to compile due to a strange compiler error
// "operator delete must return type void" if bitmap.h
// is included
class CFbsBitmap;  
                   
class CTNEVideoClipInfo;
class CTNEVideoClipInfoOperation;
class CTNEVideoClipThumbOperation;


/**
 *  class for getting information about video clip files and generating thumbnails.
 */
class CTNEVideoClipInfoImp : public CTNEVideoClipInfo
    {
public:

    static CTNEVideoClipInfo* NewL(const RFile& aFileHandle, 
                                    MTNEVideoClipInfoObserver& aObserver);
    /**
     * Destroys the object and releases all resources.
     */    
    ~CTNEVideoClipInfoImp();
    
    
    /* General property methods. */

    /**
     * Returns the file name of the clip. Panics if there is no file 
     * associated with this clip or info is not yet ready for reading.
     * 
     * @return  file name of the clip
     */
     TPtrC FileName() const;
    
    /**
     * Returns the resolution clip. Panics if the
     * info is not yet ready for reading.
     * 
     * @return  TSize of the clip
     */
     TSize GetResolution() const;
    
    /* Thumb methods. */
   
    /**
     * Returns the number of video frames in this clip. Panics if info
     * is not yet ready for reading.
     * 
     * @return  number of video frames in this clip
     */
    TInt VideoFrameCount() const;

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

    void GetThumbL(MTNEVideoClipThumbObserver& aObserver,
                            TInt aIndex,
                            TSize* const aResolution,
                            TDisplayMode aDisplayMode,
                            TBool aEnhance,
                            TInt aPriority);
                            
     /**
     * Cancels Thumbnail generation. If no thumb is currently being 
     * generated, the function does nothing.
     */
    void CancelThumb();


private:
    CTNEVideoClipInfoImp();

    /**
     * Constructor for CTNEVideoClipInfoImp using RFile handle.
     */    
    void ConstructL(const RFile& aFileHandle,
                    MTNEVideoClipInfoObserver& aObserver);

    /**
     * Constructor for CTNEVideoClipInfoImp using file name.
     */    
    void ConstructL(const TDesC& aFileName,
                    MTNEVideoClipInfoObserver& aObserver);
private:
    // Member variables

    // Get video info operation.
    CTNEVideoClipInfoOperation* iInfoOperation;
   
    // Flag to indicate then info is available
    TBool iReady;
      
	// Duplicate RFile handle for the video clip
	RFile iFileHandle;
	   
    // Vídeo format.
    TTNEVideoFormat iFormat;
   
    // Video type/codec.
    TTNEVideoType iVideoType;
   
    // Resolution of video clip.
    TSize iResolution;    
   
    // Frame count of video
    TInt iVideoFrameCount;
    
    // Operation to retrieve thumbnail of video clip.
    CTNEVideoClipThumbOperation* iThumbOperation;

	// These are for supporting TNE requests which send filename
	// internally we open a RFile handle to the file.
    RFs*        iFs;

    friend class CTNEVideoClipInfoOperation;
    friend class CTNEVideoClipThumbOperation;
    friend class CTNEVideoClipInfo;
    };



/**
 * Internal class for reading information from a video clip file.
 * Also implements a simple active object to notify the video clip info 
 * observer when reading has been completed.
 */
class CTNEVideoClipInfoOperation : public CActive
    {
public:
    /* Static constructor */
    static CTNEVideoClipInfoOperation* NewL(CTNEVideoClipInfoImp* aInfo,
                                        MTNEVideoClipInfoObserver& aObserver);
protected:
    /*
    * From CActive
    * Standard active object RunL 
    */
    void RunL();

    /*
    * From CActive
    * Standard active object DoCancel
    */
    void DoCancel();

private:
    /* Default constructor */
    CTNEVideoClipInfoOperation(CTNEVideoClipInfoImp* aInfo, 
                               MTNEVideoClipInfoObserver& aObserver);
    /* Standard Symbian OS two phased constructor */
    void ConstructL();
    /* Destructor */
    ~CTNEVideoClipInfoOperation();

private:
    // Class to contain video clip info.
    CTNEVideoClipInfoImp* iInfo;
    
    // Observer of video clip info operation.
    MTNEVideoClipInfoObserver* iObserver;
    
    // Error code from TNEprosessor.
    TInt iTNEProcessorError;

    friend class CTNEVideoClipInfoImp;
    };

/**
 * Internal class for generating a thumb from a video clip file.
 */
class CTNEVideoClipThumbOperation : public CActive
    {
public:
    /* Static constructor */
    static CTNEVideoClipThumbOperation* NewL(CTNEVideoClipInfoImp* iInfo);

protected:
    /*
    * From CActive
    * Standard active object RunL 
    */
    void RunL();

    /*
    * From CActive
    * Standard active object DoCancel
    */
    void DoCancel();

private:
    /* Default constructor */
    CTNEVideoClipThumbOperation(CTNEVideoClipInfoImp* iInfo);
    /* Standard Symbian OS two phased constructor */
    void ConstructL();
    /* Destructor */
    ~CTNEVideoClipThumbOperation();

    /*
    * Start thumb operation.
    *
    * @aparam aObserver Observer of thumbnail operation.
    * @aparam aIndex Index of frame that is converted to thumbnail.
    * @aparam aResolution Wanted resolution of thumbnail.
    * @aparam aPriority Priority of active object.
    */
    void StartL(MTNEVideoClipThumbObserver& aObserver,
                TInt aIndex, TSize* const aResolution, 
                TDisplayMode aDisplayMode, TBool aEnhance, TInt aPriority);

private:
    // Pointer to info class this thumbnail operation is part of.
    
    CTNEVideoClipInfoImp* iInfo;
    
    // Index of the wanted frame.
    TInt iIndex;

    // Observer of the thumbnail operation.
    MTNEVideoClipThumbObserver* iObserver;

    TBool iThumbRequestPending;

    CTNEProcessor* iTNEProcessor;

    friend class CTNEVideoClipInfoImp;
    };

#endif // __TNEVIDEOCLIPINFOIMP_H__

