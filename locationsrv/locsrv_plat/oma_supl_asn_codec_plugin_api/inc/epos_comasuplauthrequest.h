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
* Description:  Header file for supl auth request message
*
*/

#ifndef C_COMASUPLAUTHREQUEST_H
#define C_COMASUPLAUTHREQUEST_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"



/**
 *  Class for providing the supl auth request message
 *  For holding the Supl Request message
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 3.1
 */

class COMASuplAuthRequest  : public COMASuplAsnMessageBase
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 3.1
        * @param None
        * @return COMASuplAuthRequest pointer to SuplAuthRequest Message
        */         
    
	    IMPORT_C static COMASuplAuthRequest* NewL();
    
    	/**
        * Destructor
        * @since S60 3.1
        * @param None
        * @return None
        */ 
     	IMPORT_C ~COMASuplAuthRequest();

	 public: // Functions from base classes
	 
		/**
        * Cloning of SuplAuthRequest
        * @since S60 3.1
        * @param aResponse Base pointer
        * @param aErrorCode error code
        * @return Returns None
        */
		IMPORT_C void Clone(const COMASuplAsnMessageBase* aResponse,TInt& aErrorCode);
		
		/**
        * SetKeyIdentity
        * @since S60 3.1
        * @param aKeyIdentity Key Identity
        * @return Returns KErrArgument if length of supplied buffer is greter then allocated
        *         else KErrNone   
        */
		IMPORT_C  TInt SetKeyIdentity(TDesC8& aKeyIdentity);
		
		/**
        * SetSETNounce
        * @since S60 3.1
        * @param aSETNounce SET Nounce
        * @return Returns KErrArgument if length of supplied buffer is greter then allocated
        *         else KErrNone   
        */
		IMPORT_C  TInt SetSETNounce(TDesC8& aSETNounce);
                
  protected:    
      
	    IMPORT_C  void ConstructL();        
	       
	    IMPORT_C COMASuplAuthRequest();	

 protected:    // Data
    
    	//For holding SETNounce
    	RBuf8 iSETNounce;  
    	
    	//For holding Key Identity
    	RBuf8 iKeyIdentity; 
    
    };


#endif //C_COMASUPLAUTHREQUEST_H
