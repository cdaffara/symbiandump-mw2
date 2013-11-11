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

#include "FT_CPosTp18.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLMPartialReadParameters.h>  
#include <EPos_CPosLMOperation.h> 
#include <LbsPosition.h>


// CONSTANTS
const TInt KNoPreparePartial = 40;
_LIT(KTakePreparedPartialErr, "Method TakePreparedPartialLandmarksL doesn't leave with KErrNotFound");
_LIT(KNoPreparedPartialErr, "Method TakePreparedPartialLandmarksL returns incorrect no. landmarks");
_LIT(KNoPreparedPartialErr2, "Method TakePreparedPartialLandmarksL returns incorrect landmarks");

_LIT(KTp18DbFile, "TP18Test.ldb");
_LIT(KTp18DbFile1, "TP18Test1.ldb");

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp10::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::CloseTest()
    {
    if (iArrayPtr)
        {
        iArrayPtr->ResetAndDestroy();
        }
    delete iArrayPtr;
    iArrayPtr = NULL;
    
    delete iDatabase;
    iDatabase = NULL;
    
    delete iOperation;
    iOperation = NULL;
    }

// ---------------------------------------------------------
// CPosTp18::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::StartL()
    {
    _LIT(KNotPartialErr, "Landmark is not a partial landmark");
    CopyTestDbFileL(KTp18DbFile);
    iDatabase = CPosLandmarkDatabase::OpenL(KTp18DbFile);
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    
    // Test some non-existing landmark ids
    TPosLmItemId id = KPosLmNullItemId;
    TRAPD(err, iDatabase->ReadPartialLandmarkLC(id));
    if (err != KErrNotFound)
    {
    	iLog->Log(_L("ReadPartialLandmarkLC should leave with KErrnotFound"));
    	User::Leave(err);
    }
    id = 2000;
    TRAP(err, iDatabase->ReadPartialLandmarkLC(id));
    if (err != KErrNotFound) 
    {
    	iLog->Log(_L("ReadPartialLandmarkLC should leave with KErrnotFouns"));
    	User::Leave(err);
    }

    TestAllPartialAttributesL();
    
    TestPartialPositionFieldsL();

    PreparePartialLandmarksL();

    PreparePartialLandmarksAsyncL(EAsynchronous);

    PreparePartialLandmarksAsyncL(EWaitForRequest);

    iLog->Log(_L("TESTS BELOW ARE TESTING CANCEL"));
    PreparePartialLandmarksAsyncAndCancelL();
    
    iLog->Log(_L("TEST BELOW IS TESTING CANCEL IN CALLBACK"));
    PreparePartialLandmarksAsyncAndCancelL(ETrue);

    TUint attr=CPosLandmark::ELandmarkName;
    CPosLmPartialReadParameters* partialParam = iDatabase->PartialReadParametersLC();
    partialParam->SetRequestedAttributes(attr);    
    CPosLandmark* lm = GetPartialLandmarkLC(1, *partialParam);
    CheckPartialLandmarkL(*lm, attr);
    
    CPosLandmark* copy = CPosLandmark::NewLC(*lm);
    
    iDatabase->AddLandmarkL(*copy);
    
    CPosLandmark* lm2 = iDatabase->ReadLandmarkLC(copy->LandmarkId());
  
    AssertTrueSecL(!lm2->IsPartial(), KNotPartialErr);
    CleanupStack::PopAndDestroy(4,partialParam);
	}

