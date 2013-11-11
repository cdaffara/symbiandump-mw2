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
* Description:  
*       CPsetVariationProxy class is proxy class for 
*       central repository variation
*       (Call waiting/distinguish between not provisioned and not activated).
*  
*
*/


#ifndef PSETVARIATIONPROXY_H
#define PSETVARIATIONPROXY_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CRepository;

// CONSTANTS
const TUint KCallWaitingDistiquishNotProvisioned = 0x00000001;

// CLASS DECLARATION
/**
*  CPsetVariationProxy class is proxy class for central repository variation
*  @lib phonesettings.lib
*  @since 1.0
*/
NONSHARABLE_CLASS (CPSetVariationProxy): public CBase 
    {
    public: //constructor & destructor
    
        /* 
        * Symbian OS 2-phase Constructor. 
        * 
        * @param aUid identifing the central repository UID.
        * @param aId central repository ID.
        * @return the created instance.
        */
        static CPSetVariationProxy* NewL( const TUid& aUid, 
                                          const TUint aId );
        
        /* Destructor */
        ~CPSetVariationProxy();
        
    public:
        
        /**
        * Check from member variable 'iFeature' is the requested feature active.
        *
        * @param aFeature is uid for used central repository variable.
        * @return returns TBool type ETrue/EFalse is current feature enabled or not
        */
        TBool IsFeatureEnabled( TUint aFeature ) const;
        
    private:
        
        /**
        * Constructor
        *
        * Open CenRep, read data into 'iFeatures' and close CenRep 
        */
        void ConstructL( const TUid& aUid, const TUint aId );   
        
    private:
        
        /* Default constructor */
        CPSetVariationProxy();
        
    private:
        
        // Saves cen rep data readed in ConstructL()
        TInt iFeature;
    };

#endif      // PSETVARIATIONPROXY_H
 