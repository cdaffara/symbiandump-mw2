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

inline TBool CSpecialNbsSmsSocketWatcher::CanStoreMessage() const
	{
	__ASSERT_DEBUG(iReportHandling != CSmsSettings::EDoNotWatchForReport, PanicWatcher(EReportHandlingInvalid));
	return (iReportHandling != CSmsSettings::EDiscardReport && iReportHandling != CSmsSettings::EDiscardReportAndMatch && iReportHandling != CSmsSettings::EDoNotWatchForReport);
	}

inline TBool CSpecialNbsSmsSocketWatcher::Visible() const
	{
	return (iReportHandling == CSmsSettings::EMoveReportToInboxVisible || iReportHandling == CSmsSettings::EMoveReportToInboxVisibleAndMatch);
	}
