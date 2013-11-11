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
* Description:  Header file for supl Pos init message
*
*/

#ifndef C_COMASUPLPOSINIT_H
#define C_COMASUPLPOSINIT_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_suplconstants.h"
#include "epos_tomasuplecellinfo.h"

//Forward Declaration
class COMASuplReqAsstData;
class COMASuplPosition;
class TOMASuplPositionEstimate;
class COMASuplVelocity;
class COMASuplPosPayload;
class COMASuplHorizVelocity;
class COMASuplSETCapabilities;
class TOMASuplUtcTime;
class COMASuplLocationId;

/**
 *  Class for providing the suplposinit message
 *  For holding the Supl Pos Init message and encoding it
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */
class COMASuplPosInit  : public COMASuplAsnMessageBase
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplPosInit pointer to SuplPosInit Message
        */         
    
	    IMPORT_C static COMASuplPosInit* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
     	IMPORT_C virtual ~COMASuplPosInit();

   	public:    
		/**
        * Getter method for RequestAsstData
        * @since S60 S60_3.1u
        * @param aReqAsstData RequestAsstData
        * @return ErrorCode if param not set
        */
	 	IMPORT_C TInt RequestAssistanceData(COMASuplReqAsstData*& aReqAsstData)const;
        
		/**
        * Getter method for Position
        * @since S60 S60_3.1u
        * @param aPosition Position
        * @return Returns ErrorCode if parameter not set
        */
	 	IMPORT_C TInt Position(COMASuplPosition*& aPosition)const;
       
	 	/**
        * Getter method for ver
        * @since S60 S60_3.1u
        * @param aVer ver
        * @return Returns ErrorCode if parameter not set
        */
	 	IMPORT_C TInt Ver(TDes8& aVer)const;
	 	
		/**
        * Getter method for PosPayload in PosInit
        * @since S60 S60_3.1u
        * @param aPosPayload Pos Payload
        * @return Returns ErrorCode if parameter not set
        */
	 	IMPORT_C TInt SuplPosPayload(COMASuplPosPayload*& aPosPayload)const;
	 	
	 	/**
        * Getter method for Velocity
        * @since S60 S60_3.1u
        * @param aVelocity COMASuplVelocity
        * @return Error Code is param not set
        */
	 	IMPORT_C TInt Velocity(COMASuplVelocity*& aVelocity)const;
	 	
      	/**
        * Setter method for E-Cellid in Supl start
        * @since S60 S60_3.2
        * @param aECId,Enhance Cell id
        * @return None
        */
		IMPORT_C void SetECellId(TOMASuplECellInfo& aECId);

     
	public:		//Setter Methods

         /**
        * Setter method for Supl PosInit
        * @since S60 S60_3.1u
        * @param aSuplSETCaps SetCapabilities
        * @aSuplLocationId Location Id
        * @return Returns None
        */
        
        IMPORT_C void SetSuplPosInit(COMASuplSETCapabilities* aSuplSETCaps,
						  			 COMASuplLocationId* aSuplLocationId,
						  			 TBool aeCID);
        /**
        * Setter method for Supl request asst data
        * @since S60 S60_3.1u
        * @param aReqAsstData RequestAsstData- ownership of request asst data passed to COMASuplPosInit
        * @return Returns None
        */
		
		IMPORT_C void SetRequestAssistanceData(COMASuplReqAsstData* aReqAsstData);
	     
	   /**
        * Setter method for ver
        * @since S60 S60_3.1u
        * @param aVer ver
        * @return Returns None
        */
	 	IMPORT_C void SetVer(TDesC8& aVer);
	 		 	
	 	/**
        * Setter method for Position
        * @since S60 S60_3.1u
        * @param aPosition Position- ownership of position passed to COMASuplPosInit
        * @return Returns None
        */
	 	IMPORT_C void SetPosition(COMASuplPosition* aPosition);
	 	
	 	/**
        * Setter method for PosPayload in PosInit
        * @since S60 S60_3.1u
        * @param aPosPayload PosPayload - ownership of payload passed to COMASuplPosInit
        * @return Returns None
        */
		
		IMPORT_C void SetSuplPosPayload(COMASuplPosPayload* aPosPayload);
		
		/**
        * Setter method for Velocity
        * @since S60 S60_3.1u
        * @param aVelocity COMASuplVelocity- ownership of Velocity passed to COMASuplPosInit
        * @return None
        */
	 	IMPORT_C void SetVelocity(COMASuplVelocity* aVelocity); 
	 	
	public: // Functions from base classes
	    /**
	     * Cloning of SuplPosInit
	     * @since S60 S60_3.1u
	     * @param aPosInit Base pointer to be cloned
	     * @param aErrorCode error code
	     * @return Returns None
	     */
	    IMPORT_C void Clone(const COMASuplAsnMessageBase* aPosInit,TInt& aErrorCode);

	protected:    

	    IMPORT_C void ConstructL();        

	    IMPORT_C COMASuplPosInit();  

	protected:    // Data

	    /* SetCapabilities*/
	    COMASuplSETCapabilities* iSETCapabilities; 

	    /* Location Id*/
	    COMASuplLocationId* iLocationId;

	    /*Request Asst Data*/
	    COMASuplReqAsstData* iReqAsstData;

	    /*ver field of Supl_posint*/
	    TBuf8<KVer> iVer;

	    /*Position Information*/
	    COMASuplPosition* iPosition;

	    /* PosPayload*/
	    COMASuplPosPayload* iPosPayload;

	    /* Velcoity Information*/
	    COMASuplVelocity* iVelocity;

	    /* Enhanced Cell Id*/
	    TBool   ieCID;

	    /*To hold E-cell id data*/
	    TOMASuplECellInfo iMmCellInfo;

   };


#endif //C_COMASUPLPOSINIT_H
