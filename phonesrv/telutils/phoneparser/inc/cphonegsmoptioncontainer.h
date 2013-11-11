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
* Description:  It is container for options, boolean flags with identifiers.
*                Using these flags, parsing process can be configured.
*
*/


#ifndef CPHONEGSMOPTIONCONTAINER_H
#define CPHONEGSMOPTIONCONTAINER_H

// INCLUDES
#include    "cphonegsmoptioncontainerbase.h" 

// FORWARD DECLARATIONS

// For two digit calling, some options are defined
// elsewhere.
class CPhoneGsmOptionContainerExt;

// CLASS DECLARATION

/**
* It is container for options. Each option is identified by 
* a single uid and each option has a boolean status.
*
* @since 1.0
* @lib phoneparser.lib
*/
NONSHARABLE_CLASS( CPhoneGsmOptionContainer )
        : public CPhoneGsmOptionContainerBase    
    {
    public:  // Constructors and destructor      
        /**
        * Two-phased constructor.
        */
        static CPhoneGsmOptionContainer* NewL();
        
        /**
        * Destructor.
        */
        ~CPhoneGsmOptionContainer();

    public: // New functions
        
        /**
        * Checks if option is defined.
        * 
        * @param aOptionUid It is the uid of the option.
        * @return Returns ETrue, only if it is defined.
        */
        TBool IsOptionDefined( TInt aOptionUid ) const;

        /**
        * Defines an option.
        *
        * Method panics if option is already defined. Note that 
        * the method might leave.
        *
        * @param aOptionUid It is the uid of the new option.
        * @param aStatus It is the initial status for the option.
        */
        void DefineOptionL( TInt aOptionUid, TBool aStatus );
            
        /**
        * This method can be used to make overrides to definition of phone numbers.
        *
        * For example, there can be two digit emergency numbers which are normally
        * considered as ussd requests.
        *
        * Note: this can return directly EFalse in which case standard
        * GSM practices are applied.
        *
        * @param aNumber string to be checked.
        * @return ETrue if should be considered as phone number.
        */
        TBool IsEmergencyNumber( const TDesC& aNumber, TDes& aEmergencyNumber );

        /**
        * From CPhoneGsmOptionContainerBase. 
        * Sets status of option.
        *
        * Method panics if option is not defined.
        *
        * @param aOptionUid It is the uid of the option.
        * @param aStatus It is the new status for the option.
        */
        void SetOptionStatus( TInt aOptionUid, TBool aStatus );   
                
        /**
        * From CPhoneGsmOptionContainerBase.
        * Finds status of option.
        *
        * Method panics if option is not defined.
        *
        * @param aOptionUid It is the uid of the option.
        * @return Returne status of the option.
        */
        TBool FindOptionStatus( TInt aOptionUid ) const;             

    private:

        /**
        * C++ default constructor.
        */
        CPhoneGsmOptionContainer();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Creates key.
        */
        static TKeyArrayFix MakeKey();

        // By default, prohibit copy constructor
        CPhoneGsmOptionContainer( const CPhoneGsmOptionContainer& );
        // Prohibit assigment operator
        CPhoneGsmOptionContainer& operator= ( const CPhoneGsmOptionContainer& );

    private:    // Data
        
        /**
        * It is the structure containing information about single option.
        */
        class TOptionItem 
            {
            public:
                TInt iUid;
                TBool iStatus;
            };

        /**
        * It is the type for array of options.
        */
        typedef CArrayFixFlat< TOptionItem > COptionArray;

        // Owned: array of options.
        COptionArray* iOptions;

        // Owned: option extension for two digit calling.
        CPhoneGsmOptionContainerExt* iExtension;

    };

#endif      // CPHONEGSMOPTIONCONTAINER_H
            
// End of File
