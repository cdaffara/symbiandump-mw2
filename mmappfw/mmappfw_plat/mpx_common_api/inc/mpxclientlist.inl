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
* Description:  Inline implementation of client list
*
*/

#include <mpxmessagegeneraldefs.h>
#include <mpxlog.h>

// -----------------------------------------------------------------------------
// Convert TMPXMessage into CMPXMessage
// -----------------------------------------------------------------------------
//
inline CMPXMessage* CMPXClientList::ConvertMsgLC(const TMPXMessage& aMsg)
    {
    CMPXMessage* msg = CMPXMessage::NewL();
    CleanupStack::PushL(msg);
    TMPXMessageId id=static_cast<TMPXMessageId>(KMPXMessageGeneral);
    msg->SetTObjectValueL<TMPXMessageId>(KMPXMessageGeneralId, id);
    msg->SetTObjectValueL<TInt>(KMPXMessageGeneralEvent, aMsg.Event());
    msg->SetTObjectValueL<TInt>(KMPXMessageGeneralType, aMsg.Type());
    msg->SetTObjectValueL<TInt>(KMPXMessageGeneralData, aMsg.Data());
    return msg;
    }
// -----------------------------------------------------------------------------
// Send message to all clients in the list
// -----------------------------------------------------------------------------
//
inline void CMPXClientList::SendMsgL(const TMPXMessage& aMsg)
    {
    MPX_FUNC_EX("CMPXClientList::SendMsgL(aTMsg)");
    CMPXMessage* msg = ConvertMsgLC(aMsg);
    // Magic number 1 which is always be old EError value in TMPXMessage
    TInt err = aMsg.Event()==1 ? aMsg.Data() : KErrNone;
    SendMsg(msg, err);
    CleanupStack::PopAndDestroy(msg);
    }    
    
// -----------------------------------------------------------------------------
// Send message to a client
// -----------------------------------------------------------------------------
//
inline void CMPXClientList::SendMsgL(TInt aIndex, const TMPXMessage& aMsg)
    {
    MPX_FUNC_EX("CMPXClientList::SendMsgL(aIndex, aTMsg)");
    CMPXMessage* msg = ConvertMsgLC(aMsg);
    SendMsg(aIndex, msg, KErrNone);
    CleanupStack::PopAndDestroy(msg);
    }

//End of file
