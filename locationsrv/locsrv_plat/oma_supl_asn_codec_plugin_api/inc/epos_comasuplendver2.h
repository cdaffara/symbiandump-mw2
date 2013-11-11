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
* Description:  Header file for supl 2.0 end message
*
*/

#ifndef C_COMASUPLENDVER2_H
#define C_COMASUPLENDVER2_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>

#include "epos_comasuplend.h"
#include "epos_suplconstants.h"

//Forward declarations

struct t_SETCapabilities;
typedef struct t_SETCapabilities SETCapabilities;

class COMASuplTrace;
class COMASuplSETCapabilities;

/**
 *  Class for providing the suplend message for SUPL 2.0
 *  For holding the Supl End message and for encoding the supl end message 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */

class COMASuplEndVer2  : public COMASuplEnd
 {	
   
	public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_5.2
        * @param None
        * @return COMASuplEnd pointer to SuplEnd Message
        */         
    
	    IMPORT_C static COMASuplEndVer2* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_5.2
        * @param None
        * @return None
        */    
     	IMPORT_C virtual ~COMASuplEndVer2();
          
     	    
	public:		
	    /**
        * Getter method for SET Capabilities
        * @since S60 S60_5.2
        * @param aSETCapabilities A reference to the setcaps present 
        *                         in the SUPL End object is returned in this.
        *                         Ownership stays with the SUPL End object.
        * @return Returns ErrorCode if parameter not set
        */
	 	IMPORT_C TInt SETCapabilities(COMASuplSETCapabilities *& aSETCapabilities)const;
	 	
	 	/**
        * Setter method for SET Capabilities
        * @since S60 S60_5.2
        * @param aSETCapabilities Set Caps to be set in the SUPL End Object.
        *                         Ownership is transferred to the SUPL End object.
        * @return Returns None
        */
	 	IMPORT_C void SetSETCapabilities(COMASuplSETCapabilities* aSETCapabilities);
        
	 	public: // Functions from base classes
	     
	    /**
        * Function for cloning the Supl End Message
        * @since S60 S60_5.2
        * @param aEnd A pointer to the SUPL End object to be cloned
        * @aErrorCode error code
        * @return None
        */
	    
	    IMPORT_C void Clone(const COMASuplAsnMessageBase* aEnd,TInt& aErrorCode);
	    
 protected:    
     
      IMPORT_C void ConstructL();        
   	  
   	 IMPORT_C  COMASuplEndVer2();
      
 protected:    // Data

		COMASuplSETCapabilities* iSETCapabilities;    

    
     };
#endif
