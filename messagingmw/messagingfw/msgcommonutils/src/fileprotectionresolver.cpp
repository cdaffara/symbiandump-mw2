/*
* Copyright (c) 2005, 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
*       Resolves the protection status for a file.
*       Protection checks include:
*       - CommonContenPolicy
*       - OMA DRM
*       - Java SuperD
*
*/




// INCLUDE FILES
#include <e32std.h>
#include <e32base.h> // CBase
#include <apmstd.h>  // TDataType
#include <f32file.h> // RFs, RFile, CFileMan
#include <CommonContentPolicy.h>

#include <Oma2Agent.h>  // EFileType
#include <caf/caf.h>

#include <bldvariant.hrh> // Feature IDs

#include "fileprotectionresolver.h"
#include "javaprotectionresolver.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

_LIT8( KFileProtMimeJar, "application/java-archive");
//_LIT8( KFileProtEpocApp, "x-epoc/x-app*" );
_LIT8( KFileProtWrongSis, "x-epoc/x-app268436505" );
// CCommonContentPolicy uses 16-bit MIME types
_LIT16( KFileProtMimeSis, "application/vnd.symbian.install" );

const TInt KMaxContentUriLength = 256;

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// Factory
// -----------------------------------------------------------------------------
//
EXPORT_C CFileProtectionResolver* CFileProtectionResolver::NewLC( RFs& aFs )
    {
    CFileProtectionResolver* self = new ( ELeave ) CFileProtectionResolver( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
         
// -----------------------------------------------------------------------------
// Factory
// -----------------------------------------------------------------------------
//
EXPORT_C CFileProtectionResolver* CFileProtectionResolver::NewL( RFs& aFs )
    {
    CFileProtectionResolver* self = NewLC( aFs );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CFileProtectionResolver::CFileProtectionResolver( RFs& aFs ) :
    iFs( aFs )
    {
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CFileProtectionResolver::~CFileProtectionResolver()
    {
    delete iClosedContent;
    }

// -----------------------------------------------------------------------------
// 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CFileProtectionResolver::ConstructL()
    {
    iClosedContent = CCommonContentPolicy::NewL();
    }

// -----------------------------------------------------------------------------
// CFileProtectionResolver::CheckDRMStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CFileProtectionResolver::ProtectionStatusL( const TDesC& aFilePath, 
                                                          TDataType& aMimeType )
    {
    RFile file;
    User::LeaveIfError( file.Open( iFs, aFilePath, EFileRead | EFileShareReadersOnly ) );
    CleanupClosePushL( file );
    TInt ret = ProtectionStatusL( file, aMimeType );
    CleanupStack::PopAndDestroy( &file );
    return ret;
    }

// -----------------------------------------------------------------------------
// CFileProtectionResolver::CheckDRMStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CFileProtectionResolver::ProtectionStatusL( RFile& aFile, 
                                                          TDataType& aMimeType )
    {
    TInt ret( EFileProtNoProtection );
    HBufC8* contentUri = NULL;
    TRAPD( err,
        {
        ret = ProtectionStatusL( aFile, aMimeType, contentUri );
        } );
    delete contentUri;
    User::LeaveIfError( err );
    return ret;
    }

// -----------------------------------------------------------------------------
// CFileProtectionResolver::CheckDRMStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CFileProtectionResolver::ProtectionStatusL( const TDesC& aFilePath, 
                                                          TDataType& aMimeType, 
                                                          HBufC8*& aContentUri )
    {
    RFile file;
    User::LeaveIfError( file.Open( iFs, aFilePath, EFileRead | EFileShareReadersOnly ) );
    CleanupClosePushL( file );
    TInt ret = ProtectionStatusL( file, aMimeType, aContentUri );
    CleanupStack::PopAndDestroy( &file );
    return ret;
    }


// -----------------------------------------------------------------------------
// CFileProtectionResolver::CheckDRMStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CFileProtectionResolver::ProtectionStatusL( RFile& aFile, 
                                                          TDataType& aMimeType, 
                                                          HBufC8*& aContentUri )
    {
    TInt protection( EFileProtNoProtection );
    TBuf<KMaxDataTypeLength> temp;
    temp.Zero();
    //if ( aMimeType.Des8().MatchF( KFileProtEpocApp ) != KErrNotFound )
    if ( aMimeType.Des8().CompareF( KFileProtWrongSis ) == 0 )
        {
        // Symbian code has not been corrected. 
        // For some sis files it works, for some it returns "x-epoc/x-app268436505"
        // So we must redefine all "x-epoc/x-app268436505" as "application/vnd.symbian.install"
        temp.Copy( KFileProtMimeSis );
        }
    else
        {
        temp.Copy( aMimeType.Des8() );
        }

    TFileName name;
    User::LeaveIfError( aFile.Name( name ) );
    TParsePtrC parse( name );

    if ( iClosedContent->IsClosedType( temp ) ||
         iClosedContent->IsClosedExtension( parse.Ext() ) )
        {
        protection |= EFileProtClosedContent;
        }        
    
    delete aContentUri; // just in case
    aContentUri = NULL;

    ContentAccess::CContent* content = ContentAccess::CContent::NewLC( aFile );

    TInt isProtected( 0 );
    TInt err = content->GetAttribute( ContentAccess::EIsProtected, isProtected );
    if ( !err && isProtected )
        {
        TInt isForwardable( 0 );
        err = content->GetAttribute( ContentAccess::EIsForwardable, isForwardable );
        if ( !err && isForwardable )
            {
            protection |= EFileProtSuperDistributable;
            }
        else
            {
            protection |= EFileProtForwardLocked;
            }
            
        // Is DRM2
        TInt drmType( 0 );
        content->GetAttribute( ContentAccess::EFileType, drmType );
        if ( drmType == ContentAccess::EOma2Dcf )
            {
            protection |= EFileProtDrm2;
            }
            else
            {
            protection &= (~EFileProtDrm2);
            }

        HBufC* contentUri = HBufC::NewLC( KMaxContentUriLength );
        TPtr tempPtr = contentUri->Des();
        // "EContentID" has the "content uri" we want. Not "EContentURI"!
        err = content->GetStringAttribute( ContentAccess::EContentID, tempPtr );
        if ( !err && contentUri->Length() > 0 )
            {
            // Convert to 8-bit string
            aContentUri = HBufC8::NewL( contentUri->Length() );
            aContentUri->Des().Copy( *contentUri );
            }
        else
            {
            // If content id cannot be retrieved or is invalid 
            // then the file is treated as an DRM 2 file.
            protection |= EFileProtDrm2;
            }
        
        CleanupStack::PopAndDestroy( contentUri );
        }
    else
        {
        if ( aMimeType.Des8().CompareF( KFileProtMimeJar ) == 0 )
            {
            CJavaProtectionResolver* javaResolver = CJavaProtectionResolver::NewLC( iFs );
            if ( javaResolver->IsSuperDistributionPackageL( aFile ) )
                {
                protection |= EFileProtSuperDistributable;
                }
            CleanupStack::PopAndDestroy( javaResolver );
            }
        }

    CleanupStack::PopAndDestroy( content );        
    return protection;
    }

//  End of File  
