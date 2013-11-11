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
* Description:  This class provides the the implementation for COMASuplSessionID
*				 					to provide the session IDs for Supl Sessions
*
*/

#ifndef C_COMASUPLSESSIONID_H
#define C_COMASUPLSESSIONID_H

// INCLUDES
#include <e32def.h>
#include <e32std.h>
#include <e32base.h>
#include "epos_suplconstants.h"


/**
*  SUPL SET Session Id Object 
*  This class is responsible for storing the SET id in Session Id
*  @lib eposomasuplasnconverter.lib
*  @since S60 S60_3.1u
*/

class COMASuplSETSessionID : public CBase
{
    public:
    
	  /**
	  *enum for type of Set ID
	  */
    enum TSuplSETIDType
    {
    	EIPv4Address = 0, 
    	EIPv6Address,
    	EIMSI
    };
    public:  // Constructors and destructor

   /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplSETSessionID pointer to SETSessionId 
        */   
    IMPORT_C static COMASuplSETSessionID* NewL();
    
    /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */ 
     IMPORT_C virtual ~COMASuplSETSessionID();
    
    public: // Getter Methods
     
    /**
        * SuplSETSessionID Method.
        * @since S60 S60_3.1u
        * @param aSETSessionID the SET Session ID
        * @param aIPvAddress IPvAddress
        * @return None
     */
  	IMPORT_C void SuplSETSessionID(TInt& aSETSessionID,
  							  	   TDes8& aIPvAddress)const;
   /**
        * SuplSETIDType Method.
        * @since S60 S60_3.1u
        * @param None
        * @return SET ID Type
     */
    IMPORT_C COMASuplSETSessionID::TSuplSETIDType SuplSETIDType()const;
     
  	public: //Setter Methods
  	
  	/**
        * SuplSETSessionID Method.
        * @since S60 S60_3.1u
        * @param aSETSessionID the SET Session ID
        * @param aIPvAddress IPvAddress
        * @return None
     */						  
  	IMPORT_C void SetSuplSETSessionIDIPv4(TInt aSETSessionID,
  							     const TDesC8& aIPv4Address);						        
    
    /**
        * SuplSETSessionID Method.
        * @since S60 S60_3.1u
        * @param aSETSessionID the SET Session ID
        * @param aIPv6Address IPvAddress
        * @return None
     */	
    IMPORT_C void SetSuplSETSessionIDIPv6(TInt aSETSessionID,
  							     	  const TDesC8& aIPv6Address);
    
    /**
        * SuplSETSessionID Method.
        * @since S60 S60_3.1u
        * @param aSETSessionID the SET Session ID
        * @param aIMSI IMSI
        * @return None
     */
    IMPORT_C void SetSuplSETSessionIDIMSI(TInt aSETSessionID,
  							     	  const TDesC8& aIMSI);
   	 /**
        * SetSuplSETIDType Method.
        * @since S60 S60_3.1u
        * @param SET ID Type
        * @return None
     */	
    IMPORT_C void SetSuplSETIDType(TSuplSETIDType aSuplSETIDType);
    
    /**
        * overloaded == operator
        * @since S60 S60_3.1u
        * @param aSuplSETSessionID
        * @return true or false 
     */	
    IMPORT_C TBool operator==(const COMASuplSETSessionID& aSuplSETSessionID);	
    
    /*
     * Clone Method for COMASuplSETSessionID
     */
    IMPORT_C COMASuplSETSessionID* CloneL();
    private:

     COMASuplSETSessionID();


    private:    // Data
	  /* set session id*/
	  TInt 	   			   iSETSessionID;
	  /*IPV Address*/
	  TBuf8<KIPv6Address>  iSETAddress;
	  
	  /*Type of IP address*/
	  TSuplSETIDType       iChoice;
};


/**
*  SUPL SLP Address in Session Id 
*  Object This class is responsible for storing the SLP Address  in Session Id
*  @lib eposomasuplasnconverter.lib
*  @since S60 S60_3.1u
*/

class COMASuplSLPAddress : public CBase
{
    public:
    
    /* enum for SLP Address Type*/
    enum TSuplSLPAddressType
	    {
	    	EIPv4Address = 0, 
	    	EIPv6Address = 1,
	    	EFqdn 
	    };
    public:  // Constructors and destructor
    
    /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplSLPAddress pointer to SLP address
        */   
    
	IMPORT_C static COMASuplSLPAddress* NewL();
    
    /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
    
     IMPORT_C virtual ~COMASuplSLPAddress();
	
      
    public: // New functions
    /**
	* IPv4Address Method.
	* @since S60 S60_3.1u
	* @param aIPvddress IPv4Address
	* @return Error if IPv4Address is not set
	*/
  	IMPORT_C TInt IPvAddress(TDes8& aIPvddress)const;
  
  	
  	/**
	* Fqdn Method.
	* @since S60 S60_3.1u
	* @param aFqdn Fqdn
	* @return Error if Fqdn is not set
	*/
  	IMPORT_C TInt Fqdn(HBufC8*& aFqdn)const;
  	
  	/**
	* To retrieve the type of SLP Address
	* @since S60 S60_3.1u
	* @param None
	* @return SLP Address Type
	*/
  	IMPORT_C TSuplSLPAddressType SLPAddressType()const;					  
    
    public: 
    
    /**
	* SetIPv4Address Method.
	* @since S60 S60_3.1u
	* @param aIPv4Address IPv4Address
	* @return None
	*/
  	IMPORT_C void SetIPv4Address(const TDesC8& aIPv4Address);
  
