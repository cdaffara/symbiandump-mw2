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
*  Interface   : Domain
*
*/


#ifndef VOICEMAILBOXDOMAINCRKEYS_H
#define VOICEMAILBOXDOMAINCRKEYS_H
#include <e32std.h>

/** VoiceMailbox Settings API */
/** Provides access to the VoiceMailbox video mailbox support key
determination */

const TUid KCRUidVideoMailbox = {0x101F874F};

/**
*
* Integer type
*
* Default value: 1
*/
const TUint32 KVideoMbxSupport = 0x00000015;


#endif      // VOICEMAILBOXDOMAINCRKEYS_H
