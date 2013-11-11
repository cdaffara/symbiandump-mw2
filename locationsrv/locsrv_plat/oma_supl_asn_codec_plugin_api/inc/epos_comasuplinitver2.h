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
* Description:  Header file for supl 2.0 init message
*
*/

#ifndef C_COMASuplInitVer2_H
#define C_COMASuplInitVer2_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_suplconstants.h"
#include "epos_comasuplinit.h"
#include "epos_comasuplnotificationver2.h"


/**
 *  Class for providing the time window for reporting
 *  @lib epos_comasuplasnplugin.lib
 *  @since S60 S60_5.2
 */
class TOMASuplTimeWindow 
    {
public:
    /**
     * Constructor for  TOMASuplTimeWindow
     */
    IMPORT_C TOMASuplTimeWindow();

    /**
     * Getter method  for TOMASuplTimeWindow parameters
     * @since S60 S60_5.2
     * @param aStartTime Start Time for time window is returned in this
     * @param aStopTime Stop Time for time window is returned in this
     * @return Returns KErrNone if parameters were set before and are retrieved successfully
     *                 KErrOMASuplParamNotSet otherwise
     */
    IMPORT_C TInt GetTimeWindow(TInt32& aStartTime, TInt32& aStopTime);
public:
    /**
     * Setter method  for TOMASuplTimeWindow
     * @since S60 S60_5.2
     * @param aStartTime Start Time to set for time window
     * @param aStopTime Stop Time to set for time window
     * @return Returns None
     */
    IMPORT_C void SetTimeWindow(TInt32 aStartTime,TInt32 aStopTime);
private:
    //holds the start time for time window
    TInt32 iStartTime;
    //holds the stop time for time window
    TInt32 iStopTime;
    //indicates if the timewindow is set or not
    TInt iIsTimeWindowSet;
    } ;

/**
 *  Class for providing the reporting criteria for reporting
 *  @lib epos_comasuplasnplugin.lib
 *  @since S60 S60_5.2
 */
class TOMASuplReportingCriteria 
    {

public:
    IMPORT_C TOMASuplReportingCriteria();
    /**
     * Getter method  for TOMASuplTimeWindow
     * @since S60 S60_5.2
     * @param aTimeWindow Time window that is set for reporting criteria.
     * @return Returns KErrNone if this parameter is set and retrieved successfully,
     *                 KErrOMASuplParamNotSet otherwise
     */
    IMPORT_C TInt GetTimeWindow(TOMASuplTimeWindow& aTimeWindow);
    /**
     * Getter method  for number of reports
     * @since S60 S60_5.2
     * @return Returns Number of reports  if this parameter is set and retrieved successfully,
     *                 KErrOMASuplParamNotSet otherwise
     */
    IMPORT_C TInt32 GetNumOfreports();
    
    /**
     * Getter method  for minimum time interval
     * @since S60 S60_5.2
     * @return Returns Minimum time interval value  if this parameter is set and retrieved successfully,
     *                 KErrOMASuplParamNotSet otherwise
     */
    IMPORT_C TInt32 GrtMinTimeInt();
    
public:
    
    /**
     * Setter method  for TOMASuplTimeWindow
     * @since S60 S60_5.2
     * @param aTimeWindow Time window to set for reporting criteria.
     * @return none
     */
    IMPORT_C void SetTimeWindow(TOMASuplTimeWindow aTimeWindow);
    
    /**
     * Setter method  for number of reports
     * @since S60 S60_5.2
     * @param aMaxRepNum value to be set as maximum number of reports  
     * @return none
     */
    IMPORT_C void SetNumOfreports(TInt32 aMaxRepNum);
    
    /**
     * Setter method  for minimum interval time
     * @since S60 S60_5.2
     * @param aMaxRepNum value to be set as minimum time interval  
     * @return none
     */
    IMPORT_C void SetMinTimeInt(TInt32 aMintTime);
private:
    TInt iOptionalMask;
    TOMASuplTimeWindow iTimeWindow;
    TInt32 iMaxNumberofReports;
    TInt32 iMinTimeInterval;
    };

/**
 *  Class for providing the historic reporting data
 *  @lib epos_comasuplasnplugin.lib
 *  @since S60 S60_5.2
 */
