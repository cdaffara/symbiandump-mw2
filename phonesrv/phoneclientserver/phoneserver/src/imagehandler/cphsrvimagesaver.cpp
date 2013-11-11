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
* Description:  Image saver implementation
*
*/


#include "cphsrvimagesaver.h"
#include "mphsrvimagesaverobserver.h"
#include <imageconversion.h>
#include <f32file.h>
#include <bitmaptransforms.h>

// primary file name for VT image
_LIT( KPhSrvPrimaryVtImageFileName, "vtstillimage1" );

// file name used if primary file is in use while save occurs
_LIT( KPhSrvAlternateVtImageFileName, "vtstillimage2" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// 1st constructor
// ---------------------------------------------------------------------------
//
CPhSrvImageSaver* CPhSrvImageSaver::NewL( 
        RFs& aFs,
        const TFileName& aPrivatePath, 
        MPhSrvImageSaverObserver& aObserver )
    {
    CPhSrvImageSaver* saver = new ( ELeave ) CPhSrvImageSaver( 
        aFs, aPrivatePath, aObserver );
    CleanupStack::PushL( saver );
    saver->ConstructL();
    CleanupStack::Pop();
    return saver;
    }

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CPhSrvImageSaver::~CPhSrvImageSaver()
    {
    delete iImageDecoder;
    delete iFileManager;
    delete iScaler;
    }

// ---------------------------------------------------------------------------
// Saves image
// 1) decode from file
// 2a) scale (for single frame images) and save to phone server private dir, or
// 2b) multiframe image - copy file as is to phone server private directory
// ---------------------------------------------------------------------------
//
void CPhSrvImageSaver::SaveImageL( const TDesC& aFileName )
    {
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }
    iImageDecoder = CImageDecoder::FileNewL( iFs, aFileName );
    const TInt frameCount = iImageDecoder->FrameCount();
    if ( frameCount > 1 )
        {
        // decoder not needed because image is copied as is
        delete iImageDecoder;
        iImageDecoder = NULL;
        CopyImageL( aFileName );
        }
    else if ( frameCount == 1 )
        {
        ScaleImageL();
        }
    else
        {
        // invalid image?
        User::Leave( KErrCorrupt );
        }
    }

// ---------------------------------------------------------------------------
// Copies the file
// ---------------------------------------------------------------------------
//
void CPhSrvImageSaver::CopyImageL( const TDesC& aFileName )
    {
    TName targetFileName;
    ConstructVtFileSaveName( aFileName, targetFileName );
    iFileManager = CFileMan::NewL( iFs );
    iFileManager->Copy( aFileName, targetFileName, 
        CFileMan::EOverWrite, iStatus );    
    }

// ---------------------------------------------------------------------------
// ScaleImageL
// ---------------------------------------------------------------------------
//
void CPhSrvImageSaver::ScaleImageL()
    {
    }

// ---------------------------------------------------------------------------
// Checks if primary file is in use.
// ---------------------------------------------------------------------------
//
TInt CPhSrvImageSaver::IsPrimaryFileInUse( TBool& aIsInUse )
    {
    CDir* entries = NULL;
    TInt res = iFs.GetDir( iPath, KEntryAttNormal, EDirsLast, entries );
    if ( res == KErrNone && entries )
        {
        const TInt entryCount( entries->Count() );
        for ( TInt index = 0; index < entryCount; index++ )
            {
            const TEntry& entry = ( *entries )[index];            
            if ( entry.iName.Compare( KPhSrvPrimaryVtImageFileName() ) >= 0 )
                {
                // primary file name matches with directory entry (extension
                // depends on file format)
                res = iFs.IsFileOpen( entry.iName, aIsInUse );
                index = entryCount; // break loop
                }
            }
        }
    delete entries;
    return res;
    }
    
// ---------------------------------------------------------------------------
// Constructs target file name for VT still image. Alternate name is used if
// file with primary name is open.
// ---------------------------------------------------------------------------
//
TInt CPhSrvImageSaver::ConstructVtFileSaveName(
    const TDesC& aSourceFileName, 
    TName& aTargetFileName )
    {
    TBool inUse( EFalse );
    TInt res( IsPrimaryFileInUse( inUse ) );
    if ( res == KErrNone )
        {
        TParse sourceName;
        sourceName.Set( aSourceFileName, NULL, NULL );
        aTargetFileName = iPath;        
        if ( inUse )
            {
            // primary file in use (open), use alternalte name
            aTargetFileName.Append( KPhSrvAlternateVtImageFileName() );
            }
        else
            {
            aTargetFileName.Append( KPhSrvPrimaryVtImageFileName() );
            }
        aTargetFileName.Append( sourceName.Ext() );
        }
    return res;
    }
    
// ---------------------------------------------------------------------------
// CPhSrvImageSaver::RunL
// ---------------------------------------------------------------------------
//
void CPhSrvImageSaver::RunL()
    {
    iObserver.ImageSaved( iStatus.Int() );
    }
    
// ---------------------------------------------------------------------------
// CPhSrvImageSaver::DoCancel
// ---------------------------------------------------------------------------
//
void CPhSrvImageSaver::DoCancel()
    {
    iObserver.ImageSaved( KErrCancel );
    }
    
// ---------------------------------------------------------------------------
// CPhSrvImageSaver::ConstructL
// ---------------------------------------------------------------------------
//
void CPhSrvImageSaver::ConstructL()
    {
    User::Leave( KErrNotSupported );
    }
    
// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPhSrvImageSaver::CPhSrvImageSaver( RFs& aFs, const TFileName& aPrivatePath,
    MPhSrvImageSaverObserver& aObserver) 
    : CActive( CActive::EPriorityStandard ), 
    iFs( aFs ), 
    iPath( aPrivatePath ),
    iObserver( aObserver )
    {
    }
