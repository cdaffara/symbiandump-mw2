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
* Description:  Monitors file creations, modifications and deletions.*
*/


inline TInt RMdsFSPEngine::Enable()
	{
	return DoControl( EMdsFSPOpEnable );
	}

inline TInt RMdsFSPEngine::Disable()
	{
	return DoControl( EMdsFSPOpDisable );
	}

inline void RMdsFSPEngine::RegisterNotification(
		TMdsFSPStatusPckg& aMdsFSPStatus, TRequestStatus& aStat )
	{
	DoRequest( EMdsFSPOpRegisterNotification, aStat, aMdsFSPStatus );
	}

inline void RMdsFSPEngine::AddNotificationPath( const TDesC& aPath )
	{
	TMdsFSPStatusPckg pckg;
	TRequestStatus rs;
	TMdsFSPStatus& status = pckg();

	status.iFileName.Zero();
	status.iFileName.Copy( aPath );

	DoRequest( EMdsFSPOpAddNotificationPath, rs, pckg );
	User::WaitForRequest( rs );
	}

inline void RMdsFSPEngine::RemoveNotificationPath( const TDesC& aPath )
	{
	TMdsFSPStatusPckg pckg;
	TRequestStatus rs;
	TMdsFSPStatus& status = pckg();

	status.iFileName.Zero();
	status.iFileName.Copy( aPath );

	DoRequest( EMdsFSPOpRemoveNotificationPath, rs, pckg );
	User::WaitForRequest( rs );
	}

inline void RMdsFSPEngine::AddIgnorePath (const TDesC& aPath )
	{
	TMdsFSPStatusPckg pckg;
	TRequestStatus rs;
	TMdsFSPStatus& status = pckg();

	status.iFileName.Zero();
	status.iFileName.Copy( aPath );

	DoRequest( EMdsFSPOpAddIgnorePath, rs, pckg );
	User::WaitForRequest( rs );
	}

inline void RMdsFSPEngine::RemoveIgnorePath( const TDesC& aPath )
	{
	TMdsFSPStatusPckg pckg;
	TRequestStatus rs;
	TMdsFSPStatus& status = pckg();

	status.iFileName.Zero();
	status.iFileName.Copy( aPath );

	DoRequest( EMdsFSPOpRemoveIgnorePath, rs, pckg );
	User::WaitForRequest( rs );
	}

inline void RMdsFSPEngine::NotificationCancel()
	{
	DoCancel( EMdsFSPOpNotificationCancel );
	}

