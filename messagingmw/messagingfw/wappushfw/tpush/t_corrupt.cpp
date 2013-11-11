// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// T_SLMESSAGE.CPP
// 
//

#include <msvids.h>
#include <msvstd.h>

#include  <push/sislpushmsgutils.h>
#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"
#include "T_corruptSi.h"
#include "T_corruptSL.h"
#include "T_corruptSia.h"
#include "T_corruptMpm.h"
#include "T_corruptMpr.h"
#include "T_corruptMpa.h"


/** set the test name for this test case
	@param void
	@return string
		returns test name
*/
const TDesC& CWapPushCorruptMessageTest::TestName()
	{
	_LIT(KTextCorruptMessageTest,"Corrupt Message Test");
	return KTextCorruptMessageTest;
	}


/** Set the specify the number of test messages to use 
 * @param None
 * @return None
*/
void CWapPushCorruptMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = ECorruptMultiAlternativeMsg21+1;  // last enumeration used!
	}


/** create test messages to use where aTestCase represents the
	message index for multiple messages.
	@param TInt
		message index value for selectiong a test message
	@return void
 */
void CWapPushCorruptMessageTest::PrepareTestMessageL(TInt aTestCase)
{
	iHeadersBuf.Zero();
	iBodyBuf.Zero();
	iHeadersBuf.Delete(0, KBufferSize);
	iBodyBuf.Delete(0,KBufferSize);

	TBuf8<KBufferSize> temp;
	CWapPushMsgGen* msgGen = CWapPushMsgGen::NewL();
	CleanupStack::PushL(msgGen);

	switch (aTestCase)
		{
//-- start of Si Corrupt msg Tests --
		case ECorruptSiMsg1 ://'href' - Optional data missing
		/** if 'href' is empty , or omitted, the SI corresponds to a notification.
		 *  in this case si-id  value is used as href value.
		 *  we assuming that this msg has not expired yet by setting the date to '2001-10-30T00:00:00Z'
		 *  and the action is not set to 'delete' which should generate an Entry in the Msg store. 
		 */
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
		iHeadersBuf.Append(KMessageHeaderSi, sizeof(KMessageHeaderSi));
		
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId_Msg1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KSeptemberDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, KNotExipredDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSIActionSignal);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("Msg1: You have 4 new e-mails"));
		break;

		case ECorruptSiMsg2://'si-id' - Optional data missing
		/** if message has no si-id but does have a href - use href as si-id */
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
		iHeadersBuf.Append(KMessageHeaderSi, sizeof(KMessageHeaderSi));
		
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KSeptemberDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, KNotExipredDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSIActionSignal);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("Msg2: You have 4 new e-mails"));
		break;

	case ECorruptSiMsg3://'created' - Optional data missing
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
		iHeadersBuf.Append(KMessageHeaderSi, sizeof(KMessageHeaderSi));
		
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, KNotExipredDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSIActionSignal);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("Msg3: You have 4 new e-mails"));
		break;

	case ECorruptSiMsg4 ://'si-expires' - Optional data missing
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
		iHeadersBuf.Append(KMessageHeaderSi, sizeof(KMessageHeaderSi));
		
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KSeptemberDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSIActionSignal);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("Msg4: You have 4 new e-mails"));
		break;

	case ECorruptSiMsg5 ://'action' - Optional data missing
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
		iHeadersBuf.Append(KMessageHeaderSi, sizeof(KMessageHeaderSi));
		
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KSeptemberDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, KNotExipredDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("Msg5: You have 4 new e-mails"));
		break;

	case ECorruptSiMsg6 ://Mandatory data missing - The INFO Element-
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
		iHeadersBuf.Append(KMessageHeaderSi, sizeof(KMessageHeaderSi));
		
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KSeptemberDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, KSIOldDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSIActionSignal);
		break;

	case ECorruptSiMsg7:// Si Missing Body
		iHeadersBuf.Append(KMessageHeaderSi, sizeof(KMessageHeaderSi));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, KNullDesC8);
		break;

