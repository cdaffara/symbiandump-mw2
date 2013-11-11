/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declaration of geo WCDMA cell class.
*
*/



#ifndef LBTGEOWCDMACELL_H
#define LBTGEOWCDMACELL_H

#include "lbtgeocell.h"

/**
 * Geographical WCDMA cell.
 *  
 * CLbtGeoWcdmaCell class is defined by the area covered by a WCDMA network
 *  cell.
 *  
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class TLbtWcdmaCellInfo
	{
	public:
	/**
	 * Structure of cell measurements
	 */ 
	struct TLbtCellMeasuredResult
		{
		TInt16 iEcNo;
		TInt16 iRscp;
		};
	/**
	 * Structure for single WCDMA cell information
	 */
	struct TLbtWcdmaCell
		{
		TInt8	iCarrierRssi; /// The carrier radio signal strength indication
		TInt8	iRscp;		  /// The received signal code power for the common pilot channel.
		TInt8	iEcNo;	      /// The common pilot channel energy per chip divided by the noise density.
		TInt 	iPathLoss;	  /// The cell signal path loss
		};
	
	/**
	 * An WCDMA cell information along with neighboring cell 
	 * measurement result
	 *
	 */
	public:
		TLbtWcdmaCell					iCell;
		RArray<TLbtCellMeasuredResult>	iMeasurementResult;
		
		IMPORT_C ~TLbtWcdmaCellInfo();
	};
 
class CLbtGeoWcdmaCell : public CLbtGeoCell
    {	
public:
    /**
     * Allocates and constructs a new geographical WCDMA cellular area object.
     * In the returned object,the country code,network code,location area code and  
     * cell id are set to zero.
     * @return Pointer to the new geographical cellular area object.
     */
     IMPORT_C static CLbtGeoWcdmaCell* NewL();
    
    /**
     * Allocates and constructs a new geographical WCDMA cellular area object and
     * pushes it onto cleanup stack. 
     * In the returned object,the country code,network code,location area code and  
     * cell id are set to zero.
     * @return Pointer to the new geographical cellular area object.
     */
     IMPORT_C static CLbtGeoWcdmaCell* NewLC();
    
    /**
     * Allocates and constructs a new geographical WCDMA cellular area object.
     * 
     * @panic ELbtErrArgument If inputs are negative or zero.
     * 
     * @param[in] aNetworkType The network type
     * @param[in] aCountryCode The network country code
     * @param[in] aNetworkId The network identity code
     * @param[in] aLocationAreaCode The location area code
     * @param[in] aCellId The cell id 
     * @return Pointer to the new geographical cellular area object.
     */
     IMPORT_C static CLbtGeoWcdmaCell* NewL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
			                                RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
			                                RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
			                                TUint aLocationAreaCode,
			                                TUint aCellId);
    
    /**
     * Allocates and constructs a new geographical WCDMA cellular area object and
     * pushes it onto cleanup stack. 
     *
     * @panic ELbtErrArgument If inputs are negative or zero.
     * 
     * @param[in] aNetworkType The network type
     * @param[in] aCountryCode The network country code
     * @param[in] aNetworkId The network identity code
     * @param[in] aLocationAreaCode The location area code
     * @param[in] aCellId The cell id 
     * @return Pointer to the new geographical cellular area object.
     */                                      
     IMPORT_C static CLbtGeoWcdmaCell* NewLC(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
		                                     RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
		                                     RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
		                                     TUint aLocationAreaCode,
		                                     TUint aCellId);
       
    /**
     * Destructor.
     */
     IMPORT_C virtual ~CLbtGeoWcdmaCell();
    
        
    /**
     * Sets the WCDMA Cell information
     *
     * aparam[in] aWcdmaCell The WCDMA cell information
     */ 
     IMPORT_C void SetWcdmaCellInfo( const TLbtWcdmaCellInfo& aWcdmaCellInfo );
     
    /**
     * Gets the WCDMA Cell information.
     *
     * @return The WCDMA cell information
     */
	 IMPORT_C const TLbtWcdmaCellInfo& WcdmaCellInfo();
	 
	/**
     * Returns ETrue if the CGI information is specified by client during creation.
     * Returns EFalse  otherwise.
     */
    IMPORT_C virtual TBool CGISpecifiedDuringCreation() const;

protected:    
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL(RWriteStream& aStream) const ;
    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL(RReadStream& aStream);	

private:
    /**
     * default constructor
     */
    CLbtGeoWcdmaCell();
    
    /**
     * By default, prohibit copy constructor
     */
    CLbtGeoWcdmaCell( const CLbtGeoWcdmaCell& );
   
    /**
     * Prohibit assigment operator
     */
    CLbtGeoWcdmaCell& operator=( const CLbtGeoWcdmaCell& );   

private://data
    /**
     * The cell info of this cell
     */
	TLbtWcdmaCellInfo		iCellInfo;
    };


#endif //LBTGEOWCDMACELL_H
