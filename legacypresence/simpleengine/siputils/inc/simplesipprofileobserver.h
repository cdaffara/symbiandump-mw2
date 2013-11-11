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
* Description:    profile observer
*
*/




#ifndef CSImpleSipProfileObserver_H
#define CSImpleSipProfileObserver_H

//  INCLUDES
#include <e32base.h>
#include <sipprofileregistryobserver.h>

#include "simplesipconnectionobserver.h"

// FORWARD DECLARATIONS
class CSIPProfileRegistry;
class CSIPProfile;

// CLASS DECLARATIONS

class CSimpleSipProfileObserver: public CBase, public MSIPProfileRegistryObserver
{

public:

    ~CSimpleSipProfileObserver();

    /**
    * Two phase constructor
    * @param aSIP SIP core connection
    * @param aObs SIP connection observer for callbacks
    */
    static CSimpleSipProfileObserver* NewL( CSIP* aSIP, CSimpleSipConnectionObserver& aObs );

    /**
    * Register default profile
    * This start the registering and later the GiveConnectionL can be
    * used to get the ready connection.
    */
    void RegisterDefaultProfileL();
    
    /**
    * Register specified profile
    * This start the registering and later the GiveConnectionL can be
    * used to get the ready connection.
    * @param aId SIP Profile ID
    */
    void RegisterGivenProfileL( TUint32 aID );        

    /**
    * Give the registered connection.
    * Leaves with KErrNotReady if not ready.
    * MSimpleSipConnCallback::ConnectionStateChange() is called when
    * the registeration is ready.
    * @return SIP connection, ownership is changed
    */
    CSIPConnection* GiveConnectionL();
    
    /**
    * Is the SIP Profile active
    * @return ETrue if active.
    */
    TBool IsProfileActive();    

    /**
     * Give current registered user's AOR.
     * Leaves with KErrNotReady if profile is not registered.
     * @return AOR URI
     */
    TPtrC8 GiveUserAorL();

    /**
     * Current profile accessor
     */
    MSIPRegistrationContext* ProfileContext();

public://functions from MSIPProfileObserver
    /**
    * An event related to SIP Profile has accorred
    * @param aProfileId a profile Id
    * @param aEvent an occurred event
    **/
    void ProfileRegistryEventOccurred(TUint32 aProfileId, TEvent aEvent);

    /**
    * An asynchronous error has occurred related to SIP profile
    * Event is send to those observers, who have the
    * corresponding profile instantiated.
    * @param aProfileId the id of failed profile
    * @param aError an occurred error
    */
    void ProfileRegistryErrorOccurred(TUint32 aProfileId, TInt aError);

protected:

    /**
     * Second-phase constructor.
     */
    void ConstructL( CSIP* aSIP );

    CSimpleSipProfileObserver( CSimpleSipConnectionObserver& aObs );

private: // data

    /**
     * Next observer class where to send the events.
     */ 
    CSimpleSipConnectionObserver& iObs;
    
    /**
     * Own. SIP profile registry.
     */
    CSIPProfileRegistry* iRegistry;
    
    /**
     * Own. SIP profile
     */
    CSIPProfile* iProfile;

#ifdef _DEBUG
    friend class T_CSimpleSipConnection;
#endif
};

#endif
