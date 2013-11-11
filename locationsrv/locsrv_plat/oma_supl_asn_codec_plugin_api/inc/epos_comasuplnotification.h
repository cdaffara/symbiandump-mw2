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
* Description:  Header file for Notification in Supl Init
*
*/

#ifndef C_COMASUPLNOTIFICATION_H
#define C_COMASUPLNOTIFICATION_H


// INCLUDES
#include <e32std.h>
#include <e32def.h>
#include <e32base.h>


// CLASS DECLARATION
/**
 *  contains notification information
 *  
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */

class COMASuplNotification: public CBase
    {
    public:
    /**
    ** enumeration for type of notification
    **/
    enum TOMASuplNotificationType
    	{
    		ENoNotificationNoVerification = 0,
    		ENotificationOnly			  = 1,
    		ENotificationAndVerficationAllowedNA,
    		ENotificationAndVerficationDeniedNA,
    		EPrivacyOverride
    	};
    /**
    ** enumeration for type of encoding
    **/
    enum TOMASuplEncodingType
    	{
    		EUCS2 = 0,
    		EGsmDefault = 1,
    		EUtf8
    	};
    /**
    ** enumeration for type of format
    **/
    enum TOMASuplFormatIndicator
    	{
    		ELogicalName  = 0,
    		EEMailAddress = 1,
    		EMsisdn       = 2,
    		EUrl 		  = 3,
    		ESipUrl,  
    		EMin,
    		EMdn,
    		EIMSPublicIdentity    		
    	};
    	
    public:
   		 /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return Instance of COMASuplNotification
        */          
    
        IMPORT_C static COMASuplNotification* NewL();
        
       /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
        IMPORT_C virtual ~COMASuplNotification();
    
    public: // New functions
        
        /**
        * SuplNotification
        * @since S60 S60_3.1u
        * @param None
        * @return type of notification
        */
        
        IMPORT_C TOMASuplNotificationType SuplNotification()const;
        
        /**
        * SetSuplNotification
        * @since S60 S60_3.1u
        * @param type of notification
        * @return none
        */
        
        IMPORT_C void SetSuplNotification(TOMASuplNotificationType aNotificationType);
        

    	/**
        * RequestorId : Gets Id of requestor and type of requestor
        * @since S60 S60_3.1u
        * @param aRequestorId Id of requestor
        * @param aRequestorType type of requestor
        * @return Error in case param not set
        */
        
        IMPORT_C TInt RequestorId(HBufC8*& aRequestorId,
        								TOMASuplFormatIndicator& aRequestorType)const;
        /**
        * RequestorId : Sets Id of requestor and type of requestor
        * @since S60 S60_3.1u
        * @param aRequestorId Id of requestor-The ownsership of requestor is passed to COMASuplNotification
        * @param aRequestorType type of requestor
        * @return None
        */
        
        IMPORT_C void SetRequestorId(HBufC8* aRequestorId,
        								TOMASuplFormatIndicator aRequestorType);
        
        /**
        * ClientName : Gets client name and type of clientname
        * @since S60 S60_3.1u
        * @param aClientName name of client
        * @param aClientType type of client
        * @return Error in case param not set
        */
        
        IMPORT_C TInt ClientName(HBufC8*& aClientName,TOMASuplFormatIndicator& aClientType)const;
        
        /**
        * ClientName : Sets client name and type of client name
        * @since S60 S60_3.1u
        * @param aClientName name of client-The ownsership of clientname is passed to COMASuplNotification
        * @param aClientType type of client
        * @return none
        */
        
        IMPORT_C void SetClientName(HBufC8* aClientName,TOMASuplFormatIndicator aClientType);
        
        
        /**
        * EncodingType : Gets encoding type
        * @since S60 S60_3.1u
        * @param aEncodingType encoding type
        * @return Error in case param not set
        */
        
        IMPORT_C TInt EncodingType(TOMASuplEncodingType& aEncodingType)const;
        
        /**
        * SetEncodingType : Sets encoding type
        * @since S60 S60_3.1u
        * @param aEncodingType encoding type
        * @return none
        */
        
        IMPORT_C void SetEncodingType(TOMASuplEncodingType aEncodingType);
    
    protected:
     COMASuplNotification();
     
    private:     // Data
        // Type of notification
    	TOMASuplNotificationType  iNotificatonType;
    	//Type of requestor id
    	TOMASuplFormatIndicator   iRequestorType;
    	// type of client name
    	TOMASuplFormatIndicator   iClientType;
    	// Requestor Id
    	HBufC8* 			   iRequestorId;
    	// Client Name
    	HBufC8* 			   iClientName;
    	// Type of encoding
    	TOMASuplEncodingType 	   iEncodingType;
    	// Bit mask for optional parameters
    	TInt 				   iOptionalMask;
    };

#endif //C_COMASUPLNOTIFICATION_H
