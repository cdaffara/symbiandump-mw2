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
* Description:  Definitions for media type containing other media objects
*
*/



#ifndef CMPXMEDIACONTAINERDEFS_H
#define CMPXMEDIACONTAINERDEFS_H

#include <e32base.h>
#include <mpxattribute.h>

/**
*  Content ID identifying conatiner category. This media object
*  contains other media objects contained in a CMPXMediaArray  
*/

const TInt KMPXMediaIdContainer=0x101FFC43;

/**
*  Attributes supported for KMPXMediaIdContainer
*/    
const TMPXAttributeData KMPXMediaArrayContents = {KMPXMediaIdContainer, 0x01}; // CMPXMediaArray
const TMPXAttributeData KMPXMediaArrayCount = {KMPXMediaIdContainer, 0x02}; // TInt
const TMPXAttributeData KMPXMediaArrayAll = {KMPXMediaIdContainer, 0xffffffff};

/** 
*  Attributes supported for KMPXMediaIdContainer.
*
*  @deprecated Use the above consts
*/    
enum 
    {
    EMPXMediaArrayContents=0x01, // CMPXMediaArray
    EMPXMediaArrayCount=0x02, // TInt
    EMPXMediaArrayAll=0xffffffff
    };
     
  
#endif // CMPXMEDIACONTAINERDEFS_H
