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
* Description:  Observer for CenRep changes
*
*/



#ifndef M_MPXCENREPKEYOBSERVER_H
#define M_MPXCENREPKEYOBSERVER_H

//  INCLUDES
#include <e32base.h>

//  FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Observer interface for getting notifications about changes in settings 
*  values.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(MMPXCenRepObserver)
    {
    public: // New functions
        /**
        * Handle a change in a setting value.
        *
        * @since S60 3.2.3
        * @param aRepositoryUid Central repository UID containing the setting
        * @param aSettingId Id of the setting
        */
        virtual void HandleSettingChange(const TUid& aRepositoryUid,
                                         TUint32 aSettingId) = 0;
    };


#endif      // M_MPXCENREPKEYOBSERVER_H

// End of File
