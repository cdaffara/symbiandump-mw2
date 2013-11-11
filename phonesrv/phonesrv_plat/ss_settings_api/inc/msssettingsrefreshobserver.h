/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer to listen refresh events
*
*/



#ifndef MSSSETTINGSREFRESHOBSERVER_H
#define MSSSETTINGSREFRESHOBSERVER_H


//  INCLUDES
#include <e32std.h>
#include <etelsat.h>
#include <msatrefreshobserver.h> 


// DATA TYPES

// CLASS DECLARATION

/**
*  Observer to listen refresh events
*
*  @lib SSSettings.lib
*  @since 2.8
*/
class MSSSettingsRefreshObserver 
    {
    public: // New functions
        
        /**
        * Refresh query. Client should determine whether it allow the
        * refresh to happen.
        * 
        * @param aType  Refresh type.
        * @param aFiles Elementary files which are to be changed. 
        *               May be zero length. 
        * @return ETrue to allow refresh, EFalse to decline refresh.
        */
        virtual TBool AllowRefresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles ) = 0;

        /**
        * Notification of refresh.
        * 
        * @param aType Type of refresh which has happened.
        * @param aFiles List of elementary files which have been changed.
        *               May be zero length.
        */
        virtual void Refresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles ) = 0;
    };

#endif      // MSSSETTINGSREFRESHOBSERVER_H  
            
// End of File
