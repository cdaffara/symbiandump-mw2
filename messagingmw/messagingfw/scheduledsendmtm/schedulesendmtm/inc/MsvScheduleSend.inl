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
//



/**
 
 
 Gets off-peak times settings.
 
 @return Off-peak times settings
*/

inline CMsvOffPeakTimes& CMsvScheduleSend::OffPeakTimes() const
	{
	return *iOffPeakTimes;
	}


/**
Gets scheduler settings.

@return Scheduler settings
*/

inline CMsvScheduleSettings& CMsvScheduleSend::ScheduleSettings() const
	{
	return *iSettings;
	}


/**
Gets action-on-sending-error settings.

@return Action-on-sending-error settings
*/

inline CMsvSendErrorActions& CMsvScheduleSend::SendErrorActions() const
	{
	return *iErrorActions;
	}


/**
Gets System Agent conditions that must be satisfied before a sending attempt.

@return System Agent conditions
*/

inline CMsvSysAgentActions& CMsvScheduleSend::AgentActions() const
	{
	return *iAgentActions;
	}