// ---------------------------------------------------------
// CPosTp18::TestAllPartialAttributes
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::TestAllPartialAttributesL()
    {
     iLog->Log(_L("TestAllPartialAttributesL"));
     iLog->Log(_L("Testing GetPartialLandmarks with all combination of attributes requested"));
    _LIT(KNoAttrReqErr, "No attributes have been requested -> RequestedAttributes should return ENoAttribute");
    
    CPosLmPartialReadParameters* partialParam = iDatabase->PartialReadParametersLC();
    
    AssertTrueSecL(partialParam->RequestedAttributes() == CPosLandmark::ENoAttribute, KNoAttrReqErr);
    
    iDatabase->SetPartialReadParametersL(*partialParam);
    
    TPosLmItemId  id=1; // landmarks Id with all attr set

    TUint max = CPosLandmark::EDescription;
    max <<= 1;
    max -= 1;
    CPosLandmark* lm = NULL;
    
    TUint attr=0;
    for (attr=0; attr<=max; attr++)
        {
        partialParam->SetRequestedAttributes(attr);    
        lm = GetPartialLandmarkLC(id, *partialParam);
        CheckPartialLandmarkL(*lm, attr);
        CleanupStack::PopAndDestroy(lm);         
        }
   
    id = 2; // attr coverage radius not availible  
    attr = 0x003F; // req all
    partialParam->SetRequestedAttributes(attr);    
    lm = GetPartialLandmarkLC(id, *partialParam);
    attr = attr & 0x00FB; // remove coverage radius
    CheckPartialLandmarkL(*lm, attr);
    CleanupStack::PopAndDestroy(lm);         

    id = 3; // Description not available
    attr = 0x003F; // req all
    partialParam->SetRequestedAttributes(attr);    
    lm = GetPartialLandmarkLC(id, *partialParam);
    
   
    // After error report "ESLI-5ZDH4N Landmarks - Empty descriptors"
    // Even if name or description not has been set it will return empty string "" when reading
    // it from landmarks db, thus GetLandmarkDescription will KErrNone (instead of KErrNotFound as earlier)
    //attr = attr & 0x00DF; // remove description
    CheckPartialLandmarkL(*lm, attr);
    CleanupStack::PopAndDestroy(lm);

    id= 5; // IconName and IconIndex not availible
    attr = 0x003F; // req all
    partialParam->SetRequestedAttributes(attr);    
    lm = GetPartialLandmarkLC(id, *partialParam);
    attr = attr & 0x00EF; // remove IconName and IconIndex
    CheckPartialLandmarkL(*lm, attr);
    CleanupStack::PopAndDestroy(lm);         

    CleanupStack::PopAndDestroy(partialParam); 
    }

// ---------------------------------------------------------
// CPosTp18::TestPartialPositionFields
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::TestPartialPositionFieldsL()
    {
    iLog->Log(_L("TestPartialPositionFieldsL"));
    iLog->Log(_L("Testing GetPartialLandmarks with different combinations of positionfields requested"));
    _LIT(KNoFieldsReqErr, "No fields have been requested -> GetRequestedPositionFields should return 0");
        
    CPosLmPartialReadParameters* partialParam = CPosLmPartialReadParameters::NewLC(); 

    RArray<TUint> reqFields;
    partialParam->GetRequestedPositionFields(reqFields);
    CleanupClosePushL(reqFields);
    
    AssertTrueSecL(partialParam->GetRequestedPositionFields(reqFields) == 0, KNoFieldsReqErr);
    iDatabase->SetPartialReadParametersL(*partialParam);
    
        
    // Landmark Id with Street, PostalCode, City, LocationName, 
    // Country, MediaLinks, MediaLink1, MediaLink2 fields
    TPosLmItemId  id=2;
   
    AppendFields(reqFields);
    RArray<TBool> availFields;
    CleanupClosePushL(availFields);
    for(TInt i=0; i<8; i++)
        {
        availFields.Append(ETrue);
        }

    partialParam->SetRequestedPositionFields(reqFields);  
   
    CPosLandmark* lm = NULL;
    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);         

    // Landmark Id with PostalCode, City, LocationName, 
    // Country, MediaLinks, MediaLink1, MediaLink2 fields
    id=3;
    availFields[0] = EFalse;
    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);
    
    // Landmark Id with Street, City, LocationName, 
    // Country, MediaLinks, MediaLink1, MediaLink2 fields
    id=4;
    availFields[0] = ETrue;
    availFields[1] = EFalse;
    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);

    // Landmark Id with Street, PostalCode, LocationName, 
    // Country, MediaLinks, MediaLink1, MediaLink2 fields
    id=5;
    availFields[1] = ETrue;
    availFields[2] = EFalse;
    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);

    // Landmark Id with Street,PostalCode, City, 
    // Country, MediaLinks, MediaLink1, MediaLink2 fields
    id=6;
    availFields[2] = ETrue;
    availFields[3] = EFalse;
    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);

    // Landmark Id with Street,PostalCode, City, 
    // MediaLinks, MediaLink1, MediaLink2 fields
    id=7;
    availFields[3] = ETrue;
    availFields[4] = EFalse;
    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);

    // Landmark Id with Street,PostalCode, City, 
    // Country fields
    id=8;
    availFields[4] = ETrue;
    availFields[5] = EFalse;
    availFields[6] = EFalse;
    availFields[7] = EFalse;

    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);

    // Landmark Id with Street,PostalCode, City, 
    // Country, MediaLinks, MediaLink1 fields
    id=9;
    availFields[5] = ETrue;
    availFields[6] = ETrue;
    availFields[7] = EFalse;

    lm = GetPartialLandmarkLC(id, *partialParam);
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    CleanupStack::PopAndDestroy(lm);
    
    availFields[0] = EFalse;
    
    reqFields[0] = EPositionFieldMagneticCourse;

    partialParam->SetRequestedPositionFields(reqFields);
    lm = GetPartialLandmarkLC(id, *partialParam);
    
    reqFields[0] = EPositionFieldStreet; // To verify that street is not return when not requested
    CheckPartialLandmarkL(*lm, reqFields, availFields);
    
    CleanupStack::PopAndDestroy(4, partialParam);
   
    }

