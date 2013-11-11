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
* Description:  Sub Session image handler.
*                Does image saving and loading.
*
*/



// INCLUDE FILES
#include    "cphsrvsubsessionimagehandler.h" 
#include    "cphsrvsession.h" 
#include    "cphsrvserver.h" 
#include    <e32property.h>
#include    <phclttypes.h> 
#include    "phsrvdebuginfo.h" 

// CONSTANTS
_LIT( KPhSrvImageDriveRom, "z:" );
_LIT( KPhSrvVTImageName, "vtstillimage" );
_LIT( KPhSrvVTDefaultImageName, "vtdefaultimage." );
_LIT( KPhSrvImageSuffix, ".mbm" );
_LIT( KPhSrvImageWildCard, "*" );
_LIT( KPhSrvLogoMncMccSeparator, "_" );
_LIT( KPhSrvLogoTypeIndicatorOTA, "OTA" );
_LIT( KPhSrvLogoTypeIndicatorProg, "Prog" );
_LIT( KPhSrvOperatorLogoPrefix, "Logo" );
const TInt KPhSrvNameMaxLen = 1024;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::CPhSrvSubSessionImageHandler
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionImageHandler::CPhSrvSubSessionImageHandler( 
    CPhSrvSession& aSession )
:   CPhSrvSubSessionBase( aSession, EPhSrvSubSessionTypeImageHandler )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Ctor" )
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::~CPhSrvSubSessionImageHandler
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionImageHandler::~CPhSrvSubSessionImageHandler()
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Dtor" )
    iVTBitMaps.ResetAndDestroy();
    iOperatorLogos.ResetAndDestroy();
    PhoneSession().PhoneServer().DisconnectFBS();
    iFs.Close();
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Dtor>" )    
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase* CPhSrvSubSessionImageHandler::NewL(
    CPhSrvSession& aSession )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::NewL" )    
    CPhSrvSubSessionBase* self = 
        new( ELeave ) CPhSrvSubSessionImageHandler( aSession );
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::NewL>" )        
    return self;
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::ImageSaved
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::ImageSaved( TInt aResult )
    {
    iImageSaver = NULL;
    // Message for save request
    iSaveVtImageMessage.Complete( aResult );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionImageHandler::PhSrvMessageDecoderCanProcessMessage( 
    TInt aFunction )
    {
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::CanProcess", aFunction )    
    switch( aFunction )
        {
        case EPhoneServerImageHandlerSubSessionOpen:
        case EPhoneServerImageHandlerSubSessionClose:
        case EPhoneServerImageHandlerSubSessionSave:
        case EPhoneServerImageHandlerSubSessionLoad:
        case EPhoneServerImageHandlerSubSessionCommitLoad:
        case EPhoneServerOpenVtImage:
        // new saving scheme 
        case EPhoneServerSaveVtImage:
        case EPhoneServerSaveVtImageCancel:
            return ETrue;
            
        default:
            return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::PhSrvMessageProcessorHandleMessageL
// 
// Framework function. Handle any op-codes here.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::PhSrvMessageProcessorHandleMessageL( 
    const RMessage2& aMessage )
    {
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Handle", 
        aMessage.Function() )
    TBool permissionStatus ( EFalse );
    
    // Check needed capabilities    
    TBool hasCapWriteDeviceData( EFalse );
    hasCapWriteDeviceData = aMessage.HasCapability( ECapabilityWriteDeviceData );
    TBool hasCapReadDeviceData( EFalse );
    hasCapReadDeviceData = aMessage.HasCapability( ECapabilityReadDeviceData );

    switch( aMessage.Function() )
        {
        case EPhoneServerImageHandlerSubSessionClose:
            CmdSubSessionClose( aMessage );
            permissionStatus = ETrue;
            break;

        case EPhoneServerImageHandlerSubSessionSave:
            if ( hasCapWriteDeviceData )
                {        
                CmdSubSessionSaveImagesL( aMessage );
                permissionStatus = ETrue;
                }
            break;
            
        case EPhoneServerImageHandlerSubSessionCommitLoad:
            if ( hasCapReadDeviceData )
                {        
                CmdSubSessionCommitLoadL( aMessage );
                permissionStatus = ETrue;
                }
            break;
            

        case EPhoneServerImageHandlerSubSessionLoad:
            if ( hasCapReadDeviceData )
                {        
                CmdSubSessionLoadImagesL( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerOpenVtImage:
            if ( hasCapReadDeviceData )
                {        
                CmdSubSessionOpenDefaultVtImageL( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerSaveVtImage:
            if ( hasCapWriteDeviceData )
                {
                CmdSubSessionSaveVtImageL( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerSaveVtImageCancel:            
            if ( hasCapWriteDeviceData )
                {
                permissionStatus = ETrue;
                if ( iImageSaver )
                    {
                    // saving in progress, cancel it.
                    iImageSaver->Cancel();
                    }
                else
                    {
                    // nothing to cancel or too late cancellation 
                    iSaveVtImageMessage.Complete( KErrNotReady );
                    }
                }
            break;
        default:
            _DPRINT( 4, "CPhSrvSubSessionImageHandler::Handle: PANIC" )        
            PhoneSession().PanicClient(
                aMessage,
                EPhCltServerInitiatedPanicInvalidRequest );
            break;
        }
    
    // If caps are not ok
    if( !permissionStatus )
        {
        aMessage.Complete( KErrPermissionDenied );
        }   
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Handle>" )
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::CmdSubSessionClose
// 
// Close the sub-session.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::CmdSubSessionClose( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Close" )    
    PhoneSession().CloseSubSession( aMessage );
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Close>" )    
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::CmdSubSessionSaveImagesL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::CmdSubSessionSaveImagesL( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Save" )    
    TPhCltImageArray handles;
    TPckg< TPhCltImageArray > pHandles( handles );
    
    Read( aMessage, 0, pHandles );

    // Version check in here, if multiple possibilities.
    switch( pHandles().iType )
        {
        case EPhCltTypeVTStill:
            DoSaveVTImageL( handles );
            break;
        case EPhCltTypeOperatorLogo:
            DoSaveOperatorLogoL( handles );
            break;
        default:
            _DPRINT( 4, "CPhSrvSubSessionImageHandler::Save US" )        
            User::Leave( KErrNotSupported );
        }

    // Leave is propagated into framework.
    aMessage.Complete( KErrNone );
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Save>" )    
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::CmdSubSessionLoadImagesL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::CmdSubSessionLoadImagesL( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Load" )    
    TPhCltImageArray handles;
    TPckg< TPhCltImageArray > pHandles( handles );

    Read( aMessage, 0, pHandles );

    // Version check in here, if multiple possibilities.
    switch( pHandles().iType )
        {
        case EPhCltTypeVTStill:
        case EPhCltTypeVTDefault:
            DoLoadVTImagesL( handles );
            break;
        case EPhCltTypeOperatorLogo:
            DoLoadOperatorLogoL( handles );
            break;
        default:
            _DPRINT( 4, "CPhSrvSubSessionImageHandler::Load US" )        
            User::Leave( KErrNotSupported );
        }
    
    Write( aMessage, 0, pHandles );
    aMessage.Complete( KErrNone );
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Load>" )    
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::CmdSubSessionOpenDefaultVtImageL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::CmdSubSessionOpenDefaultVtImageL( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, 
        "CPhSrvSubSessionImageHandler::CmdSubSessionOpenDefaultVtImageL<" )
    TBool defaultImage( ETrue );
    TPckg< TBool > pckg( defaultImage );
    Read( aMessage, 1, pckg );

    // Currently only default still image uses file handle sharing. User
    // defined image uses legacy scheme. If user defined image is attempted
    // to open here, complete with error.
    if ( !defaultImage )
        {
        User::Leave( KErrNotSupported );
        }
    
    RFs fs;
    RFile defaultVtImageFile;    
    TFileName fileName;
                                                     
    // File extension may be any supported file type. Resolve which one is used.
    User::LeaveIfError( ResolveDefaultVtImageFileName( fileName ) );    
    
    // Open separate session because ownwership of transferred file's
    // session goes with the file (when file is closed by the client,
    // related session is also closed)
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
     
    User::LeaveIfError( defaultVtImageFile.Open( fs, fileName, EFileRead ) );
    
    // Transfer file handle to client by writing file handle value to
    // message argument 0
    User::LeaveIfError( 
        defaultVtImageFile.TransferToClient( aMessage, 0 ) );    
    defaultVtImageFile.Close(); // file handle duplicated for client
    CleanupStack::PopAndDestroy(); // fs, can be closed, handle duplicated
     
    _DPRINT( 4, 
        "CPhSrvSubSessionImageHandler::CmdSubSessionOpenDefaultVtImageL>" )
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::ResolveDefaultVtImageFileName
// 
// Resolves file name of default VT image. Extension cannot be known
// in advance and directory entries must be read and parsed.
// -----------------------------------------------------------------------------
//
TInt CPhSrvSubSessionImageHandler::ResolveDefaultVtImageFileName(
    TDes& aFileName )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::ResolveDefaultVtImageFileName<" )
    CDir* entries = NULL;    
    TFileName path = KPhSrvImageDriveRom();
    
    // read directory entries in the server private path
    path.Append( iFilePath );    
    TInt result = iFs.GetDir( path, KEntryAttNormal, EDirsLast, entries );
    if ( result == KErrNone && entries )
        {
        const TInt entryCount( entries->Count() );
        // go through entries and stop when VT image file is found
        for ( TInt index = 0; index < entryCount; index++ )
            {
            const TEntry& entry = ( *entries )[index];  
            if ( entry.iName.Find( KPhSrvVTDefaultImageName() ) > KErrNotFound )
                {
                aFileName = path;
                aFileName.Append( entry.iName );
                // fileName now contains full path
                index = entryCount; // break loop
                result = KErrNone;
                }
            }
        }
    delete entries;
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::ResolveDefaultVtImageFileName>" )
    return result;
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::CmdSubSessionSaveVtImageL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::CmdSubSessionSaveVtImageL( 
    const RMessage2& aMessage )
    {
    TFileName sourceFile;
    TPckg<TFileName> pckg( sourceFile );
    
    Read( aMessage, 0, pckg );
    CPhSrvImageSaver* saver = CPhSrvImageSaver::NewL( iFs, iFilePath, *this );
    CleanupStack::PushL( saver );
    saver->SaveImageL( sourceFile );
    CleanupStack::Pop();
    iImageSaver = saver;
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::CmdSubSessionCommitLoadL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::CmdSubSessionCommitLoadL( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Commit" )    
    TPhCltImageArray handles;
    TPckg< TPhCltImageArray > pHandles( handles );

    Read( aMessage, 0, pHandles );
    
    switch ( pHandles().iType )
        {
        case EPhCltTypeVTStill:
        case EPhCltTypeVTDefault:
            DoCommitLoadVTL();
            break;
        case EPhCltTypeOperatorLogo:
            DoCommitLoadLogo( handles );
            break;
        default:
            _DPRINT( 4, "CPhSrvSubSessionImageHandler::Commit US" )        
            User::Leave( KErrNotSupported );        
        }
    aMessage.Complete( KErrNone );
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Commit" )    
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DoCommitLoadVTL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::DoCommitLoadVTL()
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::CommitVT" )    
    iVTBitMaps.ResetAndDestroy(); 
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::CommitVT>" )        
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DoCommitLoadLogo
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::DoCommitLoadLogo( TPhCltImageArray& aImages )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::CommitLogo" )        
    for ( TInt i = 0; i < iOperatorLogos.Count(); i ++ )
        {
        if ( aImages.iImages[ EPhCltExtCountryCodeIndex ] == 
            iOperatorLogos[ i ]->iCountryCode && 
            aImages.iImages[ EPhCltExtNetworkCodeIndex ] == 
            iOperatorLogos[ i ]->iNetworkCode )
            {
            _DPRINT( 4, "CPhSrvSubSessionImageHandler::CommitLogo found" )                
            delete iOperatorLogos[ i ];
            iOperatorLogos.Remove( i );
            break;
            }
        }
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::CommitLogo>" )            
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DoSaveVTImage
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::DoSaveVTImageL( TPhCltImageArray& aImages )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::SaveVT" )        
    HBufC* hbuf = HBufC::NewLC(KPhSrvNameMaxLen);
    TPtr buf = hbuf->Des();
    for ( TInt i = 0; i < aImages.iImageCount; i++ )
        {
        buf.Zero();
        buf.Append( iDriveName );
        buf.Append( iFilePath );
        buf.Append( KPhSrvVTImageName );
        buf.AppendNum( i );
        buf.Append( KPhSrvImageSuffix );
        buf.ZeroTerminate();
#ifdef __PHENG_DEBUG_INFO__
        RDebug::Print( _L("CPhSrvSubSessionImageHandler::SaveVT: %S"), &buf );
#endif
        CFbsBitmap* bitMap = new ( ELeave ) CFbsBitmap();
        CleanupStack::PushL( bitMap );
        User::LeaveIfError( bitMap->Duplicate( aImages.iImages[ i ] ) );
        User::LeaveIfError( bitMap->Save( buf ) );
        CleanupStack::PopAndDestroy( bitMap );
        }
    CleanupStack::PopAndDestroy( hbuf );
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::SaveVT>" )        
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DoLoadVTImagesL
// 
// Loads images.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::DoLoadVTImagesL( TPhCltImageArray& aImages )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::LoadVT" )    
    // Load the images from file.
    const TBool loadDefaultImage = aImages.iType == EPhCltTypeVTDefault;    
    DoReadVTImagesL( loadDefaultImage );
    
    // Copy handles.
    aImages.iImageCount = iVTBitMaps.Count();
    for ( TInt i = 0; i < aImages.iImageCount; i++ )
        {
        aImages.iImages[ i ] = iVTBitMaps[ i ]->Handle();
        _DDPRINT( 4, 
            "CPhSrvSubSessionImageHandler::LoadVT: ", aImages.iImages[ i ] )
        }
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::LoadVT>" )        
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DoReadVTImagesL
// 
// Loads images.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::DoReadVTImagesL( const TBool aDefaultImage )
    {
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::ReadVT is default=", aDefaultImage )     
    iVTBitMaps.ResetAndDestroy();
    
    TPtrC path; // pointer to sub-string of searchPath containing drive+path
    
    // Search path = drive + path + file-name (with wildcard) + extension
    HBufC* searchPath = ConstructVTImageSearchPathLC( 
        aDefaultImage, path );

#ifdef __PHENG_DEBUG_INFO__
        TPtr ptr = searchPath->Des();  // temp pointer to avoid lvalue compile warning
        RDebug::Print( _L( 
            "CPhSrvSubSessionImageHandler::ReadVT: %S"), ( &ptr ));
#endif    
    RDir dir;
    User::LeaveIfError( dir.Open( iFs, *searchPath, KEntryAttNormal ) );    
    CleanupClosePushL( dir );
    
    TEntryArray entryArray;
    TInt readResult = dir.Read( entryArray );
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::ReadVT res: ", readResult )
    HBufC* hbitmapName = HBufC::NewLC( KMaxFileName );
    TPtr bitmapName = hbitmapName->Des();
    
    // If directory entry array read successfully...
    while( ( readResult == KErrNone ) || ( readResult == KErrEof ) )
        {
        const TInt arrayLength = entryArray.Count();
        _DDPRINT( 4, "CPhSrvSubSessionImageHandler::DoReadVTImagesL count: ", arrayLength )
        // ...go through directory entries
        for( TInt i = 0; i < arrayLength; i++ )
            {
            const TEntry& entry = entryArray[ i ];
            const TDesC& name = entry.iName;
            TBool load = EFalse;
            // If client wanted default image, load it only
            if ( aDefaultImage && 
                 name.FindF( KPhSrvVTDefaultImageName ) == 0 )
                {
                /* Next two statements for stopping to process 
                  directory entries */                       
                i = arrayLength;
                readResult = KErrEof;
                load = ETrue;
                }
            // If client wanted still image, load it (or them)
            else if ( !aDefaultImage &&
                      name.FindF( KPhSrvVTImageName ) == 0 )
                {
                load = ETrue;
                }
            if ( load )
                {
                CFbsBitmap* bitmap = new ( ELeave ) CFbsBitmap();
                CleanupStack::PushL( bitmap );
                bitmapName = path;  // initialize the file path
                bitmapName.Append( name );
#ifdef __PHENG_DEBUG_INFO__
                RDebug::Print( _L( 
                    "CPhSrvSubSessionImageHandler::ReadVT BM: %S"), 
                    &bitmapName );
#endif    
                User::LeaveIfError( bitmap->Load( bitmapName ) );
                User::LeaveIfError( iVTBitMaps.Append( bitmap ) );
                CleanupStack::Pop(); // bitmap
                }
            }
        
        if ( readResult == KErrEof )
            {
            _DPRINT( 4, "CPhSrvSubSessionImageHandler::ReadVT EOF" )            
            break;            
            }
        readResult = dir.Read( entryArray );
        }
    CleanupStack::PopAndDestroy( 3 ); // hbitmapName, dir, search path
    if( ( readResult != KErrNone ) && ( readResult != KErrEof ) )
        {
        _DDPRINT( 4, "CPhSrvSubSessionImageHandler::ReadVT Leave", readResult )                    
        User::Leave( readResult );
        }    
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::ReadVT>" )
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DoSaveOperatorLogoL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::DoSaveOperatorLogoL( TPhCltImageArray& aImages )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::SaveLogo" )
    // Check the values prior saving, 
    // to check whether deletion is actually ongoing
    const TPhCltCountryCode mcc = aImages.iImages[ EPhCltExtCountryCodeIndex ];
    const TPhCltNetworkCode mnc= aImages.iImages[ EPhCltExtNetworkCodeIndex ];
    const TInt handle = aImages.iImages[ EPhCltExtOperatorLogoIndex ];
    const TPhCltExtOperatorLogoType logoType = 
        ( TPhCltExtOperatorLogoType )aImages.iImages[ EPhCltExtLogoTypeIndex ];
        
    // Only OTA logos can be saved and deleted.
    if ( logoType == EPhCltLogoTypeProgrammable )
        {
        _DPRINT( 4, "CPhSrvSubSessionImageHandler::SaveLogo PROG" )
        User::Leave( KErrAccessDenied );
        }

    _DPRINT( 4, "PhSrvImageHandler::SaveLogo test delete " )        
    // Check if this a delete command.
    // If no logo deleted, then this is save command.
    if ( !DeleteLogoL( mcc, mnc, logoType, handle ) )
        {
        _DPRINT( 4, "CPhSrvSubSessionImageHandler::SaveLogo Save" )
        HBufC* hbuf = HBufC::NewLC(KPhSrvNameMaxLen);
        TPtr buf = hbuf->Des();
        ConstructLogoFileNameL( mcc, mnc, logoType, buf );
        CFbsBitmap* bitMap = new ( ELeave ) CFbsBitmap();
        CleanupStack::PushL( bitMap );
        User::LeaveIfError( bitMap->Duplicate( 
            aImages.iImages[ EPhCltExtOperatorLogoIndex ] ) );
        User::LeaveIfError( bitMap->Save( buf ) );
        CleanupStack::PopAndDestroy( 2, hbuf );   // hbuf, bitMap
        NotifyListeners( // Error ignored.
            mcc, mnc, TTelephonyOTALogoUpdate::EPSTelephonyLogoChanged );
        // new P&S key update         
        NotifyListeners( // Error ignored.
            mcc, mnc, TTelOTALogoUpdate::EPSTelephonyLogoChanged );
        }
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::SaveLogo>" )                
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DoLoadOperatorLogoL
// 
// Loads images.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::
    DoLoadOperatorLogoL( TPhCltImageArray& aImages )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::LoadLogo" )            
    // Construct the file path first.
    HBufC* hbuf = HBufC::NewLC(KPhSrvNameMaxLen);
    TPtr buf = hbuf->Des();
    TInt err( KErrNone );
    ConstructLogoFileNameL( aImages.iImages[ EPhCltExtCountryCodeIndex ], 
        aImages.iImages[ EPhCltExtNetworkCodeIndex ], 
        ( TPhCltExtOperatorLogoType ) aImages.iImages[ EPhCltExtLogoTypeIndex ],
        buf );
    
    // Load the image.
    CFbsBitmap* bitMap = new ( ELeave ) CFbsBitmap();
    CleanupStack::PushL( bitMap );
    err = bitMap->Load( buf );
    if ( err == KErrPathNotFound )
        {
        err = KErrNotFound;
        }
    User::LeaveIfError( err );    
    
    CPhSrvOperatorLogoContainer* logoContainer = 
        new (ELeave)CPhSrvOperatorLogoContainer( 
        aImages.iImages[ EPhCltExtCountryCodeIndex ], 
        aImages.iImages[ EPhCltExtNetworkCodeIndex ], 
        ( TPhCltExtOperatorLogoType ) aImages.iImages[ EPhCltExtLogoTypeIndex ],
        bitMap );
    aImages.iImages[ EPhCltExtOperatorLogoIndex ] = bitMap->Handle();
    aImages.iImageCount = 1;
    CleanupStack::PushL(logoContainer);
    iOperatorLogos.AppendL( logoContainer );
    CleanupStack::Pop(logoContainer);
    CleanupStack::Pop( bitMap );
    CleanupStack::PopAndDestroy( hbuf );
    
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::LoadLogo>" )            
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::MakeImageDirL
// 
// Creates directory structure.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::MakeImageDirL()
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::MakeDir" )    
    TDriveNumber sysDrive;
    User::LeaveIfError( BaflUtils::GetSystemDrive( sysDrive ) );
    iDriveName.Format( _L("%c:"), sysDrive +'A' );
    const TInt err( iFs.CreatePrivatePath( sysDrive ) );
    if (  err != KErrAlreadyExists && err != KErrNone )
        {
        User::Leave( err );
        }
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::MakeDir>" )            
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::ConstructVTImageSearchPathLC
// 
// Construct search path for VT image.
// -----------------------------------------------------------------------------
//
HBufC* CPhSrvSubSessionImageHandler::ConstructVTImageSearchPathLC( 
    const TBool aDefaultImage,
    TPtrC& aPath ) const
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::ConstructVTImageSearchPathLC<" )
    HBufC* hfilePath = HBufC::NewLC( KMaxFileName );
    TPtr filePath = hfilePath->Des();

    filePath = iFilePath; // = "\private\<phone server uid>\"

    if ( aDefaultImage )
        {
        // = "z:\private\<phone server uid>\"
        //    ==
        filePath.Insert( 0, KPhSrvImageDriveRom );      
        
        aPath.Set( filePath );
        
        // = "z:\private\<phone server uid>\vtdefaultimage"
        //                                  ==============
        filePath.Append( KPhSrvVTDefaultImageName() );  
        }
    else
        {
        // = "c:\private\<phone server uid>\"
        //    ==
        filePath.Insert( 0, iDriveName );
        
        aPath.Set( filePath );
        
        // = "c:\private\<phone server uid>\vtstillimage"         
        //                                  ============
        filePath.Append( KPhSrvVTImageName() );
        }        
    // = "c:\private\<phone server uid>\<still|default>*"
    //                                                 =
    filePath.Append( KPhSrvImageWildCard() );           
    
    // = "c:\private\<phone server uid>\<still|default>*.mbm"
    //                                                  ====
    filePath.Append( KPhSrvImageSuffix() );             
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::ConstructVTImageSearchPathLC>" )
    return hfilePath;
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::ConstructL
// 
// 2nd phase construction.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionImageHandler::ConstructL()
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::CtorL" )        
    PhoneSession().PhoneServer().ConnectFBS();
    User::LeaveIfError( iFs.Connect() );
    User::LeaveIfError( iFs.ShareProtected() );
    MakeImageDirL();
    User::LeaveIfError( iFs.PrivatePath( iFilePath ) );
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::CtorL>" )  
    }
 
// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::DeleteLogoL
// 
// Checks if logo(s) should be deleted, and deletes them if needed.
// -----------------------------------------------------------------------------
//    
TBool CPhSrvSubSessionImageHandler::DeleteLogoL( 
    const TPhCltCountryCode aCountryCode, 
    const TPhCltNetworkCode aNetworkCode,
    const TPhCltExtOperatorLogoType aLogoType,
    const TInt aFileHandle )
    {
    _DPRINT( 4, "PhSrvImageHandler::DeleteLogoL" )    
    TBool deleted = EFalse;
    
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::DelLogo ", aCountryCode )      
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::DelLogo ", aNetworkCode )      
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::DelLogo ", aLogoType )      
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::DelLogo ", aFileHandle )      
    
    // Check if all, one or none.
    if ( aFileHandle == KPhCltDeleteOperatorLogo )
        {
        HBufC* hbuf = HBufC::NewLC(KPhSrvNameMaxLen);
        TPtr buf = hbuf->Des();
        if ( aCountryCode == KPhCltDeleteOperatorLogo && 
            aNetworkCode == KPhCltDeleteOperatorLogo )
            {
            // Delete all logos.
            TFindFile* finder = new(ELeave) TFindFile(iFs);
            CleanupStack::PushL(finder);
            
            HBufC* hfilePath = HBufC::NewLC(KMaxFileName);
            TPtr filePath = hfilePath->Des();
            filePath = iDriveName;
            filePath.Append( iFilePath );

            buf.Append( KPhSrvOperatorLogoPrefix );
            buf.Append( KPhSrvLogoMncMccSeparator );
            buf.Append( KPhSrvImageWildCard );
#ifdef __PHENG_DEBUG_INFO__
    RDebug::Print( _L( 
        "CPhSrvSubSessionImageHandler::DelLogo: %S"), &buf );
#endif              
            CDir* foundFiles = NULL;
            const TInt err = finder->FindWildByDir( buf, filePath, foundFiles );
            if ( err != KErrNone )
                {
                _DDPRINT( 4, "CPhSrvSubSessionImageHandler::DelLogo Leave", err )
                delete foundFiles;
                foundFiles = NULL;
                User::Leave( err );
                }
            for ( TInt i = 0; i < foundFiles->Count(); i++ )
                {
                const TEntry& entry = foundFiles->operator[ ]( i );
#ifdef __PHENG_DEBUG_INFO__
    RDebug::Print( _L( 
        "CPhSrvSubSessionImageHandler::DelLogo: rem %S"), &entry.iName );
#endif                 
                BaflUtils::DeleteFile( iFs, entry.iName );
                }
            delete foundFiles;
            deleted = ETrue;
            NotifyListeners( // Error ignored.
                KPhCltDeleteOperatorLogo, KPhCltDeleteOperatorLogo, 
                TTelephonyOTALogoUpdate::EPSTelephonyAllLogosDeleted );
            // new P&S key update    
            NotifyListeners( // Error ignored.
                KPhCltDeleteOperatorLogo, KPhCltDeleteOperatorLogo, 
                TTelOTALogoUpdate::EPSTelephonyAllLogosDeleted );                
             
            CleanupStack::PopAndDestroy( 2, hfilePath );    // finder, hfilePath
            }
        else
            {
            // Delete specified logo.
            ConstructLogoFileNameL( 
                aCountryCode, aNetworkCode, aLogoType, buf );
            BaflUtils::DeleteFile( iFs, buf );
            deleted = ETrue;
            NotifyListeners( aCountryCode, aNetworkCode, 
                TTelephonyOTALogoUpdate::EPSTelephonyLogoDeleted );
            NotifyListeners( // Error ignored.
                KPhCltDeleteOperatorLogo, KPhCltDeleteOperatorLogo, 
                TTelOTALogoUpdate::EPSTelephonyAllLogosDeleted );                
            }
            
        CleanupStack::PopAndDestroy( hbuf );
        }
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::DelLogo>", deleted )              
    return deleted;
    }
   
// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::ConstructLogoFileNameL
// 
// Creates operator logo file name.
// -----------------------------------------------------------------------------
//     
void CPhSrvSubSessionImageHandler::ConstructLogoFileNameL(
    const TPhCltCountryCode aCountryCode, 
    const TPhCltNetworkCode aNetworkCode,
    const TPhCltExtOperatorLogoType aLogoType,
    TDes& aFileName )    
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::LogoName")    
    aFileName.Zero();
    if ( aLogoType == EPhCltLogoTypeOTA )
        {
        aFileName.Append( iDriveName );  
        }
    else
        {
        aFileName.Append( KPhSrvImageDriveRom );
        }
    aFileName.Append( iFilePath );
    aFileName.Append( KPhSrvOperatorLogoPrefix );
    aFileName.Append( KPhSrvLogoMncMccSeparator );        
    aFileName.AppendNum( aCountryCode );
    aFileName.Append( KPhSrvLogoMncMccSeparator );
    aFileName.AppendNum( aNetworkCode );
    aFileName.Append( KPhSrvLogoMncMccSeparator );
    AppendLogoTypeAsTextL( aLogoType, aFileName );
    aFileName.Append( KPhSrvImageSuffix );
#ifdef __PHENG_DEBUG_INFO__
    RDebug::Print( _L( 
        "CPhSrvSubSessionImageHandler::LogoName: %S"), &aFileName );
#endif    
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::AppendLogoTypeAsTextL
// 
// Appends logo type to buffer.
// -----------------------------------------------------------------------------
//     
void CPhSrvSubSessionImageHandler::
    AppendLogoTypeAsTextL( 
    const TPhCltExtOperatorLogoType aLogoType, TDes& aType )
    {
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::LogoType: ", aLogoType )        
    switch ( aLogoType )
        {
        case EPhCltLogoTypeOTA:
            aType.Append( KPhSrvLogoTypeIndicatorOTA );
            break;
        case EPhCltLogoTypeProgrammable:
            aType.Append( KPhSrvLogoTypeIndicatorProg );
            break;
        default:
            User::Leave( KErrNotSupported );
            break;
        }
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::LogoType>" )
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::NotifyListeners
// 
// Updates publish & subscribe information with event happened.
// -----------------------------------------------------------------------------
//     
TInt CPhSrvSubSessionImageHandler::
    NotifyListeners( const TPhCltCountryCode aCountryCode, 
    const TPhCltNetworkCode aNetworkCode,
    const TTelephonyOTALogoUpdate::TPSOperatorLogoUpdateEvent aEvent )  
    {
    TInt err( KErrNone );
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify ", aCountryCode ) 
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify ", aNetworkCode )
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify ", aEvent )
    // If not exist define key
    if ( !iPropertyDefined )
        {
        err = RProperty::Define(  KPSUidPhoneClientServerInformation, 
            KTelOperatorLogoUpdated, RProperty::EByteArray, ECapability_None, ECapabilityWriteDeviceData);

        _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify defined: ", err )
        if ( err == KErrAlreadyExists )
            {
            // No error.
            err = KErrNone;
            }
        }

    // Set event
    if ( err == KErrNone )
        {
        TTelephonyOTALogoUpdate update;
        TTelephonyOTALogoUpdate::TOTALogoUpdatePckg logoUpdate( update );
        logoUpdate().iEvent = aEvent;
        logoUpdate().iMCC = aCountryCode;
        logoUpdate().iMNC = aNetworkCode;
        iPropertyDefined = ETrue;        
        err = RProperty::Set( 
            KPSUidPhoneClientServerInformation, KTelOperatorLogoUpdated, logoUpdate );
        }
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify> ", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionImageHandler::NotifyListeners
// 
// Updates publish & subscribe information with event happened.
// New P&S key used.
// -----------------------------------------------------------------------------
//     
TInt CPhSrvSubSessionImageHandler::
    NotifyListeners( const TPhCltCountryCode aCountryCode, 
    const TPhCltNetworkCode aNetworkCode,
    const TTelOTALogoUpdate::TPSOperatorLogoUpdateEvent aEvent )  
    {
    TInt err( KErrNone );
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify ", aCountryCode ) 
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify ", aNetworkCode )
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify ", aEvent )
    // If not exist define key
    if ( !iPropertyDefined )
        {
        err = RProperty::Define(  KPSUidPhoneClientServerInformation, 
            KTelOperatorLogoUpdated, RProperty::EByteArray, ECapability_None, ECapabilityWriteDeviceData);

        _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify defined: ", err )
        if ( err == KErrAlreadyExists )
            {
            // No error.
            err = KErrNone;
            }
        }

    // Set event
    if ( err == KErrNone )
        {
        TTelOTALogoUpdate update;
        TTelOTALogoUpdate::TOTALogoUpdatePckg logoUpdate( update );
        logoUpdate().iEvent = aEvent;
        logoUpdate().iMCC = aCountryCode;
        logoUpdate().iMNC = aNetworkCode;
        iPropertyDefined = ETrue;      
        err = RProperty::Set( 
            KPSUidPhoneClientServerInformation, KTelOperatorLogoUpdated, logoUpdate );  
        }
    _DDPRINT( 4, "CPhSrvSubSessionImageHandler::Notify> ", err )
    return err;
    }
    
// -----------------------------------------------------------------------------
// CPhSrvOperatorLogoContainer::CPhSrvOperatorLogoContainer
// 
// Constructor.
// -----------------------------------------------------------------------------
//   
CPhSrvSubSessionImageHandler::
    CPhSrvOperatorLogoContainer::CPhSrvOperatorLogoContainer(
    const TPhCltCountryCode aCountryCode, 
    const TPhCltNetworkCode aNetworkCode,
    const TPhCltExtOperatorLogoType aLogoType, 
    CFbsBitmap* aBitmap ) :
    iBitmap( aBitmap ), iCountryCode( aCountryCode ), 
    iNetworkCode( aNetworkCode ), iLogoType( aLogoType )
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Container::Ctor" )    
    }
    
// -----------------------------------------------------------------------------
// CPhSrvOperatorLogoContainer::~CPhSrvOperatorLogoContainer
// 
// Destructor.
// -----------------------------------------------------------------------------
//   
CPhSrvSubSessionImageHandler::
    CPhSrvOperatorLogoContainer::~CPhSrvOperatorLogoContainer( ) 
    {
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Container::Dtor" )        
    delete iBitmap;
    iBitmap = NULL;
    _DPRINT( 4, "CPhSrvSubSessionImageHandler::Container::Dtor>" )        
    }    
    
// End of File
