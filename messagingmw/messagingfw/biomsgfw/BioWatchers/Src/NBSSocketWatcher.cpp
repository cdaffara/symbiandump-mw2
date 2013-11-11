// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// NBSocketWatcher.cpp
// 
//

#include "NBSSocketWatcher.h"
#include "SmsSocketWatcher.h"

#include <smuthdr.h>
#include <msventry.h>
#include <msvstd.h>
#include <msvuids.h>
#include <smut.h>
#include <watcher.h>
#include <smsuaddr.h>
#include <gsmuelem.h>
#include <gsmunonieoperations.h>
#include <s32strm.h>
#include <s32mem.h>
#include <gsmubuf.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include <tmsvsmsentry.h>
#include "cwatcher.h"			
#endif

const TInt KSmsWRetryTimer = 1000000 * 15; // 15 Secs

/*
 * CNbsSocketWatcher
 */

CNbsSmsSocketWatcher::CNbsSmsSocketWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, const TDesC& aNbsPattern, TSmsAddrFamily aAddrFamily)
:	CBaseSmsActiveSocketWatcher(aWatcherLog, aPriority, aBioID, aFs),
	iAddrFamily(aAddrFamily),
	iNbsPattern(aNbsPattern)
	{
	}

CNbsSmsSocketWatcher::~CNbsSmsSocketWatcher()
	{
	Cancel();

	if (iSocket.SubSessionHandle())
		iSocket.Close();

	if (iSocketServer.Handle())
		iSocketServer.Close();
	
	//destroying class 0 SMS E-Com pluin implementation 
	if (iSmsClass0 != NULL)
		{
		REComSession::DestroyedImplementation(iDtor_ID_Key);
		REComSession::FinalClose();
		delete iSmsClass0;
		}
	RProperty::Delete(KUidSystemCategory, KUidPSDiskSpaceMonitorKeyType);
	}

CNbsSmsSocketWatcher* CNbsSmsSocketWatcher::NewLC(TMsvId aBioServiceId, 
									   			  TMsvId aSmsServiceId, 
									   			  CBIODatabase& aBioDb, 
									   			  CWatcherLog& aWatcherLog, 
									   			  TInt aPriority, 
									   			  TUid aBioID, 
									   			  RFs& aFs, 
									   			  const TDesC& aNbsPattern, 
									   			  TSmsAddrFamily aAddrFamily /*= ESmsAddrMatchText*/)
	{
	CNbsSmsSocketWatcher* self = new(ELeave) CNbsSmsSocketWatcher(aWatcherLog, aPriority, aBioID, aFs, aNbsPattern, aAddrFamily);
	CleanupStack::PushL(self);
	self->ConstructL(aBioServiceId, aSmsServiceId, aBioDb);
	return self;	
	}


void CNbsSmsSocketWatcher::ConstructL(TMsvId aBioServiceId, TMsvId aSmsServiceId, CBIODatabase& aBioDb)
	{
	CBaseSmsActiveSocketWatcher::ConstructL(aBioDb, aBioServiceId, aSmsServiceId);
	User::LeaveIfError(iSocketServer.Connect());
	TSecurityPolicy  readPolicy(ECapabilityReadDeviceData);
	TSecurityPolicy  writePolicy(ECapabilityWriteDeviceData);

 
	TInt ret = RProperty::Define(KUidSystemCategory, KUidPSDiskSpaceMonitorKeyType,
  	  	 				  RProperty::EInt, readPolicy, writePolicy);
 
	BIOWATCHERLOG(iWatcherLog.Printf(_L(" Define return value %d"), ret));
 
	if (ret != KErrNone  &&  ret != KErrAlreadyExists)
  		{
  		User::Leave(ret);
  		}   
  		 
	TInt setRet = RProperty::Set(KUidSystemCategory, KUidPSDiskSpaceMonitorKeyType, ESmsDiskSpaceAvailable);
 
	BIOWATCHERLOG(iWatcherLog.Printf(_L(" Set value %d"), setRet));
 
	if (setRet != KErrNone)
  		{
  		User::Leave(setRet);
  		}
	}

void CNbsSmsSocketWatcher::DoCancel()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: DoCancel")));
	if (iIoCtlActive)
		{
		iSocket.CancelIoctl();
		}

	if(iTimer.Handle() != NULL)
		iTimer.Cancel();

	Complete(KErrCancel);
	}

