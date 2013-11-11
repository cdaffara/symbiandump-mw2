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
* Description:  Header file for location id information
*
*/

#ifndef C_COMASUPLLOCATIONID_H
#define C_COMASUPLLOCATIONID_H

#include<e32def.h>
#include<e32std.h>
#include<e32base.h>


/**
 *  Class containing CellInforamtion
 *  For holding the cell information, base class for GSM cell Information
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */
class COMASuplCellInfo : public CBase
    {
    	
public:
	/**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return COMASuplCellInfo pointer to location information
        */   
	IMPORT_C static COMASuplCellInfo* NewL();
    
    /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
    IMPORT_C virtual ~COMASuplCellInfo();

public:

        /**
        * Getter method for cellInfo
        * @since S60 S60_3.1u
        * @param aRefMNC RefMNC Network country code
        * @param aRefMcc Mobile country code
        * @param aRefCI cell identity
        * @return Returns None
        */
        
    IMPORT_C void SuplCellInfo(TInt& aRefMNC,TInt& aRefMcc,TInt& aRefCI) const ;

		/**
        * Setter method for cellInfo
        * @since S60 S60_3.1u
        * @param aRefMNC RefMNC Network country code
        * @param aRefMcc Mobile country code
        * @param aRefCI cell identity
        * @return Returns None
        */ 
    IMPORT_C void SetSuplCellInfo(TInt aRefMNC,TInt aRefMcc,TInt aRefCI);

    /**
     * Getter method for cell identity
     * @since S60 S60_3.1u
     * @return Returns The cell identity
     */ 
    IMPORT_C TInt GetCellId();
    
protected:
    //Deafult Constructor
    IMPORT_C COMASuplCellInfo();

private: // Construction
	 void ConstructL();

protected:
	//Network country code
	TInt iRefMNC;
	//Mobile country code
	TInt iRefMCC;
	//Cell identity
	TInt iRefCI;
	};
// End of class COMASuplCellInfo
	
/**
 *  Class containing GSM CellInforamtion
 *  For holding the GSM cell information
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */
 
class COMASuplGSMCellInfo: public COMASuplCellInfo
    {
    
public:
	/**
	* Performs construction of an object
	* @since S60 S60_3.1u
	* @param None
	* @return Returns instance of COMASuplGSMCellInfo
	*/

	IMPORT_C static COMASuplGSMCellInfo* NewL();
    
   /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
    IMPORT_C virtual ~COMASuplGSMCellInfo();


public: 
        /**
        * Getter method for GSM cellInfo
        * @since S60 S60_3.1u
        * @param aRefMNC RefMNC Network country code
        * @param aRefMcc Mobile country code
        * @param aRefCI cell identity
        * @param aRefLAC Location Area Code
        * @return Returns None
        */
        
    IMPORT_C void SuplGSMCellInfo(TInt& aRefMNC,TInt& aRefMcc,TInt& aRefCI,TInt& aRefLAC) const ;

		/**
        * Setter method for GSM cellInfo
        * @since S60 S60_3.1u
        * @param aRefMNC RefMNC Network country code
        * @param aRefMcc Mobile country code
        * @param aRefCI cell identity
        * @param aRefLAC Location Area Code
        * @return Returns None
        */    
    IMPORT_C void SetSuplGSMCellInfo(TInt aRefMNC,TInt aRefMcc,TInt aRefCI,TInt aRefLAC);
     
private: // Construction
	
	//Deafult Constructor
	IMPORT_C COMASuplGSMCellInfo();
	/**
    * Two-phased constructor.
    */	
	 void ConstructL();
  

private :// Data Members
	
	//Location Area Code
	TInt iRefLAC;
    
    };
// End of class COMASuplGSMCellInfo

/**
 *  Class containing location information
 *  
 *  @lib eposomasuplasnconverter.lib
 *  @since S60 S60_3.1u
 */

class COMASuplLocationId : public CBase
{	

	 public:
	 /**
	  Enums for tyep of cell Information           
	 */
	enum TOMASuplCellInfoType
		{
			EGSM = 0,
			EWCDMA = 1,
			EWLAN
		};
	 /**
		 * Description: Enums for status of location id information
		 */
		enum TOMASuplStatus
		{
			EStale = 0,
			ECurrent = 1,
			EUnknown 
		};
	public:
	/**
        * Two phased constructor
        * @since S60 S60_3.1u
        * @param None
        * @return instance of COMASuplLocationId
        */ 

	IMPORT_C static COMASuplLocationId* NewL();
    
     /**
        * Destructor
        * @since S60 S60_3.1u
        * @param None
        * @return None
        */
     IMPORT_C ~COMASuplLocationId();


public:	
        /**
        * Getter method for Location Id
        * @since S60 S60_3.1u
        * @param aCellInfo GSMCellInfo
        * @param aStatus Status of information
        * @return error code if param not set
        */

		IMPORT_C TInt SuplLocationId(COMASuplCellInfo*& aCellInfo,TOMASuplStatus& aStatus) const;	

		/**
        * Getter method for Location Id
        * @since S60 S60_3.1u
        * @param aCellInfo GSMCellInfo
        * @param aStatus Status of information
        * @return error code if param not set
        */

		IMPORT_C TInt SuplLocationId(COMASuplGSMCellInfo*& aCellInfo,TOMASuplStatus& aStatus) const;
        
        /**
        * Getter method for type of location information
        * @since S60 S60_3.1u
        * @param None
        * @return Returns the type of cellinfo i.e. GSM or WCDMA
        */
		IMPORT_C TOMASuplCellInfoType SuplCellInfoType()const;
	
public:
      	/**
        * Setter method for Location Id
        * @since S60 S60_3.1u
        * @param GSMCellInfo - The ownsership of cellinfo is passed to COMASuplLocationId
        * @param aStatus Status of information
        * @return Returns None
        */

		IMPORT_C void SetSuplLocationId(COMASuplCellInfo* aCellInfo,TOMASuplStatus& aStatus);	

		/**
        * Getter method for Location Id
        * @since S60 S60_3.1u
        * @param GSMCellInfo - The ownsership of cellinfo is passed to COMASuplLocationId
        * @param aStatus Status of information
        * @return Returns None
        */

		IMPORT_C void SetSuplLocationId(COMASuplGSMCellInfo* aCellInfo,TOMASuplStatus& aStatus);
	
		/**
		 * Getter method for Cell Id
		 * @since S60 S60_3.1u
		 * @param none
		 * @return Returns The Cell Identity from Cell Info
		 */

		IMPORT_C TInt GetCellId();
		
		/**
		 * Clone Method for COMASuplLocationId
		 * 
		 */
		IMPORT_C COMASuplLocationId* CloneL();
protected: // Construction
	
	 IMPORT_C COMASuplLocationId();

	 void ConstructL();        
protected :	   
	//Status of CellInfo
	TOMASuplStatus   iStatus;
	//Type of CellInfo
	TOMASuplCellInfoType iChoice;
	//CellInfo
	COMASuplCellInfo* iCellInfo;
    };

#endif //  C_COMASUPLLOCATIONID_H
