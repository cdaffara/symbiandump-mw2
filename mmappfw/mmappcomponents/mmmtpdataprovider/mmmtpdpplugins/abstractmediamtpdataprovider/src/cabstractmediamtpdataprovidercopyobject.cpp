/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implement the operation: copyobject
*
*/


#include <mtp/cmtptypeobjectproplist.h>

#include "cabstractmediamtpdataprovidercopyobject.h"
#include "abstractmediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpdefs.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderCopyObject::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CAbstractMediaMtpDataProviderCopyObject::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CAbstractMediaMtpDataProviderCopyObject* self =
        new ( ELeave ) CAbstractMediaMtpDataProviderCopyObject( aFramework,
            aConnection,
            aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderCopyObject::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderCopyObject::ConstructL()
    {
    CCopyObject::ConstructL();
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderCopyObject::~CAbstractMediaMtpDataProviderCopyObject
// Destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderCopyObject::~CAbstractMediaMtpDataProviderCopyObject()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderCopyObject::CAbstractMediaMtpDataProviderCopyObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderCopyObject::CAbstractMediaMtpDataProviderCopyObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CCopyObject( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderCopyObject::ServiceGetSpecificObjectPropertyL
// do nothing here
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderCopyObject::ServiceGetSpecificObjectPropertyL( TUint16 /*aPropCode*/,
    TUint32 /*aHandle*/,
    const CMTPObjectMetaData& /*aObject*/ )
    {
    PRINT( _L( "MM MTP <> CAbstractMediaMtpDataProviderCopyObject::ServiceGetSpecificObjectPropertyL" ) );
    // Do nothing now.
    // May need add implementation here for further extension.
    }

// end of file
