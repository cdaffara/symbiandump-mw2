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
* Description:  Header file for supl init message
*
*/

#ifndef C_COMASUPLINIT_H
#define C_COMASUPLINIT_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplposmethod.h"
#include "epos_tomasuplqop.h"

//Forward declaration
class COMASuplNotification;
class COMASuplSLPAddress;

/**
 *  Class for providing the suplinit message
 *  For holding the Supl Init message
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */
class COMASuplInit  : public COMASuplAsnMessageBase
 {
 		public:
 		/**
		*  Enumeration for holding the SLP Mode
		*/
		enum TOMASuplSLPMode
		{
			EProxy = 0,
		    ENonProxy
		};
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplInit pointer to SuplInit Message
        */          
    
	    IMPORT_C static COMASuplInit* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
     	IMPORT_C virtual ~COMASuplInit();
	    
	    /**
        * Getter method for SuplInit
        * @since S60 S60_3.1u
        * @param aPosMethod Pos Method 
        * @param aSLPMode SLP Mode to indcate proxy or non proxy
        * @return Returns None
        */
	 	IMPORT_C void SuplInit(TOMASuplPosMethod& aPosMethod,TOMASuplSLPMode& aSLPMode)const;
	 	
	 	/**
        * Setter method for Supl Init
        * @since S60 S60_3.1u
        * @param aPosMethod PosMethod
        * @param aSLPMode SLPMode
        * @return Returns None
        */
	 	IMPORT_C void SetSuplInit(TOMASuplPosMethod aPosMethod,TOMASuplSLPMode aSLPMode);
        
	    /**
        * Getter method for SLP Address
        * @since S60 S60_3.1u
        * @param aAddress A reference to the SLP address present in the SUPL Init message
        *                 is returned in this. Ownership stays with the SUPL Init Message object.
        * @return Returns Error if param not set
        */
	 	IMPORT_C TInt SLPAddress(COMASuplSLPAddress*& aAddress)const;
	 	
	 	/**
        * Setter method for SLP Address
        * @since S60 S60_3.1u
        * @param aAddress SLP address to be set into the SUPL Init message.Ownership stays with
        *                 the SUPL Init object.
        * @return Returns None
        */
	 	IMPORT_C void SetSLPAddress(COMASuplSLPAddress* aAddress);
	 	
	 	/**
        * Getter method for Notification
        * @since S60 S60_3.1u
        * @param aNotification  A reference to the Notification field present in the SUPL Init message
        *                 is returned in this. Ownership stays with the SUPL Init Message object.
        * @return Returns Error if param not set
        */
	 	IMPORT_C TInt Notification(COMASuplNotification*& aNotification)const;
	 	
	 	/**
        * Setter method for Notification
        * @since S60 S60_3.1u
        * @param aNotification Notification field to be set in the SUPL Init  object. 
        *                      Ownership is transferred to the SUPL Init object. 
        * @return Returns None
        */
	 	IMPORT_C void SetNotification(COMASuplNotification* aNotification);
        
        /**
        * Getter method for Qop in Supl Init
        * @since S60 S60_3.1
        * @param aSuplQop Quality of position is retrieved in this object.
        * @return error code in case Qop is not set
        */
		
		IMPORT_C TInt Qop(TOMASuplQop& aSuplQop)const;
		
        /**
        * Setter method for Qop in Supl init
        * @since S60 S60_3.1
        * @param aSuplQop Quality of position to be set in the SUPL Init object.
        * @return Returns None
        */
		
		IMPORT_C void SetQop(TOMASuplQop& aSuplQop);
		
		public: // Functions from base classes
	     
	     /**
        * Method to clone 
        * @since S60 S60_3.1u
        * @param aSuplInit suplinit pointer
        * @param aErrorCode errorcode
        * @return None
        */
	     IMPORT_C void Clone(const COMASuplAsnMessageBase* aSuplInit,TInt& aErrorCode);
	     
 protected:   
     
      IMPORT_C void ConstructL();        
      
      IMPORT_C COMASuplInit();
 protected:
     
       /**
        * Method to clone the notification if present
        * @since S60 S60_3.1u
        * @param aSuplInit SuplInit to clone
        * @return Returns None
        */
       void CloneNotificationL(const COMASuplInit* aSuplInit);
       /**
        * Method to clone the SlpAddres if present
        * @since S60 S60_3.1u
        * @param aSuplInit SuplInit to clone
        * @return Returns None
        */
       void CloneSLPAddressL(const COMASuplInit* aSuplInit);

 protected:    // Data
    
    //Pos Method
	TOMASuplPosMethod iPosMethod;
	
	// SLP Mode
    TOMASuplSLPMode   iSLPMode;
    
    // Notification
    COMASuplNotification* iNotification;
    
    // SLP Address
    COMASuplSLPAddress* iSLPAddress;
    
    //Quality of position
    TOMASuplQop iSuplQop;
    };


#endif // C_COMASUPLINIT_H
