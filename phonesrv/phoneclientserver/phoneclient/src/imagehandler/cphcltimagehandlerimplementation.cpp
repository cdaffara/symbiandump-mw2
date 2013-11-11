/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the PhoneClient Messenger interface.
*
*/



// INCLUDE FILES
#include    "cphcltimagehandlerimplementation.h" 

#ifdef _DEBUG
#define _DxPRINT( x ) RDebug::Print(_L( x ) );
#define _DxPRINT2( x,y ) RDebug::Print(_L( x ), y );
#else
#define _DxPRINT(x)
#define _DxPRINT2( x,y ) 
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::NewL
// 
// Creates Phone Client image handler instance.
// -----------------------------------------------------------------------------
//
CPhCltImageHandlerImplementation* 
    CPhCltImageHandlerImplementation::NewL()
    {
    CPhCltImageHandlerImplementation* self = 
        new (ELeave) CPhCltImageHandlerImplementation();   
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::~CPhCltImageHandlerImplementation
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhCltImageHandlerImplementation::~CPhCltImageHandlerImplementation()
    {
    iStillHandler.Close();
    iServer.Close();
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::SaveImages
// 
// Saves a number of images to PhoneServer.
// -----------------------------------------------------------------------------
//
TInt CPhCltImageHandlerImplementation::
    SaveImages( CPhCltImageParams& aParams )
    {
    return iStillHandler.SaveImages( aParams );
    }
  
// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::LoadImages
// 
// Loads a number of images from PhoneServer.
// -----------------------------------------------------------------------------
//  
TInt CPhCltImageHandlerImplementation::
    LoadImages( CPhCltImageParams*& aParams )
    {
    return iStillHandler.LoadImages( aParams );
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CPhCltImageHandlerImplementation
// 
// C++ constructor.
// -----------------------------------------------------------------------------
//
CPhCltImageHandlerImplementation::CPhCltImageHandlerImplementation()
    {
    }
    
    
 // -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhCltImageHandlerImplementation::ConstructL()
    {
    User::LeaveIfError( iServer.Connect() );    
    User::LeaveIfError ( iStillHandler.Open( iServer ) ); 
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CPhCltBaseImageParamsL
// 
// Creates Phone Client image handler parameter class instance.
// -----------------------------------------------------------------------------
//
CPhCltImageParams* CPhCltImageHandlerImplementation::
    CPhCltBaseImageParamsL( const TPhCltImageType aType )
    {
    CPhCltBaseImageParams* param = NULL;
    switch ( aType )
        {
        case EPhCltTypeVTStill:        
        case EPhCltTypeVTDefault: // <- this returns NULL
            param = CPhCltExtVTImageParams::NewL( aType );
            break;
        case EPhCltTypeOperatorLogo:
            param = CPhCltExtOperatorLogoParams::NewL();
            break;
        default:
            User::Leave( KErrArgument );
            break;
        }
    return param;
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::OpenDefaultVtImage
// 
// Opens Video telephony default image.
// -----------------------------------------------------------------------------
//
TInt CPhCltImageHandlerImplementation::OpenDefaultVtImage( RFile& aFile )
    {
    _DxPRINT("CPhCltImageHandlerImplementation.OpenDefaultVtImage")
    return OpenVtImageCommon( aFile, ETrue );
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::SaveVtImageL
// -----------------------------------------------------------------------------
void CPhCltImageHandlerImplementation::SaveVtImageL( 
    MPhCltImageObserver& aObserver,
    const TDesC& aImagePath )
    {
    // new style saving not supported yet
    User::Leave( KErrNotSupported );    
    iVtImageSaver =
        CPhCltImageHandlerImplementation::CVtImageSaver::SaveLD( 
            *this, iStillHandler, aImagePath );
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CancelSaveVtImage
// -----------------------------------------------------------------------------
void CPhCltImageHandlerImplementation::CancelSaveVtImage()
    {
    if ( iVtImageSaver )
        {
        iVtImageSaver->Cancel();
        iVtImageSaver = NULL;
        }
    }
// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::OpenVtImageCommon
// -----------------------------------------------------------------------------
TInt CPhCltImageHandlerImplementation::OpenVtImageCommon( 
    RFile& aFile, 
    const TBool aDefaultImage )
    {
    _DxPRINT2("CPhCltImageHandlerImplementation.OpenDefaultVtImage default=%d",
        aDefaultImage)
    TInt fileServerHandle( 0 );    
    TInt fileHandle( 0 );
    // ensure file handle is not open
    aFile.Close();
    fileServerHandle = iStillHandler.OpenVtImage( fileHandle, aDefaultImage );
    _DxPRINT2("CPhCltImageHandlerImplementation.OpenDefaultVtImage fileServerHandle=%d",
        fileServerHandle )
    TInt result( fileServerHandle );
    if ( fileServerHandle > 0 )
        {
        result = aFile.AdoptFromServer( fileServerHandle, fileHandle );
        }
   _DxPRINT2("CPhCltImageHandlerImplementation.OpenDefaultVtImage result=%d",
        result )
    return result;
    }
        
// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::OpenVtImage
// -----------------------------------------------------------------------------
TInt CPhCltImageHandlerImplementation::OpenVtImage( RFile& aFile )
    {   
    return OpenVtImageCommon( aFile, EFalse );    
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::HandleImageSaveCompleteL
// -----------------------------------------------------------------------------
void CPhCltImageHandlerImplementation::HandleImageSaveCompleteL( TInt aResult )
    {
    if ( iObserver )
        {
        iVtImageSaver = NULL; // the saver deletes itself
        MPhCltImageObserver* obs = iObserver;
        iObserver = NULL;
        obs->HandleImageSaveCompleteL( aResult );        
        }
    }
    
// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CVtImageSaver::SaveLD
// -----------------------------------------------------------------------------
CPhCltImageHandlerImplementation::CVtImageSaver* 
    CPhCltImageHandlerImplementation::CVtImageSaver::SaveLD(
    MPhCltImageObserver& aObserver,
    RPhCltImageHandler& aHandler,
    const TDesC& aImagePath )
    {
    CPhCltImageHandlerImplementation::CVtImageSaver* saver = 
        new ( ELeave ) CPhCltImageHandlerImplementation::CVtImageSaver(
        aObserver, aHandler );
    CleanupStack::PushL( saver );
    saver->DoSave( aImagePath );
    CleanupStack::Pop();
    return saver;
    }

                                      
// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CVtImageSaver::CVtImageSaver
// -----------------------------------------------------------------------------
CPhCltImageHandlerImplementation::CVtImageSaver::CVtImageSaver( 
    MPhCltImageObserver& aObserver,
    RPhCltImageHandler& aHandler ) : CActive( CActive::EPriorityStandard ),
    iObserver( aObserver), iHandler( aHandler )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CVtImageSaver::~CVtImageSaver
// -----------------------------------------------------------------------------
CPhCltImageHandlerImplementation::CVtImageSaver::~CVtImageSaver()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CVtImageSaver::DoSave
// -----------------------------------------------------------------------------
void CPhCltImageHandlerImplementation::CVtImageSaver::DoSave( 
    const TDesC& aImagePath )
    {
    iHandler.SaveVtImage( iStatus, aImagePath );
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CVtImageSaver::RunL
// -----------------------------------------------------------------------------
void CPhCltImageHandlerImplementation::CVtImageSaver::RunL()
    {
    MPhCltImageObserver& observer = iObserver;
    delete this;
    TRAP_IGNORE( observer.HandleImageSaveCompleteL( iStatus.Int() ) );    
    }

// -----------------------------------------------------------------------------
// CPhCltImageHandlerImplementation::CVtImageSaver::DoCancel
// -----------------------------------------------------------------------------
void CPhCltImageHandlerImplementation::CVtImageSaver::DoCancel()
    {
    iHandler.CancelSaveVtImage();
    }

//  End of File
