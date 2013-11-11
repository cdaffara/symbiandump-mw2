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
 * Description: Telephony Multimedia Service
 *
 */


#ifndef METADATAUTILITYCRKEYS_H
#define METADATAUTILITYCRKEYS_H

const TUid KCRUidMetadataUtility = {0x10207AF7};

// Auto Detect for Chineses BIG5 & GB2312 character sets in ID3v2
const TUint32 KMetadataUtilityAutoDetectChineseChars =  	0x00000001;
const TUint32 KMetadataUtilityAutoDetectRussianChars =  	0x00000002;
const TUint32 KMetadataUtilityVFKKSpecificMapping =  		0x00000003;
const TUint32 KMetadataUtilityAutoDetectJapaneseChars =  	0x00000004;

#endif      // METADATAUTILITYCRKEYS_H