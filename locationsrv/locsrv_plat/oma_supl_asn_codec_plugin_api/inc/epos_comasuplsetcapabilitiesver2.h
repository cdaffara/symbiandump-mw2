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
* Description:  Header file for supl set caps ver2 message
*
*/
#ifndef C_COMASuplSETCapabilitiesVer2_H
#define C_COMASuplSETCapabilitiesVer2_H

#include <e32def.h>
#include <e32std.h>


#include "lbs/epos_comasuplsetcapabilities.h"
#include "lbs/epos_tomasuplposprotocolversion.h"

const TInt K3GPP2SupPosProtoVerElem = 8;


/**
 * @Description Class to hold the SUPL v2.0 extension to Pos Technolgy
 *
 * @since S60 S60_5.2
 */
class TOMASuplVer2PosTechExt
    {

    public: 
    /**
     * Constructor for  TOMASuplVer2PosTechExt
     */
    IMPORT_C TOMASuplVer2PosTechExt();

   
    private :      // Positioning Technologies



    };

/**
 * @Description Class to hold supported 3GPP2 pos protocol version
 *
 * @since S60 S60_5.2
 */
class TOMASuplSupported3GPP2PosProtoVer
    {

    public: 

    IMPORT_C TOMASuplSupported3GPP2PosProtoVer();


    public: //Getter Methods

    /**
     * @Description GetTOMASuplPosProtocolVersion3GPP 
     */  
    IMPORT_C void GetTOMASuplSupported3GPP2PosProtoVer(TUint16& aRevNum,TInt& aPointReleaseNumber,TInt& aInternalEditLevel) const;


    public:     //Setter Methods

    /**
     * @Description SetTOMASuplPosProtocolVersion3GPP
     */  
    IMPORT_C void SetTOMASuplSupported3GPP2PosProtoVer(TUint16 aRevNumber,TInt aPointReleaseNumber,TInt aInternalEditLevel);

    private :      // Positioning Technologies

    TUint16     iRevNumber;
    TInt        iPointReleaseNumber;
    TInt        iInternalEditLevel;

    };

/**
 * @Description TOMASuplPosProtocolVersion3GPP
 *
 * @since S60 S60_5.2
 */
class TOMASuplPosProtocolVersion3GPP2
    {

    public: 

    IMPORT_C TOMASuplPosProtocolVersion3GPP2();


    public: //Getter Methods

    /**
     * @Description GetTOMASuplPosProtocolVersion3GPP 
     */  
    IMPORT_C void GetTOMASuplPosProtocolVersion3GPP2(RArray<TOMASuplSupported3GPP2PosProtoVer>& aSupProtoVer) const;


    public:     //Setter Methods

    /**
     * @Description SetTOMASuplPosProtocolVersion3GPP
     */  
    IMPORT_C void SetTOMASuplPosProtocolVersion3GPP2(TOMASuplSupported3GPP2PosProtoVer aVersion);

    private :      // Positioning Technologies

    //TOMASuplSupported3GPP2PosProtoVer iSupProtoVer[K3GPP2SupPosProtoVerElem];
    RArray<TOMASuplSupported3GPP2PosProtoVer> iSupProtoVer;

    };

/**
 * @Description Class to hold SUPL 2.0 extension to pos protocol
 *
 * @since S60 S60_5.2
 */
class TOMASuplVer2ProtocolExt
    {

    public: 
    /**
     * Constructor for TOMASuplVer2ProtocolExt
     */
    IMPORT_C TOMASuplVer2ProtocolExt();


    public: //Getter Methods

    /**
     * @Description Getter method for pos protocol extn
     */  
    IMPORT_C void GetTOMASuplVer2ProtocolExt(TOMASuplPosProtocolVersion& aRRLPVersion,TOMASuplPosProtocolVersion& aRRCVersion,TOMASuplPosProtocolVersion3GPP2& aTIA801Version) const;


    public:     //Setter Methods

    /**
     * @Description Setter method for pos protocol extn
     */  
    IMPORT_C void SetTOMASuplVer2ProtocolExt(TOMASuplPosProtocolVersion aRRLPVersion,TOMASuplPosProtocolVersion aRRCVersion,TOMASuplPosProtocolVersion3GPP2 aTIA801Version);

    private :      // Positioning Technologies

    TOMASuplPosProtocolVersion  iPosProtocolVersionRRLP;
    TOMASuplPosProtocolVersion  iPosProtocolVersionRRC;
    TOMASuplPosProtocolVersion3GPP2 iPosProtocolVersionTIA801;
    };

