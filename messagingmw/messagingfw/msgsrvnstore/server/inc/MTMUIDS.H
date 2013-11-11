// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// MTMUIDS.H
//

/**
 @file
 @publishedAll
 @released
*/
#if !defined(__MTMUIDS_H__)
#define __MTMUIDS_H__
#if !defined(__E32STD_H__)
#include <e32std.h>
#endif
#if !defined(__MTMDEF_HRH__)
#include <mtmdef.hrh>
#endif
#if !defined(__MTMUIDSDEF_HRH__)
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include <mtmuidsdef.hrh>
#endif
#endif

// capabilities (TUid)
/**
* Maximum message body size.
* @Return type: TInt. 
* @publishedAll
* @released
*/
const TUid KUidMtmQueryMaxBodySize={KUidMtmQueryMaxBodySizeValue};
/**
* Maximum total size of message.
* @publishedAll
* @released
* @Return type: TInt.
*/
const TUid KUidMtmQueryMaxTotalMsgSize={KUidMtmQueryMaxTotalMsgSizeValue};
/** Character widths supported by message type (returned TInt is the sum of the 
appropriate values KMtm7BitBody, KMtm8BitBody, KMtm16BitBody, and KMtmBinaryBody). */

/**
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySupportedBody={KUidMtmQuerySupportedBodyValue};
/**
* Are attachments supported.
* @No return value. 
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySupportAttachments={KUidMtmQuerySupportAttachmentsValue};
/**
* Does the MTM message type have a subject field.
* @No return value. 
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySupportSubject={KUidMtmQuerySupportSubjectValue};
/**
* Does the MTM support folders.
* @No return value. 
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySupportsFolder={KUidMtmQuerySupportsFolderValue};
/**
* Off-line operation allowed.
* @No return value.
* @publishedAll
* @released
*/
const TUid KUidMtmQueryOffLineAllowed={KUidMtmQueryOffLineAllowedValue};
/**
* Send message.
* @No return value.
* @publishedAll
* @released
*/ 
const TUid KUidMtmQueryCanSendMsg={KUidMtmQueryCanSendMsgValue};
/**
* Receive message.
* @No return value. 
* @publishedAll
* @released
*/
const TUid KUidMtmQueryCanReceiveMsg={KUidMtmQueryCanReceiveMsgValue};
/**
* Maximum number of recipients (-1 indicates unlimited numbers).
* @Return type: TInt
* @publishedAll
* @released
*/
const TUid KUidMtmQueryMaxRecipientCount={KUidMtmQueryMaxRecipientCountValue};
/**
* When using the MTM in Send-As, does a rendered image have to be prepared (i.e.fax).
* @Return type: TBool. 
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySendAsRequiresRenderedImage={KUidMtmQuerySendAsRequiresRenderedImageValue};
/**
* Printer driver UID for rendering the fax image.
* @Return type: TUid
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySendAsRenderingUid={KUidMtmQuerySendAsRenderingUidValue};
/**
* UID of default message editor.
* @Return type: TUid. 
* @publishedAll
* @released
*/
const TUid KUidMsvMtmQueryEditorUid={KUidMsvMtmQueryEditorUidValue};
/**
* Does the MTM support BIO messages.
* @No return value. 
* @publishedAll
* @released
*/
const TUid KUidMsvQuerySupportsBioMsg={KUidMtmQuerySupportsBioMsgValue};
/**
* Does the MTM support scheduled sending.
* @No return value. 
* @publishedAll
* @released
*/
const TUid KUidMsvQuerySupportsScheduling={KUidMtmQuerySupportsSchedulingValue};
/**
* Does the MTM support the use of recipient type.
* @No return value. 
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySupportsRecipientType={KUidMtmQuerySupportsRecipientTypeValue};
/**
* Support for Sending messages using SendAs.  
* If this is supported, then the MTM supports sending messages created through the SendAs API.
* @Return type: TBool. 
* @publishedAll
* @released
*/
const TUid KUidMtmQuerySendAsMessageSendSupport={KUidMtmQuerySendAsMessageSendSupportValue};

#endif
