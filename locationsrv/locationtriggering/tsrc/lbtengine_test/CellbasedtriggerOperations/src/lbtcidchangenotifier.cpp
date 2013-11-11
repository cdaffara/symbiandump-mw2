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
* Description:     Implementation of Timer class 
*
*/






#include "lbtcidchangenotifier.h"

//_LIT(KLbtTraceDir,"epos");
// _LIT(KLbtTraceFile,"TriggerFireInfo.log");
 

CLbtCidChangeNotifier* CLbtCidChangeNotifier::NewL(RFileLogger &aLog, MLbtCidChangeObsrvr *aObs)
    {
    	CLbtCidChangeNotifier* self = new (ELeave) CLbtCidChangeNotifier(aLog, aObs);
    	
    	self->ConstructL();
    	
    	return self;
    }
CLbtCidChangeNotifier::~CLbtCidChangeNotifier()
    {
//    	 	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtCidChangeNotifier before Cancel"));
    	 Cancel();
//    	 	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtCidChangeNotifier After Cancel"));
    	 delete iTelephony;
//    	 	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtCidChangeNotifier After ITelephony delete"));
    }
void CLbtCidChangeNotifier::Start( )
    {
    //iStatus = KRequestPending;
    if(!IsActive())
        SetActive();	
    
    //Make Async call
    iTelephony->NotifyChange(iStatus, CTelephony::ECurrentNetworkInfoChange, iNwInfoPckg);
    iState = ENotifyNetworkChange;
    }

void CLbtCidChangeNotifier::GetCurrentCGIInfo()
    {
    //iStatus = KRequestPending;
    if(IsActive())
        {
        return;
        }
        
    iTelephony->GetCurrentNetworkInfo(iStatus, iNwInfoPckg);
    SetActive();     
    iState = ECurrentNetwork;
    }

void CLbtCidChangeNotifier::DoCancel()
    {
    switch( iState )
        {
        case ECurrentNetwork:
            iTelephony->CancelAsync(CTelephony::EGetCurrentNetworkInfoCancel);
            iState = ENone;
            break;
            
        case ENotifyNetworkChange:
            iTelephony->CancelAsync(CTelephony::ECurrentNetworkInfoChangeCancel);
            iState = ENone;
            break;
            
        default:
            break;
        }
    
    }

CLbtCidChangeNotifier::CLbtCidChangeNotifier(RFileLogger &aLog, MLbtCidChangeObsrvr *aObs) 
                        : CActive( EPriorityStandard ),
                        iNwInfoPckg(iNwInfo),
                        iLog(aLog), 
                        iObs(aObs),
                        iState( ENone ) 
                        
    {
    CActiveScheduler::Add( this ); 
    }
void CLbtCidChangeNotifier::ConstructL()
    {
    //Construct Async Object
    iTelephony = CTelephony::NewL();
    }
    
void CLbtCidChangeNotifier::RunL()
	{
    switch( iState )
        {
        case ECurrentNetwork:
            iObs->HandleCIDChangeL(iNwInfo);        
            iState = ENone;
            break;
            
        case ENotifyNetworkChange:
            iObs->HandleCIDChangeL(iNwInfo);
            Start();
            break;
            
        default:
            break;
        }
    }
    
    
