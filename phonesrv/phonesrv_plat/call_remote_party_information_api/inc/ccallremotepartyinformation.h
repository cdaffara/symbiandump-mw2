/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Remote party information
*
*/

#ifndef CCALLREMOTEPARTYINFORMATION_H
#define CCALLREMOTEPARTYINFORMATION_H

#include <e32base.h>

class CCallRemotePartyInformationImpl;
class MCallRemotePartyInfo;
class MCallRemotePartyInformationObserver;
class CCallRemotePartyInfoIter;

/**
 * 
 */
NONSHARABLE_CLASS( CCallRemotePartyInformation ) : public CBase
    {

public:

    /**
     * Two-phased constructor.          
     */
    IMPORT_C static CCallRemotePartyInformation* NewL();
    
    
    /**
    * Destructor.
    */
    virtual ~CCallRemotePartyInformation();
    
    /**
     * Allows a client to be notified when there are changes in
     * remote party information. There can be only one observer
     * per CCallRemotePartyInformation instance.
     *
     * @since S60 v5.2 
     * @param aObserver Observer for notifications.
     */
    IMPORT_C void NotifyChanges( MCallRemotePartyInformationObserver& aObserver );
        
    /**
     * Used to cancel previously placed NotifyCallInformationChanges call.
     *
     * @since S60 v5.2      
     */
    IMPORT_C void CancelNotification();
    
    /**
     * Gets the snapshot of remote party information of ongoing calls.
     *
     * @since S60 v5.2      
     * @return Remote party info of ongoing calls.
     */
    IMPORT_C CCallRemotePartyInfoIter& GetRemotePartyInformationL( );
    
    /**
     * Gets the remote party information of a call.
     *
     * @since S60 v5.2  
     * @param aCallIndex Specifies a call.    
     * @return Remote party information. NULL if the remote party information
     * is not available for call. Ownership not given.
     */
     IMPORT_C const MCallRemotePartyInfo* GetRemotePartyInformationL( TInt aCallIndex );
     
private:

    CCallRemotePartyInformation();

    void ConstructL();


private: // data    
    
    CCallRemotePartyInformationImpl* iImpl; 
    
    };

#endif // CCALLREMOTEPARTYINFORMATION_H