//-- start of Sic Corrupt msg Tests --
	case ECorruptSicMsg1://Sic Missing Body
		iHeadersBuf.Append(KMessageHeaderSic, sizeof(KMessageHeaderSic));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, KNullDesC8);
		break;

//-- start of Sl Corrupt msg Tests --
	case ECorruptSlMsg1:// Header - wrong App ID 
		iHeadersBuf.Append(KSlMsgHeaderWrongAppId, sizeof(KSlMsgHeaderWrongAppId));
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlActionStr);
		break;

	case ECorruptSlMsg2: // Header - Content type SLC instead of SL, body SL
		iHeadersBuf.Append(KSlcMsgValidHeader, sizeof(KSlcMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlActionStr);
		break;

	case ECorruptSlMsg3:// Missing Body
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, KNullDesC8);
		break;

	case ECorruptSlMsg4: // Unsupported XML version 2.0
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::ECorruptXmlVersionTest);
		msgGen->AppendMsgDesL(KSlStartTag);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlActionStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EXMLNoFieldStr, KSlEndTag);
		break;

	case ECorruptSlMsg5: // Corrupt text body - wrong DTD in XML header
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::ECorruptMsgTest);
		msgGen->AppendMsgDesL(KBadDTDSlStart);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlActionStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EXMLNoFieldStr, KNogoodEnd);
		break;

	case ECorruptSlMsg6: // Corrupt text body - wrong tag in XML header
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::ECorruptMsgTest);
		msgGen->AppendMsgDesL(KBadTagSlStart);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlActionStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EXMLNoFieldStr, KSlEndTag);
		break;

	case ECorruptSlMsg7: // Corrupt text body - invalid tags in XML body
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		msgGen->AppendFieldL(CWapPushMsgGen::EXMLNoFieldStr, KSLDummyXMLElement);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlActionStr);
		break;

	case ECorruptSlMsg8: // Corrupt text body - invalid tags in XML body
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		msgGen->AppendFieldL(CWapPushMsgGen::EBadHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlActionStr);
		break;


	case ECorruptSlMsg9: // Corrupt text body - invalid tags in XML body
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		msgGen->AppendFieldL(CWapPushMsgGen::EXMLNoFieldStr, KSLDummyXMLElement);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr);
		msgGen->AppendFieldL(CWapPushMsgGen::EBadAction, KSlActionStr);
		break;

	case ECorruptSlMsg10 :// Attribute  'href' Not specified.
			// the SL Content handler should leave with KErrCorrupt
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSlHrefStr);
		break;

	case ECorruptSlMsg11 : 	// Attribute  'action' Not specified.
			// the value 'execute-low' should be used
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSlHrefStr2);
		break;

