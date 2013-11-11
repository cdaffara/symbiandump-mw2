/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes testing.
*
*/

#ifndef M_MOMASUPLTESTOBSERVER_H
#define M_MOMASUPLTESTOBSERVER_H

/**
 *  Observes testing
 *
 *  Provides callback functions to respond to various scenarios of testing.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
class MOMASuplTestObserver
	{
	public:
		virtual void StartTestingL() = 0;
		virtual void TestingCancelled(const TDesC8& aInfo) = 0;
		virtual void TestingComplete(const TDesC8& aInfo) = 0;
		virtual void TestingAborted(const TDesC8& aError) = 0;
	};
#endif