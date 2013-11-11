/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provides functionality for searching DRM content link.
*
*/




#ifndef SENDUIUTILS_H
#define SENDUIUTILS_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>
#include <apmstd.h>

// CLASS DECLARATION

/**
*  Provides functionality for searching DRM content link.
*
*  @lib SenduiDataUtils.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS ( SendUiUtils )
    {
    public: // New functions

    
       /**
        * Searches for drm link content in a file
        *
        * @since S60 v3.0
        * @param aFilePath File to check
        * @param aFs File server session
        * @return ETrue if Content URL exists,
        *         EFalse otherwise
        */
        static RFile OpenFileHandleL( const TDesC& aFilePath, RFs& aFs );

       /**
        * Searches for drm link content in a file handle
        *
        * @since S60 v3.0
        * @param aFile File to check
        * @return ETrue if Content URL exists,
        *         EFalse otherwise
        */
        static TBool HasContentUrlL( RFile& aFile );
        
        /**
        *
        */
        static void ResolveMimeTypeL( const RFile& aFile, TDataType& aMimeType );

    };

#endif      // SENDUIUTILS_H   
            
// End of File
