/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Resource Manager.
*
*/



// INCLUDE FILES
#include "cphsrvresourcemanager.h" 
#include "phcltclientserver.h" 
#include <bautils.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvResourceManager::CPhSrvResourceManager
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
CPhSrvResourceManager::CPhSrvResourceManager( RFs& aFsSession )
:   iFsSession(aFsSession)
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvResourceManager::~CPhSrvResourceManager
// 
// C++ destructor
// -----------------------------------------------------------------------------
//
CPhSrvResourceManager::~CPhSrvResourceManager()
    {
    iResourceFile.Close();
    }


// -----------------------------------------------------------------------------
// CPhSrvResourceManager::ConstructL
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CPhSrvResourceManager::ConstructL()
    {
    TFileName resourceFile( KPhCltServerZDrive );
    resourceFile.Append( KDC_RESOURCE_FILES_DIR );
    resourceFile.Append( KPhClientAndServerResourceFileName );
    resourceFile.Append( KPhClientAndServerResourceFileExtensionNoWild );
    resourceFile.ZeroTerminate();

    BaflUtils::NearestLanguageFile( iFsSession, resourceFile );
    iResourceFile.OpenL( iFsSession, resourceFile );
    iResourceFile.ConfirmSignatureL( KPhSrvMagicResourceFileSignature );
    }


// -----------------------------------------------------------------------------
// CPhSrvResourceManager::ReadResourceLC
// 
// Read resource text
// -----------------------------------------------------------------------------
//
HBufC* CPhSrvResourceManager::ReadResourceLC( TUint aResourceId )
    {
    HBufC8* resourceBuffer = iResourceFile.AllocReadLC( aResourceId );

    TResourceReader reader;
    reader.SetBuffer( resourceBuffer );

    HBufC* ret = reader.ReadHBufCL();
    CleanupStack::PopAndDestroy( resourceBuffer );

    CleanupStack::PushL( ret );
    return ret;
    }


// End of File