void CNbsSmsSocketWatcher::DoRunL()
	{
	iIoCtlActive = EFalse;
	switch(iState)
		{
		case ESmsWWaitForMsg:
			iState = ESmsWReadMsg;
			WaitForMessageL();
			break;
		
		case ESmsWReadMsg:
			iState = ESmsWWaitForMsg;
			ReceiveL();
			break;

		case ESmsWRetryError:
			if (!iTimer.Handle())
				{
				User::LeaveIfError(iTimer.CreateLocal());
				}
				
			iState=ESmsWWaitForMsg;
			iTimer.After(iStatus, KSmsWRetryTimer);
			break;
			
		case ESmsIsClass0Msg:
			iState = ESmsSupportClass0Msg;
			CheckForClass0SmsSupport();
			break;
		
		case ESmsSupportClass0Msg:
			iState = ESmsWWaitForMsg;
			SupportForClass0SmsL();
			break;
		}

	SetActive();

	}

void CNbsSmsSocketWatcher::DoSetupL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: DoSetupL: %S"), &iBioMsgText));

	// Set up the Socket to watch the port

	// test if it's there... 
	TProtocolDesc protoInfo;
	TProtocolName protocolname;

	BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioNbs: DoSetupL: Finding protocol")));

	protocolname.Copy(KSmsDatagram);
	User::LeaveIfError(iSocketServer.FindProtocol(protocolname,protoInfo));

	BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioNbs: DoSetupL: Opening socket")));

	// Create the RSocket
	User::LeaveIfError(iSocket.Open(iSocketServer,
										protoInfo.iAddrFamily,
										protoInfo.iSockType, 
										protoInfo.iProtocol));
	
	// Set up the Socket to watch the pattern
	if (iBioMsgUID != KNullUid && iAddrFamily == ESmsAddrMatchText)
	{
		// need an 8 bit pattern
		TBuf8<KMaxBioIdText> pattern; 
		pattern.Copy(iNbsPattern);

		iSmsAddr.SetSmsAddrFamily(ESmsAddrMatchText);
		iSmsAddr.SetTextMatch(pattern);
	}
	else
	{
		// Binding to a non-patter address family
		// like StatusReports, Special Messages, and RecieveAny
		iSmsAddr.SetSmsAddrFamily(iAddrFamily);
	}
	
	BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioNbs: DoSetupL: Binding socket")));

	User::LeaveIfError(iSocket.Bind(iSmsAddr));
	}

void CNbsSmsSocketWatcher::DoComplete(TInt& rStatus)
	{	
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: DoComplete: %S, status %d"), &iBioMsgText, rStatus));

	if (iSocket.SubSessionHandle())
		{
		iSocket.Close();
		}
	}

void CNbsSmsSocketWatcher::WaitForMessageL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: WaitForMessageL: %S"), &iBioMsgText));
	iOctlRes()= KSockSelectRead;
	iIoCtlActive = ETrue;
	iSocket.Ioctl(KIOctlSelect, iStatus, &iOctlRes, KSOLSocket);
	}
	
// CNbsSmsSocketWatcher::CheckForClass0SmsSupport to find out whether SMS stack is 
// configured for this new behaviour of class 0 message or not.
void CNbsSmsSocketWatcher::CheckForClass0SmsSupport()
	{
	// Need verify SMS stack configuration, if type of socket address is ESmsAddrRecvAny only
	if (iAddrFamily == ESmsAddrRecvAny)
		{
		BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: CheckForClass0SmsSupport")));
		iOctlRes()= KSockSelectRead;
		iIoCtlActive = ETrue;
		//iStatus will return KErrNone if stack supports new way of handling class 0 messages. 
		//Otherwise it will return KErrNotSupported.
		iSocket.Ioctl(KIoctlSupportOODClass0SmsMessages, iStatus, NULL, KSolSmsProv);
		}
	else
		{
		//Change state to Wait For Message
		iState = ESmsWWaitForMsg;
		//Complete self to enter state machine
		TRequestStatus* status = &iStatus;
		iStatus=KRequestPending;
		User::RequestComplete(status, KErrNone);
		}
	}

