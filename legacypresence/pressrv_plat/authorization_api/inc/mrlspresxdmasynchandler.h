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
* Description:    RLS and Presence XDM, This is Asynchronous handler
*
*/



#ifndef __MRLSPRESXDMASYNCHANDLER_H__
#define __MRLSPRESXDMASYNCHANDLER_H__

// Class declaration
/** 
* This is asynchronous handler class for CPresenceXDM's and CRLSXDM's async 
*  functions. The client classes should drive from this class and must implement 
*  it's functions to get async. requests handled.
*/
class MRLSPresXDMAsyncHandler
    {
    public:
        
    // Functions needed implementation for using CRLSXDM, and empty implementation
    // for using CPresenceXDM.
        /**
        * This method handles the async. call to function CRLSXDM::UpdateToServerL
        * and CRLSXDM::UpdateAllFromServerL.  Clients should implement this method 
        * to get callbacks
        *
        * @param aErrorCode, XDMengine error codes
        */
        virtual void HandleRLSUpdateDocumentL(TInt aErrorCode) = 0;
        
        /**
        * This method is called when client cancels the update request to/from server
        * using  to function CRLSXDM::CancelUpdateL.
        * Clients should implement this method to get a callback
        *
        * @param aErrorCode, XDMengine error codes
        */
        virtual void HandleRLSUpdateCancelL(TInt aErrorCode) = 0;

        /**
        * This method handles the async. call to function CRLSXDM::DeleteAllEmptyListsL
        * Clients should implement this method to get a callback
        *
        * @param aErrorCode, XDMengine error codes
        */
        virtual void HandleRLSDeleteAllEmptyListsL(TInt aErrorCode) = 0;

    // Functions needed implementation for using CPresenceXDM, and empty implementation
    // for using CRLSXDM.

        /**
        * This method handles the async. call to function CPresenceXDM::UpdateToServerL
        * and CPresenceXDM::UpdateAllFromServerL. Clients should implement this method
        * to get a callback.
        *
        * @param aErrorCode, XDMengine error codes
        */
        virtual void HandlePresUpdateDocumentL(TInt aErrorCode) = 0;        
        
        /**
        * This method is called when client cancels the update request to/from server
        * using  to function CPresenceXDM::CancelUpdateL.
        * Clients should implement this method to get a callback
        *
        * @param aErrorCode, XDMengine error codes
        */
        virtual void HandlePresUpdateCancelL(TInt aErrorCode) = 0;       
    };
    
#endif //__MRLSPRESXDMASYNCHANDLER_H__