// ---------------------------------------------------------
// CPosTp18::PreparePartialLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::PreparePartialLandmarksL()
    {
    iLog->Log(_L("PreparePartialLandmarksL"));
    iLog->Log(_L("Testing Prepare and TakePartialLandmarks syncronously"));

    CPosLmPartialReadParameters* partialParam = CPosLmPartialReadParameters::NewLC(); 
    
    TUint attr = CPosLandmark::ELandmarkName;
    partialParam->SetRequestedAttributes(attr);
    iDatabase->SetPartialReadParametersL(*partialParam);
    
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    
    for (TInt i=0; i<KNoPreparePartial; i++)
        {
        TPosLmItemId id= i+1;
        ids.Append(id);
        }
    // Test with non-existing id
    ids.Append(13112321);
   
    //*** Add time measurements
    iOperation = iDatabase->PreparePartialLandmarksL(ids); 
    iOperation->ExecuteL();

    iArrayPtr = iDatabase->TakePreparedPartialLandmarksL(iOperation);
    AssertTrueSecL(iArrayPtr->Count() == ids.Count(), KNoPreparedPartialErr);

    for (TInt j=0; j<iArrayPtr->Count()-1; j++)
        {
        CheckPartialLandmarkL(*iArrayPtr->At(j), CPosLandmark::ELandmarkName);
        }
    AssertTrueSecL(iArrayPtr->At(iArrayPtr->Count()-1) == NULL, KNoPreparedPartialErr2); 

    TRAPD(err, iArrayPtr=iDatabase->TakePreparedPartialLandmarksL(iOperation));
    AssertTrueSecL(err == KErrNotFound, KTakePreparedPartialErr); 

    delete iOperation;
    iOperation = NULL;

    // landmarks with Name, coverage radius, description and fields street and country set 
    const TInt idsStart = 8;
    const TInt idsEnd = 28;

    attr = attr | CPosLandmark::ECoverageRadius | CPosLandmark::EDescription;
    partialParam->SetRequestedAttributes(attr);
    
    RArray<TUint> reqFields;
    CleanupClosePushL(reqFields);

    reqFields.Append(EPositionFieldStreet);
    reqFields.Append(EPositionFieldCountry);
    partialParam->SetRequestedPositionFields(reqFields);

    iDatabase->SetPartialReadParametersL(*partialParam);
    
    RArray<TBool> availFields;
    CleanupClosePushL(availFields);
    for(TInt jj=0; jj<2; jj++)
        {
        availFields.Append(ETrue);
        }
    
    ids.Reset();
    for (TInt k=idsStart; k<=idsEnd; k++)
        {
        ids.Append(k);    
        }
    
    iOperation = iDatabase->PreparePartialLandmarksL(ids);
    iOperation->ExecuteL();

    iArrayPtr->ResetAndDestroy();
    delete iArrayPtr; 
    iArrayPtr = NULL;

    iArrayPtr = iDatabase->TakePreparedPartialLandmarksL(iOperation);
    delete iOperation;
    iOperation = NULL;
 
    AssertTrueSecL(iArrayPtr->Count() == ids.Count(),KNoPreparedPartialErr);
    for (TInt ii=0; ii<iArrayPtr->Count()-1; ii++)
        {
        CheckPartialLandmarkL(*iArrayPtr->At(ii), attr);
        CheckPartialLandmarkL(*iArrayPtr->At(ii), reqFields, availFields);
        }
    
    CleanupStack::PopAndDestroy(4, partialParam);
        
    iArrayPtr->ResetAndDestroy();
    delete iArrayPtr; 
    iArrayPtr = NULL;

    // Test not setting any attributes with SetRequestedAttributes,
    // should only return empty landmarks
    delete iDatabase;
    iDatabase = NULL;
    CopyTestDbFileL(KTp18DbFile1);
	iDatabase = CPosLandmarkDatabase::OpenL(KTp18DbFile1);
    
    if (iDatabase->IsInitializingNeeded()) ExecuteAndDeleteLD(iDatabase->InitializeL());
       
    CleanupClosePushL(ids);
    ids.Append(1);
    ids.Append(2);
    ids.Append(10);
    ids.Append(20);
    iOperation = iDatabase->PreparePartialLandmarksL(ids);
    iOperation->ExecuteL();
    
    iArrayPtr = iDatabase->TakePreparedPartialLandmarksL(iOperation);
    AssertTrueSecL(iArrayPtr->Count() == ids.Count(), _L("Nr of Ids not correct"));
    delete iOperation;
    iOperation = NULL;
    
    // Check that returned landmarks are empty
    for (TInt ii=0; ii<iArrayPtr->Count()-1; ii++)
        {
        for (TInt j=0;j<KMaxTUint16;j++)
            {            
            AssertTrueSecL(!iArrayPtr->At(ii)->IsPositionFieldAvailable(j), _L("Only empty landmarks should be returned"));
            }
        }
    iArrayPtr->ResetAndDestroy();
    delete iArrayPtr; 
    iArrayPtr = NULL;

    CleanupStack::PopAndDestroy(&ids);
    
    // Test with empty list, should cause error
    partialParam = CPosLmPartialReadParameters::NewLC();
    attr = CPosLandmark::ELandmarkName;
    partialParam->SetRequestedAttributes(attr);
    iDatabase->SetPartialReadParametersL(*partialParam);
    
    CleanupClosePushL(ids);
    TRAP(err, iDatabase->PreparePartialLandmarksL(ids));
   
    CleanupStack::PopAndDestroy();
    if (err != KErrArgument) 
    {
    	iLog->Log(_L("PreparePartialLandmarksL should leave with KErrArgument"));
    	User::Leave(err);
    }
    
    CleanupStack::PopAndDestroy(partialParam);
//    CleanupStack::PopAndDestroy(&ids);

    }

