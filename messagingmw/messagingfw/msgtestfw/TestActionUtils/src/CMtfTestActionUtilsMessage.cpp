// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

#include "MtfTestActionUtilsUser.h"
#include "CMtfTestActionUtilsMessage.h"
#include "TestFrameworkActionsUtils.h"


#include "txtfmlyr.h"
#include "txtrich.h"
#include "smuthdr.h" 
#include "miuthdr.h"
#include "miutmsg.h"

#include<btmsgtypeuid.h>
#include<irmsgtypeuid.h>

#include "obexheader.h"

#include<btheader.h>
#include<irheader.h>


/*
 Compare the Body text contents of the message with the file contents
*/
TInt CMtfTestActionUtilsMessage::VerifyBodyTextContentsL(CMsvEntry& aEntry, HBufC* aFileName )
	{
	TInt returnValue = KErrNone;
	
	// Create a Rich Text object
	CPlainText::TTextOrganisation ttOrg = {CPlainText::EOrganiseByLine};

	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* bodyRichText1 = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(bodyRichText1);
	
	// Store the file contents into the CRichText object
	bodyRichText1->ImportTextFileL(0, aFileName->Des(), ttOrg);
	
	TInt len1 = bodyRichText1->DocumentLength();
	HBufC* buf1 = HBufC::NewL(len1);
	CleanupStack::PushL(buf1);
	TPtr fileContents = buf1->Des();
	bodyRichText1->Extract(fileContents,0,(len1));
	
	// Get the type of the message 
	TUid msgMtmType = aEntry.Entry().iMtm; 


	CRichText* bodyRichText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(bodyRichText);
	
	// Email message
	if((msgMtmType.operator==(KUidMsgTypeSMTP)) || (msgMtmType.operator==(KUidMsgTypePOP3)) || (msgMtmType.operator==(KUidMsgTypeIMAP4)) )
		{
		CImEmailMessage*	emailMsg = CImEmailMessage::NewLC(aEntry);
		CImEmailMessage::TImEmailEntryType tt = CImEmailMessage::EThisMessageOnly;
		
		emailMsg->GetBodyTextL(aEntry.EntryId(),tt,*bodyRichText,*paraFormatLayer,*charFormatLayer);
			
		TInt len = bodyRichText->DocumentLength();
		HBufC* buf = HBufC::NewL(len);
		CleanupStack::PushL(buf);
		TPtr bodyContents = buf->Des();
		bodyRichText->Extract(bodyContents,0,(len));

		returnValue = bodyContents.Compare(fileContents);
		CleanupStack::PopAndDestroy(2,emailMsg);
		}
	else if((msgMtmType.operator==(KUidMsgTypeSMS)))
		{
		// SMS message	
		CMsvStore* store = aEntry.ReadStoreL();
		CleanupStack::PushL(store);
		
		store->RestoreBodyTextL(*bodyRichText);
		TInt len = bodyRichText->DocumentLength();
		
		HBufC* buf = HBufC::NewL(len);
		CleanupStack::PushL(buf);
		TPtr bodyContents = buf->Des();
		bodyRichText->Extract(bodyContents,0,(len));

		returnValue = bodyContents.Compare(fileContents);
		CleanupStack::PopAndDestroy(2,store);
		}
	else if((msgMtmType.operator==(KUidMsgTypeIrUID)) ||(msgMtmType.operator==(KUidMsgTypeBt)))
		{
		returnValue = KErrNotSupported;
		}
		else
		{
		returnValue = KErrUnknown;
		}
	CleanupStack::PopAndDestroy(5,paraFormatLayer);
	return returnValue;
	}

TInt CMtfTestActionUtilsMessage::VerifySmsRecipientsL(CMsvStore& aMsgStore,CDesC16Array&  aRecipientArray)
	{
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(richText);
	richText->Reset();
	
	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *richText);
	CleanupStack::PushL(header);
	
	header->RestoreL(aMsgStore);
	
	TInt returnValue = KErrNone;
	
	// Get the recipient addresses set for the SMS message
	CArrayPtrFlat<CSmsNumber>& recipientList = header->Recipients();

	// Verify the count of the recipient address set
	if(aRecipientArray.Count() != recipientList.Count())
		{
		returnValue = KErrGeneral;
		}
	else
		{
		TBool foundFlag = EFalse;
		
		for(TInt index = 0; index < aRecipientArray.Count(); index++, foundFlag = EFalse)
			{
			for(TInt indexRcptArray = 0; indexRcptArray < recipientList.Count(); indexRcptArray++)
				{
				TPtrC address = recipientList.At(indexRcptArray)->Address();
				if(aRecipientArray.MdcaPoint(index).Compare(address) == 0)
					{
					foundFlag = ETrue;
					break;
					}
				}//End of inner For Loop
					
			if (foundFlag == EFalse)
				{
				returnValue = KErrNotFound;
				break;
				}
			}// End If
		}// End else
		
	CleanupStack::PopAndDestroy(4,paraFormatLayer);

	return returnValue;
	}
			

