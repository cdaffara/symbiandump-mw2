/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provides private utilities for SendUi.
*
*/




// INCLUDE FILES
#include <apmstd.h>
#include <apgcli.h>
#include <caf/caf.h>
//#include <MsgMimeTypes.h>

#include "SendUiUtils.h"

// CONSTANTS
_LIT8( KMmsSymbianInternalPattern, "x-epoc/x-app0");

const TInt KSenduiUrlMaxLen = 1024;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// SendUiUtils::OpenFileHandleL
// -----------------------------------------------------------------------------
//
RFile SendUiUtils::OpenFileHandleL( const TDesC& aFilePath, RFs& aFs )
    {
    }

// -----------------------------------------------------------------------------
// SendUiUtils::DrmContentUrlL
// -----------------------------------------------------------------------------
//
TBool SendUiUtils::HasContentUrlL( RFile& aFile )
    {
    }

// -----------------------------------------------------------------------------
// SendUiUtils::ResolveMimeTypeL
// -----------------------------------------------------------------------------
//
void SendUiUtils::ResolveMimeTypeL( const RFile& aFile, TDataType& aMimeType )
    {
    }

//  End of File  
