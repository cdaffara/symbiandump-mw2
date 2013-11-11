/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provide common utility function for all test cases
*
*/

// INCLUDE FILES
#include <bautils.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxmediacollectiondetaildefs.h>
#include <mpxcollectionpath.h>
#include <mpxmedia.h>
#include "testutility.h"

// ----------------------------------------------------------------------------------------------------------
// Test whether aContainer containing all the text object in aArray with aAttr attribues in order
// ----------------------------------------------------------------------------------------------------------
//
TBool TestUtility::MatchMediaArrayTextObject(const CMPXMedia& aContainer, const MDesC16Array& aArray, 
                                             TMPXAttribute& aAttr)
    {
    TBool match = ValidateMediaArrayObject(aContainer, aArray.MdcaCount());
    if( match )
        {
        CMPXMediaArray* contArray = aContainer.Value<CMPXMediaArray>( TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayContents) );
        TInt contCnt = contArray->Count();
            
        for(TInt i = 0; i < contCnt; i++)
            {
            const CMPXMedia* contItem = ( *contArray )[i];
            const TDesC& contText = contItem->ValueText( aAttr );
            if(contItem->IsSupported( aAttr ) == EFalse || aArray.MdcaPoint( i ) != contText )
                {
                match = EFalse;
                break;
                }
            }
        }
    return match;
    }

// ----------------------------------------------------------------------------------------------------------
// Test whether aMedia1 and aMedia2 support same attribute
// ----------------------------------------------------------------------------------------------------------
//
TBool TestUtility::ValidateMediaObject(const CMPXMedia& aMedia1, const CMPXMedia& aMedia2,
                                       TMPXAttribute& aAttr)
    {
    TBool match = ETrue;    // assume true
    if(!aMedia1.IsSupported(aAttr) || !aMedia2.IsSupported(aAttr))
        {
        match = EFalse;
        }
    return match;
    }
    
// ----------------------------------------------------------------------------------------------------------
// Test whether aMedia1 and aMedia2 support same array attribute
// ----------------------------------------------------------------------------------------------------------
//
TBool TestUtility::ValidateMediaArrayObject(const CMPXMedia& aMedia1, const CMPXMedia& aMedia2)
    {
    TBool match = ETrue;    // assume true
    TMPXAttribute arrayAttr(KMPXMediaIdContainer,EMPXMediaArrayContents);
    TMPXAttribute arrayCntAttr(KMPXMediaIdContainer,EMPXMediaArrayCount);
    if(!aMedia1.IsSupported( arrayAttr )     ||
       !aMedia1.IsSupported( arrayCntAttr )  ||
       !aMedia2.IsSupported( arrayAttr )          ||
       !aMedia2.IsSupported( arrayCntAttr)        )
        {
        match = EFalse;
        }
    else
        {
        // TODO: commented part because a minor bug in CMPXCollectionEngine::ListPluginsL
        //       where "count" doesn't match array's count
        //TInt m1Cnt = *aMedia1.Value<TInt>( arrayCntAttr );
        //TInt m2Cnt = *aMedia2.Value<TInt>( arrayCntAttr );
        //if(m1Cnt == m2Cnt)
        //    {
            CMPXMediaArray* m1Array = aMedia1.Value<CMPXMediaArray>( arrayAttr );
            CMPXMediaArray* m2Array = aMedia1.Value<CMPXMediaArray>( arrayAttr );
            
            //if(m1Array->Count() != m1Cnt || m2Array->Count() != m2Cnt)
            if(m1Array->Count() != m2Array->Count())
                {
                match = EFalse;
                }
        //    }
        //else
        //    {
        //    match = EFalse;
        //    }
        }
    return match;
    }
    
// ----------------------------------------------------------------------------------------------------------
// Test whether aMedia is media array and same count as aArray
// ----------------------------------------------------------------------------------------------------------
//
TBool TestUtility::ValidateMediaArrayObject(const CMPXMedia& aMedia, TInt aArrayCnt)
    {
    TBool match = ETrue;    // assume true
    TMPXAttribute arrayAttr(KMPXMediaIdContainer,EMPXMediaArrayContents);
    TMPXAttribute arrayCntAttr(KMPXMediaIdContainer,EMPXMediaArrayCount);
    if(!aMedia.IsSupported( arrayAttr )     ||
       !aMedia.IsSupported( arrayCntAttr )  )
        {
        match = EFalse;
        }
    else
        {
        // TODO: commented part because a minor bug in CMPXCollectionEngine::ListPluginsL
        //       where "count" doesn't match array's count
        //TInt contCnt = *aMedia.Value<TInt>( arrayCntAttr );
        //if(contCnt == aArrayCnt)
        //    {
            CMPXMediaArray* contArray = aMedia.Value<CMPXMediaArray>( arrayAttr );
            TInt contArrayCnt = contArray->Count();
            if(contArrayCnt != aArrayCnt)
                {
                match = EFalse;
                }
        //    }
        //else
        //    {
        //    match = EFalse;
        //    }
        }
    return match;
    }
   
/*
// ----------------------------------------------------------------------------------------------------------
// Log the content of collection path
// ----------------------------------------------------------------------------------------------------------
//
void TestUtility::LogCollectionPath(CMPXCollectionPath& aPath, CStifLogger* aLogger)
    {
    TInt levels = aPath.Levels();
    TInt count = aPath.Count();
    aLogger->Log(_L("Collection Path debug: levels=  %d, count = %d"), levels, count);
    for (TInt i=0; i<levels;++i)
        {
        aLogger->Log(_L("Collection Path debug: Level[%d] = (index %d, id 0x%08x)"), i, 
                                                                             aPath.Index(i), 
                                                                             aPath.Id(i) );
        }
    if (count > 0)
        {
        TInt index = aPath.Index();
        CMPXCollectionPath* dpath = const_cast<CMPXCollectionPath*>(&aPath);
        dpath->SetToFirst();
        TInt c = 0;
        do
            {
            aLogger->Log(_L("Collection Path debug top level: item %d = id 0x%08x"), c, dpath->Id());
            } while (++(*dpath) && ++c < 15 );
        dpath->Set(index);
        }
    }
*/
