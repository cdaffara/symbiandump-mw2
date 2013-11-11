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
* Description:  Header file for supl pos message
*
*/

#ifndef C_COMASUPLPOS_H
#define C_COMASUPLPOS_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"

//Forward declaration
class COMASuplVelocity;
class COMASuplHorizVelocity;
class COMASuplPosPayload;

/**
 *  Class for providing the suplpos message
 *  For holding the Supl Pos message and for encoding the supl Pos message 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */
class COMASuplPos  : public COMASuplAsnMessageBase
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplPos pointer to SuplPos Message
        */          
            
    
	    IMPORT_C static COMASuplPos* NewL();
    
    		/**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
     	IMPORT_C ~COMASuplPos();
   
		public:
		/**
        * Getter method for Supl pos
        * @since S60 S60_3.1u
        * @param None
        * @return pointer to instance of PosPayload 
        */
	 	IMPORT_C COMASuplPosPayload* SuplPos()const;
	 	
	   /**
        * Setter method for Supl pos
        * @since S60 S60_3.1u
        * @param aPosPayload PosPayload - ownership of payload passed to COMASuplPos
        * @return Returns None
        */
		
		IMPORT_C void SetSuplPos(COMASuplPosPayload* aPosPayload);
	 	
	 	/**
        * Getter method for Velocity
        * @since S60 S60_3.1u
        * @param aVelocity COMASuplVelocity
        * @return Error Code is param not set
        */
	 	IMPORT_C TInt Velocity(COMASuplVelocity*& aVelocity)const;
     
	    /**
        * Setter method for Velocity
        * @since S60 S60_3.1u
        * @param aVelocity COMASuplVelocity- ownership of Velocity passed to COMASuplPos
        * @return None
        */
	 	IMPORT_C void SetVelocity(COMASuplVelocity* aVelocity); 	 	
	 	
	 	public: // Functions from base classes
	     /**
        * Method to clone 
        * @since S60 S60_3.1u
        * @param aErrorCode errorcode
        * @param aSuplPos suplpos pointer
        * @return None
        */
	     IMPORT_C void Clone(const COMASuplAsnMessageBase* aSuplPos,TInt& aErrorCode);
       
                  
 protected:   
      
      IMPORT_C void ConstructL();        
       
      IMPORT_C COMASuplPos();
      
 protected:    // Data
        
   /* PosPayload*/
    COMASuplPosPayload* iPayload;
    
    /* Velcoity Information*/
    COMASuplVelocity* iVelocity;
    };


#endif //C_COMASUPLPOS_H
