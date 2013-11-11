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
* Description:  Implement the operation: moveobject
*
*/


#include <mtp/cmtptypeobjectproplist.h>

#include "cabstractmediamtpdataprovidermoveobject.h"
#include "abstractmediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderMoveObject::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CAbstractMediaMtpDataProviderMoveObject::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CAbstractMediaMtpDataProviderMoveObject* self =
        new ( ELeave ) CAbstractMediaMtpDataProviderMoveObject( aFramework,
            aConnection,
            aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderMoveObject::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderMoveObject::ConstructL()
    {
    CMoveObject::ConstructL();
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderMoveObject::~CAbstractMediaMtpDataProviderMoveObject
// Destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderMoveObject::~CAbstractMediaMtpDataProviderMoveObject()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderMoveObject::CAbstractMediaMtpDataProviderMoveObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderMoveObject::CAbstractMediaMtpDataProviderMoveObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CMoveObject( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderMoveObject::ServiceGetSpecificObjectPropertyL
// do nothing here
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderMoveObject::ServiceGetSpecificObjectPropertyL( TUint16 /*aPropCode*/,
    TUint32 /*aHandle*/,
    const CMTPObjectMetaData& /*aObject*/ )
    {
    PRINT( _L( "MM MTP <> CAbstractMediaMtpDataProviderMoveObject::ServiceGetSpecificObjectPropertyL" ) );

    // Do nothing now.
    // May need add implementation here for further extension.

    }

// end of file
