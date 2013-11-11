/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CTestPlaybackPlugin implementation
*
*/

//
//  INCLUDE FILES
//
#include <apgcli.h>
#include <e32cmn.h>
#include <mpxmediageneraldefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxplaybackcommanddefs.h>
#include <mpxplaybackpluginobserver.h>
#include "testplaybackplugin.h"

//
//  CONSTANTS
//
const TUid KLocalPlaybackUid = { 0x10282556 };


// ============================ MEMBER FUNCTIONS ===============================

//  ----------------------------------------------------------------------------
//    Two-phased constructor.
//  ----------------------------------------------------------------------------
//
CTestPlaybackPlugin* CTestPlaybackPlugin::NewL( TAny* /*aInitParams*/ )
{
    CTestPlaybackPlugin* p = new (ELeave) CTestPlaybackPlugin();
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
}

//  ----------------------------------------------------------------------------
//    Symbian 2nd phase constructor can leave.
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::ConstructL()
{
    User::LeaveIfError( iFs.Connect() );
    iFs.ShareProtected();
}

//  ----------------------------------------------------------------------------
//    C++ constructor
//  ----------------------------------------------------------------------------
//
CTestPlaybackPlugin::CTestPlaybackPlugin()
{
}

//  ----------------------------------------------------------------------------
//    Destructor
//  ----------------------------------------------------------------------------
//
CTestPlaybackPlugin::~CTestPlaybackPlugin()
{
    iFile.Close();
    iFs.Close();
}

//  ----------------------------------------------------------------------------
//    Set observer
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::SetObserver( MMPXPlaybackPluginObserver& aObs )
{
	iObs = &aObs;
}

//  ----------------------------------------------------------------------------
//    Initializes a clip for playback from a file name
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::InitialiseL( const TDesC& aSong )
{
	delete iClipName;
    iClipName = NULL;
    iClipName = aSong.AllocL();

    iFile.Close();

    TInt err = iFile.Open( iFs, aSong, EFileRead | EFileShareReadersOrWriters );

    //
    //  Remap KErrNotReady to KErrNotFound, because it is referencing a drive
    //  that is not existent
    //
    if ( KErrNotReady == err )
    {
        err = KErrNotFound;
    }

    // if aSong is an streaming link and contains one of the streaming schemas
    // eg. rtsp:// , http:// etc. then a file handle can not be opened
    // ignore KErrBadName
    if (err != KErrBadName)
    {
        User::LeaveIfError( err );    
    }    
}

//  ----------------------------------------------------------------------------
//    Initializes a clip for playback from a file handle
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::InitialiseL( RFile& aSong )
{
	delete iClipName;
    iClipName = NULL;
    iClipName = HBufC::NewL( KMaxFileName );
    TPtr ptr = iClipName->Des();
    aSong.FullName( ptr );

    iFile.Close();
    User::LeaveIfError( iFile.Duplicate( aSong ));
}

/**
* Initializes a file for playback.
*
* @since S60 9.2
* @param aUri URI of the item
* @param aType the mime type of the item
* @param aAccessPoint the access point
*/
void CTestPlaybackPlugin::InitStreamingL(const TDesC& /*aUri*/, 
        const TDesC8& /*aType*/, TInt /*aAccessPoint*/)
    {
    }

/**
* Initializes a file handle for playback.
*
* @since S60 9.2
* @param aFile file handle of a file
* @param aAccessPoint the access point
*/
void CTestPlaybackPlugin::InitStreamingL(RFile& /*aFile*/, TInt /*aAccessPoint*/)
    {
    }

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
/**
* Initializes a file handle for playback.
*
* @since S60 9.2
* @param aFile 64 bit file handle of a file
* @param aAccessPoint the access point
*/
void CTestPlaybackPlugin::InitStreaming64L(RFile64& /*aFile*/, TInt /*aAccessPoint*/)
    {
    }

/**
* Initializes a song for playback.
*
* @since S60 9.2
* @param aFile 64 bit file handle of a song
*/
void CTestPlaybackPlugin::Initialise64L(RFile64& /*aSong*/)
    {
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

//  ----------------------------------------------------------------------------
//    Executes a command on the selected song
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::CommandL( CMPXCommand& /*aCmd*/ )
{
}


//  ----------------------------------------------------------------------------
//    Executes a command on the selected song
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::CommandL(TMPXPlaybackCommand /*aCmd*/, TInt /*aData*/)
{
}


//  ----------------------------------------------------------------------------
//    Sets a property of the plugin
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::SetL( TMPXPlaybackProperty /*aProperty*/, TInt /*aValue*/ )
{
}

//  ----------------------------------------------------------------------------
//    Gets a property of the plugin (async)
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::PropertyL( TMPXPlaybackProperty /*aProperty*/ ) const
{
}

//  ----------------------------------------------------------------------------
//    Gets a list of sub players, UPnP only
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::SubPlayerNamesL()
{
    iObs->HandleSubPlayerNames( KLocalPlaybackUid, NULL, ETrue, KErrNone );
}

//  ----------------------------------------------------------------------------
//    Select a sub player
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::SelectSubPlayerL( TInt /*aIndex*/ )
{
    User::Leave( KErrNotSupported );
}

//  ----------------------------------------------------------------------------
//    Returns current sub player name
//  ----------------------------------------------------------------------------
//
const TDesC& CTestPlaybackPlugin::SubPlayerName()
{
    return KNullDesC;
}

//  ----------------------------------------------------------------------------
//    Current sub player index
//  ----------------------------------------------------------------------------
//
TInt CTestPlaybackPlugin::SubPlayerIndex() const
{
   return KErrNotFound;
}

//  ----------------------------------------------------------------------------
//    Gets media properties
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::MediaL( const TArray<TMPXAttribute>& /*aAttrs*/ )
{
}

//  ----------------------------------------------------------------------------
//    Cancel request
//  ----------------------------------------------------------------------------
//
void CTestPlaybackPlugin::CancelRequest()
{
}

//  ----------------------------------------------------------------------------
//  CTestPlaybackPlugin::GetFileHandle()
//  ----------------------------------------------------------------------------
//
RFile CTestPlaybackPlugin::GetFileHandle()
{
    return iFile;
}

// End of file
