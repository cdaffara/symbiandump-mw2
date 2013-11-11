/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class declaration of Cell data handler 
*
*/


#ifndef C_LBTCELLOPERATION_H
#define C_LBTCELLOPERATION_H

// INCLUDES
#include <etelmm.h>
#include <rmmcustomapi.h>
#include "lbtoperation.h"


/**
 * Cell id handler. It takes care of issuing requests to Etel for getting
 * notified about cell changes
 */
class CLbtCellOperation : public CLBTOperation
    {
public:
    /**
     * Instantiation method.
     * 
     * @param aMPhone handle to Etel API
     * @param aObserver reference to observer
     * @return pointer to new instance
     */
     static CLbtCellOperation* NewL( TRequestStatus& aStatus, CLbtGeoAreaBase ** aArea );
    
        
     /**
      * Default destructor
      */
     virtual ~CLbtCellOperation();
       
     /**
      * Gets the current cell information required for creating cell based trigger      
      */
     void GetCurrentCellInformation(  ); 
     
     /**
      * Constructor.
      * 
      */
     CLbtCellOperation(TRequestStatus& aStatus, CLbtGeoAreaBase ** aArea);

protected:
     void RunL();
     void DoCancel();    		
           
private:
     // functions
        
     /**
      * Initialises Etel.
      */
     void InitialiseEtelL();

     /**
      * Second level constructor
      */
     void ConstructL(  );
        
private:
         
     RMobilePhone::TMobilePhoneNetworkMode          iNetworkMode;   
     RTelServer 			                        iTelServer;
     RMobilePhone 								    iMPhone; ///< etel api
     RMmCustomAPI                                   iMmApi; 
     TInt 										    iCommandId; ///< command id
     RMobilePhone::TMobilePhoneNetworkInfoV1		iNwInfo; ///< network info
     RMobilePhone::TMobilePhoneNetworkInfoV1Pckg	iNwInfoPckg; ///< package
     RMobilePhone::TMobilePhoneLocationAreaV1	    iLocArea; ///< location info
            
     RMmCustomAPI::TMmCellInfo                      iCellInfo;  ///< ecid info  
     RMmCustomAPI::TMmCellInfoPckg                  iCellPckg;  ///< package for cell info  
     TRequestStatus&                                iClientStatus;    
     CLbtGeoAreaBase** 								iArea;
    };

#endif // C_LBTCELLOPERATION_H

// End of File
