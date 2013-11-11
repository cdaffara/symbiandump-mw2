/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains server typedefs and enumerations.
*                Note that most of these types are aliases to database types.
*
*/



#ifndef     CBSSERVERTYPES_H
#define     CBSSERVERTYPES_H

// INCLUDES

#include <e32base.h>
#include "CbsServerConstants.h"

// DATA TYPES

// Represents a handle to a message.
typedef TCbsDbMessageHandle TCbsMessageHandle;

// Represents the length of the message.
typedef TCbsDbMessageLength TCbsMessageLength;

// Represents a handle to a topic collection.
typedef TUint TCbsTopicCollectionHandle;

// Represents a handle to a topic.
typedef TCbsDbTopicHandle TCbsTopicHandle;

// Represents topic list handles.
typedef TInt16 TCbsTopicListHandle;

// Represents the topic numbers.
typedef TCbsDbTopicNumber TCbsTopicNumber;

// Enumerates the reasons for the client panic.
typedef TCbsDbMessageKey TCbsMessageKey;

#endif      //  CBSSERVERTYPES_H   
            
// End of File


