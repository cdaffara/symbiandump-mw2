/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observers SAT refresh events
*
*/


#ifndef CSATREFRESHOBSERVER_H
#define CSATREFRESHOBSERVER_H

#include <stiflogger.h>
#include <testscripterinternal.h>
#include <stiftestmodule.h>
// API under testing:
#include <rsatrefresh.h>
#include <msatrefreshobserver.h>
// This is needed for Refresh API
#include <rsatsession.h>

/**
*  Refresh event observer
*
*  @lib satrefreshapitest.dll
*  @since S60 5.0
*/
class CSatRefreshObserver : public MSatRefreshObserver
    {
    public:  

        /**
        * C++ default constructor.
        */
        CSatRefreshObserver(CStifLogger* aLog);

        /**
        * Destructor.
        */
        virtual ~CSatRefreshObserver();
        

        /**
        * From MSatRefreshObserver
        * 
        * @since 2.6
        * @param aType Refresh type.
        * @param aFiles Elementary files which are to be changed.
        * May be zero length.
        * @return ETrue to allow refresh, EFalse to decline refresh.
        */
        TBool AllowRefresh(
            TSatRefreshType aType, const TSatRefreshFiles& aFiles );

         /**
        * From MSatRefreshObserver, Notification of refresh.
        * @since 2.6
        * @param aType Type of refresh which has happened.
        * @param aFiles List of elementary files which has been changed.
        * May be zero length.
        */
        void Refresh(
            TSatRefreshType aType, const TSatRefreshFiles& aFiles );

    private:    // Methods

    private:    // Data
   
        /**
        * Pointer to logger
        * not own
        */
        CStifLogger* iLog;
        
        /**
        * Object of RSatSession
        */
        RSatSession iSatSession;
     };

#endif      // CSATREFRESHOBSERVER_H
