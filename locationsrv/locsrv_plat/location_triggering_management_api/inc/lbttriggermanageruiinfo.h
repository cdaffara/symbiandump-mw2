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
* Description:  Structure for location triggering status
*
*/



#ifndef LBTTRIGGERMANAGERUIINFO_H
#define LBTTRIGGERMANAGERUIINFO_H


#include <lbtcommon.h>

/**
 * Data class for trigger manager UI information.
 * 
* @since S60 5.1
 */
struct TLbtTriggerManagerUiInfo
    {
    /**
     * The UID of the manager UI
     */
    TUid iUid;
    
    /**
     * Number of enabled triggers that associated with this manager UI
     */
    TInt iNumberOfEnabledTriggers;
    
    /**
     * Number of disabled triggers that associated with this manager UI.
     */
    TInt iNumberOfDisabledTriggers;
    
    /**
     * Number of invalid triggers that associated with this manager UI.
     */
    TInt iNumberOfInvalidTriggers;
    
    /**
     * Reserved for future use
     */
    TUint8 iReserved[8];
    };

#endif // LBTTRIGGERMANAGERUIINFO_H
