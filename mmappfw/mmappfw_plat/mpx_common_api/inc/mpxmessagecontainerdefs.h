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
* Description:  Definitions for message type containing other message objects
*
*/



#ifndef CMPXMESSAGECONTAINERDEFS_H
#define CMPXMESSAGECONTAINERDEFS_H


#include <e32base.h>
#include <mpxattribute.h>

/**
*  Content ID identifying conatiner category. This message object
*  contains other message objects contained in a CMPXMessageArray  
*/

const TInt KMPXMessageIdContainer=0x10282946;
/**
*  Attributes supported for KMPXMediaIdContainer
*/
// CMPXMessageArray
const TMPXAttributeData KMPXMessageArrayContents={KMPXMessageIdContainer, 0x01};
const TMPXAttributeData KMPXMessageArrayCount={KMPXMessageIdContainer, 0x02}; // TInt
const TMPXAttributeData KMPXMessageArrayAll={KMPXMessageIdContainer, 0xffffffff};

/**
*  Attributes supported for KMPXMediaIdContainer.
*
*  @deprecated Use the above consts
*/    
enum 
    {
    EMPXMessageArrayContents=0x01, // CMPXMessageArray
    EMPXMessageArrayCount=0x02, // TInt
    EMPXMessageArrayAll=0xffffffff
    };
     
  
#endif // CMPXMESSAGECONTAINERDEFS_H
