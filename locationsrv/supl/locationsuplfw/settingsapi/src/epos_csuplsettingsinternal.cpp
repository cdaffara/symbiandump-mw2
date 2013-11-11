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
* Description:  Implementation of CSuplSettingsInternal class..Not exposed
*
*/



// INCLUDE FILES

#include "epos_csuplsettingsengine.h"
#include "epos_csuplsettings.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_csuplsettingparams.h"
#include "epos_csuplsettingextensionparams.h"
#include "epos_msuplsettingsobserver.h"
#include "epos_csuplsettingsnotifier.h"
#include "epos_csuplsettingsconstants.h"

// CONSTANTS

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSettingsInternal::CSuplSettingsInternal()
   
    {
    
    }

EXPORT_C CSuplSettingsInternal* CSuplSettingsInternal::NewL()
    {
    CSuplSettingsInternal* self = new (ELeave) CSuplSettingsInternal();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// EPOC default constructor can leave.
void CSuplSettingsInternal::ConstructL()
    {
    CSuplSettings::ConstructL();    
    }

// Destructor
EXPORT_C CSuplSettingsInternal::~CSuplSettingsInternal()
    {
    
    }


EXPORT_C CSuplSettingsInternal* CSuplSettingsInternal::NewLC()
    {
    CSuplSettingsInternal* self = new (ELeave) CSuplSettingsInternal();

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

            
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetVersion()
//
//This method is used to change protocol version supported by SUPL server.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetVersion( const TInt64 aSlpId, const TInt aMajor, const TInt aMinor )
    {
    return iSettingsEngine->SetVersion(aSlpId,aMajor,aMinor );            
    }            
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetVersion()
//
//This method is used to retrieve protocol version supported by SUPL server. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetVersion( const TInt64 aSlpId,TInt& aMajor,TInt& aMinor )const
    {
    return iSettingsEngine->GetVersion(aSlpId,aMajor,aMinor );            
    }         
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetManufacturerName()
//
//This method is used to change manufacturer in server parameters. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetManufacturerName( const TInt64 aSlpId, const TDesC& aManufacturer)
    {
    return iSettingsEngine->SetManufacturerName(aSlpId,aManufacturer);            
    }   
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetManufacturerName()
//
//This method is used to retrieve manufacturer in server parameters. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetManufacturerName( const TInt64 aSlpId,TDes& aManufacturerName)const
    {
    return iSettingsEngine->GetManufacturerName(aSlpId,aManufacturerName);            
    }
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetLastUseTime()
//
//This method is used to change timestamp when the HSLP was used last time. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetLastUseTime( const TInt64 aSlpId, const TTime aTime )
    {
    return iSettingsEngine->SetLastUseTime(aSlpId,aTime );             
    }            

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetLastTriedTime()
//
//This method is used to change timestamp when the HSLP was tried last time. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetLastTriedTime( const TInt64 aSlpId, const TTime aTime )
    {
    return iSettingsEngine->SetLastTriedTime(aSlpId,aTime);            
    }      

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetLastUseTime()
//
//This method is used to get the timestamp when the HSLP was used last time. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetLastUseTime(const TInt64 aSlpId,TTime& aTime) const
    {
    return iSettingsEngine->GetLastUseTime(aSlpId,aTime );             
    }            

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetLastTriedTime()
//
//This method is used to get timestamp when the HSLP was used last tried. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetLastTriedTime(const TInt64 aSlpId,TTime& aTime) const
    {
    return iSettingsEngine->GetLastTriedTime(aSlpId,aTime);            
    } 
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetNetInfoLastUse()
//
//his method is used to change network information when the SUPL session was last done. 
//-------------------------------------------------------------------------------------- 
EXPORT_C TInt CSuplSettingsInternal::SetNetInfoLastUse( const TInt64 aSlpId, 
                                            const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType, 
                                            const TInt aMcc, const TInt aMnc, const TInt aCid, const TInt aLac)
    {
    return iSettingsEngine->SetNetInfoLastUse(aSlpId,aNetType,aMcc,aMnc,aCid,aLac);             
    }            

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetNetInfoLastSucess()
//
//This method is used to change network information when the SUPL session was last done sucessfully. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetNetInfoLastSucess( const TInt64 aSlpId, 
                                            const CSuplSettingsInternal::TSuplSettingsNetworkType aNetType,
                                            const TInt aMcc, const TInt aMnc, const TInt aCid, const TInt aLac)
    {
    return iSettingsEngine->SetNetInfoLastSucess(aSlpId,aNetType,aMcc,aMnc,aCid,aLac);
    }  
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetNetInfoLastSucess()
//
//This method is used to retrieve network information when the SUPL session was last
//done sucessfully. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetNetInfoLastSucess(const TInt64 aSlpId,CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType,
                                   TInt& aMcc,TInt& aMnc,TInt& aCid,TInt& aLac) const
   {
   return iSettingsEngine->GetNetInfoLastSucess(aSlpId,aNetType,aMcc,aMnc,aCid,aLac);
   }
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetNetInfoLastUse()
//
//This method is used to retrieve network information when the SUPL session was last done.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetNetInfoLastUse(const TInt64 aSlpId,CSuplSettingsInternal::TSuplSettingsNetworkType& aNetType,
                                   TInt& aMcc,TInt& aMnc,TInt& aCid,TInt& aLac) const
   {
   return iSettingsEngine->GetNetInfoLastUse(aSlpId,aNetType,aMcc,aMnc,aCid,aLac);
   }
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetEmergencySupportFlag()
//
//This method is used to change parameter which indicates whether server
// supports emergency service or not.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetEmergencySupportFlag( const TInt64 aSlpId, const TBool aEmergencySupportFlag )
    {
    return iSettingsEngine->SetEmergencySupportFlag(aSlpId,aEmergencySupportFlag );             
    }   
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetEmergencySupportFlag()
//
//This method is used to get parameter which indicates whether server
// supports emergency service or not.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetEmergencySupportFlag(const TInt64 aSlpId,TBool& aEmergencySupportFlag) const
    {
    return iSettingsEngine->GetEmergencySupportFlag(aSlpId,aEmergencySupportFlag);
    }
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetTlsSupportFlag()
//
//This method is used to change parameter which indicates whether server
// supports TLS or not. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetTlsSupportFlag( const TInt64 aSlpId, const TBool aTlsSupportFlag )
    {
    return iSettingsEngine->SetTlsSupportFlag(aSlpId,aTlsSupportFlag );             
    }   
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetTlsFlag()
//
//This method is used to retrieve parameter which indicates whether server
// supports TLS or not. 
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetTlsFlag(const TInt64 aSlpId,TBool& aTlsFlag) const
    {
    return iSettingsEngine->GetTlsFlag(aSlpId,aTlsFlag ); 
    }
