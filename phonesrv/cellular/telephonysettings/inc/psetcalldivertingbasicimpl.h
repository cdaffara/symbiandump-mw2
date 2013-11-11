/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Divert basic implementation.
*
*/


#ifndef CPSETCALLDIVERTINGBASICIMPL_H
#define CPSETCALLDIVERTINGBASICIMPL_H

//  INCLUDES
#include <e32base.h>
#include <psetcalldiverting.h> 
#include <mmretrieve.h>
#include <ctsydomaincrkeys.h>
#include "msssettingsobserver.h" 
#include "nwdefs.h"             
#include "psetconstants.h" 
#include "psetcalldivertingbase.h" 

// CLASS DECLARATION
class CPsetSAObserver;
class CDesC16ArrayFlat;
class RSSSettings;
class MPsetDivertObserver;
class MPsetRequestObserver;
class RVmbxNumber;
class CRepository;
class TCallDivertNotifySetting;

/**
 * CPSetCallDivertingBasicImpl implements basic divert funcationality.
 *
 *  @lib phonesettings
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS(CPSetCallDivertingBasicImpl) : public CPSetCallDivertingBase
    {
    public: // Constructors and destructors.

        /**
        * Two-phased constructor.
        *
        * @return new instance of the class.
        */
        static CPSetCallDivertingBasicImpl* NewL(
                MPsetDivertObserver& aObserver, 
                RMobilePhone& aPhone,
                CPsetCallDiverting* aDivert );

        /**
        * Destructor.
        */
        ~CPSetCallDivertingBasicImpl();

    public: // From base class.
        
        /**
        * @see MCallDiverting.
        */ 
        void SetDivertingL( const TCallDivertSetting& aDivert,  
                            TBasicServiceGroups aBsc );        

    public: //from base class CActive

        void RunL();
        
        void DoCancel();
        
        TInt RunError( TInt aError );
            
    protected:
        
        /**
        * C++ constructor.
        */
        CPSetCallDivertingBasicImpl( RMobilePhone& aPhone, CPsetCallDiverting* aDivert );

        /**
        * Symbian OS constructor.
        */
        void ConstructL( MPsetDivertObserver& aObserver );
    
    private:
        
        void HandleSANotificationL( 
            TBool aVmbxDivert, TDivertingStatus& aCfStatus );
        
        TBool SetVoiceDivert( const TCallDivertSetting& aDivert,
            const TBasicServiceGroups aBsc );
        
        TSelectedLine GetSAAls();
        
    private:
        // Memorize requested service group
        TServiceGroup iRequestedServiceGroup;
    };

#endif      // CPSETCALLDIVERTINGBASICIMPL_H 

// End of File
