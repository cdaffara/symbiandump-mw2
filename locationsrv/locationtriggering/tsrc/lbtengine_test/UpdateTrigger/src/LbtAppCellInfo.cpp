/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of Timer class 
*
*/



#include "LbtAppCellInfo.h"

//_LIT(KLbtTraceDir,"epos");
 //_LIT(KLbtTraceFile,"TriggerFireInfo.log");
 

CLbtAppCgiRequestor* CLbtAppCgiRequestor::NewL(RFileLogger &aLog)
    {
    	CLbtAppCgiRequestor* self = new (ELeave) CLbtAppCgiRequestor(aLog);
    	
    	self->ConstructL();
    	
    	return self;
    }
CLbtAppCgiRequestor::~CLbtAppCgiRequestor()
    {
//    	 	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtAppCgiRequestor before Cancel"));
    	 Cancel();
//    	 	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtAppCgiRequestor After Cancel"));
    	 delete iTelephony;
//    	 	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtAppCgiRequestor After ITelephony delete"));
    }
void CLbtAppCgiRequestor::Start(CActiveSchedulerWait* aWait )
    {
    //iStatus = KRequestPending;
    iWait=aWait;
    if(!IsActive())
        SetActive();	
    
    //Make Async call
    iTelephony->GetCurrentNetworkInfo(iStatus, iNwInfoPckg);
    
    
    }
    
CTelephony::TNetworkInfoV1Pckg CLbtAppCgiRequestor::CurrentCGI()
    {
    return iNwInfoPckg;
        
    }    
void CLbtAppCgiRequestor::DoCancel()
    {
    //Cancel Async call
//    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "DoCancel before CancelAsync"));
    iTelephony->CancelAsync(CTelephony::ECurrentNetworkInfoChangeCancel);
//    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "DoCancel After CancelAsync"));
    
    }
CLbtAppCgiRequestor::CLbtAppCgiRequestor(RFileLogger &aLog) 
                        : CActive( EPriorityStandard ),iNwInfoPckg(iNwInfo),iLog(aLog)
                          
    {
    CActiveScheduler::Add( this ); 
    }
void CLbtAppCgiRequestor::ConstructL()
    {
    //Construct Async Object
    iTelephony = CTelephony::NewL();
    
    }
    
void CLbtAppCgiRequestor::RunL()
	{
	
	if(iWait->IsStarted())
	{
		iWait->AsyncStop();
	}
	//iLog.Write(_L("CLbtAppCgiRequestor::RunL"));	
/*	iObs->HandleCIDChangeL(iNwInfo);
	Start();*/
    }
    
    
