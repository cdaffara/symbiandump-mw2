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

#include "ctracklog.h"
#include "locationmanagerdebug.h"
#include "locationtraildefs.h"
#include "rtracklog.h"
#include <f32file.h>
#include <s32file.h>
#include <pathinfo.h>
#include <centralrepository.h>
#include <locationeventdef.h>
#include "cgpxconverterao.h"

EXPORT_C CTrackLog* CTrackLog::NewL()
	{
    CTrackLog* self = new (ELeave) CTrackLog();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}

void CTrackLog::ConstructL()
	{
	LOG("CTrackLog::ConstructL start");
	
	iGpxConverter = CGpxConverterAO::NewL();
	
    TInt bufSize( 0 );
    TRAPD( err, ReadCenRepValueL( KTrackLogSizeKey, bufSize ));
    LOG1("CLocationManagerServer::ConstructL, cenrep bufsize value:%d", bufSize);
    
    if ( err != KErrNone )
    	{
        LOG1("CTrackLog::ConstructL, cenrep err:%d", err);
        iMaxBufferSize = KTrackLogBufSize;
    	}
    else
    	{
    	iMaxBufferSize = bufSize;
    	}
	User::LeaveIfError( iFs.Connect() );
	
	LOG("CTrackLog::ConstructL end");
	}

CTrackLog::CTrackLog() :
	iMaxBufferSize( KTrackLogBufSize ),
	iRecording( EFalse )
	{
	}

EXPORT_C CTrackLog::~CTrackLog()
	{
	delete iGpxConverter;
	iGpxConverter = NULL;
	iFs.Close();
	}

EXPORT_C void CTrackLog::StartRecordingL(TItemId aId)
	{
	LOG("CTrackLog::StartRecording start");
	_LIT( KExtTmp, ".tmp" );
	_LIT( KExtGpx, ".gpx" );
	
	iTagId = aId;
	iFs.PrivatePath( iTmpFileName );
	TInt err;
	err = iFs.MkDirAll( iTmpFileName );
	if ( err != KErrAlreadyExists && err != KErrNone )
		{
		User::Leave( err );
		}

	TTime now( 0 );
	now.HomeTime();
	TDateTime datetime = now.DateTime();
	
	_LIT( KGpxFileName, "tracklog%d%02d%02d%02d%02d%02d" );
	HBufC* fileName = HBufC::NewL( KMaxFileName );
	CleanupStack::PushL( fileName );
	TPtr namePtr = fileName->Des();
	
	namePtr.Format(KGpxFileName, datetime.Year(), datetime.Month() + 1, datetime.Day() + 1,
			datetime.Hour(), datetime.Minute(), datetime.Second() );

	iTrackLogItemArray.Reset();
	iRecording = ETrue;
	iGpxFileName.Copy( namePtr );
	iTmpFileName.Append( iGpxFileName );

	iGpxFileName.Append( KExtGpx );
	iTmpFileName.Append( KExtTmp );	
	
	WriteBufferToFileL();
	
	CleanupStack::PopAndDestroy( fileName );
	LOG("CTrackLog::StartRecording end");
	}

EXPORT_C void CTrackLog::StopRecordingL()
	{
	LOG("CTrackLog::StopRecording start");
	
	// "flush" buffer
	WriteBufferToFileL();
	iTrackLogItemArray.Reset();
	// gpx converter will clean up boundaries
	iGpxConverter->AddToQueueL( iTmpFileName, iBoundaries );
	
	iBoundaries = NULL;
	iRecording = EFalse;
	LOG("CTrackLog::StopRecording end");
	}

EXPORT_C void CTrackLog::CancelRecording()
	{
	LOG("CTrackLog::CancelRecording start");
	iRecording = EFalse;
	iTrackLogItemArray.Reset();
	iFs.Delete( iTmpFileName );
	LOG("CTrackLog::CancelRecording end");
	}

