/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides an observer interface for GSM data.
*
*/



#ifndef M_LBTCELLIDMVMTDETCELLOBSERVER_H
#define M_LBTCELLIDMVMTDETCELLOBSERVER_H


#include <rmmcustomapi.h>


/**
 *  Observer interface for getting notifications from class which gets 
 *  GSM data @ref CLbtCellIdMvmtDetGetCellData.
 *
 *  Classes interested in getting notifications when GSM data is received
 *  must implement this interface. 
 *
 *  @lib ltcellidmovdet.lib
 *  @since  S60 v4.0
 */
class MLbtCellIdMvmtDetCellOberserver
	{

public:

    /**
     * An observer callback to handle notification event. This is called when GSM data is 
     * received.
     * @param aError Error reported by service provider.
     * @since S60 TACO
     */
	virtual void HandleCellDataL( RMmCustomAPI::TMmCellInfo& aCellInfo, TInt& aError )=0;

	};
	
#endif //M_LBTCELLIDMVMTDETCELLOBSERVER_H	
