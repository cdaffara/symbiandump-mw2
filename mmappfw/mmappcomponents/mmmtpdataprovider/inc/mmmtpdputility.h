/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definition for MTP Utility File
*
*/


#ifndef MMMTPDPUTILITY_H
#define MMMTPDPUTILITY_H

#include <mtp/mtpprotocolconstants.h>
#include <mtpprivatepskeys.h>

#include "mmmtpdpfiledefs.h"

// forward declacration
class RFs;

class MmMtpDpUtility
    {
public:
    /**
    * Utility function to get the type of an object from the filename
    * @param aFullFileName, full path of file name
    * @return TMTPFormatCode for the passed file name
    */
    IMPORT_C static TMTPFormatCode FormatFromFilename( const TDesC& aFullFileName );

    /**
    * Utility function to decide if has metadata
    * @param aObjFormatCode, object format code
    * @return TBool for decide if has metadata
    */
    static TBool HasMetadata( TUint16 aObjFormatCode );

    /**
    * Utility function to decide if there is any reference
    * @param aObjFormatCode, object format code
    * @return TBool for decide if there are some references
    */
    static TBool HasReference( TUint16 aObjFormatCode );

    /**
    * Check the filename length to see if it exceeds Symbian 256 limit.
    * @param aPathName, the path name of the file
    * @param aFileName, the file to be check
    * @return TBool for decide if the file name validate
    */
    static TBool ValidateFilename( const TDesC& aPathName,
        const TDesC& aFileName );

    /**
    * Check the file size, if file doesn't exist, leave
    * @param aFs, handle of file server session
    * @param aFileName, file name
    * @return TUint64 size for specified file
    */
    static TUint64 GetObjectSizeL( RFs& aFs, const TDesC& aFileName );

    /**
    * Get file date modified info
    * @param aFs , handle of file server session
    * @param aFullFileName, file name
    * @param aDateModified, MTP time format (YYYYMMDDTHHMMSS) for the file
    */
    static void GetObjectDateModifiedL( RFs& aFs, const TDesC& aFullFileName, TDes& aDateModified );


    /**
    * Converts a date time string in the internal MTP format (YYYYMMDDTHHMMSS)
    * to a TTime.
    * @param aDateTime string, containing date and time
    * @param aTime, the TTime objec if the date time string is parsed correctly
    * @return error code 
    */
    static TInt DesToTTime( const TDesC& aDateTime, TTime& aTime );

    /**
    * Check if file is read-only, if file doesn't exist, leave
    * @param aFs , handle of file server session
    * @param aFullFileName, the file to be check
    */
    static TUint16 GetProtectionStatusL( RFs& aFs, const TDesC& aFullFileName );

    /**
    * Check if prop type and data type match
    * @param aPropertyCode, the proptery code
    * @param aDataType, the data type
    * @param TMTPResponseCode, decide if match
    */
    static TMTPResponseCode CheckPropType( TUint16 aPropertyCode,
            TUint16 aDataType );

    /**
    * Update object file name
    * @param aFs
    * @param aPathName, the path name of the file
    * @param aFullFileName, the file to be check
    */
    static TInt UpdateObjectFileName( RFs& aFs,
        const TDesC& aFullFileName,
        TDes& aNewName );

    /**
    * Get the mime type of file, support mp4/3gp now
    * @param aFullPath, full path of file
    * @return mime type string, if return NULL, no mime or failed
    */
    static HBufC8* ContainerMimeType( const TDesC& aFullPath );

    /**
    * Get the Mime from Mp4 file
    * @param aFullFileName, full path of file
    * @return mime type string, if return NULL, no mime or failed
    */
    static HBufC8* Mp4MimeTypeL( const TDesC& aFullFileName );

    /**
    * Get the Mime from Odf file
    * @param aFullFileName, full path of file
    * @return mime type string, if return NULL, no mime or failed
    */
    static HBufC8* OdfMimeTypeL( const TDesC& aFullFileName );

    /**
    * Get the Mime from Asf file
    * @param aFullFileName, full path of file
    * @return mime type string, if return NULL, no mime or failed
    */
    static HBufC8* AsfMimeTypeL( const TDesC& aFullFileName );

    /**
    * get the sub format code by mime
    * @param aMimeType, mime type
    * @param aSubFormatCode, output the sub format code
    * @return if success, return KErrNone, otherwise, error code
    */
    static TMmMtpSubFormatCode SubFormatCodeFromMime( const TDesC8& aMimeType );

    /**
    * Get DRM status
    * @param aFullFileName, full file name
    * @return return DRM status code
    */
    IMPORT_C static TInt GetDrmStatus( const TDesC& aFullFileName );
    
    /**
    * Set MTP PS Status
    * @param aStatus, next TMtpPSStatus
    *
    * newly implemented function to as enabler
    * to provide more state transition on application (MusicPlayer/MPX) during MTP by fully utilize the 3 PSStatus:
    *
    * EMtpPSStatusActive, == MTP operation related Media Files is performaning
    * EMtpPSStatusUninitialized == MTP not in use
    * EMtpPSStatusReadyToSync == MTP is connected, but no active operation related to Media Files
    *
    * this CAN enable 2 new behaviors:
    * 1. MTP status monitor is extracted out from USB Event Observer in harvester
    *    Harvester and Music player should be fully blocked if PS Status is EMtpPSStatusActive
    *    Music Player would prevent refresh/delete/add on UI level if PS Status is EMtpPSStatusReadyToSync  
    * 2. collectioncachedhelper to observed this status.
    *    when the ps status changed from EMtpPSStatusActive to EMtpPSStatusReadyToSync, it starts timer.
    *    and when it is reached, it flush all cache.
    *    the timer is canceled when ps status is back to active.
    */
    IMPORT_C static void SetPSStatus( TMtpPSStatus aStatus );

    /**
    * Set MTP PS Status (actually)
    * @param aStatus, next TMtpPSStatus
    */
    static void DoSetPSStatus( TMtpPSStatus aStatus );

    };

#endif // MMMTPDPUTILITY_H