void CNbsSmsSocketWatcher::DoReceiveL()
 	{
 	// Get the Message
 	CSmsMessage* smsmessage = CSmsMessage::NewL(iFs, CSmsPDU::ESmsDeliver, CSmsBuffer::NewL());
 	CleanupStack::PushL(smsmessage);
 			
 	RSmsSocketReadStream readstream(iSocket);
 	readstream >> *smsmessage; //can leave
 			
 	CleanupStack::Pop(smsmessage);
 	
 	//get the class type here
	TSmsDataCodingScheme::TSmsClass classType(TSmsDataCodingScheme::ESmsClass2);
	const CSmsDeliver& deliver = static_cast<const CSmsDeliver&>(smsmessage->SmsPDU());
    TBool classDefined = deliver.Class(classType);
    
    //handling class 0 sms message 
 	if(iSmsClass0 != NULL && classType == TSmsDataCodingScheme::ESmsClass0)
 		{
 		//destroy the CSmsMessage, once passed succesfully to Class0SMS plugin
 		CleanupStack::PushL(smsmessage);
 		
		//Externalize CSmsMessage into a TDesC8
		CSmsBufferBase& convSmsMsg = smsmessage->Buffer();
		TInt len = convSmsMsg.Length() ;
	
		//for sending Incomplete MessageInfo to UI.
		TInt startPos = 0;
		TInt endPos = 0;
		TBool isLastMessage = EFalse;
		
		HBufC *buffer = HBufC::NewLC(len);
		TPtr temp = buffer->Des();
		convSmsMsg.Extract(temp, 0, len);
		
		//allocate memory for CBufFlat, length sholud be: length of Class0SMS data + 2*(size of TInt)+(size of TBool)
		CBufFlat* flat = CBufFlat::NewL(len + (3 * sizeof(TInt)));
		CleanupStack::PushL(flat);
		flat->ExpandL(0, len);
		
		RBufWriteStream writeStream(*flat);
		CleanupClosePushL (writeStream);
			
		if (smsmessage->IsComplete())
			{
			//if it's complete class 0 SMS message
			//passing startPos, endPos and isLastMessage value to 0
			ExternalizeL(writeStream, startPos, endPos, isLastMessage, temp);
			TPtr8 smsMsg(flat->Ptr(0));
			iSmsClass0->DisplayMessageHandler(smsMsg, ETrue);
			}
		else
			{
			//handling incomplete SMS Message
			CIncompleteClass0MessageInfo  &incompleteMessageInfo = (CIncompleteClass0MessageInfo&)smsmessage->GetOperationsForNonIEL(ESmsIncompleteClass0MessageParameter);
			TInt msgRefNo = smsmessage->SmsPDU().ConcatenatedMessageReference();
			
			//get the information about the messsage & externalize it..	
			incompleteMessageInfo.GetIncompleteMessageInfoL(startPos, endPos, isLastMessage);
			ExternalizeL(writeStream, startPos, endPos, isLastMessage, temp);
			TPtr8 smsMsg(flat->Ptr(0));
					
			if (iMsgRefNumber == -1 || iMsgRefNumber == msgRefNo)
				{
				//if it's 1st part of the Message, assign msgRefNo to iMsgRefNumber  
				if (iMsgRefNumber == -1)
					{
					iMsgRefNumber = msgRefNo;
					}
						
				iSmsClass0->DisplayMessageHandler(smsMsg, EFalse);
							
				//if it's last part of the Message, assign iMsgRefNumber to -1  							
				if (isLastMessage)
					{
					iMsgRefNumber = -1;	
					}
				}
			}
			//delete smsmessage, as owned by header
			CleanupStack::PopAndDestroy(4, smsmessage);
 		} //handling class 0 sms message
 	
 	// If the message is delivered to an unknown port (as opposed to no port at all)
 	// then we should only store the message if it is a "TextMessage".
 	// Otherwise the message should be silently consumed without being delivered.
 	// This is a requirement of the Java "Mobile Service Architecture", JSR 248, section 6.11.3.2 	
	
 	else if (	iAddrFamily == ESmsAddrRecvAny 			// << This watcher is listening on an unknown port
 			&& MessageHasPort(*smsmessage)			// << The message is addressed to a specific port
 			&& ! IsJavaTextMessage(*smsmessage) )	// << The message is defined by JSR 205 as NOT a TextMessage object
 		{
 		BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioNbs: Non-text message delivered to unknown port - deleting message")));
 		}
 	else
 		{
 		StoreMsgL(smsmessage, ETrue); // Store the messaage and check for SID entry for ownership
 		}
 	}

