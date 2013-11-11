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
* Description:  Queue the message
*
*/

#include <mpxlog.h>
#ifdef _DEBUG
#include <mpxmessagegeneraldefs.h>
#endif
#include "mpxmessagequeue.h"

// ============================ PRIVATE CLASSES ==============================
NONSHARABLE_CLASS(CMPXMessageItem) : public CBase
    {
public:
    static CMPXMessageItem* NewL(const CMPXMessage* aMsg, TInt aError);
    ~CMPXMessageItem();
    /**
     *  message object
     *
     *  @return message object
     */
    CMPXMessage* Message()
        {
        return iMsg;
        }
    /**
     *  Error code associated with the message
     *
     *  @return the error code
     */
    TInt Error()
        {
        return iError;
        }
    /**
     *  Offset to the task queue
     *
     *  @return the offset
     */
     static TInt Offset()
         {
         return _FOFF(CMPXMessageItem,iLink);
         }
private:
    CMPXMessageItem(TInt aError);
    void ConstructL(const CMPXMessage* aMsg);
private:
    TSglQueLink iLink;
    CMPXMessage* iMsg;
    TInt iError;
    };

CMPXMessageItem* CMPXMessageItem::NewL(const CMPXMessage* aMsg, TInt aError)
    {
    CMPXMessageItem* self = new ( ELeave ) CMPXMessageItem(aError);
    CleanupStack::PushL( self );
    self->ConstructL(aMsg);
    CleanupStack::Pop(self);
    return self;
    }

CMPXMessageItem::CMPXMessageItem(TInt aError)
:   iError(aError)
    {
    }

CMPXMessageItem::~CMPXMessageItem()
    {
    MPX_DEBUG2("-->CMPXMessageItem::~CMPXMessageItem 0x%08x", this);
    if (iMsg)
        {
        MPX_DEBUG3("CMPXMessageItem::~CMPXMessageItem msg 0x%08x, msgHandle 0x%08x",
                   iMsg, iMsg->Data());

        delete iMsg;
        iMsg = NULL;
        }
    MPX_DEBUG2("<--CMPXMessageItem::~CMPXMessageItem 0x%08x", this);
    }

