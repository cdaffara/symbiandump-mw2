/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Connection to ETEL.
*
*
*/


#ifndef     PSETTELEPHONY_H
#define     PSETTELEPHONY_H

//  INCLUDES
#include <e32base.h>
#include "nwdefs.h"

//  CONSTANTS  
const TInt KPsetMaxNameLength = 50;

//  FORWARD DECLARATIONS
class RTelServer;
class RMobilePhone;
class RMobileLine;

//  CLASS DEFINITIONS 
/**
*  CPsetTelephony class is used to connect phone settings to ETEL.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   CPsetTelephony : public CBase
    {

    public:        

        /**
        * Symbian OS 2-phase constructor.
        *
        * @param aPhone Phone object.
        * @param aTsyName TSY module name.
        * @return Created CPsetTelephony object.
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        static CPsetTelephony* NewL( RMobilePhone& aPhone, 
            const TDesC* aTsyName = NULL );
        
        /* Destructor */
        ~CPsetTelephony();

    public: // new functions
         
        /**
        * Checks that correct line for diverting is open. If not, lines
        * are gone through and when one with correct capabilities is found,
        * it is opened.
        *
        * @param aServiceGroup Service group for which line is checked.
        * @param aPhone Phone object.
        * @param aLine Line to check.
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        static void CheckLineModeL( const TServiceGroup aServiceGroup, 
            RMobilePhone* aPhone, RMobileLine* aLine );

    private:    // constructors

        //2nd phase constructor.
        void ConstructL( const TDesC* aTsyName );

        //C++ constructor.
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        CPsetTelephony ( RMobilePhone& aPhone );

    private: // new

        //Makes a ETEL connection.
        TInt ConnectToEtelL( const TDesC* aTsyName );

        //Sets caps.
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        static TUint32 SetCapFlagsL( const TServiceGroup aServiceGroup, TBool& aMode,
            RLine::TCaps& aCaps );

    private: //member data

        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        // ETEL server
        RTelServer*              iTelServer;
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        // TSY Phone
        RMobilePhone*            iPhone;
        // Connection to ETEL has been made by this module.
        TBool                    iOwnConnection;
        // Name of the TSY module
        TBuf<KPsetMaxNameLength> iTsyName;
    };
#endif //PSETTELEPHONY_H
// End of File
