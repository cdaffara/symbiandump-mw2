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


#include <centralrepository.h>

#include "epos_comasuplprotocolmanager1.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_csuplsettingsinternal.h"

#include "epos_omasuplconstants.h"
#include "epos_comasuplconnrequestor.h"
#include "epos_momasuplconnobserver.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplfallbackhandler.h"
#include "epos_csuplsettingsinternalcrkeys.h"


#include "epos_comasupldialogtimer.h"
_LIT(KTraceFileName,"SUPL_OMA_SESSION::epos_comasuplconnrequestor.cpp");



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::COMASuplConnRequestor
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplConnRequestor::COMASuplConnRequestor(CSuplCommunicationManager& aCommMgr,
        COMASUPLProtocolManager1& aProtoMgr, 
        TUint aPort, 
        MOMASuplConnObserver& aObserver):
        CActive( EPriorityStandard ),
        iCommMgr(aCommMgr), 
        iProtocolManager(aProtoMgr),
        iPort(aPort),
        iObserver( aObserver),iPrompt(EFalse),iWlanOnly(EFalse),iIsStaleLocIdPresent(EFalse)
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
    iRepository = CRepository::NewL(KCRUidSuplSettings);
    iLastConnectionError = KErrNone;

    iCurrentSLPId = KErrNotFound;

    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//


COMASuplConnRequestor* COMASuplConnRequestor::NewL( CSuplCommunicationManager& aCommMgr,COMASUPLProtocolManager1& aProtoMgr,  
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
    delete iRepository;
        iRepository = NULL;
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::CreateConnectionL()
    {
	if(!iIsSettingInitilized)
        {
		iState = EInitialState;
        InitilizeSetting();
        }
    else
        {
		TBuf<100> iapName;
		TBuf<128> buffer;
    iState = EConnecting;  
		TBool isIapDialogShown = EFalse;
		TInt errorCode = iFallBackHandler->GetNextSLPAddressL(iCurrentSLPId,iHostAddress,iapName,iTls,iPskTls,iLastConnectionError,isIapDialogShown);
		
		if(errorCode == KErrNone)
			{
				TBool ret = ConvertIAPNameToIdL(iapName,iIAPId);
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
// COMASuplConnRequestor::CreateConnection for OCC
// -----------------------------------------------------------------------------
//    
void COMASuplConnRequestor::CreateConnectionL(TBool aPrompt,TBool aWlanOnly)
    {
        TBuf<30> GeoTagServerName;
        TBuf<128> buffer;
        iState = EConnecting;  
       
        iPrompt = aPrompt;
        iWlanOnly = aWlanOnly;
        iIsStaleLocIdPresent = ETrue;
               
        TInt err;
        err = iRepository->Get(KSuplGeoInfoConvServerName, GeoTagServerName);
        User::LeaveIfError(err);
        buffer.Copy(_L("Connecting to"));
        buffer.Append(GeoTagServerName);
        iTrace->Trace(buffer,KTraceFileName, __LINE__);                 
        iTls = ETrue;
        iPskTls = EFalse;
        iIAPId = 0;
        
        
        iConnection = iCommMgr.CreateConnectionL(GeoTagServerName,iTls,iPskTls,iPort,iIAPId);
       
        OpenConnection();
        
    }

// -----------------------------------------------------------------------------
// COMASuplConnRequestor::OpenConnection
// -----------------------------------------------------------------------------
//  TODO : Remove this method  
void COMASuplConnRequestor::CreateConnectionL(TInt /*aDialogTimeOutDelay*/)
    {
    CreateConnectionL();
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
        iState = ESending;
	    iConnection->Send(aPacket,iStatus);
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
            if(iIsStaleLocIdPresent)
                {
                iTrace->Trace(_L("OpenConnection OCC"),KTraceFileName, __LINE__);
                iConnection->Connect(iStatus,iPrompt,iWlanOnly);
                }
            else
                {
                iConnection->Connect(iStatus);
                }
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
            CreateConnectionL();
            
            break;
            }

        case  EConnecting:
            {
            if(iLastConnectionError < 0)
                {
                iHostAddress.Zero();
                CloseConnection();
                if(iIsStaleLocIdPresent)
                    {
                    iTrace->Trace(_L("Request completed with error..."), KTraceFileName, __LINE__);       
                    iObserver.OperationCompleteL(iLastConnectionError);
                    }
                else
                    {
                    iTrace->Trace(_L("Setting API Initilizing Completed..."), KTraceFileName, __LINE__);       
                    CreateConnectionL();
                    }
                
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

TUint COMASuplConnRequestor::GetPortNumber()
	{
	if(iConnection)
		return iConnection->GetPortNumberUsed();
	else 
		return 0;
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


//  End of File