void CTrackLog::LocationAdded( const TLocationTrailItem& aTrailItem, 
							   const TPositionSatelliteInfo& aSatellites )
	{
	LOG("CTrackLog::LocationAdded start");

	const TInt KMaxLat = 90;
	const TInt KMinLat = -90;
	const TInt KMaxLon = 180;
	const TInt KMinLon = -180;
	if ( !iBoundaries )
		{
		iBoundaries = new TBoundaries;	
		iBoundaries->minLatitude = KMaxLat;
		iBoundaries->maxLatitude = KMinLat;
		iBoundaries->minLongitude = KMaxLon;
		iBoundaries->maxLongitude = KMinLon;
		iBoundaries->distance = 0;
		}
	
	if ( IsRecording() )
		{
		TTrackLogItem newItem;
		newItem.iTimeStamp = aTrailItem.iTimeStamp;
		newItem.iLatitude = aTrailItem.iLocationData.iPosition.Latitude();
		newItem.iLongitude = aTrailItem.iLocationData.iPosition.Longitude();
		newItem.iAltitude = aTrailItem.iLocationData.iPosition.Altitude();
		newItem.iHdop = aSatellites.HorizontalDoP();
		newItem.iVdop = aSatellites.VerticalDoP();
		newItem.iCourse = aTrailItem.iLocationData.iCourse.Course();
		newItem.iQuality = aTrailItem.iLocationData.iQuality;
		newItem.iNumSatellites = aTrailItem.iLocationData.iSatellites;

		iTrackLogItemArray.Append(newItem);
		
		// Keep the latest satellite information safe.
		iSatelliteInfo = aSatellites;
		
		// Check if the GPS fix is lost or buffer is full.
		// If true, then write buffer to a file.
		if ( Math::IsNaN(newItem.iLatitude) || Math::IsNaN(newItem.iLongitude) ||
				iTrackLogItemArray.Count() > iMaxBufferSize )
			{
			LOG("CTrackLog::LocationAdded - buffer full, writing to file");
			TRAPD( err, WriteBufferToFileL() );
			if( err != KErrNone )
				{
				LOG1("WriteBufferToFileL leave error: %d", err);
				}
			iTrackLogItemArray.Reset();
			}
		if( !Math::IsNaN( newItem.iLatitude ) && !Math::IsNaN( newItem.iLongitude ) )
			{
			TReal32 distance;
			if ( !lastCoords )
				{
				lastCoords = new TCoordinate( aTrailItem.iLocationData.iPosition );
				}
			else
				{
				TLocality newCoords(aTrailItem.iLocationData.iPosition,
						aTrailItem.iLocationData.iPosition.HorizontalAccuracy()	);
				TInt err = newCoords.Distance(*lastCoords, distance);
				if ( err == KErrNone )
					{
					delete lastCoords;
					lastCoords = NULL;
					lastCoords = new TCoordinate( aTrailItem.iLocationData.iPosition );
					iBoundaries->distance += distance;
					}
				}
			iBoundaries->maxLatitude = Max( iBoundaries->maxLatitude, newItem.iLatitude );
			iBoundaries->minLatitude = Min( iBoundaries->minLatitude, newItem.iLatitude );
			iBoundaries->maxLongitude = Max( iBoundaries->maxLongitude, newItem.iLongitude );
			iBoundaries->minLongitude = Min( iBoundaries->minLongitude, newItem.iLongitude );
			}
		}
	LOG("CTrackLog::LocationAdded end");	
	}

void CTrackLog::WriteBufferToFileL()
	{
	LOG("CTrackLog::WriteBufferToFileL start");

	RFile64 file;
	RFileWriteStream writer;
	
	CleanupClosePushL( writer );
	
	TInt err;
	err = file.Open( iFs, iTmpFileName, EFileRead );
	if ( err != KErrNone )
		{
		err = writer.Create( iFs, iTmpFileName, EFileWrite );
		if( err != KErrNone )
			{
			User::Leave( err );
			}
		writer << I64LOW( static_cast<TUint64>(iTagId) );
		writer << I64HIGH( static_cast<TUint64>(iTagId) );
		}
	else 
		{
		CleanupClosePushL( file );
		TInt64 endpos( 0 );
		file.Seek( ESeekEnd, endpos );
		CleanupStack::PopAndDestroy( &file );
		err = writer.Open( iFs, iTmpFileName, EFileWrite );
		if( err != KErrNone )
			{
			User::Leave( err );
			}
		writer.Sink()->SeekL( MStreamBuf::EWrite, TStreamPos( endpos ));
		}
	
	TInt count = iTrackLogItemArray.Count();
	
	for( TInt i = 0; i < count; i++ )
		{
		writer << iTrackLogItemArray[i];
		}
	
	writer.CommitL();
	
	CleanupStack::PopAndDestroy( &writer );
	LOG("CTrackLog::WriteBufferToFileL end");
	}

