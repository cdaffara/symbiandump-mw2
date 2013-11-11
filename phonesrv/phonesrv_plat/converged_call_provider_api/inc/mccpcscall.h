/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCP CS call class
*
*/


#ifndef MCCPCSCALL_H
#define MCCPCSCALL_H

//  INCLUDES
#include <e32base.h>
#include <mccpcall.h>

/**
* Methods used only with CS Call.
*
*  @since S60 3.2
*/
class MCCPCSCall : public MCCPCall

    {
 public:
 
   /**
    * Starts dialing to recipient
    * @since S60 3.2
    * @param aCallParams The call parameters used by the TSY (a TCallParamsPckg object)
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in idle state.
    * @return KErrNotSupported If call is not mobile originated.
    * @pre Call state is MCCPCallObserver::ECCPStateIdle and call type is MO.
    * @since S60 3.2
    */  
    virtual TInt Dial( const TDesC8& aCallParams ) = 0;

    /**
    * Instruct to do no FDN checking when dialling.
    * Needs to be called before every dial for each call separately.
    * If this method is not called at all default is to use FDN. 
    * Value will be reset to default when call goes back to disconnected state.
    * @since S60 v3.2
    * @param none
    * @return none
    */
    virtual void NoFDNCheck() = 0;

    /**
    * Returns call information
    * @since S60 3.2
    * @param aCallInfo TMobileCallInfoV3
    * @return KErrNone if succesfull, otherwise another system wide error code
    */
    virtual TInt GetMobileCallInfo( TDes8& aCallInfo ) const = 0;

    /**
    * Switch in the alternating call mode of an alternating call
    * @since S60 3.2
    * @param None
    * @return KErrNone or system error code.
    */
    virtual TInt SwitchAlternatingCall() = 0;

    /**
    * Gets the calls data call capabilities
    * @since S60 3.2
    * @param aCaps TMobileCallDataCapsV1
    * @return KErrNone if the function member was successful, 
    * @return KErrNotSupported if call does not support circuit switched data, 
    * @return KErrNotFound if this call is not a data call
    */
    virtual TInt GetMobileDataCallCaps( TDes8& aCaps ) const = 0;

    /**
    * Log dialed  number. SAT related, check if dialed number is to be logged or not.
    * @since S60 3.2
    * @param None
    * @return Log dialed number or not.
    */
    virtual TBool LogDialedNumber() const = 0;
    };


#endif // CCPCSCALL_H
