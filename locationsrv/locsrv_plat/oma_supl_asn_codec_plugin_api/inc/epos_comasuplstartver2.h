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
* Description:  Header file for supl 2.0 start message
*
*/

#ifndef C_COMASuplStartVer2_H
#define C_COMASuplStartVer2_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>

#include <epos_comasuplstart.h>

#include "epos_comasuplasnmessagebase.h"
#include "epos_tomasuplqop.h"
#include "epos_tomasuplecellinfo.h"
#include "epos_suplconstants.h"
#include "epos_comasuplstart.h"

//Forward declaration
class COMASuplSETCapabilitiesVer2;
class COMASuplLocationIdVer2;
class COMASuplPosition;
/**
 *  Class for providing the location id data for suplstart message
 *
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */

class COMASuplLocationIdData: public CBase
    {
    public:  // Constructors and destructor
    
        /**
         * @Description Creates a new instance of a COMASuplLocationIdData
         *
         * @return A pointer to COMASuplLocationIdData
         * It leaves with KErrNoMemory if object creation fails        
         */
        IMPORT_C static COMASuplLocationIdData* NewL();
    
        /**
         * Destructor.
         */
        IMPORT_C ~COMASuplLocationIdData();
    
    public:
        /**
         * Setter method for Time Stamp
         */
        IMPORT_C void SetTimeStamp(const TInt32 aTime);        
        /**
         * Setter method for Serving Flag
         */
        IMPORT_C void SetServingFlag(const TBool aFlag);
        
        /**
         * Setter method for Location Id
         * @param aLocationId location id to be set in location id data
         *         ownership is transferred to COMASuplLocationIdData object
         */
        IMPORT_C void SetLocationId(COMASuplLocationIdVer2* aLocationId);
        
        /**
         * Setter method for Enhanced Cell Info
         */
        IMPORT_C void SetECellInfo(TOMASuplECellInfo aMmCellInfo);
        
        /**
         * Getter method for time stamp
         */
        IMPORT_C void GetTimeStamp(TInt32& aTime)const;        

        /**
         * getter method for serving flag
         */
        IMPORT_C void GetServingFlag(TBool& aFlag)const;

        /**
         * Getter method for location id
         * @param aLocationId Returns a reference to the location id member
         *         Ownership is transferred to the location id data object
         */
        IMPORT_C void GetLocationId(COMASuplLocationIdVer2*& aLocationId)const;
        
        /**
         * Getter method for enhanced cell info
         */
        IMPORT_C TInt GetECellInfo(TOMASuplECellInfo& aMmCellInfo)const;
        
    public:
        /**
         * Clone method for COMASuplLocationIdData
         * @return Returns a clone of Location Id data
         */
        IMPORT_C COMASuplLocationIdData* CloneL();
    private:
    
        /**
         * C++ default constructor.
         */
        COMASuplLocationIdData();
    
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();
        
    private:    
        
        TInt32 iRelativeTimeStamp;
        TBool   iServingFlag;
        COMASuplLocationIdVer2* iLocationId2;
        /*To hold E-cell id data*/
        TOMASuplECellInfo iMmCellInfo;
        TBool iECId;
    };

/**
 *  Class for providing the multiple location id 
 *
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */
class COMASuplMultipleLocIds: public CBase
    {
    public:  // Constructors and destructor
    
        /**
         * @Description Creates a new instance of a COMASuplMultipleLocIds
         *
         * @return A pointer to COMASuplMultipleLocIds
         * It leaves with KErrNoMemory if object creation fails        
         */
        IMPORT_C static COMASuplMultipleLocIds* NewL();
    
        /**
         * Destructor.
         */
        IMPORT_C ~COMASuplMultipleLocIds();
        
    public:
        /**
         * Setter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
         * @param aLocIdData the location id data to be added to the location id data array
         *        the ownership of the object is trasferred to the location id data array in the
         *       multiple location id  object
         */
        IMPORT_C void AddNextLocIdData(COMASuplLocationIdData* aLocIdData);

    public:
        /**
         * Getter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
         * @param aLocationIdDataEle the location id data elements are returned in this
         *        ownership of the  COMASuplLocationIdData pointer present in aLocationIdDataEle
         *        are retined in the original COMASuplMultipleLocIds object from whcih they were retrieved.
         *        However the ownership of the array itself is with the caller.
         */
        IMPORT_C void GetLocIdDataArray(RPointerArray<COMASuplLocationIdData>& aLocationIdDataEle);
        
        
        /**
         * Clone Method for Mupltiple Loc Ids
         * @return returns a clone of the multiple location id object 
         */
        IMPORT_C COMASuplMultipleLocIds* CloneL();
    private:
    
        /**
         * C++ default constructor.
         */
        COMASuplMultipleLocIds();
    
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();
        
    private:    
        
        RPointerArray<COMASuplLocationIdData> iLocationIdDataEle;
    };