/**
* @Description TOMASuplPosTechnology2 describes the positioning method extension for SUPL v2.0 . Possible values for POS technology are 
* - A-GPS SET Assisted only
* - A-GPS SET Based only
* - Autonomous GPS
* - AFLT (Advanced Forward Link Trilateration)
* - EOTD (Enhanced Observed Time Difference)
* - OTDOA (Observed Time Difference of Arrival)
*
*  @since S60 S60_5.2
*/
class TOMASuplPosTechnology2 : public TOMASuplPosTechnology
{

public: 
        /**
         * Constructor for TOMASuplPosTechnology2
         */
     	IMPORT_C TOMASuplPosTechnology2();

     
public:	//Getter Methods
		/**
		 * Getter method for Version 2 Pos technology extension
		 * @since S60 S60_5.2
		 * @param aExtn The TOMASuplVer2PosTechExt value is returned in this
		 * @return KErrNone if the value is set and retrieved successfully
		 *         KErrOMASuplParamNotSet otherwise
		 */
        IMPORT_C TInt GetVer2PosExtn(TOMASuplVer2PosTechExt& aExtn) const;
public:		//Setter Methods
		/**
		 * Setter method for  Version 2 Pos technology extension
		 */
		IMPORT_C void SetVer2PosExtn(TOMASuplVer2PosTechExt aExtn);
		
private :	   // Positioning Technologies

        TBool iIsVer2ExtSet;
		TOMASuplVer2PosTechExt iVer2Ext;
	
};

// End of TPosTechnology

/**
* @Description TOMASuplPosProtocol determines the underlying control plane
* positioning protocol. The different possible values for the positioning protocol
* are 
* - TIA 801 (Telecommunication Industry Association 801)
* - RRLP (Radio Resource LCS Protocol)
* - RRC (Radio Resource Control)
*
*  @since S60 S60_5.2
**/
class TOMASuplPosProtocol2: public TOMASuplPosProtocol
{

public: // Construction
    /**
     * Constructor for TOMASuplPosProtocol2
     */
     IMPORT_C TOMASuplPosProtocol2();
     

public:	//Getter Methods
	/**
	 * Getter method for Posprotocol extension
	 */
	    IMPORT_C TInt GetVer2PosProtocolExtn(TOMASuplVer2ProtocolExt& aVer2Extn)const;
	    
public: // Setter Methods
	  /**
	   * Setter method for Posprotocol extension
	   */
	  IMPORT_C void SetVer2PosProtoExtn(TOMASuplVer2ProtocolExt aVer2Extn);
	  
private :	//Data members , Positioning protocols

	TBool iIsVer2ExtnSet;
	TOMASuplVer2ProtocolExt iVer2Ext;
	
};
// End of TPosProtocol2

/**
 * Class to hold type of service supported
 */
class TOMASuplServiceSupported
    {
public:
    /**
     * Getter method for periodic trigger support
     */
    IMPORT_C void GetPeriodicTrigger(TBool &aPeriodicTrigger)const;
    /**
     * Getter method for area event trigger support
     */
    IMPORT_C void GetAreaEventTrigger(TBool &aAreaEventTrigger)const;
    
public:
    /**
     * Setter method for periodic trigger support
     */
    IMPORT_C void SetPeriodicTrigger(const TBool aPeriodicTrigger);
    /**
     * Setter method for area even trigger support
     */
    IMPORT_C void SetAreaEventTrigger(const TBool aAreaEventTrigger);
    
private:
    TBool   iPeriodicTrigger;
    TBool   iAreaEventTrigger;
    };


