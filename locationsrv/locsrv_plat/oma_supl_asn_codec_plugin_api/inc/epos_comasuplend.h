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
* Description:  Header file for supl end message
*
*/

#ifndef C_COMASUPLEND_H
#define C_COMASUPLEND_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>

#include "epos_comasuplasnmessagebase.h"
#include "epos_suplconstants.h"

//Forward declaration
class COMASuplPosition;
class TOMASuplPositionEstimate;
class COMASuplHorizVelocity;
class TOMASuplUtcTime;

/**
 *  Class for providing the suplend message
 *  For holding the Supl End message and for encoding the supl end message 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */

class COMASuplEnd  : public COMASuplAsnMessageBase
 {	
  public:
  /** Enum for status of Position*/ 
   
   enum TOMASuplStatusCode
   	{
   		EUnspecified 	 	      = 0,
   		ESystemFailure  	      ,
   		EUnexpectedMessage	      ,
   		EProtocolError		      ,
   		EDataMissing 		 	  ,
   		EUnexpectedDataValue 	  ,
   		EPosMethodFailure    	  ,
  		EPosMethodMismatch	  	  ,
  		EPosProtocolMismatch      ,
  		ETargetSETnotReachable    ,
  		EVersionNotSupported      ,
  		EResourceShortage	      , 
  		EInvalidSessionId         ,
  		ENonProxyModeNotSupported ,
  		EProxyModeNotSupported    ,
  		EPositioningNotPermitted  ,
  		EAuthNetFailure           ,
  		EAuthSuplInitFailure      ,
        EVer2IncompatibleProtectionLevel = 18,
        EVer2ServiceNotSupported = 19,
        
  		EConsentDeniedByUser      = 100,
  		EConsentGrantedByUser = 101,
  		EVer2SessionStopped = 102
   	};
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplEnd pointer to SuplEnd Message
        */         
    
	    IMPORT_C static COMASuplEnd* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */    
     	IMPORT_C virtual ~COMASuplEnd();
          
     	    
	public:		
		/**
        * Getter method for Position
        * @since S60 S60_3.1u
        * @param Position a reference to the position object 
        *                  present in the SUPL End message is
        *                  returned in this. Ownerships stays
        *                  with the SUPL End message.
        * @return Returns ErrorCode if parameter not set
        */
	 	IMPORT_C TInt Position(COMASuplPosition*& aPosition)const;
	 	
	 	/**
        * Setter method for Position
        * @since S60 S60_3.1u
        * @param Position position to be set into the SUPL End message
        *                  ownership is transferred to the SUPL End message.
        * @return Returns None
        */
	 	IMPORT_C void SetPosition(COMASuplPosition* aPosition);
        
        /**
        * Getter method for StatusCode of position
        * @since S60 S60_3.1u
        * @param StatusCode of position
        * @return Returns ErrorCode if parameter not set
        */
	 	IMPORT_C TInt StatusCode(TOMASuplStatusCode& aStatusCode)const;
	 	
	 	/**
        * Setter method for StatusCode of position
        * @since S60 S60_3.1u
        * @param StatusCode of position to be set
        * @return Returns None
        */
	 	IMPORT_C void SetStatusCode(TOMASuplStatusCode aStatusCode);
	 	
	 	/**
        * Getter method for ver
        * @since S60 S60_3.1u
        * @param aVer 
        * @return Returns ErrorCode if parameter not set
        */
	 	IMPORT_C TInt Ver(TDes8& aVer)const;
	 	
	 	/**
        * Setter method for ver
        * @since S60 S60_3.1u
        * @param ver
        * @return Returns None
        */
	 	IMPORT_C void SetVer(TDesC8& aVer);
	 	
	 	public: // Functions from base classes
	     
	   /**
        * Function for cloning the Supl End Message
        * @since S60 S60_3.1u
        * @param aEnd end message to be cloned
        * @aErrorCode error code
        * @return None
        */
	    
	    IMPORT_C void Clone(const COMASuplAsnMessageBase* aEnd,TInt& aErrorCode);
	    
 protected:    
     
      IMPORT_C void ConstructL();        
   	  
   	 IMPORT_C  COMASuplEnd();
      
 protected:    // Data
    
    //ver field of Supl_end
    TBuf8<KVer> iVer;
    
    //Position Information
    COMASuplPosition* iPosition;
    
    //Status Code for position
    TOMASuplStatusCode iStatusCode; 
    
     };
#endif
