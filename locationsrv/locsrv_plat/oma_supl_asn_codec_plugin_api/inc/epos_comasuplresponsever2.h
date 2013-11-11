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
* Description:  Header file for supl 2.0 response message
*
*/

#ifndef C_COMASuplResponseVer2_H
#define C_COMASuplResponseVer2_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplposmethod.h"
#include "lbs/epos_comasuplposition.h"
#include "epos_suplconstants.h"
#include "epos_comasuplresponse.h"
//Forward declaration
class COMASuplSLPAddress;

/**
 * Class to hold SPC SET Key
 * 
 */
class TOMASuplSPCSETKey 
    {    
public:
    /**
     * Constructor for TOMASuplSPCSETKey
     */
    IMPORT_C TOMASuplSPCSETKey();
    /**
     * getter method for SPCSetKey
     * 
     */
    IMPORT_C TInt GetSuplSPCSetKey(TDes8& keyStr)const;
public:
    /**
     * Setter method for SPCSetKey
     */
    IMPORT_C void SetSuplSPCSetKey(TDes8& keyStr);
private:
    TBool iIsKeySet; 
    TBuf8<KSPCSETKeyLen> st;
    };





class TOMASuplSuppWCDMAInfo
    {
public:
    IMPORT_C TBool GetMRL();
public:
    IMPORT_C void SetMRL(TBool aMrl);
private:
    TBool iMRL;
    };

/**
 * Class to hold supported network information
 */
class TOMASuplSuppNetworkInformation 
    {
    
public:
    /**
     * Constructor for TOMASuplSuppNetworkInformation
     */
    IMPORT_C TOMASuplSuppNetworkInformation();
   
    /**
     * Setter metthod for setting GSM
     */  
    IMPORT_C void SetGSM(TBool aGSM);
    /**
     * Setter method for setting WCDMA
     */    
     IMPORT_C void SetWCDMA(TBool aWCDMA);
     /**
      * Setter method for setting CDMA
      */    
     IMPORT_C void SetCDMA(TBool aCDMA);
     /**
      * Setter method for setting historic
      */       
     IMPORT_C void SetHistoric(TBool aHistoric);
     /**
      * Setter method for setting non serving flag
      */ 
     IMPORT_C void SetNonServing(TBool aNonServing);
     /**
      * Setter method for Set UTRAN GPS Ref Time
      */   
     IMPORT_C void SetUTRANGPSRefTime(TBool aRefTime);
     /**
      * Setter method for Set UTRAN GANSS Ref Time
      */    
     IMPORT_C void SetUTRANGANSSRefTime(TBool aRefTime);
    
     /**
      * Setter method for WCDMA Info
      */   
     IMPORT_C void SetSupWCDMAInfo(TOMASuplSuppWCDMAInfo aSupportedWCDMAInfo);

public:
   
     
    IMPORT_C TBool GetGSM();
    /**
     * Setter method for setting WCDMA
     */  
    IMPORT_C TBool GetWCDMA();
    /**
     * Setter method for setting CDMA
     */ 
    IMPORT_C TBool GetCDMA();
    /**
     * Setter method for setting historic
     */    
    IMPORT_C TBool GetHistoric();
    /**
     * Setter method for setting non serving
     */ 
    IMPORT_C TBool GetNonServing();
    /**
     * Setter method for setting UTRAN GPS Ref Time
     */ 
    IMPORT_C TBool GetUTRANGPSRefTime();
    /**
     * Setter method for setting UTRAN GANSS Ref Time
     */ 
    IMPORT_C TBool GetUTRANGANSSRefTime();
   
    /**
     * Setter method for setting WCDMA Info
     */    
    IMPORT_C TInt GetSupWCDMAInfo(TOMASuplSuppWCDMAInfo& aSupportedWCDMAInfo);

private:

    TBool iGSM;
    TBool iWCDMA;
    TOMASuplSuppWCDMAInfo iSupportedWCDMAInfo;
    TBool iCDMA;
    TBool iHistoric;
    TBool iNonServing;
    TBool iUTRANGPSReferenceTime;
    TBool iUTRANGANSSReferenceTime;
    //iSupportedWLANInfo|iSupportedWLANApsList|iSupportedWCDMAInfo
    TInt iOptionalMask;
    };


class COMASuplSPCTID: public CBase
        {
    public:  // Constructors and destructor
    
        /**
         * Two phased constructor
         * @since S60 S60_5.2
         * @param None
         * @return COMASuplSPCTID pointer 
         */         
    
        IMPORT_C static COMASuplSPCTID* NewL();
    
        /**
         * Destructor
         * @since S60 S60_5.2
         * @param None
         * @return None
         */ 
        IMPORT_C ~COMASuplSPCTID();
    
    
    
    public: 
        /**
         * GetRAND Method.
         * @since S60 S60_5.2
         * @param aRAND   RAND
         * @return None
         */
        IMPORT_C TInt GetRAND(TDes8& aRAND)const;

        /**
         * GetFqdn Method.
         * @since S60 S60_5.2
         * @param aFqdn Fqdn
         * @return None
         */
        IMPORT_C TInt GetFqdn(HBufC8*& aFqdn)const;  

    
    public:     //Setter Methods
    
    
      
       /**
        * SetRAND Method.
        * @since S60 S60_5.2
        * @param aRAND   RAND
        * @return None
        */
        IMPORT_C void SetRAND(const TDesC8& aRAND);
    
      /**
       * SetFqdn Method.
       * @since S60 S60_5.2
       * @param aFqdn Fqdn
       * @return None
       */
       IMPORT_C void SetFqdn(HBufC8* aFqdn);  
    
       /**
        * Clone Method for COMASuplSPCTID
        * @return a clone of 'this' object is returned 
        *          ownership of the returned object is not with the COMASuplSPCTID class
        */
       IMPORT_C COMASuplSPCTID* CloneL();
    private: 
    
        void ConstructL();        
    
        COMASuplSPCTID();
        
    private:
        /*FQDN */
        HBufC8* iFqdn;        
        TBuf8<KRANDStrLen>  iRAND;
        
        TInt iOptionalMask;
    };
    
