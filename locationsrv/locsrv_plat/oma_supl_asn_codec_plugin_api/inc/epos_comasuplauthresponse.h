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

#ifndef C_COMASUPLAUTHRESPONSE_H
#define C_COMASUPLAUTHRESPONSE_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"



/**
 *  Class for providing the suplauthresponse message
 *  For holding the Supl Response message
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1
 */

class COMASuplAuthResponse  : public COMASuplAsnMessageBase
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_3.1
        * @param None
        * @return COMASuplAuthResponse pointer to SuplResponse Message
        */         
    
	    IMPORT_C static COMASuplAuthResponse* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_3.1
        * @param None
        * @return None
        */ 
     	IMPORT_C ~COMASuplAuthResponse();

	public:
	
		/**
        * Cloneing of SuplAuthResponse
        * @since S60 S60_3.1
        * @param aResponse Base pointer
        * @param aErrorCode error code
        * @return Returns None
        */
		IMPORT_C void Clone(const COMASuplAsnMessageBase* aResponse,TInt& aErrorCode);
		
	public:		    // Setters method
		
  		/**
        * SetKeyIdentity3
        * @since S60 3.1
        * @param aSETNounce Key Identity
        * @return Returns KErrArgument if length of supplied buffer is greter then allocated
        *         else KErrNone   
        */
		IMPORT_C  TInt SetKeyIdentity3(TDesC8& aKeyIdentity);
		
		/**
        * SetLongSPCKey
        * @since S60 3.1
        * @param aSPCLongKey SPC Key
        * @return Returns KErrArgument if length of supplied buffer is greter then allocated
        *         else KErrNone   
        */
		IMPORT_C  TInt SetLongSPCKey(TDesC8& aSPCLongKey);

		/**
        * SetShortSPCKey
        * @since S60 3.1
        * @param aSPCShortKey SPC Key
        * @return Returns KErrArgument if length of supplied buffer is greter then allocated
        *         else KErrNone   
        */
		IMPORT_C  TInt SetShortSPCKey(TDesC8& aSPCShortKey);
		
		IMPORT_C TInt KeyIdentity3(TDes8& aKeyIdentity3) const;
		IMPORT_C TInt ShortSPCKey(TDes8& aSPCShortKey) const;
		IMPORT_C TInt LongSPCKey(TDes8& aSPCLongKey ) const;

   protected:   
      
	      IMPORT_C void ConstructL();        
	       
	      IMPORT_C COMASuplAuthResponse();	
	

   protected:    // Data

		//For holding SPC AuthKey
		RBuf8 iSPCAuthKey;  //0  for short key .... 1 for long key
		
    	//For holding Key Identity
    	RBuf8 iKeyIdentity3; //2 for KeyIdentity
    
    };


#endif //C_COMASUPLAUTHRESPONSE_H
