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
* Description:  Contains constants used by the database.
*
*/


#ifndef     CBSDBCONSTANTS_H
#define     CBSDBCONSTANTS_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// The default size of database observer array.
const TInt KCbsDbObserverArraySize = 3;

// The default size of the topic array.
const TInt KCbsDbTopicArraySize = 50;

// The maximum amount of received (not saved) messages in a topic.
const TInt KCbsDbMaxReceivedMessages = 6;

// The maximum amount of saved (not received) messages (all topics).
const TInt KCbsDbMaxSavedMessages = 50;

// Minimum valid topic number
const TInt KCbsMinValidTopicNumber = 0;

// Maximum valid topic number 
const TInt KCbsMaxValidTopicNumber = 999;

// Maximum number of characters in one message. 15 pages, 93 characters per page.
const TUint KCbsMaxCharsInMessage = 15*93;
  
#endif      //  CBSDBCONSTANTS_H   
            
// End of File


