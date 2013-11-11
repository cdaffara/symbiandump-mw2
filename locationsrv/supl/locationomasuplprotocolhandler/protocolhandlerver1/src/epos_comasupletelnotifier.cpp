/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of Notifier class of ETel
*
*/



// INCLUDE FILES
#include "epos_comasupltrace.h"
#include "epos_comasupletelnotifier.h"
#include "epos_comasupllocationid.h"

_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplETelNotifier.cpp");

// ============================ MEMBER FUNCTIONS OF COMASuplETelNotifier ===============================

// -----------------------------------------------------------------------------
// COMASuplETelNotifier::COMASuplETelNotifier,constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

COMASuplETelNotifier::COMASuplETelNotifier(MOMASuplETelNotifierObserver& aObserver,RMobilePhone& aPhone) 
					    : CActive(EPriorityStandard),
					      iPhone(aPhone),
					      iNetworkInfoPkg( iNetworkInfo),
					      iHomeNetworkInfoPkg( iHomeNetworkInfo),
					      iObserver(aObserver)
	{
		 CActiveScheduler::Add(this);            
	}

// -----------------------------------------------------------------------------
// COMASuplETelNotifier::ConstructL,two phase constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETelNotifier::ConstructL()
    {
    iCellMode = EOMASuplNone;
    iTrace = COMASuplTrace::NewL();	
    iTrace->Trace(_L("COMASuplETelNotifier::ConstructL"), KTraceFileName, __LINE__); 
    }

