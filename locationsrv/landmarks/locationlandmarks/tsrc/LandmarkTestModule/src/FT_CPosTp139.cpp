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


//  INCLUDES

#include "FT_CPosTp139.h"
#include <EPos_CPosLandmark.h> 


           
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp139::GetSortAttribute
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp139::GetSortAttribute()
    {
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
   
    CPosLandmark::TAttributes att;
                    
    for (TInt i=0; i<6; i++)
        {
         if (i==0)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
            att = sortPref.LandmarkAttributeToSortBy();
            if(att == CPosLandmark::ELandmarkName)
            	{
            	iLog->Log(_L("The landmark attribute to sort by is :ELandmarkName"));	
           		 }
              }
        else if (i==1)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::EPosition, TPosLmSortPref::EDescending);
            att = sortPref.LandmarkAttributeToSortBy();
            if(att == CPosLandmark::EPosition )
            	{
            	iLog->Log(_L("The landmark attribute to sort by is :EPosition"));	
           		 }
            }
        else if (i==2)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ECoverageRadius, TPosLmSortPref::EAscending);
            att = sortPref.LandmarkAttributeToSortBy();
            if(att == CPosLandmark::ECoverageRadius)
            	{
            	iLog->Log(_L("The landmark attribute to sort by is :ECoverageRadius"));	
           		 }
            }
        else if (i==3)
            {
            
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::ECategoryInfo, TPosLmSortPref::EDescending);
            att = sortPref.LandmarkAttributeToSortBy();
            if(att == CPosLandmark::ECategoryInfo)
            	{
            	iLog->Log(_L("The landmark attribute to sort by is :ECategoryInfo"));	
           		 }
            }
        else if (i==4)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::EIcon, TPosLmSortPref::EAscending);
            
            att = sortPref.LandmarkAttributeToSortBy();
            if(att == CPosLandmark::EIcon)
            	{
            	iLog->Log(_L("The landmark attribute to sort by is :EIcon"));	
           		 }
            }
            
        else if (i==5)
            {
            sortPref.SetSortByLandmarkAttribute(CPosLandmark::EDescription, TPosLmSortPref::EDescending);
            
            att = sortPref.LandmarkAttributeToSortBy();
            if(att == CPosLandmark::EDescription)
            	{
            	iLog->Log(_L("The landmark attribute to sort by is :EDescription"));	
           		 }
            }
            
  		 }
    }
    

   
    
//  End of File
