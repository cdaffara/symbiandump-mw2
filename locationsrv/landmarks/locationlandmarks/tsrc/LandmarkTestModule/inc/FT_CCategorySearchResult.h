/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*   ?description_line
*
*/


#ifndef CCategorySearchResult_H
#define CCategorySearchResult_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmark.h>

#include "Ft_CSearchResultBase.h"



class CCategorySearchResult : public CSearchResultBase
    {

    public:  // Constructors and destructor
        
         /**
        * Two-phased constructor.
        */
        static CCategorySearchResult* NewL(const TDesC& aDes); 

        /**
        * Destructor.
        */
        ~CCategorySearchResult();      

    public: 

         TPtrC TextCriteria() const;
        
         const RArray<TUint>& PositionFields() const;

    private: 
        
        CCategorySearchResult();

        void ConstructL(const TDesC& aDes);
        

    private:

         // By default, prohibit copy constructor
        CCategorySearchResult(const CCategorySearchResult&);
        // Prohibit assigment operator
        CCategorySearchResult& operator=(const CCategorySearchResult&);

        void ParsePositionFields(const TDesC& aDes);

        void Print();
        
        TBool Equals32(const TReal aValue1, const TReal aValue2, const TReal aAccuracy);
        
    private: // Data
        
        enum TValueIndex
			{
            ETextCriteria           =0,	
            ELmSearchResult         =1,
            ERedefined              =2
			};
        
        HBufC*                      iTextCriteria;
        RArray<TUint>               iPositionFields;
        
    };

#endif      // CCategorySearchResult_H
            
// End of File