void CNbsSmsSocketWatcher::ReceiveL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: ReceiveL: %S"), &iBioMsgText));
	
	TInt error;
	
	switch (iStatus.Int())
		{
		case KErrNone:
			{
			TRAP(error, DoReceiveL());
			
			// Tell the socket if we handled the message or not
			if (error == KErrNone)
				{
				iIoCtlActive = ETrue;
				iSocket.Ioctl(KIoctlReadMessageSucceeded, iStatus, NULL, KSolSmsProv);
				}
			else
				{
				BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: Recv fail %d: %S"), error, &iBioMsgText));
 				iState = ESmsWRetryError;
				iIoCtlActive = ETrue;
				iSocket.Ioctl(KIoctlReadMessageFailed, iStatus, NULL, KSolSmsProv);
				}
			}
			break;
			
		case KIoctlSelectModemPresent:
			// Enumerate to see it there are any messages when the phone's hooked up
			// Only enumerate with default recievAny watcher
			// this pattern watcher should do nothing
			// SMS Prot should get the phone memory's SMS messages here
		case KIoctlSelectModemNotPresent:
			{
			// Phone's disconnected ... wait in Ioctl again
			// base class does this ... we're okay
			TRequestStatus* pS=&iStatus;
			User::RequestComplete(pS,0);
			}			
			break;

		default:
			User::Leave(iStatus.Int());
		}
	}

/**
Returns whether the message contains optional port addressing information.
@return ETrue if message contains port addressing information, otherwise EFalse.
*/
TBool CNbsSmsSocketWatcher::MessageHasPort(const CSmsMessage& aSmsMessage)
	{
	// Declare variables that will receive their values in ApplicationPortAddressing()
 	TInt destinationPort=0;
 	TInt originatorPort=0;
 	TBool is16bit=EFalse;
 	TBool hasPortAddressing = aSmsMessage.SmsPDU().ApplicationPortAddressing(destinationPort, originatorPort, &is16bit);
 	
 	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: MessageHasPort()"))); 	
 	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs:  > has:%d, dest:%d, orig:%d, 16bit:%d)"), hasPortAddressing, destinationPort, originatorPort, is16bit));
 	
 	return hasPortAddressing;
	}

/**
Check whether Discard Unknown Port message flag is set, if not check for Java wireless messaging.

Returns whether the message is considered a TextMessage, according to Java wireless messaging
specifications - JSR 205 "Wireless Messaging API" Appendix A.5.0 - which associates the TextMessage
object with the 7-bit and 16-bit UCS-2 alphabets, and BinaryMessage with the 8-bit alphabet.
@param aSmsMessage the message whose alphabet we are testing.
@return ETrue if the alphabet is 7-bit or 16-bit UCS-2, otherwise EFalse.
*/
TBool CNbsSmsSocketWatcher::IsJavaTextMessage(const CSmsMessage& aSmsMessage)
	{
	TBool isTextMessage = EFalse;
	if(!iSettings->DiscardUnknownPortMessage())
		{
		TSmsDataCodingScheme::TSmsAlphabet alphabet = aSmsMessage.SmsPDU().Alphabet();
 	
		if (alphabet == TSmsDataCodingScheme::ESmsAlphabet7Bit || alphabet == TSmsDataCodingScheme::ESmsAlphabetUCS2)
			{
			isTextMessage = ETrue;
			}

			BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: IsJavaTextMessage()")));
	 		BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs:  > isTextMessage:%d, alphabet:%d)"), isTextMessage, alphabet));
		}
	
	return isTextMessage;
	}	
	
/*
 *	CSpecialNbsSmsSocketWatcher Implementation
 */
 
CSpecialNbsSmsSocketWatcher* CSpecialNbsSmsSocketWatcher::NewLC(
															TMsvId 			aBioServiceId, 
															TMsvId 			aSmsServiceId, 
															CBIODatabase&	aBioDb, 
															CWatcherLog&	aWatcherLog,
															TInt			aPriority,
															TUid			aBioID,
															RFs&			aFs,
															const TDesC&	aNbsPattern, 
															TSmsAddrFamily	aAddrFamily)
	{
	CSpecialNbsSmsSocketWatcher* self = new (ELeave) CSpecialNbsSmsSocketWatcher(aWatcherLog, aPriority, aBioID, aFs, aNbsPattern, aAddrFamily);
	CleanupStack::PushL(self);
	self->ConstructL(aBioServiceId, aSmsServiceId, aBioDb);
	return self;
	}