class TOMASuplGeoAreaShapeSupported
    {
public:
    void GetElipticalArea(TBool &aElipticalArea)const;
    void GetPolygonalArea(TBool &aPolygonalArea)const;

public:
    void SetElipticalArea(const TBool aElipticalArea);
    void SetPolygonalArea(const TBool aPolygonalArea);

private:
    TBool   iElipticalArea;
    TBool   iPolygonalArea;
    };


class TOMASuplEventTriggerCapabilities
    {
public:
    void GetGeoAreaSupp(TOMASuplGeoAreaShapeSupported &aGeoAreaSup)const;
    void GetMaxNumGeoAreaSupported(TInt8 &aMaxNumGeoAreaSupp)const;
    void GetMaxAreaIdListSupported(TInt8 &aMaxAreaIdListSupp)const;
    void GetMaxAreaIdSupported(TInt &aMaxAreaIdSupp)const;

public:
    void SetGeoAreaSupp(const TOMASuplGeoAreaShapeSupported aGeoAreaSup);
    void SetMaxNumGeoAreaSupported(const TInt8 aMaxNumGeoAreaSupp);
    void SetMaxAreaIdListSupported(const TInt8 aMaxAreaIdListSupp);
    void SetMaxAreaIdSupported(const TInt aMaxAreaIdSupp);

private:
    TOMASuplGeoAreaShapeSupported iGeoAreaSupp;
    TInt8  iMaxNumGeoAreaSupported;
    TInt8  iMaxAreaIdListSupported;
    TInt   iMaxAreaIdSupported;
    };

class TOMASuplSessionCapabilities
    {
public:
		IMPORT_C TOMASuplSessionCapabilities();
    IMPORT_C TInt GetMaxNumberTotalSessions(TInt &aMaxNumberTotalSessions)const;
    IMPORT_C TInt GetMaxNumberPeriodicSessions(TInt8 &aMaxNumberPeriodicSessions)const;
    IMPORT_C TInt GetMaxNumberTriggeredSessions(TInt8 &aMaxNumberTriggeredSessions)const;
public:
    IMPORT_C void SetMaxNumberTotalSessions(const TInt aMaxNumberTotalSessions);
    IMPORT_C void SetMaxNumberPeriodicSessions(const TInt8 aMaxNumberPeriodicSessions);
    IMPORT_C void SetMaxNumberTriggeredSessions(const TInt8 aMaxNumberTriggeredSessions);

private:
    TInt iOptionalMask;
    TInt    iMaxNumberTotalSessions;
    TInt8   iMaxNumberPeriodicSessions;
    TInt8   iMaxNumberTriggeredSessions;
    };

class TOMASuplReportMode
    {
public:
    IMPORT_C  TOMASuplReportMode();
    IMPORT_C void GetRealTime(TBool &aRealTime)const;
    IMPORT_C void GetQuasiRealTime(TBool &aQuasiRealTime)const;
    IMPORT_C void GetBatch(TBool &aBatch)const;
public:
    IMPORT_C void SetRealTime(const TBool aRealTime);
    IMPORT_C void SetQuasiRealTime(const TBool aQuasiRealTime);
    IMPORT_C void SetBatch(const TBool aBatch);
private:
    TBool   iRealTime;
    TBool   iQuasiRealTime;
    TBool   iBatch;
    };

class TOMASuplBatchReportMode
    {
public:
    IMPORT_C TOMASuplBatchReportMode();
    IMPORT_C TInt GetReportPosition(TBool &aReportPosition)const;
    IMPORT_C TInt GetReportMeasurement(TBool &aReportMeasurement)const;
    IMPORT_C TInt GetMaxPositions(TInt &aMaxPositions)const;
    IMPORT_C TInt GetMaxMeasurements(TInt &aMaxMeasurements)const;
public:
    IMPORT_C void SetReportPosition(const TBool aReportPosition);
    IMPORT_C void SetReportMeasurement(const TBool aReportMeasurement);
    IMPORT_C void SetMaxPositions(const TInt aMaxPositions);
    IMPORT_C void SetMaxMeasurements(const TInt aMaxMeasurements);
private:
    TBool   iReportPosition;
    TBool   iReportMeasurements;
    TInt    iMaxPositions;
    TInt    iMaxMeasurements;
    
    //iReportPosition = 3|iReportMaxMeasurements = 2 | iMaxPositions = 1|iMaxMeasurements
    TInt iOptionalMask;
    };


