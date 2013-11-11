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
* Description:  PosLandmarkSerialization class definition
*
*/


#ifndef __EPOS_POSLANDMARKSERIALIZATION_H
#define __EPOS_POSLANDMARKSERIALIZATION_H

#include <e32base.h>

class CPosLandmark;

/**
 *  This class contains helper methods for landmark serialization.
 *  A landmark can be packed into a buffer with PackL method and unpacked
 *  back with UnpackL method.
 *
 *  @lib eposlandmarks.lib
 *  @since S60 3.1
 */
class PosLandmarkSerialization
    {
    public:
        /** Packs landmark object into a buffer.
         *
         *  All the data is preserved when packing. Database information is not stored.
         *  Packed landmark can be unpacked with @ref UnpackL method.
         *
         *  @param aLandmark The landmark to be packed.
         *  @return The buffer with the landmark data packed.
         *          Ownership is transferred to the client.
         */
        IMPORT_C static HBufC8* PackL( const CPosLandmark& aLandmark );

        /** Unpacks a landmark object, packed with @ref PackL method, from buffer.
         *
         *  All the data is preserved when packing, and restored exactly
         *  when unpacking. It means that landmark ID and
         *  category IDs are valid only for the database, this landmark was read from.
         *
         *  @param aBuffer The buffer containing landmark.
         *  @return The landmark unpacked. Ownership is transferred to client.
         *  @leave KErrCorrupt if buffer does not contain valid landmark package.
         */
        IMPORT_C static CPosLandmark* UnpackL( const TDesC8& aBuffer );
    };

#endif // __EPOS_POSLANDMARKSERIALIZATION_H
