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
* Description:  Inline implementation of collection messge 
*
*/


// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
inline TMPXCollectionMessage::TMPXCollectionMessage(TEvent aEvent,
                                                    TInt aType/*= 0*/,
                                                    TInt aData/*= 0*/)
    : TMPXMessage(aEvent,aType, aData)
    {}

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
inline TMPXCollectionMessage::TMPXCollectionMessage()
    : TMPXMessage(ENoEvent,0,0)
    {} 
