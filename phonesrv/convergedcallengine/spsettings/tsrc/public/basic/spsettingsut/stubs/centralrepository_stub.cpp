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
* Description:  Cenrep stub
*
*/

#include <centralrepository.h>


CRepository* CRepository::NewLC( TUid )
    {
    CRepository* ret = new (ELeave) CRepository;
    CleanupStack::PushL( ret );
    return ret;
    }

CRepository::~CRepository()
    {
    
    }

TInt CRepository::Reset()
    {
    return KErrNone;
    }

TInt CRepository::Get( TUint32 , TInt& )
    {
    return KErrNone;
    }

TInt CRepository::Set( TUint32 , TInt )
    {
    return KErrNone;
    }
