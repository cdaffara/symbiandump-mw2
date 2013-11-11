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
* Description:   Class for doing asynchronous service.
*
*/



#include "epos_comasuplprotocolmanager2.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_csuplsettingsinternal.h"

#include "epos_omasuplconstants.h"
#include "epos_comasuplconnrequestor.h"
#include "epos_momasuplconnobserver.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplfallbackhandler.h"

#include "epos_comasupldialogtimer.h"
_LIT(KTraceFileName,"SUPL_OMA_SESSION::epos_comasuplconnrequestor.cpp");



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::COMASuplConnRequestor
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplConnRequestor::COMASuplConnRequestor(CSuplCommunicationManager& aCommMgr,
											 COMASUPLProtocolManager2& aProtoMgr, 
											 TUint aPort, 
											 MOMASuplConnObserver& aObserver):
    				 						 CActive( EPriorityStandard ),
    				 						 iCommMgr(aCommMgr), 
    				 						 iProtocolManager(aProtoMgr),
    				 						 iPort(aPort),
    				 						 iObserver( aObserver),
    				 						 iTriggerSession(EFalse)
    {
			
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
//void COMASuplConnRequestor::ConstructL(const TDesC8& aHostAddr)
void COMASuplConnRequestor::ConstructL()
    {
    	CActiveScheduler::Add( this );
    	
    	iTLSAuthenticationFailed = EFalse;
    	iSuplSettings = CSuplSettingsInternal::NewL();
    	iTrace = COMASuplTrace::NewL();
    	iIsHSLPGenerated = EFalse;
        iHslpAddrFromImsiUsed = EFalse;
    	
    	iFallBackHandler = COMASuplFallBackHandler::NewL(*iSuplSettings);
    	
    	iLastConnectionError = KErrNone;
    	
    	iCurrentSLPId = KErrNotFound;
    	
    
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//


COMASuplConnRequestor* COMASuplConnRequestor::NewL( CSuplCommunicationManager& aCommMgr,COMASUPLProtocolManager2& aProtoMgr,  
													TUint aPort,MOMASuplConnObserver& aObserver)

    {
	    COMASuplConnRequestor* self =  new ( ELeave ) COMASuplConnRequestor(aCommMgr,aProtoMgr,aPort,
	    																	aObserver);	    																	
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }


// -----------------------------------------------------------------------------
// COMASuplConnRequestor::COMASuplConnRequestor
// -----------------------------------------------------------------------------
//
COMASuplConnRequestor::~COMASuplConnRequestor()
    {
    	Cancel();
    
	  	delete iSuplSettings;
    	delete iTrace;
    	delete iFallBackHandler;
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::CreateConnectionL(TBool aTriggerSession)
	{
	if(!iIsSettingInitilized)
        {
        iTriggerSession = aTriggerSession;
				iState = EInitialState;
        InitilizeSetting();
        }
    else
        {
		TBuf<100> iapName;
		TBuf<128> buffer;
		iState = EConnecting;  
		TInt errorCode = KErrNone;
		
		TBool isIapDialogShown = EFalse;
		if (!aTriggerSession)
			errorCode = iFallBackHandler->GetNextSLPAddressL(iCurrentSLPId,iHostAddress,iapName,iTls,iPskTls,iLastConnectionError,isIapDialogShown);
		
		if(errorCode == KErrNone)
			{
                TBool ret = ETrue;
                if (!aTriggerSession)
				    ret = ConvertIAPNameToIdL(iapName,iIAPId);
                buffer.Copy(_L("ConvertIAPNameToIdL error code:"));
                buffer.AppendNum(ret);
                iTrace->Trace(buffer,KTraceFileName, __LINE__); 
                if(!ret)
					{
						buffer.Copy(_L("No access point configured for "));
						buffer.Append(iHostAddress);
						iTrace->Trace(buffer,KTraceFileName, __LINE__); 				
						buffer.Copy(_L("Calling CreateConnection with no IAP"));
						iTrace->Trace(buffer,KTraceFileName, __LINE__);
						iConnection = iCommMgr.CreateConnectionL(iHostAddress,iTls,iPskTls,iPort,-1);
						OpenConnection();
						
					}
				else
				    {
				    buffer.Copy(_L("Connecting to "));
				    buffer.Append(iHostAddress);
				    iTrace->Trace(buffer,KTraceFileName, __LINE__); 				
				    iConnection = iCommMgr.CreateConnectionL(iHostAddress,iTls,iPskTls,iPort,iIAPId);
				    OpenConnection();
				    }	
            }	
		else
			{
				iHostAddress.Zero();
				iObserver.OperationCompleteL(errorCode);
			}
	}
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//  TODO : Remove this method  
void COMASuplConnRequestor::CreateConnectionL(TInt /*aDialogTimeOutDelay*/, TBool aTriggerSession)
    {
    CreateConnectionL(aTriggerSession);
    }
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
TInt COMASuplConnRequestor::GetLocalIPAddress(TInetAddr &aAddress)
	{
		if(iConnection)
		{
			return iConnection->GetLocalIPAddress(aAddress);	
		}
		
		return KErrGeneral;
		
	}

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SendPacket
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::SendPacket(const TDesC8& aPacket)
    {
      if(iConnection)
    	{
	    	iConnection->Send(aPacket,iStatus);
    		iState = ESending;
			SetActive();
	  	}
	}
	
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::OpenConnection()
	{
	
	if(iIsSettingInitilized)
		{
			if(iConnection)
			{
				iState = EConnecting;
				iConnection->Connect(iStatus);
				SetActive();
			}
		}
		else
		{
		iState = EInitilizeSetting;
			InitilizeSetting();
		}
	}
    
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::ListenToMessages
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::ListenToMessages()
	{
		if(iConnection)
			{
				iConnection->ListenToMessages();
			}
	}


// -----------------------------------------------------------------------------
// COMASuplConnRequestor::CloseConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::CloseConnection()
	{
		if(iConnection )
		    {
		    iConnection->CloseConnection();           
		iCommMgr.DestroyConnection(iConnection);
		    iConnection = NULL;
		    }
	}

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::RunL
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::RunL()
    {
    	iLastConnectionError = iStatus.Int();
    	
    	TBuf<64> buffer(_L("Error during connection : "));
			buffer.AppendNum(iLastConnectionError);
			iTrace->Trace(buffer,KTraceFileName, __LINE__); 					
    	
    	switch(iState)
    		{
    		
		case EInitialState:
            {
            iIsSettingInitilized = ETrue;
            iTrace->Trace(_L("Setting API Initilizing Completed..."), KTraceFileName, __LINE__);
            CreateConnectionL(iTriggerSession);
            
            break;
            }
			case  EConnecting:
				{
					if(iLastConnectionError < 0)
					{
						iHostAddress.Zero();
						CloseConnection();
						CreateConnectionL(EFalse);
					}
					else
					{
            			ListenToMessages();                            
						iObserver.OperationCompleteL(iLastConnectionError);
					}
					break;
				}
    		case EInitilizeSetting:
    			{
    				if(iLastConnectionError == KErrNone)
    					{
    						iIsSettingInitilized = ETrue;
    						iTrace->Trace(_L("Setting API Initilizing Completed..."), KTraceFileName, __LINE__); 				
    						OpenConnection();
    					}
    				else
    					{
    						iTrace->Trace(_L("Setting API Initilizing Failed..."), KTraceFileName, __LINE__); 				
    						iObserver.OperationCompleteL(iLastConnectionError);
    					}	
    				break;
    			}
   			
			case ESending:
    			{
    				iObserver.OperationCompleteL(iLastConnectionError);
    				break;
    			}
    		}
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::RunError
// -----------------------------------------------------------------------------
//
TInt COMASuplConnRequestor::RunError(TInt /* aError */)
    {
     return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::DoCancel()
    {    
     	switch(iState)
    	{
    		case EConnecting:
    		    if ( iConnection )
    			    iConnection->CancelConnect(iStatus);
    			break;
    		case ESending:
                if ( iConnection )	
    			    iConnection->CancelSend(iStatus);
    			break;
    		case EInitilizeSetting:
    			iSuplSettings->CancelInitialize();
    		default:
    			break;
    	} 
    }
     
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::CancelReadOperation()
    {    
    	if(iConnection)
    	{
    		iConnection->CancelListenToMessages();
    	}
    }
    

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SetIAPID
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::SetIAPID(TInt aIAPID)
    {
    iIAPId = aIAPID;
    }

	
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::InitilizeSetting
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::InitilizeSetting()
	{
			iTrace->Trace(_L("Intilizing Setting API..."), KTraceFileName, __LINE__); 				
			
			iSuplSettings->Initialize(iStatus);
			SetActive();
	}

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::IsHslpGenerated
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::IsHslpGenerated(TBool aIsGenerated)
	{
	iIsHSLPGenerated = aIsGenerated;
	}	

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::IsHslpAddrFromImsiUsed
// -----------------------------------------------------------------------------
//
TBool COMASuplConnRequestor::IsHslpAddrFromImsiUsed()
	{
	return iHslpAddrFromImsiUsed;
	}	
	
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SetDefaultParametersL
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::SetDefaultParametersL(const TDesC& aClientProvidedHSLPAddress,TBool aFallBackAllowed)	
	{
		iFallBackHandler->SetFallBackParametersL(aClientProvidedHSLPAddress,aFallBackAllowed);	
	}

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SetDefaultParametersL
// -----------------------------------------------------------------------------
//
void COMASuplConnRequestor::GetUsedServerAddress(TDes& aHSLPAddressUsed)	
	{
		aHSLPAddressUsed.Copy(iHostAddress);
	}
	
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::ConvertIAPNameToIdL
// 
// -----------------------------------------------------------------------------
TBool COMASuplConnRequestor::ConvertIAPNameToIdL(const TDesC& aIAPName, TUint32& aIAPId)
	{
		TBool result = EFalse;
		CCommsDatabase* commDb = CCommsDatabase::NewL(EDatabaseTypeIAP);
		CleanupStack::PushL(commDb);
		CCommsDbTableView* tableView = commDb->OpenIAPTableViewMatchingBearerSetLC(ECommDbBearerCSD|ECommDbBearerGPRS,
											ECommDbConnectionDirectionOutgoing);
	   	TInt retval = tableView->GotoFirstRecord();
	   	while ((retval == KErrNone) && (!result))
	   	{
	      HBufC * iap_name = tableView->ReadLongTextLC( TPtrC( COMMDB_NAME) );

	      iap_name->Des().LowerCase();
	      	      
	      if (iap_name && (iap_name->Compare(aIAPName) == 0))
	        {
	           	tableView->ReadUintL(TPtrC(COMMDB_ID), aIAPId);
	            result = ETrue;
	       	}
	       CleanupStack::PopAndDestroy(); // iap_name
	       retval = tableView->GotoNextRecord();
	   	}
		CleanupStack::PopAndDestroy(2);//delete tableView and commDb
		return result;
	}
	


// -----------------------------------------------------------------------------
// COMASuplConnRequestor::SaveAccessPoint
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::SaveAccessPoint(const TDesC& aIapName)
	{
		TInt err = iSuplSettings->SetIapName(iCurrentSLPId,aIapName);
	}

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UpdateNetInfo
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UpdateNetInfo(const TTime& aLastTimeUsed,
										  TInt& aMcc,
							              TInt& aMnc,
							              TInt& aCid, 
							              TInt& aLac,
							              TInt& aType,
							              TInt& aErrorCode 
							              )
	{
        iTrace->Trace(_L("COMASuplConnRequestor:UpdateNetInfo start"), KTraceFileName, __LINE__); 
	
		CSuplSettingsInternal::TSuplSettingsNetworkType type = (CSuplSettingsInternal::TSuplSettingsNetworkType)aType; 

		if(aErrorCode == KErrNone)
			{
				 iSuplSettings->SetNetInfoLastSucess(iCurrentSLPId,type,aMcc,aMnc,aCid,aLac);
				 iSuplSettings->SetLastUseTime(iCurrentSLPId,aLastTimeUsed);
			}
		else
			{
				 iSuplSettings->SetNetInfoLastUse(iCurrentSLPId,type,aMcc,aMnc,aCid,aLac);
				 iSuplSettings->SetLastTriedTime(iCurrentSLPId,aLastTimeUsed);
			}	
        iTrace->Trace(_L("COMASuplConnRequestor:UpdateNetInfo end"), KTraceFileName, __LINE__); 
		
	}
// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UseDefaultServerL
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UseDefaultServerL()
    {
    iFallBackHandler->ReadSLPListForNIL(KNullDesC);
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UseServerL
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UseServerL(const TDesC& aClientProvidedHSLPAddress)
{
			iFallBackHandler->ReadSLPListForNIL(aClientProvidedHSLPAddress);
}

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::UseServerL
// 
// -----------------------------------------------------------------------------
void COMASuplConnRequestor::UpdateSLPListForHomeUsage(TBool aHomeNetwork)
{
			iFallBackHandler->UpdateSLPListForHomeUsage(aHomeNetwork);
}


TUint COMASuplConnRequestor::GetPortNumber()
	{
		if(iConnection)
			return iConnection->GetPortNumberUsed();
		else 
			return 0;
	}
	
	
//  End of File
