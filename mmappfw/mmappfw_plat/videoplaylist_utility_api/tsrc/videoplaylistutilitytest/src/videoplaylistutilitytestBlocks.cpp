/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

// Version : %version: 2 %

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "videoplaylistutilitytest.h"
#include "videoplaylistutilitytester.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cvideoplaylistutility_test::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// -----------------------------------------------------------------------------
//
void Cvideoplaylistutility_test::Delete()
{
    if ( iPlaylistUtilityTester )
    {
        delete iPlaylistUtilityTester;
        iPlaylistUtilityTester = NULL;
    }
}

// -----------------------------------------------------------------------------
// Cvideoplaylistutility_test::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cvideoplaylistutility_test::RunMethodL( CStifItemParser& aItem )
{
    static TStifFunctionInfo const KFunctions[] =
    {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "CreatePlaylistUtility", Cvideoplaylistutility_test::CreatePlaylistUtilityL ),
        ENTRY( "GetFileInfo", Cvideoplaylistutility_test::GetFileInfoL ),
        ENTRY( "GetLink", Cvideoplaylistutility_test::GetLinkL ),
        ENTRY( "GetPlaylist", Cvideoplaylistutility_test::GetPlaylistL ),
        ENTRY( "IsSeekable", Cvideoplaylistutility_test::IsSeekableL ),

    };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
}

// -----------------------------------------------------------------------------
//   Cvideoplaylistutility_test::CreateEngineL
// -----------------------------------------------------------------------------
//
TInt Cvideoplaylistutility_test::CreatePlaylistUtilityL( CStifItemParser& aItem )
{
    iPlaylistUtilityTester = CVideoPlaylistUtility_Tester::NewL();

    return iPlaylistUtilityTester->CreatePlaylistUtilityL( aItem );
}

// -----------------------------------------------------------------------------
//   Cvideoplaylistutility_test::CreateGenericParametersL
// -----------------------------------------------------------------------------
//
TInt Cvideoplaylistutility_test::GetFileInfoL(CStifItemParser& aItem)
{
    return iPlaylistUtilityTester->GetFileInfoL(aItem);
}

// -----------------------------------------------------------------------------
//   Cvideoplaylistutility_test::OpenFileL
// -----------------------------------------------------------------------------
//
TInt Cvideoplaylistutility_test::GetLinkL( CStifItemParser& aItem )
{
    return iPlaylistUtilityTester->GetLinkL( aItem );
}

// -----------------------------------------------------------------------------
//   Cvideoplaylistutility_test::SetDefaultAccessPointL
// -----------------------------------------------------------------------------
//
TInt Cvideoplaylistutility_test::GetPlaylistL( CStifItemParser& aItem )
{
    return iPlaylistUtilityTester->GetPlaylistL( aItem );
}

// -----------------------------------------------------------------------------
//   Cvideoplaylistutility_test::SetDefaultAccessPointL
// -----------------------------------------------------------------------------
//
TInt Cvideoplaylistutility_test::IsSeekableL( CStifItemParser& aItem )
{
    return iPlaylistUtilityTester->IsSeekableL( aItem );
}

//  [End of File] - Do not remove
