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
* Description:   Header file for Trigger Fix Notification 
*
*/


#ifndef M_MSUPLSESSION_TRIGGERNOTIFIER_H
#define M_MSUPLSESSION_TRIGGERNOTIFIER_H

#include <e32base.h>
/**
*  MSuplSessionTriggerNotifier is a an observer for receiving fix
*  from the supl trigger. Whenever any supl trigger fired, SUPL Connection
*  notifies this observer
*  
*  @lib epossuplgateway.exe
*  @since Series 60 9.2TB //e.g. Series 60 9.2
*/
class MSuplSessionTriggerNotifier
    {    
    public:    
    	enum TResultCode
    		{
			EOutOfRadioCoverage=1, 
			ENoPosition, 
			ENoMeasurement, 
			ENoPositionNoMeasurement, 
			EOutOfMemory, 
			EOutOfMemoryIntermediateReporting, 
			EOther
    		};
    
    public:  // Interface methods
    
    	/**
        * NotifyTrigger.
        * @since Series 60 9.2TB
        * @param aPacket packet received from the network
        * @return void *
        */        
      virtual void NotifyTrigger( 
        				TInt& aRequestId, 
        				TInt& aSETCaps, 
        				TPositionInfo& aPositionInfo 
       ) = 0;
        
        /**
        * NotifyTriggerError.
        * @since Series 60 9.2TB
        * @param aResultCode value indicating the Error
        * @return void *
        */       
      virtual void NotifyTriggerError( 
        				MSuplSessionTriggerNotifier::TResultCode& aResultCode 
       ) = 0;        
        
    };

#endif      // M_MSUPLSESSION_TRIGGERNOTIFIER_H   
            
// End of File