// ---------------------------------------------------------
// CPosTp18::PreparePartialLandmarksAsyncL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::PreparePartialLandmarksAsyncL(TExecutionMode aExecutionMode)
    {
    iLog->Log(_L("Testing Prepare and TakePartialLandmarks asyncronously"));
   
    CPosLmPartialReadParameters* partialParam = iDatabase->PartialReadParametersLC();
    
    TUint attr = CPosLandmark::ELandmarkName;
    partialParam->SetRequestedAttributes(attr);
    iDatabase->SetPartialReadParametersL(*partialParam);
      
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    for (TInt i=0; i<KNoPreparePartial; i++)
        {
        TPosLmItemId id= i+1;
        ids.Append(id);
        }
    ids.Append(13112321);
    
    iOperation = iDatabase->PreparePartialLandmarksL(ids);
    
    TRAPD(err, iArrayPtr=iDatabase->TakePreparedPartialLandmarksL(iOperation));
    AssertTrueSecL(err == KErrNotFound, KTakePreparedPartialErr); 
    
    switch (aExecutionMode)
        {
        case EAsynchronous:
            RunAsyncOperationL(iOperation);
            break;
        case EWaitForRequest:
            RunAsyncOperationByWaitForReqL(iOperation);
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }
 
    if (iArrayPtr != NULL)
        {
        iArrayPtr->ResetAndDestroy();
        delete iArrayPtr;
        iArrayPtr=NULL;
    }
    
    iArrayPtr = iDatabase->TakePreparedPartialLandmarksL(iOperation);

    AssertTrueSecL(iArrayPtr->Count() == ids.Count(),KNoPreparedPartialErr);
    AssertTrueSecL(iArrayPtr->At(iArrayPtr->Count()-1) == NULL, KNoPreparedPartialErr2); 

    for (TInt j=0; j<iArrayPtr->Count()-1; j++)
      {
      CheckPartialLandmarkL(*iArrayPtr->At(j), CPosLandmark::ELandmarkName);
      }

    iArrayPtr->ResetAndDestroy();
    delete iArrayPtr;
    iArrayPtr = NULL;

    delete iOperation;
    iOperation = NULL;
   
    // landmarks with Name, Icon, Position MediaLink1 set 
    const TInt idsStart = 10;
    const TInt idsEnd = 35;
    
    attr = attr | CPosLandmark::EIcon | CPosLandmark::EPosition;
    partialParam->SetRequestedAttributes(attr);

    RArray<TUint> reqFields;
    CleanupClosePushL(reqFields);
    reqFields.Append(EPositionFieldMediaLinks);
    reqFields.Append(EPositionFieldMediaLinksStart);
    partialParam->SetRequestedPositionFields(reqFields);
    
    iDatabase->SetPartialReadParametersL(*partialParam);

    RArray<TBool> availFields;
    CleanupClosePushL(availFields);
    for(TInt jj=0; jj<2; jj++)
        {
        availFields.Append(ETrue);
        }

    ids.Reset();
    for (TInt k=idsStart; k<=idsEnd; k++)
        {
        ids.Append(k);
        }
    
    iOperation = iDatabase->PreparePartialLandmarksL(ids);

    switch (aExecutionMode)
        {
        case EAsynchronous:
            RunAsyncOperationL(iOperation);
            break;
        case EWaitForRequest:
            RunAsyncOperationByWaitForReqL(iOperation);
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }
 
    iArrayPtr = iDatabase->TakePreparedPartialLandmarksL(iOperation);

    AssertTrueSecL(iArrayPtr->Count() == ids.Count(),KNoPreparedPartialErr);
      
    for (TInt x=0; x<iArrayPtr->Count()-1; x++)
        {  
        CheckPartialLandmarkL(*iArrayPtr->At(x), attr);        
        CheckPartialLandmarkL(*iArrayPtr->At(x), reqFields, availFields);
        }
    
    delete iOperation;
    iOperation = NULL;
    
    CleanupStack::PopAndDestroy(4, partialParam);
    }

