// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Definitions of WBXML message tokens for use in the Wap  Parser, WAPP
// 
//

#if !defined (__WAPPDEF_H__)
#define __WAPPDEF_H__

// *********      GENERAL WBXML TOKENS       ***************
// WBXML global tokens - applicable to all messages
// See WAP Binary XML Content Format, 16-Jun-99, p16.  
// These and our message specific tokens define our DTD

// As our DTD is hard coded into the app, we don't use the following - 
//		SWITCH_PAGE - not defined as we only have 1 page
//		EXT_I_0, EXT_I_1, EXT_I_2
//		PI - don't define processing inline - all hard coded
//		LITERAL,LITERAL_C,LITERAL_A,LITERAL_AC  - not using literals, don't define elements inline
//		EXT_T_0,EXT_T_1,EXT_T_2, EXT_0, EXT_1, EXT_2  - not using document extensions
//		OPAQUE - not using opaque data

#define KWAPP_STR_TERM  0x00	// Basically the NULL terminator character for a string
#define KWAPP_END_TAG	0x01    // Code for the end of an element
#define KWAPP_ENTITY	0x02    // indicates the start of a character entity number
#define KWAPP_STR_I		0x03    // indicates the start of an inline string,
								// terminated by '\0', OR STR_TERMINATOR
#define	KWAPP_STR_T		0X83    // a reference to the string table follows. Token is followed
								// by multi byte Int32 referencing an offset in the string table

#define KWAPP_TAG_CONTENT		0x40 // Bit 6 of the Wap Tag is set when a element has content
#define KWAPP_TAG_ATTRIBUTES	0x80 // Bit 7 of Tag is set when the element has attributes
#define KWAPP_TAG_ATTRIBUTES_CONTENT 0XC0  // Bits 6 & 7 are set - attributes and content
#define KWAPP_MB_INT32_CONTINUATION  0x80  // Continuation Bit of an octet of a Multibyte 32 bit int is set.
// Binary encoding for remote configuration of WAP browser settings
// same as for the Nokia 7110 

// **********    APPLICATION SPECIFIC CODE LEVEL TAG TOKENS    *************
#define	KWAPP_CHAR_LIST		  0x05	// Code level tag (CLT), indicates start 
									// of a characteristic list element
#define KWAPP_CHARACTERISTIC  0x06	// code level tag (CLT), indicates start 
									// of a characteristic element
#define KWAPP_PARM			  0x07	// CLT  indicating start of a 
									// attribute element of a characteristic

// ***************	 ATTRIBUTE TOKENS    *****************************
#define KWAPP_TYPE				0x05 // Undocumented token which indicates the 
								  	 // start of a TYPE attribute i.e. "TYPE="
#define KWAPP_ADDR_TYPE			0x06 // Characteristic type  = ADDRESS
#define KWAPP_URL_TYPE			0x07 // Characteristic type = URL 
#define KWAPP_NAME_TYPE			0x08 // Characteristic Type = NAME 
#define KWAPP_ID				0x7D // Characteristic Type = ID 
#define KWAPP_BOOKMARK			0x7F // Characteristic Type = BOOKMARK

#define KWAPP_NAME_ATTRIB		0x10 // A NAME attribute follows, i.e "NAME="
#define KWAPP_VALUE_ATTRIB		0x11 // A VALUE attribute follows, i.e "VALUE"

// ***************   PARM TYPES and VALUES  ***************************
#define KWAPP_BEARER			0x12 // This is a BEARER element 
#define KWAPP_PROXY				0x13 // This is a PROXY 
#define KWAPP_PORT				0x14 // PORT element
#define KWAPP_NAME				0x15 // This is a NAME element 

// IMPORTANT tag 0x16 had different meanings for different versions of the OTA settings 
// Not in Ver 4.0, = ICON token in Version 4.3 but 4.3 NOT SUPPORTED!!

#define KWAPP_PROXYTYPE			0x16 // PROXY_TYPE in Settings specification version 5.0!

#define KWAPP_URL				0x17 // Not defined for Version 4.0
									 // Start of a URL, i.e. "TYPE=URL" for Version 5.0 
