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
* Description:  Definitions for image media type
*
*/



#ifndef CMPXMEDIAIMAGEDEFS_H
#define CMPXMEDIAIMAGEDEFS_H


#include <e32base.h>
#include <mpxattribute.h>

/**
*  Content ID identifying image category of content provided
*  in the media object and associated attributes. 
*/

const TInt KMPXMediaIdImage=0x101FFC44;

/**
*  Attributes supported for KMPXMediaIdImage
*/
const TMPXAttributeData KMPXMediaImageSize={KMPXMediaIdImage,0x01};
const TMPXAttributeData KMPXMediaImageAll={KMPXMediaIdImage,0xffffffff};

/** DEPRECATED, please use consts above
*  Attributes supported for KMPXMediaIdImage
*/
enum 
    {
    EMPXMediaImageSize=0x01, // TSize   
    EMPXMediaImageAll=0xffffffff
    };

  
#endif // CMPXMEDIAIMAGEDEFS_H