// ---------------------------------------------------------
// CPosTp18::PreparePartialLandmarksAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::PreparePartialLandmarksAsyncAndCancelL(const TBool& aInCallback)
    {
    iLog->Log(_L("Testing Prepare and TakePartialLandmarks asyncronously with cancel"));
   
    CPosLmPartialReadParameters* partialParam = iDatabase->PartialReadParametersLC();
    
    TUint attr = CPosLandmark::ELandmarkName;
    partialParam->SetRequestedAttributes(attr);
    iDatabase->SetPartialReadParametersL(*partialParam);
      
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    for (TInt i=0; i<KNoPreparePartial; i++)
        {
        TPosLmItemId id= i+1;
        ids.Append(id);
        }
    ids.Append(13112321);
    
    iOperation = iDatabase->PreparePartialLandmarksL(ids);
    
    TRAPD(err, iArrayPtr=iDatabase->TakePreparedPartialLandmarksL(iOperation));
    AssertTrueSecL(err == KErrNotFound, KTakePreparedPartialErr); 
    
    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iOperation);
        iOperation = NULL;
        }
    else
        {
        RunAsyncOperationAndCancelLD(iOperation);
        }
    
    // landmarks with Name, Icon, Position MediaLink1 set 
    const TInt idsStart = 10;
    const TInt idsEnd = 35;
    
    attr = attr | CPosLandmark::EIcon | CPosLandmark::EPosition;
    partialParam->SetRequestedAttributes(attr);

    RArray<TUint> reqFields;
    CleanupClosePushL(reqFields);
    reqFields.Append(EPositionFieldMediaLinks);
    reqFields.Append(EPositionFieldMediaLinksStart);
    partialParam->SetRequestedPositionFields(reqFields);
    
    iDatabase->SetPartialReadParametersL(*partialParam);

    RArray<TBool> availFields;
    CleanupClosePushL(availFields);
    for(TInt jj=0; jj<2; jj++)
        {
        availFields.Append(ETrue);
        }

    ids.Reset();
    for (TInt k=idsStart; k<=idsEnd; k++)
        {
        ids.Append(k);
        }
    
    iOperation = iDatabase->PreparePartialLandmarksL(ids);
     if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iOperation);
        iOperation = NULL;
        }
    else
        {
        RunAsyncOperationAndCancelLD(iOperation);
        }
   
    CleanupStack::PopAndDestroy(4, partialParam);
    }

