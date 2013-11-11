/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Client-Server communication
*
*/

#include <s32mem.h>
#include <e32std.h>
#include <lbspositioninfo.h>

#include <epos_suplterminalconstants.h>
#include <epos_suplterminal.h>
#include <epos_suplterminalparams.h>

#include "epos_suplterminalipc.h"
#include "epos_suplterminalerrors.h"
#include "epos_startsuplserver.h"
#include "epos_suplterminalptrholder.h"
#include "epos_suplterminalqop.h"
#include "epos_suplparameters.h"

#include "epos_csuplsettings.h"
#include "epos_csuplsettingparams.h"

const TUint	KFirstReq 			= 0x1000;
const TInt KMaxBufLength        = 255;

//-----------------------------------------------------------------------------
// RSuplTerminalServer
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// RSuplTerminalServer::RSuplTerminalServer()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RSuplTerminalServer::RSuplTerminalServer()
  : RSessionBase(), 
    iReserved(NULL)
   	{
	}

// -----------------------------------------------------------------------------
// RSuplTerminalServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void RSuplTerminalServer::ConstructL()
	{            
	}

// ---------------------------------------------------------
// RSuplTerminalServer::Destruct
//
// Destruction method for RSuplTerminalServer. 
// ---------------------------------------------------------
//
void RSuplTerminalServer::Destruct()
	{
	}

// ---------------------------------------------------------
// RSuplTerminalServer::Connect
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplTerminalServer::Connect()
	{
    __ASSERT_ALWAYS(Handle() == 0, 
    		User::Panic(KSuplClientFault, ESuplServerHandleNotClosed));
    		
	TRAPD(ret, ConstructL());

	if (ret == KErrNone)
		{
		//TInt retry = 200;
		TInt retry = 3;
		for (;;) // FOREVER
			{
			ret = CreateSession(KSuplServerName, Version(), KDefaultMessageSlots);

			if (ret != KErrNotFound && ret != KErrServerTerminated)
				{
				break;
				}

			if (--retry == 0)
				{
				break;
				}

			ret = StartServer();

			if (ret != KErrAlreadyExists && ret != KErrNone)
				{
				break;
				}
			}
		}

	if (ret != KErrNone)
		{
		Destruct();
		}

	return ret;
	}

// ---------------------------------------------------------
// RSuplTerminalServer::Close
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalServer::Close()
	{
    RSessionBase::Close();
	Destruct();
	}

// ---------------------------------------------------------
// RSuplTerminalServer::Version
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TVersion RSuplTerminalServer::Version() const
	{
	return TVersion(
		KSuplMajorVersionNumber,
		KSuplMinorVersionNumber,
		KSuplBuildVersionNumber);
	}
// ---------------------------------------------------------
// RSuplTerminalServer::Connect
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplTerminalServer::Connect(TRequestStatus& aStatus)
	{
    __ASSERT_ALWAYS(Handle() == 0, User::Panic(KSuplClientFault, ESuplServerHandleNotClosed));
	TRAPD(ret, ConstructL());

	if (ret == KErrNone)
		{
		TInt retry = 200;
		for (;;) // FOREVER
			{
			ret = CreateSession(KSuplServerName, Version(), KDefaultMessageSlots, EIpcSession_Unsharable, (TSecurityPolicy*)0, &aStatus);

			if (ret != KErrNotFound && ret != KErrServerTerminated)
				{
				break;
				}

			if (--retry == 0)
				{
				break;
				}

			ret = StartServer();

			if (ret != KErrAlreadyExists && ret != KErrNone)
				{
				break;
				}
			}
		}

	if (ret != KErrNone)
		{
		Destruct();
		}

	return ret;
	}	
//-----------------------------------------------------------------------------
// RSuplTerminalSubSession
//-----------------------------------------------------------------------------

