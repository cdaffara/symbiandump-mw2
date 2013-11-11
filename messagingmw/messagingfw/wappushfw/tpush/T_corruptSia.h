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

#ifndef __T_corruptSia_h__
#define __T_corruptSia_h__


const TUint8 KSiaMsgHeader[] = 
	{
	0xB4,	//Content type "application/vnd.wap.sia" 
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

//corrupt Sia Body
const TUint8 KSiaCorruptBody[]=
	{
	0xdd, 0xdd, 0xdd, 0xdd,	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
	0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
	0xdd, 0xdd
	};

/*

// NOTE: tests for valid address require address in COMMD
// normal SIA message with single valid address 
const TUint8 SIA[]=
	{
	0x00,		//	version
	0x02,		//	AppIdListLen
	0x44,		//	Application Id List 
	0x44,			
	0x1B,		// ContactPointsLen
	// Contact Points
	0xD7,		//	bearer flag and port flag and address len 
	0x0A,		//	bearer Type (CSD)
	0x0B,		//	port number
	0x84,		//		= 2948
	0x68,		// address - "http://www.symbian.com/" (as text)
	0x74,		
	0x74,
	0x70,
	0x3A,
	0x2F,
	0x2F,
	0x77,		
	0x77,		
	0x77,
	0x2E,
	0x73,
	0x79,
	0x6D,
	0x62,
	0x69,
	0x61,
	0x6E,
	0x2E,
	0x63,
	0x6F,
	0x6D,
	0x2F
	};


// normal SIA message but no port or bearer ( valid )
const TUint8 SIA[]=
	{
	0x00,		//	version
	0x02,		//	AppIdListLen
	0x44,		//	Application Id List 
	0x44,			
	0x18,		// ContactPointsLen
	// Contact Points
	0x17,		//	 no bearer flag or port flag only address len 
				// no bearer or port specified
	0x68,		// address - "http://www.symbian.com/" (as text)
	0x74,		
	0x74,
	0x70,
	0x3A,
	0x2F,
	0x2F,
	0x77,		
	0x77,		
	0x77,
	0x2E,
	0x73,
	0x79,
	0x6D,
	0x62,
	0x69,
	0x61,
	0x6E,
	0x2E,
	0x63,
	0x6F,
	0x6D,
	0x2F
	};
	
// SIA message with no contact points specified (error)
const TUint8 SIA[]=
	{
	0x00,		//	version
	0x02,		//	AppIdListLen
	0x44,		//	Application Id List 
	0x44,			
	0x00		// ContactPointsLen == 0
			// No Contact Points
	};
*/
#endif
