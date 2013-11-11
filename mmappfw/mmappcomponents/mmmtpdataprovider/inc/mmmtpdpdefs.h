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
* Description:  define properties
*
*/


#ifndef MMMTPDPDEFS_H
#define MMMTPDPDEFS_H

/**
* define all the object properties that are supported by the mm mtp data provider
*/

// Mandatory for all formats
static const TUint16 KMmMtpDpSupportedPropMandatoryAll[] =
    {
    EMTPObjectPropCodeStorageID,
    EMTPObjectPropCodeObjectFormat,
    EMTPObjectPropCodeProtectionStatus,
    EMTPObjectPropCodeObjectSize,
    EMTPObjectPropCodeObjectFileName,
    EMTPObjectPropCodeParentObject,
    EMTPObjectPropCodePersistentUniqueObjectIdentifier,
    EMTPObjectPropCodeName,
    EMTPObjectPropCodeNonConsumable,
    };

// Additional for all foramts
static const TUint16 KMmMtpDpSupportedPropAdditionalAll[] =
    {
    EMTPObjectPropCodeDateAdded,
    EMTPObjectPropCodeDateCreated,
    EMTPObjectPropCodeDateModified,
    };

#endif // MMMTPDPDEFS_H
