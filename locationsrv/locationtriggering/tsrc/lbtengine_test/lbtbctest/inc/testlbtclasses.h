/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Test cases for BC
*
*/




#ifndef CLBSTESTREQUESTOR_H
#define CLBSTESTREQUESTOR_H

// System Includes
#include <StifLogger.h>
#include <StifParser.h>
#include <lbtcommon.h>
#include <lbterrors.h>
#include <lbtgeoareabase.h>
#include <lbtgeocircle.h>
#include <lbtgeocell.h>
#include <lbtgeorect.h>
#include <lbtlisttriggeroptions.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbtserver.h>
#include <lbttriggerchangeevent.h>
#include <lbttriggerchangeeventnotifier.h>
#include <lbttriggerchangeeventobserver.h>
#include <lbttriggerconditionarea.h>
#include <lbttriggerconditionbase.h>
#include <lbttriggerdynamicinfo.h>
#include <lbttriggerentry.h>
#include <lbttriggerfilterbase.h>
#include <lbttriggerfilterbyarea.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerfiltercomposite.h>
#include <lbttriggerfiringeventnotifier.h>
#include <lbttriggerfiringeventobserver.h>
#include <lbttriggerinfo.h>
#include <lbttriggeringsystemsettings.h>
#include <lbttriggeringsystemsettingschangeeventnotifier.h>
#include <lbttriggeringsystemsettingschangeeventobserver.h>
#include <lbscommon.h>
#include <LbsRequestor.h>
#include <LbsPosition.h>
#include <LbsPositionInfo.h>
#include <lbs.h>
#include <e32property.h>




class TTestlbtclasses
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        TTestlbtclasses(CStifLogger* aLog);

        /**
        * Destructor. 
        */
        ~TTestlbtclasses();

    public:         
        
        void AssertTrueL( TBool aCondition,const TDesC& aMessage);          
        void EnableSimPSYL( );            
		  
        // New functions  
	    
	    TInt TestCLbtGeoCell1L(CStifItemParser& aItem);
	    TInt TestCLbtGeoCell2L(CStifItemParser& aItem);
	    TInt TestCLbtGeoCell3L(CStifItemParser& aItem);
	    TInt TestCLbtGeoCell4L(CStifItemParser& aItem);
	    TInt TestCLbtGeoCell5L(CStifItemParser& aItem);
	    TInt TestCLbtGeoCell6L(CStifItemParser& aItem);
	    TInt TestCLbtGeoCell7L(CStifItemParser& aItem);
	    TInt TestCLbtGeoCircle1L(CStifItemParser& aItem);
		TInt TestCLbtGeoCircle2L(CStifItemParser& aItem);
		TInt TestCLbtGeoCircle3L(CStifItemParser& aItem);
		TInt TestCLbtGeoCircle4L(CStifItemParser& aItem);
		TInt TestCLbtGeoCircle5L(CStifItemParser& aItem);
		TInt TestCLbtGeoCircle6L(CStifItemParser& aItem);
		TInt TestCLbtGeoCircle7L(CStifItemParser& aItem);
		TInt TestCLbtGeoRect1L(CStifItemParser& aItem);   
		TInt TestCLbtGeoRect2L(CStifItemParser& aItem);  
		TInt TestCLbtGeoRect3L(CStifItemParser& aItem);  
		TInt TestCLbtGeoRect4L(CStifItemParser& aItem);  
		TInt TestCLbtGeoRect5L(CStifItemParser& aItem);  
		TInt TestCLbtListTriggerOptions1L(CStifItemParser& aItem);  
		TInt TestCLbtListTriggerOptions2L(CStifItemParser& aItem);  
		TInt TestCLbtListTriggerOptions3L(CStifItemParser& aItem);  
		TInt TestCLbtListTriggerOptions4L(CStifItemParser& aItem);  
		TInt TestCLbtListTriggerOptions5L(CStifItemParser& aItem);  
		TInt TestCLbtListTriggerOptions6L(CStifItemParser& aItem);  
		TInt TestRLbtServerL(CStifItemParser& aItem);  
		TInt TestCLbtSessionTrigger1L(CStifItemParser& aItem);  		  
		TInt TestCLbtSessionTrigger2L(CStifItemParser& aItem);  		  
		TInt TestCLbtSessionTrigger3L(CStifItemParser& aItem);  		  
		TInt TestCLbtSessionTrigger4L(CStifItemParser& aItem);  
		TInt TestCLbtSessionTrigger5L(CStifItemParser& aItem);  
		TInt TestCLbtStartupTrigger1L(CStifItemParser& aItem); 
		TInt TestCLbtStartupTrigger2L(CStifItemParser& aItem);
		TInt TestCLbtStartupTrigger3L(CStifItemParser& aItem);
		TInt TestCLbtStartupTrigger4L(CStifItemParser& aItem); 	
		TInt TestCLbtStartupTrigger5L(CStifItemParser& aItem); 	
		TInt TestCLbtTriggerChangeEventNotifierL(CStifItemParser& aItem);	  
		TInt TestCLbtTriggerConditionArea1L(CStifItemParser& aItem);
		TInt TestCLbtTriggerConditionArea2L(CStifItemParser& aItem);
		TInt TestCLbtTriggerConditionArea3L(CStifItemParser& aItem);
		TInt TestCLbtTriggerConditionArea4L(CStifItemParser& aItem);	
		TInt TestCLbtTriggerConditionArea5L(CStifItemParser& aItem);
		TInt TestCLbtTriggerConditionArea6L(CStifItemParser& aItem);	
		TInt TestCLbtTriggerConditionArea7L(CStifItemParser& aItem);	
		TInt TestCLbtTriggerConditionArea8L(CStifItemParser& aItem);	
		TInt TestCLbtTriggerEntry1L(CStifItemParser& aItem);
		TInt TestCLbtTriggerEntry2L(CStifItemParser& aItem); 	
		TInt TestCLbtTriggerFilterByArea1L(CStifItemParser& aItem);	  
		TInt TestCLbtTriggerFilterByArea2L(CStifItemParser& aItem);	  
		TInt TestCLbtTriggerFilterByArea3L(CStifItemParser& aItem);	  
		TInt TestCLbtTriggerFilterByArea4L(CStifItemParser& aItem);	  
		TInt TestCLbtTriggerFilterByArea5L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterByArea6L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterByArea7L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterByArea8L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterByAttribute1L(CStifItemParser& aItem);	  
		TInt TestCLbtTriggerFilterByAttribute2L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterByAttribute3L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterComposite1L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterComposite2L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterComposite3L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFilterComposite4L(CStifItemParser& aItem);
		TInt TestCLbtTriggerFiringEventNotifierL(CStifItemParser& aItem);
		TInt TestCLbtTriggerInfo1L(CStifItemParser& aItem);
		TInt TestCLbtTriggerInfo2L(CStifItemParser& aItem);
		TInt TestCLbtTriggerInfo3L(CStifItemParser& aItem);
		TInt TestCLbtTriggerInfo4L(CStifItemParser& aItem);
		TInt TestTLbtTriggeringSystemSettingsL(CStifItemParser& aItem);
		TInt TestCLbtTriggeringSystemSettingsChangeEventNotifier1L(CStifItemParser& aItem);
			
			
			
	private:
	    
	   
	   
	    
    private:  //data
	   
	    /**
	     * Pointer to Stif Logger - Does not Own
	     */           
        CStifLogger* 			iLog;
        
     };     

#endif      //  CLBSTESTREQUESTOR_H

// End of File
