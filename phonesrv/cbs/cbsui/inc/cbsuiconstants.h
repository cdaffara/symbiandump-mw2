/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
*     Contains constants which are used in CBSUI application.
*
*
*/


#ifndef __CBS_UI_CONSTANTS_H
#define __CBS_UI_CONSTANTS_H

//  INCLUDES

// CONSTANTS
const TInt KCbsUiCustomMessageIdInt = 11;
const TUid KCbsUiCustomMessageId = { KCbsUiCustomMessageIdInt };
                                    // When message view is launched by other
                                    // application its message id is 0, this 
                                    // just has to differ from the other 
                                    // applications message id.

// When view is launched with this id it knows to set the focus to id, 
// which is passed as parameter to DoActivateL.
const TUid KSetFocusToId = { 12 };

// activates view to its current id.
const TUid KSetFocusToCurrentId = { 13 };

// How many digits there are in topic number
const TInt KTopicNumberWidth = 3; //EMaxTopicNumberLength; 

const TInt KNumberOfTemporaryMessages = 3;

// Error value used in Cbs ui.
const TInt KCbsErrValue = -1;

// This kind of topic number cannot exists
const TInt KCbsInvalidTopicNumber = 22000;

// Max message title length
const TInt KMaxMessageTitleLength = 40;

// Max topic name length
const TInt KMaxTopicNameLength = 80;

// DATA TYPES
enum { EMinTopicNumber = 0, EMaxTopicNumber = 999 };

enum TCbsUiLanguageSelectionIndex
    { 
    ECbsUiAll = 0, 
    ECbsUiSelected = 1, 
    ECbsUiOther = 2
    };

// for activating topic view with right state
struct TCbsUiTopicActivationPckg
    {
    // current topic number
    TCbsTopicNumber iTopicNumber;

    // current message handle
    TCbsMessageHandle iMessageHandle;
    };

#endif      // __CBS_UI_CONSTANTS_H

