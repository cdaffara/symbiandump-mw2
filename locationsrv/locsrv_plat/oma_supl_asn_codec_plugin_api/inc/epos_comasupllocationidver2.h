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
* Description:  Header file for providing SUPL 2.0 location id
*
*/

#ifndef C_COMASuplLocationIdVer2_H
#define C_COMASuplLocationIdVer2_H

#include<e32def.h>
#include<e32std.h>
#include<e32base.h>

#include "epos_suplconstants.h"
#include "epos_comasupllocationid.h"

/**
 *  Class for providing holding location data
 *  @lib epos_comasuplasnplugin.lib
 *  @since S60 S60_5.2
 */
class TOMASuplLocationData
    {
    public:
        
        IMPORT_C TOMASuplLocationData();
        
        /**
         * Getter method  for location accuracy
         * @since S60 S60_5.2
         * @param aLocationAccuracy location accuracy value is returned in this
         * @return Returns KErrNone if parameter was set before and are retrieved successfully
         *                 KErrOMASuplParamNotSet otherwise
         */
        IMPORT_C TInt GetLocationAccuracy(TInt& aLocationAccuracy)const ;
        
        /**
         * Setter method  for location accuracy
         * @since S60 S60_5.2
         * @param aLocationAccuracy location accuracy value to be set
         * @return none
         */
        IMPORT_C void SetLocationAccuracy(TInt aLocationAccuracy);
        /**
         * Getter method  for location value
         * @since S60 S60_5.2
         * @param aLocationValue location accuracy value is returned in this
         * @return none
         */
        IMPORT_C void GetLocationValue(TDes8& aLocationValue) const ;
        /**
         * Setter method  for location accuracy
         * @since S60 S60_5.2
         * @param aLocationAccuracy location accuracy value to be set
         * @return none
         */       
        IMPORT_C void SetLocationValue(TDes8& aLocationValue);
        
    private:
    //flag to indicate if location accuracy is set or not
    TBool iIsLocationAccSet;
    // holds location accuracy value
    TInt iLocationAccuracy;
    //holds location values
    TBuf8<KMaxLocDataValueLen>  iLocationValue;
    };

/**
 *  Class for  holding reported location
 *  @lib epos_comasuplasnplugin.lib
 *  @since S60 S60_5.2
 */
class TOMASuplReportedLocation 
    {
    public:
        
        /**
         * enum to indicate the type of location  descriptor  encoding present
         */
        enum TOMASuplLocEncDescriptor
        {
         EOMASupLocEncDescUnknown = -1,
         EOMASupLocEncDescLCI = 0 , 
         EOMASupLocEncDescASN1 = 1,
        };
        /**
         * Getter method  for location  descriptor encoding type
         * @since S60 S60_5.2
         * @param aLocationEncodingDescriptor location  descriptor encoding type is returned in this
         * @return none
         */
        IMPORT_C void GetLocationEncDescriptor(TOMASuplLocEncDescriptor&   aLocationEncodingDescriptor) const ;
        
        /**
         * Getter method  for location  data
         * @since S60 S60_5.2
         * @param aLocationData location  data is returned in this
         * @return none
         */
        IMPORT_C void GetLocationData(TOMASuplLocationData& aLocationData) const ;
        
        /**
         * Setter method  for location  descriptor encoding type
         * @since S60 S60_5.2
         * @param aLocationEncodingDescriptor location  descriptor encoding type to be set
         * @return none
         */
        IMPORT_C void SetLocationEncDescriptor(const TOMASuplLocEncDescriptor   aLocationEncodingDescriptor) ;
        
        /**
         * Setter method  for location  data
         * @since S60 S60_5.2
         * @param aLocationData location  data to be set
         * @return none
         */
        IMPORT_C void SetLocationData(const TOMASuplLocationData aLocationData);

    private:
        
    TOMASuplLocEncDescriptor   iLocationEncodingDescriptor;
    
    //location data field
    TOMASuplLocationData       iLocationData;
    };

   
