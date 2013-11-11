/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsRecNetworkListener class
*
*/


#ifndef     CCBSRECNETWORKLISTENER_H
#define     CCBSRECNETWORKLISTENER_H

// FORWARD DECLARATIONS
class RMobilePhone;

//  CLASS DECLARATION

/**
*   CCbsRecNetworkListener is used to keep CbsServer up to date 
*   with the current network information. This information
*   is used to assemble multipaged CB messages.
*
*   This is an active object which receives notifications from EPOC Telephony Server
*   when the current network information changes. A request for 
*   new network information is constantly held active at EPOC Telephony Server,
*   first requested at the time of construction of CCbsRecNetworkListener.
*
*   Network information can be retrieved from CCbsRecNetworkListener
*   with GetCurrentNetworkInfo().
*/
class CCbsRecNetworkListener : public CActive
    {
    public:     // New functions

        /**
        *   Creates an instance of the class.
        *
        *   @param  aPhone              Open phone session.
        *   @return                     A pointer to the created instance.
        */        
        static CCbsRecNetworkListener* NewL( RMobilePhone& aPhone );

        /**
        *   Destructor.
        */
        ~CCbsRecNetworkListener();

        /**
        *   Fetches the current networking info. 
        *
        *   Returns KErrNotFound, if the current network info hasn't been
        *   received yet. Otherwise KErrNone is returned.
        *
        *   @param  aNetworkInfo        Return: current network information.
        *   @param  aArea               Return: current area information
        *   @return                     Result code
        */        
        TInt GetCurrentNetworkInfo( 
            RMobilePhone::TMobilePhoneNetworkInfoV1& aNetworkInfo,
            RMobilePhone::TMobilePhoneLocationAreaV1& aArea );

    private:    // From CActive
        /**
        *   Called whenever EPOC Telephony Server notifies this object of the presence
        *   of updated network information.
        */
        void RunL();

        /**
        *   Called whenever the listener has been requested to cancel.
        */
        void DoCancel();

    private:    // new functions

        /**
        *   Default constructor.
        *
        *   @param  aPhone  Reference to phone object
        */
        CCbsRecNetworkListener( RMobilePhone& aPhone );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();

        /**
        *   Requests getting current network from EPOC Telephony Server.
        *
        *   Called when CCbsRecNetworkListener is constructed.
        */
        void IssueGetCurrentNwRequest();

        /**
        *   Requests notification of network registration state change from 
        *   EPOC Telephony Server.
        */
        void IssueNotifyNwRegChangeRequest();

        /**
        *   Requests notification of network state change from EPOC Telephony Server.
        */
        void IssueNotifyNwChangeRequest();

        /**
        *   When the current request is GetCurrentNetwork, handle the return result
        */
        void HandleGetCurrentNetworkResult();

        /**
        *   When the current request is NotifyNetworkRegistrationStatusChange, 
        *   handle the return result
        */
        void HandleNotifyNetworkRegistrationStatusChangeResult();

        /**
        *   When the current request is NotifyCurrentNetworkChange, 
        *   handle the return result
        */
        void HandleNotifyCurrentNetworkChangeResult();

        // Prohibited copy constructor
        CCbsRecNetworkListener( const RMobilePhone& );

        // Prohibited assignment operator
        CCbsRecNetworkListener& operator=( const RMobilePhone& );

    private:    // Data
        // ETrue, if iNetworkInfo contains current network information
        TBool iNetworkInfoRetrieved;

        // Phone instance
        RMobilePhone& iPhone;

        // Current network information data
        RMobilePhone::TMobilePhoneNetworkInfoV1 iNetworkInfo;

        // Current network information package        
        RMobilePhone::TMobilePhoneNetworkInfoV1Pckg iNetworkInfoPckg;

        // Current area information
        RMobilePhone::TMobilePhoneLocationAreaV1 iArea;

        // Status info
        TBool iGetCurrentNetworkActive;

        // Status of notification of network registration state change
        TBool iNotifyNwRegChange;

        // Network registration information
        RMobilePhone::TMobilePhoneRegistrationStatus iRegistration;
    };

#endif      //  CCBSRECNETWORKLISTENER_H   
            
// End of File


