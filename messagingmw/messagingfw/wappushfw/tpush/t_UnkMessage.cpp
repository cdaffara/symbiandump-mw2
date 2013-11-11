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
//

#include <msvids.h>
#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"
#include <push/cwappushmsgutils.h>

#ifdef __MESSAGING_API_V2__
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#endif // __MESSAGING_API_V2__

_LIT8(KBIOLine1,	"begin:vcard\r\n");
_LIT8(KBIOLine2,	"n:Blumin;J\r\n");
_LIT8(KBIOLine3,	"tel;cell:+44 774 706 5443\r\n");
_LIT8(KBIOLine4,	"x-mozilla-html:TRUE\r\n");
_LIT8(KBIOLine5,	"org:Pure Matrix;Engineering\r\n");
_LIT8(KBIOLine6,	"adr:;;;;;;\r\n");
_LIT8(KBIOLine7,	"version:2.1\r\n");
_LIT8(KBIOLine8,	"email;internet:julia.blumin@symbian.com\r\n");
_LIT8(KBIOLine9,	"title:engneer\r\n");
_LIT8(KBIOLine10,	"end:vcard\r\n");

_LIT8(KUnkLine1,	"just some random text for the unknown message\r\n");
_LIT8(KUnkLine2,	"body though text will may no influence on how stored.\r\n");
_LIT8(KUnkLine3,	"Test wap push plugin leaving from HandleMessageL\r\n");
//_LIT8(KCrLf,		"\r\n");

_LIT8(KWMLTextLine1,	"<?xml version=\"1.0\" ?>\r\n");
_LIT8(KWMLTextLine2,	"<!DOCTYPE wml PUBLIC \"-//WAPFORUM//DTD WML 1.2//EN\"\r\n");
_LIT8(KWMLTextLine3,	"\"http://www.wapforum.org/DTD/wml12.dtd\">\r\n");
_LIT8(KWMLTextLine4,	"<wml>\r\n");
_LIT8(KWMLTextLine5,	"<template>\r\n");
_LIT8(KWMLTextLine6,	"<do label=\"TOP\" type=\"accept\" name=\"soft1\">\r\n");
_LIT8(KWMLTextLine7,	"<go href=\"../index.wml#menu\"/>\r\n");
_LIT8(KWMLTextLine8,	"</do>\r\n");
_LIT8(KWMLTextLine9,	"<do label=\"Back\" type=\"prev\" name=\"soft2\">\r\n");
_LIT8(KWMLTextLine10,	"<prev/>\r\n");
_LIT8(KWMLTextLine11,	"</do>\r\n");
_LIT8(KWMLTextLine12,	"</template>\r\n");
_LIT8(KWMLTextLine13,	"<card id=\"index\">\r\n");
_LIT8(KWMLTextLine14,	"<p>\r\n");
_LIT8(KWMLTextLine15,	"<big><b>WML Sample</b></big>\r\n");
_LIT8(KWMLTextLine16,	"</p>\r\n");
_LIT8(KWMLTextLine17,	"<p>\r\n");
_LIT8(KWMLTextLine18,	"<select multiple=\"true\" value=\"\">\r\n");
_LIT8(KWMLTextLine19,	"<option onpick=\"text.wml\">Text Formats</option>\r\n");
_LIT8(KWMLTextLine20,	"<option onpick=\"control.wml\">Controls</option>\r\n");
_LIT8(KWMLTextLine21,	"<option onpick=\"script.wml\">WMLScript</option>\r\n");
_LIT8(KWMLTextLine22,	"</select>\r\n");
_LIT8(KWMLTextLine23,	"</p>\r\n");
_LIT8(KWMLTextLine24,	"</card>\r\n");
_LIT8(KWMLTextLine25,	"</wml>\r\n");
					
_LIT(KUnkTestStart, " ******** Wap Push Unknown Message Test - START ******** ");
_LIT(KUnkTestFinish, " ******** Wap Push Unknown Message Test - FINISH ******** ");
_LIT(KSpace, " ");
_LIT(KDefaultFilename, "content.dat");
_LIT(KContentWml, "content.wml");
_LIT(KCase5, "case5.txt");
_LIT(KCase6, "case6.txt");
_LIT(KCase7, "case7.txt");
_LIT(KCase8, "case8.txt");
_LIT(KCase9, "case9.txt");
_LIT(KTest1, "Test Case 1");
_LIT(KTest2, "Test Case 2");
_LIT(KTest3, "Test Case 3");
_LIT(KTest4, "Test Case 4");
_LIT(KTest5, "Test Case 5");
_LIT(KTest6, "Test Case 6");
_LIT(KTest7, "Test Case 7");
_LIT(KTest8, "Test Case 8");
_LIT(KTest9, "Test Case 9");
_LIT(KLogSucceeded, "OK");
_LIT(KLogFailed, "FAILED");
_LIT(KLogOutput,"WapPushUnknownMessageTest: %S - %S");


