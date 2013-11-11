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
* Description:  Header file for supl 2.0 triggered response message
*
*/
#ifndef EPOS_COMASUPLTRIGGEREDRESPONSE_H_
#define EPOS_COMASUPLTRIGGEREDRESPONSE_H_

#include <e32def.h>
#include <e32base.h>
#include "epos_suplconstants.h"
#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplqop.h"
#include "epos_tomasuplecellinfo.h"
#include "lbs/epos_comasuplposition.h"
#include "epos_comasuplstartver2.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "epos_tomasuplposmethod.h"
#include "epos_comasupltriggeredstart.h"
#include "epos_comasuplresponsever2.h"

class COMASuplSLPAddress;

/**
 * Class to hold Batch report Type
 */
class TOMASuplBatchRepType
    {
public:
    
    /**
     * Getter method for batch rep type parameters
     */
    IMPORT_C void GetBatchRepType(TBool& aRepPosition,TBool& aRepMeas,TBool& aInterMed,TBool& aDiscardOld);
public:

    /**
     * Setter method to set value of report position
     */
    IMPORT_C void SetRepPos(TBool aRepPos);

    /**
     * Setter method to set value of report measurements
     */
    IMPORT_C void SetRepMeas(TBool aRepMeas);
    /**
     * Setter method to set value of intermediate reports
     */
    IMPORT_C void SetInterRep(TBool aInterMedRep);
    /**
     * Setter method to set value of discard oldest
     */ 
    IMPORT_C void SetDiscOld(TBool aDisOld);
       
private:
    TBool iReportPosition;
    TBool iReportMeasurements;
    TBool iIntermediateReports;
    TBool iDiscardOldest;

    };

/**
 * Class to hold batch report conditions
 */
class TOMASuplBatchRepConditions
    {
public:
    /**
     * Enum to indicate batch report condition type
     */
    enum TOMABatchRepCondType
        {
        ETOMASuplBatchRepInterval,
        ETOMASuplBatchRepMins
        };

public:
    /**
     * getter method for batch report type
     */
    IMPORT_C TOMABatchRepCondType GetBatchRepType();
     
    /**
     * getter method for number of intervals
     */
    IMPORT_C TInt NumOfIntervals();
    /**
     * getter method for num of minutes
     */
    IMPORT_C TInt NumOfMins();
   
    /**
     * setter method for batch report type
     */
    IMPORT_C void SetBatchRepType(TOMABatchRepCondType aType);
    /**
     * setter method for number of intervals
     */
    IMPORT_C void SetNumOfIntervals(TInt aNoOfIntervals);
    
    /**
     * setter method for number of mins
     */
    IMPORT_C void SetNumOfMins(TInt aNoOfMins);


private:
    TOMABatchRepCondType iBatchRepType;
    TInt iNumOfIntervals;
    TInt iNumOfMins;
    };

/**
 * Class to hold reporting mode
 */
class TOMASuplReportingMode
    {
public:
    /**
     * Enum to indicate report mode
     */
    enum TOMASuplRepMode
        {
        ETOMASuplRepModeUnknown = -1,
        ETOMASuplRepModeRealtime = 1,
        ETOMASuplRepModeQuasirealtime = 2,
        ETOMASuplRepModeBatch = 3, 
        };
public:
    /**
     * getter method for report mode
     */
    IMPORT_C void GetRepMode(TOMASuplRepMode& aRepMode);
    /**
     * getter method for batch report type
     */
    IMPORT_C void GetBatchRepType(TOMASuplBatchRepType& aBatchRepType);
    
    /**
     * getter method for batch report conditions
     */
    IMPORT_C void GetBatchRepCond(TOMASuplBatchRepConditions& aBatchRepCond);

public:
    
    /**
     * Setter method for report mode
     */
    IMPORT_C void SetRepMode(TOMASuplRepMode aRepMode);

    /**
     * setter method for batch report type
     */
    IMPORT_C void SetBatchRepType(TOMASuplBatchRepType aBatchRepType);
    
    /**
     * setter method for batch report conditions
     */
    IMPORT_C void SetBatchRepCond(TOMASuplBatchRepConditions aBatchRepCond);
    
private:
    TOMASuplBatchRepConditions iRepConditions;
    TOMASuplBatchRepType iBatchRepType;
    TOMASuplRepMode iRepMode;
};

