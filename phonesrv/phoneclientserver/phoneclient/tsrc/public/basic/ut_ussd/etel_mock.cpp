

#include "ut_testenv.h"
#include <etel.h>
#include <etelmm.h>
#include <etelmmcs.h>
#include <QTest>


// global variables
CPeriodic* messageResponseTimer = 0;

// ==============================================================================
// RMobileUssdMessaging
// ==============================================================================

RMobileUssdMessaging::RMobileUssdMessaging()
{
}

RMobileUssdMessaging::RMobileUssdMessaging(const RMobileUssdMessaging&)
{
}

void RMobileUssdMessaging::ConstructL()
{
	QVERIFY(!messageResponseTimer);
    messageResponseTimer =
		CPeriodic::NewL(CActive::EPriorityStandard+1);
}

TInt RMobileUssdMessaging::Open(RMobilePhone& /*aPhone*/)
{
	TRAPD(err,ConstructL());
    return err;
}

void RMobileUssdMessaging::Close()
{
	delete messageResponseTimer;
	messageResponseTimer = 0;
}

void RMobileUssdMessaging::Destruct()
{
}

TInt RMobileUssdMessaging::GetCaps(TDes8& aCaps) const
{
    TMobileUssdCapsV1 caps;
    caps.iUssdFormat = KCapsPackedString;
    caps.iUssdTypes = KCapsMTUssd | KCapsMOUssd;
    TMobileUssdCapsV1Pckg pckgCaps( caps );
    aCaps.Copy( pckgCaps );
    return KErrNone;
}

void RMobileUssdMessaging::ReceiveMessage(
    TRequestStatus& aReqStatus, TDes8& aMsgData,
    TDes8& aMsgAttributes) const
{
	EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
    QVERIFY2(!loop.mReceiveMessageReqStatus,
    		"etel_mock: ReceiveMessage called twice!");
    aReqStatus = KRequestPending;
    loop.mReceiveMessageReqStatus = &aReqStatus;
	loop.mReceiveMessageData = &aMsgData;
	loop.mReceiveMessageAttributes = &aMsgAttributes;
}

TInt SendMessageCallback(TAny* /*aPtr*/)
	{
	messageResponseTimer->Cancel();

	EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
    const TestMode& mode = TestEnv::env().testMode();
    if (mode.m_event == TestMode::Normal)
    	{
    	User::RequestComplete(loop.mSendMessageReqStatus, mode.m_error);
    	}
    return 0;
	}

void RMobileUssdMessaging::SendMessage(
    TRequestStatus& aReqStatus, const TDesC8& /*aMsgData*/,
    const TDesC8& /*aMsgAttributes*/) const
{
	EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
    QVERIFY2(!loop.mSendMessageReqStatus,
    		"etel_mock: SendMessage called twice!");
    aReqStatus = KRequestPending;
    loop.mSendMessageReqStatus = &aReqStatus;

    const TestMode& mode = TestEnv::env().testMode();
    if (mode.m_event != TestMode::NoResp)
    	{
		// start timer to respond
		TTimeIntervalMicroSeconds32 delay(
				1000*mode.m_timeout);
		messageResponseTimer->Start(delay,delay,
				TCallBack(SendMessageCallback,(TAny*)this));
    	}
}

void RMobileUssdMessaging::SendMessageNoFdnCheck(
    TRequestStatus& /*aReqStatus*/, const TDesC8& /*aMsgData*/,
    const TDesC8& /*aMsgAttributes*/) const
{
	// not used
}

void RMobileUssdMessaging::SendRelease(
    TRequestStatus& /*aReqStatus*/, TDes8& /*aReturnResult*/) const
{
	// not used
}

void RMobileUssdMessaging::NotifyNetworkRelease(
    TRequestStatus& aReqStatus, TDes8& aReturnResult) const
{
	EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
    QVERIFY2(!loop.mNetworkReleaseReqStatus,
    		"etel_mock: NotifyNetworkRelease called twice!");
    aReqStatus = KRequestPending;
    loop.mNetworkReleaseReqStatus = &aReqStatus;
	loop.mNetworkReleaseMessageData = &aReturnResult;
	loop.mNetworkReleaseMessageAttributes = 0;
}

void RMobileUssdMessaging::NotifyNetworkRelease(
    TRequestStatus& aReqStatus, TDes8& aMsgData,
    TDes8& aMsgAttributes) const
{
	EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
    QVERIFY2(!loop.mNetworkReleaseReqStatus,
    		"etel_mock: NotifyNetworkRelease called twice!");
    aReqStatus = KRequestPending;
    loop.mNetworkReleaseReqStatus = &aReqStatus;
	loop.mNetworkReleaseMessageData = &aMsgData;
	loop.mNetworkReleaseMessageAttributes = &aMsgAttributes;
}


RMobileUssdMessaging::TMobileUssdAttributesV1::TMobileUssdAttributesV1()
{
    iFlags = 0;
    iFormat = EFormatUnspecified;
    iType = EUssdUnknown;
    iDcs = 0;
}

RMobileUssdMessaging::TMobileUssdCapsV1::TMobileUssdCapsV1()
{
    iUssdFormat = 0;
    iUssdTypes = 0;
}

// ==============================================================================
// RMobilePhone
// ==============================================================================

RMobilePhone::TMobilePhoneSendSSRequestV3::TMobilePhoneSendSSRequestV3()
{
    iOpCode = 0;
}

RMobilePhone::TMultimodeType::TMultimodeType()
{
    iExtensionId = 0;
}

// ==============================================================================
// RTelSubSessionBase
// ==============================================================================

RTelSubSessionBase::RTelSubSessionBase()
{
}

void RTelSubSessionBase::CancelAsyncRequest(TInt aReqToCancel) const
{
	if ( aReqToCancel == EMobileUssdMessagingSendMessage )
	{
		EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
		if ( loop.mSendMessageReqStatus )
		{
		User::RequestComplete( loop.mSendMessageReqStatus, KErrCancel );
		}
	}
	else if ( aReqToCancel == EMobileUssdMessagingReceiveMessage )
	{
		EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
		if ( loop.mReceiveMessageReqStatus )
    	{
        User::RequestComplete( loop.mReceiveMessageReqStatus, KErrCancel );
    	}
	}
	else if ( aReqToCancel == EMobileUssdMessagingNotifyNetworkRelease )
	{
		EtelMsgLoop& loop = TestEnv::env().etelMsgLoop();
		if ( loop.mNetworkReleaseReqStatus )
    	{
        User::RequestComplete( loop.mNetworkReleaseReqStatus, KErrCancel );
    	}
	}
	else if ( aReqToCancel == EMobileUssdMessagingSendRelease )
	{
	// not used
	}
}