// ---------------------------------------------------------
// RSuplTerminalSubSession::RSuplTerminalSubSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C RSuplTerminalSubSession::RSuplTerminalSubSession()
  : RSubSessionBase(), 
    iPtrHolder(NULL),
    iReserved(NULL),
    isTriggeringUsed(EFalse),
    iNotifyRequestCtr(0)
	{
	}

// ---------------------------------------------------------
// RSuplTerminalSubSession::ConstructL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::ConstructL()
	{
    __ASSERT_ALWAYS(iPtrHolder == NULL, 
    		User::Panic(KSuplClientFault, ESuplServerHandleNotClosed));

	iPtrHolder = CSuplSubSessnPtrHolder::NewL(1, 1);
	}

// ---------------------------------------------------------
// RSuplTerminalSubSession::Destruct
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::Destruct()
	{
    delete iPtrHolder;
	iPtrHolder = NULL;
	}

// ---------------------------------------------------------
// RSuplTerminalSubSession::Open
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplTerminalSubSession::Open(
					RSuplTerminalServer& aSuplServer, 
					TSuplServiceType aSuplService)
	{
	__ASSERT_ALWAYS(aSuplServer.Handle() != 0,
		User::Panic(KSuplClientFault, ESuplServerBadHandle));

	iSuplService = aSuplService;
	
	TInt suplVersion;
	
	switch( aSuplService )
		{
		case ESUPL_1_0:
			{
			suplVersion = 1;
			break;					
			}
		case ESUPL_2_0:
			{
			suplVersion = 2;
			break;					
			}
		default:
			{
			suplVersion = 1;
			break;					
			}
		}
	
	TIpcArgs args;
    args.Set(0, suplVersion);		
	
	TRAPD(ret, ConstructL());
	if (ret == KErrNone)
		{
		ret = CreateSubSession(aSuplServer, ESuplTerminalSubssnOpen, args);
		}
	if (ret != KErrNone)
		{
		Destruct();
		}
	
	return ret;            
	}

// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::RunSession(
					TRequestStatus& aStatus, 
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
    {
	__ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KSuplClientFault, ESuplServerBadHandle));

	if(aRequestId < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}
		
	if(aSETCaps < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}

    TInt setCaps;
    
    if (aFirstReq)
        setCaps = aSETCaps | KFirstReq; 
    else
        setCaps = aSETCaps;

	TIpcArgs args;
    args.Set(0, setCaps);		
    args.Set(1, aRequestId);		
	
	SendReceive(ESuplTerminalSubssnAsyncRunsession, args, aStatus);	
    }
    
//---------------TDESC METHODS START HERE---------------------------------------
// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::RunSession(
					TRequestStatus& aStatus, 
					TSuplTerminalQop& aQop, 
					const TDesC& aHslpAddress, 
					TBool aFallBack, 
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), 
    			User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
    if(aRequestId < 0 )     
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrArgument); 
        return;
        }
        
    if(aSETCaps < 0 )   
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrArgument); 
        return;
        }
    
    if(aHslpAddress.Length() < 0 || aHslpAddress.Length() > KMaxHslpAddressLength )
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrArgument);
        return;
        }
        
    //Check if the passed HSLPAddress exists or not!
    TInt serverExists = CheckIfSlpExists(aHslpAddress);
    if(serverExists != KErrNone)
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,serverExists);
        return;
        }
    
    TReal32 aHorAcc,aVerAcc;
    TInt aDelay,aMaxAge;    
    if((aQop.GetHorizontalAccuracy(aHorAcc)==KErrNotFound)&&
    			(aQop.GetVerticalAccuracy(aVerAcc)==KErrNotFound)&&
    			(aQop.GetDelay(aDelay)==KErrNotFound)&&
    			(aQop.GetMaxLocationAge(aMaxAge)==KErrNotFound))
    	{
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrArgument); 
        return;
    	}      
    
    iSuplParameters.qop = aQop;
    iSuplParameters.HslpAddress = aHslpAddress;
    iSuplParameters.FallBack = aFallBack;
    
    iPtrHolder->Ptr(0).Set(
        reinterpret_cast<TUint8*>(&iSuplParameters),
        sizeof(iSuplParameters),sizeof(iSuplParameters));
    
    TInt setCaps;
    
    if (aFirstReq)
        setCaps = aSETCaps | KFirstReq; 
    else
        setCaps = aSETCaps;

    TIpcArgs args;
    
    args.Set(0, setCaps);       
    args.Set(1,aRequestId);
    args.Set(2,&iPtrHolder->Ptr(0));

    SendReceive(ESuplTerminalSubssnASyncRunsessionWithServernameQop, args, aStatus);  
    }
    