  	/**
	* SetIPv6Address Method.
	* @since S60 S60_3.1u
	* @param aIPv6Address IPv6Address
	* @return None
	*/
  	IMPORT_C void SetIPv6Address(const TDesC8& aIPv6Address);
  	
  	/**
	* SetFqdn Method.
	* @since S60 S60_3.1u
	* @param aFqdn Fqdn
	* @return None
	*/
  	IMPORT_C void SetFqdn(HBufC8* aFqdn);    
    
 	/**
	* To set the type of SLP Address
	* @since S60 S60_3.1u
	* @param SLP Address Type
	* @return None
	*/
  	IMPORT_C void SetSLPAddressType(TSuplSLPAddressType aSLPAddressType);			
  	
 	/**
	* Compare two SLP addresses
	* @since S60 S60_3.1u
	* @param aSuplSLPAddress, COMASuplSLPAddress address to be compared
	* @return TBool , ETrue if both matches else EFalse
	*/
  	IMPORT_C TBool operator==(const COMASuplSLPAddress& aSuplSLPAddress);
  	
  	/*
  	 * Clone Method for COMASuplSLPAddress
  	 */
  	
  	IMPORT_C COMASuplSLPAddress* CloneL();
    private:

    /**
    * C++ default constructor.
    */
    COMASuplSLPAddress();

    private:    // Data
	/*Type of SLP Address*/
	TSuplSLPAddressType iChoice;
	/*IP Address*/
	TBuf8<KIPv6Address>  iIPvAddress;
	/*FQDN */
	HBufC8* iFqdn;
};

/**
*  SUPL SLP Session Id Object
*  This class is responsible for storing the SLP id in Session Id
*  @lib eposomasuplasnconverter.lib
*  @since S60 S60_3.1u
*/

class COMASuplSLPSessionID : public CBase
{
    public:  // Constructors and destructor
    
	   /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplSLPSessionID pointer to SLP Session Id
        */ 
	IMPORT_C static COMASuplSLPSessionID* NewL();
    
   /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
    IMPORT_C ~COMASuplSLPSessionID();
     
    public: // New functions
        
	/**
	* SuplSLPSessionID Method.
	* @since S60 S60_3.1u
	* @param aSLPSessionID SLPSessionID 
	* @param aSLPAddress  SLP Address
	* @return None
	*/
  	IMPORT_C void SuplSLPSessionID(TDes8& aSLPSessionID,
  							       COMASuplSLPAddress*& aSLPAddress)const;
  							  
  	/**
	* SetSuplSETSessionID Method.
	* @since S60 S60_3.1u
	* @param aSLPSessionID SLPSessionID 
	* @param aSLPAddress SLP Address
	* @return None
	*/

  	IMPORT_C void SetSuplSLPSessionID(const TDesC8& aSLPSessionID,
  							     	  COMASuplSLPAddress* aSLPAddress);						        
    /**
	* Compare two SLP Session Ids
	* @since S60 S60_3.1u
	* @param aSuplSLPSessionID, COMASuplSLPSessionID slp session ids to be compared
	* @return TBool , ETrue if both matches else EFalse
	*/
    IMPORT_C TBool operator==(const COMASuplSLPSessionID& aSuplSLPSessionID);   
   
    /**
     * Clone mothod for
     */
    IMPORT_C COMASuplSLPSessionID* CloneL();
    private:
    COMASuplSLPSessionID();


    private:    // Data
	  /*SLP Session Id*/
  	TBuf8<KSLPSessionID>  iSLPSessionID;
  	/* SLP Address*/
  	COMASuplSLPAddress*     iSLPAddress;
};
            
/**
*  SUPL Session Id Object
*  This class is responsible for storing the Session Id
*  @lib eposomasuplasnconverter.lib
*  @since S60 S60_3.1u
*/

class COMASuplSessionID : public CBase
    {
    public:  // Constructors and destructor
    /**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplSessionID pointer to Session Id
        */   
    IMPORT_C static COMASuplSessionID* NewL();
    
    /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */ 
     IMPORT_C ~COMASuplSessionID();
     
          
     public: // Getter Methods
        
	/**
        * SETSessionID Method.
        * @since S60 S60_3.1u
        * @param aSETSessionID the SET Session ID
        * @return error code if parameter is not set
     */
  	IMPORT_C TInt SETSessionID(COMASuplSETSessionID*& aSETSessionID) const;
	
	/**
        * SLPSessionID Method.
        * @since S60 S60_3.1u
        * @param aSLPSessionID the SLP Session ID
        * @return error code if parameter is not set
     */  
    IMPORT_C TInt SLPSessionID(COMASuplSLPSessionID*& aSLPSessionID) const;

	public: //Setter Methods
	
	/**
        * SetSETSessionID Method.
        * @since S60 S60_3.1u
        * @param aSETSessionID the SET Session ID
        * @return None
     */
	IMPORT_C void SetSETSessionID(COMASuplSETSessionID* aSETSessionID);
	
	/**
        * SetSLPSessionID Method.
        * @since S60 S60_3.1u
        * @param aSLPSessionID the SLP Session ID
        * @return None
     */
	IMPORT_C void SetSLPSessionID(COMASuplSLPSessionID* aSLPSessionID);           
    
	/*
	 * Clone Method for COMASuplSessionId
	 */
	IMPORT_C COMASuplSessionID* CloneL();
	
    private:

    COMASuplSessionID();


	public:
	
	/* Mask for optional parameters*/
	TInt iOptionalMask;
	
  private:    // Data
    	
	/*SET Session ID*/
	COMASuplSETSessionID* iSETSessionID;
	
	/*SLP Session ID*/
	COMASuplSLPSessionID* iSLPSessionID;			    
    };
    
#endif      // C_COMASUPLSESSIONID_H


// End of File
