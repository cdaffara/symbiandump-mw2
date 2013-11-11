/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
*       Stub for CJavaProtectionResolver when Java feature is not supported.
*
*/




// INCLUDE FILES
#include <e32std.h>
#include <e32base.h> // CBase
#include <apmstd.h>  // TDataType
#include <f32file.h> // RFs, RFile, CFileMan

#include "javaprotectionresolver.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// Factory
// -----------------------------------------------------------------------------
//
CJavaProtectionResolver* CJavaProtectionResolver::NewLC( RFs& aFs )
    {
    CJavaProtectionResolver* self = new ( ELeave ) CJavaProtectionResolver( aFs );
    CleanupStack::PushL( self );
    return self;
    }
         
// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CJavaProtectionResolver::CJavaProtectionResolver( RFs& aFs ) :
    iFs( aFs )
    {
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CJavaProtectionResolver::~CJavaProtectionResolver()
    {
    }

// -----------------------------------------------------------------------------
// CJavaProtectionResolver::CheckDRMStatus
// -----------------------------------------------------------------------------
//
TBool CJavaProtectionResolver::IsSuperDistributionPackageL( RFile& /*aFile*/ )
    {
    return EFalse;
    }

//  End of File  
