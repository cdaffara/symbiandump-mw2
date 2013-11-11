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
* Description:  API for container of event monitors
*
*/



#ifndef MSATEVENTMONITORCONTAINER_H
#define MSATEVENTMONITORCONTAINER_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>
// CLASS DECLARATION

/**
*  Interface for observing events.
*
*  @lib SatEngine
*  @since Series 60 3.0
*/
class MSatEventMonitorContainer
    {
    protected: // Constructors and destructor

        /**
        * C++ default constructor.
        */
        MSatEventMonitorContainer() {};

        /**
        * Destructor.
        */
        virtual ~MSatEventMonitorContainer() {};

    public: // New functions

        /**
        * Updates monitors state.
        * @param aEvents Events that should be monitored.
        * @return KErrNone if all goes OK
        */
        virtual TInt SetUpEventMonitors( TUint aEvents ) = 0;

        /**
        * Cancels all monitors.
        * @return KErrNone if all goes OK
        */
        virtual TInt CancelAllMonitors() = 0;

        /**
        * Solves language code.
        * @param aLanguageId Resolved language ID
        */
        virtual void LanguageSettingL( TInt& aLanguageId ) = 0;

        /**
        * Gets the ISO639 language code.
        * @param aId Language ID
        * @param aText Language code can be read from this after function call.
        */
        virtual void GetLanguageString( const TInt aId, TDes& aText ) = 0;

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatEventMonitorContainer( const MSatEventMonitorContainer& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatEventMonitorContainer& operator=( const MSatEventMonitorContainer& );

    };

#endif      // MSATEVENTMONITORCONTAINER_H

// End of File
