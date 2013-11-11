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
* Description:  Media Fetch Implementation API
*
*/


// INCLUDE FILES
#include    <ecom/ecom.h>

// -----------------------------------------------------------------------------
// ?implementation_description
// -----------------------------------------------------------------------------
//
inline CMediaFetchImplementation* CMediaFetchImplementation::NewL(
                                    const TUid& aImplementationUid )
    {
    TAny* ptr = REComSession::CreateImplementationL(
                aImplementationUid,
                _FOFF( CMediaFetchImplementation, iDtor_ID_Key ) );

    return reinterpret_cast<CMediaFetchImplementation*>(ptr);
    }


// -----------------------------------------------------------------------------
// ?implementation_description
// -----------------------------------------------------------------------------
//
inline CMediaFetchImplementation::~CMediaFetchImplementation()
    {
    if( iDtor_ID_Key.iUid != 0 )
        {
        REComSession::DestroyedImplementation( iDtor_ID_Key );
        }
    }
