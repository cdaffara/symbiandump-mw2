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



#ifndef CCategoryLandmarkSearchResult_H
#define CCategoryLandmarkSearchResult_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmark.h>

#include "Ft_CSearchResultBase.h"



class CCategoryLandmarkSearchResult : public CSearchResultBase
    {

    public:  // Constructors and destructor
        
         /**
        * Two-phased constructor.
        */
        static CCategoryLandmarkSearchResult* NewL(const TDesC& aDes); 

        /**
        * Destructor.
        */
        ~CCategoryLandmarkSearchResult();      

    public: 

         TPtrC CategoryName() const;

         TPosLmItemId CategoryId() const;
        
         const RArray<TUint>& PositionFields() const;

    private: 
        
        CCategoryLandmarkSearchResult();

        void ConstructL(const TDesC& aDes);
        

    private:

         // By default, prohibit copy constructor
        CCategoryLandmarkSearchResult(const CCategoryLandmarkSearchResult&);
        // Prohibit assigment operator
        CCategoryLandmarkSearchResult& operator=(const CCategoryLandmarkSearchResult&);

        

        void Print();
        
        
        
    private: // Data
        
        enum TValueIndex
			{
            ECategoryName           =0,	
            ECategoryId             =1,	
            ELmSearchResult         =2,
            ERedefined              =3
			};
        
        HBufC*                      iCategoryName;
        RArray<TUint>               iPositionFields;
        TPosLmItemId                iCategoryId;
        
    };

#endif      // CCategorySearchResult_H
            
// End of File
