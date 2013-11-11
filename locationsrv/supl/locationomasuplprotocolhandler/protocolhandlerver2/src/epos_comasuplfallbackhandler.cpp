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
* Description:   Class for handling fallback in case of multiple SLP
*
*/



#include "epos_csuplsettingsinternal.h"
#include "epos_comasuplfallbackhandler.h"
#include "epos_omasuplconstants.h"
#include "epos_momasuplconnobserver.h"
#include "epos_comasupltrace.h"
#include "epos_csuplsettingparams.h"
#include "epos_csuplsettingextensionparams.h"
#include "epos_csuplsettingsinternalconstants.h"

#include <networking/dnd_err.h>   //Error codes
#include <etelpckt.h> //Error codes
#include <exterror.h>

_LIT(KTraceFileName,"SUPL_OMA_SESSION::epos_comasuplfallbackhandler.cpp");



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::COMASuplFallBackHandler
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplFallBackHandler::COMASuplFallBackHandler(CSuplSettingsInternal& aSettings):
												iSuplSettings(aSettings),
												iAttemptedBackupServer(EFalse)
    {

    }

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplFallBackHandler::ConstructL()
    {
    	iCurrentServerCounter = 0;
    	iTLSAuthenticationFailed = EFalse;
    	iErrorCode = KErrNone;
    	
    	iSLPList = new (ELeave) CArrayFixFlat<TSLPDetails>(16);
    	
    	iTrace = COMASuplTrace::NewL();
		
		 TInt err = iSuplSettings.GetFallBackTimerValue(iAllowedFallBackTimerValue);
		 if(err != KErrNone)
		 	{
		 		iAllowedFallBackTimerValue = 200000000;
		 	}
		 	
		TBuf<64> buffer(_L("Fallback Timer Value in Seconds : "));
		buffer.AppendNum(iAllowedFallBackTimerValue);
		iTrace->Trace(buffer,KTraceFileName, __LINE__); 				

    }

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplFallBackHandler* COMASuplFallBackHandler::NewL( CSuplSettingsInternal& aSettings)
    {
	    COMASuplFallBackHandler* self =  new ( ELeave ) COMASuplFallBackHandler(aSettings);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::COMASuplFallBackHandler
// -----------------------------------------------------------------------------
//
COMASuplFallBackHandler::~COMASuplFallBackHandler()
    {
    	delete iTrace;
    	if(iSLPList)
    	    {
    	    iSLPList->Reset();
    	    }
    	delete iSLPList;
    	iSLPList=NULL;
    	iGenratedHslpAddress.Close();
    	iDefaultIAPName.Close();
    }

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::GetNextSLPAddressL
// -----------------------------------------------------------------------------
//    TODO: Merge if conditions...
TInt COMASuplFallBackHandler::GetNextSLPAddressL(TInt64& aSLPId, TDes& aHslpToBeUsedAddress,TDes& aIAPName,TBool& aTls,
                                                 TBool& aPskTls,TInt aLastErrorCode,TBool& aIsIapDialogShown)
    {
	iTrace->Trace(_L("COMASuplFallBackHandler::GetNextSLPAddressL"),KTraceFileName, __LINE__);
	
    _LIT(KFormatTxt,"%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%:3%+B"); 
    
    TBuf<256> LogBuffer;
    
     if( aLastErrorCode != KErrNone)// If its KErrNone then its means server name is asked first time and no need for Tls failure check
    	{
    	
    	if( CheckErrorCodeL(aLastErrorCode) )
        	{
        
        	if( iTLSAuthenticationFailed )
        		{
        		if(iErrorCode == KErrNone)
            		{	
            		aHslpToBeUsedAddress.Copy(iGenratedHslpAddress);
            		aIAPName.Zero();   
            		aIAPName.Copy(iDefaultIAPName);
            		aTls = ETrue;
            		aPskTls = EFalse;
            		aIsIapDialogShown = ETrue;
					
					CServerParams* param = CServerParams::NewL();
					CleanupStack::PushL(param);
					
					//Find out if this alternative generated SUPL server is in the SUPL Settings list
					TInt err = iSuplSettings.GetSlpInfoAddress(iGenratedHslpAddress, param);
					if(err == KErrNotFound)
						{
						//Server does not exist
						iTrace->Trace(_L("Server does not exist in list so adding it in."),KTraceFileName, __LINE__);
						CServerParams* newParam=CServerParams::NewL();
						CleanupStack::PushL(newParam);
						User::LeaveIfError(newParam->Set( iGenratedHslpAddress,iDefaultIAPName,ETrue,ETrue,ETrue,EFalse ));

						err = iSuplSettings.AddNewServer( newParam, aSLPId ); //Ignore error
						LogBuffer.Copy(_L("AddNewServer() completed with err: "));
						LogBuffer.AppendNum(err);
						iTrace->Trace(LogBuffer,KTraceFileName, __LINE__); 		
						
						CleanupStack::PopAndDestroy(newParam);
						}
					else
						{
						//Server exists, get the SLP ID and the IAP Access point for this server
						HBufC* hslpAddress =  HBufC::NewL(KHSLPAddressLength);       
						HBufC* iapName =  HBufC::NewL(KMaxIapNameLength);
						TBool serverEnabled;
						TBool simChangeRemove;
						TBool usageInHomeNw;
						TBool editable;
						
						iTrace->Trace(_L("Server already exists, getting the SLP ID and Access Point."),KTraceFileName, __LINE__);
						
						param->Get(aSLPId,hslpAddress->Des(),iapName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
						aIAPName.Copy(iapName->Des());
						
						delete hslpAddress;
						delete iapName;
						}
					
					CleanupStack::PopAndDestroy(param);
                                
            		iTrace->Trace(_L("Fallback allowed & TLSAuth failed"),KTraceFileName, __LINE__);
            		iTrace->Trace(_L("Server being used:"),KTraceFileName, __LINE__);
            		LogBuffer.Copy(aHslpToBeUsedAddress);
            		iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
                                                                        
            		iTrace->Trace(_L("iap being used:"),KTraceFileName, __LINE__);
            		LogBuffer.Copy(aIAPName);
            		iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
                                                                        
            		iTrace->Trace(_L("iap dlg configuration:"),KTraceFileName, __LINE__);
            		LogBuffer.Delete(0,256);
            		LogBuffer.AppendNum(aIsIapDialogShown);
            		iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
            		return KErrNone;
            		}
        		else
            		{
            		return KErrArgument;        
            		}
            	}
       	 	}
        
     	else
	     	{
	     	return KErrNotFound;
	     	}  
	 	}

	//Check to see if the server list is empty.  If it is create the HSLP Address from the IMSI and use that
	// as the server address.  This does not add the server to the list and this functionality should only
	// be tried once
	if(iSLPList->Count() <= 0 && (!iAttemptedBackupServer))
		{
		iTrace->Trace(_L("Going to create and use alternative HSLP Address from IMSI"),KTraceFileName, __LINE__);

		iAttemptedBackupServer = ETrue;

		//Generate the HSLP Address
		GenerateHslpAddressFromIMSIL();

		//Copy the generated address into the supplied function arguments
		aHslpToBeUsedAddress.Copy(iGenratedHslpAddress);
		aIAPName.Zero();   
		aIAPName.Copy(iDefaultIAPName);
		aTls = ETrue;
		aPskTls = EFalse;
		aIsIapDialogShown = ETrue;

		iTrace->Trace(_L("Server being used:"),KTraceFileName, __LINE__);
		LogBuffer.Copy(aHslpToBeUsedAddress);
		iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);

		iTrace->Trace(_L("iap being used:"),KTraceFileName, __LINE__);
		LogBuffer.Copy(aIAPName);
		iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
		
		//Add the server to the list for future connections
		CServerParams* param=CServerParams::NewL();
		CleanupStack::PushL(param);
		User::LeaveIfError(param->Set( iGenratedHslpAddress,iDefaultIAPName,ETrue,ETrue,ETrue,EFalse ));

		TInt err = iSuplSettings.AddNewServer( param, aSLPId ); //Ignore error
		LogBuffer.Copy(_L("AddNewServer() completed with err: "));
		LogBuffer.AppendNum(err);
		iTrace->Trace(LogBuffer,KTraceFileName, __LINE__); 		
		
		CleanupStack::PopAndDestroy(&param);

		return KErrNone;
		}
    
    if( iSLPList->Count() <= 0 || iCurrentServerCounter >= iSLPList->Count() ) 
        {
        LogBuffer.Copy(_L("No more servers available..."));
        iTrace->Trace(LogBuffer,KTraceFileName, __LINE__); 				
        return KErrNotFound;
        }
             
    iTrace->Trace(_L("Trying next server..."),KTraceFileName, __LINE__); 				
             
    //Fallback Timer...
    if(aLastErrorCode == KErrNone) //This means, server name is asked for first time only...
        {
        iTrace->Trace(_L("Start Time: "),KTraceFileName, __LINE__); 				
        iFallBackStartTime.HomeTime();
        LogBuffer.Zero();
        iFallBackStartTime.FormatL(LogBuffer,KFormatTxt); 
        iTrace->Trace(LogBuffer,KTraceFileName, __LINE__); 				
        }
    else
        {
        TTime currentTime;
        currentTime.HomeTime();
        TTimeIntervalMicroSeconds diff = currentTime.MicroSecondsFrom(iFallBackStartTime); 
                        
                        
        iTrace->Trace(_L("Current Time: "),KTraceFileName, __LINE__); 				
        LogBuffer.Zero();
        currentTime.FormatL(LogBuffer,KFormatTxt); 
        iTrace->Trace(LogBuffer,KTraceFileName, __LINE__); 				
                        
        LogBuffer.Copy(_L("Difference between time: "));
        LogBuffer.AppendNum(diff.Int64());
        iTrace->Trace(LogBuffer,KTraceFileName, __LINE__); 				
                        
        if( diff.Int64() > iAllowedFallBackTimerValue)	
            {
            LogBuffer.Copy(_L("Timeout happened..."));
            iTrace->Trace(LogBuffer,KTraceFileName, __LINE__); 		
            
            return KErrTimedOut;
            }
        }	
                
    if(aLastErrorCode == KErrNone)  //This will be called only once...for first time only
        {
        aSLPId = (*iSLPList)[0].iSLPId;
        aHslpToBeUsedAddress.Copy((*iSLPList)[0].iHSLPAddress);
        aIAPName.Copy((*iSLPList)[0].iIapName);
        aTls = (*iSLPList)[0].iTls;
        aPskTls = (*iSLPList)[0].iPskTls;
        aIsIapDialogShown = (*iSLPList)[0].iIsIAPDialgShown;
        iCurrentServerCounter++;				
                    
        iTrace->Trace(_L("Server being used:"),KTraceFileName, __LINE__);
        LogBuffer.Copy(aHslpToBeUsedAddress);
        iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
                    
        iTrace->Trace(_L("iap being used:"),KTraceFileName, __LINE__);
        LogBuffer.Copy(aIAPName);
        iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
                    
        iTrace->Trace(_L("iap dlg configuration:"),KTraceFileName, __LINE__);
        LogBuffer.Delete(0,256);
        LogBuffer.AppendNum(aIsIapDialogShown);
        iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
                    
        return KErrNone;
        }
    else
        {
        if(iIsFallBackAllowed && CheckErrorCodeL(aLastErrorCode))// Do not fallback in case if IMSI generated address failed for previous try.
            {
            aSLPId = (*iSLPList)[iCurrentServerCounter].iSLPId;	
            aHslpToBeUsedAddress.Copy((*iSLPList)[iCurrentServerCounter].iHSLPAddress);
            aIAPName.Copy((*iSLPList)[iCurrentServerCounter].iIapName);    			
            aTls = (*iSLPList)[iCurrentServerCounter].iTls;
            aPskTls = (*iSLPList)[iCurrentServerCounter].iPskTls;
            aIsIapDialogShown = (*iSLPList)[iCurrentServerCounter].iIsIAPDialgShown;
            iCurrentServerCounter++;
            iTrace->Trace(_L("Fallback allowed & TLSAuth passed"),KTraceFileName, __LINE__);
            iTrace->Trace(_L("Server being used:"),KTraceFileName, __LINE__);
            LogBuffer.Copy(aHslpToBeUsedAddress);
            iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
                                                                   
            iTrace->Trace(_L("iap being used:"),KTraceFileName, __LINE__);
            LogBuffer.Copy(aIAPName);
            iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
                                                                    
            iTrace->Trace(_L("iap dlg configuration:"),KTraceFileName, __LINE__);
            LogBuffer.Delete(0,256);
            LogBuffer.AppendNum(aIsIapDialogShown);
            iTrace->Trace(LogBuffer,KTraceFileName, __LINE__);
            return KErrNone;
            }
        else
            {
            return KErrNotFound;
            }
        }
                    
    }
    
// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::CheckErrorCodeL
// -----------------------------------------------------------------------------
//    
TBool COMASuplFallBackHandler::CheckErrorCodeL(TInt aLastErrorCode)
{
	
		if(iTLSAuthenticationFailed)
		{
			iTrace->Trace(_L("IMSI Generated SLP also failed after TLS faliure for previous SLP..."), KTraceFileName, __LINE__); 				
			return EFalse;		
				
		}
		if ( (aLastErrorCode >= KErrSSLAlertNoRenegotiation ) && (aLastErrorCode <= KErrSSLAlertCloseNotify)  && !iTLSAuthenticationFailed )
		{
			iTrace->Trace(_L("TLS Authentication Failed..."), KTraceFileName, __LINE__); 				
			iTLSAuthenticationFailed = ETrue;	
			GenerateHslpAddressFromIMSIL();
			return ETrue;
		}
		
		if ( (aLastErrorCode >= KErrDndServerUnusable ) && (aLastErrorCode <= KErrDndNameNotFound) )
		{
			iTrace->Trace(_L("DNS Error..."), KTraceFileName, __LINE__); 				
			return ETrue;		
		}
		
		if ( (aLastErrorCode >= KErrUrgentData  ) && (aLastErrorCode <= KErrNetUnreach ) )
		{
			iTrace->Trace(_L("Sockets Error..."), KTraceFileName, __LINE__); 				
			return ETrue;		
		}

		if  (aLastErrorCode == KErrCouldNotConnect  )
		{
			iTrace->Trace(_L("Could not connect to Server..."), KTraceFileName, __LINE__); 				
			return ETrue;		
		}

		if  (aLastErrorCode == KErrSuplTimeout  )
		{
			iTrace->Trace(_L("Timeout for DNS/TLS operation..."), KTraceFileName, __LINE__); 				
			return ETrue;		
		}
		
		if( aLastErrorCode == KErrGsmMMServiceOptionNotSubscribed )
		{
		   iTrace->Trace(_L("GSM MM Service Option Not Subscribed ..."), KTraceFileName, __LINE__);                 
		   return ETrue;  
	    }
		if ( (aLastErrorCode >= KErrGprsFeatureNotSupported ) && (aLastErrorCode <= KErrGsmRRUnspecifedAbnormalRelease) )
		{
			iTrace->Trace(_L("GPRS Service is not allowed/rejected/not supported..."), KTraceFileName, __LINE__); 				
			return EFalse;		
		}


	return EFalse;
}

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::ReadSLPListL
// -----------------------------------------------------------------------------
//    
void COMASuplFallBackHandler::ReadSLPListL(const TDesC& aClientProvidedHSLPAddress)
{
	  	RPointerArray<CServerParams> SLPList;
	  
		TBool isSLPEntryExist = EFalse;
		
    	TBool serverEnabled,simChangeRemove,usageInHomeNw,editable;
    	
		TInt err = iSuplSettings.GetAllSlp(SLPList);
		TInt cnt = SLPList.Count();
		
		TSLPDetails	 SlpEntry;
		TSLPDetails  FirstSlpEntry;
		HBufC* dmConfigurableIapFirstEntry =  HBufC::NewL(KMaxIapNameLength);
		TPtr dmConfigIap(NULL,KMaxIapNameLength);
		dmConfigIap.Set(dmConfigurableIapFirstEntry->Des());
		HBufC* hslpAddress =  HBufC::NewL(KHSLPAddressLength);       
		HBufC* iapName =  HBufC::NewL(KMaxIapNameLength);   
		HBufC* dmConfigurableIapName =  HBufC::NewL(KMaxIapNameLength); 
		
		HBufC* serverid =  HBufC::NewL(KMaxServerIdLen); 
		HBufC* iap2 =  HBufC::NewL(KMaxIapNameLength); 
		HBufC* serveradd =  HBufC::NewL(KMaxServerAddLen); 
		HBufC* serveraddtype =  HBufC::NewL(KMaxServerAddTypeLen); 
		
		iSLPList->Reset(); // Reset the list
		for(TInt i = 0; i < cnt; i++ )
		{
				TInt64 SLPId;	
			   	CServerParams* params = SLPList[i];
			   	params->Get(SLPId,hslpAddress->Des(),iapName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
			   	
			   	CServerExtensionParams* extnparams = CServerExtensionParams::NewL();
			   	TInt err = iSuplSettings.GetServerExtensionParams(SLPId,extnparams); // Ignore error
			   	extnparams->Get(serverid->Des(),dmConfigurableIapName->Des(),iap2->Des(),serveradd->Des(),serveraddtype->Des() );
			   	
		   	    delete extnparams; 
			   	SlpEntry.iHSLPAddress.LowerCase();
			   	
			   	TBuf<KHSLPAddressLength> clientHSLP;
			   	clientHSLP.Copy(aClientProvidedHSLPAddress);
			   	clientHSLP.LowerCase();
			   	
			   	SlpEntry.iSLPId = SLPId;
			   	SlpEntry.iHSLPAddress.Copy(hslpAddress->Des());
			   	SlpEntry.iIapName.Copy(iapName->Des());
			   	SlpEntry.iIapName.LowerCase();
			   	SlpEntry.iHomeNetworkUsage = usageInHomeNw;
			   	
			   	iSuplSettings.GetTlsFlag(SLPId,SlpEntry.iTls);
			   	iSuplSettings.GetPskTlsFlag(SLPId,SlpEntry.iPskTls);
			   	SlpEntry.iHSLPAddress.LowerCase();
			   	SlpEntry.iIsIAPDialgShown=ETrue;//To identify its a entry with primary iap


		   	if(!isSLPEntryExist && (SlpEntry.iHSLPAddress.Compare(clientHSLP) == KErrNone) )// Client provided SLP will be on top of list.
		   	{
		   		if(serverEnabled)   // Following is to handle specical case where client specified SLP is disabled and fallback not allowed...
				{
				 FirstSlpEntry = SlpEntry;
				 // Store the 2ndary iap if exists
				if( dmConfigurableIapName->Length() > 0 )
				    {
				    dmConfigIap=dmConfigurableIapName->Des();
				    }
				else
				    {
				    dmConfigIap.Zero();
				    }
				
				isSLPEntryExist = ETrue;
				} 		
        		  else
				  {
						if(iIsFallBackAllowed)
						{
							continue;
						}
			   		  	else
			   		  	{
			   		  			isSLPEntryExist = EFalse;
			   		  			iSLPList->Reset();
			   		  			break;
			   		  	}
                  } 	
		   	}
		   	else
		   	{
		   		if(serverEnabled)
		   		{
				iSLPList->AppendL(SlpEntry);
				if( dmConfigurableIapName->Length() > 0 )
                    {
                    // Replace default iap field with 2ndary iap field
                    SlpEntry.iIapName.Copy(dmConfigurableIapName->Des());
                    SlpEntry.iIapName.LowerCase();
                    // Set IapDialog flag to false for entry with secondary iap
                    SlpEntry.iIsIAPDialgShown=EFalse;
                    // Append this entry to the list
                    iSLPList->AppendL(SlpEntry);
                    }
							
				}	
			}
				
		}
		
		if(isSLPEntryExist)
		{
		iSLPList->InsertL(0,FirstSlpEntry);
		if( dmConfigIap.Length() > 0 )
		    {
		    FirstSlpEntry.iIapName.Copy(dmConfigIap);
		    SlpEntry.iIapName.LowerCase();
		    FirstSlpEntry.iIsIAPDialgShown=EFalse;
		    iSLPList->AppendL(FirstSlpEntry);
		    }
		}
			
		delete serverid;
		delete iap2;
		delete serveradd;
		delete serveraddtype;
		
		delete dmConfigurableIapFirstEntry;
		delete hslpAddress;
		delete iapName;
		delete dmConfigurableIapName;
		SLPList.ResetAndDestroy();
		
		SLPList.Close();
		
}

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::GenerateHslpAddressFromIMSIL
// -----------------------------------------------------------------------------
//
void COMASuplFallBackHandler::GenerateHslpAddressFromIMSIL()
	{
	    iGenratedHslpAddress.Close();
	    iDefaultIAPName.Close();
	    iGenratedHslpAddress.CreateL(KHSLPAddressLength);
	    iDefaultIAPName.CreateL(KMaxIapNameLength);
		TInt err = iSuplSettings.GenerateHslpAddressFromImsi(iGenratedHslpAddress);
		if(err != KErrNone)
			{
				iTrace->Trace(_L("GenerateHslpAddressFromImsi Failed..."), KTraceFileName, __LINE__); 				
				iErrorCode = err;				
			}
		else
			{
				TBuf<256> msg(_L("New Generated H-SLP address: "));
				msg.Append(iGenratedHslpAddress);
				iTrace->Trace(msg, KTraceFileName, __LINE__); 		
				iErrorCode = KErrNone;
				err = iSuplSettings.GetDefaultIAPName(iDefaultIAPName);
				if(err != KErrNone)
				    {
                    iTrace->Trace(_L("GetDefaultIAPName Failed..."), KTraceFileName, __LINE__);               
                    iErrorCode = err;
				    }
				else
				    {
				    msg.Copy(iDefaultIAPName);
                    iTrace->Trace(msg, KTraceFileName, __LINE__);       
                    iErrorCode = KErrNone;
				    }
			}
	}

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::SetFallBackParametersL
// -----------------------------------------------------------------------------
//
void COMASuplFallBackHandler::SetFallBackParametersL(const TDesC& aClientProvidedHSLPAddress,TBool aFallBackAllowed)
	{
		
		TBool userSetFallback = ETrue;
		iSuplSettings.GetFallBack(userSetFallback);		
		iIsFallBackAllowed = aFallBackAllowed & userSetFallback;
		
		TBuf<64> buffer(_L("Final Fallback Value... "));
		buffer.AppendNum(iIsFallBackAllowed);
		iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
		
		ReadSLPListL(aClientProvidedHSLPAddress);			
		// Reset the counter
		iCurrentServerCounter=0;
		
		PrintSLPList();
	}
	
// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::PrintSLPList()
// -----------------------------------------------------------------------------
//
void COMASuplFallBackHandler::PrintSLPList()
{
//This function can be put under debug flag...	
	TBuf<256> hslpAddress;
	TBuf<100> accessPoint;
	TInt64 slpid;
	TBool Psk,Tls,HomeUsage;
	TBuf<256> buffer;
	TInt cnt = iSLPList->Count();	
	
	buffer.Copy(_L("Number of Entries : "));
	buffer.AppendNum(cnt);
	iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
	
	for(TInt Counter = 0; Counter < cnt; Counter++)
	{
				slpid = (*iSLPList)[Counter].iSLPId;	
				hslpAddress.Copy((*iSLPList)[Counter].iHSLPAddress);
				accessPoint.Copy((*iSLPList)[Counter].iIapName);    			
				Tls = (*iSLPList)[Counter].iTls;
				Psk = (*iSLPList)[Counter].iPskTls;
				HomeUsage = (*iSLPList)[Counter].iHomeNetworkUsage;
				
				buffer.Copy(_L("==================================="));
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
				
				buffer.Copy(_L("SLP id : "));
				buffer.AppendNum(slpid);
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
				
				buffer.Copy(_L("Server Name: "));
				buffer.Append(hslpAddress);
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
				
				buffer.Copy(_L("Access Point: "));
				buffer.Append(accessPoint);
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
				
				buffer.Copy(_L("TLS : "));
				buffer.AppendNum(Tls);
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
				
				buffer.Copy(_L("PSK TLS : "));
				buffer.AppendNum(Psk);
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
				
				buffer.Copy(_L("Home Usage : "));
				buffer.AppendNum(HomeUsage);
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
				
	}
	
	if(cnt > 0)
	{
				buffer.Copy(_L("==================================="));
				iTrace->Trace(buffer,KTraceFileName, __LINE__); 	
	}
	
}

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::ReadSLPListForNIL
// -----------------------------------------------------------------------------
//    
void COMASuplFallBackHandler::ReadSLPListForNIL(const TDesC& aClientProvidedHSLPAddress)
{

		TBuf<64> msg;
		iSLPList->Reset();//Clear previous list if any
		// Reset the counter
		iCurrentServerCounter=0;
		iIsFallBackAllowed=ETrue;// IAP Fallback is allowed within a SLP
 		
		TInt err;
 		if(aClientProvidedHSLPAddress.Length() == 0)
 		{
			GetFirstEnabledSLPL();
			msg.Copy(_L("Using Default SLP for SUPL_INIT"));
			iTrace->Trace(msg,KTraceFileName, __LINE__); 
			PrintSLPList();
			return;
		}
		else
		{
		 		CServerParams* params =  CServerParams::NewL();
				err = iSuplSettings.GetSlpInfoAddress(aClientProvidedHSLPAddress,params);
				msg.Copy(_L("Using SLP provided in SUPL_INIT"));
				iTrace->Trace(msg,KTraceFileName, __LINE__); 
				if(err != KErrNone)
				{
					msg.Copy(_L("Error in getting Parameters : "));
					msg.AppendNum(err);
		            iTrace->Trace(msg,KTraceFileName, __LINE__); 
		            delete params; 
					return;
				}
			
				CleanupStack::PushL(params);		
		    	TBool serverEnabled,simChangeRemove,usageInHomeNw,editable;
				
				TSLPDetails	 SlpEntry;
				
				HBufC* hslpAddress =  HBufC::NewL(KHSLPAddressLength);		
				CleanupStack::PushL(hslpAddress);
				
				HBufC* iapName =  HBufC::NewL(KMaxIapNameLength);		
				CleanupStack::PushL(iapName);
				
				TInt64 SLPId;	
		   	
		  	 	params->Get(SLPId,hslpAddress->Des(),iapName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
		   	
		  	 	HBufC* dmConfigurableIapName =  HBufC::NewL(KMaxIapNameLength); 
		  	 	HBufC* serverid =  HBufC::NewL(KMaxServerIdLen); 
		  	 	HBufC* iap2 =  HBufC::NewL(KMaxIapNameLength); 
		  	 	HBufC* serveradd =  HBufC::NewL(KMaxServerAddLen); 
		  	 	HBufC* serveraddtype =  HBufC::NewL(KMaxServerAddTypeLen); 
			   	
		  	 	CServerExtensionParams* extnparams = CServerExtensionParams::NewL();
		  	 	err = iSuplSettings.GetServerExtensionParams(SLPId,extnparams); // Ignore error
		  	 	extnparams->Get(serverid->Des(),dmConfigurableIapName->Des(),iap2->Des(),serveradd->Des(),serveraddtype->Des() );
		  	 	delete extnparams; 
		  	 	
		  	 	if(serverEnabled)
			   	{
					   	SlpEntry.iSLPId = SLPId;
					   	SlpEntry.iHSLPAddress.Copy(hslpAddress->Des());
					   	SlpEntry.iIapName.Copy(iapName->Des());
					   	SlpEntry.iIapName.LowerCase();
					   	SlpEntry.iHomeNetworkUsage = usageInHomeNw;
					   	
					   	iSuplSettings.GetTlsFlag(SLPId,SlpEntry.iTls);
					   	iSuplSettings.GetPskTlsFlag(SLPId,SlpEntry.iPskTls);
					   	SlpEntry.iIsIAPDialgShown=ETrue;
					   	iSLPList->AppendL(SlpEntry);
					   	if( dmConfigurableIapName->Length() > 0 )
                            {
                            // Replace default iap field with 2ndary iap field
                            SlpEntry.iIapName.Copy(dmConfigurableIapName->Des());
                            SlpEntry.iIapName.LowerCase();
                            // Set IapDialog flag to false for entry with secondary iap
                            SlpEntry.iIsIAPDialgShown=EFalse;
                            // Append this entry to the list
                            iSLPList->AppendL(SlpEntry);
                            }
					   	
			   	}
				
				CleanupStack::PopAndDestroy(3);//params,hslpAddress iapName	   	
		        delete dmConfigurableIapName;
		        delete serverid;
		        delete iap2;
		        delete serveradd;
		        delete serveraddtype;
		}
		PrintSLPList();
}

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::UpdateSLPListForHomeUsage
// -----------------------------------------------------------------------------
//    
void COMASuplFallBackHandler::UpdateSLPListForHomeUsage(TBool aHomeNetwork)
{
	if(!aHomeNetwork)//User is in Roaming...
	{
				TInt TotalServers = iSLPList->Count();	
				TInt cnt = 0;
				while( cnt < TotalServers )
				{
					TInt  hwUsageInt = (*iSLPList)[cnt].iHomeNetworkUsage;
					
					
					if (hwUsageInt)
					{
						(*iSLPList).Delete(cnt);
						TotalServers--;
					}
					else
					{
							cnt++;
					}
				}
	}		
		iTrace->Trace(_L("After roaming check..."),KTraceFileName, __LINE__); 	
		PrintSLPList();
		
}

// -----------------------------------------------------------------------------
// COMASuplFallBackHandler::GetFirstEnabledSLPL
// -----------------------------------------------------------------------------
//    
void COMASuplFallBackHandler::GetFirstEnabledSLPL()
{
		RPointerArray<CServerParams> SLPList;
	  
    	TBool serverEnabled,simChangeRemove,usageInHomeNw,editable;
    	
		TInt err = iSuplSettings.GetAllSlp(SLPList);
		TInt cnt = SLPList.Count();
		HBufC* hslpAddress =  HBufC::NewL(KHSLPAddressLength);		
		CleanupStack::PushL(hslpAddress);
		HBufC* iapName =  HBufC::NewL(KMaxIapNameLength);		
		CleanupStack::PushL(iapName);
		
		HBufC* dmConfigurableIapName =  HBufC::NewL(KMaxIapNameLength); 
		HBufC* serverid =  HBufC::NewL(KMaxServerIdLen); 
		HBufC* iap2 =  HBufC::NewL(KMaxIapNameLength); 
		HBufC* serveradd =  HBufC::NewL(KMaxServerAddLen); 
		HBufC* serveraddtype =  HBufC::NewL(KMaxServerAddTypeLen); 
		                
		CServerExtensionParams* extnparams = CServerExtensionParams::NewL();
			
		for(TInt i = 0; i < cnt; i++ )
		{
				TInt64 SLPId;	
			   	CServerParams* params = SLPList[i];
			   	params->Get(SLPId,hslpAddress->Des(),iapName->Des(),serverEnabled,simChangeRemove,usageInHomeNw,editable);
			   	if(serverEnabled)
		   		{
					   	TSLPDetails	 SlpEntry;
					   	SlpEntry.iSLPId = SLPId;
					   	SlpEntry.iHSLPAddress.Copy(hslpAddress->Des());
					   	SlpEntry.iIapName.Copy(iapName->Des());
					   	SlpEntry.iIapName.LowerCase();
					   	SlpEntry.iHomeNetworkUsage = usageInHomeNw;
						iSuplSettings.GetTlsFlag(SLPId,SlpEntry.iTls);
						iSuplSettings.GetPskTlsFlag(SLPId,SlpEntry.iPskTls);
						SlpEntry.iIsIAPDialgShown=ETrue;
						iSLPList->AppendL(SlpEntry);
						
						err = iSuplSettings.GetServerExtensionParams(SLPId,extnparams); // Ignore error
						extnparams->Get(serverid->Des(),dmConfigurableIapName->Des(),iap2->Des(),serveradd->Des(),serveraddtype->Des() );
						
						if( dmConfigurableIapName->Length() > 0 )
                            {
                            // Replace default iap field with 2ndary iap field
                            SlpEntry.iIapName.Copy(dmConfigurableIapName->Des());
                            SlpEntry.iIapName.LowerCase();
                            // Set IapDialog flag to false for entry with secondary iap
                            SlpEntry.iIsIAPDialgShown=EFalse;
                            // Append this entry to the list
                            iSLPList->AppendL(SlpEntry);
                            }
						
						break;
				}	
		}
		
		CleanupStack::PopAndDestroy(2);//hslpAddress iapName	   	
		delete extnparams; 
		delete dmConfigurableIapName;
		delete serverid;
		delete iap2;
		delete serveradd;
		delete serveraddtype;
		SLPList.ResetAndDestroy();
		SLPList.Close();
}





