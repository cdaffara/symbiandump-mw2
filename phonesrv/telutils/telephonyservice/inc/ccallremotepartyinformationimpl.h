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
* Description: 
*
*/

#ifndef CCALLREMOTEPARTYINFORMATIONIMPL_H
#define CCALLREMOTEPARTYINFORMATIONIMPL_H

#include <e32base.h>

#include "ctelsrvbase.h"


class MCallRemotePartyInfo;
class CCallRemotePartyInfos;
class MCallRemotePartyInformationObserver;
class CCallRemotePartyInfoIter;

/**
 *  Implementation for CCallRemotePartyInformation API.
 *  
 *  @lib TelephonyService
 *  @since S60 5.2
 */
NONSHARABLE_CLASS( CCallRemotePartyInformationImpl ) : public CTelSrvBase
    {
public:
     static CCallRemotePartyInformationImpl* NewL();
     static CCallRemotePartyInformationImpl* NewLC();

    /**
    * Destructor.
    */
    virtual ~CCallRemotePartyInformationImpl();
    
    /**
     * Sets the observer for changes in remote party info.
     *
     * @since S60 v5.2 
     * @param aObserver Observer for notifications.
     */
    void NotifyChanges( MCallRemotePartyInformationObserver& aObserver );
    
    /**
     * Used to cancel previously placed NotifyChanges call.
     *
     * @since S60 v5.2      
     */
    void CancelNotification();    
       
    /**
     * Gets the snapshot of remote party information of ongoing calls.
     *
     * @since S60 v5.2      
     * @return Remote party info of ongoing calls.
     */
    CCallRemotePartyInfoIter& GetRemotePartyInformationL( );
    
    /**
     * Gets the remote party information of a call.
     *
     * @since S60 v5.2  
     * @param aCallIndex Specifies a call.    
     * @return Remote party information. NULL if the remote party information
     * is not available for call. Ownership not given.
     */
    const MCallRemotePartyInfo* GetRemotePartyInformationL( TInt aCallIndex );
    
// from base class CTelSrvBase
        
    /**
     * From CTelSrvBase.
     * A response to a Mediator Service command.
     *
     * @since S60 v5.1
     * @param aData      The parameters of the response.
     */
    void DoHandleCommandResponseL( const TDesC8& aData );      
          
        
    /**
     * From CTelSrvBase.
     * A Mediator Service event.
     *
     * @since S60 v5.1     
     * @param aData      The parameters of the event.
     * @return None.
     */
    void DoHandleMediatorEventL( const TDesC8& aData );  

protected:

    CCallRemotePartyInformationImpl();

    void ConstructL(CMediatorService* aMediatorService);

private: // data
    
    /**
     * Observer to be notified when call information changes
     * Not own.  
     */    
    MCallRemotePartyInformationObserver* iObserver;
    
    
    CCallRemotePartyInfos*      iRemotePartyInfos;
    CCallRemotePartyInfoIter*   iRemotePartyInfoIter; 
    };

#endif // CCALLREMOTEPARTYINFORMATIONIMPL_H
