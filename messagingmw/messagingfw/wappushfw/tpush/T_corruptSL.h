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

#ifndef __T_corruptSL_h__
#define __T_corruptSL_h__

//  -------------------------------------------
//				Binary WAP Headers
//  ---------------------------------------------

// Perfectly valid header for a text based SL message
// Handlers should have no problems with this, but will if body content is not WML
const TUint8 KSlMsgValidHeader[] = 
	{
	//0xB0,  //Content type "application/vnd.wap.slc"
	0xAF,  //Content type "application/vnd.wap.sl" 
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

// Valid Header except for the Application ID which is 0x19 - unknown.  Message
// should be passed to the Unknown App Handler
const TUint8 KSlMsgHeaderWrongAppId[] = 
	{
	//0xB0,  //Content type "application/vnd.wap.slc"
	0xAF,  //Content type "application/vnd.wap.sl" 
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
	0x99  // Short int - 0x19 represent unknown x-wap-application - should be passed to 
		  // Unknown App Handler.
	};

// Valid header with content type = binary SL message
// Handlers should have no problems with this, only if body content doesn't match
const TUint8 KSlcMsgValidHeader[] = 
	{
	0xB0,  //Content type "application/vnd.wap.slc"
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

// Invalid header with content type = binary SL message, but spoof App Id
// Handlers should have no problems with this, only if body content doesn't match
const TUint8 KSlcMsgHeaderWrongAppId[] = 
	{
	0xB0,  //Content type "application/vnd.wap.slc"
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
	0x99  // Short int - 0x99 represent unregistered application
	};

// Invalid header for a text based SL message - has no content type
const TUint8 KSlMsgValidHeaderNoContent[] = 
	{
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



// ---------------------------------------------------
//				Descriptor constants
// ---------------------------------------------------

//	*****  VALID XML DATA & TAGS  *****

_LIT8(KHRefFieldTagStr, "href=\"");
_LIT8(KActionField, "action=\"");
_LIT8(KSlActionStr, "execute-high");
_LIT8(KSlHrefStr , "http://www.xyz.com/ppaid/123/abc.wml");
_LIT8(KSlHrefStr2, "http://www.xyz.com/ppaid/100/aaa.wml");
_LIT8(KSlStartTag, "<!DOCTYPE sl PUBLIC \"-//WAPFORUM//DTD SL 1.0//EN\"\r\n\"http://www.wapforum.org/DTD/sl.dtd\">\r\n<sl ");
_LIT8(KSlEndTag, ">\r\n</sl>\r\n");
_LIT8(KDoubleQuoteTag,"\"");

// 16 bit used for checking if entry exists
_LIT16(KSlcHrefschmookie, "http://mmm.schmookie.code/"); 

//   *****  INVALID XML TAGS & DATA  *****
_LIT8(KSLDummyXMLElement, "dummy=\"Some dummy data\"");
_LIT8(KSLActionErrorStr, "xcute-high");
_LIT8(KSlHrefErrorStr , "ttp:/qqq.xyz.com/ppaid/123/abc.wml"); // misspelt HTTP Ref.

// Following refers to non-existent DTD - should generate error in XML parser
_LIT8(KBadDTDSlStart, "<!DOCTYPE nogood PUBLIC \"-//WAPFORUM//DTD NOGOOD 1.0//EN\"\r\n\"http://www.wapforum.org/DTD/nogood.dtd\">\r\n<nogood ");
// termination for nogood document
_LIT8(KNogoodEnd, ">\r\n</nogood>\r\n");

// Syntax error in this string
_LIT8(KBadTagSlStart, "<!DOCTPE sl PUBLIC \"-//WAPFORUM//DTD SL 1.0//EN\"\r\n\"http://www.wapforum.org/DTD/sl.dtd\">\r\n<sl ");


// Binary content  -  Valid SLC message
const TUint8 KValidSLCMessageBody[]=
	{// Body - slc (www.schmookie.com)
	0x01, 0x06, 0x6A, 0x00, 0x85, 0x09, 
	0x03, 0x6D, 0x6D, 0x6D, 0x2E, 0x73, 
	0x63, 0x68, 0x6D, 0x6F, 0x6F, 0x6B,
	0x69, 0x65, 0x2E, 0x63, 0x6F, 0x64,
	0x65, 0x2F, 0x00, 0x06, 0x01
	};

// Binary SL Content XML version 2.0 -  unsupported
const TUint8 KSLCMsgBodyWrongXMLVersion[]=
	{	// Body - slc (www.schmookie.com)
   //XML ver
	0x10,  0x06,  0x6A,  0x00,  0x85,
	0x09,  0x03,  0x6D,  0x6D,  0x6D,
	0x2E,  0x73,  0x63,  0x68,  0x6D,
	0x6F,  0x6F,  0x6B,  0x69,  0x65,
	0x2E,  0x63,  0x6F,  0x64,  0x65,
	0x2F,  0x00,  0x06,  0x01
	};

// Binary content  -  Invalid SLC message HRef missing
const TUint8 KSLCMsgBodyNoUri[]=
	{
	// Body - slc (www.schmookie.com)
	0x01, 0x06, 0x6A, 0x00, 0x85, 0x06, 0x01
	};


// Binary content  -  Valid SLC message - No action
const TUint8 KSLCMsgBodyNoAction[]=
	{
	// Body - slc (www.schmookie.com)
	0x01, 0x06, 0x6A, 0x00, 0x85, 0x09, 0x03,
	0x6D, 0x6D, 0x6D, 0x2E, 0x73, 0x63, 0x68,
	0x6D, 0x6F, 0x6F, 0x6B, 0x69, 0x65, 0x2E,
	0x63, 0x6F, 0x64, 0x65, 0x2F, 0x00, 0x01
	};

// Binary content  -  Corrupt SLC message uses undefined 0x0D
const TUint8 KCorruptSLCMsgBody[]=
	{// Body - slc (www.schmookie.com)
	0x01, 0x06, 0x6A, 0x00, 0x85, 0x0D, 
	0x03, 0x6D, 0x6D, 0x6D, 0x2E, 0x73, 
	0x63, 0x68, 0x6D, 0x6F, 0x6F, 0x6B,
	0x69, 0x65, 0x2E, 0x63, 0x6F, 0x64,
	0x65, 0x2F, 0x00, 0x06, 0x01
	};

// Binary content  -  Corrupt SLC message end of message missing
// String unterminated.
const TUint8 KUnterminatedSLCMsgBody[]=
	{// Body - slc (www.schmookie.com)
	0x01, 0x06, 0x6A, 0x00, 0x85, 0x09, 
	0x03, 0x6D, 0x6D, 0x6D, 0x2E, 0x73, 
	0x63, 0x68, 0x6D, 0x6F, 0x6F, 0x6B,
	};
// Binary content contains just one character - NULL
// prevents the WbxmlLib.dll blowing up
const  TUint8 KNullSLCMsgBody[] = 
	{
	0x00
	};


#endif
