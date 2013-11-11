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
* Description:  videoplaylistutility test class for STIF testing
*
*/

// Version : %version:  2 %

#ifndef VIDEOPLAYLISTUTILITY_TESTER_H_
#define VIDEOPLAYLISTUTILITY_TESTER_H_

#include <e32base.h>
#include <e32def.h>
#include <StifItemParser.h>
#include <mpxmessage2.h>


enum TStifLinkTypes
{
    ERamFileStreaming,
    ERamFileLocal,
    EAsxFileStreaming,
    EAsxFileLocal
};

enum TStifFileTypes
{
    ERamFile,
    EAsxFile
};

enum TAsxEmbeddedTypes
{
    ENoEmbeddedInfo,
    EEmbeddedSeekInfo
};

// FORWARD DEC
class CVideoPlaylistUtility;


class CVideoPlaylistUtility_Tester : public CBase
{
    public: // constructors and destructors
        static CVideoPlaylistUtility_Tester* NewL();

        virtual ~CVideoPlaylistUtility_Tester();

        TInt CreatePlaylistUtilityL( CStifItemParser& aItem );
        TInt GetFileInfoL( CStifItemParser& aItem );
        TInt GetLinkL( CStifItemParser& aItem );
        TInt GetPlaylistL( CStifItemParser& aItem );
        TBool IsSeekableL( CStifItemParser& aItem );

    private:

        TFileName GetFileNameAndPathL( CStifItemParser& aItem );
        
        void ConstructL();

        CVideoPlaylistUtility_Tester();

    private:

        CVideoPlaylistUtility*      iPlaylistUtility;
};

#endif /* VIDEOPLAYLISTUTILITY_TESTER_H_ */
