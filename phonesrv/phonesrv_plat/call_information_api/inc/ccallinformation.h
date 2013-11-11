/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides information about ongoing calls.
*
*/


#ifndef C_CALLINFORMATION_H
#define C_CALLINFORMATION_H

#include <e32base.h>

#include "mcall.h"


// forward declarations 
class MCallInformationObserver;
class CCallInformationImpl;
class CCallInfoIter;


/**
 *  Class to be used for receiving information about ongoing calls. 
 *
 *  @code
 *  // create the CCallInformation instance e.g. when the client component is created
 *  iCallInfo = CCallInformation::NewL(); 
 *  iCallInfo->NotifyCallInformationChanges(*iObserver);
 *  
 *  // and use it when needed. 
 *  CCallInfoIter& iter = iCallInfo->GetCallsL();
 *  @endcode
 *
 *  @lib telephonyservice.lib
 *  @since S60 v5.1 
 */
NONSHARABLE_CLASS( CCallInformation ) : public CBase
    {

public:

    /**
     * Two-phased constructor.          
     */
    IMPORT_C static CCallInformation* NewL();
    

    /**
    * Destructor.
    */
    virtual ~CCallInformation();

    /**
     * Allows a client to be notified when there are changes in current calls.
     * There can be only one observer per CCallInformation instance.
     *
     * @since S60 v5.1 
     * @param aObserver Observer for notifications.
     */
     IMPORT_C void NotifyCallInformationChanges( MCallInformationObserver& aObserver );
    
    /**
     * Used to cancel previously placed NotifyCallInformationChanges call.
     *
     * @since S60 v5.1      
     */
     IMPORT_C void CancelNotification();
     
    /**
     * Gets the snapshot of ongoing calls. 
     * The returned reference should not be stored as member variable 
     * as it goes out of scope when there are changes in call information.
     *
     * @since S60 v5.1      
     * @return Ongoing calls.
     */
     IMPORT_C CCallInfoIter& GetCallsL( );
        
    /**
     * Returns information whether call in given state exist.
     * 
     * @since S60 v5.2
     * @param aState Specifies a call state that is used as a search key.
     * @return ETrue if call in given state exist.
     */
    IMPORT_C TBool IsCallInState( CCPCall::TCallState aState ) const;
     
     
private:

    CCallInformation();

    void ConstructL();


private: // data    
    
    CCallInformationImpl* iImpl;    
    
    };


#endif // C_CALLINFORMATION_H
