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
* Description:  A class for TrackLog functionality
 *
*/


#ifndef CTRACKLOG_H_
#define CTRACKLOG_H_

#include <f32file.h>
#include <lbs.h>
#include "clocationrecord.h"
#include <lbssatellite.h>

class MTrackLogObserver;
class CGpxConverterAO;

/**
 * class for recording position min/max values
 */
class TBoundaries
{
public:
	TReal64 minLatitude;
	TReal64 maxLatitude;
	TReal64 minLongitude;
	TReal64 maxLongitude;
	TReal32 distance;
};

/**
* Location trail item class.
*/
class TTrackLogItem
	{
public:
	TTime iTimeStamp;
	TReal64 iLatitude;
	TReal64 iLongitude;
	TReal32 iAltitude;
	TReal32 iHdop;
	TReal32 iVdop;
	TReal32 iCourse;
	TReal32 iQuality;
	TUint32 iNumSatellites;
	void ExternalizeL( RWriteStream& aStream ) const;
	void InternalizeL( RReadStream& aStream );
	};
	
class MGpxConversionObserver
	{
public:
	/**
	 * This method is used to notify about converted GPX files
	 */
	virtual void GpxFileCreated( const TDesC& aFileName, TItemId aTagId, TReal32 aLength,
			TTime aStart, TTime aEnd ) = 0;
	};    

class CTrackLog : public CBase, public MLocationAddObserver
	{

public:
	/**
	 * 2-phased constructor.
	 */
	IMPORT_C static CTrackLog* NewL();

	/**
	 * C++ destructor.
	 * @since S60 3.1
	 */
	IMPORT_C virtual ~CTrackLog();
	
	IMPORT_C TInt GetStatus( TBool& aRecording,	TPositionSatelliteInfo& aFixQuality );
	
	IMPORT_C TBool IsRecording();

	/**
	 * Initialize recording
	 */
	IMPORT_C void StartRecordingL(TItemId aId);
	
	/**
	 * Stop tracklog recording
	 */
	IMPORT_C void StopRecordingL();
	
	/**
	 * stop tracklog recording without saving gpx file
	 */
	IMPORT_C void CancelRecording();
	
	/**
	 * get tracklog file name
	 */
	IMPORT_C void GetTrackLogName(TFileName& aFileName);
	
	/*
	 * from MLocationAddObserver
	 */
	void LocationAdded( const TLocationTrailItem& aTrailItem, const TPositionSatelliteInfo& aSatellites );
		
	IMPORT_C void AddGpxObserver( MGpxConversionObserver* aObserver );
	
	IMPORT_C void StartRecoveryL();
	IMPORT_C TInt AddObserver( MTrackLogObserver* aObserver );
	
	IMPORT_C TInt RemoveObserver( MTrackLogObserver* aObserver );

private:
	/**
	 * C++ constructor.
	 */
	CTrackLog();

	/**
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * Write Tracklog data from buffer to temporary file
	 */
	void WriteBufferToFileL();
	
    /**
     * Read interval value from Central repository
     * @param aKey, Key to item
     * @param aValue, Read value
     */ 
    void ReadCenRepValueL(TInt aKey, TInt& aValue);
	
private:
	/**
	 * Array for storing tracklog items
	 */
	RArray<TTrackLogItem> iTrackLogItemArray;
	
	/**
	 * Array of track log observers.
	 */
	RPointerArray<MTrackLogObserver> iObservers;
	
	TInt iMaxBufferSize;
	TBool iRecording;

	TPositionSatelliteInfo iSatelliteInfo;
	
	TFileName iGpxFileName;
	TFileName iTmpFileName;
	TItemId iTagId;
	
	CGpxConverterAO* iGpxConverter;
	TBoundaries* iBoundaries;
	TCoordinate* lastCoords;
	
	RFs iFs;
	};

#endif /*CTRACKLOG_H_*/
