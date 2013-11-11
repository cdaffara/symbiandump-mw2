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
* Description:  Implementation of CSuplSettingsEngine, in turn calls database handler methods
*
*/



// INCLUDE FILES
#include <centralrepository.h>
#include <etel.h>
#include <etelmm.h>
#include <commdb.h>

#include "epos_csuplsettings.h"
#include "epos_csuplsettingsengine.h"
#include "epos_msuplsettingsobserver.h"
#include "epos_csuplsettingsinternalcrkeys.h"
#include "epos_csuplsettingsconstants.h"
#include "epos_csuplsettingparams.h"
#include "epos_csuplsettingextensionparams.h"
#include "epos_csettingsdatabasehandler.h"
#include "epos_comasuplproductconfigcrkeys.h"

// CONSTANTS

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSettingsEngine::CSuplSettingsEngine():
    iState(EUndefined),
    iNetworkInfoPkg( iNetworkInfo),
    iInitDone(EFalse),
    iInitPassed(EFalse)
    {
    }

CSuplSettingsEngine* CSuplSettingsEngine::NewL()
    {
    CSuplSettingsEngine* self = new (ELeave) CSuplSettingsEngine();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// EPOC default constructor can leave.
void CSuplSettingsEngine::ConstructL()
    {
    iRepository = CRepository::NewL(KCRUidSuplSettings);
    iSettingsDBHandler = CSettingsDatabaseHandler::NewL();
    iImsi.Copy(_L(""));
    }

// Destructor
CSuplSettingsEngine::~CSuplSettingsEngine()
    {
    delete iRepository;
    iRepository = NULL;
    
    delete iSettingsDBHandler;
    iSettingsDBHandler = NULL;
    }
    
/*
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::LockEngineL()
//
//This method creates a concurrent read/write transaction.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::LockEngineL()
    {
    // Start transaction and add it to clean-up stack if no error
    TInt err = iRepository->StartTransaction(CRepository::EReadWriteTransaction);
    if (err != KErrNone)
        return err;
    //iRepository->CleanupRollbackTransactionPushL();
    return err;
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::UnLockEngine()
//
//This method commits a concurrent read/write transaction and pops
// the roll-back clean-up item from the stack
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::UnLockEngine(TBool aWithCommit)
    {
    if (aWithCommit)            
        {            
        // Commit transaction
        TUint32 changedKeys = 0;
        //CleanupStack::Pop(); // roll-back cleanup item
        TInt err = iRepository->CommitTransaction(changedKeys);
        return err; 
        }
    else
        {            
        iRepository->CancelTransaction();
        return KErrNone;
        }
    }
*/

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::Initialize()
//
// This method connects to ETEL, instantiates ETEL request
// active object.
//--------------------------------------------------------------------------------------
void CSuplSettingsEngine::Initialize(TRequestStatus& aStatus)
    {
        iStatus = &aStatus;

        if (IsInitDone() && IsInitPassed())
        {
        	User::RequestComplete(iStatus, KErrNone);
        	return;
        }
            

        TInt err2 = KErrNone;
        TRAPD(err, err2 = ConnectToEtelL()); 

        if (err != KErrNone || err2 != KErrNone)            
        {
        	User::RequestComplete(iStatus, KErrGeneral);
        	return;
        }

        iInitDone = EFalse; 
        iInitPassed = EFalse; 
        *iStatus = KRequestPending;

        TRequestStatus imsiStatus, netStatus;
        TInt etelErr = KErrNone;
        iState = EImsi;  //SMP Change
        iMobPhone.GetSubscriberId(imsiStatus, iImsi);
       
        User::WaitForRequest(imsiStatus);
        if (imsiStatus.Int() != KErrNone)
        {
            if (imsiStatus.Int() == KErrCancel)
                etelErr = KErrCancel;
            else            
                etelErr = KErrGeneral;
            iImsi.Copy(_L(""));
        }                
#if 0
        else
        {                
            iMobPhone.GetCurrentNetwork(netStatus, iNetworkInfoPkg, iLocationInfo);
            iState = ELoc;
            User::WaitForRequest(netStatus);
            if (netStatus.Int() != KErrNone)
            {                
                if (imsiStatus.Int() == KErrCancel)
                    etelErr = KErrCancel;
                else            
                    etelErr = KErrGeneral;
            }
            else
            {
                TInt    MCC=0;
                TInt64  MNC=0;
                TLex LexMCC(iNetworkInfo.iCountryCode);
                TLex LexMNC(iNetworkInfo.iNetworkId);
                LexMCC.Val(MCC);
                LexMNC.Val(MNC);

                iMcc = MCC;
                iMnc = MNC;
            }                    
        }            
#endif
        if (etelErr == KErrNone)
            iInitPassed = ETrue;    

        iInitDone = ETrue; 

        iMobPhone.Close(); 
	    iTelServer.UnloadPhoneModule(iTsyName);
	    iTelServer.Close();
	    iState = EUndefined;
        User::RequestComplete(iStatus, etelErr);
    }        

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::CancelInitialize()
//
//This method cancels initialization request. 
//--------------------------------------------------------------------------------------
void CSuplSettingsEngine::CancelInitialize()
    {
        switch(iState)
        {
            case EImsi:
                iMobPhone.CancelAsyncRequest(EMobilePhoneGetSubscriberId);
                break;
            case ELoc:
                iMobPhone.CancelAsyncRequest(EMobilePhoneGetCurrentNetwork);                
                break;
            default:
                break;
        }
    }            

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::ConnectToEtelL()
//
//This method connects to ETEL.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::ConnectToEtelL()
    {
   
    TInt err;            
    
    err = iTelServer.Connect();
    if(err != KErrNone)
		return err;

	CCommsDatabase* const db = CCommsDatabase::NewL(EDatabaseTypeUnspecified); 
	CleanupStack::PushL(db); 

	TUint32 modemId = 0; 
	db->GetGlobalSettingL(TPtrC(MODEM_PHONE_SERVICES_SMS), modemId); 
	CCommsDbTableView* const view = db->OpenViewMatchingUintLC(TPtrC(MODEM), TPtrC(COMMDB_ID), 
																modemId); 
	err = view->GotoFirstRecord(); 
	if(err != KErrNone)
	{
		CleanupStack::PopAndDestroy(2,db);			
		return err;	
	}
		
	view->ReadTextL(TPtrC(MODEM_TSY_NAME), iTsyName); 
	err = iTelServer.LoadPhoneModule(iTsyName);
	
	if(err != KErrNone)
	{
		CleanupStack::PopAndDestroy(2,db);		
		return err;
	}

	// For the phone information
	RTelServer::TPhoneInfo info;

	iTelServer.GetPhoneInfo(0, info); 
	err = iMobPhone.Open(iTelServer, info.iName);
	if(err != KErrNone)
	{
		CleanupStack::PopAndDestroy(2,db);		
		return err;
	}
	
	CleanupStack::PopAndDestroy(2,db);

	return KErrNone; 
    }        

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::IsInitDone()
//
//This method is used to check if the initialisation has been done  using Initialize()
//--------------------------------------------------------------------------------------
TBool CSuplSettingsEngine::IsInitDone()
    {
    if (iInitDone)
        return ETrue;
    else
        return EFalse;    
    }        

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::IsInitPassed()
//
//This method is used to check if the initialisation has been successful
//--------------------------------------------------------------------------------------
TBool CSuplSettingsEngine::IsInitPassed()
    {
    if (iInitPassed)
        return ETrue;
    else
        return EFalse;    
    }        

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetHslpAddressFromImsi()
//
// This method generates HSLP address from the IMSI retrieved from
// ETEL during initialization.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetHslpAddressFromImsi(TDes& aHslpAddress)
    {
    TBuf<3> mcc;
    TBuf<3> mnc;
    
     if (iImsi.Length()  < KMaxIMSILen)
    {
    	return KErrUnderflow;
    }
    RMobilePhone::TMobilePhoneSubscriberId imsi = iImsi;
	
	_LIT(KHslp1, "h-slp.mnc");
	_LIT(KHslp2, ".mcc");
    _LIT(KHslp3, ".pub.3gppnetwork.org");

	aHslpAddress.Copy(KHslp1);
		
    TPtrC ptr = imsi.Mid(0,3);
    mcc.Copy(ptr);
    TInt mccNum;
    TLex LexMCC(mcc);
    LexMCC.Val(mccNum);
    TInt mncLen = GetMncLen(mccNum);
    if (iMnc < 100)
    if (mncLen == 2)
    {
    TPtrC ptr2 = imsi.Mid(3,2);
    mnc.Copy(ptr2);
    _LIT(KNullChar, "0");
    aHslpAddress.Append(KNullChar);
    aHslpAddress.Append(mnc);
    }
    else
    {
    TPtrC ptr2 = imsi.Mid(3,3);
    mnc.Copy(ptr2);
    aHslpAddress.Append(mnc);
    }
	
    aHslpAddress.Append(KHslp2);
    aHslpAddress.Append(mcc);
    aHslpAddress.Append(KHslp3);
    
	return KErrNone;
    }        

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetImsi()
//
//This method retrieves currently active IMSI.  Currently active IMSI
// is retrieved during initialization time.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetImsi(TDes& aImsi)
    {
    aImsi.Copy(iImsi);
    return KErrNone;
    }    

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::IsImsiChanged()
//
//This method compares IMSI value currently stored in SUPL settings and
// currently active IMSI.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::IsImsiChanged(TBool& aChanged)
    {
    RBuf settingsImsi, etelImsi;

	TRAPD(err, settingsImsi.CreateL(15));
    if (err != KErrNone)
        return KErrGeneral;            
	TRAPD(err2, etelImsi.CreateL(15));
    if (err2 != KErrNone)
        return KErrGeneral;            
    
    err = iRepository->Get(KSuplSettingsIMSI, settingsImsi);
    if (err != KErrNone)
        {            
        settingsImsi.Close();
        etelImsi.Close();
        return err;
        }

    etelImsi.Copy(iImsi);

    if (etelImsi == settingsImsi)
        aChanged = EFalse;
    else
        aChanged = ETrue;    
    
    settingsImsi.Close();
    etelImsi.Close();
    return KErrNone;
    }    


/*
 * GetMccLen
 * In Symbian, there is no API which gives info on the
 * length of MNC value in the IMSI.  To achieve this
 * for the known MCC for which the MNC length could be
 * 3 is obtained from http://en.wikipedia.org/wiki/Mobile_Network_Code
 * and implemented here.
 */
TInt CSuplSettingsEngine::GetMncLen(TInt aMcc)
    {
    TInt mncLen;
    if (aMcc == 302 || 
        aMcc == 310 || 
        aMcc == 311 || 
        aMcc == 316 || 
        aMcc == 338 || 
        aMcc == 342 || 
        aMcc == 344 || 
        aMcc == 348 || 
        aMcc == 346 || 
        aMcc == 356 || 
        aMcc == 358 || 
        aMcc == 360 || 
        aMcc == 364 || 
        aMcc == 366 || 
        aMcc == 372 || 
        aMcc == 467 || 
        aMcc == 722 || 
        aMcc == 732 || 
        aMcc == 736)
        mncLen = 3;
    else
        mncLen = 2;    
    
    return mncLen;
    }     

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetServerAddress()
//
//This method is used to change server address parameter in server settings
//--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetServerAddress( const TInt64 aSlpId, const TDesC& aServerAddress)
     {
     CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EServerAddress;
     TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aServerAddress));
     if(err != KErrNone)
         return err;
     return KErrNone;

     }    
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetServerAddress()
 //
 //This method is used to retrieve server address parameter from server settings.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetServerAddress(const TInt64 aSlpId, TDes& aServerAddress) const
    {
    return iSettingsDBHandler->GetServerAddress(aSlpId,aServerAddress);
    }       
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetVersion()
 //
 //This method is used to change protocol version supported by SUPL server.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetVersion( const TInt64 aSlpId, const TInt aMajor, const TInt aMinor )
     {
     TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,aMajor,aMinor));
     if(err != KErrNone)
         return err;
     return KErrNone;          
     }   
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetVersion()
 //
 //This method is used to retrieve protocol version supported by SUPL server. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetVersion( const TInt64 aSlpId, TInt& aMajor, TInt& aMinor ) const
    {
    return iSettingsDBHandler->GetVersion(aSlpId,aMajor,aMinor);
    }  
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetIapName()
 //
 //This method is used to change IAP Name in server parameters. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetIapName( const TInt64 aSlpId, const TDesC& aIapName)
     {
     CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EIap;
     TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aIapName));
     if(err != KErrNone)
         return err;
     return KErrNone;
     }  
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetIapName()
 //
 //This method is used to retrieve IAP Name in server parameters.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetIapName( const TInt64 aSlpId, TDes& aIapName) const
    {
    return iSettingsDBHandler->GetIAPName(aSlpId,aIapName);    
    }      
 
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetManufacturerName()
//
//This method is used to change manufacturer in server parameters.
//--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetManufacturerName( const TInt64 aSlpId, const TDesC& aManufacturer)
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EManufacturer;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aManufacturer));
    if(err != KErrNone)
            return err;
        return KErrNone;
    }     
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetManufacturerName()
 //
 //This method is used to retrieve manufacturer name in server parameters. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetManufacturerName( const TInt64 aSlpId, TDes& aManufacturerName) const
    {
    return iSettingsDBHandler->GetManufacturer(aSlpId,aManufacturerName);   
    }    
 
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetLastUseTime()
//
//This method is used to change timestamp when the HSLP was used last time.
//--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetLastUseTime( const TInt64 aSlpId, const TTime aTime )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ETimeLastUse;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aTime));
    if(err != KErrNone)
            return err;
        return KErrNone;
    }     
 
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetLastUseTime()
//
//This method is used to retrieve timestamp when the SLP was used last time. 
//--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetLastUseTime( const TInt64 aSlpId, TTime& aTime ) const
    {
    return iSettingsDBHandler->GetLastUsedTime(aSlpId,aTime);
    }     
 
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetLastTriedTime()
//
//This method is used to change timestamp when the HSLP was used tried last time.
//--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetLastTriedTime( const TInt64 aSlpId, const TTime aTime )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ETimeLastUse;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aTime));    
    if(err != KErrNone)
            return err;
        return KErrNone;
    }    
 
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetLastTriedTime()
//
//This method is used to retrieve timestamp when the HSLP was used tried last time.
//--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetLastTriedTime( const TInt64 aSlpId, TTime& aTime ) const
    {
    return iSettingsDBHandler->GetLastTriedTime(aSlpId,aTime);               
    }    
 
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetNetInfoLastUse()
//
//This method is used to change network information when the SUPL session was last done.
//--------------------------------------------------------------------------------------       
 TInt CSuplSettingsEngine::SetNetInfoLastUse( const TInt64 aSlpId, const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType, const TInt aMcc, const TInt aMnc, const TInt aCid, const TInt aLac)
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ENetInfoLastUse;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aNetType,aMcc,aMnc,aCid,aLac));
    if(err != KErrNone)
        return err;
    return KErrNone;
    }            

 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetNetInfoLastUse()
 //
 //This method is used to retrieve network information when the SUPL session was last done.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetNetInfoLastUse( const TInt64 aSlpId, CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType, TInt& aMcc, TInt& aMnc, TInt& aCid, TInt& aLac) const
     {
     CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ENetInfoLastUse;
     TRAPD(err,iSettingsDBHandler->GetNwInfoPropertiesL(aSlpId,parameter,aNetType,aMcc,aMnc,aCid,aLac));
     if(err != KErrNone)
         return err;
     return KErrNone;        
     }  
 