//-- start of Slc Corrupt msg Tests --
	case ECorruptSlcMsg1:// Header - Wrong App Id -  0x19 instead of 2.
		iHeadersBuf.Append(KSlcMsgHeaderWrongAppId, sizeof(KSlcMsgHeaderWrongAppId));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KValidSLCMessageBody, sizeof(KValidSLCMessageBody));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	case ECorruptSlcMsg2:// Header - Content type SL instead of SLC
		iHeadersBuf.Append(KSlMsgValidHeader, sizeof(KSlMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KValidSLCMessageBody, sizeof(KValidSLCMessageBody));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	case ECorruptSlcMsg3:// WMLC - body has corrupt bit uses undefined 0x0D 
		iHeadersBuf.Append(KSlcMsgValidHeader, sizeof(KSlcMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KCorruptSLCMsgBody, sizeof(KCorruptSLCMsgBody));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, temp);
		break;

	case ECorruptSlcMsg4: //Msg body ends abruptedly, string and message unterminated
		iHeadersBuf.Append(KSlcMsgValidHeader, sizeof(KSlcMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KUnterminatedSLCMsgBody, sizeof(KUnterminatedSLCMsgBody));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	case ECorruptSlcMsg5:// WMLC - missing Body
		iHeadersBuf.Append(KSlcMsgValidHeader, sizeof(KSlcMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, KNullDesC8);
		break;

	case ECorruptSlcMsg6:// WMLC - empty Body ( 1 Null character)
		iHeadersBuf.Append(KSlcMsgValidHeader, sizeof(KSlcMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KNullSLCMsgBody, sizeof(KNullSLCMsgBody));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, temp);
		break;

	case ECorruptSlcMsg7:// WMLC - missing Href
		iHeadersBuf.Append(KSlcMsgValidHeader, sizeof(KSlcMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KSLCMsgBodyNoUri, sizeof(KSLCMsgBodyNoUri));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	case ECorruptSlcMsg8:// WMLC - missing Action
		iHeadersBuf.Append(KSlcMsgValidHeader, sizeof(KSlcMsgValidHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KSLCMsgBodyNoAction, sizeof(KSLCMsgBodyNoAction));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

//-- start of Sia Corrupt msg Tests --
	case ECorruptSiaMsg1: // Corrupt Body
		iHeadersBuf.Append(KSiaMsgHeader, sizeof(KSiaMsgHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KSiaCorruptBody, sizeof(KSiaCorruptBody));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	case ECorruptSiaMsg2:	// Corrupt CO/ Ack
		iHeadersBuf.Append(KSiaMsgHeader, sizeof(KSiaMsgHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KSiaCorruptBody, sizeof(KSiaCorruptBody));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,KNullDesC8);
		break;

	case ECorruptSiaMsg3:	// Corrupt CO/no Ack
		iHeadersBuf.Append(KSiaMsgHeader, sizeof(KSiaMsgHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,KNullDesC8);
		break;

	case ECorruptSiaMsg4:	// SIA doesn't accept a connection, the test harness should time out
		iHeadersBuf.Append(KSiaMsgHeader, sizeof(KSiaMsgHeader));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,KNullDesC8);
		break;


//-- start of Multi-Part/Mixed  Corrupt msg Tests --
//-- Multi-Part/Related  Corrupt msg Tests &      --
//-- Multi-Part/Alternative  Corrupt msg Tests    --

	//Corrupt first part
	case ECorruptMultiMixedMsg1:
	case ECorruptMultiRelatedMsg1:
	case ECorruptMultiAlternativeMsg1:	
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyCorrupt, sizeof(KPart1BodyCorrupt));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;
	
	//Corrupt second part
	case ECorruptMultiMixedMsg2:
	case ECorruptMultiRelatedMsg2:
	case ECorruptMultiAlternativeMsg2:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyCorrupt, sizeof(KPart2BodyCorrupt));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	//Corrupt Push Message header
	case ECorruptMultiMixedMsg3:
	case ECorruptMultiRelatedMsg3:
	case ECorruptMultiAlternativeMsg3:
		// Add correct content type
		if (aTestCase == ECorruptMultiMixedMsg3)
			iHeadersBuf.Copy( KMultiMixedContentType,1);
		else if (aTestCase == ECorruptMultiRelatedMsg3)
			iHeadersBuf.Copy(KMultiRelatedContentType, 1);
		else if (aTestCase == ECorruptMultiAlternativeMsg3)
			iHeadersBuf.Copy(KMultiAlternativeContentType, 1);
		// Now add some junk to header
		iHeadersBuf.Append(KWSPHeaderCorrupt, sizeof(KWSPHeaderCorrupt));
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid, sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 1st Multi Part Header - header length value too small, header actually longer
	case ECorruptMultiMixedMsg4:
	case ECorruptMultiRelatedMsg4:
	case ECorruptMultiAlternativeMsg4:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1HeaderValTooSmall, sizeof(KHeaderPart1HeaderValTooSmall)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 2nd Multi Part Header - header length value too small, header actually longer
	case ECorruptMultiMixedMsg5:
	case ECorruptMultiRelatedMsg5:
	case ECorruptMultiAlternativeMsg5:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid, sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2HeaderValTooSmall, sizeof(KHeaderPart2HeaderValTooSmall));//Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd body
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 1st Multi Part Header - header length value too large, header actually smaller
	case ECorruptMultiMixedMsg6:
	case ECorruptMultiRelatedMsg6:
	case ECorruptMultiAlternativeMsg6:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1HeaderValTooBig, sizeof(KHeaderPart1HeaderValTooBig));// Add 1st header
		temp.Append(KPart1BodyValid,sizeof(KPart1BodyValid)); // Add 1st part body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 2nd Multi Part Header - header length value too large, header actually smaller
	case ECorruptMultiMixedMsg7:
	case ECorruptMultiRelatedMsg7:
	case ECorruptMultiAlternativeMsg7:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid, sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2HeaderValTooBig, sizeof(KHeaderPart2HeaderValTooBig));
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;
	
	// 1st	Multipart header - part 1 body value too small
	case ECorruptMultiMixedMsg8:
	case ECorruptMultiRelatedMsg8:
	case ECorruptMultiAlternativeMsg8:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1BodyValTooSmall,sizeof(KHeaderPart1BodyValTooSmall)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;


	// 2nd	Multipart header - part 2 body value too small
	case ECorruptMultiMixedMsg9:
	case ECorruptMultiRelatedMsg9:
	case ECorruptMultiAlternativeMsg9:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid, sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2BodyValTooSmall, sizeof(KHeaderPart2BodyValTooSmall));
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 1st Multipart header - part 1 body value too large 
	case ECorruptMultiMixedMsg10:
	case ECorruptMultiRelatedMsg10:
	case ECorruptMultiAlternativeMsg10:
		SetHeaderBuffer(aTestCase+ECorruptMultiMixedMsg10);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1BodyValTooBig,sizeof(KHeaderPart1BodyValTooBig)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 2nd	Multipart header - part 2 body value too large 
	case ECorruptMultiMixedMsg11:
	case ECorruptMultiRelatedMsg11:
	case ECorruptMultiAlternativeMsg11:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2HeaderValTooBig, sizeof(KHeaderPart2HeaderValTooBig));
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 1st Multipart - body is smaller than the value given in the part header
	// SL message is terminated
	case ECorruptMultiMixedMsg12:
	case ECorruptMultiRelatedMsg12:
	case ECorruptMultiAlternativeMsg12:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyTooShort, sizeof(KPart1BodyTooShort));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;


	// 2nd Multipart - body is smaller than the value given in the part header
	// SI message is terminated
	case ECorruptMultiMixedMsg13:
	case ECorruptMultiRelatedMsg13:
	case ECorruptMultiAlternativeMsg13:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));
		temp.Append(KPart2BodyTooShort, sizeof(KPart2BodyTooShort));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;


	// 1st Part Body - part body longer than header length value 
	// SL message is terminated
	case ECorruptMultiMixedMsg14:
	case ECorruptMultiRelatedMsg14:
	case ECorruptMultiAlternativeMsg14:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyTooLong, sizeof(KPart1BodyTooLong));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// 2nd Part Body - part body longer than header length value 
	case ECorruptMultiMixedMsg15:
	case ECorruptMultiRelatedMsg15:
	case ECorruptMultiAlternativeMsg15:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));
		temp.Append(KPart2BodyTooLong, sizeof(KPart2BodyTooLong));// Add 2nd part hdr
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// Multipart header incorrectly specifies 3 parts when there's 2
	case ECorruptMultiMixedMsg16:
	case ECorruptMultiRelatedMsg16:
	case ECorruptMultiAlternativeMsg16:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts3, sizeof(KMultiAltNoParts3));// Copy No. parts
		temp.Append(KHeaderPart1Valid, sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// Multipart header incorrectly specifies 2 parts when there's 3
	case ECorruptMultiMixedMsg17:
	case ECorruptMultiRelatedMsg17:
	case ECorruptMultiAlternativeMsg17:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid, sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// 3rd Multipart, use 2nd again
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	
	// Multipart Push Msg header has wrong App Id
	case ECorruptMultiMixedMsg18:
	case ECorruptMultiRelatedMsg18:
	case ECorruptMultiAlternativeMsg18:  
		switch (aTestCase)
			{
			case ECorruptMultiMixedMsg18:
				iHeadersBuf.Append(KWSPHeaderMultiMixedWrongAppID, 
										sizeof(KWSPHeaderMultiAltWrongAppID));
				break;
			case  ECorruptMultiRelatedMsg18:
				iHeadersBuf.Append(KWSPHeaderMultiRelWrongAppID, 
										sizeof(KWSPHeaderMultiRelWrongAppID));
				break;
			case ECorruptMultiAlternativeMsg18:  
				iHeadersBuf.Append(KWSPHeaderMultiAltWrongAppID, 
										sizeof(KWSPHeaderMultiAltWrongAppID));
				break;
			}
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Valid,sizeof(KHeaderPart1Valid)); // Add 1st part header
		temp.Append(KPart1BodyValid, sizeof(KPart1BodyValid));// Add 1st body
		temp.Append(KHeaderPart2Valid, sizeof(KHeaderPart2Valid));// Add 2nd part hdr
		temp.Append(KPart2BodyValid, sizeof(KPart2BodyValid));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr,temp);
		break;

	// No binary data - WAP Push message body is empty		
	case ECorruptMultiMixedMsg19:
	case ECorruptMultiRelatedMsg19:
	case ECorruptMultiAlternativeMsg19:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, KNullDesC8);
		break;

	// WAP Push message body contains only a single null value		
	case ECorruptMultiMixedMsg20:
	case ECorruptMultiRelatedMsg20:
	case ECorruptMultiAlternativeMsg20:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Append(KPart1BodyNull, sizeof(KPart1BodyNull));// Add 1st body
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, temp);
		break;
		
	//Message body full of junk, no proper WAP message headers or bodies
	case ECorruptMultiMixedMsg21:
	case ECorruptMultiRelatedMsg21:
	case ECorruptMultiAlternativeMsg21:
		SetHeaderBuffer(aTestCase);
		msgGen->StartNewMsgL(CWapPushMsgGen::EEmptyMsgBody);
		temp.Copy(KMultiAltNoParts2, sizeof(KMultiAltNoParts2));// Copy No. parts
		temp.Append(KHeaderPart1Corrupt, sizeof(KHeaderPart1Corrupt)); // Add 1st part header
		temp.Append(KPart1BodyCorrupt, sizeof(KPart1BodyCorrupt));// Add 1st body
		temp.Append(KHeaderPart2Corrupt, sizeof(KHeaderPart2Corrupt));// Add 2nd part hdr
		temp.Append(KPart2BodyCorrupt, sizeof(KPart2BodyCorrupt));
		msgGen->AppendFieldL(CWapPushMsgGen::EWBXMLNoFieldStr, temp);
		break;
		
	default: 
		// do nothing
		break;
		}

	HBufC8* msg = msgGen->CreateMsgLC();
	// copy data to iBodyBuf
	iBodyBuf = msg->Des(); 
	CleanupStack::PopAndDestroy(2); // msg, msgGen;
}