class TOMASuplReportingCap
    {
public:
    IMPORT_C TOMASuplReportingCap();
    IMPORT_C TInt GetReportMode(TOMASuplReportMode &aReportMode)const;
    IMPORT_C TInt GetBatchReportMode(TOMASuplBatchReportMode &aBatchReportMode)const;

public:
    IMPORT_C void SetReportMode(const TOMASuplReportMode aReportMode);
    IMPORT_C void SetBatchReportMode(const TOMASuplBatchReportMode aBatchReportMode);

private:
    TOMASuplReportMode    iReportMode;
    TOMASuplBatchReportMode    iBatchRepMode;
    //iReportMode|iBatchRepMode
    TInt iOptionalMask;
    };

class TOMAServiceCapabilities
    {
public:
    IMPORT_C TOMAServiceCapabilities();
    IMPORT_C TInt GetSuplServiceSupported(TOMASuplServiceSupported& aServicesSupported)const;
    IMPORT_C TInt GetSuplReportingCap(TOMASuplReportingCap& aReportingCapabilities)const;
    IMPORT_C TInt GetEventTriggerCapabilities(TOMASuplEventTriggerCapabilities& aEventTriggerCapabilities)const;
    IMPORT_C TInt GetSuplSessionCapabilities(TOMASuplSessionCapabilities& aSessionCapabilities)const;
public:
    IMPORT_C void SetSuplServiceSupported(const TOMASuplServiceSupported aServicesSupported);
    IMPORT_C void SetSuplReportingCap(const TOMASuplReportingCap aReportingCapabilities);
    IMPORT_C void SetEventTriggerCapabilities(const TOMASuplEventTriggerCapabilities aEventTriggerCapabilities);
    IMPORT_C void SetSuplSessionCapabilities(const TOMASuplSessionCapabilities aSessionCapabilities);

private:

    TOMASuplReportingCap iReportingCapabilities;
    TOMASuplServiceSupported iServicesSupported;
    TOMASuplEventTriggerCapabilities iEventTriggerCapabilities;
    TOMASuplSessionCapabilities iSessionCapabilities;
    //reportingcaps|servicesupp|eventtrigparams|sessioncaps
    TInt iOptionalMask;
    };

/**
 * 
 * Class to hold SUPL 2.0 extension to SET Capabilities
 */
class TOMAVer2SetCapExtn
    {
public:
    /**
     * constructor for TOMAVer2SetCapExtn
     */
    IMPORT_C TOMAVer2SetCapExtn();
    /**
     * Getter method for Service caps
     */
    IMPORT_C TInt GetServiceCaps(TOMAServiceCapabilities& aServiceCaps)const;
    /**
     *Setter methods for Service Caps 
     */
    IMPORT_C void SetServiceCaps(const TOMAServiceCapabilities aServiceCaps);
    
private:
    TBool iIsServiceCapSet;
    TOMAServiceCapabilities iServiceCaps;
    
    };
/**
* @Description COMASuplSETCapabilitiesVer2 defines the SUPL v2.0 extn to SET capabilities of the SUPL Enabled Terminal.
* SET capabilities are defined in terms of
* - POS Technology
* - Preferred Method and
* - POS Protocol
*
* SUPL Framework retrieves the SET Capabilities for SUPL START and SUPL POS INIT messages
*
*  @since S60 S60_5.2
**/

class COMASuplSETCapabilitiesVer2 : public CBase//: public COMASuplSETCapabilities //Manish
{
	public:  // Constructors and destructor
	    /**
	     * SUPL 2.0 exetension for  Preferred Method 
	     */
	    enum TOMASuplPrefMethod2
	        {
	        EOMAPrefMethodUnknown = -1,
	        EOMAAGpsSETAssitedPreferred = 0,
	        EOMAAGpsSETBasedPreferred = 1,
	        EOMANoPreferred = 2,
	        };
				 
