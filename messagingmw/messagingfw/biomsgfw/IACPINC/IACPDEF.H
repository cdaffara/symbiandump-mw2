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
//

#ifndef __IACPDEF_H__
#define __IACPDEF_H__

//-- Mapping of Bio-Message Field names to Commbd and Miut Field Names --
// SMS header and password 
_LIT(KSmsHeader,		"//SIAP11");
_LIT(SMS_HEADER,		"Header");
_LIT(SMS_PASSWORD,		"Password");

//Internet Settings (Dial Out Service Table)

//--- Fields for Basic IAP Settings (Nokia Smart Messaging Spec 2.0.0Pre)
_LIT(SMS_ISP_I_NAME,		"Iname");			//Isp Provider Name
_LIT(SMS_DEFAULT_TEL_NUM,	"Itel");			// The default telephone number to be used if the location does not match those below
_LIT(SMS_LOGIN_NAME,		"Iuid");			// Login name
_LIT(SMS_PROMPT_FOR_LOGIN,	"Ipwp");			// Prompt user for username and password?
_LIT(SMS_LOGIN_PASS,		"Ipwd");			// Login password
_LIT(SMS_INIT_STRING,		"Iini");			// Modem Initialisation
_LIT(SMS_IP_ADDR,			"Iip");				// IP Address of Interface
_LIT(SMS_IP_NAME_SERVER1,	"Idns1");			// IP Address of primary name server reachable through this interface
_LIT(SMS_IP_NAME_SERVER2,	"Idns2");			// IP Address of secondary name server reachable through this interface
_LIT(SMS_IP_NETMASK,		"Imsk");			// IP Netmask of Interface
_LIT(SMS_IP_GATEWAY,		"Idgw");			// IP Address of Gateway

//--- Fields for Extended IAP (ISP Settings) 
_LIT(SMS_PROXY_EXCEPTIONS,		"Inop");//HTTP no proxy for
_LIT(SMS_PROXY_PORT_NUMBER,		"Ippo");//HTTP Port, maximum length 5 digits
_LIT(SMS_PROXY_SERVER_NAME,		"Iprx");//HTTP proxy <ip-string>
_LIT(SMS_LOGIN_CUSTOMISATION,	"Ilgn");//login customisation "0" for none "1" for manual, or name of the script. 
_LIT(SMS_ENABLE_SW_COMP,		"Icmp");//PPP compression "Y"|"N"
_LIT(SMS_SECURE_PROXY,			"Isrx");//secure proxy ip <ip-string>
_LIT(SMS_SECURE_PORT,			"Ispo");//secure port, maximum length 5 digits

//-- Script Settings (dial out ISP Table)
_LIT(SMS_SCRIPT_NAME,			"Pname");
_LIT(SMS_SCRIPT_TYPE,			"Ptype"); //corresponds to ISP Comms DB ISP_IF_NAME which is set to "PPP" for eg.
_LIT(SMS_SCRIPT_DATA,			"Pdata");
_LIT(SMS_SCRIPT_ADD,			"Padd");


//-- Email settings --
//Right tokens identifying sms as a POP3 or IMAP4
_LIT(SMS_POP,				"PO");				//POP3 Protocol
_LIT(SMS_IMAP,				"IM");				//IMAP4 Protocol
// common for POP3 and  IMAP4
_LIT(SMS_ISP_M_NAME,		"Mname");			// ISP Name
_LIT(SMS_MAILBOX_NAME,		"Muid");			// iLoginName(POP3/IMAP)
_LIT(SMS_MAILBOX_PASS,		"Mpwd");			// iPassword (POP3/IMAP)
_LIT(SMS_RECEIVING_HOST,	"Mrcv");			// iServerAddress (POP3)
_LIT(SMS_SENDING_HOST,		"Msnd");			// iServerAddress (SMTP)
_LIT(SMS_USER_EMAIL_ADDR,	"Madr");			// iEmailAddress(SMTP)
_LIT(SMS_MAIL_PROTOCOL,		"Mpro");			// PO/IM

//IMAP4 settings
_LIT(SMS_FOLDER_PATH,		"Mbox");			// IMAP4 Folder path as defined in V2.0.0 of Nokia Smart Messaging Spec

//SMS Settings
_LIT(SMS_SERVICE_CENTER_NAME,		"Sname");
_LIT(SMS_SERVICE_CENTER_ADDRESS,	"Stel");

//---Telephone Settings (Configure the voice mail number)
_LIT(SMS_TEL_VOICE_MAILBOX_NUM, "TBox");

//---WWW Hotlist Settings
_LIT(SMS_HOTLIST_ITEM_NAME,		"Hname");
_LIT(SMS_HOTLIST_ITEM_URL,		"Hurl");
_LIT(SMS_HOTLIST_AUTOSELECT_IAP,"Haap");
_LIT(SMS_HOTLIST_ACCESS_POINT,	"Hiap");
_LIT(SMS_HOTLIST_FOLDER,		"Hdir");

//GPRS Settings
_LIT(GPRSS_NAME, "Gname");
_LIT(GPRSS_APN, "Gapn");
_LIT(GPRSS_PDP_TYPE, "GPdpt");
_LIT(GPRSS_PDP_ADDRESS,		"Gpdp");
_LIT(GPRSS_IF_NAME,			"Gifn");

_LIT(GPRSS_LOGIN_NAME,		"Guid");			// Login name
_LIT(GPRSS_PROMPT_FOR_LOGIN,"Gpwp");			// Prompt user for username and password?
_LIT(GPRSS_LOGIN_PASS,		"Gpwd");			// Login password
_LIT(GPRSS_IP_ADDR,			"Gip");				// IP Address of Interface
_LIT(GPRSS_DNS_FROM_SERVER,	"Gsdns");
_LIT(GPRSS_IP_NAME_SERVER1,	"Gdns1");			// IP Address of primary name server reachable through this interface
_LIT(GPRSS_IP_NAME_SERVER2,	"Gdns2");			// IP Address of secondary name server reachable through this interface
_LIT(GPRSS_IP_NETMASK,		"Gmsk");			// IP Netmask of Interface
#endif //__IACPDEF_H__