void CWapPushCorruptMessageTest::SetHeaderBuffer(TUint aTestCase)
	{
	switch (aTestCase)
		{
		case ECorruptMultiMixedMsg1:
		case ECorruptMultiMixedMsg2:
		case ECorruptMultiMixedMsg3:
		case ECorruptMultiMixedMsg4:
		case ECorruptMultiMixedMsg5:
		case ECorruptMultiMixedMsg6:
		case ECorruptMultiMixedMsg7:
		case ECorruptMultiMixedMsg8:
		case ECorruptMultiMixedMsg9:
		case ECorruptMultiMixedMsg10:
		case ECorruptMultiMixedMsg11:
		case ECorruptMultiMixedMsg12:		
		case ECorruptMultiMixedMsg13:		
		case ECorruptMultiMixedMsg14:		
		case ECorruptMultiMixedMsg15:
		case ECorruptMultiMixedMsg16:			
		case ECorruptMultiMixedMsg17:		
		case ECorruptMultiMixedMsg18:		
		case ECorruptMultiMixedMsg19:
		case ECorruptMultiMixedMsg20:
		case ECorruptMultiMixedMsg21:
			iHeadersBuf.Append(KMessageHeaderMultipartMixed, sizeof(KMessageHeaderMultipartMixed));
			break;

		case ECorruptMultiRelatedMsg1:
		case ECorruptMultiRelatedMsg2:
		case ECorruptMultiRelatedMsg3:
		case ECorruptMultiRelatedMsg4:
		case ECorruptMultiRelatedMsg5:
		case ECorruptMultiRelatedMsg6:
		case ECorruptMultiRelatedMsg7:
		case ECorruptMultiRelatedMsg8:
		case ECorruptMultiRelatedMsg9:
		case ECorruptMultiRelatedMsg10:
		case ECorruptMultiRelatedMsg11:
		case ECorruptMultiRelatedMsg12:
		case ECorruptMultiRelatedMsg13:
		case ECorruptMultiRelatedMsg14:
		case ECorruptMultiRelatedMsg15:
		case ECorruptMultiRelatedMsg16:
		case ECorruptMultiRelatedMsg17:
		case ECorruptMultiRelatedMsg18:
		case ECorruptMultiRelatedMsg19:
		case ECorruptMultiRelatedMsg20:
		case ECorruptMultiRelatedMsg21:
			iHeadersBuf.Append(KWSPHeaderMultiRelValid,sizeof(KWSPHeaderMultiRelValid));
			break;

		case ECorruptMultiAlternativeMsg1:
		case ECorruptMultiAlternativeMsg2:
		case ECorruptMultiAlternativeMsg3:
		case ECorruptMultiAlternativeMsg4:
		case ECorruptMultiAlternativeMsg5:
		case ECorruptMultiAlternativeMsg6:
		case ECorruptMultiAlternativeMsg7:
		case ECorruptMultiAlternativeMsg8:
		case ECorruptMultiAlternativeMsg9:		
		case ECorruptMultiAlternativeMsg10:		
		case ECorruptMultiAlternativeMsg11:		
		case ECorruptMultiAlternativeMsg12:		
		case ECorruptMultiAlternativeMsg13:		
		case ECorruptMultiAlternativeMsg14:		
		case ECorruptMultiAlternativeMsg15:		
		case ECorruptMultiAlternativeMsg16:
		case ECorruptMultiAlternativeMsg17:
		case ECorruptMultiAlternativeMsg18:	
		case ECorruptMultiAlternativeMsg19:
		case ECorruptMultiAlternativeMsg20:		
		case ECorruptMultiAlternativeMsg21:		
			iHeadersBuf.Append(KWSPHeaderMultiAltValid,sizeof(KWSPHeaderMultiAltValid));
			break;
		}
	}

