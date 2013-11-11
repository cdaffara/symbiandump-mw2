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
//

#include "SmsSocketWatcher.h"
#include <e32std.h>
#include <bsp.h>
#include <biouids.h>
#include <biodb.h>
#include <msvstd.h>
#include <msventry.h>
#include <smut.h>
#include <smuthdr.h>
#include <msvuids.h>
#include <watcher.h>
#include <smutset.h>
#include <csmsemailfields.h>
#include <csmsaccount.h>
#include <e32property.h>
#include <smsuaddr.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include <bifchangeobserver.h>
#include <biomessageuids.h>
#include <tmsvsmsentry.h>
#include "cwatcher.h"
#include "tmsvbioinfo.h"
#endif

const TInt KSmsThresholdDiskSpaceValue = 125000; // 125 KB

/*
 *	CBaseSmsActiveSocketWatcher
*/

EXPORT_C CBaseSmsActiveSocketWatcher::CBaseSmsActiveSocketWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs)
:	CActive(aPriority), 
	iState(ESmsIsClass0Msg),
	iBioMsgUID(aBioID),
	iBioServiceId(KMsvLocalServiceIndexEntryId),
	iSmsServiceId(KMsvLocalServiceIndexEntryId),
	iWatcherLog(aWatcherLog),
	iFs(aFs)
	{
	// Add it to the scheduler
	CActiveScheduler::Add(this);
	}

EXPORT_C CBaseSmsActiveSocketWatcher::~CBaseSmsActiveSocketWatcher() 
	{
	iTimer.Close();
	delete iSettings;
	delete iGetDetDesc;
	REComSession::FinalClose();
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::Complete(TInt aStatus)
	{
 	DoComplete(aStatus);
	WatcherComplete(aStatus);
	}

EXPORT_C TInt CBaseSmsActiveSocketWatcher::RunError(TInt aError)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L8("Bio: RunL Failed with %d"), aError));
	Complete(aError);
	return KErrNone;
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::RunL()
//
// When the AO is state driven, this form of Run() is very effective
// DoRunL() takes the AO through the states, queuing another asynch step as required
// if DoRunL() detects the end of the cycle it returns without queuing another cycle.
//
// If Run() would exit without queuing another cycle it reports completion to the client.
// This is true if the asynch step or DoRunL fails, or the state cycle is complete
//
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("Bio: DoRunL: %S, iStatus %d"), &iBioMsgText, iStatus.Int()));
		
	DoRunL();
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::SetupL()
	{
	DoSetupL();
	// Start the RunL in the WaitForMessage
	if (iState != ESmsIsClass0Msg)
		{
		iState = ESmsWWaitForMsg;
		}
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::StartL()
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("Bio: StartL: %S"), &iBioMsgText));

	TRequestStatus* pS=&iStatus;
	
	// Nothing Asynchronous, so just throw into the wait state RunL()
	User::RequestComplete(pS, KErrNone);
	SetActive();
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::RestoreSettingsL(CMsvSession& aSession)
	{
	SetBioServiceId(aSession);

	// access sms service settings
	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	// load settings
	smsAccount->LoadSettingsL(*iSettings);

	CleanupStack::PopAndDestroy(smsAccount);
	}
	
