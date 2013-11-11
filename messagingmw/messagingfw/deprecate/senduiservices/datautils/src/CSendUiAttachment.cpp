/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Encapsulates file path and handle attachments into single class.
*
*/



#include <f32file.h> 
#include <e32base.h> 
#include <apmstd.h>
#include <badesca.h>
#include <eikenv.h>
#include <msgtextutils.h>
#include <txtrich.h>
#include <MuiuMsvUiServiceUtilities.h> 
#include <mmf/common/mmfcontrollerpluginresolver.h>
#include <sysutil.h>
#include <MsgMimeTypes.h>

#ifdef RD_MULTIPLE_DRIVE
#include <driveinfo.h>
#endif
#include "CSendUiAttachment.h"

// Temporary is file saved to RAM drive
_LIT( KSenduiTempFilePath, ":\\system\\temp\\sendui\\");
_LIT( KSenduiTempFilePathFormat, "%x_%x" );
_LIT16( KExtTextPlain_16, ".txt" );
const TInt KSendUiMaxTemporaryFileNameLength = 15;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSendUiAttachment::CSendUiAttachment() :CBase()
	{
	}

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CSendUiAttachment::~CSendUiAttachment()
	{
	if ( Type() == EAttachmentPath )
		{
		RFile* file = MUTABLE_CAST( RFile*, iHandle ); // must close temporary file before deleting it
		file->Close();
		delete file;
		if ( iFlags & EAttachmentTemporary && iFileManager )
		    {
            // Remove temp file and directory
            iFileManager->Delete( *iPath );
            iFileManager->RmDir( *iPath );  // In case of rubbish, empty the folder
 		    }
        if ( iFileManager ) 
            {
            delete iFileManager;
            }
		delete iPath;
		}
	}	
// -----------------------------------------------------------------------------
// NewLC, Path
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiAttachment*	CSendUiAttachment::NewLC( const TDesC16* aPath, RFs& aRFs )
    {
    CSendUiAttachment* self = new(ELeave)CSendUiAttachment( );
    CleanupStack::PushL( self );
    self->ConstructL( aPath, aRFs );
    return self;
    }

// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//
void CSendUiAttachment::ConstructL( const TDesC16* aPath, RFs& aRFs )
    {
    RFile* myfile = new(ELeave)RFile;
    CleanupStack::PushL( myfile );
    TInt err = myfile->Open( aRFs, *aPath, EFileShareReadersOnly );
    
    if ( err )
        {
        User::LeaveIfError( myfile->Open( aRFs, *aPath, EFileShareAny ) );
        }
    
    iPath = aPath->AllocL();
    myfile->Size( iSize );
    iHandle = myfile;
    CleanupStack::Pop( myfile );    
    }
// -----------------------------------------------------------------------------
// NewLC, file handle
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiAttachment*	CSendUiAttachment::NewLC( const RFile* aHandle )
    {
    CSendUiAttachment* self = new(ELeave)CSendUiAttachment( );
    CleanupStack::PushL( self );
    self->iHandle = aHandle;
    aHandle->Size( self->iSize );
    return self;
    }

// -----------------------------------------------------------------------------
// NewLC, BodyText
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiAttachment*	CSendUiAttachment::NewLC( const CRichText& aBodyText, RFs& aRFs )
    {
    CSendUiAttachment* self = new(ELeave)CSendUiAttachment( );
    CleanupStack::PushL( self );
    self->ConstructL( aBodyText, aRFs );
    return self;
    }

