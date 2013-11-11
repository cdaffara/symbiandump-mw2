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
* Description:  3AM geo tagger handler
*
*/

#ifndef __CLOCATIONGEOTAGTIMERAO_H__
#define __CLOCATIONGEOTAGTIMERAO_H__


// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32msgqueue.h>
#include <geotagger.h>

class CLocationGeoTagTimerAO : public CTimer,
                public MGeoTaggerObserver
{
    
public:
     /**
        * 1st phase constructor.
        * @param aMdeSession, CMdESession reference
        */
     static CLocationGeoTagTimerAO* NewL(CMdESession& aMdeSession, 
                MGeoTaggerObserver& aObserver); 

     /**
        * Destructor
        */
     ~CLocationGeoTagTimerAO();

     /**
        * Start 3AM timer
        */
     void StartTimer();

     
protected:
    /*
        * @see CActive::RunL()
        */
    void RunL();
    // MGeoTaggerObserver
		/**
	   * This method is used for notifying that the Geo Tagging is Completed, from MGeoTaggerObserver
	   * @param aError, TInt value to indicate if there is any error in geotagging
	   * @return None.
	   */
    void GeoTaggingCompleted( const TInt aError );   
    
	/**
	   * This method is used for notifying completion of query for pending geo tags from MGeoTaggerObserver
	   * @param aError, TInt value to indicate if there is any error in geotagging
	   * @return None.
	   */
    void PendingGeoTagReqComplete( const TInt aError );

    /*
        * Get registrer network country code
        *
        * @return current register n/w info
        */
        RMobilePhone::TMobilePhoneNetworkInfoV2& GetCurrentRegisterNw();

    /*
    * UE is registered to home network?
    *
    * @return ETrue if UE is registered at home network else EFalse
    */
    TBool IsRegisteredAtHomeNetwork();


    /*
    * Get home network country code
    * @param aHomeNwInfoAvailableFlag ETrue if home n/w info available else EFalse
    * @return user home n/w info
    */
    const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag);
    
private:
    
    /**
    * 2nd Phase constructor
    */
    void ConstructL();
    
		/**
	   * CLocationGeoTagTimerAO
	   * Constructor
	   * @param aMdeSession, CMdESession reference
	   * @return None.
	   */
     CLocationGeoTagTimerAO(CMdESession& aMdeSession, MGeoTaggerObserver& aObserver);
     
     TBool IsLowBattery();
      
private:      
      CGeoTagger* iGeoTagger;
      CMdESession& iMdeSession;
      MGeoTaggerObserver& iObserver;
};


#endif /*__CLOCATIONGEOTAGTIMERAO_H__*/

// End of file