//-------------------2ND METHOD(SYNCHRONOUS)--------------------------
// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplTerminalSubSession::RunSession(
					TSuplTerminalQop& aQop, 
					const TDesC& aHslpAddress, 
					TBool aFallBack, 
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), 
    		User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
    if(aRequestId < 0 )     
        {
        return KErrArgument;
        }
        
    if(aSETCaps < 0 )   
        {
        return KErrArgument;
        }
        
    if(aHslpAddress.Length() < 0 || aHslpAddress.Length() > KMaxHslpAddressLength )
        {
        return KErrArgument;
        }
    
    //Check if the passed HSLPAddress exists or not!
    TInt serverExists = CheckIfSlpExists(aHslpAddress);
    if(serverExists != KErrNone)
        {
        return serverExists;
        }
                
    TReal32 aHorAcc,aVerAcc;
    TInt aDelay,aMaxAge;    
    if((aQop.GetHorizontalAccuracy(aHorAcc)==KErrNotFound)
    			&&(aQop.GetVerticalAccuracy(aVerAcc)==KErrNotFound)
    			&&(aQop.GetDelay(aDelay)==KErrNotFound)
    			&&(aQop.GetMaxLocationAge(aMaxAge)==KErrNotFound))
	    {
	    return KErrArgument;
	    }

    iSuplParameters.qop = aQop;
    iSuplParameters.HslpAddress = aHslpAddress;
    iSuplParameters.FallBack = aFallBack;
    
    iPtrHolder->Ptr(0).Set(
        reinterpret_cast<TUint8*>(&iSuplParameters),
        sizeof(iSuplParameters),sizeof(iSuplParameters));
    
    TInt setCaps;
    
    if (aFirstReq)
        setCaps = aSETCaps | KFirstReq; 
    else
        setCaps = aSETCaps;

    TIpcArgs args;
    
    args.Set(0, setCaps);       
    args.Set(1,aRequestId);
    args.Set(2,&iPtrHolder->Ptr(0));
    return SendReceive(ESuplTerminalSubssnSyncRunsessionWithServernameQop, args);  
    }
    
//------------------------------ASYNC WITHOUT QOP--------------------------------------------------------------
// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::RunSession(
					TRequestStatus& aStatus, 
					const TDesC& aHslpAddress, 
					TBool aFallBack, 
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
    if(aRequestId < 0 )     
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrArgument); 
        return;
        }
        
    if(aSETCaps < 0 )   
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrArgument); 
        return;
        }
        
    if(aHslpAddress.Length() < 0 || aHslpAddress.Length() > KMaxHslpAddressLength )
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrArgument);
        return;
        }
    
    //Check if the passed HSLPAddress exists or not!
    TInt serverExists = CheckIfSlpExists(aHslpAddress);
    if(serverExists != KErrNone)
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,serverExists);
        return;
        }
    
    iSuplParameters.HslpAddress = aHslpAddress;
    iSuplParameters.FallBack = aFallBack;
    
    iPtrHolder->Ptr(0).Set(
        reinterpret_cast<TUint8*>(&iSuplParameters),
        sizeof(iSuplParameters),sizeof(iSuplParameters));
    
    TInt setCaps;
    
    if (aFirstReq)
        setCaps = aSETCaps | KFirstReq; 
    else
        setCaps = aSETCaps;

    TIpcArgs args;
    
    args.Set(0, setCaps);       
    args.Set(1,aRequestId);
    args.Set(2,&iPtrHolder->Ptr(0));
    SendReceive(ESuplTerminalSubssnASyncRunsessionServernameWithoutQop, args, aStatus); 
    }
    
