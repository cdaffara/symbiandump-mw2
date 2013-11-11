/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  It encapsulates all dependencies.
*
*/


#ifndef CPHONEPARSERFEATURES_H
#define CPHONEPARSERFEATURES_H

#include    <e32base.h>

class CPhCltEmergencyCall;
class CRepository;

// INCLUDES
#include    <mphcltemergencycallobserver.h> 

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* It is extension for options. 
*
* @since 1.2
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneParserFeatures )
    : public CBase, public MPhCltEmergencyCallObserver
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneParserFeatures* InstanceL();
        
       /**
        * To release resources.
        */
        static void Free();

    public:  // From MPhCltEmergencyCallObserver   
        
        /**
        * It is called whenever client's dial request is completed.
        *
        * @param aStatus error code of the success of the operation.
        */       
        void HandleEmergencyDialL( const TInt aStatus );

    public: // New functions
        
        /**
        * Checks if two digit calling is enabled.
        *
        * @return ETrue if supported.
        */
        static TBool TwoDigitCallingEnabled();
            
        /**
        * Checks if zero send should make a call.
        *
        * @return ETrue if enabled.
        */
        static TBool IsZeroDialEnabled();

        /**
        * Checks if feature is supported.
        *
        * @param aFeature feature.
        * @return ETrue if supported.
        */
        static TBool FeatureSupported( TInt aFeature );
        
        /**
        * Checks if given number is emergency.
        *
        * @param aNumber Number to be queried.aNumber might contain prefix (SS code)
        *                and/or postfix (DTMF part).
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumber. 
        *
        * @param aEmergencyNumber Matched emergencynumber without prefix or 
        *                postfix is returned in this parameter. The aMatchedEmergencyNumber 
        *                parameter should be a buffer and the clients are
        *                provided with atypedef called TPhCltEmergencyNumber.
        * 
        * @returns ETrue if emergency.
        */
        static TBool IsEmergencyNumber( 
            const TDesC& aNumber, 
            TDes& aEmergencyNumber );
            
        /**
        * To set stub for testing purposes.
        *
        * @param aStub test stub.
        */       
#ifdef __WINS__
        static void SetInstance( CPhoneParserFeatures* aStub );
#endif // __WINS__
    

    private:

        /**
        * C++ default constructor.
        */
        CPhoneParserFeatures();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Non leaving instance method.
        */
        static CPhoneParserFeatures* Instance();
        
        /**
        * Destructor.
        */
        ~CPhoneParserFeatures();

        /**
        * Checks if given number is emergency.
        *
        * @param aNumber Number to be queried.aNumber might contain prefix (SS code)
        *                and/or postfix (DTMF part).
        *                The aNumber parameter should be a buffer and the 
        *                clients are provided with a typedef called 
        *                TPhCltEmergencyNumber. 
        *
        * @param aEmergencyNumber Matched emergencynumber without prefix or 
        *                postfix is returned in this parameter. The aMatchedEmergencyNumber 
        *                parameter should be a buffer and the clients are
        *                provided with atypedef called TPhCltEmergencyNumber.
        * 
        * @returns ETrue if emergency.
        */
        virtual TBool IsEmergencyPhoneNumber( 
            const TDesC& aNumber, 
            TDes& aEmergencyNumber );
            
        /**
        * Checks if feature is supported.
        *
        * @param aFeature feature.
        * @return ETrue if supported.
        */
        virtual TBool IsFeatureSupported( TInt aFeature );
        
        virtual TInt GetTelephoneVariant();

    private:    // Data


        // Owned: Phone client emergency number session.
        CPhCltEmergencyCall* iEmergencyCall;
 
        CRepository* iRepository;
        
    };

#endif      // CPHONEPARSERFEATURES_H
            
// End of File
