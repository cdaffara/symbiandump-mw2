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
* Description:   Utility class for SendUI.
*
*/




#ifndef CSENDUIDATAUTILS_H
#define CSENDUIDATAUTILS_H

//  INCLUDES
#include    <e32base.h>
#include    <f32file.h>
#include    <apmstd.h>
#include    <msgmimetypes.h>

class CSendingServiceInfo;
// CONSTANTS
_LIT8( KSenduiAudioPattern,    "*audio/*" );
_LIT8( KSenduiVideoPattern,    "*video/*" );

// FORWARD DECLARATION
class RApaLsSession;

// CLASS DECLARATION

/**
*  Utility class for SendUI.
*
*  @lib SenduiDataUtils.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS (CSendUiDataUtils) : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSendUiDataUtils* NewL( RFs& aFs );

        /**
        * Destructor.
        */
        virtual ~CSendUiDataUtils();

   public: // New functions

        /**
        * Resolves mime types for 
        * @since Series ?XX ?SeriesXX_version
        * @param aTotalSize Return value contains total size of confirmed files
        * @return None.
        */
        IMPORT_C void ResolveFileMimeTypeL(
            const TDesC& aFilePath,
            TDataType& aMimeType );

        /**
        * ?member_description.
        * @since Series ?XX ?SeriesXX_version
        * @param aTotalSize Return value contains total size of confirmed files
        * @return None.
        */
        IMPORT_C void ResolveFileMimeTypeL(
            const RFile& aFile,
            TDataType& aMimeType );

    private:

     /**
        * C++ default constructor.
        */
        CSendUiDataUtils( RFs& aFs );
    
    private:

        // Reference to an open fileserver session
        RFs& iFs;
        
        RApaLsSession* iApaLsSession;

    };

#endif      // CSENDUIDATAUTILS_H   
            
// End of File
