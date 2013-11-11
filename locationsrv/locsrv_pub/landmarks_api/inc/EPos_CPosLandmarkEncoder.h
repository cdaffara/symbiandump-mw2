/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLandmarkEncoder class
*
*/


#ifndef CPOSLANDMARKENCODER_H
#define CPOSLANDMARKENCODER_H

#include <e32base.h>
#include "EPos_Landmarks.h"
#include "EPos_CPosLandmark.h"
#include "EPos_CPosLandmarkCategory.h"
#include "EPos_CPosLmOperation.h"

class CPosLandmarkEncoderExtension;

/**
*  Class used for encoding landmark content.
*
*  When creating an instance of this class, the type (e.g. the mime type) of
*  the landmark content must be specified. The client will then receive an
*  encoder implementation which understands the requested landmark content.
*
*  Output is written either to a buffer or to a file.
*
*  The basic protocol for encoding is to 
*  -# define where to write the output to by calling @ref SetUseOutputBufferL or 
*  @ref SetOutputFileL, 
*  -# optionally add collection data using @ref AddCollectionDataL,
*  -# add landmark data to encode by using functions in @ref CPosLandmarkEncoder
*  and/or @ref CPosLandmarkDatabase::ExportLandmarksL 
*  -# finalize the encoding by calling @ref FinalizeEncodingL. 
*
*  If this protocol is not followed
*  the client is panicked with error code @p EPosLmProtocolBreak. Encoding can
*  be performed multiple times using the same encoder object.
*
*  If @ref CPosLandmarkEncoder is used, the client must call the global
*  function @ref ReleaseLandmarkResources before terminating, in order to
*  release all used landmark resources, otherwise the client may receive
*  an ALLOC panic.
*
*  @lib eposlandmarks.lib
*  @since S60 3.0
*/
class CPosLandmarkEncoder : public CBase
    {
    public:

        /**
        * Two-phased constructor.
        *
        * The client must specify the type (e.g. the MIME type) of the content
        * format which should be used for encoding.
        *
        * @param[in] The MIME type of the encode format.
        * @return A new instance of this class.
        *
        * @leave KErrNotSupported Content format is not supported.
        */
        IMPORT_C static CPosLandmarkEncoder* NewL( const TDesC8& aContentMimeType );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLandmarkEncoder();

    public:

        /**
        * Encode to a buffer.
        *
        * This function returns a dynamic buffer which will be filled with
        * encoded landmark content. The client takes ownership of the buffer.
        *
        * The client must not delete the buffer until encoding is finalized.
        *
        * @return The output buffer.
        */
        virtual CBufBase* SetUseOutputBufferL() = 0;

        /**
        * Encode to a file.
        *
        * The client specifies an output file for the encoder. The encoder
        * will then write all encoded data to this file.
        *
        * The file will be opened in exclusive mode which means that the file
        * cannot be accessed until the file is closed. The file is closed when
        * @ref FinalizeEncodingL has been executed. The file is also closed
        * if a new encoding is initialized by a call to
        * @ref SetUseOutputBufferL or @ref SetOutputFileL. After this, further
        * encoding to the old file is not possible.
        *
        * @param[in] aOutputFile The file name to write the encoded data to.
        *
        * @leave KErrAlreadyExists Specified file already exists.
        */
        virtual void SetOutputFileL( const TDesC& aOutputFile ) = 0;

        /**
        * Add landmark collection data to be encoded.
        *
        * @pre Output buffer or file is specified.
        *
        * Landmark collection data is generic information about the landmark
        * collection. It can for instance contain a name for the landmark
        * collection. Predefined collection attributes are defined by
        * @ref TPosLmCollectionDataId enumeration but also format specific
        * collection meta data can be defined.
        *
        * Collection data must be added before any landmarks are added.
        * Each collection ID can only be specified once.
        *
        * If the collection data is not a part of the chosen landmark encoding
        * format, it will be silently ignored.
        *
        * @param[in] aDataId Identifies which collection data to add.
        * @param[in] aCollectionData The collection data which should be added.
        *
        * @leave KErrAlreadyExists Collection data ID is specified twice.
        *
        * @panic "Landmarks Client"-EPosLmProtocolBreak 
        *   -# Output buffer or file not specified. 
        *   -# Collection data is added after some landmarks are added. 
        * @panic "Landmarks Client"-EPosLmInvalidArgument @p EPosLmCollDataNone is 
        *   specified as collection data ID.
        */
        virtual void AddCollectionDataL(
            TPosLmCollectionDataId aDataId,
            const TDesC& aCollectionData
        ) = 0;

        /**
        * Add a landmark to be encoded.
        *
        * @pre Output buffer or file is specified.
        *
        * If the landmark contains any categories, those categories may be
        * added by calling @ref AddCategoryForLatestLandmarkL.
        *
        * The client can either call this function directly or pass this encoder
        * object to @ref CPosLandmarkDatabase::ExportLandmarksL.
        *
        * @param[in] aLandmark The landmark to add.
        *
        * @panic "Landmarks Client"-EPosLmProtocolBreak Output buffer or file not specified. 
        */
        virtual void AddLandmarkL( const CPosLandmark& aLandmark ) = 0;

        /**
        * Add a landmark category for the most recently added landmark.
        *
        * @pre Output buffer or file is specified.
        *
        * The landmark category is added to the landmark which was most
        * recently added using @ref AddLandmarkL.
        *
        * The client can either call this function directly or pass this
        * encoder object to @ref CPosLandmarkDatabase::ExportLandmarksL.
        *
        * @param[in] aCategory The landmark category to add.
        *
        * @panic "Landmarks Client"-EPosLmProtocolBreak 
        *   -# Output buffer or file not specified. 
        *   -# No landmarks have been added yet.
        */
        virtual void AddCategoryForLatestLandmarkL(
            const CPosLandmarkCategory& aCategory
        ) = 0;

        /**
        * Finalize the encode process.
        *
        * Writes any buffered data to the output buffer/file. If an output
        * buffer is used it is compressed so that unused memory is freed. If an
        * output file is used, it is closed.
        *
        * After finalizing, further encoding to the specified output is not
        * possible. To start a new encoding, @ref SetUseOutputBufferL or
        * @ref SetOutputFileL must be called.
        *
        * The function returns an operation object which can be run in
        * incremental mode. If it is run incrementally the client can supervise
        * the progress of the operation.
        *
        * The client takes ownership of the returned operation object.
        *
        * If the @ref CPosLmOperation object is deleted before the operation
        * is complete, finalize is cancelled. Further encoding will not be
        * possible.
        *
        * @return A handle to the operation.
        */
        virtual CPosLmOperation* FinalizeEncodingL() = 0;

    protected:

        // C++ constructor.
        IMPORT_C CPosLandmarkEncoder();

    private:

        // Prohibit copy constructor
        CPosLandmarkEncoder( const CPosLandmarkEncoder& );
        // Prohibit assigment operator
        CPosLandmarkEncoder& operator= ( const CPosLandmarkEncoder& );

    private:
        CPosLandmarkEncoderExtension* iExtension;

        // Implementation Uid
        TUid iDtorIdKey;
    };

#endif      // CPOSLANDMARKENCODER_H


