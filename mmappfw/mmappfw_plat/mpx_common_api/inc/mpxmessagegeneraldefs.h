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
* Description:  Definitions for message type
*
*/



#ifndef CMPXMESSAGEGENERALDEFS_H
#define CMPXMESSAGEGENERALDEFS_H

#include <e32base.h>
#include <mpxattribute.h>

/**
* Content ID identifying general category of content provided
* in the message object and associated attributes. This should
* be supported by ALL message objects as a common base
*/

const TInt KMPXMessageContentIdGeneral = 0x10282944;
/**
*  Attributes supported for KMPXMessageIdGeneral
*/    
/**
*  TMPXMessageId, *unique* id of the message
*/
const TMPXAttributeData KMPXMessageGeneralId={KMPXMessageContentIdGeneral, 0x01};
/**
 *  General event, optional, TInt
 */
const TMPXAttributeData KMPXMessageGeneralEvent={KMPXMessageContentIdGeneral, 0x02};
/**
 *  General type, optional, TInt
 */
const TMPXAttributeData KMPXMessageGeneralType={KMPXMessageContentIdGeneral, 0x04};
/**
 *  General data, optional, TInt
 */
const TMPXAttributeData KMPXMessageGeneralData={KMPXMessageContentIdGeneral, 0x08};

/** 
*  Attributes supported for KMPXMessageIdGeneral.
*
*  @deprecated Use the above consts
*/    
enum 
    {
    /**
    *  TMPXMessageId, *unique* id of the message
    */
    EMPXMessageGeneralId = 0x01
    
    };

/**
 *  Unique id of KMPXMessageGeneral
 *  KMPXMessageGeneral contains the attributes: KMPXMessageGeneralEvent,
 *  KMPXMessageGeneralType, KMPXMessageGeneralData
 */
const TInt KMPXMessageGeneral = 0x10282967;

#endif // CMPXMESSAGEGENERALDEFS_H
