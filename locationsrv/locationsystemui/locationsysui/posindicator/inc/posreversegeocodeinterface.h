/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Declaration of Geo coding interface class.
*
*/

#ifndef POSREVERSEGEOCODEINTERFACE_H
#define POSREVERSEGEOCODEINTERFACE_H

#include <e32base.h> 
#include <EPos_CPosLandmark.h>

/*
 * Observer class which exposes callbacks to notify the completion of reversegeocoding event.
 *
 **/
class MPosReverseGeocodeObserver
    {
public:
    /*
     * Callback function which notifys the completion of reverse geocode event. This signals the completion
     * of the asynchronous function CReverseGeoCode::GetAddressByCoordinate.
     * 
     *  @param aErrorcode Error status KErrNone in case of success or other system specific errorcodes
     *                    in case of failures.
     */

    virtual void ReverseGeocodeComplete( TInt aErrorcode ) =0;
    };

/*
 * CReverseGeocode
 * Concrete class which exposes interfaces to convert the geo-coordinates information
 * in to the address information.
 *
 **/
class CPosReverseGeocodeInterface : public CBase
    {
public:
    /*
     * Factory function to create the instance of CReverseGeocode Class. This also registers
     * observer for getting the reverse geocode completion notifications.
     * 
     * @param[in] aObserver refrence to the instance MReverseGeocodeObserver's 
     *                  implementation class.
     * @return pointer to the instance of CReverseGeocode.                 
     */
    IMPORT_C static CPosReverseGeocodeInterface* NewL( MPosReverseGeocodeObserver& aObserver );
    
    /**
     * Unloads the plugin.
     */
    IMPORT_C virtual ~CPosReverseGeocodeInterface();
    
    /*
	* 
	* Gets the address information for the given geo coordinates. The geo-coordinates should be 
	* encapsulated inside a landmark object which is passed as argument to this method. This is an
	* asyncronous function whose completion will be notified  by
	* MReverseGeocodeObserver::ReverseGeocodeComplete call back.Address information corresponding to
	* coordinates can be retrieved from the same landmarks object.  
	* 
    * @param aLandmark[in/out] Landmarks object in which address information is expected.
    * 
    */
    IMPORT_C virtual void GetAddressByCoordinateL( CPosLandmark& aLandmark ) = 0;
    
    
    /**
     * Cancels the current operation.
     */
    IMPORT_C virtual void Cancel() = 0;
private:
    /**
     * Instance identifier key
     */
    TUid iDtor_ID_Key;
    };

#endif //POSREVERSEGEOCODEINTERFACE_H
