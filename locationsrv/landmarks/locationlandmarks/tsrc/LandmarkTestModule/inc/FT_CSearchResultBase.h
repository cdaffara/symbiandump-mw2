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



#ifndef CSEARCHRESULTBASE_H
#define CSEARCHRESULTBASE_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmark.h>



class CSearchResultBase : public CBase
    {

    public:  // Constructors and destructor
        
        CSearchResultBase(); 

        /**
        * Destructor.
        */
        virtual ~CSearchResultBase();      

    public: 

         RArray<TPosLmItemId>& SearchResult();

         TBool FindSearchResult(const TPosLmItemId aId);
         
         TBool Redefined() const;
         
         TUint Attributes() const;

    protected:

            void ParseSearchResult(const TDesC& aDes);

            void ParseAttributesL(const TDesC& aDes);

    private:

         // By default, prohibit copy constructor
        CSearchResultBase(const CSearchResultBase&);
        // Prohibit assigment operator
        CSearchResultBase& operator=(const CSearchResultBase&);
      
    protected: // Data
        
        TBool                        iRedefined;     

        TUint32                     iAttributes; 

        RArray<TPosLmItemId>        iSearchResult;

    private: // Data
        

    };

#endif      // CSearchResultBase_H
            
// End of File
