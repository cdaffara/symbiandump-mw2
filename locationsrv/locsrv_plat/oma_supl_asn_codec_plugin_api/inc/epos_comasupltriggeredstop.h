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
* Description:  Header file for supl 2.0 triggered stop message
*
*/
#ifndef EPOS_COMASUPLTRIGGEREDSTOP_H_
#define EPOS_COMASUPLTRIGGEREDSTOP_H_

#include <e32def.h>
#include <e32base.h>
#include "epos_suplconstants.h"
#include "epos_comasuplasnmessagebase.h"
#include "epos_comasuplend.h"

/**
  *  Class for providing the Triggered Stop message for SUPL v2.0
  *
  *  @lib eposomasuplasnconverter.lib
  *  @since S60 S60_5.2
  */
 class COMASuplTriggeredStop:public COMASuplAsnMessageBase
     {
 

     public:  // Constructors and destructor
    
         /**
          * Two phased constructor
          *@since S60 S60_5.2
          * @param None
          * @return COMASuplTriggeredStop pointer 
          */         
    
         IMPORT_C static COMASuplTriggeredStop* NewL();
    
         /**
          * Destructor
          * @since S60 S60_5.2
          * @param None
          * @return None
          */  
         IMPORT_C ~COMASuplTriggeredStop();
             
     public: // Functions from base classes

         /**
          * Cloning of COMASuplTriggeredStop
          *@since S60 S60_5.2 
          * @param aTrigStop Pointer to triggered stop object to be cloned
          * @param aErrorCode error code
          * @return Returns None
          */
         IMPORT_C void Clone(const COMASuplAsnMessageBase* aTrigStop,TInt& aErrorCode);

         /**
          * Getter method for SUPL Status code 
          */
         IMPORT_C TInt GetSuplStatusCode(COMASuplEnd::TOMASuplStatusCode& aStatusCode) const;
         
         /**
          * Setter method for SUPL Status code
          */
         IMPORT_C void SetSuplStatusCode(const COMASuplEnd::TOMASuplStatusCode aStatusCode) ;
     protected: 

         IMPORT_C void ConstructL();        

         IMPORT_C COMASuplTriggeredStop(); 
         
     protected: 
         //Status Code for position
         COMASuplEnd::TOMASuplStatusCode iStatusCode; 
         
     };

#endif /*EPOS_COMASUPLTRIGGEREDSTOP_H_*/
