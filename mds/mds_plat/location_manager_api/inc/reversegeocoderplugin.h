/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:	The header file for ReverseGeoCoderPlugin that creates the ReverseGeocoder
*
*/


#ifndef __REVERSEGEOCODERPLUGIN_H__
#define __REVERSEGEOCODERPLUGIN_H__

// INCLUDES

#include <e32base.h> 
#include <lbsposition.h> 
#include <etel.h>
#include <etelmm.h>
#include <geotagobserver.h>

const TUid KReversegeocodeInterfaceUid = {0x2002DD13};


/*
 * Data class to get the address details. An handle to this type will be given to the
 * user through MReverseGeocodeObserver::ReverseGeocodeComplete callback, through which
 * user can retrieve the address details using the following interfaces.
 *
 **/
class MAddressInfo
    {
    public:
    /*
     * Gets the reference to the country name. Scope of this returned reference is limited to
     * this perticular call. User has to store it for their further processing.
     * 
     *  @return reference to the String which holds the Country Name. 
     */
    virtual TDesC& GetCountryName()= 0;
    
    /*
     * Gets the reference to the State. Scope of this returned reference is limited to
     * this perticular call. User has to store it for their further processing.
     * 
     *  @return reference to the String which holds the State name. 
     */
    virtual TDesC& GetState()= 0;
    
    /*
     * Gets the reference to the City. Scope of this returned reference is limited to
     * this perticular call. User has to store it for their further processing.
     * 
     *  @return reference to the String which holds the City Name. 
     */
    virtual TDesC& GetCity()= 0;
    
    /*
     * Gets the reference to the District name. Scope of this returned reference is limited to
     * this perticular call. User has to store it for their further processing.
     * 
     *  @return reference to the String which holds the District Name. 
     */
    virtual TDesC& GetDistrict()= 0;
    
    /*
     * Gets the reference to the postal code. Scope of this returned reference is limited to
     * this perticular call. User has to store it for their further processing.
     * 
     *  @return reference to the String which holds the postal code. 
     */
    virtual TDesC& GetPincode()= 0;
    
    /*
     * Gets the reference to the thoroughfare name. Scope of this returned reference is limited to
     * this perticular call. User has to store it for their further processing.
     * 
     *  @return reference to the String which holds the thoroughfare name. 
     */
    virtual TDesC& GetThoroughfareName()= 0;
    
    /*
     * Gets the reference to the thoroughfare number. Scope of this returned reference is limited to
     * this perticular call. User has to store it for their further processing.
     * 
     *  @return reference to the String which holds the thoroughfare number. 
     */
    virtual TDesC& GetThoroughfareNumber() = 0;
    };
/*
 * Observer class which exposes callbacks to notify the completion of reversegeocoding event.
 *
 **/
class MReverseGeocodeObserver
    {
    public:
    /*
     * Callback function which notifys the completion of reverse geocode event. This signals the completion
     * of the asynchronous function CReverseGeoCode::GetAddressByCoordinate.
     * 
     *  @param aErrorcode Error status KErrNone in case of success or other system specific errorcodes
     *                    in case of failures.
     *                       
     *  @param aAddressInfo refrence to the address stucture, through which user can access the
     *                      address information. 
     */

    virtual void ReverseGeocodeComplete( TInt& aErrorcode, MAddressInfo& aAddressInfo ) =0;

    /*
    * Get registrer network country code
    *
    * @return current register n/w info
    */
    virtual RMobilePhone::TMobilePhoneNetworkInfoV2& GetCurrentRegisterNw() = 0;

    
    /*
    * UE is registered to home network?
    *
    * @return ETrue if UE is registered at home network else EFalse
    */
    virtual TBool IsRegisteredAtHomeNetwork() = 0;

    /*
    * Get home network country code
    * @param aHomeNwInfoAvailableFlag ETrue if home n/w info available else EFalse
    * @return user home n/w info
    */
    virtual const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag) = 0;
    };


/**
 * CReverseGeoCoderPlugin
 *
 * An implementation of the CReverseGeoCoderPlugin definition. 
 *              Encapsulates the reverse goecoding functionality
 *              This is concrete class, instance of which
 *              ECOM framework gives to ECOM clients.
 */
class CReverseGeoCoderPlugin : public CBase
	{
public:
	
	/**
	* Create instance of concrete implementation. 
	* @return: Instance of this class.
	*/
	static CReverseGeoCoderPlugin* NewL();


	/**
	* Destructor.
	*/
	virtual ~CReverseGeoCoderPlugin();


public:
	
	/**
	 * Creates the instance of Reverse Geocoder
	 * 
	 */
	virtual void CreateReverseGeoCoderL() = 0;

	/**
	 * Initializes the ReverseGeoCodeObserver
	 * @param: aObserver The observer class instance that is to be notified when reverse geocoding completes
	 * 
	 */
    virtual void AddObserverL(MReverseGeocodeObserver& aObserver)=0;

	/**
	 * A wrapper API to fetch the address from geocoordinates
	 * Internally calls the ReverseGeoCoder
	 * @param aLocality A TLocality object that contains the geocoordinate information
	 * @param aOption Indicates if the connection is silent connection or not
	 * 
	 */
    virtual void GetAddressByCoordinateL( TLocality aLocality,const TConnectionOption aOption )=0;

	/**
	 * Wrapper API to check if the ReverseGeoCoder allows a silent connection
	 * @return:TBool Indicates if a silent connection is allowed
	 * 
	 */
	  virtual TBool SilentConnectionAllowed() = 0;
	
private:
	
	TUid iDtorKey;

	};
#include "reversegeocoderplugin.inl"

#endif //__REVERSEGEOCODERPLUGIN_H__

//End of file