//--------------------------------SYNC WITHOUT QOP--------------------------------------------------------
// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplTerminalSubSession::RunSession(
					const TDesC& aHslpAddress, 
					TBool aFallBack, 
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), 
    			User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
    if(aRequestId < 0 )     
        {
        return KErrArgument;
        }
        
    if(aSETCaps < 0 )   
        {
        return KErrArgument;
        }
    
    if(aHslpAddress.Length() < 0 || aHslpAddress.Length() > KMaxHslpAddressLength )
        {
        return KErrArgument;
        }
    
    //Check if the passed HSLPAddress exists or not!
    TInt serverExists = CheckIfSlpExists(aHslpAddress);
    if(serverExists != KErrNone)
        {
        return serverExists;
        }
    
    iSuplParameters.HslpAddress = aHslpAddress;
    iSuplParameters.FallBack = aFallBack;
    
    iPtrHolder->Ptr(0).Set(
        reinterpret_cast<TUint8*>(&iSuplParameters),
        sizeof(iSuplParameters),sizeof(iSuplParameters));
    
    TInt setCaps;
    
    if (aFirstReq)
        setCaps = aSETCaps | KFirstReq; 
    else
        setCaps = aSETCaps;

    TIpcArgs args;
    
    args.Set(0, setCaps);       
    args.Set(1,aRequestId);
    args.Set(2,&iPtrHolder->Ptr(0));
    return SendReceive(ESuplTerminalSubssnSyncRunsessionServernameWithoutQop, args);  
    }

// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
EXPORT_C void RSuplTerminalSubSession::RunSession(
					TRequestStatus& aStatus,
					TSuplTerminalQop& aQop, 
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
	{
	__ASSERT_ALWAYS(SubSessionHandle(), 
				User::Panic(KSuplClientFault, ESuplServerBadHandle));

	if(aRequestId < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}

	if(aSETCaps < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}

	TReal32 aHorAcc,aVerAcc;
	TInt aDelay,aMaxAge;	
	if((aQop.GetHorizontalAccuracy(aHorAcc)==KErrNotFound)
					&&(aQop.GetVerticalAccuracy(aVerAcc)==KErrNotFound)
					&&(aQop.GetDelay(aDelay)==KErrNotFound)
					&&(aQop.GetMaxLocationAge(aMaxAge)==KErrNotFound))
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}

	iQoP = aQop;	

	iPtrHolder->Ptr(0).Set(
	reinterpret_cast<TUint8*>(&iQoP),
	sizeof(iQoP),sizeof(iQoP));

	TInt setCaps;

	if (aFirstReq)
		setCaps = aSETCaps | KFirstReq; 
	else
		setCaps = aSETCaps;

	TIpcArgs args;

	args.Set(0, setCaps);		
	args.Set(1,aRequestId);
	args.Set(2,&iPtrHolder->Ptr(0));

	SendReceive(ESuplTerminalSubssnAsyncRunsessionWithQop, args, aStatus);	
	}
	
// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplTerminalSubSession::RunSession(
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
    {
	__ASSERT_ALWAYS(SubSessionHandle(), 
				User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
	if(aRequestId < 0 )  	
		{
		return KErrArgument;
		}
		
	if(aSETCaps < 0 )  	
		{
		return KErrArgument;
		}
    TInt setCaps;
    
    if (aFirstReq)
        setCaps = aSETCaps | KFirstReq; 
    else
        setCaps = aSETCaps;
		
	TIpcArgs args;
    args.Set(0, setCaps);		
    args.Set(1, aRequestId);		
	return SendReceive(ESuplTerminalSubssnSyncRunsession, args);
    }
    
// ---------------------------------------------------------
// RSuplTerminalSubSession::RunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
EXPORT_C TInt RSuplTerminalSubSession::RunSession(
					TSuplTerminalQop& aQop, 
					TInt aSETCaps, 
					TInt aRequestId, 
					TBool aFirstReq)
	{
	__ASSERT_ALWAYS(SubSessionHandle(), 
				User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
	if(aRequestId < 0 )  	
		{
		return KErrArgument;
		}
		
	if(aSETCaps < 0 )  	
		{
		return KErrArgument;
		}
		
	TReal32 aHorAcc,aVerAcc;
	TInt aDelay,aMaxAge;	
	if((aQop.GetHorizontalAccuracy(aHorAcc)==KErrNotFound)
					&&(aQop.GetVerticalAccuracy(aVerAcc)==KErrNotFound)
					&&(aQop.GetDelay(aDelay)==KErrNotFound)
					&&(aQop.GetMaxLocationAge(aMaxAge)==KErrNotFound))
		{
		return KErrArgument;
		}		
		
	iQoP = aQop;		

    TInt setCaps;
    
    if (aFirstReq)
        setCaps = aSETCaps | KFirstReq; 
    else
        setCaps = aSETCaps;

	TIpcArgs args;
	TPtrC8 ptr(
		reinterpret_cast<const TUint8*>(&iQoP),
		sizeof(TSuplTerminalQop));

    args.Set(0, setCaps);		
    args.Set(1, aRequestId);		
	args.Set(2,&ptr);
	
	return SendReceive(ESuplTerminalSubssnSyncRunsessionWithQop, args);
	}
	
// ---------------------------------------------------------
// RSuplTerminalSubSession::GetPosition
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt RSuplTerminalSubSession::GetPosition(TPositionInfoBase& aSuplPosInfo)
    {
	__ASSERT_ALWAYS(SubSessionHandle(), 
				User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
	if( isTriggeringUsed )
		{
		return KErrNotSupported;		
		}
		
    iPtrHolder->Ptr(0).Set(
		reinterpret_cast<TUint8*>(&aSuplPosInfo),
		aSuplPosInfo.PositionClassSize(),
		aSuplPosInfo.PositionClassSize());

	return SendReceive(ESuplTerminalSubssnGetPosition, TIpcArgs(&iPtrHolder->Ptr(0)));
	}	

// ---------------------------------------------------------
// RSuplTerminalSubSession::GetServerAddress
//
// (other items were commented in a header).
// ---------------------------------------------------------

EXPORT_C TInt RSuplTerminalSubSession::GetServerAddress(TDes& aHslpAddress)
    {
    __ASSERT_ALWAYS(SubSessionHandle(), 
    			User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
    iPtrHolder->Ptr(0).Set(
		reinterpret_cast<TUint8*>(&aHslpAddress),
		aHslpAddress.MaxSize(),
		aHslpAddress.MaxSize()
		 );
    
    return SendReceive(ESuplTerminalSubssnGetServerAddress, 
    			TIpcArgs(&iPtrHolder->Ptr(0)));
    }

//------------------------------------------------------------------------------------

// ---------------------------------------------------------
// RSuplTerminalSubSession::GetSlpList
//
// (other items were commented in a header).
// ---------------------------------------------------------
EXPORT_C TInt RSuplTerminalSubSession::GetSlpList(
				RPointerArray<CSuplTerminalParams> &aParamValues)
	{
	__ASSERT_ALWAYS(SubSessionHandle(), 
			User::Panic(KSuplClientFault, ESuplServerBadHandle));

	RPointerArray<CServerParams> parValues;

	TInt64 slpId;
	HBufC* serverAddress = HBufC::New(KMaxBufLength);
	HBufC* iapName = HBufC::New(KMaxBufLength);
	TBool serverEnabled;
	TBool simChangeRemove;
	TBool usageInHomeNw;
	TBool editable;

	if (!serverAddress || !iapName)
		{
		delete serverAddress;
		delete iapName;
		return KErrNoMemory;
		}
	delete iSuplStorageSettings;
	iSuplStorageSettings = NULL;

	// create local object iSuplStorageSettings
	TRAPD(err,iSuplStorageSettings = CSuplSettings::NewL());
	if(err!=KErrNone)
		{
		delete serverAddress;
		delete iapName;
		parValues.Close();
		parValues.ResetAndDestroy();
		return err;
		}

	parValues.ResetAndDestroy();
    //coverity[deref_ptr_in_call]
	TInt error = iSuplStorageSettings->GetAllSlp(parValues); //handle return value...
	if (error!=KErrNone)
		{
		delete serverAddress;
		delete iapName;
		parValues.Close();
		parValues.ResetAndDestroy();
		return error;
		}
	TInt count = parValues.Count();
	for(TInt i = 0; i < count; i++ )
		{
		CSuplTerminalParams *params = NULL;
		TRAPD(err,params = CSuplTerminalParams::NewL());
		if(err!=KErrNone)
			{
			delete serverAddress;
			delete iapName;
			parValues.Close();
			parValues.ResetAndDestroy();
			return err;
			}
		parValues[i]->Get(
							slpId,
							serverAddress->Des(),
							iapName->Des(),
							serverEnabled,
							simChangeRemove,
							usageInHomeNw,
							editable);
							
		params->Set(	
						*serverAddress,
						*iapName,
						serverEnabled,
						simChangeRemove,
						usageInHomeNw,
						editable,
						slpId);
						
		aParamValues.Append(params);
		}
	delete serverAddress;
	delete iapName;	
    aParamValues.Close();
    //coverity[check_after_deref]
	if (iSuplStorageSettings)
		{
		delete iSuplStorageSettings;
		iSuplStorageSettings = NULL;		
		}

	return KErrNone;  
	}
    
//------------------------------------------------------------------------------------


// ---------------------------------------------------------
// RSuplTerminalSubSession::CheckIfSlpExists
//
// (other items were commented in a header).
// ---------------------------------------------------------

TInt RSuplTerminalSubSession::CheckIfSlpExists(const TDesC& aHslpAddress)
    {
    if(iSuplStorageSettings)
        {
        delete iSuplStorageSettings;
        iSuplStorageSettings = NULL;
        }
    
    //Check if the passed HSLPAddress exists or not!
    TRAPD(err1,iSuplStorageSettings = CSuplSettings::NewL());
    if(err1!=KErrNone)
        {
        return err1;
        }
    CServerParams *paramValues = NULL;
    TRAPD(err,paramValues = CServerParams::NewL());
    if(err!=KErrNone)
        {
        delete iSuplStorageSettings;
        iSuplStorageSettings = NULL;
        return err;
        }
    TInt retValue = iSuplStorageSettings->GetSlpInfoAddress(aHslpAddress,paramValues);
    
    if(retValue == KErrNotFound)
        {
        delete iSuplStorageSettings;
        iSuplStorageSettings = NULL;
        delete paramValues;
        paramValues = NULL;
        return KErrNotFound;
        }
    delete paramValues;
    paramValues = NULL;
  
    delete iSuplStorageSettings;
    iSuplStorageSettings = NULL;
    
    return KErrNone;
    }

// ---------------------------------------------------------
// RSuplTerminalSubSession::CancelRunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::CancelRunSession()
	{
	__ASSERT_ALWAYS(SubSessionHandle(), 
				User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
	if( !isTriggeringUsed )
		{
		SendReceive(ESuplTerminalSubssnCancelRunsession);	
		}    
	}
	    
// ---------------------------------------------------------
// RSuplTerminalSubSession::Close
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::Close()
	{
	CloseSubSession(ESuplTerminalSubssnClose);
	//Destruct();            	
	}

// ---------------------------------------------------------
// This method is used to allow polymorphic extensions to the API without breaking
// BC. See documentation for explanation.
// 
// @publishedAll
// @param aFunctionNumber contains the Id of the function to be invoked.
// @param aPtr1 a pointer to any data
// @param aPtr2 a pointer to any data.
// ---------------------------------------------------------
//
EXPORT_C TAny* RSuplTerminalSubSession::ExtendedInterface(TInt /* aFunctionNumber */,
                                              TAny* /* aPtr1 */,
                                              TAny* /* aPtr2 */)
	{
	return NULL; 
	}
	
// ---------------------------------------------------------
// RSuplTerminalSubSession::StartSuplTriggerSession
// without supl server address
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::StartSuplTriggerSession(
            TRequestStatus& aStatus,                
			TSuplTerminalPeriodicTrigger& 	aPeriodicTrigger,
			TInt 				aSETCaps,
			TInt 				aRequestId
		)
	{
	__ASSERT_ALWAYS(SubSessionHandle(), 
				User::Panic(KSuplClientFault, ESuplServerBadHandle));
   
   
  if(!CheckSuplTriggerServiceStatus()) //To check that triggering service is allowed by user...
  {
  	TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotSupported);	
		return;
  }
  
	if( iSuplService != ESUPL_2_0 )
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotSupported);	
		return;
		}	

	if(aRequestId < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}
		
	if(aSETCaps < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}

	TUint noOfFixes, interval, startTime;		
	if( aPeriodicTrigger.Get(noOfFixes,interval,startTime) != KErrNone ) 
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}
		
    iPeriodicTrigger = aPeriodicTrigger;
        
    iPtrHolder->Ptr(0).Set(
        reinterpret_cast<TUint8*>(&iPeriodicTrigger),
        sizeof(iPeriodicTrigger),sizeof(iPeriodicTrigger));            
    
	TIpcArgs args;

    args.Set(0, aSETCaps);		
    args.Set(1, aRequestId);		
	args.Set(2, &iPtrHolder->Ptr(0));

	isTriggeringUsed = ETrue;
	
	// resetting the counter	
	iNotifyRequestCtr = 0;
	
	return SendReceive(
		ESuplTerminalSubssnStartPeriodicTrigger, args, aStatus);			
	}

// ---------------------------------------------------------
// RSuplTerminalSubSession::StartSuplTriggerSession
// with supl server address
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::StartSuplTriggerSession(
            TRequestStatus& aStatus,                
			TSuplTerminalPeriodicTrigger& 	aPeriodicTrigger,
			const TDesC& 		aHslpAddress, 
			TBool 				aFallBack, 
			TInt 				aSETCaps,
			TInt 				aRequestId
		)
	{
	__ASSERT_ALWAYS(SubSessionHandle(), 
			User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
  if(!CheckSuplTriggerServiceStatus()) //To check that triggering service is allowed by user...
		{
			TRequestStatus *status = &aStatus; 
			User::RequestComplete(status,KErrNotSupported);	
			return;
		} 

	if( iSuplService != ESUPL_2_0 )
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotSupported);	
		return;
		}	

	if(aRequestId < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}
		
	if(aSETCaps < 0 )  	
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}

	TUint noOfFixes, interval, startTime;		
	if( aPeriodicTrigger.Get(noOfFixes,interval,startTime) != KErrNone ) 
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
		}
		
    if(aHslpAddress.Length() < 0 || aHslpAddress.Length() > KMaxHslpAddressLength )
        {
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrArgument);	
		return;
        }
        
    //Check if the passed HSLPAddress exists or not!
    TInt serverExists = CheckIfSlpExists(aHslpAddress);
    if(serverExists != KErrNone)
        {
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,serverExists);	
		return;
        }
    
    iSuplParameters.PeriodicTrigger 	= aPeriodicTrigger;
    iSuplParameters.HslpAddress 		= aHslpAddress;
    iSuplParameters.FallBack 			= aFallBack;

    iPeriodicTrigger = aPeriodicTrigger;

    iPtrHolder->Ptr(0).Set(
        reinterpret_cast<TUint8*>(&iSuplParameters),
        sizeof(iSuplParameters),sizeof(iSuplParameters));            
    
	TIpcArgs args;

    args.Set(0, aSETCaps);		
    args.Set(1, aRequestId);		
	args.Set(2, &iPtrHolder->Ptr(0));
    
   	isTriggeringUsed = ETrue;	
	// resetting the counter	
   	iNotifyRequestCtr = 0;
   		
	return SendReceive(
		ESuplTerminalSubssnStartPeriodicTriggerWithServer, args, aStatus);			
	}
	