     	/**
        * @Description Creates a new instance of a COMASuplSETCapabilities and sets 
        * the type in COMASuplInfoRequest to EOMASuplSETCapabilities.
        *
        * @return A pointer to COMASuplSETCapabilities. It leaves with KErrNoMemory if object creation fails        
        */
        IMPORT_C static COMASuplSETCapabilitiesVer2* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C  ~COMASuplSETCapabilitiesVer2();
        
	public:

	    /**
	     * @Description GetVer2SetCapExtn is used to retrieve the Version 2 extension
	     * of Set Capabilities.
	     * @param aCapExtn A Reference Parameter of type TOMAVer2SetCapExtn
	     * @return KErrNone if parameter is set and retrieved successfully
	     *         KErrOMASuplParameterNotSet if this paramter is not set
	     */ 
	    IMPORT_C TInt GetVer2SetCapExtn(TOMAVer2SetCapExtn& aCapExtn)const;
	    
	    /**
	     * @Description IsVer2SetCapExtnPresent is used to check if the 
	     * Version 2 extension of Set Capabilities is set or not
	     * 
	     * @return ETrue if parameter is set 
	     *         EFalse if this paramter is not set
	     */ 
	    IMPORT_C TBool IsVer2SetCapExtnPresent()const;

	public:

	    /**
	     * @Description SetVer2SetCapExtn is used to set the Version 2 extension
	     * of Set Capabilities.
	     * @param aCapExtn A  Parameter of type TOMAVer2SetCapExtn to be set
	     */ 
	    IMPORT_C void SetVer2SetCapExtn(const TOMAVer2SetCapExtn aCapExtn);
	    
	public:
	    
	   /**
	    * @Description GetSETCapabilities is invoked by SUPL Framework to retrive the 
	    * capabilities of the SET. 
	    * @param aPosTechnology A Reference Parameter of type TOMASuplPosTechnology
	    * @param aPrefMethod A Reference Parameter of type TOMASuplPrefMethod
	    * @param aPosProtocol A Reference Parameter of type TOMASuplPosProtocol 
	    */  

	IMPORT_C void GetSETCapabilities(TOMASuplPosTechnology2& aPosTechnology,
				  				 	  TOMASuplPrefMethod2&    aPrefMethod,
				  				 	  TOMASuplPosProtocol2&   aPosProtocol) const;

	   /**
	    * @Description SetSETCapabilities is invoked by POS Message plug-in to set the capabilities
	    * supported by the POS Module.
	    * @param aPosTechnology A parameter of type TOMASuplPosTechnology 
	    * that sets values for POS Technology
	    * @param aPrefMethod A parameter of type TOMASuplPrefMethod 
	    * that sets values for Preferred Method
	    * @param aPosProtocol A parameter of type TOMASuplPosProtocol 
	    * that sets values for POS Protocol
	    */  
	    IMPORT_C void SetSETCapabilities(const TOMASuplPosTechnology2& aPosTechnology,
					  				 	 const TOMASuplPrefMethod2&    aPrefMethod,
					  				 	 const TOMASuplPosProtocol2&   aPosProtocol);
	public: //From Base Class
		
	    /**
	     * @Description Clone Method
	     * @return a pionter to clone of type COMASuplSETCapabilitiesVer2 
	     */
	    IMPORT_C COMASuplSETCapabilitiesVer2* CloneL();
	
	protected:
	
	// Default Constructor
	IMPORT_C COMASuplSETCapabilitiesVer2();
	//Two phase construction
	IMPORT_C void ConstructL();
	
	private:
    // Pos Tec
	TOMASuplPosTechnology2 iPosTechnology; //mandatory parameter
	TOMASuplPosProtocol2   iPosProtocol; //mandatory parameter
	TOMASuplPrefMethod2    iPrefMethod;//mandatory parameter
  	TOMAVer2SetCapExtn    iVer2SetCapExtn; //optional parameter
  	TBool    iIsVer2SetCapExtnSet;
	};



#endif //  _SUPLSETCAPABILITIES2_H
