/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Factory class for Event monitors
*
*/



#ifndef TSATEVENTMONITORFACTORY_H
#define TSATEVENTMONITORFACTORY_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MSatEventMonitor;
class MSatEventMonitorUtils;


// CLASS DECLARATION

/**
*  Factory class.
*
*  @lib EventMonitors.lib
*  @since Series 60 3.0
*/
class TSatEventMonitorFactory
    {
    public:  // Factory methods

        /**
        * Creates pointer array containing event monitor objects.
        */
        IMPORT_C static CArrayPtrFlat<MSatEventMonitor>* 
            CreateEventMonitorsL( MSatEventMonitorUtils& aUtils );
        
    };

#endif      // TSATEVENTMONITORFACTORY_H   
            
// End of File
