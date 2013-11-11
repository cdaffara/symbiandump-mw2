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
* Description:  Implementation of class CServerExtensionParams with get and set methods for Server params
*
*/



// INCLUDE FILES

#include "epos_csuplsettingextensionparams.h"
#include "epos_csuplsettingsinternalconstants.h"
#include "epos_csuplsettingsconstants.h"

// CONSTANTS

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CServerExtensionParams::CServerExtensionParams():iServerId(NULL),       
                                        		 iIap1(NULL),
                                        		 iIap2(NULL), 
                                                 iServerAddr(NULL),
                                                 iServerAddType(NULL),
                                                 iIsValueSet(EFalse)
                                       
    {
    }

EXPORT_C CServerExtensionParams* CServerExtensionParams::NewL()
    {
    CServerExtensionParams* self = new (ELeave) CServerExtensionParams();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CServerExtensionParams* CServerExtensionParams::NewLC()
    {
    CServerExtensionParams* self = new (ELeave) CServerExtensionParams();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// EPOC default constructor can leave.
void CServerExtensionParams::ConstructL()
    {
      
    }

// Destructor
EXPORT_C  CServerExtensionParams::~CServerExtensionParams()
    {
    delete iServerId;
    iServerId = NULL;
    delete iIap1;
    iIap1 = NULL;
    delete iIap2;
    iIap2 = NULL;
    delete iServerAddr;
    iServerAddr = NULL;
    delete iServerAddType;
    iServerAddType = NULL;
   

    }
// ---------------------------------------------------------------------------
// CServerExtensionParams::Set()
//
// Sets server parameters.
// ---------------------------------------------------------------------------
EXPORT_C TInt CServerExtensionParams::Set( const TDesC& aServerId,
                						   const TDesC& aIap1,
                						   const TDesC& aIap2,
                						   const TDesC& aServerAddr,
                						   const TDesC& aServerAddType
                                         )
    {
    // Reset the state first
    delete iServerId;
    iServerId = NULL;
    delete iIap1;
    iIap1 = NULL;
    delete iIap2;
    iIap2 = NULL;
    delete iServerAddType;
    iServerAddType = NULL;
    delete iServerAddr;
    iServerAddr = NULL;
    
    if(aServerId.Length() > 0 && aServerId.Length() <= KMaxServerIdLen)
        {
        iServerId = HBufC::New(aServerId.Length());
        if(iServerId)
	        {
	        iServerId->Des().Copy(aServerId);
	        }
            
        else
	        {
	        return KErrNoMemory;
	        }
    	}
    
    else if(aServerId.Length() == 0)
	    {
	    iServerId = NULL;
	    }
       
    else
	    {
	    return KErrArgument;
	    }
        
        
    if(aIap1.Length() > 0 && aIap1.Length() <= KMaxIAPLen)
        {
        iIap1 = HBufC::New(aIap1.Length());
        if(iIap1)
	        {
	        iIap1->Des().Copy(aIap1);
	        }
            
        else
	        {
	        return KErrNoMemory;
	        }
    	}
    
    else if(aIap1.Length() == 0)
	    {
	    iIap1 = NULL;
	    }
       
    else
	    {
	    return KErrArgument;
	    }
	    
	if(aIap2.Length() > 0 && aIap2.Length() <= KMaxIAPLen)
        {
        iIap2 = HBufC::New(aIap2.Length());
        if(iIap2)
	        {
	        iIap2->Des().Copy(aIap2);
	        }
            
        else
	        {
	        return KErrNoMemory;
	        }
    	}
    else if(aIap2.Length() == 0)
	    {
	    iIap2 = NULL;
	    }
    else
	    {
	    return KErrArgument;
	    }
	    
	if(aServerAddType.Length() > 0 && aServerAddType.Length() <= KMaxServerAddTypeLen)
        {
        iServerAddType = HBufC::New(aServerAddType.Length());
        if(iServerAddType)
	        {
	        iServerAddType->Des().Copy(aServerAddType);
	        }
            
        else
	        {
	        return KErrNoMemory;
	        }
    	}
    
    else if(aServerAddType.Length() == 0)
	    {
	    iServerAddType = NULL;
	    }
    else
	    {
	    return KErrArgument;
	    }
	    
	if(aServerAddr.Length() > 0 && aServerAddr.Length() <= KMaxServerAddLen)
        {
        iServerAddr = HBufC::New(aServerAddr.Length());
        if(iServerAddr)
	        {
	        iServerAddr->Des().Copy(aServerAddr);
	        }
            
        else
	        {
	        return KErrNoMemory;
	        }
    	}
    
    else if(aServerAddr.Length() == 0)
	    {
	    iServerAddr = NULL;
	    }
    else
	    {
	    return KErrArgument;
	    }               
	
       
    iIsValueSet = ETrue;
    return KErrNone;            
    }
// ---------------------------------------------------------------------------
// CServerExtensionParams::Get()
//
// Returns server parameters.
// ---------------------------------------------------------------------------
EXPORT_C TInt CServerExtensionParams::Get( TPtr aServerId,
                						   TPtr aIap1,
                                           TPtr aIap2,
                                           TPtr aServerAddr,
                                           TPtr aServerAddType
                                         ) const
    {
    if(!iIsValueSet)
	    {
	    //if values were not set before using the Set method, return error
	    return KErrNotFound;
	    }
        
    if( iServerId )
        {
        if( aServerId.Ptr() == NULL )
            {
            return KErrArgument;
            }
        if( iServerId->Length() > aServerId.MaxLength() )
            {
            return KErrOverflow;
            }
        aServerId = iServerId->Des();
        }
    else
        {
        aServerId.Zero();
        }
        
    if( iIap1 )
        {
        if( aIap1.Ptr() == NULL )
            {
            return KErrArgument;
            }
        if( iIap1->Length() > aIap1.MaxLength() )
            {
            return KErrOverflow;
            }
        aIap1 = iIap1->Des();
        }
    else
        {
        aIap1.Zero();
        }
        
    if( iIap2 )
        {
        if( aIap2.Ptr() == NULL )
            {
            return KErrArgument;
            }
        if( iIap2->Length() > aIap2.MaxLength())
            {
            return KErrOverflow;
            }
        aIap2 = iIap2->Des();
        }
    else
        {
        aIap2.Zero();
        }
        
    if( iServerAddType )
        {
        if( aServerAddType.Ptr() == NULL )
            {
            return KErrArgument;
            }
        if( iServerAddType->Length() > aServerAddType.MaxLength() )
            {
            return KErrOverflow;
            }
        aServerAddType = iServerAddType->Des();
        }
    else
        {
        aServerAddType.Zero();
        }
        
    if( iServerAddr )
        {
        if( aServerAddr.Ptr() == NULL )
            {
            return KErrArgument;
            }
        if( iServerAddr->Length() > aServerAddr.MaxLength() )
            {
            return KErrOverflow;
            }
        aServerAddr = iServerAddr->Des();
        }
    else
        {
        aServerAddr.Zero();
        }                
    return KErrNone;            
    }
//  End of File