// ---------------------------------------------------------
// CPosTp18::GetPartialLandmarkLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmark* CPosTp18::GetPartialLandmarkLC(const TPosLmItemId aId,
                                             const CPosLmPartialReadParameters& aPartialParam)
    {
    _LIT(KIsPartialErr, "Method Landmark->IsPartial returns incorrect value");
    _LIT(KUpdateErr, "It should not be possible to update a partial landmark");

    iDatabase->SetPartialReadParametersL(aPartialParam);
    
    CPosLandmark* lm = iDatabase->ReadPartialLandmarkLC(aId);
    AssertTrueSecL(lm->IsPartial(), KIsPartialErr);
    
    TRAPD(err, iDatabase->UpdateLandmarkL(*lm));
    AssertTrueSecL(err == KErrArgument, KUpdateErr);
    
    return lm;
    }

// ---------------------------------------------------------
// CPosTp18::AppendFields
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::AppendFields(RArray<TUint>& aArray)
    {
    // Appends fields availible to set with Landmarks db generator
    aArray.Append(EPositionFieldStreet);
    aArray.Append(EPositionFieldPostalCode);
    aArray.Append(EPositionFieldCity);
    aArray.Append(EPositionFieldLocationName);
    aArray.Append(EPositionFieldCountry);
    aArray.Append(EPositionFieldMediaLinks);
    aArray.Append(EPositionFieldMediaLinksStart);

    TUint16 tmp = (TUint16)EPositionFieldMediaLinksStart;
    aArray.Append(++tmp);
    
    }