// -----------------------------------------------------------------------------
// ConstructL, BodyText
// -----------------------------------------------------------------------------
//
void CSendUiAttachment::ConstructL( const CRichText& aBodyText, RFs& aRFs )
    {
    // character count * 2 = message size
    TInt err = KErrNone;
    TInt drive;
#ifdef RD_MULTIPLE_DRIVE
    User::LeaveIfError( DriveInfo::GetDefaultDrive(
        DriveInfo::EDefaultRam, drive ) );
#else
    drive = EDriveD;
#endif    
    if ( SysUtil::DiskSpaceBelowCriticalLevelL( &aRFs, aBodyText.DocumentLength() * 2, drive ) )
        {
        User::Leave( KErrDiskFull );
        }
    
    HBufC* buf = HBufC::NewLC( KMaxFileName );
    TPtr fileNameBuf = buf->Des();
    TChar driveLetter = TChar('A');
    driveLetter += drive;
    fileNameBuf.Append( driveLetter );  
    iFileManager = CFileMan::NewL( aRFs );
    
    TFileName fileName;
    TPtrC textSnippet;
    TCharFormat charFormat;
    aBodyText.GetChars( textSnippet, charFormat, 0 );
    
    CMsgTextUtils::GetFileNameFromBuffer(
        fileName,
        textSnippet,
        KSendUiMaxTemporaryFileNameLength,
        &KExtTextPlain_16 );

    CreateTempPathL( fileNameBuf, fileName, aRFs );
    
    aBodyText.ExportAsTextL( fileNameBuf, CPlainText::EOrganiseByParagraph, 0 );
    
    iPath = fileNameBuf.AllocL();
    CleanupStack::PopAndDestroy( buf );
    SetFlags( EAttachmentTemporary );    
    
    // Open handle
    RFile* myfile = new(ELeave)RFile;
    CleanupStack::PushL( myfile );
    err = myfile->Open( aRFs, *iPath, EFileShareReadersOnly );
    
    if ( err )
        {
        User::LeaveIfError( myfile->Open( aRFs, *iPath, EFileShareAny ) );
        }

    myfile->Size( iSize );
    iHandle = myfile;
    CleanupStack::Pop( myfile ); 
    SetMimeType( TDataType(  KMsgMimeTextPlain ));
    }

// -----------------------------------------------------------------------------
// Fills the attachment array
// -----------------------------------------------------------------------------
//
EXPORT_C CArrayPtrFlat<CSendUiAttachment>* CSendUiAttachment::InitAttachmentArrayLCC(
    const CDesC16Array& aPaths,
    const RArray<RFile>& aHandles,
    RFs& aFSs )
	{
    TInt i = 0;
    if ( &aPaths ) 
        {
        i += aPaths.Count();
        }
	if ( &aHandles )
	    {
	    i += aHandles.Count();
	    }
    
    CArrayPtrFlat<CSendUiAttachment>* attachments = new(ELeave)CArrayPtrFlat<CSendUiAttachment>( i ? i : 1);
	CleanupStack::PushL( attachments );
	CleanupResetAndDestroyPushL( *attachments );
	CSendUiAttachment* attachment;
	
    if ( &aPaths )
    	{
	    for ( i = 0; i < aPaths.Count(); i++ )
	        {
	        TPtrC16 tmp(aPaths.MdcaPoint( i ) );
	        attachment = CSendUiAttachment::NewLC( &tmp, aFSs );
	        attachments->AppendL( attachment );
            CleanupStack::Pop( attachment );
	        }
    	}
    if ( &aHandles != NULL )
    	{
	    for ( i = 0; i < aHandles.Count(); i++ )
	        {
	        attachment = CSendUiAttachment::NewLC( &(aHandles[i] ));
	        attachments->AppendL( attachment );
            CleanupStack::Pop( attachment );
	        }
    	}
    
    return attachments;
	}
	
// -----------------------------------------------------------------------------
// CreateTempPathL
//
// Creates temp path and unique file name under d:\system\temp\sendui
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendUiAttachment::CreateTempPathL( 
    TDes16& aCompletePath, 
    TFileName& aTempFileName,
    RFs& aRFs )
    {
    aCompletePath.Append( KSenduiTempFilePath );
    
    if ( aTempFileName.Length() == 0 )
        {
        aCompletePath.Append( KSenduiTempFilePathFormat );

        TTime time;
        time.UniversalTime();

        TFileName* tempDir = new(ELeave) TFileName();
        CleanupStack::PushL( tempDir );
        tempDir->Format(
            aCompletePath ,
            I64HIGH( time.Int64() ),
            I64LOW( time.Int64() ) );
        aCompletePath = *tempDir;
        CleanupStack::PopAndDestroy( tempDir );
        }
    else
        {
        aCompletePath.Append( aTempFileName );
        }

    TInt err = aRFs.MkDirAll( aCompletePath );
	// path may exists
	if ( err && err != KErrAlreadyExists )
		{
		User::LeaveIfError( err );
		}
    }  

// -----------------------------------------------------------------------------
// Type
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSendUiAttachment::Type()
	{
	if ( iPath )
		{
		return EAttachmentPath;
		}
	else 
		{
		return  EAttachmentHandle;
		}
	}

// end of file
