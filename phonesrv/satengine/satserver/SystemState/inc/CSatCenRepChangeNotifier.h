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
* Description:  Notifies the changes in central repository values.
*
*/



#ifndef CSATCENREPCHANGENOTIFIER_H
#define CSATCENREPCHANGENOTIFIER_H

//  INCLUDES
#include <e32base.h>
#include "MSatSystemStateChangeNotifier.h"

// FORWARD DECLARATION
class MSatSystemStateObserver;
class CRepository;

// CLASS DECLARATION

/**
*  Notifies the changes in central repository values.
*
*  @lib SatSystemState
*  @since Series 60 3.0
*/
class CSatCenRepChangeNotifier : public CActive,
                                 public MSatSystemStateChangeNotifier
    {
    public: // Enums

        /**
        * Identifier for the value type that is being observered.
        */
        enum TCenRepTypes
            {
            // Type is TInt
            ECRTypeInteger,
             // Type is TReal
            ECRTypeReal,
             // Type is TDesC8
            ECRTypeDesC8,
            // Type is TDesC16
            ECRTypeDesC16,
            // Type not defined
            ECRTypeUnknown
            };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aCenRepCategory This is the CR Category.
        * @param aCenRepId CR value id to be listened.
        * @param aObserver A class that is notified when value changes.
        * @param aCenRepType Identifies the base type of the observered value,
        *        which can be TInt, TDesC or TReal. Default is Unknown.
        */
        static CSatCenRepChangeNotifier* NewL(
            const TUid& aCenRepCategory,
            TUint32 aCenRepId,
            MSatSystemStateObserver& aObserver,
            TInt aCenRepType );

        /**
        * Destructor.
        */
        virtual ~CSatCenRepChangeNotifier();

    public: // Functions from base classes

        /**
        * From MSatSystemStateChangeNotifier Request a notification of a
        * change in system state.
        */
        void NotifyChangeL();

        /**
        * From MSatSystemStateChangeNotifier Cancels the notification request.
        */
        void CancelNotify();

    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive.
        */
        void RunL();

    private: // New methods

        /**
        * Makes the request and activates the active object.
        */
        void ActivateL();

    private:

        /**
        * C++ default constructor.
        * @param aCenRepCategory This is the CR Category.
        * @param aCenRepId CR value id to be listened.
        * @param aObserver A class that is notified when value changes.
        * @param aCenRepType Identifies the base type of the observered value
        */
        CSatCenRepChangeNotifier(
            const TUid& aCenRepCategory,
            TUint32 aCenRepId,
            MSatSystemStateObserver& aObserver,
            TInt aCenRepType );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Change observer.
        MSatSystemStateObserver& iObserver;

        // Central repository class.
        CRepository* iCenRepClient;

        // CenRep category
        TUid iCenRepCategory;

        // CenRep value to listen
        TUint32 iCenRepId;

        // CR value type
        TInt iCenRepType;

    };

#endif      // CSATCENREPCHANGENOTIFIER_H

// End of File
