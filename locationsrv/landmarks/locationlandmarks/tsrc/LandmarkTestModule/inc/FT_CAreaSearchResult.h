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



#ifndef CAREASEARCHRESULT_H
#define CAREASEARCHRESULT_H

//  INCLUDES
#include <e32base.h>
#include <EPos_CPosLandmark.h>

#include "Ft_CSearchResultBase.h"



class CAreaSearchResult : public CSearchResultBase
    {

    public:  // Constructors and destructor
        
         /**
        * Two-phased constructor.
        */
        static CAreaSearchResult* NewL(const TDesC& aDes); 

        /**
        * Destructor.
        */
        ~CAreaSearchResult();
        
    public: 

        TReal64 SouthLat() const;

        TReal64 NorthLat() const;
        
        TReal64 WestLong() const;
        
        TReal64 EastLong() const;

        TInt ErrorCode() const;

        void Print(TDes& aDes);

    private: 
        
        CAreaSearchResult();

        void ConstructL(const TDesC& aDes);
        

    private:

         // By default, prohibit copy constructor
        CAreaSearchResult(const CAreaSearchResult&);
        // Prohibit assigment operator
        CAreaSearchResult& operator=(const CAreaSearchResult&);

    private: // Data
        
        enum TValueIndex
			{
            ESouthLat           =0,	
            ENorthLat           =1,	
            EWestLong           =2,
            EEastLong           =3,
            EErrorCode          =4,
            ERedefined          =5,
            ESearchResult       =6
			};
        
        TInt        iErrorCode;
        
        TReal64     iSouthLat;
        TReal64     iNorthLat;
        TReal64     iWestLong;
        TReal64     iEastLong;

    };

#endif      // CAREASEARCHRESULT_H
            
// End of File
