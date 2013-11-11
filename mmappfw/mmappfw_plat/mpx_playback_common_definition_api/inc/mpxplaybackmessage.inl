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
* Description:  Inline implementation of playback messge 
*
*/


// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
inline TMPXPlaybackMessage::TMPXPlaybackMessage(TEvent aEvent,
                                                TInt aType /*= 0*/,
                                                TInt aData /*= 0*/)
    : TMPXMessage(aEvent, aType, aData)
    {}

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
inline TMPXPlaybackMessage::TMPXPlaybackMessage()
    : TMPXMessage(ENoEvent,0,0)
    {} 

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
//
inline TMPXPlaybackMessage::TMPXPlaybackMessage(const TMPXPlaybackMessage& aMsg)
    : TMPXMessage(aMsg)
    {
    }

// -----------------------------------------------------------------------------
// Overloaded assignment operator
// -----------------------------------------------------------------------------
//
inline TMPXPlaybackMessage& TMPXPlaybackMessage::operator=(
    const TMPXPlaybackMessage& aMsg)
    {
    TMPXMessage::operator =(aMsg);
    return *this;
    }
