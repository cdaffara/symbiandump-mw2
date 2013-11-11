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
* Description:   class for holding SUPL settings
*
*/



#include <commdb.h>

#include "epos_comasuplsettings.h"
#include "epos_omasuplconstants.h"

//Constructor
COMASuplSettings::COMASuplSettings():iSettingsUIActive(EFalse)
	{}

//Destructor
COMASuplSettings::~COMASuplSettings()
	{
	iHSLPIPAddress.Close();
	iHSLPfromImsi.Close();
	}
	
//
// -----------------------------------------------------------------------------
// CSuplSession::NewL
// Creates new instance of COMASuplSession 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSettings* COMASuplSettings::NewL()
	{ 	
	COMASuplSettings* self = new( ELeave ) COMASuplSettings();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// COMASuplSettings::ConstuctL()
// 2nd phase construction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void COMASuplSettings::ConstructL()
	{
	iHSLPIPAddress.CreateL(KMaxIPAddressLength);
	iHSLPfromImsi.CreateL(KMaxIPAddressLength);
	}

// -----------------------------------------------------------------------------
// COMASuplSettings::SetHSLPAddress
// Sets HSPLAddress 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSettings::SetIAPId(TInt aIAPId)
	{
	iIAPId = aIAPId;
	}

// -----------------------------------------------------------------------------
// COMASuplSettings::SetHSLPAddress
// Sets HSPLAddress 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSettings::SetHSLPAddress(const TDesC& aHSLPIPAddress)
	{
	iHSLPIPAddress.Copy(aHSLPIPAddress);
	}
	
// -----------------------------------------------------------------------------
// COMASuplSettings::SetHSLPAddressfromIMSI
// Sets HSPLAddress 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSettings::SetHSLPAddressFromIMSI(const TDesC& aHSLPfromImsi)
	{
	iHSLPfromImsi.Copy(aHSLPfromImsi);
	}	

// -----------------------------------------------------------------------------
// COMASuplSettings::SLPAddress
// This method returns SLPAddress
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& COMASuplSettings::SLPAddress() const
	{
	return iHSLPIPAddress;
	}
	
// -----------------------------------------------------------------------------
// COMASuplSettings::SLPAddressfromIMSI
// This method returns SLPAddress generated from IMSI
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& COMASuplSettings::SLPAddressfromImsi() const
	{
	return iHSLPfromImsi;
	}
// -----------------------------------------------------------------------------
// COMASuplSettings::IAPId
// This method returns IAPId.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplSettings::IAPId() const
	{
	return iIAPId;
	}

// -----------------------------------------------------------------------------
// COMASuplSettings::SetSUPLUsage
// Sets the SUPL Usage
// 
// -----------------------------------------------------------------------------
//
/*EXPORT_C void COMASuplSettings::SetSUPLUsage(CSuplSettings::TSuplSettingsUsage& aSUPLUsage ) 
	{
    iSUPLUsage = aSUPLUsage;
	}*/

// -----------------------------------------------------------------------------
// COMASuplSettings::SUPLUsage() 
// Gets the SUPL Usage
// 
// -----------------------------------------------------------------------------
//
/*EXPORT_C CSuplSettings::TSuplSettingsUsage COMASuplSettings::SUPLUsage() const
	{
		return iSUPLUsage;
	}
*/

// -----------------------------------------------------------------------------
// COMASuplSettings::SetUIActive
// Sets the flag indicating the activation of Settings UI
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSettings::SetUIActive(TBool aIsActive)
	{
    iSettingsUIActive = aIsActive;
	}
	
// -----------------------------------------------------------------------------
// COMASuplSettings::SetUIActive
// Retrieves the flag indicating the activation of Settings UI
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TBool COMASuplSettings::IsUIActive() const
	{
    return iSettingsUIActive;
	}

// -----------------------------------------------------------------------------
// COMASuplSettings::ConvertIAPNameToIdL
// 
// -----------------------------------------------------------------------------
EXPORT_C TBool COMASuplSettings::ConvertIAPNameToIdL(const TDesC& aIAPName, TUint32& aIAPId)
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
	
EXPORT_C TBool COMASuplSettings::IsHslpNull() 
	{
    if(iHSLPIPAddress.Length()==0)
    	{
    		return ETrue;
    	}
    else
    	{
    		return EFalse;
    	}	
	}

// end of file
