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
* Description:  More definitions for general media type
*
*/

#ifndef CMPXMEDIAGENERALEXTDEFS_H
#define CMPXMEDIAGENERALEXTDEFS_H

#include <e32base.h>
#include <mpxattribute.h>


/**
*  Content ID identifying extended general category of content provided
*  in the media object and associated attributes.
*/

const TInt KMPXMediaIdGeneralExt=0x101F3C72;

/**
*  Attributes supported for KMPXMediaIdStreaming
*/
const TMPXAttributeData KMPXMediaGeneralExtAccessPoint={KMPXMediaIdGeneralExt,0x01}; // Tint
const TMPXAttributeData KMPXMediaGeneralExtMediaRedirect={KMPXMediaIdGeneralExt,0x02}; // Tint
const TMPXAttributeData KMPXMediaGeneralExtVideoSeekable={KMPXMediaIdGeneralExt,0x04}; // TBool
const TMPXAttributeData KMPXMediaGeneralExtRating={KMPXMediaIdGeneralExt,0x08}; // TInt
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
const TMPXAttributeData KMPXMediaGeneralExtSizeInt64={KMPXMediaIdGeneralExt,0x10}; // TInt64
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

#endif // CMPXMEDIAGENERALEXTDEFS_H