/**
 *  Class containing cell info extension
 *  
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */
class TOMASuplVer2CellInfoExtn
    {
    public:
        /**
         * enum to indicate type present in the cell info extension
         */
        enum TOMASuplCellInfoExtnType
            {
            EOMASuplCellInfoUnknown = -1,
            EOMASuplCellInfoHrpd = 0,
            EOMASuplCellInfoUmb,
            EOMASuplCellInfoLte,
            EOMASuplCellInfoWLANAP,
            EOMASuplCellInfoWimaxBiz
            };
        /**
         * Constructor for TOMASuplVer2CellInfoExtn
         * 
         */
        IMPORT_C TOMASuplVer2CellInfoExtn();
        
        /**
         * Getter method for Cell Info Extn Type
         * @since S60 S60_5.2
         * @param aTypePresent Cell Info Extn Type is retrieved in this
         * @return none
         */
        IMPORT_C void GetCellInfoExtnType(TOMASuplCellInfoExtnType& aTypePresent) const ;
        /**
         * Setter method for Cell Info Extn Type
         * @since S60 S60_5.2
         * @param aTypePresent Cell Info Extn Type is retrieved in this
         * @return none
         */
        IMPORT_C void SetCellInfoExtnType(TOMASuplCellInfoExtnType aTypePresent)  ;
    
    private:
    //TOMASuplHrpdCellInformation iHrpdCellInfo;
    //TOMASuplUmbCellInformation  iUmbCellInfo;
    //TOMASuplLteCellInformation    iLteCellInfo;
    //TOMASuplWlanAPInformation      iWLANAPInfo;
    //TOMASuplWimaxBSInfo          iWimaxBSInfo;
    TOMASuplCellInfoExtnType       iTypePresent;
    };


enum TOMASuplLocEncDescriptor
    {
    EOMASupLocEncDescUnknown = -1,
    EOMASupLocEncDescLCI = 0, 
    EOMASupLocEncDescASN1 = 1, 
    };

/**
 *  Class containing location information
 *  
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */

class COMASuplLocationIdVer2 : public COMASuplLocationId
    {
	public:
	/**
        * Two phased constructor
        * @since S60 S60_5.2
        * @param None
        * @return instance of COMASuplLocationIdVer2
        */ 

	IMPORT_C static COMASuplLocationIdVer2* NewL();
    
     /**
        * Destructor
        * @since S60 S60_5.2
        * @param None
        * @return None
        */
     IMPORT_C ~COMASuplLocationIdVer2();
     /**
      * Getter method Version 2 Cell Extension  in E-CellID
      * @since S60 S60_5.2
      * @param aTOMASuplVer2CellInfo, TOMASuplVer2CellInfoExtn
      * @return None                                           
      */
     IMPORT_C TInt Ver2CellInfo(TOMASuplVer2CellInfoExtn& aTOMASuplVer2CellInfo) const;
     /**
      * Setter method for GSM in E-CellID
      * @since S60 S60_5.2
      * @param aTOMASuplVer2CellInfo TOMASuplVer2CellInfoExtn
      * @return None                                           
      */   
     IMPORT_C void SetVer2CellInfo(TOMASuplVer2CellInfoExtn aTOMASuplVer2CellInfo);

     /**
      * Clone Method for COMASuplLocationIdVer2
      * @since S60 S60_5.2
      * @return A clone of this location id object is returned
      *         ownership of returned object is not with this object
      */
     IMPORT_C COMASuplLocationIdVer2* CloneL();
     
private: // Construction
	
	 IMPORT_C COMASuplLocationIdVer2();

	 void ConstructL();        
	
private:
    
	TOMASuplVer2CellInfoExtn iOMASuplVer2CellInfoExtn;
    TBool iIsV2ExtnSet;
    };

#endif //  C_COMASUPLLOCATIONID_H