CSpecialNbsSmsSocketWatcher* CSpecialNbsSmsSocketWatcher::NewL(
															TMsvId 			aBioServiceId, 
															TMsvId 			aSmsServiceId, 
															CBIODatabase&	aBioDb, 
															CWatcherLog&	aWatcherLog, 
															TInt			aPriority, 
															TUid			aBioID, 
															RFs&			aFs,
															const TDesC&	aNbsPattern, 
															TSmsAddrFamily	aAddrFamily)
	{
	CSpecialNbsSmsSocketWatcher* self = NewLC(aBioServiceId, aSmsServiceId, aBioDb, aWatcherLog, aPriority, aBioID, aFs, aNbsPattern, aAddrFamily);
	CleanupStack::Pop(self);
	return self;
	}

CSpecialNbsSmsSocketWatcher::CSpecialNbsSmsSocketWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, const TBioMsgIdText& aNbsPattern, TSmsAddrFamily aAddrFamily)
: CNbsSmsSocketWatcher(aWatcherLog, aPriority, aBioID, aFs, aNbsPattern, aAddrFamily)
	{
	}

void CSpecialNbsSmsSocketWatcher::RestoreSettingsL(CMsvSession& aSession)
	{
	CBaseSmsActiveSocketWatcher::RestoreSettingsL(aSession);

	switch(iAddrFamily)
		{
	case ESmsAddrMessageIndication:
		iReportHandling = iSettings->SpecialMessageHandling();
		break;
	case ESmsAddrStatusReport:
		iReportHandling = iSettings->StatusReportHandling();
		break;
	default:
		PanicWatcher(EAddrFamilyNotSupported1);
		break;
		}
	}
	
void CSpecialNbsSmsSocketWatcher::RestoreSettingsL(TMsvId aBioServiceId, TMsvId aSmsServiceId)
	{
	CBaseSmsActiveSocketWatcher::RestoreSettingsL(aBioServiceId, aSmsServiceId);
	
	switch(iAddrFamily)
		{
	case ESmsAddrMessageIndication:
		iReportHandling = iSettings->SpecialMessageHandling();
		break;
	case ESmsAddrStatusReport:
		iReportHandling = iSettings->StatusReportHandling();
		break;
	default:
		PanicWatcher(EAddrFamilyNotSupported2);
		break;
		}
	}

void CSpecialNbsSmsSocketWatcher::ConstructL(TMsvId aBioServiceId, TMsvId aSmsServiceId, CBIODatabase& aBioDb)
	{
	CNbsSmsSocketWatcher::ConstructL(aBioServiceId, aSmsServiceId, aBioDb);

	if (iReportHandling == CSmsSettings::EDoNotWatchForReport)
		{
		User::Leave(KErrNotSupported);
		}
	}
	

CSpecialNbsSmsSocketWatcher::~CSpecialNbsSmsSocketWatcher()
	{
	Cancel();
	}

void CSpecialNbsSmsSocketWatcher::SetBioMsgText(CBIODatabase& /*aBioDb*/)
	{
	switch(iAddrFamily)
		{
	case ESmsAddrMessageIndication:
		iBioMsgText = _L("Special Messages");
		break;
	case ESmsAddrStatusReport:
		iBioMsgText = _L("Status Report");
		break;
	default:
		PanicWatcher(EAddrFamilyNotSupported3);
		break;
		}
	}

void CSpecialNbsSmsSocketWatcher::PreStoreActionL(CMsvSession& aSession, CSmsMessage& aSmsMessage)
	{
	if( iAddrFamily == ESmsAddrStatusReport && 
		(iReportHandling == CSmsSettings::EMoveReportToInboxInvisibleAndMatch ||
		iReportHandling == CSmsSettings::EMoveReportToInboxVisibleAndMatch ||
		iReportHandling == CSmsSettings::EDiscardReportAndMatch) )
		{
		// This is a Status Report and the matching has been requested - so
		// do the matching!
		TLogId logId = aSmsMessage.LogServerId();
		CSmsStatusReport& report = static_cast<CSmsStatusReport&>(aSmsMessage.SmsPDU());
		
		MatchStatusReportL(aSession, logId, report.Status());
		}	
	}
	