class TOMASuplHistoricReporting 
    {
public:
    enum TOMASuplAllowedReportingType
        {
        EAllowedReportingTypeUnknown = -1,
        EAllowedReportingPositionsOnly = 0,
        EAllowedReportingMeasurementsOnly = 1,
        EAllowedReportingPositionsAndMeasurements = 2,
        };
public:
    /**
     *Constructor for TOMASuplHistoricReporting
     */
    IMPORT_C TOMASuplHistoricReporting();

    /**
     * Setter method  for allowed reporting criteria type
     * @since S60 S60_5.2
     * @param aAllowedRepType value to be set for allowed reporting criteria type
     * @return none
     */
    IMPORT_C void SetAllowedRepType(TOMASuplAllowedReportingType aAllowedRepType);
    
    /**
     * Setter method  for  reporting criteria
     * @since S60 S60_5.2
     * @param aRepCrit value to be set for  reporting criteria
     * @return none
     */
    IMPORT_C void SetRepCriteria(TOMASuplReportingCriteria aRepCrit);
public:
    /**
     * Getter method for allowed reporting criteria type
     * @since S60 S60_5.2
     * @param aAllowedRepType value of allowed reprting type is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetAllowedRepType(TOMASuplAllowedReportingType& aAllowedRepType);
    
    /**
     * Getter method for  reporting criteria 
     * @since S60 S60_5.2
     * @param aRepCrit value of  reprting  criteria is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetRepCriteria(TOMASuplReportingCriteria& aRepCrit);
    
private:
    
    TInt iOptionalMask;
    TOMASuplAllowedReportingType iAllowedReportingType;
    TOMASuplReportingCriteria iReportingCriteria;
    };

/**
 *  Class for providing the basic protection parameters
 *  @lib epos_comasuplasnplugin.lib
 *  @since S60 S60_5.2
 */
class TOMASuplBasicProtectionParams 
    {
public:
    /**
     * Getter method for  key identifier 
     * @since S60 S60_5.2
     * @param aKeyId value of  key identity is returned in this
     * @return none 
     */
    IMPORT_C void GetKeyIdentifier(TDes8& aKeyId);
    
    /**
     * Getter method for  replay coungter 
     * @since S60 S60_5.2
     * @return  value of  replay counter is returned 
     */
    IMPORT_C TInt32 GetReplayCounter();
    
    /**
     * Getter method for  basic MAC 
     * @since S60 S60_5.2
     * @param aBasicMac value of  basic MAC is returned in this 
     * @return  none
     */
    IMPORT_C void GetBasicMac(TDes8& aBasicMac);
public:
    
    /**
     * Setter method for  key identifier 
     * @since S60 S60_5.2
     * @param aKeyId value of  key identity to be set
     * @return none 
     */
    IMPORT_C void SetKeyIdentifier(TDes8& aKeyId);
    
    /**
     * Setter method for  replay counter 
     * @since S60 S60_5.2
     * @param aCounter value of  key identity to be set
     * @return none 
     */
    IMPORT_C void SetReplayCounter(TInt32 aCounter);
    
    /**
     * Setter method for  basic MAC 
     * @since S60 S60_5.2
     * @param aCounter value of  key identity to be set
     * @return none 
     */
    IMPORT_C void SetBasicMac(TDes8& aBasicMac);
private:
    TBuf8<KBasicProtKeyIdLen> iKeyIdentifier;
    TInt32 iBasicReplayCounter;
    TBuf8<KBasicProtMacLen> iBasicMAC;
    };

class TOMASuplProtectionLevel 
    {
public:
    enum TOMASuplProtLevelType
        {
        ETOMASuplProtLevelTypeUnknown = -1,
        ETOMASuplProtLevelTypeNullProtection  = 0,
        ETOMASuplProtLevelTypeBasicProtection = 1, 
        };
public:
    IMPORT_C TOMASuplProtectionLevel();
    IMPORT_C void SetProtLevel(TOMASuplProtLevelType aProtLevel);
    IMPORT_C void SetProtParams(TOMASuplBasicProtectionParams aBasicProtParams);
public:
    IMPORT_C TInt GetProtLevel(TOMASuplProtLevelType& aProtLevel);
    IMPORT_C TInt GetProtParams(TOMASuplBasicProtectionParams& aBasicProtParams);
private:    
    TInt iOptionalMask;
    TOMASuplProtLevelType iProtlevel;
    TOMASuplBasicProtectionParams iBasicProtectionParams;
    };

/**
 *  Class for providing the supl init message extension for SUPL v2.0
 *  For holding the supl init message extension 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */

class COMASuplVer2SuplInitExtn:public CBase
    {
public:
    /**
     * Enum to indicate the notification mode
     * 
     */
    enum TOMASuplNotificationMode
        {
        ETOMASuplNotificationModeUnknown = -1,
        ETOMASuplNotificationModeNormal = 0,
        ETOMASuplNotificationModeLocation = 1
        };
public:
    /**
     * Two phased constructor
     * @since S60 S60_5.2
     * @param None
     * @return COMASuplVer2SuplInitExtn pointer to SUPL Init V2 Extn Message
     */         

    IMPORT_C static COMASuplVer2SuplInitExtn* NewL();
    /**
     * Destructor
     * @since S60 S60_5.2
     * @param None
     * @return None
     */  
    IMPORT_C virtual ~COMASuplVer2SuplInitExtn();
    
private:
    COMASuplVer2SuplInitExtn();   
public:
    /**
     * Function for cloning the Supl Init v2.0 extension
     * @since S60 S60_5.2
     * @param aCloneV2Extn A pointer to the SUPL Init V2 extension  object to be cloned
     * @return None
     */
    IMPORT_C void CloneL(COMASuplVer2SuplInitExtn* aCloneV2Extn);

private:
    void CloneSLPAddressL(COMASuplVer2SuplInitExtn* aCloneV2Extn);
public:
    
    /**
     * Getter method for retrieving notification mode
     * @since S60 S60_5.2
     * @param aMode notification mode is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetNotificationMode(TOMASuplNotificationMode& aMode);
    /**
     * Getter method for retrieving supported network information
     * @since S60 S60_5.2
     * @param aSupportedNetworkInformation supported nw info is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetSupNwInfo(TOMASuplSuppNetworkInformation& aSupportedNetworkInformation);
    /**
     * Getter method for retrieving trigger type
     * @since S60 S60_5.2
     * @param aTrigType Trigger type is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetTrigType(COMASuplTriggeredStart::TOMASuplTrigType& aTrigType);
    /**
     * Getter method for retrieving slp address
     * @since S60 S60_5.2
     * @param aSLPAddress a reference to slp address is returned in this
     *                    ownership is still with the v2 extn object
     * 
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetHSLPAddress(COMASuplSLPAddress*& aSLPAddress);
    /**
     * Getter method for retrieving historic reporting 
     * @since S60 S60_5.2
     * @param aHistoricReporting historic reporting is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetHistoricRep(TOMASuplHistoricReporting& aHistoricReporting);
    /**
     * Getter method for retrieving protection level
     * @since S60 S60_5.2
     * @param aProtectionLevel protection level is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetProtLevel(TOMASuplProtectionLevel& aProtectionLevel);
    
    /**
     * Getter method for retrieving minor and major version
     * @since S60 S60_5.2
     * @param aVer version is returned in this
     * @return KErrNone if this parameter is set and retrieved successfully
     *         KErrOMASuplParamNotSet if parameter is not set 
     */
    IMPORT_C TInt GetMinMajorVer(TInt& aVer);

public:
    /**
     * Setter method for setting notification mode
     * @since S60 S60_5.2
     * @param aMode notification mode ito be set
     * @return none
     */
    IMPORT_C void SetNotificationMode(TOMASuplNotificationMode aMode);
    /**
     * Setter method for setting supported network information
     * @since S60 S60_5.2
     * @param aSupportedNetworkInformation supported nw info to be set
     * @return none
     */
    IMPORT_C void SetSupNwInfo(TOMASuplSuppNetworkInformation aSupportedNetworkInformation);
    /**
     * Setter method for setting trigger type
     * @since S60 S60_5.2
     * @param aTrigType Trigger type to be set
     * @return none
     */
    IMPORT_C void SetTrigType(COMASuplTriggeredStart::TOMASuplTrigType aTrigType);

    /**
     * Setter method for setting slp address
     * @since S60 S60_5.2
     * @param aSLPAddress  slp address to be set ownership is trasnferred to the v2 extn object
     * 
     * @return none
     */
    IMPORT_C void SetHSLPAddress(COMASuplSLPAddress* aSLPAddress);
    /**
     * Setter method for setting historic reporting 
     * @since S60 S60_5.2
     * @param aHistoricReporting historic reporting to be set
     * @return none
     */
    IMPORT_C void SetHistoricRep(TOMASuplHistoricReporting aHistoricReporting);
    
    /**
     * Setter method for setting protection level
     * @since S60 S60_5.2
     * @param aProtectionLevel protection level to be set
     * @return none
     */
    IMPORT_C void SetProtLevel(TOMASuplProtectionLevel aProtectionLevel);
    
    /**
     * Setter method for setting minor and major version
     * @since S60 S60_5.2
     * @param aVer version to be set
     * @return none
     */
    IMPORT_C void SetMinMajorVer(TInt aVer);

private:

    TOMASuplNotificationMode iNotificationMode;
    TOMASuplSuppNetworkInformation iSupportedNetworkInformation;
    COMASuplTriggeredStart::TOMASuplTrigType iTriggerType;
    COMASuplSLPAddress* iSLPAddress;
    TOMASuplHistoricReporting iHistoricReporting;
    TOMASuplProtectionLevel iProtectionLevel;
    TInt iMinMajorVersion;
    //iNotificationMode = 7|iSupportedNetworkInformation = 6|iTriggerType = 5|iSLPAddress = 4|iHistoricReporting; = 3
    //          iProtectionLevel = 2| iGnssPosTechnology(not implemented now) = 1|iMinMajorVersion 
    TInt iOptionalMask;
    };

/**
 *  Class for providing the SUPL Init message for SUPL v2.0
 *  For holding the Supl Init message and for encoding the supl init message 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60 S60_5.2
 */

class COMASuplInitVer2  : public COMASuplInit
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_5.2
        * @param None
        * @return COMASuplInitVer2 pointer to SuplStart Message
        */         
    
	    IMPORT_C static COMASuplInitVer2* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_5.2
        * @param None
        * @return None
        */  
     	IMPORT_C ~COMASuplInitVer2();
                
    public:
        /**
         * Function for cloning the Supl Init message
         * @since S60 S60_5.2
         * @param aInit A pointer to the SUPL Init  object to be cloned
         * @param aErrorCode Error code is returned in this
         * @return None
         */
		IMPORT_C void Clone(const COMASuplAsnMessageBase* aInit,TInt& aErrorCode);
		
		/**
		 * Function for cloning the Supl Init v2.0 notification
		 * @since S60 S60_5.2
		 * @param aSuplInit A pointer to the SUPL Init  object to be cloned
		 * @param aErrorCode Error code is returned in this
		 * @return None
		 */
		IMPORT_C void CloneNotificationV2L(const COMASuplInitVer2* aSuplInit);
		
		/**
		 * Function for cloning the Supl Init v2.0 extn
		 * @since S60 S60_5.2
		 * @param aSuplInit A pointer to the SUPL Init  object to be cloned
		 * @param aErrorCode Error code is returned in this
		 * @return None
		 */
		IMPORT_C void CloneInitV2ExtnL(COMASuplInitVer2* aSuplInit);
		
   	public: 
      	 
         /**
          * Setter method for Version 2 extension in Supl Init
          * @since S60 S60_5.2
          * @param aVer2Extn,COMASuplVer2SuplInitExtn to be set in the SUPL Init object
          *                  ownership is transferred to the SUPL Init object
          * @return None
          */
         IMPORT_C void SetVer2SuplInitExtn(COMASuplVer2SuplInitExtn* aVer2Extn);
         
         
         /**
          * Getter method for Version 2 extension in Supl start
          * @since S60 S60_5.2
          * @param aVer2Extn,COMASuplVer2SuplInitExtn returned
          * @return None
          */
         IMPORT_C TInt GetVer2SuplInitExtn(COMASuplVer2SuplInitExtn*& aVer2Extn);
         
         /**
          * Setter method for Version 2 notification in Supl init v2
          * @since S60 S60_5.2
          * @param aVer2Extn,COMASuplVer2SuplStartExtn to be set
          * @return None
          */
         IMPORT_C void SetVer2Notification(COMASuplNotificationVer2* aNotificationV2);


         /**
          * Getter method for Version 2 extension in Supl start
          * @since S60 S60_5.2
          * @param aVer2Extn,COMASuplVer2SuplStartExtn returned
          * @return None
          */
         IMPORT_C TInt GetVer2Notification(COMASuplNotificationVer2*& aNotificationV2)const;
         
   	protected: 

   	    void ConstructL();        

   	    IMPORT_C COMASuplInitVer2(); 


   	protected:    // Data

   	    // Notification
   	    COMASuplNotificationVer2* iNotificationV2;
   	    //v2.0 extension for supl init message
   	    COMASuplVer2SuplInitExtn* iVer2InitExtn;
 };


#endif //C_COMASuplInitVer2_H
