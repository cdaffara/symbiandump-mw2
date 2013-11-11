/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: MPXMessageQueue testing implementation (mpxmessagequeue.h)
*
*/


#include <s32mem.h>

#include "commontestclass.h"

// Begin CMPXMessageQueue testing implementation (mpxmessagequeue.h)=======================================
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateCMPXMessageQueueL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreateCMPXMessageQueueL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXMessageQueueL testing CMPXMessageQueue::NewL begin")));
    iLog->Log(_L("CCommonTestClass::CreateCMPXMessageQueueL testing CMPXMessageQueue::NewL begin"));
    TInt err = KErrNone;
    if ( iMessageQueue != NULL)
        {
        delete iMessageQueue;
        }
    iMessageQueue = NULL;
    iMessageQueue = CMPXMessageQueue::NewL();
    if ( iMessageQueue == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreateCMPXMessageQueueL testing CMPXMessageQueue::NewL end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreateCMPXMessageQueueL testing CMPXMessageQueue::NewL end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::DeleteCMPXMessageQueue()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeleteCMPXMessageQueue()
    {
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXMessageQueue testing CMPXMessageQueue::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXMessageQueue testing CMPXMessageQueue::~ begin"));
    TInt err = KErrNone;
    delete iMessageQueue;
    iMessageQueue = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::DeleteCMPXMessageQueue testing CMPXMessageQueue::~ end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeleteCMPXMessageQueue testing CMPXMessageQueue::~ end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::ResetCMPXMessageQueue()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::ResetCMPXMessageQueue()
    {
	FTRACE(FPrint(_L("CCommonTestClass::ResetCMPXMessageQueue testing CMPXMessageQueue::Reset~ begin")));
    iLog->Log(_L("CCommonTestClass::ResetCMPXMessageQueue testing CMPXMessageQueue::Reset begin"));
    TInt err = KErrNone;
    iMessageQueue->Reset();
	FTRACE(FPrint(_L("CCommonTestClass::ResetCMPXMessageQueue testing CMPXMessageQueue::Reset end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::ResetCMPXMessageQueue testing CMPXMessageQueue::Reset end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::SendNextCMPXMessageQueue()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SendNextCMPXMessageQueue()
    {
	FTRACE(FPrint(_L("CCommonTestClass::SendNextCMPXMessageQueue testing CMPXMessageQueue::SendNext begin")));
    iLog->Log(_L("CCommonTestClass::SendNextCMPXMessageQueue testing CMPXMessageQueue::SendNext begin"));
    TInt err = KErrNone;
    RMessage2 message;
    iMessageQueue->SendNext(message);    
	FTRACE(FPrint(_L("CCommonTestClass::SendNextCMPXMessageQueue testing CMPXMessageQueue::SendNext end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::SendNextCMPXMessageQueue testing CMPXMessageQueue::SendNext end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::AddCMPXMessageQueue()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::AddCMPXMessageQueue()
    {
	FTRACE(FPrint(_L("CCommonTestClass::AddCMPXMessageQueue testing CMPXMessageQueue::Add begin")));
    iLog->Log(_L("CCommonTestClass::AddCMPXMessageQueue testing CMPXMessageQueue::Add begin"));
    TInt err = KErrNone;
    CMPXMessage *message( NULL );
    TRAP(err , message = CMPXMessage::NewL());
    iMessageQueue->Add(message, err);
    delete message;
	FTRACE(FPrint(_L("CCommonTestClass::AddCMPXMessageQueue testing CMPXMessageQueue::Add end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::AddCMPXMessageQueue testing CMPXMessageQueue::Add end err=%d"), err);
	return err;
    }


// end of CMPXMessageQueue (mpxmessagequeue.h) =============================================
