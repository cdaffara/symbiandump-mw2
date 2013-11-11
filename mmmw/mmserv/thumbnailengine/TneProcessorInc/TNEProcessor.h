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
* Description:   Thumbnail processor interface class
*
*/




#ifndef __TNEPROCESSOR_H__
#define __TNEPROCESSOR_H__

#include <e32base.h>
#include <e32std.h>
#include <gdi.h>

#include <TNECommon.h>

/*
 * Forward declarations.
 */
class CTNEProcessorImpl;

// @@ YHK Do we need this observer
// probably dont need it
class MTNEMovieProcessingObserver;

/**
 * Video processor.
 */
class CTNEProcessor : public CBase
	{
public:

	/** 
	 * Constructors for instantiating new video processors.
	 * Should reserve as little resources as possible at this point.
	 */
	static CTNEProcessor* NewL();
	static CTNEProcessor* NewLC();

	/** 
	 * Destructor can be called at any time (i.e., also in the middle of a processing operation).
	 * Should release all allocated resources, including releasing all allocated memory and 
	 * *deleting* all output files that are currently being processed but not yet completed.
	 */
	virtual ~CTNEProcessor();

    /**
	 * Read the header from the specified video clip file and return its properties.
	 * The file should be opened with EFileShareReadersOnly share mode.
	 *
	 * Possible leave codes:
	 *	- <code>KErrNoMemory</code> if memory allocation fails
	 *	- <code>KErrNotFound</code> if there is no file with the specified name
	 *    in the specified directory (but the directory exists)
	 *	- <code>KErrPathNotFound</code> if the specified directory
	 *    does not exist
	 *	- <code>KErrUnknown</code> if the specified file is of unknown format
	 *
	 * @param aFileHandle           file handle to read
	 * @param aVideoFormat          for returning the video format
	 * @param aVideoType            for returning the video type
	 * @param aResolution           for returning the resolution
	 * @param aVideoFrameCount      for returning the number of video frames
	 */
	void GetVideoClipPropertiesL(RFile& aFileHandle,
		                         TTNEVideoFormat& aFormat,
								 TTNEVideoType& aVideoType, 
								 TSize& aResolution,
								 TInt& aVideoFrameCount);
	

	/**
	 * Do all initializations necessary to start generating a thumbnail, e.g. open files, 
	 * allocate memory. The video clip file should be opened with EFileShareReadersOnly 
	 * share mode. The thumbnail should be scaled to the specified resolution and 
	 * converted to the specified display mode. If this method leaves, destructor should be called to free 
	 * allocated resources.
	 * 
	 * Possible leave codes:
	 *	- <code>KErrNoMemory</code> if memory allocation fails
	 *	- <code>KErrNotFound</code> if there is no file with the specified name
	 *    in the specified directory (but the directory exists)
	 *	- <code>KErrPathNotFound</code> if the specified directory
	 *    does not exist
	 *	- <code>KErrUnknown</code> if the specified file is of unknown format
	 *  - <code>KErrNotSupported</code> if the specified combination of parameters
	 *                                  is not supported
	 *
	 * @param aFileName  name of the file to generate thumbnail from
     * @param aIndex       Frame index for selecting the thumbnail frame
     *                     -1 means the best thumbnail is retrieved
 	 * @param aResolution  resolution of the desired thumbnail bitmap, or
	 *                     <code>NULL</code> if the thumbnail should be
	 *                     in the original resolution
	 * @param aDisplayMode desired display mode; or <code>ENone</code> if 
	 *                     any display mode is acceptable
	 * @param aEnhance	   apply image enhancement algorithms to improve
	 *                     thumbnail quality; note that this may considerably
	 *                     increase the processing time needed to prepare
	 *                     the thumbnail
	 */
	void StartThumbL(RFile& aFileHandle, TInt aIndex, TSize aResolution, 
					 TDisplayMode aDisplayMode, TBool aEnhance);	
	
     /**
	 * Starts thumbnail generation. Thumbnail generation is an asynchronous operation,
	 * and its completion is informed to the caller via Active object request completion;
	 * the iStatus member of the caller is passed as a parameter to this method.
	 *
	 * This method may leave if an error occurs in initiating the thumbnail generation.	
	 * If this method leaves, destructor should be called to free allocated resources.	
	 * 
	 * @param aStatus     Reference to caller's iStatus member variable
	 *
	 * @return  
	 *          
	 */
	void ProcessThumbL(TRequestStatus &aStatus);
	
	/** 
	 * Method for retrieving the completed thumbnail bitmap.
	 * 
	 * Video processor should not free the CFbsBitmap instance after it has passed it on 
	 * as a return value of this function 
	 *	 
	 */
	void FetchThumb(CFbsBitmap*& aThumb);


protected:
	CTNEProcessor();

	void ConstructL();

private:
	TInt iThumbProgress;
	CTNEProcessorImpl* iTNEProcessor;
	
	};


#endif // __TNEPROCESSOR_H__