EXPORT_C TInt CTrackLog::GetStatus( TBool& aRecording, TPositionSatelliteInfo& aFixQuality )
	{
	aFixQuality = iSatelliteInfo;
	aRecording = IsRecording();
	
	return KErrNone;
	}

EXPORT_C TBool CTrackLog::IsRecording()
	{
	return iRecording;
	}

EXPORT_C void CTrackLog::GetTrackLogName(TFileName& aFileName)
	{
	aFileName = iGpxFileName;
	}

EXPORT_C void CTrackLog::AddGpxObserver( MGpxConversionObserver* aObserver )
	{
	iGpxConverter->AddObserver( aObserver );
	}

EXPORT_C void CTrackLog::StartRecoveryL()
	{
	_LIT( KWildTmp, "*.tmp" );

	TInt err;
	CDir* files;
	TFileName *filename = new (ELeave) TFileName();
	TFindFile finder( iFs );
	iFs.PrivatePath( iTmpFileName );
	err = finder.FindWildByDir( KWildTmp, iTmpFileName, files);
	if ( err == KErrNone )
		{
		TInt count = files->Count();
		for( TInt i = 0; i < count; i++ )
			{
			filename->Copy(iTmpFileName);
			filename->Append((*files)[i].iName);
			TRAP_IGNORE( iGpxConverter->AddToQueueL( *filename, NULL ));
			}
		}
	delete filename;
	filename = NULL;
	delete files;
	files = NULL;
	}

void CTrackLog::ReadCenRepValueL(TInt aKey, TInt& aValue)
	{
	LOG( "LocationManagerServer::ReadCenRepValueL(), begin" );
	CRepository* repository;
	repository = CRepository::NewLC( KRepositoryUid );
	User::LeaveIfError(repository->Get( aKey, aValue));
	CleanupStack::PopAndDestroy(repository);
    LOG( "LocationManagerServer::ReadCenRepValueL(), end" );   
	}


void TTrackLogItem::ExternalizeL( RWriteStream& aStream ) const
	{
	aStream.WriteReal64L( iLatitude );
	aStream.WriteReal64L( iLongitude );
	aStream.WriteReal32L( iAltitude );
	aStream.WriteReal32L( iCourse );	
	aStream.WriteReal32L( iQuality );	
	aStream.WriteUint32L( iNumSatellites );
	aStream.WriteReal32L( iHdop );
	aStream.WriteReal32L( iVdop );
	aStream.WriteUint32L( I64LOW( iTimeStamp.Int64() ));
	aStream.WriteUint32L( I64HIGH( iTimeStamp.Int64() ));
	}

void TTrackLogItem::InternalizeL( RReadStream& aStream )
	{
	iLatitude = aStream.ReadReal64L();
	iLongitude = aStream.ReadReal64L();
	iAltitude = aStream.ReadReal32L();
	TReal32 realVal;
	realVal = aStream.ReadReal32L();
	iCourse = realVal;
	realVal = aStream.ReadReal32L();
	iQuality = realVal;
	TUint32 satellites;
	satellites = aStream.ReadUint32L();
	iNumSatellites = satellites;
	realVal = aStream.ReadReal32L();
	iHdop = realVal;
	realVal = aStream.ReadReal32L();
	iVdop = realVal;
	TUint32 low( 0 );
	TUint32 high( 0 );
	low = aStream.ReadUint32L();
	high = aStream.ReadUint32L();
	TInt64 timestamp( 0 );
	timestamp = MAKE_TINT64( high, low );
	iTimeStamp = timestamp;
	}
