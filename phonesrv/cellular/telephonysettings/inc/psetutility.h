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
* Description:  Utility tool class to change between ETEL and GSM codes.
*
*/


#ifndef PSETUTILITY_H
#define PSETUTILITY_H

//  INCLUDES
#include <e32base.h>
#include <nwdefs.h>
#include <etelmm.h>

// CLASS DECLARATION
/**
*  Class is used to translate between ETEL internal basic service codes and
*  basic service codes defined in the GSM standard. Class is also used to 
*  convert multimode Etel parameters to existing parameter types.
*
*  @lib phonesettings.dll
*  @since 2.0
*/
class PSetUtility
    {
    public: // New functions
        
        /**
        * ChangeToEtelInternal maps GSM bsc codes to ETEL equivalents.
        * @param aBsc Basic service code as in GSM standard.
        * @return Basic service code as defined inside Etel.
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        static RMobilePhone::TMobileService ChangeToEtelInternal( 
            const TBasicServiceGroups& aBsc );

        /**
        * ChangeToGSM maps ETEL bsc codes to GSM equivalents.
        * @param aBsc Basic service code as defined inside Etel.
        * @return Basic service code as in GSM standard.
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        static TBasicServiceGroups ChangeToGSM( 
            const RMobilePhone::TMobileService& aBsc );
        /**
        * GetChangeInfoStatus maps multimode ETel parameter to
        * PhoneSettings internal parameter type. This is to ensure
        * binary compatibility of dependent modules.
        * @param aAction divert action type.
        * @return TDivertingStatus divert action type
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/        
        static TDivertingStatus GetChangeInfoStatus( 
            RMobilePhone::TMobilePhoneServiceAction& aAction );

        /**
        * GetDivertReason maps multimode ETel parameter to
        * PhoneSettings internal parameter type. This is to ensure
        * binary compatibility of dependent modules.
        * @param aCondition divert condition.
        * @return TCallDivertingCondition divert condition
        */
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        static RMobilePhone::TMobilePhoneCFCondition 
            GetDivertReason( TCallDivertingCondition aCondition );

        static TCallDivertingCondition
            GetDivertReason( RMobilePhone::TMobilePhoneCFCondition aCondition );

        static RMobilePhone::TMobileService VerifyAltLineUseL();
    };
    
#endif      // PSETUTILITY_H

// End of File
