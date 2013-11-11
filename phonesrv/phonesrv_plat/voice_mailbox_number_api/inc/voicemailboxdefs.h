/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Constant used for vmbx engine
*
*/

#ifndef VOICEMAILBOXDEFS_H
#define VOICEMAILBOXDEFS_H

//  INCLUDES
#include <e32base.h>
#include <spdefinitions.h>

/** Null service id, i.e. not specified service. */
const TServiceId KVmbxServiceIdNone( 0 );
/** Service id for CS voice mailbox */
const TServiceId KVmbxServiceVoice( 0x01 );
/** Service id for CS video mailbox */
const TServiceId KVmbxServiceVideo( 0x01 );
/** The max length of voice mailbox number */
const TInt KVmbxMaxNumberLength = 100;

/**  Vmbx ALS line determination */
enum TVmbxAlsLineType
    {
    /**
     *  Default line, meaning current active line.
     */
    EVmbxAlsLineDefault,
    /**
     *  Current ALS line is Line1.
     */
    EVmbxAlsLine1,
    /**
     *  Current ALS line is Line2.
     */
    EVmbxAlsLine2
    };

/** Type of Voice Mailbox. */
enum TVmbxType
    {
    /**
     *  None vmbx type.
     */
    EVmbxNone = 0,
    /**
     *  Voice type.
     */
    EVmbxVoice,
    /**
     *  Video type.
     */
    EVmbxVideo,
    /**
     *  Voip type.
     */
    EVmbxVoip
    };

/** Voice Mailbox feature flag definitions */
enum TVmbxFeatureCapabilityFlag
    {
    /**
     * Used to check whether user editing is allowed or not
     */
    EVmbxChangeNbrAllowedOnUi = 0x01,

    /**
     * Used to check whether video mailbox is supported or not
     */
    EVmbxVideoMailboxSupported = 0x02
    };

/** Vmbx Memory location which is currently used */
enum TVmbxMemoryLocation
    {
    /**
     *  Using phone memory.
     */
    EVmbxPhoneMemory,
    /**
     *   Using Sim memory.
     */
    EVmbxSimMemory
    };

/** Vmbx query type which currently wanted */
enum TVmbxQueryMode
    {
    /**
     *  User want to define number.
     */
    EVmbxDefineMode,
    /**
     *  User want to change number.
     */
    EVmbxChangeMode
    };

#endif // VOICEMAILBOXDEFS_H
