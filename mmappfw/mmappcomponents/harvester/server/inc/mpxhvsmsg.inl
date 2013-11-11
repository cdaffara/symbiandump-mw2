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
* Description:  Inline function declarations for harvester messages
*
*/


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
inline RHvsMsg::RHvsMsg( const RMessage2 aMsg,TMPXHarvesterServerOp aOp ) :
                                                            iMsg( aMsg ),
                                                            iOp( aOp) 
    {
    }
// ---------------------------------------------------------------------------
// Returns RMessage2
// ---------------------------------------------------------------------------
//
inline const RMessage2& RHvsMsg::Message()
    {
    return iMsg;
    }

// ---------------------------------------------------------------------------
// Returns the operation
// ---------------------------------------------------------------------------
//
inline const TMPXHarvesterServerOp RHvsMsg::Op() 
    {
    return iOp;
    }
    