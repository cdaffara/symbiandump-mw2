/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains the hardcoded implementaion
*
*/

#include "CExecuteDialog.h"
#include <coemain.h>
#include <APGTASK.H>


CExecuteDialog::CExecuteDialog(): CActive(0),iFirstTime(ETrue),mTimeoutTimer()
	{
	CActiveScheduler::Add(this);
	mTimeoutTimer.CreateLocal();
	}

CExecuteDialog::~CExecuteDialog()
	{
    Cancel();
    // Closes the timer
    mTimeoutTimer.Close();
	}

void CExecuteDialog::SimulateExitKeyL()
	{
	/*iStatus = KRequestPending;
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);*/
	
	// if the satellite info is not retrieved in 20 seconds then the dialob box will not be launched
	mTimeoutTimer.After( iStatus, 20000000 );
	SetActive();
	}

void CExecuteDialog::RunL()
	{
//    if( iFirstTime )
//    	{
//    	    User::After(20000000);
//    	    iStatus = KRequestPending;
//			SetActive();
//			TRequestStatus* status = &iStatus;
//			User::RequestComplete(status, KErrNone); 
//			iFirstTime = EFalse;
//    	}
//    else
//    	{
	    //User::After(2000000);	
//	
//		TApaTask task(CEikonEnv::Static()->WsSession());
//		task.SetWgId( CEikonEnv::Static()->RootWin().Identifier());
//		task.SendToBackground();
//    
//		User::After(1000000);
//		task.BringToForeground();
//		User::After(1000000);

		
		TKeyEvent key;
		key.iCode = EKeyEscape;
		key.iModifiers = 0;
		CCoeEnv::Static()->SimulateKeyEventL( key, EEventKey );   	
 //   	}


	/*	*/
	}
void CExecuteDialog::DoCancel()
	{

	}