#define KWAPP_PROXY_AUTHNAME	0x18 // Not defined for Ver. 4.0 - website uses Proxy token and inline string "AUTHNAME"
									 // Version 5.0 - proxy name follows
#define KWAPP_PROXY_AUTHSECRET	0x19 // Not defined for Version 4.0 - use Proxy token and inline string
									 // Versions 5.0 - the password for the proxy follows
#define KWAPP_SMS_SMSC_ADDR		0x1A // Service centre address for SMS service
#define KWAPP_USSD_SERV_CODE	0x1B // Service centre address for USSD service
#define KWAPP_ACCESS_POINT_NAME	0x1C // GPRS access point
#define KWAPP_PPP_LOGIN_TYPE	0x1D 
#define KWAPP_PROXY_LOGIN_TYPE	0x1E 
#define KWAPP_CSD_DIALSTRING	0x21 // Dialing sting for CSD gateway
#define KWAPP_PPP_AUTHTYPE		0x22 // type of authentification
#define KWAPP_PPP_AUTHNAME		0x23 // authentification name
#define KWAPP_PPP_AUTHSECRET	0x24 // authentification password
#define KWAPP_PPP_LOGINTYPE		0x25 // authentification password
#define	KWAPP_CSD_CALLTYPE		0x28 // CSD calltype 
#define KWAPP_CSD_CALLSPEED		0x29 // CSD callspeed

#define KWAPP_GSM_SMS_OLD		0x41 // Defined as GSM\SMS in Nokia Spec version 4.0,but not 5.0
									 // Distinguish it's Ver 4.0 by WAP Version ( = 1.0 )
									 // For versions 5.0 the WAP  XML version = 1.1, & token = 0x46 

#define KWAPP_GSM_CSD			0x45 // GSM CSD service 

// IMPORTANT tag 0x46 has multiple meaning across different versions of the Over the Air settings
#define KWAPP_GSM_SMS_NEW		0x46 // Not defined for Version 4.0
									 //  GSM SMS  Nokia spec version 5.0

#define KWAPP_GSM_USSD			0x47 //  Unstructured Short Service Data service 
									 //  Not defined for version 4.0  

//IMPORTANT tag 0x48 has different meanings in different versions of the Over the Air Settings specs
#define KWAPP_IS136				0x48 // Not defined for Version 4.0
									 // IS-136/CSD service in Nokia spec version 5.0

#define KWAPP_GPRS				0x49 // GSM/GPRS service  - added in OTA v6.0


#define KWAPP_PORT_9200			0x60 // Port 9200 - non continuous and not secure
#define KWAPP_PORT_9201			0x61 // Port 9201 - continuous but not secure
#define KWAPP_PORT_9202			0x62 // Indicates port 9202 - secure but non continuous conection
#define KWAPP_PORT_9203			0x63 // Indicates port 9203 - secure and continuous 
#define KWAPP_AUTOMATIC			0x64 
#define KWAPP_MANUAL			0x65 
#define KWAPP_SPEED_AUTO		0x6A // Use auto selection of speed
#define KWAPP_SPEED_9600		0x6B // Indicates comm speed is 9600
#define KWAPP_SPEED_14400		0x6C // Indicates comm speed is 14400
#define	KWAPP_SPEED_19200		0x6D // Comm speed is 19200
#define KWAPP_SPEED_28800		0x6E // Comms speed is 28800
#define KWAPP_SPEED_38400		0x6F // Comms speed is 38400
#define KWAPP_PAP				0x70 // Interface protocol = PAP
#define KWAPP_CHAP				0x71 // Interface protocol = CHAP
#define KWAPP_ANALOGUE			0x72 // Analogue connection
#define KWAPP_ISDN				0x73 // Digital ISDN connection
#define KWAPP_SPEED_43200		0x74 // Communication speed of 42K
#define KWAPP_SPEED_56700		0x75 // Comm speed of 56k
#define KWAPP_MSISDN_NO			0x76 // Proxy type is MSISDN number (relevant for USSD)
#define KWAPP_IPV4				0x77 // Proxy type is an IP address (relevant for USSD)
#define KWAPP_MSCHAP			0x78 // Interface protocol = CHAP
#define KWAPP_NAME_ISP			0X7E // Start of Internet Service Provider follows
#define KWAPP_MMS_URL			0x7C // Address of Multi Media Service Centre -added in OTA v7.0