//-------------------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetNetInfoLastSucess()
//
//This method is used to change network information when the SUPL session was last done sucessfully.
//--------------------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::SetNetInfoLastSucess( const TInt64 aSlpId, const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType, const TInt aMcc, const TInt aMnc, const TInt aCid, const TInt aLac)
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ENetInfoLastSucess;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aNetType,aMcc,aMnc,aCid,aLac));
    if(err != KErrNone)
        return err;
    return KErrNone;
    }   

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetNetInfoLastSucess()
//
//This method is used to retrieve network information when the SUPL session was last done sucessfully.
//--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetNetInfoLastSucess( const TInt64 aSlpId, CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType, TInt& aMcc, TInt& aMnc, TInt& aCid, TInt& aLac) const        
     {
     CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ENetInfoLastSucess;
     TRAPD(err,iSettingsDBHandler->GetNwInfoPropertiesL(aSlpId,parameter,aNetType,aMcc,aMnc,aCid,aLac));
     if(err != KErrNone)
         return err;
     return KErrNone;
     
     } 
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetEmergencySupportFlag()
 //
 //This method is used to change parameter which indicates whether server
 // supports emergency service or not. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetEmergencySupportFlag( const TInt64 aSlpId, const TBool aEmergencySupportFlag )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EEmergencySupport;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aEmergencySupportFlag));
    if(err != KErrNone)
            return err;
        return KErrNone;
    }  
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetEmergencySupportFlag()
 //
 //This method is used to retrieve parameter which indicates whether server
 // supports emergency service or not.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetEmergencySupportFlag( const TInt64 aSlpId, TBool& aEmergencySupportFlag ) const
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EEmergencySupport;
    TRAPD(err,iSettingsDBHandler->GetOtherPropertiesL(aSlpId,parameter,aEmergencySupportFlag));
    if(err != KErrNone)
            return err;
        return KErrNone;
    }  
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetTlsSupportFlag()
 //
 //This method is used to change parameter which indicates whether server
 // supports TLS or not.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetTlsSupportFlag( const TInt64 aSlpId, const TBool aTlsSupportFlag )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ETlsEnabled;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aTlsSupportFlag)); 
    if(err != KErrNone)
            return err;
        return KErrNone;
    }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetPskTlsSupportFlag()
 //
 //This method is used to change parameter which indicates whether server
 // supports PSK-TLS or not.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetPskTlsSupportFlag(const TInt64 aSlpId,const TBool aPskTlsSupportFlag)
     {
     CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EPskTlsUsed;
     TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aPskTlsSupportFlag)); 
     if(err != KErrNone)
         return err;
     return KErrNone;
     }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetPskTlsFlag()
 //
 //This method is used to retrieve parameter which indicates whether server
 // supports PSK-TLS or not. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetPskTlsFlag( const TInt64 aSlpId, TBool& aPskTlsFlag ) const
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EPskTlsUsed;
    TRAPD(err,iSettingsDBHandler->GetOtherPropertiesL(aSlpId,parameter,aPskTlsFlag));
    if(err != KErrNone)
            return err;
        return KErrNone;
           
    }     
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetTlsFlag()
 //
 //This method is used to retrieve parameter which indicates whether server
 // supports TLS or not.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetTlsFlag( const TInt64 aSlpId, TBool& aTlsFlag ) const
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ETlsEnabled;
    TRAPD(err,iSettingsDBHandler->GetOtherPropertiesL(aSlpId,parameter,aTlsFlag));
    if(err != KErrNone)
            return err;
        return KErrNone;
           
    }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetServerEnabledFlag()
 //
 //This method is used to change parameter which indicates whether server
 // can be used for SUPL session or not. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetServerEnabledFlag( const TInt64 aSlpId, const TBool aEnable )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EServerEnabled;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aEnable));
    if(err != KErrNone)
            return err;
        return KErrNone;
    }   
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetServerEnabledFlag()
 //
 //This method is used to retrieve parameter which indicates whether server
 // can be used for SUPL session or not. 
 //--------------------------------------------------------------------------------------       
 TInt CSuplSettingsEngine::GetServerEnabledFlag( const TInt64 aSlpId, TBool& aEnable ) const
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EServerEnabled;
    TRAPD(err,iSettingsDBHandler->GetOtherPropertiesL(aSlpId,parameter,aEnable));
    if(err != KErrNone)
            return err;
        return KErrNone;
           
    }     
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetSimChangeRemoveFlag()
 //
 //This method is used to change parameter which indicates whether server
 //details can be removed if SIM is changed.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetSimChangeRemoveFlag( const TInt64 aSlpId, const TBool aSimChangeFlag )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ESimChangeRemove;
    TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aSimChangeFlag)); 
    if(err != KErrNone)
            return err;
        return KErrNone;
    }   
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetSimChangeRemoveFlag()
 //
 //This method is used to retrieve parameter which indicates whether server
 // details can be removed if SIM is changed. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetSimChangeRemoveFlag( const TInt64 aSlpId, TBool& aSimChangeFlag ) const        
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::ESimChangeRemove;
    TRAPD(err,iSettingsDBHandler->GetOtherPropertiesL(aSlpId,parameter,aSimChangeFlag)); 
    if(err != KErrNone)
            return err;
        return KErrNone;
    }  
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetUsageInHomwNwFlag()
 //
 // This method is used to change parameter which indicates whether server
 // can be used out side home network or not.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetUsageInHomwNwFlag( const TInt64 aSlpId, const TBool aHomeNwFlag )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EUsageInHomeNw;
    TRAPD(err, iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aHomeNwFlag));  
    if(err != KErrNone)
            return err;
        return KErrNone;
    }     
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetUsageInHomwNwFlag()
 //
 //This method is used to retrieve parameter which indicates whether server
 // can be used out side home network or not.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetUsageInHomwNwFlag( const TInt64 aSlpId, TBool& aHomeNwFlag ) const
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EUsageInHomeNw;
    TRAPD(err,iSettingsDBHandler->GetOtherPropertiesL(aSlpId,parameter,aHomeNwFlag));
    if(err != KErrNone)
            return err;
        return KErrNone;
    }    
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetEditableFlag()
 //
 //This method is used to change parameter which indicates whether server
 // details are editable or not. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetEditableFlag( const TInt64 aSlpId, const TBool aEditFlag )
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EEditable;
    TRAPD(err, iSettingsDBHandler->UpdateSLPRecordL(aSlpId,parameter,aEditFlag));  
    if(err != KErrNone)
            return err;
        return KErrNone;
    }  
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetEditableFlag()
 //
 //This method is used to retrieve parameter which indicates whether server
 // details are editable or not.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetEditableFlag( const TInt64 aSlpId, TBool& aEditFlag ) const        
    {
    CSuplSettingsInternal::TServerParam parameter = CSuplSettingsInternal::EEditable;
    TRAPD(err,iSettingsDBHandler->GetOtherPropertiesL(aSlpId,parameter,aEditFlag));
    if(err != KErrNone)
            return err;
        return KErrNone;
    }  
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetAsDefault()
 //
 //This method is used to set one server as default in SUPL settings storage.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetAsDefault( const TInt64 aSlpId ) 
    {
    TRAPD(err,iSettingsDBHandler->SetSLPAsDefaultL(aSlpId));
    if(err != KErrNone)
        return err;
    return KErrNone;
    }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetDefaultSlpId()
 //
 //This method is used to retrieve default SLP identification.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetDefaultSlpId( TInt64& aSlpId) const
     {
     iSettingsDBHandler->GetDefaultSLPId(aSlpId);
     if(aSlpId == -1)
         return KErrNotFound;
     return KErrNone;             
     }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::RemoveServer()
 //
 //This method is deletes server entry from SUPL settings storage.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::RemoveServer(TInt64 aSlpId)
     {
     TRAPD(err,iSettingsDBHandler->DeleteSLPRecordL(aSlpId));
     if(err != KErrNone)
         return err;
     return KErrNone;
     }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::ChangePriority()
 //
 // This method is changes the priority of the server to the specified priority.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::ChangePriority(TInt64 aSlpId,TInt aPriority,TBool aDirection)
     {
     TRAPD(err,iSettingsDBHandler->ChangePriorityL(aSlpId,aPriority,aDirection));
     if(err != KErrNone)
         return err;
     return KErrNone;           
     }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::AddNewServer()
 //
 //This method is used to add new server into settings.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::AddNewServer( const CServerParams* aParamValues, TInt64& aSlpId )
     {
     TRAPD(err,iSettingsDBHandler->InsertSLPRecordL(aParamValues,aSlpId)); 
     if(err != KErrNone)
         return err;
     return KErrNone;
     }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetAllParameter()
 //
 //This method is used to change all parameters of server in SUPL settings.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetAllParameter( const CServerParams* aParamValues)
     {
     TRAPD(err,iSettingsDBHandler->UpdateSLPRecordL(aParamValues));
     if(err != KErrNone)
         return err;
     return KErrNone;
     }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetDefaultServer()
 //
 //This method is used to change all parameters of default server in SUPL settings.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetDefaultServer( const CServerParams* aParamValues)        
     {
     TRAPD(err,iSettingsDBHandler->UpdateDefaultServerL(aParamValues)); 
     if(err != KErrNone)
         return KErrGeneral;
     return KErrNone;
     }

 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetFallBack()
 //
 //This method enables or disables the fall back in SUPL settings storage.
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetFallBack(TBool aFallBack)
    {
    TInt err;
    TInt count = 0;
    iSettingsDBHandler->SlpCount(count);
    if(count <= 1) //single server entry exists
        return KErrGeneral;
    err = iRepository->Set(KSuplSettingsFallBack, aFallBack);
    if (err != KErrNone)
        return KErrGeneral;            

    return KErrNone;            
    }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetFallBack()
 //
 //This method retrieves the fallback value from SUPL settings storage 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetFallBack(TBool& aFallBack) const
    {
    TInt err;            

    err = iRepository->Get(KSuplSettingsFallBack, aFallBack);
    if (err != KErrNone)
        return KErrGeneral;            

    return KErrNone;
               
    }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::SetFallBackTimer()
 //
 //This method sets the fall back timer in SUPL settings storage. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::SetFallBackTimer(TInt aTimerValue)
    {
    TInt err;

    err = iRepository->Set(KSuplSettingsFallBackTimer, aTimerValue);
    if (err != KErrNone)
        return KErrGeneral;            

    return KErrNone;            
    }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetFallBackTimer()
 //
 //This method gets the fall back timer from SUPL settings storage. 
 //--------------------------------------------------------------------------------------
 TInt CSuplSettingsEngine::GetFallBackTimer(TInt& aTimerValue) const
    {
    TInt err;            

    err = iRepository->Get(KSuplSettingsFallBackTimer, aTimerValue);
    if (err != KErrNone)
        return KErrGeneral;            

    return KErrNone;
               
    }
 
 //-------------------------------------------------------------------------------------
 //CSuplSettingsEngine::GetSlpInfoFromId()
 //
 //This method is used to retrieve all properties of server from SUPL settings storage
 // based on SLP identification.
 //--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetSlpInfoFromId(const TInt64 aSlpId,CServerParams* aParamValues) const
    {
    return iSettingsDBHandler->GetSlpInfoFromId(aSlpId,aParamValues);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetSlpInfoAddress()
//
//This method is used to retrieve all properties of server from SUPL settings storage
//based on SLP address.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetSlpInfoAddress(const TDesC& aServerAddress,CServerParams* aParamValues) const
    {
    return iSettingsDBHandler->GetSlpInfoFromAddress(aServerAddress,aParamValues);
    }

//-------------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetDefaultServer()
//
//This method is used to retrieve all properties of default server from SUPL settings storage.
//-------------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetDefaultServer(CServerParams* aParamValues) const
    {
    return iSettingsDBHandler->GetDefaultServer(aParamValues);
    }

//---------------------------------------------------------------------------------
//CSuplSettingsEngine::SlpCount()
//
//This method is used to retrieve number of server entries present in SUPL settings.
//-----------------------------------------------------------------------------------
TInt CSuplSettingsEngine::SlpCount(TInt& aCount)
    {
    return iSettingsDBHandler->SlpCount(aCount);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::IsSlpExists()
//
//This method is used check whether server entry exists or not based on SLP
// identification 
//--------------------------------------------------------------------------------------
TBool CSuplSettingsEngine::IsSlpExists(const TInt64 aSlpId)
    {
    return iSettingsDBHandler->IsSlpExists(aSlpId);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::IsSlpExists()
//
//This method is used check whether server entry exists or not based on SLP
// address
//--------------------------------------------------------------------------------------
TBool CSuplSettingsEngine::IsSlpExists(const TDesC& aServerAddress)
    {
    return iSettingsDBHandler->IsSlpExists(aServerAddress);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetAllSlp()
//
//This synchronous method is used to retrieve properties of all servers from 
//SUPL settings storage.
//-------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetAllSlp(RPointerArray<CServerParams>& aParamValues) 
    {

    TRAPD(err,iSettingsDBHandler->GetAllSLPL(aParamValues));
    if(err != KErrNone)
        return err;
    return KErrNone;
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetAllSlp()
//
//This method is used to retrieve properties of all servers from SUPL settings storage.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetAllSlp(RPointerArray<CServerParams>& aParamValues,TRequestStatus& aStatus) 
    {
    TRAPD(err,iSettingsDBHandler->GetAllSLPL(aParamValues,aStatus));
    if(err != KErrNone)
        return err;
    return KErrNone;
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::RemoveOnSimChange()
//
//This method is used to delete all servers which have the Sim Change Remove Flag set
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::RemoveOnSimChange()
    {
    TRAPD(err,iSettingsDBHandler->RemoveOnSimChangeL());
       if(err != KErrNone)
           return KErrGeneral;
       return KErrNone;
    }
    
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetTriggerParams()
//
//This method is used to retrieve properties of all active triggers. 
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetTriggerParams(RPointerArray<CTriggerParams>& aParamValues) const
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->GetAllSessionsL(aParamValues));
    return err;
    }
    
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetTriggerParams()
//
//This method is used to retrieve properties of a given active trigger. 
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetTriggerParams(TInt64 aSessionId,
                                           CTriggerParams*& aParamValues) const
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->GetSessionL(aSessionId,aParamValues));
    return err;
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetNotificationStatus()
//
//Sets Notification status of particular session
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::SetNotificationStatus( TInt64 aSessionId,
                								 TBool aTriggerNotificationStatus )
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->UpdateSessionRecordL(aSessionId,CSuplSettingsInternal::ETriggerNotificationStatus,aTriggerNotificationStatus));
    return err;
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetOutstandingTriggers()
//
//Sets number of outstanding triggers for a particular session
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::SetOutstandingTriggers( TInt64 aSessionId,
                                                  TUint64 aOutstandingTriggers )
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->UpdateSessionRecordL(aSessionId,CSuplSettingsInternal::EOutstandingTriggers,aOutstandingTriggers));
    return err;
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::CancelTriggerSession()
//
//Cancels ongoing triggering session
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::CancelTriggerSession( TInt64 aSessionId )
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->DeleteSessionRecordL(aSessionId));
    return err;
    }
    
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::InsertSessionRecord()
//
//This method is used to insert Active session record into the SUPL session DB
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::InsertSessionRecord(const CTriggerParams* aSessionParamValues)
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->InsertSessionRecordL(aSessionParamValues));
    return err;
    }
    
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::DeleteSessionRecord()
//
//This method is used to delete a session record corresponding to the given session ID
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::DeleteSessionRecord(TInt64 aSessionId)
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->DeleteSessionRecordL(aSessionId));
    return err;
    } 