/**
 *  Class for providing the third party id 
 *
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */
class COMASuplThirdpartyId : public CBase
    {
    public:  // Constructors and destructor
    
        /**
         * @Description Creates a new instance of a COMASuplThirdpartyId
         * @since S60 S60_5.2
         * @return A pointer to COMASuplThirdpartyId
         * It leaves with KErrNoMemory if object creation fails        
         */
        IMPORT_C static COMASuplThirdpartyId* NewL();
    
        /**
         * Destructor.
         */
        IMPORT_C ~COMASuplThirdpartyId();
    
    private:
    
        /**
         * C++ default constructor.
         */
        COMASuplThirdpartyId();
    
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();
        
    public:

        /**
              Enums for tyep of third party Information           
         */
        enum TOMASuplThirdPartyInfoType
            {
            EThirdPartyUnknown = -1,
            EThirdPartyLogicalName = 0,
            EThirdPartyMSISDN,
            EThirdPartyEmail,
            EThirdPartySIPURI,
            EThirdPartyIMSPublicId,
            EThirdPartyMin,
            EThirdPartyMDN,
            EThirdPartyURI
            };

    public:
        /**
         * Setter method to set logical name
         */
        IMPORT_C void SetLogicalName(const TDesC& aLogicalName);

        /**
         * Setter method to set MSISDN
         */
        IMPORT_C void SetMSISDN(const TDesC8& aMSISDN);

        /**
         * Setter method to set EMail address
         */
        IMPORT_C void SetEmailAddress(const TDesC& aEmailAddress);

        /**
         * Setter method to set SIP URI
         */
        IMPORT_C void SetSipUri(const TDesC8& aSIPUri);

        /**
         * Setter method to set IMS public id
         */
        IMPORT_C void SetIMSPublicId(const TDesC8& aPublicId);

        /**
         * Setter method to set Min parameter
         */
        IMPORT_C void SetMinThirdPartyId(const TDesC8& aMin);
        /**
         * Setter method to set Mdn parameter
         */
        IMPORT_C void SetMdnThirdPartyId(const TDesC8& aMdn);

        /**
         * Setter method to set URI
         */
        IMPORT_C void SetURI(const TDesC8& aURI);


    public:
        /**
         * Getter method to get logical name
         */
        IMPORT_C void GetLogicalName(TDes& aLogicalName);

        /**
         * Getter method to Get MSISDN
         */
        IMPORT_C void GetMSISDN(TDes8& aMSISDN);

        /**
         * Getter method to Get EMail address
         */
        IMPORT_C void GetEmailAddress(TDes& aEmailAddress);

        /**
         * Getter method to Get SIP URI
         */
        IMPORT_C void GetSipUri(TDes8& aSIPUri);

        /**
         * Getter method to Get IMS public id
         */
        IMPORT_C void GetIMSPublicId(TDes8& aPublicId);

        /**
         * Getter method to Get Min parameter
         */
        IMPORT_C void GetMinThirdPartyId(TDes8& aMin);
        /**
         * Getter method to Get Mdn parameter
         */
        IMPORT_C void GetMdnThirdPartyId(TDes8& aMdn);

        /**
         * Getter method to Get URI
         */
        IMPORT_C void GetURI(TDes8& aURI);

        /**
         * Getter method to return the type of third party id present
         */
        IMPORT_C TOMASuplThirdPartyInfoType GetType();
        
        /**
         * Clone Method for COMASuplThirdpartyId
         */ 
        IMPORT_C COMASuplThirdpartyId* CloneL();
    private: 
        
        TOMASuplThirdPartyInfoType iType;
        HBufC16*    iLogicalName;
        TBuf8<KThirdPartyOctetLen> iMSISDN;
        HBufC16*    iEMailAddress;
        HBufC8*      iSipUri;
        HBufC8*     iIMSPublicId;
        TBuf8<KThirdPartyMinLen> iMin;
        TBuf8<KThirdPartyOctetLen> iMdn;
        HBufC8*     iURI;

    };