// Labels for use in array of ParsedFields - application specific not Nokia 

// CHARACTERISTIC DELIMITORS
_LIT(KWappCharAddress,	"CHAR-ADDRESS"); // Start of an Address Characteristic
_LIT(KWappCharName,		"CHAR-NAME");	 // Start of a Name Characteristic
_LIT(KWappCharURL,		"CHAR-URL");	 // Start of a URL Characteristic	
_LIT(KWappCharMMSURL,	"CHAR-MMSURL");	 // Start of a URL Characteristic	
_LIT(KWappCharBookmark, "CHAR-BOOKMARK");// Start of a Bookmark Characteristic
_LIT(KWappCharID,		"CHAR-ID");		 // Start of an ID characteristic

_LIT(KWappXMLversion,"VERSIONx10");
_LIT(KWappPublicID, "PUBLICID");
_LIT(KWappCharacterSet, "CHARSET");

//XML Document Characteristic Tags
_LIT(KWappAddr, "ADDRESS");  
_LIT(KWappURL, "URL");
_LIT(KWappMMSURL, "MMSURL"); // Added in v6.6 of Nokia OTA Spec
_LIT(KWappName, "NAME");  
_LIT(KWappBookmark, "BOOKMARK");
_LIT(KWappID, "ID");


// General tags
_LIT(KWappBearer, "BEARER");
_LIT(KWappProxy, "PROXY");
_LIT(KWappPort, "PORT");
_LIT(KWappProxyType, "PROXY_TYPE");

_LIT(KWappProxyAuthName, "PROXY_AUTHNAME");
_LIT(KWappProxyAuthSecret, "PROXY_AUTHSECRET");
_LIT(KWappProxyLoginType, "PROXY_LOGINTYPE");
_LIT(KWappSMSCAddress, "SMS_SMSC_ADDRESS");
_LIT(KWappUSSDCode, "USSD_SERVICE_CODE");
_LIT(KWappISP, "ISP_NAME");
_LIT(KWappAccessPointName, "ACCESS_POINT_NAME");

_LIT(KWappCsdDial, "CSD_DIALSTRING");
_LIT(KWappPPPAuthType, "PPP_AUTHTYPE");
_LIT(KWappPPPAuthName, "PPP_AUTHNAME");
_LIT(KWappPPPAuthSecret, "PPP_AUTHSECRET");
_LIT(KWappPPPLoginType, "PPP_LOGINTYPE");
_LIT(KWappCsdCallType, "CSD_CALLTYPE");
_LIT(KWappCsdCallSpeed, "CSD_CALLSPEED");

_LIT(KWappGsmCsd, "GSM/CSD");
_LIT(KWappGsmSms, "GSM/SMS");
_LIT(KWappGsmUssd, "GSM/USSD");
_LIT(KWappIS136Csd, "IS136/CSD");
_LIT(KWappGprs, "GPRS");   // Added in OTA v6.0

_LIT(KWappPort9200, "9200");
_LIT(KWappPort9201, "9201");
_LIT(KWappPort9202, "9202");
_LIT(KWappPort9203, "9203");

_LIT(KWappSpeedAuto, "AUTO");
_LIT(KWappSpeed9600, "9600");
_LIT(KWappSpeed14400, "14400");
// Additional speeds added in Version 5.0
_LIT(KWappSpeed19200, "19200");
_LIT(KWappSpeed28800, "28800");
_LIT(KWappSpeed38400, "38400");
_LIT(KWappSpeed43200, "43200");
_LIT(KWappSpeed56700, "57600");

_LIT(KWappPAP, "PAP");
_LIT(KWappCHAP, "CHAP");
_LIT(KWappMSCHAP, "MSCHAP");
_LIT(KWappAnalogue, "ANALOGUE");
_LIT(KWappISDN, "ISDN");

_LIT(KWappAuthAutomatic, "AUTOMATIC");
_LIT(KWappAuthManual, "MANUAL");

_LIT(KWappMsisdnNo, "MSISDN_NO");
_LIT(KWappIpv4, "IPV4");


#endif