void CSpecialNbsSmsSocketWatcher::MatchStatusReportL(CMsvSession& aSession, TLogId aLogId, TSmsStatus::TSmsStatusValue aDeliveryStatus)
	{
	// Need to check all SMS messages in the sent folder.
	CMsvEntry* msvEntry = aSession.GetEntryL(KMsvSentEntryId);
	CleanupStack::PushL(msvEntry);
	
	CMsvEntrySelection* selSent = msvEntry->ChildrenWithMtmL(KUidMsgTypeSMS);
	CleanupStack::PushL(selSent);
	
	// Also need to check the Outbox - if the message failed to send at least
	// one of its recipients then it will still be in the Outbox.
	msvEntry->SetEntryL(KMsvGlobalOutBoxIndexEntryId);

	CMsvEntrySelection* selOut = msvEntry->ChildrenWithMtmL(KUidMsgTypeSMS);
	CleanupStack::PushL(selOut);
	
	// Create a header object here for efficiency
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* richText = CRichText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(richText);

	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *richText);
	CleanupStack::PushL(header);
	
	TMsvId messageId;
	TBool found = SearchForMessageL(*msvEntry, *header, *selSent, aLogId, messageId);
	
	if( !found )
		{
		// Search the outbox just in case it is still there.
		found = SearchForMessageL(*msvEntry, *header, *selOut, aLogId, messageId);
		}
	
	if( found )
		{
		UpdateMessageEntryL(*msvEntry, *header, messageId, aLogId, aDeliveryStatus);
		}

	CleanupStack::PopAndDestroy(7, msvEntry);
	}

TBool CSpecialNbsSmsSocketWatcher::SearchForMessageL(CMsvEntry& aMsvEntry, CSmsHeader& aHeader, const CMsvEntrySelection& aSelection, TLogId aLogId, TMsvId& aMessageId)
	{
	TInt count = aSelection.Count();

	if( count == 0 )
		{
		// Empty selection - cannot do search.
		return EFalse;;
		}
		
	TBool found = EFalse;
	while( !found && count-- > 0 )
		{
		aMsvEntry.SetEntryL(aSelection.At(count));
		TMsvSmsEntry entry = aMsvEntry.Entry();
		
		if( entry.AckSummary(ESmsAckTypeDelivery) == TMsvSmsEntry::EPendingAcks )
			{
			// This message is waiting for delivery report - is this it?
			TLogId logId = 0;	
			TBool validId = entry.MessageId(logId);
			
			if( validId )
				{
				// Check the logId is the one
				if( aLogId == logId )
					{
					found = ETrue;
					aMessageId = entry.Id();
					}
				}
			else
				{
				// The long way round - restore the entry and search the list of 
				// recipients.
				CMsvStore* store = aMsvEntry.ReadStoreL();
				CleanupStack::PushL(store);
				aHeader.RestoreL(*store);	// no need to restore message body
				CleanupStack::PopAndDestroy(store);
				
				TInt rcptCount = aHeader.Recipients().Count();
				
				while( !found && rcptCount-- > 0 )
					{
					CSmsNumber& rcpt = *aHeader.Recipients()[rcptCount];
					
					if( rcpt.AckStatus(ESmsAckTypeDelivery) == CSmsNumber::EPendingAck &&
						rcpt.LogId() == aLogId )
						{
						found = ETrue;
						aMessageId = entry.Id();
						}					
					}				
				}
			}
		}
	return found;
	}
	