/**
 *  Class for providing the third party id data set 
 *
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */
class COMASuplThirdPartyIds:public CBase
    {

public:  // Constructors and destructor
    
    /**
     * @Description Creates a new instance of a COMASuplThirdPartyIds
     * @since S60 S60_5.2
     * @return A pointer to COMASuplThirdPartyIds
     * It leaves with KErrNoMemory if object creation fails        
     */
    IMPORT_C static COMASuplThirdPartyIds* NewL();

    /**
     * Destructor.
     */
    IMPORT_C ~COMASuplThirdPartyIds();

    /**
     * Clone Method
     */
    IMPORT_C COMASuplThirdPartyIds* CloneL(); 
private:

    /**
     * C++ default constructor.
     */
    COMASuplThirdPartyIds();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();
    
public:
    
    /**
     * Setter method to set COMASuplThirdpartyId in the COMASuplThirdPartyIds object 
     * @param aThirdPartyId the third party id data to be added to the third party id 
     *         data array.The ownership of the object that is set is trasferred to the 
     *         COMASuplThirdpartyId data array in the third party id  object
     */

    IMPORT_C void AddThirdPartyId(COMASuplThirdpartyId* aThirdPartyId);
    
public:
    /**
     * Getter method to set COMASuplThirdpartyId in the COMASuplThirdPartyIds object
     * @param aThirdpartyArr the third party id data elements are returned in this
     *        ownership of the  COMASuplThirdpartyId pointer present in aThirdpartyArr
     *        are retained in the original COMASuplThirdpartyIds object from which they were retrieved.
     *        However the ownership of the array itself is with the caller.
     */

    /**
     * Getter method to set COMASuplThirdpartyId in the Version 2 SUPL Start Extension
     */
    IMPORT_C void GetThirdPartyIds(RPointerArray<COMASuplThirdpartyId>& aThirdpartyArr)const;

private:    
    RPointerArray<COMASuplThirdpartyId> iThirdpartyArr;
    };
/**
 *  Class for providing the SUPL v2.0 extension a for suplstart message
 *
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */
class COMASuplVer2SuplStartExtn : public CBase
    {
    public:  // Constructors and destructor
        
        /**
         * @Description Creates a new instance of a COMASuplVer2SuplStartExtn
         * @since S60 S60_5.2
         * @return A pointer to COMASuplVer2SuplStartExtn
         * It leaves with KErrNoMemory if object creation fails        
         */
        IMPORT_C static COMASuplVer2SuplStartExtn* NewL();
    
        /**
         * Destructor.
         */
        IMPORT_C ~COMASuplVer2SuplStartExtn();
    
    private:
    
        /**
         * C++ default constructor.
         */
        COMASuplVer2SuplStartExtn();
    
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();
        
    public:
        /**
         * Setter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
         * @since S60 S60_5.2
         * @param aMultipleLocIds multiple location id to be set in supl start message
         *        ownership is transferred to the SUPL Start message.
         */
        IMPORT_C void SetMultipleLocIds(COMASuplMultipleLocIds* aMultipleLocIds);

        /**
         * Setter method to set COMASuplThirdpartyId in the Version 2 SUPL Start Extension
         * ownership is transferred to the SUPL Start extension
         * @since S60 S60_5.2
         */
        IMPORT_C void SetThirdParty(COMASuplThirdPartyIds* aThirdParty);

        /**
         * Setter method to set COMASuplPosition in the Version 2 SUPL Start Extension
         * Ownership of the set object is transferred to the SUPL Start extension message
         * @since S60 S60_5.2
         */
        IMPORT_C void SetPosition(COMASuplPosition* aPosition);
        
    public:
        /**
         * Getter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
         * @since S60 S60_5.2
         * @param aMultipleLocIds a reference to the multiple location id member is returned in this
         *        Ownership of the returned object is retained in the SUPL Start Extension object
         */
        IMPORT_C void GetMultipleLocIds(COMASuplMultipleLocIds*& aMultipleLocIds)const;

        /**
         * Getter method to set COMASuplThirdpartyId in the Version 2 SUPL Start Extension
         * @since S60 S60_5.2
         * @param aThirdParty a reference to the third party id member is returned in this
         *        Ownership of the returned object is retained in the SUPL Start Extension object
         */
        IMPORT_C void GetThirdParty(COMASuplThirdPartyIds*& aThirdParty)const;

        /**
         * Getter method to set COMASuplPosition in the Version 2 SUPL Start Extension
         * @param aPosition a reference to the position member is returned in this
         *        Ownership of the returned object is retained in the SUPL Start Extension object
         */
        IMPORT_C void GetPosition(COMASuplPosition*& aPosition)const;
        
        /**
         * Clone method for COMASuplVer2SuplStartExtn
         */
        IMPORT_C COMASuplVer2SuplStartExtn* CloneL();
    private:    
        
        COMASuplMultipleLocIds* iMultipleLocIds;
        
        COMASuplThirdPartyIds* iThirdparty;
        
        COMASuplPosition* iPosition;
    };