// ---------------------------------------------------------
// RSuplTerminalSubSession::StopSuplTriggerring
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::StopSuplTriggerSession()
	{    
	__ASSERT_ALWAYS(SubSessionHandle(), 
		User::Panic(KSuplClientFault, ESuplServerBadHandle));	
	if( isTriggeringUsed )
		{
	    SendReceive(ESuplTerminalSubssnStopTriggerSession);	
	    isTriggeringUsed = EFalse;		
		}
	}

// ---------------------------------------------------------
// RSuplTerminalSubSession::NotifyTriggerFired
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalSubSession::NotifyTriggerFired( 
        	TRequestStatus& aStatus,  
        	TSuplTriggerFireInfo& aFireInfo)
	{	
	__ASSERT_ALWAYS(SubSessionHandle(), 
			User::Panic(KSuplClientFault, ESuplServerBadHandle));

		if(!CheckSuplTriggerServiceStatus()) //To check that triggering service is allowed by user...
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotSupported);	
		return;
		}

	if( iSuplService != ESUPL_2_0 )
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotSupported);	
		return;
		}	

	if ( !isTriggeringUsed )
		{
		TRequestStatus *status = &aStatus; 
		User::RequestComplete(status,KErrNotFound);	
		return;
		}
	
	TUint noOfFixes,interval,startTime;
	if( iPeriodicTrigger.Get(noOfFixes,interval,startTime) != KErrNone )
		{
		if( iNotifyRequestCtr >= noOfFixes )
			{
			TRequestStatus *status = &aStatus; 
			User::RequestComplete(status,KErrNotFound);	
			return;							
			}
		}
		
	aStatus=KRequestPending;

	TIpcArgs args;
	
	TPckg<TSuplTriggerFireInfo> fireInfo(aFireInfo);
       
    iPtrHolder->Ptr(0).Set(fireInfo);
    args.Set(2, &iPtrHolder->Ptr(0));
	
	iNotifyRequestCtr++;
	
	SendReceive(ESuplTerminalSubssnNotifyTriggerFired, args, aStatus);
	}

// ---------------------------------------------------------
// RSuplTerminalSubSession::CheckSuplTriggerServiceStatus
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool RSuplTerminalSubSession::CheckSuplTriggerServiceStatus()
{
	
	delete iSuplStorageSettings;
	iSuplStorageSettings = NULL;
	
	CSuplSettings::TSuplTriggerStatus suplTriggerStatus;

	// create local object iSuplStorageSettings
	TRAPD(err,iSuplStorageSettings = CSuplSettings::NewL());
	if(err == KErrNone)
	{
			iSuplStorageSettings->GetSuplTriggeredServiceStatus(suplTriggerStatus);
			delete iSuplStorageSettings;
			iSuplStorageSettings = NULL;

			if(suplTriggerStatus == CSuplSettings::ESuplTriggerOn)
			{
					return ETrue;						
			}
			else
			{
					return EFalse;	
			}
	}		
	else
	{
			return ETrue;
	}
}

// end of file
