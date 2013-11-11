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
* Description:  
*
*/

#include "mdsdatabaseupdater.h"
#include "mdssqliteconnection.h"
#include "mdssqldbmaintenance.h"
#include "mdccommon.h"
#include "mdspreferences.h"

CMdSDatabaseUpdater* CMdSDatabaseUpdater::NewL()
    {
    CMdSDatabaseUpdater* self = new ( ELeave ) CMdSDatabaseUpdater();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;  
    }

void CMdSDatabaseUpdater::ConstructL()
    { 
    }

CMdSDatabaseUpdater::~CMdSDatabaseUpdater()
    {
    }

TBool CMdSDatabaseUpdater::UpdateDatabaseL( TInt64 /*aMinorVersion*/ )
    { 
    return EFalse;
    }