void CSpecialNbsSmsSocketWatcher::UpdateMessageEntryL(CMsvEntry& aMsvEntry, CSmsHeader& aHeader, TMsvId aMessageId, TLogId aLogId, TSmsStatus::TSmsStatusValue aDeliveryStatus)
	{
	// Set the entry - probably not necessary...
	aMsvEntry.SetEntryL(aMessageId);
	
	// Update the message entry.
	CMsvStore* store = aMsvEntry.EditStoreL();
	CleanupStack::PushL(store);
	aHeader.RestoreL(*store);	// no need to restore message body
	
	const TInt rcptCount = aHeader.Recipients().Count();
	TInt pending = 0;
	TInt fail = 0;
	TInt last = 0;
	TInt sent = 0;
	
	for( TInt count = 0; count < rcptCount; ++count )
		{
		CSmsNumber& rcpt = *aHeader.Recipients()[count];
		
		if( rcpt.Status() == CMsvRecipient::ESentSuccessfully )
			{
			++sent;
		
			CSmsNumber::TSmsAckStatus status = rcpt.AckStatus(ESmsAckTypeDelivery);
			if( status == CSmsNumber::EPendingAck )
				{
				if(	rcpt.LogId() == aLogId )
					{
					// Update the recipient with the delivery status.
					if( aDeliveryStatus == TSmsStatus::ESmsShortMessageReceivedBySME )
						rcpt.SetAckStatus(ESmsAckTypeDelivery, CSmsNumber::EAckSuccessful);
					else
						{
						// Recipient failed to get the message :(							
						rcpt.SetAckStatus(ESmsAckTypeDelivery, CSmsNumber::EAckError);
						rcpt.SetStatus(CMsvRecipient::EFailedToSend);
						rcpt.SetError(aDeliveryStatus);
						++fail;
						}
					}
				else
					{
					// This message has recipients waiting for status reports...
					++pending;
					last = count;	// index to last pending recipient.
					}
				}
			else if( status == CSmsNumber::EAckError )
				{
				// There is a recipient that failed to get the message
				++fail;				
				}
			}
		}
		
	// Store the updated recipient info
	aHeader.StoreL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(store);
	
	// Update the delivery summary info in the index...
	// 1. EPendingAcks - there are still recipients waiting for status reports.
	// 2. EAllSuccessful - all recipients were delivered to successfully.
	// 3. EAllFailed - all recipients failed to be delivered to.
	// 4. EMixed - a mixture of successful and failed deliveries to the recipients.
	TMsvSmsEntry entry = aMsvEntry.Entry();
	if( pending == 0 )
		{
		// Cases 2, 3 or 4 - check how many failed.
		if( fail == 0 )
			{
			// Case 2 - EAllSuccessful.
			entry.SetAckSummary(ESmsAckTypeDelivery, TMsvSmsEntry::EAllSuccessful);
			}
		else if( fail < sent )
			{
			// Case 4 - EMixed.
			entry.SetAckSummary(ESmsAckTypeDelivery, TMsvSmsEntry::EMixed);
			}
		else
			{
			// Case 3 - EAllFailed.
			entry.SetAckSummary(ESmsAckTypeDelivery, TMsvSmsEntry::EAllFailed);
			}
		}
	else
		{
		// Case 1 - EPendingAcks.
		entry.SetAckSummary(ESmsAckTypeDelivery, TMsvSmsEntry::EPendingAcks);
		
		if( (entry.iBioType == 0) && (pending == 1) )
			{
			// This message has only one pending recipient - update the entry to
			// reflect this.
			TLogId logId = aHeader.Recipients()[last]->LogId();
			entry.SetMessageId(logId, ETrue);
			}
		}
	aMsvEntry.ChangeL(entry);
	}

//SupportForClass0SmsL for launching the Class 0 SMS plugin, If Class 0 SMS support enabled from SMS Stack.
void CNbsSmsSocketWatcher::SupportForClass0SmsL()
	{
	//if iStatus is KErrNotSupported, class 0 messages will be handled as normal Class2 messages
	if( iStatus.Int() == KErrNone )
		{
		iMsgRefNumber = -1;
		
		//launch class 0 sms plugin 
		iSmsClass0 = reinterpret_cast<CSmsClass0Base*>	
								(REComSession::CreateImplementationL(
														KUidClass0SmsHandler,
														_FOFF(CNbsSmsSocketWatcher ,iDtor_ID_Key))); 
		
		}
		
		// Complete self to enter state machine
		TRequestStatus* status = &iStatus;
		iStatus=KRequestPending;
		User::RequestComplete(status, KErrNone);
	}	


//externalize  start position, end position, isLastMessage flag and Class 0 SMS data,
void CNbsSmsSocketWatcher::ExternalizeL(RBufWriteStream& aWriteStream, TInt aStartPos, TInt endPos, TBool aIsLastMessage, TPtr aData)
	{
	aWriteStream.WriteInt32L(aStartPos);
	aWriteStream.WriteInt32L(endPos);
	aWriteStream.WriteInt32L(aIsLastMessage);
	aWriteStream << aData;
	}