/** confirm the message has been stored in the message store.
	Writes confirmation of number of found messages to log file
	if present.
	@param void
	@return TBool
		ETrue = message(s) found
		EFalse = no message(s) found
 */
TBool CWapPushCorruptMessageTest::ConfirmMessagesSavedL()
	{
	HBufC* tempHref = HBufC::NewLC(KBufferSize);
	CSISLPushMsgUtils* wapPushUtils = CSISLPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);
	TMsvId slMsgEntryId = KMsvNullIndexEntryId;
	
	// convert KSLHref from 8 to 16 bit for FindUrlL
	for (TInt count = 0; count <3; count++)
		{
		tempHref->Des().Delete(0, KBufferSize);
		switch (count)
			{
			case 0:
				tempHref->Des().Copy(KSlHrefStr());
				break;
			case 1:
				tempHref->Des().Copy(KSlHrefStr2());
				break;
			case 2:
				tempHref->Des().Copy(KSlcHrefschmookie());
				break;
			}
		slMsgEntryId = wapPushUtils->FindUrlL(*tempHref, KUidWapPushMsgSL);
		if (slMsgEntryId)
			{
			TBuf<KPushLogBuffer> buf;
			_LIT(KLogSLId,"SL messages stored:\t 0x%08x");
			buf.Format(KLogSLId,slMsgEntryId);
			WPLPrintf(buf);	
			}
		}

	// Get Details of the multiparts saved in the Global Inbox.
	CMsvEntry* msvEntry;
	msvEntry = wapPushUtils->Session().GetEntryL(KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(msvEntry);
	CMsvEntrySelection* sel = msvEntry->ChildrenWithMtmL(KUidMtmWapPush);
	CleanupStack::PushL(sel);

	for (TInt outer =0; outer<2; outer++)
		{
		if (outer ==0)
			{
			for (TInt inner = 0; inner<sel->Count();inner++)
				{
				msvEntry->SetEntryL(sel->At(inner));
				#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
				if ( (msvEntry->Entry().iBioType == KUidWapPushMsgMultiPart.iUid) &&
					!(msvEntry->Entry().MtmData1()& EPushMsgStatusCorrupt) )
						{
						TBuf<KPushLogBuffer> buf;
						_LIT(KValidMulti,"Valid Multipart message stored:\t 0x%08x");
						buf.Format(KValidMulti,sel->At(inner));
						WPLPrintf(buf);	
						}
				#else
				if ( (msvEntry->Entry().iBioType == KUidWapPushMsgMultiPart.iUid) &&
					!(msvEntry->Entry().MtmData1()&CPushMsgEntryBase::EPushMsgStatusCorrupt) )
						{
						TBuf<KPushLogBuffer> buf;
						_LIT(KValidMulti,"Valid Multipart message stored:\t 0x%08x");
						buf.Format(KValidMulti,sel->At(inner));
						WPLPrintf(buf);	
						}
				#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
				}
			}
		else if (outer ==1)
			{
			for (TInt inner = 0; inner<sel->Count();inner++)
				{
				msvEntry->SetEntryL(sel->At(inner));
				#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
				if ( (msvEntry->Entry().iBioType == KUidWapPushMsgMultiPart.iUid) &&
					 (msvEntry->Entry().MtmData1()& EPushMsgStatusCorrupt) )
						{
						TBuf<KPushLogBuffer> buf;
						_LIT(KCorruptMulti,"Corrupt Multipart message stored:\t 0x%08x");
						buf.Format(KCorruptMulti,sel->At(inner));
						WPLPrintf(buf);	
						}
				#else
				if ( (msvEntry->Entry().iBioType == KUidWapPushMsgMultiPart.iUid) &&
					(msvEntry->Entry().MtmData1()&CPushMsgEntryBase::EPushMsgStatusCorrupt) )
						{
						TBuf<KPushLogBuffer> buf;
						_LIT(KCorruptMulti,"Corrupt Multipart message stored:\t 0x%08x");
						buf.Format(KCorruptMulti,sel->At(inner));
						WPLPrintf(buf);	
						}
				#endif //SYMBIAN_ENABLE_SPLIT_HEADERS
				}
			}

		}
	CleanupStack::PopAndDestroy(2);  // sel, msvEntry

	CleanupStack::PopAndDestroy(2);  // wapPushUtils, msgHref
	return slMsgEntryId;
	}



