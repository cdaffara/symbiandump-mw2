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

#ifndef __T_PUSHMESSAGE_H__
#define __T_PUSHMESSAGE_H__

class CWapLog;
class CPushMessage;


/**
* Test harness for the CPushMessage class.  
* 
* Creates a header and a body pair from data files, and uses
* each pair in turn to the pass to the constructor of a CPushMessage.
* On each message call number of functions to check that the class 
* holds and returns the correct data 
*/
class CPushMessageTester : public CBase
	{
public:
	static CPushMessageTester* NewLC(CWapPushLog& aWapLog);
	static CPushMessageTester* NewL(CWapPushLog& aWapLog);
	~CPushMessageTester();
	
	void CreateMessagesL();
	void OutputMessageDataL();
	void TestMultiTextMsgFromFilesL();
	void TestFinished();
private:
	CPushMessage* CreateMultiTextPushMessageL(const TDesC& aFileName, const TDesC8& aHdr);
	void GenMessageTypeL(const TDesC& aFileName, TDes8& aMsgType);
	CPushMessageTester(CWapPushLog& aWapLog);
	HBufC8* CreateMsgDataLC(const TUint8* aMessage, TInt aLength);
private:
	CWapPushLog& iLog;  //Don't own this
	CPushMessage* iPushMessage1;
	CPushMessage* iPushMessage2;
	CPushMessage* iPushMessage3;
	CPushMessage* iPushMessage4;
	CPushMessage* iPushMessage5;
	CPushMessage* iPushMessage6;
	CPushMessage* iPushMessage7;
	CPushMessage* iPushMultiMessage;
	};








const TUint8 KMessageHeader1[] =
	{
	0x80	//Content type is "*/*", No application id
	};

const TUint8 KMessageHeader2[] = 
	{
	0xAF,  //Content type "text/vnd.wap.sl"
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
	0x83   // Short int - 0x03 
	};

const TUint8 KMessageHeader3[]=
	{
	0xB4,  //Content type "text/vnd.wap.wml"
	0x94,  // Date
	0x04,  // length of field
	0x35,  // Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0xAF,	// X-Wap-Application ID
	0x83	// Id is short integer 3.
	};

const TUint8 KMessageHeader4[]=
	{
	0xB4,  //Content type "text/vnd.wap.wml"
	0x94,  // Date
	0x04,  // length of field
	0x35,  // Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0xAF,	// X-Wap-Application ID
	0x77,	// 'w'
	0x77,	// 'w'
	0x77,	// 'w'
	0x2E,   // '.'
	0x62,	// 'b'
	0x6C,	// 'l'
	0x6F,	// 'o'
	0x67,	// 'g'
	0x67,	// 'g'
	0x73,	// 's'
	0x2E,	// '.'
	0x63,	// 'c'
	0x6F,	// 'o;
	0x6D,	// 'm'
	0x2F,	// '/'
	0x69,   // 'i'
	0x6E,	// 'n'
	0x64,	// 'd
	0x65,	// 'e'
	0x78,   // 'x'
	0x2E,	// '.'
	0x77,	// 'w
	0x6D,	// 'm'
	0x6C	// 'l'
	};

const TUint8 KMessageHeaderMultipart[]=
	{
	0xA3,  //Content type "application/vnd.wap.multipart.mixed"
	0x94,  // Date
	0x04,  // length of field
	0x35,  // Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0xAF,	// X-Wap-Application ID
	0x77,	// 'w'
	0x77,	// 'w'
	0x77,	// 'w'
	0x2E,   // '.'
	0x62,	// 'b'
	0x6C,	// 'l'
	0x6F,	// 'o'
	0x67,	// 'g'
	0x67,	// 'g'
	0x73,	// 's'
	0x2E,	// '.'
	0x63,	// 'c'
	0x6F,	// 'o;
	0x6D,	// 'm'
	0x2F,	// '/'
	0x69,   // 'i'
	0x6E,	// 'n'
	0x64,	// 'd
	0x65,	// 'e'
	0x78,   // 'x'
	0x2E,	// '.'
	0x77,	// 'w
	0x6D,	// 'm'
	0x6C	// 'l'
	};

const TUint8 KMessageBodyMultipart[]=
	{
	0x01,  // number of parts
	0x0A,  // part 1 header size
	0x1D,  // part 1 body size
	0xB0,  // content type application/vnd.wap.slc ... the headers:
	0x94,  // Date
	0x04,  // length of field
	0x35,  // Thur 23 Apr 1998 13:41:37 GMT
	0x3F,	
	0x45,
	0x11,
	0x3F,	
	0x45,
	0x11,
	0x01,	// Body - slc (mmm.schookie.com)
	0x06,
	0x6A,
	0x00,
	0x85,
	0x09,
	0x03,
	0x6D,
	0x6D,
	0x6D,
	0x2E,
	0x73,
	0x63,
	0x68,
	0x6D,
	0x6F,
	0x6F,
	0x6B,
	0x69,
	0x65,
	0x2E,
	0x63,
	0x6F,
	0x64,
	0x65,
	0x2F,
	0x00,
	0x06,
	0x01
	};

const TUint8 KMessageHeader5[] = 
	{
	'm',	// Text content type 'my/type' with null terminator
	'y',
	'/',
	't',
	'y',
	'p',
	'e',
	0x00,
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
	0x83   // Short int - 0x03 
	};

const TUint8 KMessageHeader6[] = 
	{
	0x1F,	// <Octet 31> to indicate UIntVar encoding
	0x2B,	// UIntVar encoding for 43
	'a',
	'p',
	'p',
	'l',
	'i',
	'c',
	'a',
	't',
	'i',
	'o',
	'n',
	'/',
	'x',
	'-',
	'w',
	'a',
	'p',
	'-',
	'p',
	'r',
	'o',
	'v',
	'.',
	'b',
	'r',
	'o',
	'w',
	's',
	'e',
	'r',
	'-',
	'b',
	'o',
	'o',
	'k',
	'm',
	'a',
	'r',
	'k',
	's',
	0x00,	// Null terminator
	0x81,	// parameter
	0xDE,	
	0x92,  // Date
	0x04,  // length of field
	0x39,  // Monday 15 August 2000 10:41:37 GMT
	0x90,	
	0x6a,
	0xd0
	};

const TUint8 KMessageHeader7[] = 
	{
	0x1F,	// <Octet 31> to indicate UIntVar encoding
	0x01,	// UIntVar encoding for 1
	0xAF,  //Content type "text/vnd.wap.sl"
	0x92,  // Date
	0x04,  // length of field
	0x39,  // Monday 15 August 2000 10:41:37 GMT
	0x90,	
	0x6a,
	0xd0,
	};

const TUint8 KMessageHeader8[] = 
	{
	0x1F,	// <Octet 31> to indicate UIntVar encoding
	0x81,	// UIntVar encoding for 130 (1000 0001 0000 0010)
	0x02,
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'.',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'.',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'a',
	'/',
	'm',
	'y',
	'l',
	'o',
	'n',
	'g',
	't',
	'y',
	'p',
	'e',
	0x00,	// NULL terminator
	0x92,  // Date
	0x04,  // length of field
	0x39,  // Monday 15 August 2000 10:41:37 GMT
	0x90,	
	0x6a,
	0xd0
	};

_LIT8(KShortPushMessageBody, "A dummy WAP Push message");
_LIT8(KLongPushMessageBody, "A very very very long dummy message body for a WAP Push Message sent by a server somewhere"); 
_LIT8(KPushMessageServerAddress, "c:\\www.zabruga.co.uk");

#endif