EXPORT_C void CBaseSmsActiveSocketWatcher::RestoreSettingsL(TMsvId aBioServiceId, TMsvId aSmsServiceId)
	{
	SetBioServiceId(aBioServiceId, aSmsServiceId);
	
	// access sms service settings
	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	// load settings
	smsAccount->LoadSettingsL(*iSettings);

	CleanupStack::PopAndDestroy(smsAccount);
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::StoreMsgL(CSmsMessage* aSmsMessage)
	{
	StoreMsgL(aSmsMessage, EFalse);
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::StoreMsgL(CSmsMessage* aSmsMessage, TBool aCheckForSID)
	{
	BIOWATCHERLOG(iWatcherLog.Printf(_L("Bio: StoreMsgL: %S"), &iBioMsgText));

	CleanupStack::PushL(aSmsMessage);

	CMsvSession* session = CMsvSession::OpenSyncL(*this);
	CleanupStack::PushL(session);

	BIOWATCHERLOG(LogMessageL(*aSmsMessage));
	// The trap error code is ignored here. We already have loaded the settings
	// in the SetupL method. Any catastrophic failure would be reported later
	// when the message store is attempted.
	TRAP_IGNORE(RestoreSettingsL(*session));

	PreStoreActionL(*session, *aSmsMessage);

	CMsvEntry* msvEntry = session->GetEntryL(KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(msvEntry);
	TInt systemDrive = RFs::GetSystemDrive();
  	TInt driveUnit = session->CurrentDriveL();
  	 
  	TVolumeInfo volumeInfo;
  	User::LeaveIfError(iFs.Volume(volumeInfo, driveUnit));
  	 
  	 
  	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: driveUnit: %d "),driveUnit));
  	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: volumeInfo  : %d"), volumeInfo.iFree));
  	BIOWATCHERLOG(iWatcherLog.Printf(_L("BioNbs: threshold level= : %d"), KSmsThresholdDiskSpaceValue));
 
 
  	//Check if  non-system drive has enough space to store the message
  	if (driveUnit != systemDrive ) 
  	  	{
  	  	BIOWATCHERLOG(iWatcherLog.Printf(_L8("Low Memory")));
  	  	
  	  	TInt value;
  	  	TInt err = RProperty::Get(KUidSystemCategory,KUidPSDiskSpaceMonitorKeyType, value);
  	  	
  	  	BIOWATCHERLOG(iWatcherLog.Printf(_L("RProperty Get Value: %d "),err));
		
		if (volumeInfo.iFree < KSmsThresholdDiskSpaceValue)
  	  	 	{
  	  	 	if(value == ESmsDiskSpaceAvailable)
  	  	 	    {
  	  	 	    User::LeaveIfError(RProperty::Set(KUidSystemCategory,KUidPSDiskSpaceMonitorKeyType, ESmsDiskSpaceFull));
  	  	 	    }
  	  	 	User::Leave(KErrDiskFull);
  	  	 	}
  	  	 else
  	  	 	{
  	  	 	if(value == ESmsDiskSpaceFull)
  	  	 	    {
  	  	 	    User::LeaveIfError(RProperty::Set(KUidSystemCategory,KUidPSDiskSpaceMonitorKeyType, ESmsDiskSpaceAvailable));
  	  	 	    }
  	  	 	}
  	  	}


	if( CanStoreMessage() )
		{
		TBool retainReplaceMessage = ETrue;
		ReplaceTypeMessageL(*msvEntry, *aSmsMessage, retainReplaceMessage);
		CleanupStack::Pop(3, aSmsMessage); 
		CleanupStack::PushL(session); //guaranteed not leave because of the 3 previous POPs
		CleanupStack::PushL(msvEntry);

		if( retainReplaceMessage )
			CreateMessageL(*msvEntry, aSmsMessage, aCheckForSID); //destroys the CSmsMessage
		else
			delete aSmsMessage; //destroy the CSmsMessage as CreateMessageL() would have done

		CleanupStack::PopAndDestroy(2, session);
		}
	else
		{
		CleanupStack::PopAndDestroy(3, aSmsMessage);
		BIOWATCHERLOG(iWatcherLog.Printf(_L8("Not Creating Message")));
		}
	}

void CBaseSmsActiveSocketWatcher::ReplaceTypeMessageL(CMsvEntry& aEntry, CSmsMessage& aMessage, TBool& aRetainOriginalMessage)
	{
    if (aMessage.Type() == CSmsPDU::ESmsDeliver)
        {
        TUint8 pid(0);
        const CSmsPDU& sms = aMessage.SmsPDU();
        const CSmsDeliver& smsTemp = STATIC_CAST(const CSmsDeliver&,sms);
        pid = (TUint8)*smsTemp.ProtocolIdentifier();

        if ((pid & TSmsProtocolIdentifier::ESmsPIDTypeMask) == TSmsProtocolIdentifier::ESmsPIDShortMessageType)
            {
            if ( ((pid & TSmsProtocolIdentifier::ESmsShortMessageTypeMask) <= TSmsProtocolIdentifier::ESmsReplaceShortMessageType7)
                || ((pid & TSmsProtocolIdentifier::ESmsShortMessageTypeMask) == TSmsProtocolIdentifier::ESmsReturnCallMesage) )
                {
                DeleteReplaceTypeMessagesL(aEntry, pid, aMessage, aRetainOriginalMessage);
                }
            }
        }   // Type()
	}


void CBaseSmsActiveSocketWatcher::DeleteReplaceTypeMessagesL(CMsvEntry& aEntry, TUint8 aPid, CSmsMessage& aMessage, TBool& aRetainOriginalMessage)
	{
	CSmsDeliver& smsTemp = STATIC_CAST(CSmsDeliver&, aMessage.SmsPDU());
	TTime sourceMessageTime;
	TInt quart;
	smsTemp.ServiceCenterTimeStamp(sourceMessageTime, quart);
	
	// get the children
 	TMsvId parentFolder = KMsvGlobalInBoxIndexEntryId;
 
 	// If this is a class2 message, do the replacement in the class2 folder
 	TSmsDataCodingScheme::TSmsClass classTemp(TSmsDataCodingScheme::ESmsClass0);
 	smsTemp.Class(classTemp);

 	if (aMessage.Storage() == CSmsMessage::ESmsSIMStorage)
 		{	
 		if (CheckMessageExistsL(aEntry.Session(), iSettings->Class2Folder()))
 			{			
 			parentFolder = iSettings->Class2Folder();
 			}
 		}
 
 	aEntry.SetEntryL(parentFolder);
	
	TInt count = aEntry.Count();
	while (count--)
		{
		const TMsvSmsEntry& entry = aEntry[count];
		if ((entry.iMtm == KUidMsgTypeSMS || entry.iMtm == KUidBIOMessageTypeMtm) && entry.iType == KUidMsvMessageEntry)
			{
			const TUint8 entryPID = entry.ProtocolIdentifier();

			if ((entryPID & TSmsProtocolIdentifier::ESmsPIDTypeMask) == TSmsProtocolIdentifier::ESmsPIDShortMessageType)
				{
				if ((entryPID & TSmsProtocolIdentifier::ESmsShortMessageTypeMask) == (aPid & TSmsProtocolIdentifier::ESmsShortMessageTypeMask))
					{
					CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
					CleanupStack::PushL(paraLayer);
					CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
					CleanupStack::PushL(charLayer);
					CRichText* richtext = CRichText::NewL(paraLayer,charLayer);
					CleanupStack::PushL(richtext);
					
					CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *richtext);
					CleanupStack::PushL(header);
					
					CMsvEntry* currentEntry = aEntry.ChildEntryL(entry.Id());
					CleanupStack::PushL(currentEntry);
					
					CMsvStore* store = NULL;
					TRAPD(error, store = currentEntry->ReadStoreL());

					//If message arrived with "replace short message functionality", and some how message deleted from Mail2 store 
					//but entry exist in Index file. In such case, need to delete entry from Index file and treat this as a new message.
					if(error == KErrNotFound)
						{
						aRetainOriginalMessage = ETrue;
						aEntry.DeleteL(entry.Id());
						CleanupStack::PopAndDestroy(5); //paraLayer, charLayer, richtext, header, currentEntry
						return;
						}

					CleanupStack::PushL(store);
					header->RestoreL(*store);
					CleanupStack::PopAndDestroy(2); //store, currentEntry
					
					if ((header->FromAddress().Compare(smsTemp.ToFromAddress()) == 0))
						{
						CSmsDeliver& smsDeliver = static_cast<CSmsDeliver&>(header->Message().SmsPDU());
						TTime foundMessageTime;
						smsDeliver.ServiceCenterTimeStamp(foundMessageTime, quart);

						if (sourceMessageTime > foundMessageTime)
							{
							aRetainOriginalMessage = ETrue;
							aEntry.DeleteL(entry.Id());
							}
						else
							aRetainOriginalMessage = EFalse;
						}
					
					CleanupStack::PopAndDestroy(4); //paraLayer, charLayer, richtext, header
					}
				}
			}
		}
	}

TBool CBaseSmsActiveSocketWatcher::CheckForSID(TPtr& aMessage, TSecureId& aSecureId)
	{
	_LIT(KSecureIDStr, "//SYM");
	const TInt KIdLen = 8;

	// Sanity check
	if (aMessage.Length() < (KIdLen + KSecureIDStr().Length()))
		return EFalse;

	TBool locatedSecureId = (aMessage.Find(KSecureIDStr) == 0);
	if (locatedSecureId)
		{
		TPtr idPtr = aMessage.MidTPtr(KSecureIDStr().Length(), KIdLen); // Extract the string for the id.
		TLex numConverter;
		numConverter.Assign(idPtr.Ptr());

		TUint32 hexId;
		if (numConverter.Val(hexId, EHex) == KErrNone)
			aSecureId = TSecureId(hexId); // Convert the string to a number
		else
			locatedSecureId = EFalse;
		}
	return locatedSecureId;
	}


void CBaseSmsActiveSocketWatcher::CreateMessageL(CMsvEntry& aEntry, CSmsMessage* aMessage, TBool aCheckForSecureId)
	{
	// This function destroys CSmsMessage.
	CleanupStack::PushL(aMessage);

	// Create a CSmsHeader based on this message. smsHdr takes ownership of aMessage
	CSmsHeader* header = CSmsHeader::NewL(aMessage);
	CleanupStack::Pop(aMessage);
	CleanupStack::PushL(header);

	TMsvSmsEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.SetVisible(EFalse);
	entry.SetInPreparation(ETrue);
	entry.SetReadOnly(EFalse);
	entry.SetUnread(ETrue);
	entry.SetSendingState(KMsvSendStateNotApplicable);

	TInt length = iSettings->DescriptionLength();
	HBufC* buf = HBufC::NewLC(length);
	TPtr description = buf->Des();	
	
	const CSmsEmailFields& fields = header->EmailFields();
	TBool isEmail = fields.HasAddress();
					
	TMsvId parentFolder = KMsvGlobalInBoxIndexEntryId;
	// Fix for INC17771: This defect meant that a message in the outbox, e.g. would be put into the inbox;
	// therefore, we check the status of the message to see what folder it should go into
	if( aMessage->Status() == NMobileSmsStore::EStoredMessageUnsent )
		parentFolder = KMsvGlobalOutBoxIndexEntryId;
	
	if( iBioMsgUID != KNullUid )
		{
		// BioficateEntry!!!
		// Set up all the needed ids
		TSmsUtilities::PopulateMsgEntry(entry, *aMessage, iBioServiceId, *iSettings, KUidBIOMessageTypeMtm);
		entry.iBioType = iBioMsgUID.iUid;

		// Look up and set the description
		TInt index;
		CBIODatabase* bioDB = CBIODatabase::NewLC(aEntry.Session().FileSession());
		bioDB->GetBioIndexWithMsgIDL(iBioMsgUID, index);
		description.Copy(bioDB->BifReader(index).Description().Left(length));
		entry.iDescription.Set(description);
		CleanupStack::PopAndDestroy(bioDB);
		}
	else
		{
		TSmsUtilities::PopulateMsgEntry(entry, *aMessage, iSmsServiceId, *iSettings);

		if( isEmail && fields.Subject().Length() > 0 )
			{
			// For an email SMS should only set the description to be the subject
			// if there is a subject.
			entry.iDescription.Set(fields.Subject());
			}
		else
			{
			// For normal SMS and for email SMS messages that do not have a 
			// subject field, the description is the begining part of the body.
			User::LeaveIfError(iGetDetDesc->GetDescription(*aMessage, description, length));
			entry.iDescription.Set(description);
			}

		TBool classDefined(EFalse);
		TSmsDataCodingScheme::TSmsClass classTemp(TSmsDataCodingScheme::ESmsClass0);

		if( aMessage->Type() == CSmsPDU::ESmsDeliver )
			{
            const CSmsDeliver& deliver = static_cast<const CSmsDeliver&>(aMessage->SmsPDU());
            entry.SetProtocolIdentifier(*deliver.ProtocolIdentifier());
			classDefined = deliver.Class(classTemp);

			if( aMessage->Storage() == CSmsMessage::ESmsSIMStorage)
				{
				// This is a class 2 message - store the new SMS message in the 
				// folder specified in the SMS service settings (if that folder
				// exists, other wise default to Inbox).
				if( CheckMessageExistsL(aEntry.Session(), iSettings->Class2Folder()) )
					parentFolder = iSettings->Class2Folder();
				}
            }
		entry.SetClass(classDefined, classTemp);
		// Check the Existing sms message for User Prompt indicator
		const RPointerArray<const CEmsInformationElement>&  emsElements= aMessage->GetEMSInformationElementsL();
		// Loop through the array checking for a user prompt indicator element
		TInt count = emsElements.Count();
		for( TInt i=0; i < count; ++i )
			{
			if( emsElements[i]->Identifier() == CSmsInformationElement::ESmsEnhancedUserPromptIndicator )
				{
				entry.SetUserPromptIndicator(ETrue);
				break;
				}
			}
		}
	// Set the details
 	TInt detailsLen = KSmsDetailsLength;
  	if(isEmail)
		{
		detailsLen = fields.Addresses().MdcaPoint(0).Length();
		}   
  	RBuf details;
  	details.CleanupClosePushL() ;
  	details.CreateL(detailsLen);
	TInt err = KErrNone;
	if( isEmail )
		{
		// For an email SMS, details is taken from the address field.
		details.Copy(fields.Addresses().MdcaPoint(0));
		}
	else
		{
		// For normal SMS message details is taken from the info from destination
		// address in the PDU.
		err = iGetDetDesc->GetDetails(aEntry.Session().FileSession(), *aMessage, details);
		}
	if( err == KErrNone )
		entry.iDetails.Set(details);

	// Create the entry
	aEntry.SetEntryL(parentFolder);
	aEntry.CreateL(entry);
	aEntry.Session().CleanupEntryPushL(entry.Id());
	aEntry.SetEntryL(entry.Id());

	// Save the message
	CMsvStore* store = aEntry.EditStoreL();
	CleanupStack::PushL(store);

	// Save off the CSmsHdr
	header->StoreL(*store);

	// Save the body - create and fill a CRichText object.
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* richText = CRichText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(richText);

	TInt len= aMessage->Buffer().Length();
	HBufC* bufBody = HBufC::NewLC(len);

	TPtr bufBodyPtr = bufBody->Des();
	aMessage->Buffer().Extract(bufBodyPtr, 0, len);
	richText->InsertL(0, bufBodyPtr); 
	store->StoreBodyTextL(*richText);

	TBool foundSecureId(EFalse);
	TSecureId secureId;
	if (aCheckForSecureId)
		foundSecureId = CheckForSID(bufBodyPtr, secureId); // Check for SID

	CleanupStack::PopAndDestroy(4, paraFormatLayer); //bufBody, richText, charFormatLayer, paraFormatLayer
	store->CommitL();

	// Update the index...
	entry.SetReadOnly(ETrue);
	entry.SetVisible(Visible());
	entry.SetInPreparation(EFalse);
	entry.iSize = store->SizeL();

	if (foundSecureId)
		aEntry.ChangeL(entry, secureId);
	else
		aEntry.ChangeL(entry);

	CleanupStack::PopAndDestroy(store);
	aEntry.Session().CleanupEntryPop(); // entry
	CleanupStack::PopAndDestroy(&details);  
	CleanupStack::PopAndDestroy(2, header);	// this also deletes aMessage, as owned by header
	}

TBool CBaseSmsActiveSocketWatcher::CheckMessageExistsL(CMsvSession& aSession, TMsvId aId) const
	{
	TMsvEntry tempEntry;
	TMsvId tempService;
	const TInt err = aSession.GetEntry(aId, tempService, tempEntry);

	if (err != KErrNotFound)
		User::LeaveIfError(err);

	return (err == KErrNone);
	}

void CBaseSmsActiveSocketWatcher::SetBioServiceId(CMsvSession& aSession)
    {
	TInt err = KErrNone;

	if (iBioServiceId == KMsvLocalServiceIndexEntryId)
		{
		TRAP(err, TSmsUtilities::ServiceIdL(aSession, iBioServiceId,  KUidBIOMessageTypeMtm));
		if (err == KErrNotFound)
			{
			iBioServiceId = KMsvLocalServiceIndexEntryId;
			err = KErrNone;
			}
		}

	if (err == KErrNone && iSmsServiceId == KMsvLocalServiceIndexEntryId)
		{
		TRAP(err, TSmsUtilities::ServiceIdL(aSession, iSmsServiceId));
		if (err == KErrNotFound)
			{
			iSmsServiceId = KMsvLocalServiceIndexEntryId;
			err = KErrNone;
			}
		}
    }

void CBaseSmsActiveSocketWatcher::SetBioServiceId(TMsvId aBioServiceId, TMsvId aSmsServiceId)
	{
	if (iBioServiceId == KMsvLocalServiceIndexEntryId)
		{
		iBioServiceId = aBioServiceId;
		}
	if (iSmsServiceId == KMsvLocalServiceIndexEntryId)
		{
		iSmsServiceId = aSmsServiceId;
		}
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::ConstructL(CBIODatabase& aBioDb, TMsvId aBioServiceId, TMsvId aSmsServiceId)
	{
	SetBioMsgText(aBioDb);
	iSettings = CSmsSettings::NewL();	
	
	// load ECOM interface used to create SMS details and description values.
	iGetDetDesc = CSmsGetDetDescInterface::NewL();
	
	RestoreSettingsL(aBioServiceId, aSmsServiceId);
	}

EXPORT_C void CBaseSmsActiveSocketWatcher::SetBioMsgText(CBIODatabase& aBioDb)
	{
	TInt index = 0;
	TRAPD(err, aBioDb.GetBioIndexWithMsgIDL(iBioMsgUID, index));
			
	if (!err)
		iBioMsgText = aBioDb.BifReader(index).Description();
	}
	
EXPORT_C void CBaseSmsActiveSocketWatcher::PreStoreActionL(CMsvSession& /*aSession*/, CSmsMessage& /*aMessage*/)
	{
	}

#ifndef _BIOMSG_NO_LOGGING

void CBaseSmsActiveSocketWatcher::LogMessageL(const CSmsMessage& aSmsMessage)
	{
	if (!iWatcherLog.IsLogging())
		return;

	BIOWATCHERLOG(iWatcherLog.Printf(KNullDesC));
	TBuf<64> temp;
	
	switch (aSmsMessage.Type())
		{
		case CSmsPDU::ESmsDeliver:
			temp = _L("[Deliver%S]");
			break;
		case CSmsPDU::ESmsStatusReport:
			temp = _L("[StatusReport%S]");
			break;
		default:
			temp = _L("[Message%S]");
			break;
		}

	TInt length = iBioMsgText.Length();
	HBufC* hBuf = HBufC::NewLC(length);
	TPtr buf1 = hBuf->Des();

	for (TInt i = 0; i < length; i++)
		{
		if (iBioMsgText[i] != ' ' && iBioMsgText[i] != '\n' && iBioMsgText[i] != '\t' && iBioMsgText[i] != '\r')
			buf1.Append(iBioMsgText[i]);
		}

	BIOWATCHERLOG(iWatcherLog.Printf(temp, &buf1));
	CleanupStack::PopAndDestroy(); //hBuf
	hBuf = NULL;

	length = aSmsMessage.Buffer().Length();
	hBuf = HBufC::NewLC(32 + length);
	TPtr buf2 = hBuf->Des();
	aSmsMessage.Buffer().Extract(buf2, 0, length);
	buf2.Insert(0, _L("Message= "));
	LogEachLine(buf2);
	CleanupStack::PopAndDestroy(); //hBuf
	hBuf = NULL;

	temp = aSmsMessage.ToFromAddress();
	BIOWATCHERLOG(iWatcherLog.Printf(_L("Recipients= %S"), &temp));

	temp = aSmsMessage.ServiceCenterAddress();
	BIOWATCHERLOG(iWatcherLog.Printf(_L("SC= %S"), &temp));

	BIOWATCHERLOG(iWatcherLog.Printf(_L8("BioUid= %d"), iBioMsgUID.iUid));

	if (aSmsMessage.SmsPDU().DataCodingSchemePresent())
		{
		temp.Zero();
		temp.Append(_L("Encoding= "));
		switch (aSmsMessage.SmsPDU().Alphabet())
			{
			case TSmsDataCodingScheme::ESmsAlphabet7Bit:
				temp.Append(_L("7"));
				break;
			case TSmsDataCodingScheme::ESmsAlphabet8Bit:
				temp.Append(_L("8"));
				break;
			case TSmsDataCodingScheme::ESmsAlphabetUCS2:
				temp.Append(_L("16"));
				break;
			default:
				temp.Append(_L("Unsupported"));
				break;
			}

		BIOWATCHERLOG(iWatcherLog.Printf(temp));
		}

	BIOWATCHERLOG(iWatcherLog.Printf(KNullDesC));
	}

void CBaseSmsActiveSocketWatcher::Log(const TDesC& aInput)
	{
	if (!iWatcherLog.IsLogging())
		return;

	//This function is required because iWatcherLog.Printf()
	//will only accept descriptors of max length 255.

	const TInt KSmsLogMaxLength = KLogBufferSize - 22; //max number of characters RFileLogger will display
	const TInt length = aInput.Length();
	TInt start = 0;

	while (start < length)
		{
		TPtrC buf(aInput.Mid(start, Min(KSmsLogMaxLength, length - start)));
		BIOWATCHERLOG(iWatcherLog.Printf(KWatcherStringFormat, &buf));
		start += KSmsLogMaxLength;
		}
	}

void CBaseSmsActiveSocketWatcher::Log(const TDesC8& aInput)
	{
	if (!iWatcherLog.IsLogging())
		return;
	
	//This function is required because iWatcherLog.Printf()
	//will only accept descriptors of max length 255.
	
	const TInt KSmsLogMaxLength = KLogBufferSize - 22; //max number of characters RFileLogger will display
	const TInt length = aInput.Length();
	TInt start = 0;
	
	while (start < length)
		{
		TPtrC8 buf(aInput.Mid(start, Min(KSmsLogMaxLength, length - start)));
		BIOWATCHERLOG(iWatcherLog.Printf(KWatcherStringFormat, &buf));
		start += KSmsLogMaxLength;
		}
	}


void CBaseSmsActiveSocketWatcher::LogEachLine(const TDesC& aInput)
	{
	if (!iWatcherLog.IsLogging())
		return;

	//LogEachLine() logs each line in aInput separately.
	//This is required because RFileLogger (used by iWatcherLog.Printf())
	//will replace all line feeds and carriage returns with a '.'

	TPtrC part(aInput);
	TInt length = part.Length();

	while (length > 0)
		{
		TInt find = part.Locate('\n');

		if (find == KErrNotFound)
			{
			Log(part);
			break;
			}
		else
			{
			if (find == 0)
				BIOWATCHERLOG(iWatcherLog.Printf(KNullDesC));
			else
				Log(part.Left(find));

			if (find < length - 1)
				part.Set(part.Mid(find + 1));
			else
				break;
			}

		length = part.Length();
		}
	}

#endif
