/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class used for encoding landmark content.
*
*
*/


#include <ecom/ecom.h>
#include "EPos_CPosLandmarkEncoder.h"
#include "EPos_LandmarksUids.hrh"

// =========================== MEMBER FUNCTIONS ================================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkEncoder::CPosLandmarkEncoder()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkEncoder* CPosLandmarkEncoder::NewL(
    const TDesC8& aContentMimeType)
    {
    if ( aContentMimeType.Length() == 0 )
        {
        User::Leave( KErrArgument );
        }

    TEComResolverParams params;
    params.SetDataType(aContentMimeType);

    // Check if any implementation supports this mime type
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmEncoderIfUid;

    RImplInfoPtrArray implInfoArray;
    REComSession::ListImplementationsL(interfaceUid, params, implInfoArray);
    if (implInfoArray.Count() == 0)
        {
        User::Leave(KErrNotSupported);
        }
    else
        {
        implInfoArray.ResetAndDestroy();
        }

    HBufC8* strPtr = aContentMimeType.AllocLC();

    // Send the mime type to the Ecom server
    // which determines the implementation.
    TAny* ptr = REComSession::CreateImplementationL(
        interfaceUid,
        _FOFF(CPosLandmarkEncoder, iDtorIdKey),
        strPtr,
        params);

    CleanupStack::PopAndDestroy(strPtr);

    return reinterpret_cast<CPosLandmarkEncoder*>(ptr);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkEncoder::~CPosLandmarkEncoder()
    {
    // Destroy any instance variables and then inform the framework that this
    // specific instance of the interface has been destroyed.
    REComSession::DestroyedImplementation(iDtorIdKey);
    }
