/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class for doing Initilization of protocol handler operation.
*
*/



#ifndef C_COMASUPLINITILIZEOBSERVER_H
#define C_COMASUPLINITILIZEOBSERVER_H


/**
 *  MCompleteInitilization ... observer for initilization completion.
 *  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */
class MCompleteInitilization
{
	public:
		/**
		* InitilizationCompletedL Method,called after request has been completed by Active Scheduler
		* @since Series 60 3.1M
		* @param aError,Error code occured during initilization
		* @return None
		*/ 
		virtual void InitilizationCompletedL(TInt aError)=0;
		
		
};

#endif // C_COMASUPLINITILIZEOBSERVER_H
