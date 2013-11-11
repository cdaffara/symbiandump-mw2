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

#include <f32file.h>
#include <s32file.h>
#include <pathinfo.h>
#include "cgpxconverterao.h"
#include "locationmanagerdebug.h"

// ---------------------------------------------------------------------------
// CGpxConverterAO::CGpxConverterAO()
// Default constructor.
// ---------------------------------------------------------------------------
//
CGpxConverterAO::CGpxConverterAO() : CActive( CActive::EPriorityStandard ),
        iTempFile(NULL),
        iBoundaries(NULL)
    {
    }

// ---------------------------------------------------------------------------
// CGpxConverterAO::NewL
// Standard NewL for first phase construction.
// ---------------------------------------------------------------------------
//
CGpxConverterAO* CGpxConverterAO::NewL()
    {
    LOG( "CGpxConverterAO::NewL" );
    CGpxConverterAO* self = new (ELeave) CGpxConverterAO();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CGpxConverterAO::ConstructL
// 2nd phase construction.
// ---------------------------------------------------------------------------
//
void CGpxConverterAO::ConstructL()
    {
    LOG( "CGpxConverterAO::ConstructL ,begin" );
    CActiveScheduler::Add( this );
	User::LeaveIfError( iFs.Connect() );
	
	iWriteBuf = HBufC8::NewL( KWriteBufSize );
	iFormatBuf = HBufC::NewL( KWriteBufSize );
	LOG( "CGpxConverterAO::ConstructL ,end" );
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CGpxConverterAO::~CGpxConverterAO()
    {
    LOG( "CGpxConverterAO::~CGpxConverterAO ,begin" );
    Cancel();
    iFs.Close();
    
    iObservers.Reset();
    
    iFileQueue.ResetAndDestroy();
    iFileQueue.Close();
    
    iBoundQueue.ResetAndDestroy();
    iBoundQueue.Close();

    delete iTempFile;
    iTempFile = NULL;
    delete iBoundaries;
    iBoundaries = NULL;
        
    delete iWriteBuf;
    iWriteBuf = NULL;
    delete iFormatBuf;
    iFormatBuf = NULL;
	LOG( "CGpxConverterAO::~CGpxConverterAO ,end" );
    }

// ---------------------------------------------------------------------------
// CGpxConverterAO::RunError
// From CActive.
// ---------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CGpxConverterAO::RunError( TInt aError )
#else
TInt CGpxConverterAO::RunError( TInt )
#endif
    {
    LOG1( "CGpxConverterAO::RunError with error code: %d", aError );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CGpxConverterAO::DoCancel
// From CActive. 
// ---------------------------------------------------------------------------
//
void CGpxConverterAO::DoCancel()
    {
    LOG( "CGpxConverterAO::DoCancel" );
    }

// ---------------------------------------------------------------------------
// CGpxConverterAO::AddToQueueL
// Adds temp-file and possible pre-calculated boundaries
// into processing queue
// ---------------------------------------------------------------------------
//
void CGpxConverterAO::AddToQueueL( const TDesC& aFileName, TBoundaries* aBoundaries )
	{
	LOG("CGpxConverterAO::AddToQueueL ,begin");
	TFileName *filename = new (ELeave) TFileName(aFileName);
	CleanupStack::PushL( filename );
	iFileQueue.AppendL( filename );
	CleanupStack::Pop( filename );
	iBoundQueue.AppendL( aBoundaries );
	
	if ( iState == EIdle )
		{
		SetState( ENextFile );
		}

	LOG("CGpxConverterAO::AddToQueueL ,end");
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::SetState
 // Set next state for RunL
 // ---------------------------------------------------------------------------
void CGpxConverterAO::SetState( TProcessingState aState )
	{
    LOG1( "CGpxConverterAO::SetState: %d", aState );
	iState = aState;
	if ( !IsActive() )
		{
		iStatus = KRequestPending;
		SetActive();
		TRequestStatus* ptrStatus = &iStatus;
		User::RequestComplete( ptrStatus, KErrNone );
		}
	LOG("CGpxConverterAO::SetState ,end");
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::RunL()
 // ---------------------------------------------------------------------------
void CGpxConverterAO::RunL()
	{
	switch( iState )
		{
		case( EIdle ):
			{
			LOG("CGpxConverterAO::RunL - EIdle");
			// nothing to do here
			break;
			}
		case( ENextFile ):
			{
			LOG("CGpxConverterAO::RunL - ENextFile");
			if( iFileQueue.Count() > 0 )
				{
				TInt err;
				iFixLost = ETrue;
                delete iTempFile;
                iTempFile = NULL;
                delete iBoundaries;
                iBoundaries = NULL;
				// get next temp-file from queue
				iTempFile = iFileQueue[0];
				iBoundaries = iBoundQueue[0];
				err = iReader.Open( iFs, *iTempFile, EFileRead );				
				// remove from queue				
				iFileQueue.Remove( 0 );
				iBoundQueue.Remove( 0 );
				if ( err != KErrNone )
					{
					SetState( ENextFile );
					}
				TRAP( err, ReadTagIdL() );
				
				if ( !iBoundaries )
					{
					SetState( ECrashRecovery );
					}
				else
					{
					SetState( EStartFile );
					}
				}
			else
				{
				// everything done
				SetState( EIdle );
				}
			
			break;
			}
		case( ECrashRecovery ):
			{
			LOG("CGpxConverterAO::RunL - ECrashRecovery");
			TRAPD( readErr, iReader >> iTempItem );
			if ( readErr == KErrNone )
				{
				CalculateBoundaries();
				SetState( ECrashRecovery );
				}
			else
				{
				TInt err;
				iReader.Close();
				err = iReader.Open( iFs, *iTempFile, EFileRead );
				if( err != KErrNone )
					{
					SetState( ENextFile );
					}
				TRAP( err, ReadTagIdL() );
				SetState( EStartFile );
				}
			
			break;
			}
		case( EStartFile ):
			{
			LOG("CGpxConverterAO::RunL - EStartFile");
			_LIT( KExtGPX, ".gpx" );
			TParsePtrC parse( *iTempFile );
			
			TRAPD(err, GetTrackLogPathL( iGpxPath ));
			
			iGpxFileName.Copy( parse.Name() );
			iGpxFileName.Append( KExtGPX );
			iGpxPath.Append( iGpxFileName );
			
			err = iFs.MkDirAll( iGpxPath );
			if ( err != KErrAlreadyExists && err != KErrNone )
				{
				User::Leave( err );
				}
			
			err = iGpxFile.Create(iFs, iGpxPath, EFileWrite);
			if ( err != KErrNone )
				{
				err = iGpxFile.Open(iFs, iGpxPath, EFileWrite);
				if ( err != KErrNone )
					{
					User::Leave( err );
					}
				}

			WriteStartingTags();
			iStartTime = 0;
			iEndTime = 0;
			
			SetState( EProcessing );
			break;
			}
		
		case( EProcessing ):
			{
			LOG("CGpxConverterAO::RunL - EProcessing");
			
			TRAPD( readError, iReader >> iTempItem );
			if ( readError == KErrNone )
				{
				if ( iStartTime == 0 )
					{
					iStartTime = iTempItem.iTimeStamp;
					}
				iEndTime = iTempItem.iTimeStamp;
				WriteItemToFile();				
				SetState( EProcessing );
				}
			else
				{
				// no items left in file
				iReader.Close();
				SetState( ECloseFile );
				}
			break;
			}
		case( ECloseFile ):
			{
			LOG("CGpxConverterAO::RunL - ECloseFile");
			WriteClosingTags();
			iGpxFile.Close();

			TReal distance( 0 );
			if( iBoundaries )
				{
				distance = iBoundaries->distance;
				}
			
			for( TInt i = iObservers.Count() - 1; i >=0; i-- )
				{
				iObservers[i]->GpxFileCreated( iGpxPath, iTagId, distance, iStartTime, iEndTime );
				}
			
			
			iFs.Delete( *iTempFile );
			
			delete iTempFile;
			iTempFile = NULL;
			
			delete iBoundaries;
			iBoundaries = NULL;
			
			SetState( ENextFile );
			break;
			}
		}
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::ReadTagIdL
 // Read Tag Id from temp-file
 // ---------------------------------------------------------------------------
void CGpxConverterAO::ReadTagIdL()
	{
	LOG("CGpxConverterAO::ReadTagIdL ,begin");
	TUint32 low( 0 );
	TUint32 high( 0 );	

	low = iReader.ReadUint32L();
	high = iReader.ReadUint32L();

	iTagId = MAKE_TINT64( high, low );

	LOG("CGpxConverterAO::ReadTagIdL ,end");
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::CalculateBoundaries
 // Calculate min and max coordinates for tracklog
 // ---------------------------------------------------------------------------
void CGpxConverterAO::CalculateBoundaries()
	{
	LOG("CGpxConverterAO::CalculateBoundaries start");
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
	
	if( !Math::IsNaN( iTempItem.iLatitude ) && !Math::IsNaN( iTempItem.iLongitude ))
		{
		TReal32 distance;
		if ( !iLastCoords ) 
			{
			iLastCoords = new TCoordinate( iTempItem.iLatitude, iTempItem.iLongitude );
			}
		else
			{
			TCoordinate tempCoord( iTempItem.iLatitude, iTempItem.iLongitude );
			TLocality newCoords( tempCoord, iTempItem.iHdop );
			TInt err = newCoords.Distance(*iLastCoords, distance);
			if ( err == KErrNone )
				{
				delete iLastCoords;
				iLastCoords = new TCoordinate( tempCoord );
				iBoundaries->distance += distance;
				}
			}		
		iBoundaries->maxLatitude = Max( iBoundaries->maxLatitude, iTempItem.iLatitude );
		iBoundaries->minLatitude = Min( iBoundaries->minLatitude, iTempItem.iLatitude );
		iBoundaries->maxLongitude = Max( iBoundaries->maxLongitude, iTempItem.iLongitude );
		iBoundaries->minLongitude = Min( iBoundaries->minLongitude, iTempItem.iLongitude );
		}
	LOG("CGpxConverterAO::CalculateBoundaries end");
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::WriteStartingTags
 // Writes header tags for GPX file
 // ---------------------------------------------------------------------------
void CGpxConverterAO::WriteStartingTags()
	{
	LOG("CGpxConverterAO::WriteStartingTags ,begin");
	TPtr8 writePtr = iWriteBuf->Des();
	TPtr formatter = iFormatBuf->Des();
	
	// write starting tags
	writePtr.Copy( KTagXml );
	writePtr.Append( KTagGpxStart );
	iGpxFile.Write( writePtr );
	
	writePtr.Copy( KTagMetaStart );
	formatter.Format( KTagName, &iGpxFileName );
	writePtr.Append( formatter );
	iGpxFile.Write( writePtr );
	
	TTime timeStamp( 0 );
	timeStamp.UniversalTime();
	TDateTime datetime = timeStamp.DateTime();
	
	formatter.Format( KTagTimeStamp, datetime.Year(), datetime.Month() + 1, datetime.Day() + 1,
			datetime.Hour(), datetime.Minute(), datetime.Second() );
	writePtr.Copy( formatter );
	iGpxFile.Write( writePtr );
	
	if ( iBoundaries )
		{
		formatter.Format( KTagBounds, iBoundaries->minLatitude, iBoundaries->minLongitude, 
				iBoundaries->maxLatitude, iBoundaries->maxLongitude );
		writePtr.Copy( formatter );
		iGpxFile.Write( writePtr );
		}
	writePtr.Copy( KTagMetaEnd );
	iGpxFile.Write( writePtr );
	
	writePtr.Copy( KTagTrackStart );
	iGpxFile.Write( writePtr );
	LOG("CGpxConverterAO::WriteStartingTags ,end");
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::WriteItemToFile
 // Writes single trackpoint to GPX file
 // ---------------------------------------------------------------------------
void CGpxConverterAO::WriteItemToFile()
	{
	LOG("CGpxConverterAO::WriteItemToFile ,begin");
	TTime timeStamp;
	
	TPtr8 writePtr = iWriteBuf->Des();
	TPtr formatter = iFormatBuf->Des();
	
	if ( Math::IsNaN(iTempItem.iLatitude) || Math::IsNaN(iTempItem.iLongitude) )
		{
		if ( !iFixLost )
			{
			writePtr.Copy( KTagSegmentEnd );
			iGpxFile.Write( writePtr );
			iFixLost = ETrue;
			}
		}
	else
		{
		if ( iFixLost )
			{
			writePtr.Copy( KTagSegmentStart );
			iGpxFile.Write( writePtr );
			iFixLost = EFalse;
			}
		
		// write single track point
		// coordinates
		formatter.Format( KTagTrkPointStart, iTempItem.iLatitude, iTempItem.iLongitude );
		writePtr.Copy( formatter );
		iGpxFile.Write( writePtr );
		// elevation
		if ( !Math::IsNaN( iTempItem.iAltitude ))
			{
			formatter.Format( KTagElevation, iTempItem.iAltitude );
			writePtr.Copy( formatter );
			iGpxFile.Write( writePtr );
			}
		// course
		if ( !Math::IsNaN( iTempItem.iCourse ))
			{
			formatter.Format( KTagCourse, iTempItem.iCourse );
			writePtr.Copy( formatter );
			iGpxFile.Write( writePtr );
			}

		timeStamp = iTempItem.iTimeStamp;
		TDateTime datetime = timeStamp.DateTime();
		
		formatter.Format( KTagTimeStamp, datetime.Year(), datetime.Month() + 1, datetime.Day() + 1,
				datetime.Hour(), datetime.Minute(), datetime.Second() );
		writePtr.Copy( formatter );
		iGpxFile.Write( writePtr );
		
		if ( !Math::IsNaN( iTempItem.iAltitude ))
			{
			writePtr.Copy( KTagFix3D );
			iGpxFile.Write( writePtr );
			}
		else
			{
			writePtr.Copy( KTagFix2D );
			iGpxFile.Write( writePtr );
			}

		// number of satellites
		formatter.Format( KTagSatellites, iTempItem.iNumSatellites );
		writePtr.Copy( formatter );
		iGpxFile.Write( writePtr );

		// accuracy (hdop, vdop)
		if ( !Math::IsNaN( iTempItem.iHdop ))
			{
			formatter.Format( KTagHdop, iTempItem.iHdop );
			writePtr.Copy( formatter );
			iGpxFile.Write( writePtr );
			}
		if ( !Math::IsNaN( iTempItem.iVdop ))
			{
			formatter.Format( KTagVdop, iTempItem.iVdop );
			writePtr.Copy( formatter );
			iGpxFile.Write( writePtr );
			}
	
		// end track point
		writePtr.Copy( KTagTrkPointEnd );
		iGpxFile.Write( writePtr );
		}
	LOG("CGpxConverterAO::WriteItemToFile ,end");
	}

 // ---------------------------------------------------------------------------
 // CGpxConverterAO::WriteClosingTags
 // Close GPX file
 // ---------------------------------------------------------------------------
void CGpxConverterAO::WriteClosingTags()
	{
	LOG("CGpxConverterAO::WriteClosingTags ,begin");
	TPtr8 writePtr = iWriteBuf->Des();
	
	// end segment
	if ( !iFixLost )
		{
		writePtr.Copy( KTagSegmentEnd );	
		iGpxFile.Write( writePtr );
		}

	// closing tags
	writePtr.Copy( KTagTrackEnd );
	writePtr.Append( KTagGpxEnd );

	iGpxFile.Write( writePtr );
	LOG("CGpxConverterAO::WriteClosingTags ,end");
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::GetTrackLogPathL
 // Resolve where to save tracklog file
 // ---------------------------------------------------------------------------
void CGpxConverterAO::GetTrackLogPathL( TDes& aFileName )
	{
	LOG("CGpxConverterAO::GetTrackLogPathL start");
	_LIT( KPathTrackLog, "TrackLog\\" );
	
	TFileName* path = new (ELeave) TFileName();
	CleanupStack::PushL( path );
	
	// check for memory card
	*path = PathInfo::MemoryCardRootPath();
	TLex lex( *path );
	TInt drive;
	iFs.CharToDrive( lex.Get(), drive );
	TVolumeInfo* volumeinfo = new (ELeave) TVolumeInfo;
	
	TInt err = iFs.Volume( *volumeinfo, drive );
	if( err != KErrNone )
		{
		LOG("CGpxConverterAO::GetTrackLogPathL - MMC not available");
		*path = PathInfo::PhoneMemoryRootPath();
		}
	delete volumeinfo;
	
	path->Append( KPathTrackLog );
	
	aFileName.Copy( *path );
	CleanupStack::PopAndDestroy( path );
	LOG("CGpxConverterAO::GetTrackLogPathL end");
	}


 // ---------------------------------------------------------------------------
 // CGpxConverterAO::AddObserver
 // Adds observer for GPX file creation notifications
 // ---------------------------------------------------------------------------
void CGpxConverterAO::AddObserver( MGpxConversionObserver* aObserver )
	{
	iObservers.Append( aObserver );
	}


//End of File
