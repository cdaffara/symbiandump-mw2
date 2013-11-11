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
* Description:  A class responsible for geotagging of media files in background.
*
*/

#ifndef GEOTAGOBSERVER_H_
#define GEOTAGOBSERVER_H_

#include <e32base.h>
#include <etel.h>
#include <etelmm.h>

enum TConnectionOption
    {
        ESilent=0,
        EInteractive
    };

typedef enum     
    {
    EGeoTaggingPending = 0x01,
    EGeoTaggingGoingOn,
    EGeoTagCmpt
    }TGeoTaggingSatus;


class MGeoTaggerObserver
    {
public:    
    /**
     * This method is used for notifying completion of geotagging
     */
    virtual void GeoTaggingCompleted( const TInt aError ) = 0;

    /**
     * This method is used for notifying completion of query for pending geo tags
     */
    virtual void PendingGeoTagReqComplete( const TInt aError ) = 0;

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


#endif // GEOTAGOBSERVER_H_ 

// End of file.
