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
* Description:  Header file for supl Start message
*
*/

#ifndef C_COMASUPLSTART_H
#define C_COMASUPLSTART_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplqop.h"
#include "epos_tomasuplecellinfo.h"

//Forward declaration
class COMASuplSETCapabilities;
class COMASuplLocationId;

/**
 *  Class for providing the suplstart message
 *  For holding the Supl Start message and for encoding the supl start message 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */

class COMASuplStart  : public COMASuplAsnMessageBase
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplStart pointer to SuplStart Message
        */         
    
	    IMPORT_C static COMASuplStart* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */  
     	IMPORT_C ~COMASuplStart();
                
public:
		/**
        * Getter method for Qop in Supl start
        * @since S60 S60_3.1
        * @param aSuplQop Quality of position
        * @return error code in case Qop is not set
        */
		
		IMPORT_C TInt Qop(TOMASuplQop& aSuplQop)const;

        /**
        * Setter method for Supl start
        * @since S60 S60_3.1u
        * @param aSuplSETCaps SetCapabilities
        * @param aSuplLocationId Location Id
        * @return Returns None
        */
		
		IMPORT_C void SetSuplStart(COMASuplSETCapabilities* aSuplSETCaps,
						  		   COMASuplLocationId* aSuplLocationId,
						  		   TBool aeCID);
	     
      	/**
        * Setter method for Qop in Supl start
        * @since S60 S60_3.1
        * @param aSuplQop Quality of position
        * @return Returns None
        */
		
		IMPORT_C void SetQop(TOMASuplQop& aSuplQop);
		
      	/**
        * Setter method for E-Cellid in Supl start
        * @since S60 S60_3.2
        * @param aECId,Enhance Cell id
        * @return None
        */
		IMPORT_C void SetECellId(TOMASuplECellInfo& aECId);
		
   	public: // Functions from base classes
      	
      	/**
        * Cloning of SuplStart
        * @since S60 S60_3.1u
        * @param aStart Base pointer
        * @param aErrorCode error code
        * @return Returns None
        */
      	 IMPORT_C void Clone(const COMASuplAsnMessageBase* aStart,TInt& aErrorCode);
   	
 protected:    
      
     IMPORT_C void ConstructL();        
      
     IMPORT_C COMASuplStart(); 
      
    protected:    // Data
    
	/* SetCapabilities*/
    COMASuplSETCapabilities* iSETCapabilities; 
    
    /* Location Id*/
    COMASuplLocationId* iLocationId;
    
    /* Enhanced Cell Id*/
    TBool	ieCID;
    
	//To hold E-cell id data
	TOMASuplECellInfo iMmCellInfo;
    
    //Quality of position
    TOMASuplQop iSuplQop;
    
    //Boolean to indicate Qop is set or not
    TBool iQopSet;

    };


#endif //C_COMASUPLSTART_H
