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
* Description:
*  Interface   : Private
*
*/


#ifndef VOICEMAILBOXPRIVATECRKEYS_H
#define VOICEMAILBOXPRIVATECRKEYS_H
#include <e32std.h>

const TInt KVmbxUiUserEditNotAllowed = 0x01;


/** VoiceMailbox Settings API */
/** Provides access to the VoiceMailbox numbers and settings */

const TUid KCRUidVoiceMailbox = {0x101F874F};


/** Flag which decides where the vmbx number is to be stored.
* If this is On (equals 1) the voice mailbox number is stored in SIM memory,
* else it is stored in Phone memory.
*
* Integer type
*
* Default value: 1
*/
const TUint32 KVmbxUsesSimMemory           = 0x00000000;

/** Flag which decides if writing to sim allowed.
* If this is On (equals 1) writing to SIM is not allowed.
* Writing is allowed with value 0
* Value
* Integer type
*
* Default value: 1
*/
const TUint32 KVmbxNoSimWriting            = 0x00000001;

/** Voice Mailbox number for primary line
*
** Text type
*
* Default value: ""
*/
const TUint32 KVmbxNumberLinePrimary       = 0x00000002;

/** Voice Mailbox number for Alternate line
*
** Text type
*
* Default value: ""
*/
const TUint32 KVmbxNumberLineAlternate     = 0x00000003;

/** Video Mailbox number for primary line
*
* Text type
*
* Default value: ""
*/
const TUint32 KVmbxVideoNmbrLinePrimary    = 0x00000004;

/** Video Mailbox number for Alternate line
*
* Text type
*
* Default value: ""
*/
const TUint32 KVmbxVideoNmbrLineAlternate  = 0x00000005;

/** Flag which User allowed or not allowed to change the VMBX number
* If set value 0x01 no allowed to be edited by user.
*
* Integer type0
* 0 indicate allowed
* Default value: 0
*/
const TUint32 KVmbxUiCaps                   = 0x00000006;

/** Flag which Video mailbox is supported or NOT
*
* Integer type
*
* Default value: 1
*/
const TUint32 KVmbxVideoMbxSupport = 0x00000015;


#endif      // VOICEMAILBOXPRIVATECRKEYS_H
