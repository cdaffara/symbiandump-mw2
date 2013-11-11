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




#ifndef __HXTNEVIDEOCLIPINFOIMP_H__
#define __HXTNEVIDEOCLIPINFOIMP_H__


#include <e32base.h>
#include "HXTneclientservercommon.h"
#include "DisplayChain.h"
#include <TNEVideoClipInfo.h>


#include "HXTneClientObserver.h"

class CFbsBitmap;  
          
class CTNEClientObserver;                   
class CTNEVideoClipInfo;

/**
 *  class for getting information about video clip files and generating thumbnails.
 */

class CHXTNEVideoClipInfoImp : public CTNEVideoClipInfo
                               ,public RSessionBase
 {
 
 public:

    static CTNEVideoClipInfo* NewL(const RFile& aFileHandle, 
                                    MTNEVideoClipInfoObserver& aObserver);
    /**
     * Destroys the object and releases all resources.
     */    
    ~CHXTNEVideoClipInfoImp();
    
    
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

	


// client state - This will be the client side states for maintaining the signals from clientobserver
enum EThumbnailClientState  // this holds
    {
        EStartGettingMetadata = 0,
        EStartGettingThumbNail,
        EStartGettingThumbNailWithIndex,
        ECancelling
    };



//Client server
TInt CreateServer();



 TVersion Version() const;  
   
   //callback from observer
   
   void ServiceCompleted(TInt aError);
   
   
   //sendrecieve functions - rest are not implemented for simplicity
    
        TInt OpenFile(const RFile& aFileHandle, TInt aPosition);
        
        TInt  OpenFile(TInt aPosition);
        
        private:
            CHXTNEVideoClipInfoImp ();
        
            /**
            * Constructor for CHXTNEVideoClipInfoImp using RFile handle.
         */    
          void ConstructL(const RFile& aFileHandle,
                MTNEVideoClipInfoObserver& aObserver);
        
        /**
        * Constructor for CHXTNEVideoClipInfoImp using file name.
        */    
        void ConstructL(const TDesC& aFileName,
                MTNEVideoClipInfoObserver& aObserver);
                
        
        
        
        void FetchAndSendMetaData(TInt err); // Gets the metadata from the server and sents an event to the App
        
        void NotifyIfGetThumbPending(TInt aError, CFbsBitmap *&pBitMap);
        
        void GenerateThumbNail();
        
        TBool HandleThumbnailError(TInt aError);
        void  EnhanceThumbnailL(const CFbsBitmap* aInBitmap, 
        									CFbsBitmap* aTargetBitmap);
        									
	
        // Observers 	MTNEVideoClipInfoObserver and MTNEVideoClipThumbObserver
        MTNEVideoClipInfoObserver*              iClipInfoObserver;
        MTNEVideoClipThumbObserver*		 iThumbObserver;
        
        // Storage for metadata between client and server
        TNEMetaData                                  iMetaData;
        TNEThumbRequest                          iThumbRequestData;
        
       //server name variable
        HBufC*	 				     iServerName;
        
          //Client state
        EThumbnailClientState                   c_State;  
        
        TInt						    m_LastError;	   	
        TBool                                            iCloseHandle;    
        RFs*	                                             iFs;             
        RFile	                                            iFileHandle;     
        
      
         	
        CFbsBitmap* 				    iOutBitmap;
           	
        TUint8* 					    iRgbBuf;
        TFileName					    iFileName;
        //server thread
        RThread                                       iServerThread;
        TSize 			                    iOutputThumbResolution;    
        // Thumbnail index
        TInt 			                            iThumbIndex; 
        // Thumbnail display mode
        TDisplayMode 	                            iThumbDisplayMode; 
        TInt			                            iPriority;        
        // Thumbnail enhance
        TBool                                           iThumbEnhance;         
           	// frame enhancement
        CDisplayChain*                             iEnhancer;  
        
        //observer
        CTNEClientObserver*		          iObserver;
        friend class CTNEVideoClipInfo;
        friend class CTNEClientObserver;

    };
                              
#endif // __HXTNEVIDEOCLIPINFOIMP_H__


