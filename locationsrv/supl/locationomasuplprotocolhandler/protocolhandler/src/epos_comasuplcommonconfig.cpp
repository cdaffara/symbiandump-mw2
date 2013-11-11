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
* Description:   class for holding common config
*
*/



#include <centralrepository.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <ecom/ecom.h> 

#include "epos_comasupltrace.h"
#include "epos_comasuplcommonconfig.h"
#include "epos_omasuplconfigurationkeys.h"
#include "epos_omasuplconstants.h"
#include "epos_comasuplsettings.h"
#include "lbs/epos_comasuplposhandlerbase.h"

_LIT(KTraceFileName,"SUPL_OMA_PH::EPos_COMASUPLCommonConfig.cpp");

// -----------------------------------------------------------------------------
// ResetAndDestroy()
// Resets & destroys array if leave occurs
// -----------------------------------------------------------------------------
void ResetArray(TAny* aArray)
	{
	((RImplInfoPtrArray*)aArray)->ResetAndDestroy();
	}

//Constructor
COMASuplCommonConfig::COMASuplCommonConfig()
	{
	}

//Destructor
COMASuplCommonConfig::~COMASuplCommonConfig()
	{
	delete iSuplSettings;
	iSuplSettings=NULL;
	
	delete iPosHandler;
	iPosHandler=NULL;
	
	iIMSI.Close();
    delete iTrace;
    iTrace = NULL;

	}
//
// -----------------------------------------------------------------------------
// CSuplSession::NewL
// Creates new instance of COMASuplSession 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplCommonConfig* COMASuplCommonConfig::NewL()
	{ 	
		COMASuplCommonConfig* self = new( ELeave ) COMASuplCommonConfig();
		CleanupStack::PushL( self );
		self->ConstructL();
		CleanupStack::Pop(self);
		return self;
	}

// -----------------------------------------------------------------------------
// COMASuplCommonConfig::ConstuctL()
// 2nd phase construction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

void COMASuplCommonConfig::ConstructL()
	{
	
    iTrace = COMASuplTrace::NewL();	
	ReadOMASuplSettingsL();
	LoadPosHandlerL();
	ReadOMASuplConfigurationL();
		
	}
	

void COMASuplCommonConfig::ReadOMASuplConfigurationL()
	{
	CRepository* ConfigurationCenRepSession = CRepository::NewL(KCRUidOMASuplConfiguration);

	CleanupStack::PushL(ConfigurationCenRepSession);
	
	User::LeaveIfError(ConfigurationCenRepSession->Get(KOMASuplConfigurationUT1, iUT1_StartTimer));
	User::LeaveIfError(ConfigurationCenRepSession->Get(KOMASuplConfigurationUT2, iUT2_PosInitTimer));
	User::LeaveIfError(ConfigurationCenRepSession->Get(KOMASuplConfigurationUT3, iUT3_PosTimer));
	User::LeaveIfError(ConfigurationCenRepSession->Get(KOMASuplConfigurationPrivacyTimer, iPrivacyTimer));
	User::LeaveIfError(ConfigurationCenRepSession->Get(KOMASuplConfigurationSuplMode, iSETMode));
	User::LeaveIfError(ConfigurationCenRepSession->Get(KOMASuplConfigurationPersistentFailureTimer, iPersistFailTimer));

	TInt ret = ConfigurationCenRepSession->Get(KOMASuplConfigurationSuplPermissionQueryUsage, iSuplUsage);

    if (ret == KErrNone)
        {
        if (iSuplUsage <= 0)
            iSuplUsage = 0;
        else if (iSuplUsage >=2)
            iSuplUsage = 2;            
        }
    else
        {
        iSuplUsage = 0;
        }                
    	User::LeaveIfError(ConfigurationCenRepSession->Get(KOMASuplNiDialogTimeOutValue, iSuplInitTimeOut));                
	
	CleanupStack::PopAndDestroy(ConfigurationCenRepSession);
	}
	
EXPORT_C void COMASuplCommonConfig::ReadOMASuplSettingsL()
	{
	if( iSuplSettings )
		{
		delete iSuplSettings;
		iSuplSettings=NULL;			
		}	
	iSuplSettings = COMASuplSettings::NewL();
	}
	
void COMASuplCommonConfig::LoadPosHandlerL()
	{
	RImplInfoPtrArray pluginArray;
	
	REComSession& EComServerSession = REComSession::OpenL();
	
	CleanupStack::PushL(TCleanupItem(*ResetArray, &pluginArray));
	REComSession::ListImplementationsL(KOMAPOSHandlerInterfaceUid, pluginArray);
	
	TBuf<32> buf;
	if(pluginArray.Count() > 0)
		{
		iTrace->Trace(_L("Plugins Found for POS..."), KTraceFileName, __LINE__); 
		const CImplementationInformation* pluginInformation = pluginArray[0];
		TUid  pluginUid = pluginInformation->ImplementationUid();
		TRAPD(err,iPosHandler = COMASuplPosHandlerBase::NewL(pluginUid)); 
		
		buf.Copy(_L("Plugins loaded with error ..."));				
		buf.AppendNum(err);
		iTrace->Trace(buf,KTraceFileName, __LINE__); 		
					
		}
	else
		{
        iPosHandler = NULL;                
		iTrace->Trace(_L("No Plugins Found for POS..."), KTraceFileName, __LINE__); 
		}	
		
	CleanupStack::PopAndDestroy(1);
	EComServerSession.Close();
	}
  
// end of file
