/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  base class for all the supl messages
*
*/

#ifndef C_COMASUPLASNMESSAGEBASE_H
#define C_COMASUPLASNMESSAGEBASE_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>

//Forward Declarartion
class COMASuplSessionID;

/**
 *  Data Type Class for holding the version of supl messages
 *  Holds the minor, major and servind fields of version
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 Series60_3.1U
 */
	
class TOMASuplVersion
{
public: 
        /**
        * Performs construction of an object
        * @since Series 60 3.1U
        * @param None
        * @return Returns instance of TSuplVersion
        */

     	IMPORT_C TOMASuplVersion();

     
public:	//Getter Methods

        /**
        * Getter method for Version
        * @since Series 60 3.1U
        * @aMajor The major version number
        * @aMinor The minor version number
        * @aServInd The ServInd version number
        * @return None											 
        */														 
		IMPORT_C void SuplVersion(TInt& aMajor,
								  TInt& aMinor,
								  TInt& aServInd) const;
		
        
public:		//Setter Methods

        /**
        * Setter method for Version
        * @since Series 60 3.1U
        * @aMajor The major version number
        * @aMinor The minor version number
        * @aServInd The ServInd version number
        * @return None											 
        */														 
														 
		IMPORT_C void SetSuplVersion(TInt aMajor,
								     TInt aMinor,
								     TInt aServInd);

private :	 
		TInt iMajor;
	    TInt iMinor;
	    TInt iServInd;
};
// End of TOMASuplVersion

/**
*  Base class for all the messages. 
*  Stores information like version, session id 
*  Also provides the virtual function for encoding
*  @lib eposomasuplasnconverter.lib
*  @since Series 60 Series60_3.1U
*/


class COMASuplAsnMessageBase : public CBase
    {
    public:

        /** Type of Supl Message*/
        enum TSuplMessageType
            {
            ESUPL_INIT  = 0,
            ESUPL_START = 1,
            ESUPL_RESPONSE,
            ESUPL_POSINIT,
            ESUPL_POS,
            ESUPL_END,
            ESUPL_AUTH_REQ,
            ESUPL_AUTH_RESP,
            ESUPL_EXTENSION_MARKER2,
            ESUPL_TRIG_START,
            ESUPL_TRIG_RESPONSE,
            ESUPL_TRIG_STOP,
            ESUPL_SUPLNOTIFY,
            ESUPL_SUPLNOTIFYRESPONSE,
            ESUPL_SUPLSETINIT,
            ESUPL_REPORT
            };

    public:
    
        /*
        Destructor
        */
        IMPORT_C   virtual ~COMASuplAsnMessageBase();
                    
    public: // New functions
        /**
        * Getter Method for base members
        * @since Series 60 3.1U
        * @param aVersion Version of message
        * @param aSessionID SessionId of message
        * @return None
        */
        
		IMPORT_C void MessageBase(TOMASuplVersion& aVersion,
								  COMASuplSessionID*& aSessionID) const;
		/**
        * Getter method for SessionId
        * @since Series 60 3.1U
        * @param aSessionId Session Id
        * @return None
        */
		IMPORT_C void SessionId(COMASuplSessionID*& aSessionId) const;
		
		/**
        * Getter method for MessageType
        * @since Series 60 3.1U
        * @param None
        * @return Type of supl message
        */
		IMPORT_C TSuplMessageType MessageType() const;
		
		/**
        * Getter method for MessageLength
        * @since Series 60 3.1U
        * @param None
        * @return Length of supl message
        */
		IMPORT_C TInt MessageLength() const;
	
	  /**
        * Setter method for MessageBase
        * @since Series 60 3.1U
        * @param aVersion version of message
        * @param aSessionID session id of session
        * @return None
        */
 								  
		IMPORT_C void SetMessageBase(const TOMASuplVersion& aVersion,
								  	 COMASuplSessionID* aSessionID);		
								  	 
		IMPORT_C void SetMessageLength(TInt aMessageLength);							
		  	 
  public:
	    /**
        * Encode Method.
        * @since Series 60 3.1U
        * @param aErrorCode error during encoding
        * @return Encoded Buffer
        */
	     virtual IMPORT_C HBufC8* EncodeL(TInt& aErrorCode);
	     
	    /**
        * Virtual Method to clone message
        * @since Series 60 3.1U
        * @param aBase base message
        * @param aErrorCode error during cloning
        * @return None
        */
	   	IMPORT_C virtual void Clone(const COMASuplAsnMessageBase* /*aBase*/,TInt& /*aErrorCode*/) = 0;

	   	/**
	   	 * Method to clone base objects
	   	 * @since Series 60 3.1U
	   	 * @param aBase base pointer
	   	 * @return None
	   	 */										 
	   	IMPORT_C void CloneBaseL(const COMASuplAsnMessageBase* aBase,TInt& aErrorCode);

		 		 
    protected:
       /**
        * Setter method for MessageType
        * @since Series 60 3.1U
        * @param aMessageType type of supl message
        * @return None
        */
		  
  	  IMPORT_C void SetMessageType(TSuplMessageType aMessageType);

    protected:
    //Session ID
	COMASuplSessionID* iSessionId; 
	
    //Mask for optional parameters
	TInt  iOptionalMask; 

	// Message version
	TOMASuplVersion iVersion;

	//Temporary session ID used during encoding
	COMASuplSessionID* iTmpSessionId;

	// Type of Supl Message
	TSuplMessageType iMessageType;

	// Provides length of entire message after decoding
	TInt iMessageLength;
    };
#endif      // C_COMASUPLASNMESSAGEBASE_H
            
// End of File
