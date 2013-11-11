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
* Description:  Header file for supl 2.0 triggred start message
*
*/
#ifndef EPOS_COMASUPLTRIGGEREDSTART_H_
#define EPOS_COMASUPLTRIGGEREDSTART_H_

#include <e32def.h>
#include <e32base.h>
#include "epos_suplconstants.h"
#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplqop.h"
#include "epos_tomasuplecellinfo.h"
#include "lbs/epos_comasuplposition.h"
#include "epos_comasuplstartver2.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "epos_comasuplsetcapabilitiesver2.h"

class COMASuplSETCapabilitiesVer2;
class COMASuplLocationIdVer2;

/**
 * Class to hold SUPL Periodic Parameters
 */
class  TOMASuplPeriodicParams
    {
public:
    /**
     * Constructor for TOMASuplPeriodicParams
     */
    IMPORT_C TOMASuplPeriodicParams();
    
    /**
     * Getter method for TOMASuplPeriodicParams mandatory parameters
     *@since S60 S60_5.2
     * @param aNoOfFixes Number of fixes is returned in this
     * @param aIntervalBetFixes Interval between fixes is returned in this
     * @return none
     */
    IMPORT_C void GetSuplPeriodicParams(TInt32& aNoOfFixes,TInt32& aIntervalBetFixes)const;
    /**
     * Getter method for Start time
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet otherwise
     */
    IMPORT_C TInt GetStartTimeSet(TInt32& aStartTime)const;
public:
    /**
     * Setter method for number of fixes and interval between fixes
     */
    IMPORT_C void SetSuplPeriodicParams(const TInt32 aNoOfFixes,const TInt32 aIntervalBetFixes);
    /**
     * Setter method for start time value
     */
    IMPORT_C void SetStartTimeSet(TInt32 aStartTime);
private:
    TInt32 numberOfFixes;
    TInt32 intervalBetweenFixes;
    TInt32 startTime;
    TBool  iIsStartTimeSet;
    //EXTENSIONMARKER extensionmarker;
    };

 struct TOMASuplAreaEventParams 
     {
     
     };

 /**
  * 
  * Class to hold the trigger parameters for Area event or periodic triggerring
  */
 class TOMASuplTriggerParams 
     {
     
 public:
     /**
      * Enum  to indicate the type of trigger parameter present in this class 
      */
     enum TOMASuplTriggerParamsType
         {
         ETOMASuplTrigParamTypeUnknown = -1,
         ETOMASuplPeriodicParams,
         ETOMASuplAreaEventParams
         };
 public:
     /**
      * Constructor for TOMASuplTriggerParams
      */
     IMPORT_C TOMASuplTriggerParams();
 public:
     /**
      * Setter method for setting area event trigger params
      */
     IMPORT_C void SetAreaEventParams(const TOMASuplAreaEventParams areaEventParams);
     /**
      * Setter method for setting periodic trigger params
      */
     IMPORT_C void SetPeriodicParams(const TOMASuplPeriodicParams periodicEventParams);

 public:
     /**
      * Getter method for area event trigger params
      * @return KErrNone if this parameter is set and retrieved properly
      *         KErrOMASuplParamNotSet otherwise
      */
     IMPORT_C TInt GetAreaEventParams(TOMASuplAreaEventParams& areaEventParams)const;
     /**
      * Getter method for periodic event trigger params
      * @return KErrNone if this parameter is set and retrieved properly
      *         KErrOMASuplParamNotSet otherwise
      */
     IMPORT_C TInt GetPeriodicParams(TOMASuplPeriodicParams& periodicEventParams)const;
     /**
      * Getter method to get the type of trigger parameter present.
      */
     IMPORT_C void GetEventType(TOMASuplTriggerParamsType& aType)const;
 private:
     TOMASuplAreaEventParams iAreaEventParams;
     TOMASuplPeriodicParams iPeriodicEventParams;
     TOMASuplTriggerParamsType iEventType;
     };
 /**
  *  Class for providing the Triggered Start message for SUPL v2.0
  *
  *  @lib eposomasuplasnconverter.lib
  * @since S60 S60_5.2 
  */
 class COMASuplTriggeredStart:public COMASuplAsnMessageBase
     {
 public:
     
     /**
      * Enum to indicate cause code
      */
     enum TOMASuplTrigStartCauseCode
         {
         ETOMASuplCauseCodeUnknown = -1,
         ETOMASuplCauseCodeServingNetWorkNotInAreaIdList = 0,
         ETOMASuplCauseCodeSETCapabilitiesChanged = 1,
         ETOMASuplCauseCodeNoCoverage = 2,

         };
 public:
     
     /**
      * Enum to indicate the trigger type
      */
     enum TOMASuplTrigType
         {
         ETOMASuplTrigStartTrigTypeUnknown = -1,
         ETOMASuplPeriodicTrigType = 0,
         ETOMASuplAreaEventTrigType ,
         };

     public:  // Constructors and destructor
    
         /**
          * Two phased constructor
          * @since S60 S60_5.2
          * @param None
          * @return COMASuplTriggeredStart pointer 
          */         
    
         IMPORT_C static COMASuplTriggeredStart* NewL();
    
         /**
          * Destructor
          * @since S60 S60_5.2
          * @param None
          * @return None
          */  
         IMPORT_C ~COMASuplTriggeredStart();
             
     public: // Functions from base classes

         /**
          * Cloning of SuplTrigStart
          * @since S60 S60_5.2
          * @param aTrigStart pointer to triggred start message to be cloned
          * @param aErrorCode error code
          * @return Returns None
          */
         IMPORT_C void Clone(const COMASuplAsnMessageBase* aTrigStart,TInt& aErrorCode);
         
     public:
         
        /**
         * Getter method for Set Capabilities
         *@since S60 S60_5.2
         * @param aSETCapabilities a reference to the Set Caps member is returned in this
         *       ownership of the returned object stays with the Triggered start class
         */
        IMPORT_C void GetSETCapabilities(COMASuplSETCapabilitiesVer2*& aSETCapabilities) const;
        /**
         * Getter method for Location Id
         *@since S60 S60_5.2
         * @param aLocationId a refrence to the location id member is returned in this
         *          ownership of the returned object is with the triggered start class
         */
        IMPORT_C void GetLocationId(COMASuplLocationIdVer2*& aLocationId) const;
        /**
         * getter method for Qop 
         *@since S60 S60_5.2
         * @param aSuplQop qop value is returned in this
         * @return KErrNone if this parameter is set and retrieved successfully
         *         KErrOMASuplParamNotSet otherwise
         */
        IMPORT_C TInt GetQop(TOMASuplQop& aSuplQop) const;
        /**
         * getter method for Version 
         *@since S60 S60_5.2
         * @param aVer version value is returned in this
         * @return KErrNone if this parameter is set and retrieved successfully
         *         KErrOMASuplParamNotSet otherwise
         */
        IMPORT_C TInt GetVer(TDes8& aVer)const;
        /**
         * Getter method for multiple location Id
         *@since S60 S60_5.2
         * @param aMultipleLocIds a reference to the multiple location id member is returned in this
         *          ownership of the returned object is with the triggered start class
         */
        IMPORT_C void GetMultipleLocIds(COMASuplMultipleLocIds*& aMultipleLocIds)const;
        /**
         * Getter method for third party Id
         *@since S60 S60_5.2
         * @param aThirdparty a reference to the thirdparty  id member is returned in this
         *          ownership of the returned object is with the triggered start class
         */
        IMPORT_C void GetThirdParty(COMASuplThirdPartyIds*& aThirdparty)const;
        /**
         * getter method for trigger type
         * @return KErrNone if this parameter is set and retrieved successfully
         *         KErrOMASuplParamNotSet otherwise
         */
        IMPORT_C TInt GetTriggerType(TOMASuplTrigType&  aTrigType)const;
        /**
         * getter method for trigger params
         * @return KErrNone if this parameter is set and retrieved successfully
         *         KErrOMASuplParamNotSet otherwise
         */
        IMPORT_C TInt GetTriggerParams(TOMASuplTriggerParams& aTriggerParams)const;
        /**
         * Getter method for position
         *@since S60 S60_5.2
         * @param aPosition a reference to the position member is returned in this
         *          ownership of the returned object is with the triggered start class
         */
        IMPORT_C void GetPosition(COMASuplPosition*& aPosition)const;
        /**
         * getter method for reporting caps
         * @return KErrNone if this parameter is set and retrieved successfully
         *         KErrOMASuplParamNotSet otherwise
         */
        IMPORT_C TInt GetReportingCaps(TOMASuplReportingCap& aReportingCap)const;
        /**
         * getter method for cause code
         * @return KErrNone if this parameter is set and retrieved successfully
         *         KErrOMASuplParamNotSet otherwise
         */
        IMPORT_C TInt GetCauseCode(TOMASuplTrigStartCauseCode& aCauseCode)const;
     public:
         /**
          * Setter method for SET Capabilites
          *@since S60 S60_5.2
          * @param aSETCapabilities Set Capabilities to be set in Trigered Start 
          *         ownership of the object is transferred to the triggered start class
          * @return None
          */
         IMPORT_C void SetSETCapabilities(COMASuplSETCapabilitiesVer2* aSETCapabilities);
         
         /**
          * Setter method for Location Id
          *@since S60 S60_5.2
          * @param aLocationId Location Id to be set in Trigered Start 
          *         ownership of the object is transferred to the triggered start class
          * @return None
          */
         IMPORT_C void SetLocationId(COMASuplLocationIdVer2* aLocationId);
         
         /**
          * setter method for Qop
          *@since S60 S60_5.2
          * @param aSuplQop qop to be set in triggred start
          * @return None
          */
         IMPORT_C void SetQop(TOMASuplQop aSuplQop);
         
         /**
          * setter method for version
          *@since S60 S60_5.2
          * @param aVer version to be set in triggred start
          * @return None
          */
         IMPORT_C void SetVer(TDes8& aVer);
         
         /**
          * Setter method for Multiple Location Id
          *@since S60 S60_5.2
          * @param aMultipleLocIds Multiple Location Id to be set in Trigered Start 
          *         ownership of the object is transferred to the triggered start class
          * @return None
          */
         IMPORT_C void SetMultipleLocIds(COMASuplMultipleLocIds* aMultipleLocIds);
         
         /**
          * Setter method for Third party Id
          *@since S60 S60_5.2
          * @param aThirdparty Third party Id to be set in Trigered Start 
          *         ownership of the object is transferred to the triggered start class
          * @return None
          */
         
         IMPORT_C void SetThirdParty(COMASuplThirdPartyIds* aThirdparty);
         
         /**
          * setter method for trigger type
          *@since S60 S60_5.2
          * @param aTrigType trigger type to be set in triggred start
          * @return None
          */
         IMPORT_C void SetTriggerType(TOMASuplTrigType  aTrigType);
         
         /**
          * setter method for trigger params
          *@since S60 S60_5.2
          * @param aTriggerParams trigger params to be set in triggred start
          * @return None
          */
         IMPORT_C void SetTriggerParams(TOMASuplTriggerParams aTriggerParams);
         
         /**
          * Setter method for position
          *@since S60 S60_5.2
          * @param aPosition position to be set in Trigered Start 
          *         ownership of the object is transferred to the triggered start class
          * @return None
          */
         IMPORT_C void SetPosition(COMASuplPosition* aPosition);
         
         /**
          * setter method for reporting capabilities
          *@since S60 S60_5.2
          * @param aReportingCap reporting capabilities to be set in triggred start
          * @return None
          */
         IMPORT_C void SetReportingCaps(TOMASuplReportingCap       aReportingCap);
         
         /**
          * setter method for cause code
          * @since S60 S60_5.2
          * @param aCauseCode cause code to be set in triggred start
          * @return None
          */
         IMPORT_C void SetCauseCode(TOMASuplTrigStartCauseCode aCauseCode);
         
         /**
          * Setter method for Supl start
          * @since S60 S60_5.2
          * @param aSuplSETCaps SetCapabilities
          * @param aSuplLocationId Location Id
          * @return Returns None
          */

         IMPORT_C void SetSuplTriggeredStart(COMASuplSETCapabilitiesVer2* aSuplSETCaps,
                 COMASuplLocationIdVer2* aSuplLocationId,
                 TBool aeCID);

        		
         /**
          * Setter method for E-Cellid in Supl start
          * @since S60 S60_5.2
          * @param aECId,Enhance Cell id
          * @return None
          */
         IMPORT_C void SetECellId(TOMASuplECellInfo& aECId);

         /**
          * Getter method for E-Cellid in Supl start
          * @since S60 S60_5.2
          * @param aECId,Enhance Cell id
          * @return None
          */
         IMPORT_C void GetECellId(TOMASuplECellInfo& aECId);

     protected: 

         IMPORT_C void ConstructL();        

         IMPORT_C COMASuplTriggeredStart(); 

         static COMASuplTriggeredStart* NewLC();
         
     protected: 
         // SetCapabilities*/
         COMASuplSETCapabilitiesVer2* iSETCapabilities; 
         // Location Id*/
         COMASuplLocationIdVer2* iLocationId;

         //Quality of position
         TOMASuplQop iSuplQop; //optional parameter

         //To hold E-cell id data
         TOMASuplECellInfo iMmCellInfo;

         TBool ieCID;

         TBuf8<KVer> iVer;

         COMASuplMultipleLocIds* iMultipleLocIds;//optional parameter

         COMASuplThirdPartyIds* iThirdparty; //optional parameter

         TOMASuplTrigType  iTrigType; //optional parameter

         TOMASuplTriggerParams      iTriggerParams;//optional parameter
         COMASuplPosition*          iPosition; //optional parameter
         TOMASuplReportingCap       iReportingCap; //optional parameter
         TOMASuplTrigStartCauseCode iCauseCode; //optional parameter

         //order of bits in optional mask - iSuplQop|iVer|iTrigType|iTriggerParams|iReportingCap|iCauseCode

     };

#endif /*EPOS_COMASUPLTRIGGEREDSTART_H_*/
