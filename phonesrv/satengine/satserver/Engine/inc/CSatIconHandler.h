/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for accesing icon data in SIM.
*
*/


#ifndef CSATICONHANDLER_H
#define CSATICONHANDLER_H

#include <e32std.h>
#include <e32base.h>
#include "MSatSIconAPI.h"

class MSatIconObserver;

/**
*  Interface for accesing icon data in SIM.
*
*  @lib SatEngine.lib
*  @since 3.0
*/
class CSatIconHandler : public CActive,
                        public MSatSIconAPI
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CSatIconHandler( MSatApi& aSatApi );

        /**
        * Destructor.
        */
        virtual ~CSatIconHandler();

    public: // Functions from base classes

        /**
        * From MSatIconAPI. Gets the icon info from SIM.
        * @param aRecordNumber Specifies the EF_IMG in SIM.
        * @param aIconEf Contains the icon informations from SIM in returning
        * of the method.
        * @param aObserver Observer for icon info get completion.
        */
        void GetIconInfo(
            TUint8 aRecordNumber,
            RSat::TIcon& aIconEf,
            MSatIconObserver& aObserver );

        /**
        * From MSatIconAPI. Gets the icon image.
        * @param aInstanceInfo Informatio of the icon, which data is
        * fetched from SIM.
        * @param aInstance Contains the icon data from SIM in returning of the
        * method.
        * @param aObserver Observer for icon image get completion.
        */
        void GetIconInstance(
            const RSat::TInstanceInfoV3& aInstanceInfo,
            TDes8& aInstance,
            MSatIconObserver& aObserver );

        /**
        * From MSatIconAPI. Gets color lookup table of the icon.
        * @param aInstanceNumber Image instance number.
        * @param aOffset Offset of the clut.
        * @param aClut Contains the Clut of the icon in returning of the
        * method.
        * @param aObserver Observer for clut get completion.
        */
        void GetClut(
            TUint aInstanceNumber,
            TUint aOffset,
            TDes8& aClut,
            MSatIconObserver& aObserver );
        /*
         * Set a mode to determine whether to fetch icons from NAA syncronously
         * or asynchronously.
         *
         * @param aMode Synchronous or asynchronous mode. Icons would be
         * fetched from NAA in sychronous mode if SAT UI was closed by
         * using the End Key. Otherwise, in asynchronous mode.
         * @return None
         */
        void SetIconFetchingMode( TBool aMode );

    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        * @param aError Received error code
        * @return Result error code
        */
        TInt RunError( TInt aError );

    private:    // Data

        // Sat api
        MSatApi& iSatApi;

        // Indicates GetIconInfo is active.
        TBool iGetIconInfoActive;

        // Indicates GetIconInstance is active.
        TBool iGetIconInstanceActive;

        // Indicates GetClut is active.
        TBool iGetClutActive;

        // Observer for icon actions.
        MSatIconObserver* iObserver;

        // Indicates whether to fetch icons from NAA syncronously or 
        // asynchronously.
        TBool iSyncIconFetching;
    };

#endif      // CSATICONHANDLER_H
