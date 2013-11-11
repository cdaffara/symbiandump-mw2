/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides the interface between SUPL protocol handler and 
				 ASN Plugin 
*
*/

#ifndef C_COMASUPLASNBASE_H
#define C_COMASUPLASNBASE_H


#include <e32base.h>

#include "epos_comasuplasnmessagebase.h"

/**
 * Constant:    KAsnHandlerInterfaceDefinitionUid
 *
 * Description: UID of this ECOM interface. 
 */

 const TUid KAsnHandlerInterfaceDefinitionUid = {0x20026773};

//Forword Declarations

/**
 * Class:       COMASuplAsnHandlerBase
 *
 * Description: Custom ECOM interface definition. This interface is used by
 *              clients to find specific instance and implementation of 
 *				ASN Message Plugin.
 */
 
 
class COMASuplAsnHandlerBase : public CBase
    {

public: // Constructors and destructor
    /**
     * Function:   NewL, Two Phase Construction
     *
     * @Description: Wraps ECom object instantitation. Will search for
     *              interface implementation, which matches to given
     *              UID.
     *
     * @Param:      aOMASuplVersion, A protocol version of SUPL 
     * @return 		instance of implementation		
     *
     */
 IMPORT_C   static COMASuplAsnHandlerBase* NewL(const TInt& aOMASuplVersion);
 
    /**
     * Function:     EncodeL, 
     *
     * @Description: Encodes the SUPL message.  The parameters to this methos 
     * is the SUPL message to be encoded and result code to return result 
     * of encoding.
     *
     * @Param:       aMessage, Message to be encoded 
     * @Param:       aErrorCode, On completion contains the result of encoding 
     *
     * @Returns:     Encoded message if successful.  Else contains NULL.
     */
 		
  virtual HBufC8* EncodeL(COMASuplAsnMessageBase*, TInt& aErrorCode) = 0;

    /**
     * Function:     DecodeL, 
     *
     * @Description: Decodes SUPL message.  The parameters to decode message are
     * encoded message, error code to be returned and the number of bytes to be
     * decoded. 
     *
     * @Param:       aPacket , Encoded SUPL message 
     * @Param:       aErrorCode , Upon completion contains the result of decoding 
     * @Param:       aUnConsumedBytes , Unconsumed bytes in the encoded message 
     *
     * @Returns:     Pointer to decoded SUPL message
     */
  virtual COMASuplAsnMessageBase* DecodeL(const TPtrC8& aPacket,TInt& aErrorCode,TInt& aUnConsumedBytes ) = 0;
  
    /**
     * Function:     CreateOMASuplMessageL, 
     *
     * @Description: Creates SUPL message.  The parameter to create SUPL message
     * is the type of SUPL message to be created.  After creating SUPL message,
     * ownership of the created message is transferred to client.
     *
     * @Param:       aOMASuplMessageType, SUPL Message type to be created 
     *
     * @Returns:     Pointer to SUPL message
     */
  virtual COMASuplAsnMessageBase* CreateOMASuplMessageL(COMASuplAsnMessageBase::TSuplMessageType aOMASuplMessageType) = 0;

 		/**
     * Function:     GetSuplMessageVersionL, 
     *
     * @Description: Used to retrieve the version of SUPL message.
     *
     * @Param:       aErrorCode, Place holder for error code  
     *
     * @Param:       aSuplAsnMessage, Encoded message for which version is to be retrieved
     *
     * @Returns:     Pointer to SUPL message
     */
	IMPORT_C virtual TOMASuplVersion GetSuplMessageVersionL(TInt& aErrorCode,const TPtrC8& aSuplAsnMessage) = 0;

    /**
     * Function:   ImplementationUid, 
     *
     * @Description: Gives implementation UID
     *
     * @Param:      None.
     * @return 		Returns implementation UID 		
     *
     */
     
 IMPORT_C   TUid ImplementationUid() const;
 

    /**
     * Function:   	 Version 
     *
     * @Description: Gives version of implementation
     *
     * @Param:       None.
     * @return 		 Returns TInt, containing version of implementation 
     *
     */
     
 IMPORT_C   TInt Version() const;

    /**
     * Function:   ~COMASuplAsnHandlerBase  
     *
     * @Description: Wraps ECom object destruction. Notifies the ECOM
     *              framework that specific instance is being destroyed.
     */
    IMPORT_C  ~COMASuplAsnHandlerBase();

public:   

    /**
     * Function:     COMASuplAsnHandlerBase 
     *
     * @Description: Default constructor
     *
     * @Param:       None
     *
     * @Returns:     None
     */
   IMPORT_C COMASuplAsnHandlerBase();
   
      /**
     * Function:     SetImplementationUid
     *
     * @Description: Sets Uid of implementation 
     *
     * @Param:       UID of implementation.
     *
     * @Returns:     None
     */

	IMPORT_C virtual void ConstructL(const TUid& aUid,TInt aSuplVersion); 
	
	

    
private:

 	/**
     *	By default, prohibit copy constructor
	 */
         
    COMASuplAsnHandlerBase( const COMASuplAsnHandlerBase& );
    
   	/**
     *	By default, Prohibit assigment operator
	 */

    COMASuplAsnHandlerBase& operator= ( const COMASuplAsnHandlerBase& );
	
	
private:

    /** iDtor_ID_Key Instance identifier key. When instance of an
     *               implementation is created by ECOM framework, the
     *               framework will assign UID for it. The UID is used in
     *               destructor to notify framework that this instance is
     *               being destroyed and resources can be released.
     */
     
    TUid iDtor_ID_Key;
    
    //Uid of Implementation
    
    TUid iImplementationUid;
    
    TInt iSuplVersion;
    
    };


#endif // C_COMASUPLASNBASE_H

// End of File