TInt CMtfTestActionUtilsMessage::VerifyEmailRecipientsL(CMsvStore& aMsgStore,CDesC16Array& aRecipientArray, TImHeaderEncodingInfo::TFieldList aFieldType)
	{
	CImHeader* header = CImHeader::NewLC();
	header->RestoreL(aMsgStore);

	TInt returnValue = KErrNone;
	CDesCArray* rcptArray = NULL;

	switch(aFieldType)
		{
		case  TImHeaderEncodingInfo::ETo:
			rcptArray = &(header->ToRecipients());
		break;
		case  TImHeaderEncodingInfo::ECc:
			rcptArray = &(header->CcRecipients());
		break;
		case  TImHeaderEncodingInfo::EBcc:
			rcptArray = &(header->BccRecipients());
		break;	
		default:
			returnValue = KErrNotSupported;
		}
	 
	if( returnValue == KErrNone)
		{
		if( aRecipientArray.Count() != rcptArray->Count())
			{
			returnValue = KErrGeneral;
			}
		else
			{
			TInt recipientArrayCount = aRecipientArray.Count();
			
			TBool foundFlag = EFalse;
			for(TInt index = 0; index < recipientArrayCount; index++, foundFlag = EFalse)
				{
				for(TInt indexRcptArray = 0; indexRcptArray < recipientArrayCount; indexRcptArray++)
					{
					TPtrC ptr = aRecipientArray.MdcaPoint(index);
					TPtrC ptr1 =rcptArray->MdcaPoint(indexRcptArray);
					if (ptr.Match(ptr1) == 0)
						{
						foundFlag = ETrue;
						break;
						}
					}//End of inner For Loop
					
				if (foundFlag == EFalse)
					{
					returnValue = KErrNotFound;
					break;
					}
				}//End of FOR loop
			}//End Else
		}//End If
		
	CleanupStack::PopAndDestroy(header);
	return returnValue;
	}		



/**
   The function is called if IR or Bluetooth
   The recipient address set for the message is verified against
   the expected valuse
*/ 
  																			/*TPtrC8*/
TInt CMtfTestActionUtilsMessage::VerifyObexRecipientL (CMsvEntry& aEntry, CMsvStore& aStore,	CDesC16Array&  aRecipientArray)
	{
	TInt returnValue = KErrNone;
	TUid msgMtmType = aEntry.Entry().iMtm;

	CObexMtmHeader* header;
	    
	if(msgMtmType.operator == (KUidMsgTypeBt))
		{
		header = CBtHeader::NewL();
		}
	else if(msgMtmType.operator == (KUidMsgTypeIrUID))
		{
		header = CIrHeader::NewL();
		}
	else
		{
		return KErrUnknown;
		}
	CleanupStack::PushL(header);
		
	header->RestoreL(aStore);

	TPtrC8 tmp = header->Addr();
	HBufC*	itemBuff = HBufC::NewLC(tmp.Length());
	itemBuff->Des().Copy(tmp);
	TPtrC16 msgRecipientAddr = itemBuff->Des();
	
	if(aRecipientArray.MdcaPoint(0).Compare(msgRecipientAddr) != 0)
		{
		returnValue = KErrGeneral;
		}

	CleanupStack::PopAndDestroy(itemBuff);	
	CleanupStack::PopAndDestroy(header);
	return returnValue;
	}
	
	

TInt CMtfTestActionUtilsMessage::VerifyEmailSubjectL (CMsvEntry& aEntry,HBufC*  aSubject)
	{
	TInt returnValue = KErrNone;
	
	CMsvStore* store = aEntry.ReadStoreL();
	CleanupStack::PushL(store);
	
	CImHeader* messageHeader = CImHeader::NewLC();
	messageHeader->RestoreL(*store);

	TPtrC messageSubject = messageHeader->Subject();

	if(messageSubject.Compare(*aSubject) != 0)
		{
		returnValue = KErrGeneral;
		}
	
	CleanupStack::PopAndDestroy(messageHeader);
	CleanupStack::PopAndDestroy(store);
	
	return returnValue;
	}


TInt CMtfTestActionUtilsMessage::VerifyObexSubjectL (CMsvEntry& aEntry, HBufC* aSubject)
	{
	TInt returnValue = KErrNone;
	TUid msgMtmType = aEntry.Entry().iMtm;

	CObexMtmHeader* header;
    
	if(msgMtmType.operator==(KUidMsgTypeBt))
		{
		header = CBtHeader::NewL();
		}
	else if(msgMtmType.operator==(KUidMsgTypeIrUID))
		{
		header = CIrHeader::NewL();
		}
	else
		{
		return KErrUnknown;
		}
	CleanupStack::PushL(header);
	
	CMsvStore* msgStore = aEntry.ReadStoreL();
	
	CleanupStack::PushL(msgStore);
	
	header->RestoreL(*msgStore);

	TPtrC messageSubject = header->Subject();

	if( messageSubject.Compare(*aSubject) != 0 )
		{
		returnValue = KErrGeneral;
		}
		
	CleanupStack::PopAndDestroy(msgStore);
	CleanupStack::PopAndDestroy(header);
		
	return returnValue;
	}

void CMtfTestActionUtilsMessage::FormatBtRecipientAddress(TDesC& aRecipient, TDes& formattedAddr)
	{
	TPtrC16 ptr16 = aRecipient;
	
	for(int index = 0; index < 12; index+=4)
		{
		// do 4 characters to get one out. We're going to force the compiler to do the character endianness.
		TUint8 byte1 = TestFrameworkActionsUtils::GetByteFromUnicodeHexTextChar(&ptr16[index]);
		TUint8 byte2 = TestFrameworkActionsUtils::GetByteFromUnicodeHexTextChar(&ptr16[index+1]);
		TUint8 byte3 = TestFrameworkActionsUtils::GetByteFromUnicodeHexTextChar(&ptr16[index+2]);
		TUint8 byte4 = TestFrameworkActionsUtils::GetByteFromUnicodeHexTextChar(&ptr16[index+3]);

		byte1 = (TUint8) ( ((byte1 & 0x0F) << 4) | (byte2 & 0x0f) );
		byte3 = (TUint8) ( ((byte3 & 0x0F) << 4) | (byte4 & 0x0f) );

		// make sure this is put together in network endianness
		TUint16 result = (TUint16)( (TUint16)(byte3 << 8) | byte1 );

		formattedAddr.Append( result );
		}
	}
