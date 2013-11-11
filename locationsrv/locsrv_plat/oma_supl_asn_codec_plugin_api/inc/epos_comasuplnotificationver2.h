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
* Description:  Header file for supl 2.0 notification message
*
*/

#ifndef C_COMASuplNotificationVer2_H
#define C_COMASuplNotificationVer2_H


// INCLUDES
#include <e32std.h>
#include <e32def.h>
#include <e32base.h>
#include "epos_suplconstants.h"
#include "epos_comasuplnotification.h"
#include "epos_comasuplresponsever2.h"
#include "epos_comasupltriggeredstart.h"

class COMASuplSLPAddress;


class COMASuplNotificationExtnV2:public CBase
    {
public:
        /**
       * Two phased constructor
       * @since S60 S60_5.2
       * @param None
       * @return Instance of COMASuplNotificationExtnV2
       */          
   
       IMPORT_C static COMASuplNotificationExtnV2* NewL();
       
      /**
       * Destructor
       * @since S60 S60_5.2
       * @param None
       * @return None
       */
       IMPORT_C virtual ~COMASuplNotificationExtnV2();
       
   private:
       COMASuplNotificationExtnV2();
       
   private:
       
    };
// CLASS DECLARATION
/**
 *  contains notification information
 *  
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */

class COMASuplNotificationVer2: public COMASuplNotification
    {
    
    public:
   		 /**
        * Two phased constructor
        * @since S60 S60_5.2
        * @param None
        * @return Instance of COMASuplNotificationVer2
        */          
    
        IMPORT_C static COMASuplNotificationVer2* NewL();
        
       /**
        * Destructor
        * @since S60 S60_5.2
        * @param None
        * @return None
        */
        IMPORT_C virtual ~COMASuplNotificationVer2();
        
    private:
        COMASuplNotificationVer2();
        
    public:
        /** 
         * Setter method for setting COMASuplNotificationExtnV2
         * @since S60 S60_5.2
         * @param aV2NotificationExtn Version 2 notification extn to be set
         *          ownership is transferred to the notification object
         */
        IMPORT_C void SetV2NotifyExtn(COMASuplNotificationExtnV2* aV2NotificationExtn);
        
        /** 
         * Getter method for setting COMASuplNotificationExtnV2
         * @since S60 S60_5.2
         * @param aV2NotificationExtn reference of Version 2 notification extn is returned
         *          ownership will be still be retined by the notification object
         */
        IMPORT_C void GetV2NotifyExtn(COMASuplNotificationExtnV2*& aV2NotificationExtn);
     
    private:     // Data
        COMASuplNotificationExtnV2* iV2NotificationExtn;
    };

#endif //C_COMASUPLNOTIFICATION_H
