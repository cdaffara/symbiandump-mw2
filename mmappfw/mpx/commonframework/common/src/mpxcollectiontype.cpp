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
* Description:  Class representing the supported collection types
*
*/


#include <e32base.h>
#include <mpxuser.h>
#include "mpxcollectiontype.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionType::CMPXCollectionType(
    const TUid& aUid,
    CDesCArray* aMimeTypes, 
    CDesCArray* aExtensions)
:   iUid(aUid), iMimeTypes(aMimeTypes),iExtensions(aExtensions)
    {
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionType::CMPXCollectionType()
    {
    }
    
// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CMPXCollectionType::~CMPXCollectionType()
    {
    delete iMimeTypes;
    delete iExtensions;
    }

// ---------------------------------------------------------------------------
// Externalize the object
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionType::ExternalizeL(RWriteStream& aStream) const 
    {
    aStream.WriteInt32L( iUid.iUid );
    MPXUser::ExternalizeL(iMimeTypes, aStream);
    MPXUser::ExternalizeL(iExtensions, aStream);
    }

// ---------------------------------------------------------------------------
// Internalize the object
// ---------------------------------------------------------------------------
//    
EXPORT_C void CMPXCollectionType::InternalizeL(RReadStream& aStream)
    {
    // UID
    iUid.iUid = aStream.ReadInt32L();
    MPXUser::InternalizeL(iMimeTypes, aStream);
    MPXUser::InternalizeL(iExtensions, aStream);
    }
