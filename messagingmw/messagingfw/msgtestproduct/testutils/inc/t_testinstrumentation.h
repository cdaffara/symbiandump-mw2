/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* @file t_testInstrumentation.h
* This contains the list of event codes that are used to identify a particular event.
* These event codes are used by the EvenlogServer to generate a specif type of log for a specific event.
* 
*
*/




#ifndef __TEST_INSTRUMENTATION_H__
#define __TEST_INSTRUMENTATION_H__

//	Epoc Include
#include <e32base.h>

/** 
List of event codes.
*/
	enum TInstrumentationPoint
		{
		// Event codes 0 to 20 has been reserved for error/specific condition
		EBadEventCode			= 0,
		EImapRamUsageStart		= 21,
		EImapRamUsageStop,
		EPopRamUsageStart		= 23,
		EPopRamUsageStop,
		ESmtpRamUsageStart		= 25,
		ESmtpRamUsageStop,
		ESmtpRamUsageCreateMessageStart = 27,
		ESmtpRamUsageCreateMessageStop,
		ESmtpRamUsageSendMessageStart	= 29,
		ESmtpRamUsageSendMessageStop,
		ESmtpCreateMessageSpeedStart	= 31,
		ESmtpCreateMessageSpeedStop,
		ESmtpSendMessageSpeedStart		= 33,
		ESmtpSendMessageSpeedStop,
		EImapDownloadMessageSpeedStart	= 35,
		EImapDownloadMessageSpeedStop,
		
		EImapCopyMessageSpeedStart		= 37,
		EImapCopyMessageSpeedStop,
		EImapMoveMessageSpeedStart		= 39,
		EImapMoveMessageSpeedStop,
		EImapDeleteMessageSpeedStart	= 41,
		EImapDeleteMessageSpeedStop,
		ESearchSortStart				= 50,
		ESearchSortStop
		};


#endif /* __TEST_INSTRUMENTATION_H__ */