// -----------------------------------------------------------------------------
// COMASuplETelNotifier::NewL,two phase constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplETelNotifier* COMASuplETelNotifier::NewL(MOMASuplETelNotifierObserver& aObserver,
								 RMobilePhone& aPhone)
    {
    COMASuplETelNotifier* self = new( ELeave ) COMASuplETelNotifier(aObserver,aPhone);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplETelNotifier::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETelNotifier::DoCancel()
    {
    	if(iCellMode == EOMASuplCellIdHome)
    		{
            iTrace->Trace(_L("COMASuplETelNotifier, cancel home"), KTraceFileName, __LINE__); 
    		iPhone.CancelAsyncRequest(EMobilePhoneGetCurrentNetwork);            
    		}
    	else if(iCellMode == EOMASuplCellIdCurrent)
    		{
            iTrace->Trace(_L("COMASuplETelNotifier, cancel current"), KTraceFileName, __LINE__); 
    		iPhone.CancelAsyncRequest(EMobilePhoneGetHomeNetwork);            
    		}	
    	else if (iCellMode == EOMASuplCellIdNotify)
    		{
            iTrace->Trace(_L("COMASuplETelNotifier, cancel notify"), KTraceFileName, __LINE__); 
            iPhone.CancelAsyncRequest(EMobilePhoneNotifyCurrentNetworkChange);                    
    		}	
    }            

// -----------------------------------------------------------------------------
// COMASuplETelNotifier::~COMASuplETelNotifier,Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplETelNotifier::~COMASuplETelNotifier()
    {
        if (IsActive())            
    	    Cancel();
    	delete iTrace;
    }
            
// -----------------------------------------------------------------------------
// COMASuplETelNotifier::CheckForRoaming,For retriving the cell id information
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETelNotifier::CheckForRoaming()
    {
	iTrace->Trace(_L("COMASuplETelNotifier::CheckForRoaming..."),KTraceFileName, __LINE__); 							
     if( !IsActive() )
    	{
    	iCellMode = EOMASuplCellIdHome;
    	iPhone.GetHomeNetwork( iStatus, iHomeNetworkInfoPkg);
    	SetActive();    
    	}              
    }  
      
// -----------------------------------------------------------------------------
// COMASuplETelNotifier::CheckForRoaming,For retriving the cell id information
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETelNotifier::GetCurrentNetwork()
    {
	iTrace->Trace(_L("COMASuplETelNotifier::GetCurrentNetwork..."),KTraceFileName, __LINE__); 							
    if( !IsActive() )
    	{
    	iCellMode = EOMASuplCellIdCurrent;
    	iPhone.GetCurrentNetwork( iStatus, iNetworkInfoPkg, iLocationInfo );
    	SetActive();  
    	}               
    }  

// -----------------------------------------------------------------------------
// COMASuplETelNotifier::CheckForRoaming,For retriving the cell id information
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETelNotifier::NotifyCurrentNetworkChange()
    {
	iTrace->Trace(_L("COMASuplETelNotifier::NotifyCurrentNetworkChange..."),KTraceFileName, __LINE__);
    if( !IsActive() )
    	{
    	iCellMode = EOMASuplCellIdNotify;
    	iPhone.NotifyCurrentNetworkChange( iStatus, iNetworkInfoPkg, iLocationInfo );
    	SetActive();   
    	}              
    }  
      
// -----------------------------------------------------------------------------
// COMASuplETelNotifier::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplETelNotifier::RunL()
    {
    if (iStatus == KErrNone)
        {       
        	if(iCellMode == EOMASuplCellIdHome)
        		{
        		GetCurrentNetwork();
        		}
            else if (iCellMode == EOMASuplCellIdCurrent)
                {
                    CompeteRoamingCheckL();                                        
                }
            else 
                {
                    NotifyCurrentNetworkChange();
                    CompareForRoamingL();
                }                        
        }
    else
        {
        	if(iCellMode == EOMASuplCellIdHome)
	    		{
					// Error while processing
					iTrace->Trace(_L("Error in getting Home network"),KTraceFileName, __LINE__); 							
					iObserver.HandleCurrentNetworkChangeL(iStatus.Int(), ETrue);    
	    		}
        	if(iCellMode == EOMASuplCellIdCurrent)
	    		{
					// Error while processing
					iTrace->Trace(_L("Error in getting Current network"),KTraceFileName, __LINE__);
					iObserver.HandleCurrentNetworkChangeL(iStatus.Int(), ETrue);    
	    		}
            else
	    		{
					// Error while processing
					iTrace->Trace(_L("Error in getting Notify Current network change"),KTraceFileName, __LINE__);
					iObserver.HandleCurrentNetworkChangeL(iStatus.Int(), ETrue);    
	    		}
        }                
    }

// -----------------------------------------------------------------------------
// COMASuplETelNotifier::RunError, Will be called by framework,if RunL leaves.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplETelNotifier::RunError(TInt aError)
	{
		if(iCellMode == EOMASuplCellIdHome)
		{
			// Error while processing
			iTrace->Trace(_L("RunL leaved for Home Network"),KTraceFileName, __LINE__);
		}
		else if(iCellMode == EOMASuplCellIdCurrent)
		{
			// Error while processing
			iTrace->Trace(_L("RunL leaved for Current Network"),KTraceFileName, __LINE__);
		}	
		{
			// Error while processing
			iTrace->Trace(_L("RunL leaved for Current Network change notification"),KTraceFileName, __LINE__);
		}	
		TRAP_IGNORE(iObserver.HandleCurrentNetworkChangeL(aError, ETrue));    
		
	return KErrNone;
	}

void COMASuplETelNotifier::CompareForRoamingL()
    {
    TInt64  currMNC=0;
    TLex LexcurrMNC(iNetworkInfo.iNetworkId);
    LexcurrMNC.Val(currMNC);

    TInt64  homeMNC=0;
    TLex LexhomeMNC(iHomeNetworkInfo.iNetworkId);
    LexhomeMNC.Val(homeMNC);

    if (currMNC != homeMNC)
        iObserver.HandleCurrentNetworkChangeL(KErrNone, EFalse);    
    else
        iObserver.HandleCurrentNetworkChangeL(KErrNone, ETrue);    
    }        

void COMASuplETelNotifier::CompeteRoamingCheckL()
    {
    TInt64  currMNC=0;
    TLex LexcurrMNC(iNetworkInfo.iNetworkId);
    LexcurrMNC.Val(currMNC);

    TInt64  homeMNC=0;
    TLex LexhomeMNC(iHomeNetworkInfo.iNetworkId);
    LexhomeMNC.Val(homeMNC);

    if (currMNC != homeMNC)
        iObserver.HandleRoamingCheckCompleteL(KErrNone, EFalse);    
    else
        iObserver.HandleRoamingCheckCompleteL(KErrNone, ETrue);    
    }        
//  End of File  
