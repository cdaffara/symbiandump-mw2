/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CbsStreamHelper class.
*
*/



#ifndef CBSSTREAMHELPER_H
#define CBSSTREAMHELPER_H

// INCLUDES
#include <s32file.h>

// CLASS DECLARATION

/**
*   Contains common methods used by the database.
*
*
*/
class CbsStreamHelper
    {
    public:
        /**
        *   Reads boolean value from the stream.
        *
        *   @param aStream  The stream from which the value is read.
        *   @return         Read value.
        */
        static TBool ReadBoolL( RReadStream& aStream );

        /**
        *   Writes boolean value to the stream.
        *
        *   @param aStream  The stream to which the value is written.
        *   @param aBool    The value to be written.
        */
        static void WriteBoolL( RWriteStream& aStream, TBool aBool );
    };

#endif // CBSSTREAMHELPER
