/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Stubs for phoneclientserver classes
*
*/

//  INCLUDES
#include "rphcltserver.h"
#include "cphsrvresourcemanager.h" 

// ---------------------------------------------------------------------
// Stub for RPhCltServer
// ---------------------------------------------------------------------

RPhCltServer::RPhCltServer()
{
}

RPhCltServer::RPhCltServer( const RPhCltServer& )
{
}

TVersion RPhCltServer::Version() const
{
    return TVersion(0,0,0);
}

TInt RPhCltServer::Connect( TInt )
{
    return KErrNone;
}

void RPhCltServer::CreateAll( TRequestStatus& )
{
}

void RPhCltServer::Panic( TPhCltPanicTypes /*aPanic*/ )
{
}

TInt RPhCltServer::StartServer()
{
    return KErrNone;
}


RPhCltServer& RPhCltServer::operator= ( const RPhCltServer& )
{
    return *this;
}

// ---------------------------------------------------------------------
// Stub for CPhSrvResourceManager
// ---------------------------------------------------------------------

CPhSrvResourceManager::CPhSrvResourceManager( RFs& aFsSession )
    :iFsSession(aFsSession)
    {
    }

CPhSrvResourceManager::~CPhSrvResourceManager()
    {
    }

void CPhSrvResourceManager::ConstructL()
    {
    }

HBufC* CPhSrvResourceManager::ReadResourceLC( TUint /*aResourceId*/ )
    {
    HBufC* buffer = HBufC::NewLC(32);
    buffer->Des().Copy(_L("Hello!"));
    return buffer;
    }

