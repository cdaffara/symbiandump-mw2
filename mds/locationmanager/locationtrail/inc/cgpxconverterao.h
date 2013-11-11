/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Active object for converting internal fileformat to GPX
*
*/


#ifndef CGPXCONVERTERAO_H_
#define CGPXCONVERTERAO_H_

#include <e32base.h>
#include <e32cmn.h>
#include <e32property.h>
#include <f32file.h>
#include <s32file.h>
#include "ctracklog.h"

const TInt KWriteBufSize = 256;

_LIT( KTagXml, "<?xml version=\"1.0\"?>" );
_LIT( KTagGpxStart, "<gpx version=\"1.1\" creator=\"Nokia TrackLog\">" );
_LIT( KTagGpxEnd, "</gpx>" );
_LIT( KTagMetaStart, "<metadata>" );
_LIT( KTagMetaEnd, "</metadata>" );
_LIT( KTagName, "<name>%S</name>" );
_LIT( KTagBounds, "<bounds minlat=\"%f\" minlon=\"%f\" maxlat=\"%f\" maxlon=\"%f\" />" );
_LIT( KTagTrackStart, "<trk>" );
_LIT( KTagTrackEnd, "</trk>" );
_LIT( KTagSegmentStart, "<trkseg>" );
_LIT( KTagSegmentEnd, "</trkseg>" );
_LIT( KTagTrkPointStart, "<trkpt lat=\"%f\" lon=\"%f\">" );
_LIT( KTagTrkPointEnd, "</trkpt>" );
_LIT( KTagElevation, "<ele>%f</ele>" );
_LIT( KTagTimeStamp, "<time>%d-%02d-%02dT%02d:%02d:%02dZ</time>" );
_LIT( KTagCourse, "<course>%f</course>" );
_LIT( KTagFix3D, "<fix>3d</fix>" );
_LIT( KTagFix2D, "<fix>2d</fix>" );
_LIT( KTagSatellites, "<sat>%d</sat>" );
_LIT( KTagHdop, "<hdop>%f</hdop>" );
_LIT( KTagVdop, "<vdop>%f</vdop>" );

class CGpxConverterAO : public CActive
  	{
  	private:
  		enum TProcessingState
  			{
  			EIdle,
  			ENextFile,
  			ECrashRecovery,
  			EStartFile,
  			EProcessing,
  			ECloseFile
  			};
  	
  	public:
        /**
         * Creates and constructs a new instance of CContextBackupSubscriberAO.
         *
         * @return A pointer to the new instance.
         */
        static CGpxConverterAO* NewL();

        /**
         * Destructor.
         */
        virtual ~CGpxConverterAO();

        /**
         * From CActive.
         * This method will be called when Backup & Restore state changes.
         */
        void RunL();

        /**
         * From CActive.
         * Handles a leave occurring in the request completion event handler RunL().
         *
         * @param aError  An error code.
         * @return An error code.
         */
        TInt RunError( TInt aError );

        /**
         * From CActive.
         * Cancels.
         */
        void DoCancel();
        
        /**
         * Adds temp-file and possible pre-calculated boundaries
         * into processing queue
         * @param aFileName  The temp file name
         * @param aBoundaries The pre-calculated boundaries
         */
        void AddToQueueL( const TDesC& aFileName, TBoundaries* aBoundaries = NULL );
        
        /**
         * Adds observer for GPX file creation notifications
         * @param aObserver  The observer that recieves the file creation notifications
         */
        void AddObserver( MGpxConversionObserver* aObserver );

    private:

        /**
         * Default constructor, implicitly called by NewL().
         */
    	CGpxConverterAO();

        /**
         * 2nd phase construction, called by NewL().
         */
        void ConstructL();
        
        /**
         * Set next state for RunL
         * @param aState The next processing state
         */
        void SetState( TProcessingState aState );
        
        /**
         * Resolve where to save tracklog file
         * aFileName The name of the file
         */
        void GetTrackLogPathL( TDes& aFileName );
        
        /**
         * Read Tag Id from temp-file
         */
        void ReadTagIdL();
        
        /**
         * Calculate min and max coordinates for tracklog
         */
        void CalculateBoundaries();
        
        /**
         * Writes header tags for GPX file
         */
        void WriteStartingTags();
        
        /**
         * Writes single trackpoint to GPX file
         */
        void WriteItemToFile();
        
        /**
         * Close GPX file 
         */
        void WriteClosingTags();
        
    private: // data
    
    	TInt iState;
    	
    	RPointerArray<TFileName> iFileQueue;
    	RPointerArray<TBoundaries> iBoundQueue;
    	
    	TFileName* iTempFile;    	
    	TBoundaries* iBoundaries;
    	TCoordinate* iLastCoords;
    	TFileName iGpxFileName;
    	TFileName iGpxPath;
    	
    	RFs iFs;
    	RFileReadStream iReader;
    	RFile64 iGpxFile;
    	
    	TTrackLogItem iTempItem;
    	
    	HBufC8* iWriteBuf;
    	HBufC* iFormatBuf;
    	
    	TBool iFixLost;
    	TItemId iTagId;
    	
    	TTime iStartTime;
    	TTime iEndTime;
    	
    	RPointerArray<MGpxConversionObserver> iObservers;

    };

#endif /*CGPXCONVERTERAO_H_*/
