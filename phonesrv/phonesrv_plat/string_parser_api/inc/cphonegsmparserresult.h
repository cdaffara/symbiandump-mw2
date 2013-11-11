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
* Description:  Instances will contain the results of the parsing.
*
*/


#ifndef CPHONEGSMPARSERRESULT_H
#define CPHONEGSMPARSERRESULT_H

// INCLUDES
#include    <e32base.h>

// CLASS DECLARATION

/**
* Instances will contain the results of the parsing.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmParserResult )
    : public CBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmParserResult* NewL();
        
        /**
        * Destructor.
        */
        ~CPhoneGsmParserResult();

    public: // New functions
        
        /**
        * Reserve parameters.
        * 
        * @param aAmount It is the amount of parameters to reserve.
        */
        void ReserveParametersL( TUint aAmount );

        /**
        * Sets state to initial.
        */
        void ClearL();

        /**
        * Returns uid.
        */
        inline TUint Uid() const;

        /**
        * Returns auxilirary information.
        */
        inline TUint AuxInformation() const;

        /**
        * Returns the count of parameters.
        */
        inline TUint CountParameters() const;

        /**
        * Returns parameter at given position.
        *
        * @param aIndex It is the position.
        * @return Returns the parameter.
        */
        const TDesC& ParameterAt( TInt aIndex ) const;

        /**
        * Sets the uid.
        *
        * @param aUid It is the new uid.
        */
        void SetUid( TUint aUid );

        /**
        * Sets auxilirary information.
        *
        * @param aAuxInformation It is the new aux information.
        */
        void SetAuxInformation( TUint aAuxInformation );

        /**
        * Adds a parameter. 
        *
        * It will leave if there is not enough memory.
        *
        * @param aParameter It is the parameter.
        */
        void AddParameterL( const TDesC& aParameter );
            
    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmParserResult();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CPhoneGsmParserResult( const CPhoneGsmParserResult& );
        // Prohibit assigment operator
        CPhoneGsmParserResult& operator= ( const CPhoneGsmParserResult& );

    private:    // Data

        // Unique identifier, type of result.
        TUint iUid;

        // Additional information, specific to type.
        TUint iAuxInformation;

        // Array of parameters, pointer descriptors to original string.
        CArrayFixFlat< TPtrC >* iParameters;

    };

#include    "cphonegsmparserresult.inl" 

#endif      // CPHONEGSMPARSERRESULT_H
            
// End of File
