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
* Description:  Header file for supl response message
*
*/

#ifndef C_COMASUPLRESPONSE_H
#define C_COMASUPLRESPONSE_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplposmethod.h"

//Forward declaration
class COMASuplSLPAddress;

/**
 *  Class for providing the suplresponse message
 *  For holding the Supl Response message
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */

class COMASuplResponse  : public COMASuplAsnMessageBase
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplResponse pointer to SuplResponse Message
        */         
    
	    IMPORT_C static COMASuplResponse* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */ 
     	IMPORT_C ~COMASuplResponse();
                

       
   	public: 
	    /**
        * Getter method for SuplResponse
        * @since S60 S60_3.1u
        * @param None
        * @return Returns the type of PosMethod  to use
        */
	 	IMPORT_C TOMASuplPosMethod SuplResponse() const;
	 	
        		
		/**
        * Getter method for SLP Address
        * @since S60 S60_3.1u
        * @param aAddress SLP address
        * @return Returns Error if param not set
        */
	 	IMPORT_C TInt SLPAddress(COMASuplSLPAddress*& aAddress)const;
	 	
	public:		//Setter Methods

	 	
	 	/**
        * Setter method for SLP Address
        * @since S60 S60_3.1u
        * @param aAddress SLP address - ownership transeffered to COMASuplResponse
        * @return Returns None
        */
	 	IMPORT_C void SetSLPAddress(COMASuplSLPAddress* aAddress);
	 	
        /**
        * Setter method for Supl response
        * @since S60 S60_3.1u
        * @param aSuplPosMethod posmethod
        * @return Returns None
        */
		
		IMPORT_C void SetSuplResponse(TOMASuplPosMethod aSuplPosMethod);
	    
	public:
	
		/**
        * Cloning of SuplResponse
        * @since S60 S60_3.1u
        * @param aResponse Base pointer
        * @param aErrorCode error code
        * @return Returns None
        */
        
		IMPORT_C void Clone(const COMASuplAsnMessageBase* aResponse,TInt& aErrorCode);
    
                
    protected: 
      
      IMPORT_C void ConstructL();        
       
      IMPORT_C COMASuplResponse();	
    
    private:  
	
	 /**
        * Method to clone the SlpAddres if present
        * @since S60 S60_3.1u
        * @param aSuplResponse
        * @return Returns None
        */
    void CloneSLPAddressL(const COMASuplResponse* aSuplResponse);
      

    protected:    // Data
    /*Pos Method*/
	TOMASuplPosMethod iPosMethod;
	
	/* Flag to indicate whether PosMethod is set or not*/
	TBool iPosMethodSet;
	
	/* SLP Address*/
    COMASuplSLPAddress* iSLPAddress;
    
    };


#endif //C_COMASUPLRESPONSE_H