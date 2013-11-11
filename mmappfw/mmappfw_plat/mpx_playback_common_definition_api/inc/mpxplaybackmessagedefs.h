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



#ifndef MPXPLAYBACKMESSAGEDEFS_H
#define MPXPLAYBACKMESSAGEDEFS_H


#include <e32base.h>
#include <mpxattribute.h>
/**
* Media changed message
*/
const TInt KMPXMessagePbMediaChanged = 0x10282968;

/**
 *  Attributes supported for KMPXMessageIdItemChanged
 */    
/**
 *  CMPXMedia. new media object
 */
const TMPXAttributeData KMPXMessagePbMedia={KMPXMessagePbMediaChanged, 0x01};

#endif // MPXPLAYBACKMESSAGEDEFS_H

// End of file