/** set test name to 'Unknown Handler Test'
	@param void
	@return string
		string represents test name
*/
const TDesC& CWapPushUnkMessageTest::TestName()
{
	_LIT(KTextUnkMessageTest, "Unknown Handler Test");
	return KTextUnkMessageTest;
}

/** set the number of test messages to use */
void CWapPushUnkMessageTest::NumberTestCasesToRun()
{
	iNumberOfTestCases = 11; 
}

/** build different test messages to test handler
	@param TInt
		the number of test messages to create and use
	@return void
*/
void CWapPushUnkMessageTest::PrepareTestMessageL(TInt aTestCase)
	{
	// this set of test messages adds a test message
	// to the store for the match cases, 
	// replaces the stored message and ends with no stored si message 	
		
	iBodyBuf.Delete(0,KBufferSize);
	//iBodyBuf.Zero();
	iHeadersBuf.Delete(0, KBufferSize);
	
	
	switch (aTestCase)
		{
	case 0 : 
		{
		// create vcard header
		const TUint8 kHdr[]=
			{
			0x87,		 // content type text/x-vcard (bio msg)
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  //X-Wap-Application-ID
			0x82  // Short int - 0x02 represent x-wap-application:push.ua
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create vcard message and approriate text body
		iBodyBuf.Append(KBIOLine1);
		iBodyBuf.Append(KBIOLine2);
		iBodyBuf.Append(KBIOLine3);
		iBodyBuf.Append(KBIOLine4);
		iBodyBuf.Append(KBIOLine5);
		iBodyBuf.Append(KBIOLine6);
		iBodyBuf.Append(KBIOLine7);
		iBodyBuf.Append(KBIOLine8);
		iBodyBuf.Append(KBIOLine9);
		iBodyBuf.Append(KBIOLine10);
		break;
		}
	case 1 : 
		// create unknown message 
		{
		const TUint8 kHdr[]=
			{
			0xFF,  // ? unknown
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  //X-Wap-Application-ID
			0x82   // Short int - 0x02 represent x-wap-application:push.ua
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 2:
		{		
		const TUint8 kHdr[]=
			{
			0x88,  // content type "text/vnd.wap.wml" // 0x08
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  //X-Wap-Application-ID
			0x82   // Short int - 0x02 represent x-wap-application:push.ua
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create vcard message and approriate text body
		iBodyBuf.Append(KWMLTextLine1);
		iBodyBuf.Append(KWMLTextLine2);
		iBodyBuf.Append(KWMLTextLine3);
		iBodyBuf.Append(KWMLTextLine4);
		iBodyBuf.Append(KWMLTextLine5);
		iBodyBuf.Append(KWMLTextLine6);
		iBodyBuf.Append(KWMLTextLine7);
		iBodyBuf.Append(KWMLTextLine8);
		iBodyBuf.Append(KWMLTextLine9);
		iBodyBuf.Append(KWMLTextLine10);
		iBodyBuf.Append(KWMLTextLine11);
		iBodyBuf.Append(KWMLTextLine12);
		iBodyBuf.Append(KWMLTextLine13);
		iBodyBuf.Append(KWMLTextLine14);
		iBodyBuf.Append(KWMLTextLine15);
		iBodyBuf.Append(KWMLTextLine16);
		iBodyBuf.Append(KWMLTextLine17);
		iBodyBuf.Append(KWMLTextLine18);
		iBodyBuf.Append(KWMLTextLine19);
		iBodyBuf.Append(KWMLTextLine20);
		iBodyBuf.Append(KWMLTextLine21);
		iBodyBuf.Append(KWMLTextLine22);
		iBodyBuf.Append(KWMLTextLine23);
		iBodyBuf.Append(KWMLTextLine24);
		iBodyBuf.Append(KWMLTextLine25);
		break;
		}
/*	case 3 : 
		// create unknown message
		{
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition (WAP 1.1)
			0x02,  // Length (272 bytes = 0x0110)
			0x01,
			0x10,
			'i',   // disposition - inline
			'n',
			'l',
			'i',
			'n',
			'e',
			0x00,  // parameter - filename
			'f',
			'i',
			'l',
			'e',
			'n',
			'a',
			'm',
			'e',
			0x00,
			'1',   // parameter value - 255 character file name.
			'2',
			'3',
			'4',
			'5',
			'6',
			'7',
			'8',
			'9',
			'0',
			'1','2','3','4','5','6','7','8','9','0',  //20
			'1','2','3','4','5','6','7','8','9','0',  //30
			'1','2','3','4','5','6','7','8','9','0',  //40
			'1','2','3','4','5','6','7','8','9','0',  //50
			'1','2','3','4','5','6','7','8','9','0',  //60
			'1','2','3','4','5','6','7','8','9','0',  //70
			'1','2','3','4','5','6','7','8','9','0',  //80
			'1','2','3','4','5','6','7','8','9','0',  //90
			'1','2','3','4','5','6','7','8','9','0',  //100
			'1','2','3','4','5','6','7','8','9','0',  //110
			'1','2','3','4','5','6','7','8','9','0',  //120
			'1','2','3','4','5','6','7','8','9','0',  //130
			'1','2','3','4','5','6','7','8','9','0',  //140
			'1','2','3','4','5','6','7','8','9','0',  //150
			'1','2','3','4','5','6','7','8','9','0',  //160
			'1','2','3','4','5','6','7','8','9','0',  //170
			'1','2','3','4','5','6','7','8','9','0',  //180
			'1','2','3','4','5','6','7','8','9','0',  //190
			'1','2','3','4','5','6','7','8','9','0',  //200
			'1','2','3','4','5','6','7','8','9','0',  //210
			'1','2','3','4','5','6','7','8','9','0',  //220
			'1','2','3','4','5','6','7','8','9','0',  //230
			'1','2','3','4','5','6','7','8','9','0',  //240
			'1','2','3','4','5','6','7','8','9','0',  //250
			'1','2','3','4','5',                      //255
			0x00
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
*/	case 3 : 
		// create unknown message
		{
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition (WAP 1.1)
			0x1B,  // Length
			'i',   // disposition - inline
			'n',
			'l',
			'i',
			'n',
			'e',
			0x00,  // parameter - filename
			'f',
			'i',
			'l',
			'E',
			'N',
			'A',
			'm',
			'e',
			0x00,
			'"',   // parameter value - ca?e3.txt - invalid Symbian OS file name.
			'c',
			'a',
			'?',
			'e',
			'3',
			'.',
			't',
			'x',
			't',
			0x00
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 4 : 
		// create unknown message  
		{
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition
			'i',   // inline; name="bob";filename = case4.txt ;size=100 
			'n',
			'l',
			'i',
			'n',
			'e',
			';',
			' ',
			'n',
			'a',
			'm',
			'e',
			'=',
			'b',
			'o',
			'b',
			';',
			'F',
			'i',
			'l',
			'E',
			'n',
			'a',
			'M',
			'e',
			' ',
			'=',
			' ',
			'c',
			'a',
			's',
			'e',
			'4',
			'.',
			't',
			'x',
			't',
			' ',
			';',
			's',
			'i',
			'z',
			'e',
			'=',
			'1',
			'0',
			'0',
			0x0D, //CR
			0x0A  //LF
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 5 : 
		// create unknown message 
		{
		// Create test unknown push message with encoded wap-230-wsp-20010705-a section 8.4.2.53 Content-Disposition header.
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition header
			0x0C,  // Value-length (number of bytes to follow)
			0x80,  // Disposition - Form-data
			0x06,  // Parameter - well-known-parameter-token (WAP 1.1 filename)
			'c',   // Parameter value - text-string (case5.txt)
			'a',
			's',
			'e',
			'5',
			'.',
			't',
			'x',
			't',
			0x00
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 6 : 
		// create unknown message 
		{
		// Create test unknown push message with encoded wap-230-wsp-20010705-a section 8.4.2.53 Content-Disposition header.
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition header
			0x0E,  // Value-length (number of bytes to follow)
			0x80,  // Disposition - Form-data
			0x03,  // Parameter - well-known-parameter-token (WAP 1.1 type)
			0x81,  // value 1 (with msb set to 1)
			0x06,  // Parameter - well-known-parameter-token (WAP 1.1 filename)
			'c',   // Parameter value - text-string (case6.txt)
			'a',
			's',
			'e',
			'6',
			'.',
			't',
			'x',
			't',
			0x00
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 7 : 
		// create unknown message 
		{
		// Create test unknown push message with encoded wap-230-wsp-20010705-a section 8.4.2.53 Content-Disposition header.
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition header
			0x10,  // Value-length (number of bytes to follow)
			0x80,  // Disposition - Form-data
			0x03,  // Parameter - well-known-parameter-token (WAP 1.1 type)
			0x02,  // Parameter value - long-integer (0xFFFF)
			0xFF,  // msb
			0xFF,  // lsb
			0x06,  // Parameter - well-known-parameter-token (WAP 1.1 filename)
			'c',   // Parameter value - text-string (case6.txt)
			'a',
			's',
			'e',
			'7',
			'.',
			't',
			'x',
			't',
			0x00
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 8 : 
		// create unknown message 
		{
		// Create test unknown push message with encoded wap-230-wsp-20010705-a section 8.4.2.53 Content-Disposition header.
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition header
			0x14,  // Value-length (number of bytes to follow)
			0x80,  // Disposition - Form-data
			't',   // Parameter - (type)
			'y',
			'p',
			'e',
			0x00,
			0x02,  // Parameter value - long-integer (0xFFFF)
			0xFF,  // msb
			0xFF,  // lsb
			0x06,  // Parameter - well-known-parameter-token (WAP 1.1 filename)
			'c',   // Parameter value - text-string (case6.txt)
			'a',
			's',
			'e',
			'8',
			'.',
			't',
			'x',
			't',
			0x00
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 9 : 
		// create unknown message 
		{
		// Create test unknown push message with encoded wap-230-wsp-20010705-a section 8.4.2.53 Content-Disposition header.
		const TUint8 kHdr[]=
			{
			0xFF,  // unknown, default content type "application/octet-stream"
			0x92,  // Date
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x90,	
			0x6a,
			0xd0,
			0x94,  // Expires 
			0x04,  // length of field
			0x39,  // Monday 15 August 2000 10:41:37 GMT
			0x9a,	
			0x6a,
			0xd0,
			0xAF,  // X-Wap-Application-ID
			0x82,  // Short int - 0x02 represent x-wap-application:push.ua
			0x2E,  // Content-Disposition header
			0x1C,  // Value-length (number of bytes to follow)
			0x80,  // Disposition - Form-data
			't',   // Parameter - (type)
			'y',
			'p',
			'e',
			0x00,
			0x02,  // Parameter value - long-integer (0xFFFF)
			0xFF,  // msb
			0xFF,  // lsb
			'F',   // Parameter - (filename)
			'i',
			'L',
			'e',
			'n',
			'a',
			'M',
			'e',
			0x00,
			'c',   // Parameter value - text-string (case6.txt)
			'a',
			's',
			'e',
			'9',
			'.',
			't',
			'x',
			't',
			0x00
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine1);
		iBodyBuf.Append(KUnkLine2);
		break;
		}
	case 10:
		{
		// Dummy header / body. This will just load the test wap push plugin
		// which leaves from HandleMessageL
		// Test case added for DEF083590
		const TUint8 kHdr[]=
			{
			0xCE,
			0xAF,
			0x8D
			};
		iHeadersBuf.Append(kHdr, sizeof(kHdr));
		// create text body for message 
		iBodyBuf.Append(KUnkLine3);
		break;
		}
	default: 
		// do nothing
		break;
		}
	}

void CWapPushUnkMessageTest::TestAndPrint(const TDesC& aTestCase, const TDesC& aFilename, const TDesC& aLocalFilename)
	{
	TBuf<KPushLogBuffer> buffer;
	if (aLocalFilename.CompareF(aFilename) == 0)
		{
		buffer.Format(KLogOutput, &aTestCase, &KLogSucceeded);
		}
	else
		{
		buffer.Format(KLogOutput, &aTestCase, &KLogFailed);
		}
	WPLPrintf(buffer);
	}

/** confirm the message has been stored.
	Writes the number of stored messages found to the log file - if present.
	@param void
	@return TBool
		returns ETrue if messages found
		returns EFalse if no messages found 
*/
TBool CWapPushUnkMessageTest::ConfirmMessagesSavedL()
	{
	WPLPrintf(KSpace);
	WPLPrintf(KUnkTestStart);
	WPLPrintf(KSpace);

#ifndef __MESSAGING_API_V2__
	RFs fs;
	User::LeaveIfError(fs.Connect());	
	CleanupClosePushL(fs);
#endif // __MESSAGING_API_V2__

	CWapPushMsgUtils* wapPushUtils = CWapPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);	

	CMsvEntry* msvEntry = wapPushUtils->Session().GetEntryL(KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PushL(msvEntry);

	CMsvEntrySelection* selection = msvEntry->ChildrenL();
	CleanupStack::PushL(selection);

	TInt count = selection->Count();
	TInt testCase = 1;
	for (TInt curChild = 0; curChild < count; curChild++)
		{
		msvEntry->SetEntryL(selection->At(curChild));

		if (msvEntry->Entry().iMtm == KUidMtmWapPush && msvEntry->Entry().iBioType == KUidWapPushMsgUnknown.iUid)
			{
#ifdef __MESSAGING_API_V2__
			CMsvStore* store = msvEntry->ReadStoreL();
			CleanupStack::PushL(store);
			MMsvAttachmentManager& manager = store->AttachmentManagerL();
			for( TInt ii=0; ii<manager.AttachmentCount(); ++ii )
				{
				CMsvAttachment* attachment = manager.GetAttachmentInfoL(ii);
				switch (testCase)
					{
					case 1:
						{
						TestAndPrint(KTest1, KDefaultFilename, attachment->AttachmentName());
						testCase=2;
						} break;
					case 2:
						{
						TestAndPrint(KTest2, KContentWml, attachment->AttachmentName());
						testCase=3;
						} break;
					case 3:
						{
						TestAndPrint(KTest3, KDefaultFilename, attachment->AttachmentName());
						testCase=4;
						} break;
					case 4:
						{
						TestAndPrint(KTest4, KDefaultFilename, attachment->AttachmentName());
						testCase=5;
						} break;
					case 5:
						{
						TestAndPrint(KTest5, KCase5, attachment->AttachmentName());
						testCase=6;
						} break;
					case 6:
						{
						TestAndPrint(KTest6, KCase6, attachment->AttachmentName());
						testCase=7;
						} break;
					case 7:
						{
						TestAndPrint(KTest7, KCase7, attachment->AttachmentName());
						testCase=8;
						} break;
					case 8:
						{
						TestAndPrint(KTest8, KCase8, attachment->AttachmentName());
						testCase=9;
						} break;
					case 9:
						{
						TestAndPrint(KTest9, KCase9, attachment->AttachmentName());
						testCase=10;
						} break;
					default:
						break;
					} // end of switch 
				delete attachment;
				}
			
			CleanupStack::PopAndDestroy(store);
			}
		}
	CleanupStack::PopAndDestroy(3, wapPushUtils); // selection, msvEntry, wapPushUtils
#else
			TFileName filePath;
			msvEntry->GetFilePath(filePath);
			User::LeaveIfError(fs.SetSessionPath(filePath));	

			CDir* dirList = NULL;		
			User::LeaveIfError(fs.GetDir(filePath, KEntryAttNormal, ESortByName, dirList));

			if (dirList)
				{
				TFileName localFile;
				for (TInt i=0; i< dirList->Count(); i++)
					{
					localFile = (*dirList)[i].iName;

					switch (testCase)
						{
						case 1:
							{
							TestAndPrint(KTest1, KDefaultFilename, localFile);
							testCase=2;
							} break;
						case 2:
							{
							TestAndPrint(KTest2, KContentWml, localFile);
							testCase=3;
							} break;
						case 3:
							{
							TestAndPrint(KTest3, KDefaultFilename, localFile);
							testCase=4;
							} break;
						case 4:
							{
							TestAndPrint(KTest4, KDefaultFilename, localFile);
							testCase=5;
							} break;
						case 5:
							{
							TestAndPrint(KTest5, KCase5, localFile);
							testCase=6;
							} break;
						case 6:
							{
							TestAndPrint(KTest6, KCase6, localFile);
							testCase=7;
							} break;
						case 7:
							{
							TestAndPrint(KTest7, KCase7, localFile);
							testCase=8;
							} break;
						case 8:
							{
							TestAndPrint(KTest8, KCase8, localFile);
							testCase=9;
							} break;
						case 9:
							{
							TestAndPrint(KTest9, KCase9, localFile);
							testCase=10;
							} break;
						default:
							break;
						}
					}
				}
			delete dirList;
			}
		}
	CleanupStack::PopAndDestroy(4, &fs);
#endif // __MESSAGING_API_V2__

	WPLPrintf(KSpace);
	WPLPrintf(KUnkTestFinish);
	WPLPrintf(KSpace);
	return ETrue;
	}
