/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Reads metadata from rtp file meta header
*
*/

#ifndef C_HARVESTERRTPMETADATAREADER_H
#define C_HARVESTERRTPMETADATAREADER_H

#include <e32base.h>
#include <f32file.h>
#include <etelmm.h>

// CONSTANTS
const TInt KIntegerBytes ( 4 );
const TInt KStringLengthBytes( 1 );
const TInt KMaxMetaHeaderLength( 2048 );
const TInt KMetaLengthPoint( 0 );
const TInt KAttributesPoint( KMetaLengthPoint + KIntegerBytes ); //4
const TInt KPlayCountPoint( KAttributesPoint + KIntegerBytes );  // 8
const TInt KPlaySpotPoint( KPlayCountPoint + KIntegerBytes );    // 12
const TInt KReservedPoint1( KPlaySpotPoint + KIntegerBytes );    // 16
const TInt KReservedPoint2( KReservedPoint1 + KIntegerBytes );   // 20
const TInt KReservedPoint3( KReservedPoint2 + KIntegerBytes );   // 24
const TInt KReservedPoint4( KReservedPoint3 + KIntegerBytes );   // 28
const TInt KStartTimePoint( KReservedPoint4 + KIntegerBytes );   // 32
const TInt KEndTimePoint( KStartTimePoint + 2 * KIntegerBytes ); // 40
const TInt KDurationPoint( KEndTimePoint + 2 * KIntegerBytes );  // 48
const TInt KSeekArrayPoint( KDurationPoint + KIntegerBytes );    // 52
const TInt KUserIdPoint( KSeekArrayPoint + KIntegerBytes );      // 56
const TInt KDeviceInfoPoint( KUserIdPoint + KStringLengthBytes +
                             RMobilePhone::KIMSISize );          // 72

const TInt KSpaceLength( 1 );

const TInt KContentRightsLockToDevice( 2 );
const TInt KPartiallyFailedLimit ( 60 );

// Metaheader attributes
const TInt KOngoingFlagShift( 0 );
const TInt KCompletedFlagShift( 1 );
const TInt KProtectedFlagShift( 2 );
const TInt KFailedFlagShift( 3 );
const TInt KVersionFieldShift( 4 );
const TInt KQualityFieldShift( 8 );
const TInt KPostRuleFieldShift( 16 );
const TInt KParentalFieldShift( 24 );

//DATA TYPES
typedef enum // Video flags
    {
    //Duplicates in CIptvMyVideosVideoBriefDetails.h:
    //DO NOT CHANGE
    EIptvMyVideosVideoTypeRecording      = 0x00000400, // Video type is recording. 
    EIptvMyVideosVideoIsRecording        = 0x00000800, // Video is being recorded. 
    EIptvMyVideosVideoRecPartiallyFailed = 0x00020000, // Recording partially failed 
    EIptvMyVideosVideoRecFailed          = 0x00040000  // Recording failed
    } TIptvMyVideosVideoFlags;

class CHarvesterRtpClipDetails;

/**
 *  Rtp Meta data reader class definition
 *
 *  @lib harvesterrtpplugin.dll
 *  @since S60 S60 v3.x
 */
class CHarvesterRtpMetaDataReader : public CBase
    {
public:

    /**
     * Constructs a new CHarvesterRTPPlugin implementation.
     *  
     * @param aFileBuffer File data buffer
     * @return A pointer to CHarvesterRtpMetaDataReader
     */
    static CHarvesterRtpMetaDataReader* NewL( const TDesC8* aFileBuffer );

    /**
    * Destructor.
    */
    virtual ~CHarvesterRtpMetaDataReader();

    /**
    * Get metadata of rtp clip
    * 
    * @param aDetails Clip metadata from metadata header
    */
    void GetClipDetailsL( CHarvesterRtpClipDetails& aDetails );

private:

    /**
    * C++ constructor
    * @param aFileBuffer File data buffer
    */
    CHarvesterRtpMetaDataReader( const TDesC8* aFileBuffer );

    /**
    * 2nd phase constructor
    */
    void ConstructL();

    /**
    * Reads any string with one byte len info from meta header.
    * @since 
    * @param aPosition a position from to read
    * @param aData a data to read
    * @return None.
    */
    void ReadStringDataL( const TInt aPosition, TPtrC8& aData );

    /**
    * Converts bytes to integer.
    * @since Series 60 3.0
    * @param aBytes a buffer to convert.
    * @param aValue a integer converted from bytes.
    * @return a system wide error code.
    */
    TInt GetValue( const TDesC8& aBytes, TUint& aValue );

    /**
    * Get channel name and program name from meta header. 
    * Format to one string. Leaves if cannot read 
    * data from meta header.
    *
    * @param aTitle On return, clip title
    */
    void GetProgramTitleL( TDes& aTitle );

private: // data

    /**
    * Meta total length.
    */
    TInt iMetaTotal;

    /**
    * Data buffer.
    * Own.
    */
    HBufC8* iMetaData;

    /**
    * File data buffer pointer.
    */
    TPtr8 iDataPtr;
    
    /**
    * ESG data point.
    */
    TInt iEsgDataPoint;

    /**
    * File data buffer
    * Not own.
    */
    const TDesC8* iFileBuffer;
    };

#endif // C_HARVESTERRTPMETADATAREADER_H