/**
 *  Class for providing the Triggered Reponse message for SUPL v2.0
 *
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */
class COMASuplTriggeredResponse:public COMASuplAsnMessageBase
     {
 
     public:  // Constructors and destructor
    
         /**
          * Two phased constructor
          * @since S60 S60_5.2
          * @param None
          * @return COMASupltriggeredresponse pointer 
          */         
    
         IMPORT_C static COMASuplTriggeredResponse* NewL();
    
         /**
          * Destructor
          * @since S60 S60_5.2
          * @param None
          * @return None
          */  
         IMPORT_C ~COMASuplTriggeredResponse();
             
     public: // Functions from base classes

         /**
          * Cloning of SuplTrigStart
          * @since S60 S60_5.2
          * @param aTrigResponse pointer to the Trigger Response message to be cloned
          * @param aErrorCode error code
          * @return Returns None
          */
         IMPORT_C void Clone(const COMASuplAsnMessageBase* aTrigResponse,TInt& aErrorCode);
         
     protected: 

         IMPORT_C void ConstructL();        

         IMPORT_C COMASuplTriggeredResponse(); 
         
         static COMASuplTriggeredResponse* NewLC();
         

     public:
         /**
          * getter method for pos method
          * @since
          * @param aPosMethod posmethod value is returned in this
          * @return None
          */
         IMPORT_C void GetPosMethod(TOMASuplPosMethod& aPosMethod) const;
         /**
          * getter method for trigger params
          * @since
          * @param aTriggerParams trigger params is returned in this
          * @return KErrNone if the value is set and retrieved successfully
          *         KErrOMASuplParamNotSet Otherwise
          */
         
         IMPORT_C TInt GetTriggerParams(TOMASuplTriggerParams& aTriggerParams)const;
         /**
          * getter method for SLP Address
          * @since
          * @param aAddress a reference to the slp address memeber  is returned in this
          *         ownership stays witht the triggre response class
          * @return KErrNone if the value is set and retrieved successfully
          *         KErrOMASuplParamNotSet Otherwise
          */
         IMPORT_C TInt GetSLPAddress(COMASuplSLPAddress*& aAddress)const;
         /**
          * getter method for report mode
          * @since
          * @param aReportMode report mode value is returned in this
          * @return KErrNone if the value is set and retrieved successfully
          *         KErrOMASuplParamNotSet Otherwise
          */
         IMPORT_C TInt GetReportMode(TOMASuplReportingMode& aReportMode)const;
         /**
          * getter method for supported network info
          * @since
          * @param aSupNwInfo supported network info is returned in this
          * @return KErrNone if the value is set and retrieved successfully
          *         KErrOMASuplParamNotSet Otherwise
          */
         IMPORT_C TInt GetSupNetworkInfo(TOMASuplSuppNetworkInformation& aSupNwInfo)const;
         /**
          * getter method for SPCSET Key
          * @since
          * @param aSupSPCSETKey SPCSET Key is returned in this
          * @return KErrNone if the value is set and retrieved successfully
          *         KErrOMASuplParamNotSet Otherwise
          */
         IMPORT_C TInt GetSPCSETKey(TOMASuplSPCSETKey& aSupSPCSETKey)const;
         /**
          * getter method for SPCT Id
          * @since
          * @param aSupSPCTId a reference to the SPCT Id is returned in this
          *     ownership stays with the supl response object
          *     @return None
          */
         IMPORT_C TInt GetSPCTId(COMASuplSPCTID*& aSupSPCTId)const;
         /**
          * getter method for trigger params
          * @since
          * @param aTriggerParams trigger params is returned in this
          * @return KErrNone if the value is set and retrieved successfully
          *         KErrOMASuplParamNotSet Otherwise
          */
         IMPORT_C TInt GetSPCTKeyLifeTime(TInt& aKeyLifeTime)const;

     public: //setter methods
         /**
          * Setter method for pos method
          * @since
          * @param aPosMethod Pos Method value to be set in trigger response
          */
         IMPORT_C void SetPosMethod(TOMASuplPosMethod aPosMethod);
         /**
          * Setter method for trigger params
          * @since
          * @param aTriggerParams trigger params value to be set in trigger response
          */
         IMPORT_C void SetTriggerParams(TOMASuplTriggerParams aTriggerParams);
         /**
          * Setter method for SLP Address
          * @since S60 S60_5.2
          * @param aAddress SLP Address to be set in trigger response
          *         ownership of the object is transferred to the trigger response object
          */
         IMPORT_C void SetSLPAddress(COMASuplSLPAddress* aAddress);
         /**
          * Setter method for supported network info
          * @since
          * @param aSupNwInfo supported network info value to be set in trigger response
          */
         IMPORT_C void SetSupNetworkInfo(TOMASuplSuppNetworkInformation aSupNwInfo);
         /**
          * Setter method for report mode
          * @since
          * @param aReportMode report mode value to be set in trigger response
          */
         IMPORT_C void SetReportMode(TOMASuplReportingMode aReportMode);
         /**
          * Setter method for SPC SET Key
          * @since
          * @param aSupSPCSETKey SPC SET Key value to be set in trigger response
          */
         IMPORT_C void SetSPCSETKey(TOMASuplSPCSETKey aSupSPCSETKey);
         /**
          * Setter method for SPCT Id
          *@since S60 S60_5.2
          * @param aSupSPCTId SPCT Id to be set in trigger response
          *         ownership of the object is transferred to the trigger response object
          */
         IMPORT_C void SetSPCTId(COMASuplSPCTID* aSupSPCTId);
         /**
          * Setter method for SPCT Key Lifetime
          *@since S60 S60_5.2
          * @param aKeyLifeTime SPCT Key Lifetime value to be set in trigger response
          */
         IMPORT_C void SetSPCTKeyLifeTime(TInt aKeyLifeTime);
              
     protected: 
         
         //Pos Method
         TOMASuplPosMethod iPosMethod;
         TOMASuplTriggerParams      iTriggerParams;
         COMASuplSLPAddress*    iSuplSLPAddress;
         
         TOMASuplSuppNetworkInformation iSupportedNetworkInformation;
         
         TOMASuplReportingMode iReportMode;
         TOMASuplSPCSETKey iSPCSetKey;
         COMASuplSPCTID* iSPCTID;
         TInt iSPCTKeyLifeTime;
         };

#endif /*EPOS_COMASUPLTRIGGEREDSTART_H_*/