/**
 *  Class for providing the suplstart message
 *  For holding the Supl Start message and for encoding the supl start message 
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_5.2
 */

class COMASuplStartVer2  : public COMASuplStart
 {
    public:  // Constructors and destructor
        
        /**
        * Two phased constructor
        * @since S60 S60_5.2
        * @param None
        * @return COMASuplStartVer2 pointer to SuplStart Extentiosn Message
        */         
    
	    IMPORT_C static COMASuplStartVer2* NewL();
    
    	/**
        * Destructor
        * @since S60 S60_5.2
        * @param None
        * @return None
        */  
     	IMPORT_C ~COMASuplStartVer2();
                
    public:
	    /**
        * Getter method for SuplStart2
        * @since S60 S60_5.2
        * @param aSuplSETCaps SetCapabilities
        * @param aSuplLocationId LocationID
        * @return None
        */
	 	IMPORT_C void SuplStart(COMASuplSETCapabilitiesVer2*& aSuplSETCaps,
						  		COMASuplLocationIdVer2*& aSuplLocationId,
						  		TBool& aeCID)const;
        
        /**
        * Setter method for Supl start
        * @since S60 S60_5.2
        * @param aSuplSETCaps SetCapabilities
        * @param aSuplLocationId Location Id
        * @return Returns None
        */
		
		IMPORT_C void SetSuplStart(COMASuplSETCapabilitiesVer2* aSuplSETCaps,
						  		   COMASuplLocationIdVer2* aSuplLocationId,
						  		   TBool aeCID);
		        
		
   	public: 
      	 
         /**
          * Setter method for Version 2 extension in Supl start
          * @since S60 S60_5.2
          * @param aVer2Extn,COMASuplVer2SuplStartExtn to be set
          * @return None
          */
         IMPORT_C void SetVer2SuplStartExtn(COMASuplVer2SuplStartExtn* aVer2Extn);
         
         
         /**
          * Getter method for Version 2 extension in Supl start
          * @since S60 S60_5.2
          * @param aVer2Extn,COMASuplVer2SuplStartExtn returned
          * @return None
          */
         IMPORT_C void GetVer2SuplStartExtn(COMASuplVer2SuplStartExtn*& aVer2Extn);
         
         /**
          * Method to determine if SUPL Start extension is present in the message or not
          */
         IMPORT_C TBool IsVer2ExtnPresent();
         
         /**
          * Cloning of SuplStart
          * @since S60 S60_5.2
          * @param aStart Base pointer
          * @param aErrorCode error code
          * @return Returns None
          */
         IMPORT_C void Clone(const COMASuplAsnMessageBase* aStart,TInt& aErrorCode);
         
   	protected: 
      
      IMPORT_C void ConstructL();        
      
      IMPORT_C COMASuplStartVer2(); 
      
	  
    protected:    // Data
    
	/* SetCapabilities*/
    COMASuplSETCapabilitiesVer2* iSETCapabilities2; 
    
    /* Location Id*/
    COMASuplLocationIdVer2* iLocationId2;
    
    /*Extension for Supl start*/
    COMASuplVer2SuplStartExtn* iVer2Extn;
    
    TBool iIsV2ExtnPresent;
    };


#endif //C_COMASuplStartVer2_H