// ---------------------------------------------------------
// CPosTp18::ChechPartialLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
                                     const RArray<TUint>& aReqFields,
                                     const RArray<TBool>& aAvailFields)
    {

    _LIT(KPosFieldAvailableErr, "Method IsPositionFieldAvailable return incorrect value, should be availabe");
    _LIT(KPosFieldAvailableErr2, "Method IsPositionFieldAvailable return incorrect value, should not be available");
    
    TInt err=0;
    for (TInt i=0; i<aReqFields.Count(); i++) 
        {
        if (aAvailFields[i])
            {
            AssertTrueSecL(aLandmark.IsPositionFieldAvailable((TUint16)aReqFields[i]), KPosFieldAvailableErr);
            TPtrC value;
            err = aLandmark.GetPositionField((TUint16)aReqFields[i], value); 
            AssertTrueSecL(err == KErrNone, KPosFieldAvailableErr, err);
            }
        else
            {
            AssertTrueSecL(!aLandmark.IsPositionFieldAvailable((TUint16)aReqFields[i]), KPosFieldAvailableErr);
            TPtrC value;
            err = aLandmark.GetPositionField((TUint16)aReqFields[i], value); 
            AssertTrueSecL(err == KErrNotFound, KPosFieldAvailableErr2, err); 
            }
        }
    
    // Check some fields not avail
    
    for (TUint16 j = EPositionFieldCompassCapabilitiesBegin; j <=EPositionFieldCompassCapabilitiesBegin+50; j++)
        {
	    AssertTrueSecL(!aLandmark.IsPositionFieldAvailable(j), KPosFieldAvailableErr2);
        TPtrC value;
        err = aLandmark.GetPositionField(j, value); 
        AssertTrueSecL(err == KErrNotFound, KPosFieldAvailableErr2, err); 
        }
    for (TUint16 k =EPositionFieldSatelliteCapabilitiesBegin; k <=EPositionFieldSatelliteGeoidalSeparation; k++)
        {
        AssertTrueSecL(!aLandmark.IsPositionFieldAvailable(k), KPosFieldAvailableErr2);
        TPtrC value;
        err = aLandmark.GetPositionField(k, value); 
        AssertTrueSecL(err == KErrNotFound, KPosFieldAvailableErr2, err); 
        }

    AssertTrueSecL(!aLandmark.IsPositionFieldAvailable(EPositionFieldIdLast), KPosFieldAvailableErr2);
    TPtrC value;
    err = aLandmark.GetPositionField(EPositionFieldIdLast, value); 
    AssertTrueSecL(err == KErrNotFound, KPosFieldAvailableErr2, err); 
    }

// ---------------------------------------------------------
// CPosTp18::ChechPartialLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::CheckPartialLandmarkL(const CPosLandmark& aLandmark, 
                                     const CPosLandmark::TAttributes& aAttr)
    {
    _LIT(KPartialIdErr, "Incorrect Id when reading from a Partial landmark");
    _LIT(KPartialCategoryInfoErr, "Incorrect Category info when reading from a Partial landmark");

    TPosLmItemId id = aLandmark.LandmarkId();
    AssertTrueSecL(id != 0, KPartialIdErr); 
    
    TInt err;
    TPtrC name;
    err = aLandmark.GetLandmarkName(name);
    CheckPartialErrorCodeL(aAttr & CPosLandmark::ELandmarkName, err); 
    
    TReal32 coverageR;     
    err = aLandmark.GetCoverageRadius(coverageR);
    CheckPartialErrorCodeL(aAttr & CPosLandmark::ECoverageRadius, err);
    
    TPtrC desc;
    err = aLandmark.GetLandmarkDescription(desc); 
    CheckPartialErrorCodeL(aAttr & CPosLandmark::EDescription, err);
    
    TPtrC iconName;
    TInt iconIndex;
    TInt iconMaskIndex;
    err = aLandmark.GetIcon(iconName, iconIndex, iconMaskIndex); 
    CheckPartialErrorCodeL(aAttr & CPosLandmark::EIcon, err);
    
    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);

    aLandmark.GetCategoriesL(categories);
    if (aAttr & CPosLandmark::ECategoryInfo)
        {
        AssertTrueSecL(categories.Count() != 0, KPartialCategoryInfoErr);
        }
    else
        {
        AssertTrueSecL(categories.Count() == 0, KPartialCategoryInfoErr);
        }
    CleanupStack::PopAndDestroy(&categories);

    TLocality loc;
    err = aLandmark.GetPosition(loc); 
    CheckPartialErrorCodeL(aAttr & CPosLandmark::EPosition, err);
   
    }

// ---------------------------------------------------------
// CPosTp18::ChechPartialErrorCodeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp18::CheckPartialErrorCodeL(const TBool aPartial, const TInt aErr)
    {
    _LIT(KPartialLmErr, "Incorrect error code returned when reading from a Partial landmark");

    if (aPartial)
        {
        AssertTrueSecL(aErr == KErrNone, KPartialLmErr); 
        }
    else
        {
        AssertTrueSecL(aErr == KErrNotFound, KPartialLmErr);
        }
    }

//  End of File
