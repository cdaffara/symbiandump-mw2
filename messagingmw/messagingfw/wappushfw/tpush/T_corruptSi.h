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

#ifndef __T_corruptSi_h__
#define __T_corruptSi_h__

// si text message data

_LIT8(KSIActionDelete,"delete");
_LIT8(KSIActionSignal,"signal-medium");
_LIT8(KSIHref, "http://www.xyz.com/ppaid/123/abc.wml");
_LIT8(KSINewHref, "http://www.xyz.com/ppaid/123/new.wml");
_LIT8(KSIHref_Msg2, "http://www.xyz.com/ppaid/123/Msg2.wml");

_LIT8(KSIOldDate,		"2000-01-01T15:23:15Z");
_LIT8(KSeptemberDate,	"2000-09-30T00:00:00Z");
_LIT8(KOctoberDate,		"2000-10-30T00:00:00Z");
_LIT8(KNotExipredDate,	"2001-10-30T00:00:00Z"); //assuming this msg has not expired yet
_LIT8(KSISiId, "http://www.xyz.com/ppaid/123/abc.wml");
_LIT8(KSISiId_Msg1, "http://www.xyz.com/ppaid/123/abc_Msg1.wml");



const TUint8 KMessageHeaderSi[] = 
	{
	0xAD,  //Content type "application/vnd.wap.si" 
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

const TUint8 KMessageHeaderSic[] = 
	{
	0xAE,	// content type application/vnd.wap.sic
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

#endif

