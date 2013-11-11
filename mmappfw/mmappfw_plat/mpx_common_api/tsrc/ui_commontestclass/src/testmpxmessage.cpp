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
* Description:  MPXMessage testing implementation (mpxmessage.h & mpxplaybackmessage.h)
*
*/


#include <s32mem.h>

#include <mpxmessagegeneraldefs.h>
#include <mpxplaybackmessagedefs.h>

#include "commontestclass.h"

// Begin TMPXMessage testing implementation (mpxmessage.h & mpxplaybackmessage.h)
// -----------------------------------------------------------------------------
// CCommonTestClass::TMPXMessage3L()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::TMPXMessage3L()
    {
	FTRACE(FPrint(_L("CCommonTestClass::TMPXMessage3L testing TMPXMessage::TMPXMessage(...) begin")));
    iLog->Log(_L("CCommonTestClass::TMPXMessage3L testing TMPXMessage::TMPXMessage(...) begin"));
    TInt err = KErrNone;

    TMPXPlaybackMessage message;

    message = TMPXPlaybackMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);


	FTRACE(FPrint(_L("CCommonTestClass::TMPXMessage3L testing TMPXMessage::TMPXMessage(...) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::TMPXMessage3L testing TMPXMessage::TMPXMessage(...) end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::TMPXMessage1L()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::TMPXMessage1L()
    {
	FTRACE(FPrint(_L("CCommonTestClass::TMPXMessage1L testing TMPXMessage::TMPXMessage(..) begin")));
    iLog->Log(_L("CCommonTestClass::TMPXMessage1L testing TMPXMessage::TMPXMessage() begin"));
    TInt err = KErrNone;

    TMPXMessage message;

    message = TMPXPlaybackMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);

    TMPXMessage msg = TMPXMessage(message);

    TMPXPlaybackMessage message2;
    message = TMPXPlaybackMessage(message2);

	FTRACE(FPrint(_L("CCommonTestClass::TMPXMessage1L testing TMPXMessage::TMPXMessage(..) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::TMPXMessage1L testing TMPXMessage::TMPXMessage() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::TMPXMessageL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::TMPXMessageL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::TMPXMessageL testing TMPXMessage::TMPXMessage() begin")));
    iLog->Log(_L("CCommonTestClass::TMPXMessageL testing TMPXMessage::TMPXMessage() begin"));
    TInt err = KErrNone;

    TMPXMessage message;

    message = TMPXMessage();

	FTRACE(FPrint(_L("CCommonTestClass::TMPXMessageL testing TMPXMessage::TMPXMessage() end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::TMPXMessageL testing TMPXMessage::TMPXMessage() end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::EqualTMPXMessage()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::EqualTMPXMessage()
    {
	FTRACE(FPrint(_L("CCommonTestClass::EqualTMPXMessage testing TMPXMessage::operator=(..) begin")));
    iLog->Log(_L("CCommonTestClass::EqualTMPXMessage testing TMPXMessage::operator=() begin"));
    TInt err = KErrNone;

    TMPXMessage message;

    message = TMPXPlaybackMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);

    TMPXMessage msg = TMPXMessage(message);
    msg = message;
    message = msg;

	FTRACE(FPrint(_L("CCommonTestClass::EqualTMPXMessage testing TMPXMessage::operator=(..) end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::EqualTMPXMessage testing TMPXMessage::operator=() end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::EqualTMPXMessage()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Event()
    {
	FTRACE(FPrint(_L("CCommonTestClass::Event testing TMPXMessage::Event begin")));
    iLog->Log(_L("CCommonTestClass::Event testing TMPXMessage::Event begin"));
    TInt err = KErrNone;

    TMPXMessage message = TMPXMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);

    TInt event = message.Event();

	FTRACE(FPrint(_L("CCommonTestClass::Event testing TMPXMessage::Event end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::Event testing TMPXMessage::Event end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::Type()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Type()
    {
	FTRACE(FPrint(_L("CCommonTestClass::Type testing TMPXMessage::Type begin")));
    iLog->Log(_L("CCommonTestClass::Type testing TMPXMessage::Type begin"));
    TInt err = KErrNone;

    TMPXMessage message = TMPXMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);

    TInt type = message.Type();

	FTRACE(FPrint(_L("CCommonTestClass::Type testing TMPXMessage::Type end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::Type testing TMPXMessage::Type end err=%d"), err);
	return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::EqualTMPXMessage()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::Data()
    {
	FTRACE(FPrint(_L("CCommonTestClass::Data testing TMPXMessage::Data begin")));
    iLog->Log(_L("CCommonTestClass::Data testing TMPXMessage::Data begin"));
    TInt err = KErrNone;

    TMPXMessage message = TMPXMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);

    TInt data = message.Data();

	FTRACE(FPrint(_L("CCommonTestClass::Data testing TMPXMessage::Data end err=%d"), err));
    iLog->Log(_L("CCommonTestClass::Data testing TMPXMessage::Data end err=%d"), err);
	return err;
    }
