/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides an observer interface system startup state 
*
*/


#ifndef M_LBTSTARTUPOBSERVER_H
#define M_LBTSTARTUPOBSERVER_H

/**
 * System state starter listener provides this to notify
 * when system state is ready for server loading
 *
 * @since S60 v4.0
 */
class MLbtStartupObserver
	{
	public:
		/**
		 * Notifies the observer if the system state
		 * has changed so that non critical operation
		 * like LBT logic server loading can be done
		 *
		 */
		virtual void HandleStartupCompletionL( ) = 0;
	};
	
#endif //M_LBTSTARTUPOBSERVER_H