/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definitions for subscription attributes
*
*/



#ifndef MPXSUBSCRIPTIONDEFS_H
#define MPXSUBSCRIPTIONDEFS_H

#include <mpxattribute.h>

/**
* Content ID identifying general category of content provided
* in the message object and associated attributes. This should
* be supported by ALL message objects as a common base
*/

const TInt KMPXSubscriptionGeneral = 0x1028294A;

/**
*  TMPXMessageId, *unique* id of the message
*/
const TMPXAttributeData KMPXSubscriptionItems = {KMPXSubscriptionGeneral, 0x01};

#endif // MPXSUBSCRIPTIONDEFS_H