/**
 * Class to hold the SUPL 2.0 extension to the SUPL Response message
 * 
 */
class COMASuplResponseV2Extn: public CBase
    {    
    public:  // Constructors and destructor
        /**
         * Two phased constructor
         * @since S60 S60_5.2
         * @param None
         * @return COMASuplResponseV2Extn pointer 
         */         
    
        IMPORT_C static COMASuplResponseV2Extn* NewL();
    
        /**
         * Destructor
         * @since S60 S60_5.2
         * @param None
         * @return None
         */ 
        IMPORT_C ~COMASuplResponseV2Extn();

    public: //getter methods
        /**
         * Getter method for supported network information
         */
        IMPORT_C void GetSupNetworkInfo(TOMASuplSuppNetworkInformation& aSupNwInfo);
        /**
         * Getter method for SPD SET Key
         */
        IMPORT_C void GetSPCSETKey(TOMASuplSPCSETKey& aSupSPCSETKey);
        /**
         * Getter method for SPCT id
         * @param aSupSPCTid a reference to the SPCT Id is returned in this
         *                   ownership of the returned object stays with the Response Extn object
         */
        IMPORT_C void GetSPCTId(COMASuplSPCTID*& aSupSPCTId);
        /**
         * Getter method for SPC Lifetime
         */
        IMPORT_C void GetSPCLifetime(TInt8& aSPCSETLifeTime);
        /**
         * Getter method for Position
         * @param aSupPosition a reference to the position member is returned in this
         *                      ownership of the returned object stays with the response extn object
         */
        IMPORT_C void GetSuplPos(COMASuplPosition*& aSupPosition);
                
    public: //setter methods

        IMPORT_C void SetSupNetworkInfo(TOMASuplSuppNetworkInformation aSupNwInfo);
        /**
         * Setter method for SPCT Set Key
         */
        IMPORT_C void SetSPCSETKey(TOMASuplSPCSETKey aSupSPCSETKey);
        /**
         * Setter method for SPCT Id 
         * @param aSupSPCTId SPCT Id to be set
         *                   ownership is transferred to the response extn object
         */
        IMPORT_C void SetSPCTId(COMASuplSPCTID* aSupSPCTId);
        /**
         * Setter method for SPC Lifetime
         */
        IMPORT_C void SetSPCLifetime(TInt8 aSPCSETLifeTime);
        /**
         * Setter method for Position
         * @param aSupPosition position object to be set 
         *                     ownership is transferred to the response extn object
         */
        IMPORT_C void SetSuplPos(COMASuplPosition* aSupPosition);
        
        
        /***
         * Cloning of COMASuplResponseV2Extn
         * @since S60 S60_5.2
         * @return Returns returns a clone to this object
         */
        IMPORT_C COMASuplResponseV2Extn* CloneL();
    private:
     
      void ConstructL();        
       
      COMASuplResponseV2Extn();
      
    private:
        TOMASuplSuppNetworkInformation iSupportedNetworkInformation;
        TOMASuplSPCSETKey iSPCSETKey;
        COMASuplSPCTID* iSPCTID;
        TInt8 iSPCSETKeylifetime;
        COMASuplPosition* iPosition;
        };
/**
 *  Class for providing the suplresponse message
 *  For holding the Supl Response message
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */

class COMASuplResponseVer2  : public COMASuplResponse
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_5.2
        * @param None
        * @return COMASuplResponseVer2 pointer to SuplResponse Message
        */         
    
        IMPORT_C static COMASuplResponseVer2* NewL();
    
        /**
        * Destructor
        * @since S60 S60_5.2
        * @param None
        * @return None
        */ 
        IMPORT_C ~COMASuplResponseVer2();
        
    public :
        
        
        /**
         * Setter method for SUPL v2.0 extension for SUPL Response
         * @since S60 S60_5.2
         * @param aVer2Extn version 2 extn to be set in SUPL Response
         *         ownership is transferred to the SUPL Response class
         * @return Returns None
         */

        IMPORT_C void SetResponseV2Extn(COMASuplResponseV2Extn* aVer2Extn);

        /**
         * Getter method for SUPL v2.0 extension for SUPL Response
         * @since S60 S60_5.2
         * @param aVer2Extn a reference to the version 2 extn is returned in this
         *         ownership of the object is still with the SUPL Response class
         * @return Returns None
         */

        IMPORT_C TInt GetResponseV2Extn(COMASuplResponseV2Extn*& aVer2Extn);
                
        /**
         * Cloning of SuplResponse2
         * @since  S60 S60_5.2
         * @param aResponse Base pointer
         * @param aErrorCode error code
         * @return Returns None
         */

        IMPORT_C void Clone(const COMASuplAsnMessageBase* aResponse,TInt& aErrorCode);


    protected: 
      
      void ConstructL();        
       
      IMPORT_C COMASuplResponseVer2();  
      

    private:    // Data
    
        COMASuplResponseV2Extn* iVer2Extn;
    };


#endif //C_COMASUPLRESPONSE_H