//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::DeleteAllSessionRecords()
//
//This method is used to delete all session records from session table
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::DeleteAllSessionRecords()
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->DeleteAllSessionRecordsL());
    return err;
    }
    
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::GetServerExtensionParams()
//
//This method is used to retrieve extended parameters of given server
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::GetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams ) const
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->GetServerExtensionParamsL( aSlpId,aServerExtnParams ));
    return err;
    }
    
//-------------------------------------------------------------------------------------
//CSuplSettingsEngine::SetServerExtensionParams()
//
//This method is used to set extended parameters of a given server.
//--------------------------------------------------------------------------------------
TInt CSuplSettingsEngine::SetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams )
    {
    TInt err = KErrNone;
    TRAP(err,iSettingsDBHandler->UpdateServerExtensionParamsL( aSlpId,aServerExtnParams ));
    return err;
    }          
//---------------------------------------------------------------------
// CSuplSettings::GetDefaultIAPName()
//
// Returns the default configured IAP name
//---------------------------------------------------------------------
TInt  CSuplSettingsEngine::GetDefaultIAPName(TDes& aIapName)
    {
    CRepository* productConfigRepository = NULL;
    aIapName.Copy(KNullDesC);
    TRAPD(err,productConfigRepository = CRepository::NewL(KCRUidProductConfig));
    if(err == KErrNone)
        {
        err = productConfigRepository->Get(KDefaultAutoGenIAP, aIapName);
        delete productConfigRepository;
        }
    return err;
    }  
	
	//---------------------------------------------------------------------
