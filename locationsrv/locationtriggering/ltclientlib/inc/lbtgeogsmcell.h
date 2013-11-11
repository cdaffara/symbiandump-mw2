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
* Description:  Definition of geo gsm cell class.
*
*/



#ifndef LBTGEOGSMCELL_H
#define LBTGEOGSMCELL_H

#include "lbtgeocell.h"


/**
 * Geographical Gsm cell.
 *  
 * CLbtGeoGsmCell class is defined by the area covered by a GSM network
 *  cell.
 *  
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
 
class CLbtGeoGsmCell : public CLbtGeoCell
    {
public:
    /**
     * Allocates and constructs a new geographical GSM cellular area object.
     * In the returned object,the country code,network code,location area code and  
     * cell id are set to zero.
     * @return Pointer to the new geographical cellular area object.
     */
     IMPORT_C static CLbtGeoGsmCell* NewL();
    
    /**
     * Allocates and constructs a new geographical GSM cellular area object and
     * pushes it onto cleanup stack. 
     * In the returned object,the country code,network code,location area code and  
     * cell id are set to zero.
     * @return Pointer to the new geographical cellular area object.
     */
     IMPORT_C static CLbtGeoGsmCell* NewLC();
    
    /**
     * Allocates and constructs a new geographical GSM cellular area object.
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
     IMPORT_C static CLbtGeoGsmCell* NewL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
		                                  RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
		                                  RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
		                                  TUint aLocationAreaCode,
		                                  TUint aCellId);
    
    /**
     * Allocates and constructs a new geographical GSM cellular area object and
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
     IMPORT_C static CLbtGeoGsmCell* NewLC(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
		                                   RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
		                                   RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
		                                   TUint aLocationAreaCode,
		                                   TUint aCellId);
       
    /**
     * Destructor.
     */
     IMPORT_C virtual ~CLbtGeoGsmCell();
    
        
    /**
     * Sets the BSIC array.
     *
     * aparam[in] aBsicArray The Bsic array
     */ 
     IMPORT_C void SetBsic( RArray<TInt>& aBsicArray );
     
    /**
     * Gets the BSIC array.
     *
     * @return The Bsic array
     */
	 IMPORT_C RArray<TInt>& Bsic();	
	 
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
    virtual void DoInternalizeL(RReadStream& aStream)  ;	

private:
    /**
     * default constructor
     */
    CLbtGeoGsmCell();
    
    /**
     * By default, prohibit copy constructor
     */
    CLbtGeoGsmCell( const CLbtGeoGsmCell& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtGeoGsmCell& operator= ( const CLbtGeoGsmCell& );
   

private: // data
    /**
     * Bsic array
     */
    RArray<TInt>   iBsicArray;     
	};


#endif //LBTGEOGSMCELL_H
