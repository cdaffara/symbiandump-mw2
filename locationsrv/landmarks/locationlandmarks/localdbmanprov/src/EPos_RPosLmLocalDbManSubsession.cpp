/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This class is a subsession handle with operations related to
*  database management event listening.
*
*
*/



// INCLUDE FILES
#include <e32cmn.h>
#include <e32def.h>
#include <epos_poslmservercommon.h>
#include <epos_rposlandmarkserver.h>
#include "EPos_RPosLmLocalDbManSubsession.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RPosLmLocalDbManSubsession::RPosLmLocalDbManSubsession
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RPosLmLocalDbManSubsession::RPosLmLocalDbManSubsession()
    {
    }

// -----------------------------------------------------------------------------
// RPosLmLocalDbManSubsession::Open
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalDbManSubsession::Open(RPosLandmarkServer& aServer)
    {
    return CreateSubSession(aServer,
                            EPosLmServerRegisterLocalDbManSubsession);
    }

// -----------------------------------------------------------------------------
// RPosLmLocalDbManSubsession::Close
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void RPosLmLocalDbManSubsession::Close()
    {
    CloseSubSession(EPosLmServerUnregisterSubsession);
    }

// ---------------------------------------------------------
// RPosLmLocalDbManSubsession::CreateDatabase
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RPosLmLocalDbManSubsession::CreateDatabase(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TInt size = aDatabaseInfo.Size();
    TPtr8 ptr(reinterpret_cast<TUint8*>(&aDatabaseInfo), size, size);

    return SendReceive(EPosLmServerCreateDatabase,
                TIpcArgs(&ptr));
    }

// ---------------------------------------------------------
// RPosLmLocalDbManSubsession::DeleteDatabase
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RPosLmLocalDbManSubsession::DeleteDatabase(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TInt size = aDatabaseInfo.Size();
    TPtr8 ptr(reinterpret_cast<TUint8*>(&aDatabaseInfo), size, size);

    return SendReceive(EPosLmServerDeleteDatabase,
                TIpcArgs(&ptr));
    }

// ---------------------------------------------------------
// RPosLmLocalDbManSubsession::CopyDatabase
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt RPosLmLocalDbManSubsession::CopyDatabase(
    const TDesC& aSourceUri,
    const TDesC& aTargetUri)
    {
    return SendReceive(EPosLmServerCopyDatabase,
                TIpcArgs(&aSourceUri, &aTargetUri));
    }



// End of File
