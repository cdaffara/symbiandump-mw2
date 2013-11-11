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
* Description:  Implementation of class CServerParams with get and set methods for Server params
*
*/



// INCLUDE FILES

#include "epos_csuplsettingparams.h"
#include "epos_csuplsettingsconstants.h"

// CONSTANTS

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CServerParams::CServerParams():iSlpId (-1),       
                                        iServerAddress (NULL),
                                        iIap (NULL), 
                                        iServerEnabled(ETrue),                                            
                                        iSimChangeRemove  (ETrue),
                                        iUsageInHomeNw (ETrue),
                                        iEditable (ETrue),
                                        iProductConfigured(ETrue),
                                        iIsValueSet(EFalse)
    {

    }

EXPORT_C CServerParams* CServerParams::NewL()
    {
    CServerParams* self = new (ELeave) CServerParams();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

EXPORT_C CServerParams* CServerParams::NewLC()
    {
    CServerParams* self = new (ELeave) CServerParams();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// EPOC default constructor can leave.
void CServerParams::ConstructL()
    {
      
    }

// Destructor
EXPORT_C  CServerParams::~CServerParams()
    {
    if(iServerAddress)
        {
        delete iServerAddress;
        iServerAddress = NULL;
        }

    if(iIap)
        {
        delete iIap;
        iIap = NULL;
        }

    }
    
    
// ---------------------------------------------------------------------------
// CServerParams::Set()
//
// Sets server parameters.
// ---------------------------------------------------------------------------
EXPORT_C TInt CServerParams::Set( const TDesC& aServerAddress,          
                const TDesC& aIapName, const TBool aServerEnabled,
                const TBool aSimChangeRemove, const TBool aUsageInHomeNw,
                const TBool aEditable,const TInt64 aSlpId)
    {
    iSlpId = aSlpId;
    if(aServerAddress.Length() > 0 && aServerAddress.Length() < KMaxHSLPAddrLen)
        {
        delete iServerAddress;
        iServerAddress = NULL;
        iServerAddress = HBufC::New(aServerAddress.Length());
        if(iServerAddress)
            iServerAddress->Des().Copy(aServerAddress);
        else
            return KErrNoMemory;
        }
    else
        return KErrArgument;
        
    if(aIapName.Length() > 0 && aIapName.Length() < KMaxIAPLen)
        {
        delete iIap;
        iIap = NULL;
        iIap = HBufC::New(aIapName.Length());
        if(iIap)
            iIap->Des().Copy(aIapName);
        else
            return KErrNoMemory;
        }
    else if(aIapName.Length() > 0) //zero length allowed in some cases
        return KErrArgument;
    
    iServerEnabled = aServerEnabled;
    iSimChangeRemove = aSimChangeRemove;
    iUsageInHomeNw = aUsageInHomeNw;
    iEditable = aEditable;
    iIsValueSet = ETrue;
    return KErrNone;            
    }
    
// ---------------------------------------------------------------------------
// CServerParams::Get()
//
// Returns server parameters.
// ---------------------------------------------------------------------------
EXPORT_C TInt CServerParams::Get(TInt64 &aSlpId, TPtr aServerAddress,         
                TPtr aIapName,TBool& aServerEnabled,
                TBool& aSimChangeRemove, TBool& aUsageInHomeNw, TBool& aEditable) const
    {
    	
    if(!iIsValueSet) //if values were not set before using the Set method, return error
        return KErrNotFound;
    
    aSlpId = iSlpId;
    
    if(iServerAddress)
        aServerAddress = iServerAddress->Des();
    else
        return KErrNotFound;
    if(iIap)
        aIapName = iIap->Des();
    else
        aIapName.Zero();
    aServerEnabled = iServerEnabled;
    aSimChangeRemove = iSimChangeRemove;
    aUsageInHomeNw = iUsageInHomeNw;
    aEditable = iEditable;
    return KErrNone;            
    
    }
    

EXPORT_C void CServerParams::SetServerConfigurationType(TBool aConfigurationType)
	{
		iProductConfigured = aConfigurationType;
	}


EXPORT_C TInt CServerParams::GetServerConfigurationType(TBool& aConfigurationType)const
	{
    if(!iIsValueSet) //if values were not set before using the Set method, return error
        return KErrNotFound;
        
			aConfigurationType = iProductConfigured;
			return KErrNone;            
			
	}

// C++ default constructor can NOT contain any code, that
// might leave.
//
CTriggerParams::CTriggerParams(): iSessionId (0), 
                                  iSessionName(NULL),
                                  iNotificationPresent(EFalse),
                                  iTriggerNotificationStatus(EFalse),
                                  iTriggerType(EUnknown), 
                                  iRequestType(EUnSpecified),
                                  iOutstandingTrigger(0),                                            
                                  iInterval(0),
                                  iEndTime(0),
                                  iIsValueSet(EFalse)
    {

    }

EXPORT_C CTriggerParams* CTriggerParams::NewL()
    {
    CTriggerParams* self = new (ELeave) CTriggerParams();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

EXPORT_C CTriggerParams* CTriggerParams::NewLC()
    {
    CTriggerParams* self = new (ELeave) CTriggerParams();

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// EPOC default constructor can leave.
void CTriggerParams::ConstructL()
    {
      
    }

// Destructor
EXPORT_C  CTriggerParams::~CTriggerParams()
    {
    delete iSessionName;
    iSessionName = NULL;
    
    }
// ---------------------------------------------------------------------------
// CTriggerParams::Set()
//
// Sets server parameters.
// ---------------------------------------------------------------------------
EXPORT_C TInt CTriggerParams::Set( TInt64 aSessionId,
                                   const TDesC& aSessionName,
                                   TBool aNotificationPresent,
                				   TBool aTriggerNotificationStatus,
                				   TTriggerType aTriggerType,
                				   TRequestType aRequestType,
                				   TUint64  aOutstandingTrigger,
                				   TUint64  aInterval
                                  )
    {
    if(aSessionName.Length() > 0 && aSessionName.Length() < KMaxTriggerSessionNameLen)
        {
        delete iSessionName;
        iSessionName = NULL;
        iSessionName = HBufC::New(aSessionName.Length());
        if(iSessionName)
            {
            iSessionName->Des().Copy(aSessionName);
            }
        else
            {
            return KErrNoMemory;
            }
        }
    else if ( aSessionName.Length() == 0 )
        {
        // Set it to NULL & proceed.
        iSessionName = NULL;
        }
    else // Length must be > KMaxTriggerSessionNameLen
        {
        return KErrArgument;
        }
    
    iSessionId = aSessionId;
    iNotificationPresent = aNotificationPresent;
    iTriggerNotificationStatus = aTriggerNotificationStatus;
    iTriggerType = aTriggerType;
    iRequestType = aRequestType;
    iOutstandingTrigger = aOutstandingTrigger;
    iInterval = aInterval;
    iIsValueSet = ETrue;
    
    return KErrNone;            
    }
// ---------------------------------------------------------------------------
// CTriggerParams::Get()
//
// Returns server parameters.
// ---------------------------------------------------------------------------
EXPORT_C TInt CTriggerParams::Get( TInt64& aSessionId,
                                   TPtr aSessionName,
                                   TBool& aNotificationPresent,
                				   TBool& aTriggerNotificationStatus,
                				   TTriggerType& aTriggerType,
                				   TRequestType& aRequestType,
                				   TUint64&  aOutstandingTrigger,
                                   TUint64&  aInterval
                                  ) const
    {
    if( !iIsValueSet )//if values were not set before using the Set method, return error
        {
        return KErrNotFound;
        }
    if( iSessionName )
        {
        if( aSessionName.Ptr() == NULL )
            {
            return KErrArgument;
            }
        if( iSessionName->Length() > aSessionName.MaxLength() )
            {
            return KErrOverflow;
            }
        aSessionName = iSessionName->Des();
        }
    else
        {
        aSessionName.Zero();
        }
    aSessionId = iSessionId;
    aNotificationPresent = iNotificationPresent;
    aTriggerNotificationStatus = iTriggerNotificationStatus;
    aTriggerType = iTriggerType;
    aRequestType = iRequestType;
    aOutstandingTrigger = iOutstandingTrigger;
    aInterval = iInterval;
    return KErrNone;            
    }

// ---------------------------------------------------------------------------
// CTriggerParams::GetTriggerEndTime()
//
// Returns server parameters.
// ---------------------------------------------------------------------------
EXPORT_C TInt CTriggerParams::GetTriggerEndTime(TTime& aEndTime)const                
		{
	    if( !iIsValueSet ) //if values were not set before using the Set method, return error
        {
        return KErrNotFound;
        }

			aEndTime = iEndTime.Int64();
			return KErrNone;            
		}

// ---------------------------------------------------------------------------
// CTriggerParams::SetTriggerEndTime()
//
// Returns server parameters.
// ---------------------------------------------------------------------------		
EXPORT_C void CTriggerParams::SetTriggerEndTime(const TTime aEndTime)
		{
			iEndTime = aEndTime.Int64();
		}               

//  End of File

