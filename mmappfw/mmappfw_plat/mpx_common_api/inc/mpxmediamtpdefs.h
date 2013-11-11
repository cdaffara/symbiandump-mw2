/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definitions for mtp media type
*
*/



#ifndef MPXMEDIAMTPDEFS_H
#define MPXMEDIAMTPDEFS_H


#include <e32base.h>
#include <mpxattribute.h>

/**
*  Content ID identifying MTP category of content provided
*  in the media object and associated attributes. 
*/
const TInt KMPXMediaIdMTP=0x10282970;

/**
*  Attributes supported for KMPXMediaIdMTP
*/    
const TMPXAttributeData KMPXMediaMTPDrmStatus = {KMPXMediaIdMTP, 0x0001}; // TUint16
const TMPXAttributeData KMPXMediaMTPSampleDataFlag = {KMPXMediaIdMTP, 0x0002}; // TBool
const TMPXAttributeData KMPXMediaMTPSampleData = {KMPXMediaIdMTP, 0x0004}; // HBufC8*
const TMPXAttributeData KMPXMediaMTPAll = {KMPXMediaIdMTP, 0xffffffff};

#endif // MPXMEDIAMTPDEFS_H
