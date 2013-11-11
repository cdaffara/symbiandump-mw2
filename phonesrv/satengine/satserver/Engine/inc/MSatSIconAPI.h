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



#ifndef MSATSICONAPI_H
#define MSATSICONAPI_H

//  INCLUDES
#include <e32std.h>
#include <etelsat.h>
#include "MSatIconObserver.h"

// CLASS DECLARATION

/**
*  Interface for accessing icon data in SIM.
*
*  @lib SatEngine.lib
*  @since 2.6
*/
class MSatSIconAPI
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~MSatSIconAPI() {};

    public: // New functions

        /**
        * Gets the icon info from SIM.
        * @param aRecordNumber Specifies the EF_IMG in SIM.
        * @param aIconEf Contains the icon informations from SIM in returning
        * of the method.
        * @param aObserver Observer for icon info get completion.
        */
        virtual void GetIconInfo(
            TUint8 aRecordNumber,
            RSat::TIcon& aIconEf,
            MSatIconObserver& aObserver ) = 0;

        /**
        * Gets the icon image.
        * @param aInstance Informatio of the icon, which data is
        * fetched from SIM.
        * @param aInstance Contains the icon data from SIM in returning of the
        * method.
        * @param aObserver Observer for icon image get completion.
        */
        virtual void GetIconInstance(
            const RSat::TInstanceInfoV3& aInstanceInfo,
            TDes8& aInstance,
            MSatIconObserver& aObserver ) = 0;

        /**
        * Gets color lookup table of the icon.
        * @param aInstanceNumber Image instance number.
        * @param aOffset Offset of the clut.
        * @param aClut Contains the Clut of the icon in returning of the
        * method.
        * @param aObserver Observer for clut get completion.
        */
        virtual void GetClut(
            TUint aInstanceNumber,
            TUint aOffset,
            TDes8& aClut,
            MSatIconObserver& aObserver ) = 0;
        /*
         * Set a mode to determine whether to fetch icons from NAA syncronously
         * or asynchronously.
         *
         * @param aMode Synchronous or asynchronous mode. Icons would be
         * fetched from NAA in sychronous mode if SAT UI was closed by
         * using the End Key. Otherwise, in asynchronous mode.
         * @return None
         */
        virtual void SetIconFetchingMode( TBool aMode ) = 0;

    protected:

        // Constructor
        MSatSIconAPI() {};

    private:

        // Prohibit copy constructor if not deriving from CBase.
        MSatSIconAPI( const MSatSIconAPI& );

        // Prohibit assigment operator if not deriving from CBase.
        MSatSIconAPI& operator=( const MSatSIconAPI& );

    };

#endif      // MSATSICONAPI_H

// End of File
