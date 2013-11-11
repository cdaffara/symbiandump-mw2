/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is the implementation of the CAudioOutputControlUtilityProxy class.
*
*/



// INCLUDE FILES
#include <AudioOutputControlUtilityProxy.h>
#include "AudioOutputControlUtilityProxyImpl.h"


// Two-phased constructor.
EXPORT_C CAudioOutputControlUtilityProxy* CAudioOutputControlUtilityProxy::NewL()
    {
#ifdef _DEBUG    
	RDebug::Print(_L("CAudioOutputControlUtilityProxy::NewL\n"));
#endif	  
	
	CAudioOutputControlUtilityProxy* self = new (ELeave)CAudioOutputControlUtilityProxy;
  
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 		  	
 		  	 
    return self;
    }
    
// -----------------------------------------------------------------------------
// CAudioOutputControlUtilityProxy::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAudioOutputControlUtilityProxy::ConstructL()
    {
#ifdef _DEBUG    
	RDebug::Print(_L("AudioOutputControlUtilityProxy::ConstructL\n"));  
#endif

    CAudioOutputControlUtilityImpl* aImpl = CAudioOutputControlUtilityProxyImpl::NewL();
    // Transfer ownership here
  	CAudioOutputControlUtility::ConstructL(aImpl);

    }
        
CAudioOutputControlUtilityProxy::CAudioOutputControlUtilityProxy()

	{
#ifdef _DEBUG
	RDebug::Print(_L("CAudioOutputControlUtilityProxy[%x]:CAudioOutputControlUtilityProxy\n"),this);
#endif
	}    

       
// -----------------------------------------------------------------------------
// CAudioOutputControlUtilityProxy::DestructL
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CAudioOutputControlUtilityProxy::~CAudioOutputControlUtilityProxy()
    {
#ifdef _DEBUG    
	RDebug::Print(_L("CAudioOutputControlUtilityProxy[%x]::~AudioOutputControlUtilityProxy\n"),this); 
#endif	
    }
   
    
// End of file

