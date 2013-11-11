/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CPSetSubscriberIdCheck class.
*
*/


#ifndef CPSETSIMCHECK_H
#define CPSETSIMCHECK_H

//  INCLUDES
#include    <e32base.h>
#include    <etelmm.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * Class is used for checking SIM imsi.
 *
 *  @lib phonesettings
 *  @since S60 v5.1
 */
NONSHARABLE_CLASS( CPSetSubscriberIdCheck ): public CBase
    {
    public:
        
        /**
        * Symbian OS 2-phase Constructor.
        * @return Created CPSetSIMCheck object.
        */
        static CPSetSubscriberIdCheck* NewL();
               
        /* 
        * Destructor.
        */
        ~CPSetSubscriberIdCheck();
        
    public:
    
       TBool DoesSIMSupportDualActivation();
    
    private: // constructors
       
        /**
        * Default constructor
        */
        CPSetSubscriberIdCheck();
        
    private:
        
        RMobilePhone::TMobilePhoneSubscriberId iImsi;
    };

#endif      // CPSETSIMCHECK_H
            
// End of File
