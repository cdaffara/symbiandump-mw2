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
* Description:  Encapsulates the supported types for the collection
*
*/


#ifndef CMPXCOLLECTIONTYPE_H
#define CMPXCOLLECTIONTYPE_H

#include <s32strm.h>
#include <badesca.h>
// CONSTANTS

/**
 *  Class to encapsulate supported collection type.
 *
 *  @lib mpxcommon.lib
 */
class CMPXCollectionType : public CBase
    {

public:
    /**
    * C++ constructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C CMPXCollectionType();
    
    /**
    * C++ constructor.
    * 
    * @since S60 3.2.3
    * @param aUid uid of the collection plugin
    * @param aMimeTypes mime types supported by plugin, take over the ownership
    * @param aExtensions extensions supported by plugin, take over the ownership
    */
    IMPORT_C CMPXCollectionType(const TUid& aUid,
 		                        CDesCArray* aMimeTypes, 
   		                        CDesCArray* aExtensions);
    
    /**
    * virtual destructor.
    *
    * @since S60 3.2.3
    */
    virtual ~CMPXCollectionType();

    /**
    * Externalize a object of this class to stream.
    *
    * @since S60 3.2.3
    * @param aStream write stream
    */
    IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
    
    /**
    * Internalize a object of this class from stream.
    *
    * @since S60 3.2.3
    * @param aStream read stream
    */
    IMPORT_C void InternalizeL(RReadStream& aStream);
    
    /**
    * Returns the extension.
    *
    * @since S60 3.2.3
    */
    inline const CDesCArray& Extensions() const;
    
    /**
    * Returns the UID.
    *
    * @since S60 3.2.3
    */
    inline const TUid& Uid() const;
    
    /**
    * Returns the mime type.
    *
    * @since S60 3.2.3
    */
    inline const CDesCArray& Mimetypes() const;
    
private: // data
    TUid iUid;
    CDesCArray* iMimeTypes;
    CDesCArray* iExtensions;
    };

#include "mpxcollectiontype.inl"

#endif // CMPXCOLLECTIONTYPE_H