//--------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetPskTlsSupportFlag()

// This method is used to change parameter which indicates whether server
// supports PSK-TLS or not.
//---------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::SetPskTlsSupportFlag(const TInt64 aSlpId,const TBool aPskTlsSupportFlag)
    {
    return iSettingsEngine->SetPskTlsSupportFlag(aSlpId,aPskTlsSupportFlag ); 
    }
//--------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetPskTlsFlag()

// This method is to retrieve  parameter which indicates whether server
// supports PSK-TLS or not.
//---------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetPskTlsFlag(const TInt64 aSlpId,TBool& aPskTlsFlag) const
    {
    return iSettingsEngine->GetPskTlsFlag(aSlpId,aPskTlsFlag );
    }
            
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetTopPriority()
//
//This method is used to set one server entry as default in SUPL settings.
//--------------------------------------------------------------------------------------
EXPORT_C  TInt CSuplSettingsInternal::SetTopPriority(const TInt64 aSlpId)
    {
    return iSettingsEngine->SetAsDefault(aSlpId);
    }
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetDefaultSlpId()
//
//This method is used to retrieve default SLP identification.
//--------------------------------------------------------------------------------------
EXPORT_C TInt CSuplSettingsInternal::GetDefaultSlpId(TInt64& aSlpId) const
    {
    return iSettingsEngine->GetDefaultSlpId(aSlpId);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::RemoveOnSimChange()
//
//This method is used to delete all servers which have the Sim Change Remove Flag set
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::RemoveOnSimChange()
    {
    return iSettingsEngine->RemoveOnSimChange();
    }
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetFallBackTimerValue()
//
//This Method is used to Set the fallback timer value into the CenRep
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::GetFallBackTimerValue(TInt& aTimerValue)
    {
    return iSettingsEngine->GetFallBackTimer(aTimerValue);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetFallBackTimerValue()
//
//This Method is used to Set the fallback timer value to the SUPL Settings
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::SetFallBackTimerValue(const TInt aTimerValue)
    {
    return iSettingsEngine->SetFallBackTimer(aTimerValue);
    }
    
//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::InsertSessionRecord()
//
//This method is used to insert Active session record into the SUPL session DB
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::InsertSessionRecord(const CTriggerParams* aSessionParamValues)
    {
    return iSettingsEngine->InsertSessionRecord(aSessionParamValues);
    }
    

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetOutstandingTriggers()
//
//This method is used to set outstanding triggers attribute for a given session
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::SetOutstandingTriggers( TInt64 aSessionId,
                                                            TUint64 aOutstandingTriggers )
    {
    return iSettingsEngine->SetOutstandingTriggers(aSessionId,aOutstandingTriggers);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::DeleteSessionRecord()
//
//This method is used to delete a session record corresponding to the given session ID
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::DeleteSessionRecord(TInt64 aSessionId)
    {
    return iSettingsEngine->DeleteSessionRecord(aSessionId);
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::DeleteAllSessionRecords()
//
//This method is used to delete all session records from session table
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::DeleteAllSessionRecords()
    {
    return iSettingsEngine->DeleteAllSessionRecords();
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::GetServerExtensionParams()
//
//This method is used to retrieve extended parameters of given server
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::GetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams ) const
    {
    if( aServerExtnParams )
	    {
	    return iSettingsEngine->GetServerExtensionParams( aSlpId,aServerExtnParams );
	    }
    return KErrArgument;
    }

//-------------------------------------------------------------------------------------
//CSuplSettingsInternal::SetServerExtensionParams()
//
//This method is used to set extended parameters of a given server.
//--------------------------------------------------------------------------------------

EXPORT_C TInt CSuplSettingsInternal::SetServerExtensionParams( TInt64 aSlpId,CServerExtensionParams* aServerExtnParams )
    {
    if( aServerExtnParams )
	    {
	    return iSettingsEngine->SetServerExtensionParams( aSlpId,aServerExtnParams );
	    } 
    return KErrArgument;
    }
//  End of File
