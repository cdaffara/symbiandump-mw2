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
* Description:  Common utilities for the Metadata System.
*
*/

#ifndef MDSUTILS_H
#define MDSUTILS_H

#include <ecom/implementationinformation.h>

#include <e32math.h>

const TInt KMdEErrHarvestingFailed = 300; 			// Error occured while harvesting - retry in case of this error
const TInt KMdEErrHarvestingFailedPermanent = 301;  // Error occured while harvesting - do not retry harvesting
const TInt KMdEErrHarvestingFailedUnknown = 302;    // Unknown error occurred - do not retry harvesting 
    
namespace MdsUtils
    {
    
    /**
     * CleanupPtrArray function is used for cleanup support of locally declared arrays.
     *
     * @param aArray  An array to cleanup
     */
    template<typename T> inline void CleanupPtrArray( TAny* aArray )
        {
        static_cast<RPointerArray<T>*>(aArray)->ResetAndDestroy();
        static_cast<RPointerArray<T>*>(aArray)->Close();
        }
        
    /**
     * CleanupEComArray function is used for cleanup support of locally declared arrays.
     *
     * @param aArray  An array to cleanup
     */
    inline void CleanupEComArray( TAny* aArray )
        {
        CleanupPtrArray<CImplementationInformation>( aArray );
        }

    /**
     * Get file volume info from the file system.
     * @param aUri  File URI.
     * @param aVolumeInfo  Reference to the object where volume info is stored to.
     */
    inline TInt GetVolumeInfo(const RFs& aFs, const TDesC& aUri, TVolumeInfo& aVolumeInfo )
        {
        if ( aUri.Size() <= 0 )
            {
            return KErrArgument;
            }

        TInt driveNumber( 0 );
        TInt error( 0 );

        error = aFs.CharToDrive( aUri[0], driveNumber );

        if ( error != KErrNone )
            {
            return error;
            }

        error = aFs.Volume( aVolumeInfo, driveNumber );

        return error;
        }

    /**
     * Returns a 32-bit unsigned integer from the location pointed by
     * the parameter.
     * @param aPointer  Memory pointer (input).
     * @return Converted integer.
     */
    inline TUint32 ToUInt32L( TUint8* aPointer )
        {
        if ( !aPointer )
            {
            User::Leave( KErrGeneral );
            }
            
        TUint32 ret = *( aPointer + 3 );
        ret = ( ret << 8 ) + *( aPointer + 2 );
        ret = ( ret << 8 ) + *( aPointer + 1 );
        ret = ( ret << 8 ) + *aPointer;
        return ret;
        }

    /**
     * Returns a 16-bit unsigned integer from the location pointed by
     * the parameter.
     * @param aPointer  Memory pointer (input).
     * @return Converted integer.
     */
    inline TUint16 ToUInt16L( TUint8* aPointer )
        {
        if ( !aPointer )
            {
            User::Leave( KErrGeneral );
            }
            
        TUint16 ret = *( aPointer + 1 );
        ret = STATIC_CAST( TUint16, ( ret << 8 ) + *aPointer );
        return ret;
        }

    /**
     * Converts GPS coordinates from Exif degrees to decimal representation.
     * @param aSourceDes  Source descriptor.
     * @param aCoordinate  Output coordinate (latitude or longitude).
     */
    inline void ConvertFromDegreesToDecimalL( const TDes8& aSourceDes, TReal64& aCoordinate )
        {
        if ( aSourceDes.MaxSize() < 24 )
            {
            User::Leave( KErrArgument );
            }

        TUint8* ptr = CONST_CAST( TUint8*, aSourceDes.Ptr() );

        TUint32 degrees         = ToUInt32L( ptr );
        TUint32 deg_denominator = ToUInt32L( ptr + 4 );
        TUint32 minutes         = ToUInt32L( ptr + 8 );
        TUint32 min_denominator = ToUInt32L( ptr + 12 );
        TUint32 seconds         = ToUInt32L( ptr + 16 );
        TUint32 sec_denominator = ToUInt32L( ptr + 20 );

        // check that coordinate doesn't contain division by zero
        // in those numerators which are not 0
        if ( ( deg_denominator == 0 && degrees ) || 
        	 ( min_denominator == 0 && minutes ) || 
        	 ( sec_denominator == 0 && seconds ) )
        	{
        	User::Leave( KErrCorrupt );
        	}

        aCoordinate = 0.0;

        if( degrees )
        	{
            TReal64 degreesReal = degrees;
            TReal64 deg_denominatorReal = deg_denominator;
        	aCoordinate += degreesReal / deg_denominatorReal;
        	}
        if( minutes )
        	{
            TReal64 minutesReal = minutes;
            TReal64 min_denominatorReal = min_denominator;
        	aCoordinate += minutesReal / (min_denominatorReal * 60.0);
        	}
        if( seconds )
        	{
            TReal64 secondsReal = seconds;
            TReal64 sec_denominatorReal = sec_denominator;
        	aCoordinate += secondsReal / (sec_denominatorReal * 3600.0);
        	}
        }

    /**
     * Converts GPS coordinates from decimal to degree representation used by Exif.
     * Method leaves with KErrArgument if max size of aTgtDes < 24.
     * @param aCoordinate  Input coordinate (latitude or longitude).
     * @param aTgtDes  Output descriptor.
     */
    inline void ConvertFromDecimalToDegreesL( TReal64 aCoordinate, TDes8& aTgtDes )
        {
        if ( aTgtDes.MaxSize() < 24 )
            {
            User::Leave( KErrArgument );
            }

        const TUint32 KDenominator1 = 1;
        const TUint32 KDenominator10M = 10000000;
        const TReal64 KDenominator10MReal = KDenominator10M;

        TReal64 degs64 = aCoordinate;

        TReal64 mins64 = 0.0;
        Math::Frac( mins64, degs64 );
        mins64 *= 60.0;

        TReal64 secs64 = 0.0;
        Math::Frac( secs64, mins64 );
        secs64 *= 60.0 * KDenominator10MReal;

        TUint32 degrees = (TUint32)( degs64 );
        TUint32 minutes = (TUint32)( mins64 );
        TUint32 seconds = (TUint32)( secs64 );

        aTgtDes.Append( (TUint8*) &degrees, 4 );
        aTgtDes.Append( (TUint8*) &KDenominator1, 4 );

        aTgtDes.Append( (TUint8*) &minutes, 4 );
        aTgtDes.Append( (TUint8*) &KDenominator1, 4 );

        aTgtDes.Append( (TUint8*) &seconds, 4 );
        aTgtDes.Append( (TUint8*) &KDenominator10M, 4 );
        }

    inline TBool IsValidProcessId( const TUid aUid )
        {
        const TUint32 KMinProcessId = 0x10000000;
        const TUint32 KMaxProcessId = 0xFFFFFFFF;

        if ( aUid.iUid < KMinProcessId || aUid.iUid > KMaxProcessId )
            {
            return EFalse;
            }
        return ETrue;    
        }
    
    /**
     * Converts trap errors to harvesting errors.
     * @param aTrapError  Trapped error to convert
     * @param aPluginError  Output error
     */
    inline void ConvertTrapError( TInt aTrapError, TInt &aPluginError )
        {
        aPluginError = KErrNone;
        if ( aTrapError == KErrArgument ||        // -6
            aTrapError == KErrCorrupt ||          // -20
            aTrapError == KErrAccessDenied ||     // -21
            aTrapError == KErrPermissionDenied || // -46
            aTrapError == KErrNotFound ||         // -1
            aTrapError == KErrNotSupported )      // -5
            {
            aPluginError = KMdEErrHarvestingFailedPermanent;    
            }
        else if ( aTrapError == KErrGeneral ||       // -2
        		 aTrapError == KErrNoMemory ||       //-4
                 aTrapError == KErrInUse ||          // -14
                 aTrapError == KErrServerBusy ||     // -16
                 aTrapError == KErrLocked ||         // -22
                 aTrapError == KErrCouldNotConnect ) // -34
            {
            aPluginError = KMdEErrHarvestingFailed;
            }
        else
            {
            aPluginError = KMdEErrHarvestingFailedUnknown;
            }
        }

    /**
     * Compare descriptors locale-independent
     * 
     * @param aDes1 first descriptor to compare
     * @param aDes2 second descriptor to compare
     * 
     * @return Positive, if this descriptor is greater than the specified 
     *         descriptor. Negative, if this descriptor is less than the 
     *         specified descriptor. Zero, if both descriptors have the same 
     *         length and the their contents are the same.
     */
    inline TInt Compare(const TDesC& aDes1, const TDesC& aDes2)
    	{
    	return aDes1.CompareF( aDes2 );
    	}
    
    /**
     * Find descriptor locale-independent
     * 
     * @param aWhereDes descriptor where to search
     * @param aWhatDes descriptor what to search
     * 
     * @return The offset of the data sequence from the beginning of this 
     *         descriptor's data. KErrNotFound, if the data sequence cannot be 
     *         found. Zero, if the length of the search data sequence is zero.
     */
    inline TInt Find(const TDesC& aWhereDes, const TDesC& aWhatDes)
    	{
    	return aWhereDes.FindF( aWhatDes );
    	}
    
    /**
     * Check if file exist in file system. Requires AllFiles capability.
     * 
     * @param aFs handle to file server session
     * @param aFilename filename
     * 
     * @return Does file exist
     */
    inline TBool FileExists(RFs& aFs, const TDesC& aFilename)
    	{
    	TUint fileAttributes;
    	// Att method is used instead of Entry method 
    	// because smaller stack memory usage (4 bytes vs. 548 bytes).
    	// There is no performance difference between methods Att and Entry.
    	return KErrNone == aFs.Att( aFilename, fileAttributes );
    	}

	/**
     * Get name from filename. For example 'Test' is returned from 
     * 'C:\Data\Test.jpg'. aFilename must contain drive letter, path and 
     * filename (file extension is not required).
     *
     * @param aFilename filename
     * @param aName returned name
     *
     * @return Does name exist (if true, lenght of aName is > 0)
     */
    inline TBool GetName(const TDesC& aFilename, TPtrC& aName)
     	{
     	// find name (everything after last back slash)
     	TInt pos = aFilename.LocateReverseF( '\\' );
     	if( pos >= 0 )
     		{
     		aName.Set( aFilename.Mid( pos + 1 ) );

     		// remove extension
     		TInt pos = aName.LocateReverseF( '.' );
     		if( pos >= 0 )
     			{
     			aName.Set( aName.Left( pos ) );
     			}

     		if( aName.Length() > 0 )
     			{
     			return ETrue;
     			}
     		}
     	
     	return EFalse;
     	}

	/**
     * Get name and extension from filename. For example 'Test.jpg' is 
     * returned from 'C:\Data\Test.jpg'. aFilename must contain drive letter, 
     * path and filename (file extension is not required).
     *
     * @param aFilename filename
     * @param aNameExt returned name and extension
     *
     * @return Does name and extension exist (if true, lenght of aNameExt is > 0)
     */
    inline TBool GetNameExt(const TDesC& aFilename, TPtrC& aNameExt)
     	{
     	// find name (everything after last back slash)
		TInt pos = aFilename.LocateReverseF( '\\' );
		if( pos >= 0 )
			{
			aNameExt.Set( aFilename.Mid( pos + 1 ) );
			
			if( aNameExt.Length() > 0 )
				{
				return ETrue;
				}
			}

		return EFalse;
     	}

	/**
     * Get extension from filename. For example 'jpg' is returned from 
     * 'C:\Data\Test.jpg'. aFilename must contain drive letter, path and 
     * filename (file extension is not required).
     *
     * @param aFilename filename
     * @param aExt returned extension
     *
     * @return Does extension exist (if true, lenght of aExt is > 0)
     */
    inline TBool GetExt(const TDesC& aFilename, TPtrC& aExt)
     	{
     	// find extension (everything after last dot)
		TInt pos = aFilename.LocateReverseF( '.' );
		if( pos >= 0 )
			{
			aExt.Set( aFilename.Mid( pos + 1 ) );

			if( aExt.Length() > 0 )
				{
				return ETrue;				
				}
			}

		return EFalse;
		}

	/**
     * Get path from filename. For example 'C:\Data\' is returned from 
     * 'C:\Data\Test.jpg'. aFilename must contain drive letter, path and 
     * filename (file extension is not required).
     *
     * @param aFilename filename
     * @param aPath returned path
     *
     * @return Does path exist (if true, lenght of aPath is > 0)
     */
    inline TBool GetPath(const TDesC& aFilename, TPtrC& aPath)
     	{     	
     	// find path (everything before last back slash)
		TInt pos = aFilename.LocateReverseF( '\\' );
		if( pos >= 0 )
			{
			aPath.Set( aFilename.Left( pos + 1 ) );
			
			if( aPath.Length() > 0 )
				{
				return ETrue;
				}
			}
		
		return EFalse;
     	}    
    }

	/**
	 * Serialize an array to a newly created descriptor buffer.
	 * Leaves on error.
	 * @param aArray  Array to serialize.
	 * @return  A new output descriptor pointer. Ownership is transferred.
	 */
	template<typename T>
	HBufC8* SerializeArrayL( const RArray<T>& aArray )
	    {
	    const TInt KItemCount = aArray.Count();
	    if ( KItemCount <= 0 )
	        {
	        return NULL;
	        }
	    const TInt KItemSizeInBytes = sizeof( T );
	    const TInt KBufferLength = KItemSizeInBytes * KItemCount;
	
	    HBufC8* buf = HBufC8::NewL( KBufferLength );
	    void* ptr = NULL;
	    for ( TInt i = 0; i < KBufferLength; i += KItemSizeInBytes )
	        {
	        const T& item = aArray[ i / KItemSizeInBytes ];
	        ptr = (void*)( buf->Ptr() + i );
	        Mem::Copy( ptr, &item, KItemSizeInBytes );
	        }
	
	    buf->Des().SetLength( KBufferLength );
	    return buf; // ownership is transferred
	    }
	
	/**
	 * Deserialize an array from a descriptor buffer.
	 * Leaves on error.
	 * @param aDesc  Descriptor containing the serialized array.
	 * @param aArray  Target array.
	 */
	template<typename T>
	void DeserializeArrayL( const TDesC8& aDesc, RArray<T>& aArray )
	    {
	    aArray.Reset();
	    const TInt KItemSizeInBytes = sizeof( T );
	    const TInt KBufferLength = aDesc.Size();
	    const TInt KItemCount = KBufferLength / KItemSizeInBytes;
	    aArray.Reserve( KItemCount );
	
	    for ( TInt i = 0; i < KItemCount; ++i )
	        {
	        T item;
	        void* ptr = (void*)( aDesc.Ptr() + i * KItemSizeInBytes );
	        Mem::Copy( &item, ptr, KItemSizeInBytes );
	        aArray.Append( item );
	        }
	    }
	
	
	
	/**
	 * Serialize a pointer array to a newly created descriptor buffer.
	 * Leaves on error.
	 * @param aArray  Pointer Array to serialize.
	 * @return  A new output descriptor pointer. Ownership is transferred.
	 */
	template<typename T>
	HBufC8* SerializePointerArrayL( const RPointerArray<T>& aArray )
	    {
	    const TInt KItemCount = aArray.Count();
	    if ( KItemCount <= 0 )
	        {
	        return NULL;
	        }
	    const TInt KItemSizeInBytes = sizeof( T );
	    const TInt KBufferLength = KItemSizeInBytes * KItemCount;
	
	    HBufC8* buf = HBufC8::NewL( KBufferLength );
	    void* ptr = NULL;
	    for ( TInt i = 0; i < KBufferLength; i += KItemSizeInBytes )
	        {
	        const T* item = aArray[ i / KItemSizeInBytes ];  // alkup. oli const T&
	        ptr = (void*)( buf->Ptr() + i );
	        Mem::Copy( ptr, &item, KItemSizeInBytes );
	        }
	
	    buf->Des().SetLength( KBufferLength );
	    return buf; // ownership is transferred
	    }
	
	/**
	 * Deserialize a pointer array from a descriptor buffer.
	 * Leaves on error.
	 * @param aDesc  Descriptor containing the serialized array.
	 * @param aArray  Target Pointer Array.
	 */
	template<typename T>
	void DeserializePointerArrayL( const TDesC8& aDesc, RPointerArray<T>& aArray )
	    {
	    aArray.Reset();
	    const TInt KItemSizeInBytes = sizeof( T );
	    const TInt KBufferLength = aDesc.Size();
	    const TInt KItemCount = KBufferLength / KItemSizeInBytes;
	    aArray.Reserve( KItemCount );
	
	    for ( TInt i = 0; i < KItemCount; ++i )
	        {
	        T* item;	// alkup. oli pelkkä T ilman pointteria
	        void* ptr = (void*)( aDesc.Ptr() + i * KItemSizeInBytes );
	        Mem::Copy( &item, ptr, KItemSizeInBytes );
	        aArray.Append( item );
	        }
	    }

#endif // MDSUTILS_H
