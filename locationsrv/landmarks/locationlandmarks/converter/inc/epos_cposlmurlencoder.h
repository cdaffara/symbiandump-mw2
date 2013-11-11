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
* Description: Class used for encoding landmark content into an URL.
*
*/


#ifndef CPOSLMURLENCODER_H
#define CPOSLMURLENCODER_H

#include <EPos_CPosLandmarkEncoder.h>
#include "EPos_LmConverter.h"

class CPosLmOutput;

/**
*  Class used for encoding landmark content into an URL.
*
*  Output is written either to a buffer or to a file.
*
*  The basic protocol for encoding is to 1) define where to write the
*  output to by calling @ref SetUseOutputBufferL or @ref SetOutputFileL, 
*  2) add landmark data to encode by using functions in @ref CPosLandmarkEncoder
*  and/or @ref CPosLandmarkDatabase::ExportLandmarksL and 3) finalize the
*  encoding by calling @ref FinalizeEncodingL. If this protocol is not followed
*  the client is panicked with code @p EPosLmProtocolBreak. Encoding can be
*  performed multiple times using the same encoder object.
*
*/
class CPosLmUrlEncoder : public CPosLandmarkEncoder
    {
    public:  

        /**
        * Two-phased constructor.
        *
        * @returns A new instance of this class.
        */
        static CPosLmUrlEncoder* NewL();
        /**
        * Destructor.
        */
        ~CPosLmUrlEncoder();
       
    private:
        /**
        *  Default Constructor
        */
        CPosLmUrlEncoder();
        /**
        * Prohibit copy constructor
        */ 
        CPosLmUrlEncoder( const CPosLmUrlEncoder& );
        /**
        * Prohibit assigment operator
        */ 
        CPosLmUrlEncoder& operator= ( const CPosLmUrlEncoder& );

    public: 
        /**
        * From CPosLandmarkEncoder
        */ 
        CBufBase* SetUseOutputBufferL();
        /**
        * From CPosLandmarkEncoder
        */ 
        void SetOutputFileL( const TDesC&  aOutputFile );
        /**
        * From CPosLandmarkEncoder
        */ 
        void AddCollectionDataL(
            TPosLmCollectionDataId  aDataId,
            const TDesC&  aCollectionData );
        /**
        * From CPosLandmarkEncoder
        * leaves with KErrNotSupported if it is called more than once during
        * the encoding process.
        */ 
        void AddLandmarkL( const CPosLandmark&  aLandmark );
        /**
        * From CPosLandmarkEncoder
        */ 
        void AddCategoryForLatestLandmarkL( const CPosLandmarkCategory&  aCategory );
        /**
        * From CPosLandmarkEncoder
        */ 
        CPosLmOperation* FinalizeEncodingL();

    

    private:

        
        /** 
        * Starts encoding. Writes URL scheme & host string. 
        */
        void InitializeEncodingL();
        /**
         * Resets the encoding by deleting the old pointers.
         */
        void ResetEncoderL();
        /**
         * Returns the URL scheme component
         */
        void WriteUrlSchemeL();
        /**
         * Returns the URL host component
         */
        void WriteUrlHostL();
        /**
         * Encodes the landmark 
         * @leave KErrArgument IF the landmark that is passed to 
         * AddLandmarkL method does not contain latitude & longitude values
         * (i.e their values are NaN )
         */
        void EncodeLandmarkL();
        /**
         * Write the landmark coordinate information i.e latitude,longitude
         * altitude.
         * @param in aPosition contains the coordinate information
         * @leave KErrArgument If the latitude & longitude are not set(i.e NaN) or
         *        any Symbian error codes
         */
        void WriteCoordinatesL( const TLocality& aPosition);
        /**
         * Writes the URL query Parameter,value pair
         */
        void WriteQueryParamValueL( const TDesC16& aParam,const TDesC16& aValue );
        /**
         * Encodes the text value to conform to the URL encoding format
         * @param In aText the text to be encoded.
         * @param In aMaxEncodingLength The length of the buffer used for encoding
         * @return Returns the encoded buffer
         * @leave With any of the symbian error codes.
         */
        HBufC* EncodeTextL( const TDesC16& aText, const TInt aMaxEncodingLength );
        /**
         * Enocdes the category that is added for the landmark and writes to the 
         * encoded output.
         */
        void EncodeCategoryL();
        /**
         * Encodes the time according to the following time format 
         * YYYYMMDD:HHMMSS.MMMMMM
         * @param in aTimeStamp The timestamp of the landmark to be encoded
         * @return Returns the encoded time in the form of a buffer.
         */
        HBufC* EncodeTimeL( const TTime& aTimeStamp );
               
        
           
   

    private:
        /**
         * Enumeration to indicate the encoding state
         */
        enum TPosEncoderStatus
                    {
                    EPosUninitialized = 0,
                    EPosInitialized,
                    EPosLmInfoAdded
                    };
        /**
         * Encoding status used in verifying if the encoding
         * protocol is being followed.
         */
        TPosEncoderStatus           iEncoderStatus;

        /**
         * A Pointer to the landmark to be encoded.
         */
        CPosLandmark* iLastLandmark;
        /**
         * List of categories associated with the landmark to be encoded.
         */
        RPointerArray<const CPosLandmarkCategory> iCategories;
        /**
         * A pointer to the encoded output.
         */
        CPosLmOutput*  iOutput;

      
    };

#endif      // CPOSLMURLENCODER_H
