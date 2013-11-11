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
// Definitions for use in the Compact Business Card parser.
// 
//

#if !defined (__CBCPDEF_H__)
#define __CBCPDEF_H__

// Compact Business Card parser UID.
const TUid KUidMsvBIOBusinessCardParserDLL = {0x1000526B};

// Parse constants
#define KCharColon          ':'
#define KCharComma		    ','
#define KCharSlash			'/'
#define KCharCarriageReturn '\r'

_LIT(KCharSemiColon,";");
_LIT(KVCardExt,".vcf");

// Header fields defined in Nokia Smart Messaging Specification grammar (3.3.1).
// N.B. If fields are modified then PrintFieldToRichTextL() will need to be changed accordingly.
_LIT(KBusinessCardHeader,"Business Card");
_LIT(KHeaderName, "Name");
_LIT(KHeaderCompany,"Company");
_LIT(KHeaderTitle,"Title");
#define KTelOffset 4
_LIT(KHeaderPhone,"Phone");
_LIT(KHeaderFax,"Fax");
_LIT(KHeaderEmail,"Email");
#define KNumberOfAddressLines 5
_LIT(KHeaderStreet1,"Street1");
_LIT(KHeaderStreet2,"Street2");
_LIT(KHeaderStreet3,"Street3");
_LIT(KHeaderStreet4,"Street4");
_LIT(KHeaderStreet5,"Street5");
_LIT(KHeaderExtra,"Extra lines");
_LIT(KInitialFieldValue,"");				// For initialising newly created fields.

// vCard fields.
// These are all the vCard fields for which we can extract data from a Compact Business Card.
_LIT(KVCardHeader,"BEGIN:VCARD");
_LIT(KVCardFormattedName,"\r\nFN:");
_LIT(KVCardName,"\r\nN:");
_LIT(KVCardAddress,"\r\nADR:");
_LIT(KVCardNote,"\r\nNOTE:");
_LIT(KVCardTel,"\r\nTEL;VOICE:");
_LIT(KVCardTelCell,"\r\nTEL;CELL:");
_LIT(KVCardFax,"\r\nTEL;FAX:");
_LIT(KVCardFaxCell,"\r\nTEL;FAX;CELL:");
_LIT(KVCardEmail,"\r\nEMAIL;INTERNET:");
_LIT(KVCardTitle,"\r\nTITLE:");
_LIT(KVCardOrg,"\r\nORG:");
_LIT(KVCardVersion,"\r\nVERSION:2.1");
_LIT(KVCardClass,"\r\nCLASS:PUBLIC");
_LIT(KVCardFooter,"\r\nEND:VCARD\r\n");


// Prefix added to indicate a cellphone number
_LIT(KVPrefixCell,"(GSM)");


#define KUidCharsetISO88591         0x10003b10
#define	KServerIdModVal				512


#endif //__CBCPDEF_H__