// SetSuplTriggeredServiceStatus()
//
// Changes status of SUPL triggering service. 
//---------------------------------------------------------------------    
 TInt CSuplSettingsEngine::SetSuplTriggeredServiceStatus( const CSuplSettings::TSuplTriggerStatus& aSuplTriggerStatus )
	{
		TInt val;
    if( aSuplTriggerStatus == CSuplSettings::ESuplTriggerOff) //Can be casted to enum directly...
    {
    		val = 1;
    }
    else
    {
    		val = 0;
    }
    return  iRepository->Set(KSuplSettingsTriggerServiceStatus, val);
	}   
	 
//---------------------------------------------------------------------
// GetSuplTriggeredServiceStatus()
//
// Gets status of SUPL triggering service.SUPL Triggering service can be either ESuplTriggerOn or ESuplTriggerOff
//---------------------------------------------------------------------    
 TInt CSuplSettingsEngine::GetSuplTriggeredServiceStatus( CSuplSettings::TSuplTriggerStatus& aSuplTriggerStatus )
	{
		TInt val;
    TInt err = iRepository->Get(KSuplSettingsTriggerServiceStatus,val);
    
    if( val == 1) //Can be casted to enum directly...
    {
    	aSuplTriggerStatus = CSuplSettings::ESuplTriggerOff;
    }
    else
    {
    		aSuplTriggerStatus = CSuplSettings::ESuplTriggerOn;
    }
    
    return err;
	}
	
//End of File