void CMPXMessageItem::ConstructL(const CMPXMessage* aMsg)
    {
    MPX_FUNC_EX("CMPXMessageItem::ConstructL");
    iMsg = aMsg ? CMPXMessage::NewL(*aMsg) : NULL;
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXMessageQueue* CMPXMessageQueue::NewL()
    {
    CMPXMessageQueue* self = new ( ELeave ) CMPXMessageQueue();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// C++ constructor.
// ----------------------------------------------------------------------------
//
CMPXMessageQueue::CMPXMessageQueue()
:   iMsgs(CMPXMessageItem::Offset()),
    iFailure(KErrNone)
    {
    }

// ----------------------------------------------------------------------------
// Second-phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXMessageQueue::ConstructL()
    {
    MPX_FUNC_EX("CMPXMessageQueue::ConstructL");
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXMessageQueue::~CMPXMessageQueue()
    {
    MPX_FUNC_EX("CMPXMessageQueue::~CMPXMessageQueue()");
    Reset();
    }

// ----------------------------------------------------------------------------
// Send next message request from client
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXMessageQueue::SendNext(const RMessage2& aMsgRequest)
    {
    MPX_FUNC_EX("CMPXMessageQueue::SendNext()");
    MPX_ASSERT(iMsgRequest.IsNull());
    iMsgRequest = aMsgRequest;
    if (!iMsgs.IsEmpty())
        {
        Send();
        }
    }

// ----------------------------------------------------------------------------
// Adds a message into the queue and sends the message if slot is available
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXMessageQueue::Add(const CMPXMessage* aMessage, TInt aError)
    {
    // aMessage should not be NULL and aError is KErrNone
    MPX_ASSERT(aMessage || aError);
    MPX_FUNC_EX("CMPXMessageQueue::Add()");
    CMPXMessageItem* item(NULL);
    TRAPD(err, item = CMPXMessageItem::NewL(aMessage, aError));
    if (KErrNone==err)
        {
        iMsgs.AddLast(*item);
#ifdef _DEBUG
        MPX_DEBUG3("CMPXMessageQueue::Add 0x%08x, items %d", this, ++iCount);
#endif
        }//else failed to create message item.
    else
        {
        MPX_DEBUG2("CMPXMessageQueue::Add Failed to create message item %d", err);
        }

    if (KErrNone!=err && KErrNone==iFailure)
        { // Set the failure code
        iFailure = err;
        }

    if (!iMsgRequest.IsNull())
        { // outstanding request
        Send();
        } // else client has not finished current message yet
    }

// ----------------------------------------------------------------------------
// Reset message queue
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXMessageQueue::Reset()
    {
    MPX_FUNC_EX("CMPXMessageQueue::Reset()");
    if (!iMsgRequest.IsNull())
        {
        iMsgRequest.Complete(KErrCancel);
        }
    delete iMsgSent;
    iMsgSent = NULL;
    TSglQueIter<CMPXMessageItem> iter(iMsgs);
    CMPXMessageItem* msgItem=NULL;
    while ((msgItem=iter++) != NULL) // Compiler on warning while(msgItem==iter++)
        {
        delete msgItem;
        }
    iMsgs.Reset();
    }

// ----------------------------------------------------------------------------
// Send a message
// ----------------------------------------------------------------------------
//
void CMPXMessageQueue::Send()
    {
    MPX_DEBUG3("-->CMPXMessageQueue::Send() 0x%08x, iMsgSent 0x%08x",
              this, iMsgSent);
    delete iMsgSent;
    iMsgSent = NULL;
    TInt data(0);
    TInt err(KErrNone);
    if (iFailure)
        {
        err=iFailure;
        iFailure = KErrNone; // Reset failure code
        }
    else
        {
        MPX_ASSERT(!iMsgs.IsEmpty());
        iMsgSent = iMsgs.First();
        iMsgs.Remove(*iMsgSent);
#ifdef _DEBUG
        MPX_DEBUG4("CMPXMessageQueue::Send 0x%08x, msg 0x%08x, items %d",
                    this, iMsgSent->Message(), --iCount);
#endif
        if (iMsgSent->Message())
            {
            data = iMsgSent->Message()->Data();
            MPX_ASSERT(data>0);
#ifdef _DEBUG
            CMPXMessage* msg = iMsgSent->Message();
            TMPXMessageId* pId = msg->Value<TMPXMessageId>(KMPXMessageGeneralId);
            if (pId)
                {
                TInt id = static_cast<TInt>(*pId);
                MPX_DEBUG5("CMPXMessageQueue::Send msg item 0x%08x, msg 0x%08x, msgHandle 0x%08x, msgId 0x%08x",
                        iMsgSent, iMsgSent->Message(), data, id);
                if (id==KMPXMessageGeneral)
                    {
                    MPX_ASSERT(msg->IsSupported(KMPXMessageGeneralEvent));
                    TInt* pEvent = msg->Value<TInt>(KMPXMessageGeneralEvent);
                    MPX_ASSERT(msg->IsSupported(KMPXMessageGeneralType));
                    TInt* pType = msg->Value<TInt>(KMPXMessageGeneralType);
                    MPX_ASSERT(msg->IsSupported(KMPXMessageGeneralData));
                    TInt* pData = msg->Value<TInt>(KMPXMessageGeneralData);
                    if (pEvent && pType && pData)
                        {
                        MPX_DEBUG5("CMPXMessageQueue::Send general msg item 0x%08x, event %d, type %d, data %d",
                                    iMsgSent, *pEvent, *pType, *pData);
                        }
                    else
                        {
                        MPX_DEBUG1("CMPXMessageQueue::Send OOM");
                        }
                    }
                }
            else
                {
                MPX_DEBUG1("CMPXMessageQueue::Send OOM");
                }
#endif
            } // else NULL message
        else // else NULL message
            {
            MPX_ASSERT(iMsgSent->Error());
            MPX_DEBUG3("CMPXMessageQueue::Send NULL message 0x%08x, err %d",
                       this, iMsgSent->Error());
            }
        err = iMsgSent->Error();
        }

    TPckgC<TInt> dataPkg(data);
    TPckgC<TInt> errPkg(err);
    TInt ret = (iMsgRequest.Write(0, dataPkg));
    if (!ret)
        {
        ret = iMsgRequest.Write(1, errPkg);
        }
#ifdef _DEBUG
    if (ret)
        {
        MPX_DEBUG1("CMPXMessageQueue::Send Failed to write data");
        }
#endif
    iMsgRequest.Complete(ret);
    MPX_DEBUG5("<--CMPXMessageQueue::Send() 0x%08x, sentMsgHandle 0x%08x, sentError %d,  sentRet %d",
               this, data, err, ret);
    }

// ----------------------------------------------------------------------------
// Adds a message into the top of the queue and sends the message if slot is available
// ----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXMessageQueue::AddFirst(const CMPXMessage* aMessage, TInt aError)
    {
    // aMessage should not be NULL and aError is KErrNone
    MPX_ASSERT(aMessage || aError);
    MPX_FUNC_EX("CMPXMessageQueue::AddFirst()");
    CMPXMessageItem* item(NULL);
    TRAPD(err, item = CMPXMessageItem::NewL(aMessage, aError));
    if (KErrNone==err)
        {
        iMsgs.AddFirst(*item);
#ifdef _DEBUG
        MPX_DEBUG3("CMPXMessageQueue::AddFirst 0x%08x, items %d", this, ++iCount);
#endif
        }//else failed to create message item.
    else
        {
        MPX_DEBUG2("CMPXMessageQueue::AddFirst Failed to create message item %d", err);
        }
    if (KErrNone!=err && KErrNone==iFailure)
        { // Set the failure code
        iFailure = err;
        }
    if (!iMsgRequest.IsNull())
        { // outstanding request
        Send();
        } // else client has not finished current message yet
    return err;
    }
// End of file
