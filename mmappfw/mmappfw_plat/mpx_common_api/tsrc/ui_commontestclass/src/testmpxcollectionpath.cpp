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
* Description:  MPXCollectionPath testing implementation (mpxcollectionpath.h)
*
*/


#include <s32mem.h>
#include <mpxcollectionpath.h>
#include "commontestclass.h"

// Begin MPXCollectionPath testing implementation mpxcollectionpath.h================================

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxItemIdConst()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxItemIdConst(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxItemIdConst testing TMPXItemId::TMPXItemId() begin")));
    iLog->Log(_L("CCommonTestClass::MpxItemIdConst testing TMPXItemId::TMPXItemId() begin"));
    TInt err=KErrNone;
    TUint id1;
    TUint id2;
    TUint numParam=0;
    TMPXItemId item;
    
    // read in parameters
    if ( aItem.GetNextInt(id1) == KErrNone )
        {
        numParam++;
        if ( aItem.GetNextInt(id2) == KErrNone )
            {
            numParam++;
            }
        }
    
    switch (numParam)
        {
        case 1:
            item.iId1 = item.iId2 = id1;
            break;
        case 2:
            item.iId1 = id1;
            item.iId2 = id2;
            break;
        }
    
    iMPXItemIdArray.Append(item);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxItemIdReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxItemIdReset(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxItemIdReset begin")));
    iLog->Log(_L("CCommonTestClass::MpxItemIdReset begin"));
    TInt err=KErrNone;

    iMPXItemIdArray.Reset();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathNewL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathNewL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathNewL testing CMPXCollectionPath::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathNewL testing CMPXCollectionPath::NewL() begin"));
    TInt err=KErrNone;

    if ( iMPXCollectionPath )
        {
        delete iMPXCollectionPath;
        iMPXCollectionPath = NULL;
        }
    iMPXCollectionPath = CMPXCollectionPath::NewL();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathNewLPathL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathNewLPathL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathNewLPathL testing CMPXCollectionPath::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathNewLPathL testing CMPXCollectionPath::NewL() begin"));
    TInt err=KErrNone;

    if ( !iMPXCollectionPath )
        {
        iLog->Log(_L("Error: MPXCollectionPath not created."));
        return KErrBadTestParameter;
        }
    CMPXCollectionPath* path = CMPXCollectionPath::NewL(*iMPXCollectionPath);
    // Verification
    if ( path->Levels() != iMPXCollectionPath->Levels() )
        {
        iLog->Log(_L("Verification Failed: new num levels=%d, old num levels=%d."), 
                path->Levels(), iMPXCollectionPath->Levels());
        err = KErrUnexpectedValue;
        }
    if ( path->Count() != iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Verification Failed: new Count=%d, old Count=%d."), 
                path->Count(), iMPXCollectionPath->Count());
        err = KErrUnexpectedValue;
        }
    
    delete path;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathNewLStreamL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathNewLStreamL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathNewLPathL testing CMPXCollectionPath::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathNewLPathL testing CMPXCollectionPath::NewL() begin"));
    TInt err=KErrNone;

    if ( !iMPXCollectionPath )
        {
        iLog->Log(_L("Error: MPXCollectionPath not created."));
        return KErrBadTestParameter;
        }

    CBufBase* buffer = CBufFlat::NewL( 200 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    iMPXCollectionPath->ExternalizeL( writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );

    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );                
    CMPXCollectionPath* path = CMPXCollectionPath::NewL(readStream);
    // Verification
    if ( path->Levels() != iMPXCollectionPath->Levels() )
        {
        iLog->Log(_L("Verification Failed: new num levels=%d, old num levels=%d."), 
                path->Levels(), iMPXCollectionPath->Levels());
        err = KErrUnexpectedValue;
        }
    if ( path->Count() != iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Verification Failed: new Count=%d, old Count=%d."), 
                path->Count(), iMPXCollectionPath->Count());
        err = KErrUnexpectedValue;
        }
    CleanupStack::PopAndDestroy( &readStream );
    CleanupStack::PopAndDestroy( buffer );
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathAppendLIdL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathAppendLIdL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathAppendLIdL testing CMPXCollectionPath::AppendL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathAppendLIdL testing CMPXCollectionPath::AppendL() begin"));
    TInt err=KErrNone;
    TUint itemId;
    
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
        {
        iLog->Log(_L("Missing Parameter: ItemId."));
        return KErrBadTestParameter;
        }

    // if first time, generate TUid and use that value to append.
    if ( iMPXCollectionPath->Levels() == 0 )
        {
        iCollectionPathUid.Uid(itemId);
        itemId = iCollectionPathUid.iUid;
        }
    TMPXItemId item(itemId);
    iMPXCollectionPath->AppendL(item);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathAppendLIdsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathAppendLIdsL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathAppendLIdsL testing CMPXCollectionPath::AppendL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathAppendLIdsL testing CMPXCollectionPath::AppendL() begin"));
    TInt err=KErrNone;

    iMPXCollectionPath->AppendL(iMPXItemIdArray.Array());
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathLevels()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathLevels(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathLevels testing CMPXCollectionPath::Levels() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathLevels testing CMPXCollectionPath::Levels() begin"));
    TInt err=KErrNone;
    TUint expectedLevels;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedLevels) )
        {
        iLog->Log(_L("Missing Parameter: Expected Level."));
        return KErrBadTestParameter;
        }

    TInt levels = iMPXCollectionPath->Levels();
    // Verification
    if ( levels != expectedLevels )
        {
        iLog->Log(_L("Verification Failed: levels=%d, expectedLevels=%d."), levels, expectedLevels);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathBack()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathBack(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathBack testing CMPXCollectionPath::Back() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathBack testing CMPXCollectionPath::Back() begin"));
    TInt err=KErrNone;

    iMPXCollectionPath->Back();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathReset(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathReset testing CMPXCollectionPath::Reset() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathReset testing CMPXCollectionPath::Reset() begin"));
    TInt err=KErrNone;

    iMPXCollectionPath->Reset();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathIndex testing CMPXCollectionPath::Index() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathIndex testing CMPXCollectionPath::Index() begin"));
    TInt err=KErrNone;
    TUint expectedIndex;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedIndex) )
        {
        iLog->Log(_L("Missing Parameter: Expected Index."));
        return KErrBadTestParameter;
        }

    TInt index = iMPXCollectionPath->Index();
    // Verification
    if ( index != expectedIndex )
        {
        iLog->Log(_L("Verification Failed: index=%d, expectedIndex=%d."), index, expectedIndex);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathOperatorPlus()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathOperatorPlus(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathOperatorPlus testing CMPXCollectionPath::Operator++() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathOperatorPlus testing CMPXCollectionPath::Operator++() begin"));
    TInt err=KErrNone;
    TUint expectedResult;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected Result."));
        return KErrBadTestParameter;
        }

    TBool result = iMPXCollectionPath->operator ++();
    // Verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathOperatorMinus()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathOperatorMinus(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathOperatorMinus testing CMPXCollectionPath::Operator--() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathOperatorMinus testing CMPXCollectionPath::Operator--() begin"));
    TInt err=KErrNone;
    TUint expectedResult;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected Result."));
        return KErrBadTestParameter;
        }

    TBool result = iMPXCollectionPath->operator --();
    // Verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSetToFirst()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSetToFirst(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSetToFirst testing CMPXCollectionPath::SetToFirst() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSetToFirst testing CMPXCollectionPath::SetToFirst() begin"));
    TInt err=KErrNone;

    iMPXCollectionPath->SetToFirst();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSetToLast()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSetToLast(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSetToLast testing CMPXCollectionPath::SetToLast() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSetToLast testing CMPXCollectionPath::SetToLast() begin"));
    TInt err=KErrNone;

    iMPXCollectionPath->SetToLast();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSetIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSetIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSetIndex testing CMPXCollectionPath::Set() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSetIndex testing CMPXCollectionPath::Set() begin"));
    TInt err=KErrNone;
    TInt index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }

    iMPXCollectionPath->Set(index);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSetId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSetId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSetId testing CMPXCollectionPath::Set() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSetId testing CMPXCollectionPath::Set() begin"));
    TInt err=KErrNone;
    TUint itemId;
    
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
        {
        iLog->Log(_L("Missing Parameter: ItemId."));
        return KErrBadTestParameter;
        }

    TMPXItemId id(itemId);
    iMPXCollectionPath->Set(id);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSetMode()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSetMode(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSetMode testing CMPXCollectionPath::Set() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSetMode testing CMPXCollectionPath::Set() begin"));
    TInt err=KErrNone;
    TUint mode;
    
    // read in parameters
    if ( aItem.GetNextInt(mode) )
        {
        iLog->Log(_L("Missing Parameter: Open mode."));
        return KErrBadTestParameter;
        }

    iMPXCollectionPath->Set(static_cast<TMPXOpenMode>(mode));
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSetLAttrsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSetLAttrsL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSetLAttrsL testing CMPXCollectionPath::SetL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSetLAttrsL testing CMPXCollectionPath::SetL() begin"));
    TInt err=KErrNone;
    
    iMPXCollectionPath->SetL(iMPXAttArray.Array());
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathOpenNextMode()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathOpenNextMode(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathOpenNextMode testing CMPXCollectionPath::OpenNextMode() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathOpenNextMode testing CMPXCollectionPath::OpenNextMode() begin"));
    TInt err=KErrNone;
    TUint expectedMode;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedMode) )
        {
        iLog->Log(_L("Missing Parameter: Expected open mode."));
        return KErrBadTestParameter;
        }

    TMPXOpenMode mode = iMPXCollectionPath->OpenNextMode();
    // Verification
    if ( mode != expectedMode )
        {
        iLog->Log(_L("Verification Failed: mode=%d, expectedMode=%d."), mode, expectedMode);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathOpenPreviousMode()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathOpenPreviousMode(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathOpenPreviousMode testing CMPXCollectionPath::OpenPreviousMode() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathOpenPreviousMode testing CMPXCollectionPath::OpenPreviousMode() begin"));
    TInt err=KErrNone;
    TUint expectedMode;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedMode) )
        {
        iLog->Log(_L("Missing Parameter: Expected open mode."));
        return KErrBadTestParameter;
        }

    TMPXOpenMode mode = iMPXCollectionPath->OpenPreviousMode();
    // Verification
    if ( mode != expectedMode )
        {
        iLog->Log(_L("Verification Failed: mode=%d, expectedMode=%d."), mode, expectedMode);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSelectLIdL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSelectLIdL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSelectLIdL testing CMPXCollectionPath::SelectL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSelectLIdL testing CMPXCollectionPath::SelectL() begin"));
    TInt err=KErrNone;
    TUint itemId;
    
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
        {
        iLog->Log(_L("Missing Parameter: ItemId."));
        return KErrBadTestParameter;
        }

    TMPXItemId id(itemId);
    iMPXCollectionPath->SelectL(id);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSelectLIndexL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSelectLIndexL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSelectLIndexL testing CMPXCollectionPath::SelectL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSelectLIndexL testing CMPXCollectionPath::SelectL() begin"));
    TInt err=KErrNone;
    TInt index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }

    iMPXCollectionPath->SelectL(index);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSelectAllL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSelectAllL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSelectAllL testing CMPXCollectionPath::SelectAllL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSelectAllL testing CMPXCollectionPath::SelectAllL() begin"));
    TInt err=KErrNone;
    
    iMPXCollectionPath->SelectAllL();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathDeselectId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathDeselectId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathDeselectId testing CMPXCollectionPath::Deselect() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathDeselectId testing CMPXCollectionPath::Deselect() begin"));
    TInt err=KErrNone;
    TUint itemId;
    
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
        {
        iLog->Log(_L("Missing Parameter: ItemId."));
        return KErrBadTestParameter;
        }

    TMPXItemId id(itemId);
    iMPXCollectionPath->Deselect(id);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathDeselectIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathDeselectIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathDeselectIndex testing CMPXCollectionPath::Deselect() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathDeselectIndex testing CMPXCollectionPath::Deselect() begin"));
    TInt err=KErrNone;
    TInt index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }

    iMPXCollectionPath->Deselect(index);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathDeselectAll()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathDeselectAll(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathDeselectAll testing CMPXCollectionPath::DeselectAll() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathDeselectAll testing CMPXCollectionPath::DeselectAll() begin"));
    TInt err=KErrNone;
    
    iMPXCollectionPath->DeselectAll();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathRemoveId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathRemoveId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathRemoveId testing CMPXCollectionPath::Remove() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathRemoveId testing CMPXCollectionPath::Remove() begin"));
    TInt err=KErrNone;
    TUint itemId;
    
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
        {
        iLog->Log(_L("Missing Parameter: ItemId."));
        return KErrBadTestParameter;
        }

    TMPXItemId id(itemId);
    iMPXCollectionPath->Remove(id);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathRemoveIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathRemoveIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathRemoveIndex testing CMPXCollectionPath::Remove() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathRemoveIndex testing CMPXCollectionPath::Remove() begin"));
    TInt err=KErrNone;
    TInt index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }

    iMPXCollectionPath->Remove(index);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathIsSelectedId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathIsSelectedId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathIsSelectedId testing CMPXCollectionPath::IsSelected() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathIsSelectedId testing CMPXCollectionPath::IsSelected() begin"));
    TInt err=KErrNone;
    TUint itemId;
    TInt expectedResult;
    
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
        {
        iLog->Log(_L("Missing Parameter: ItemId."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }

    TMPXItemId id(itemId);
    TBool result = iMPXCollectionPath->IsSelected(id);
    // Verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathIsSelectedIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathIsSelectedIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathIsSelectedIndex testing CMPXCollectionPath::IsSelected() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathIsSelectedIndex testing CMPXCollectionPath::IsSelected() begin"));
    TInt err=KErrNone;
    TInt index;
    TInt expectedResult;

    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    if ( index >= iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Bad Parameter: index out of range."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }

    TBool result = iMPXCollectionPath->IsSelected(index);
    // Verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathClearSelection()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathClearSelection(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathClearSelection testing CMPXCollectionPath::ClearSelection() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathClearSelection testing CMPXCollectionPath::ClearSelection() begin"));
    TInt err=KErrNone;
    
    iMPXCollectionPath->ClearSelection();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSelection()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSelection(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSelection testing CMPXCollectionPath::Selection() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSelection testing CMPXCollectionPath::Selection() begin"));
    TInt err=KErrNone;
    TInt data;
    RArray<TInt> expectedIndices;
    
    // read in parameters
    while ( aItem.GetNextInt(data) == KErrNone )
        {
        expectedIndices.Append(data);
        }
    
    TArray<TInt> indices = iMPXCollectionPath->Selection();
    // Verification
    if ( indices.Count() == expectedIndices.Count() )
        {
        for (TInt i=0; i<indices.Count(); i++)
            {
            if ( expectedIndices.Find(indices[i]) == KErrNotFound )
                {
                iLog->Log(_L("Verification Failed: Index not Found = %d."), indices[i]);
                err = KErrUnexpectedValue;
                }
            }
        }
    else
        {
        iLog->Log(_L("Verification Failed: count=%d, expected count=%d."), 
                indices.Count(), expectedIndices.Count());
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSelectionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathSelectionL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSelectionL testing CMPXCollectionPath::SelectionL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSelectionL testing CMPXCollectionPath::SelectionL() begin"));
    TInt err=KErrNone;
    TUint data;
    RArray<TMPXItemId> expectedIds;
    RArray<TMPXItemId> ids;
    
    // read in parameters
    while ( aItem.GetNextInt(data) == KErrNone )
        {
        TMPXItemId id(data);
        expectedIds.Append(id);
        }
    
    iMPXCollectionPath->SelectionL(ids);
    // Verification
    if ( ids.Count() == expectedIds.Count() )
        {
        for (TInt i=0; i<ids.Count(); i++)
            {
            if ( expectedIds.Find(ids[i]) == KErrNotFound )
                {
                iLog->Log(_L("Verification Failed: Index not Found = %d."), ids[i].iId1);
                err = KErrUnexpectedValue;
                }
            }
        }
    else
        {
        iLog->Log(_L("Verification Failed: count=%d, expected count=%d."), 
                ids.Count(), expectedIds.Count());
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathId testing CMPXCollectionPath::Id() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathId testing CMPXCollectionPath::Id() begin"));
    TInt err=KErrNone;
    TUint data;
    
    // read in parameters
    if ( aItem.GetNextInt(data) )
        {
        iLog->Log(_L("Missing Parameter: Expected ItemId."));
        return KErrBadTestParameter;
        }

    TMPXItemId id = iMPXCollectionPath->Id();
    // Verification
    TMPXItemId expectedId(data);
    if ( id != expectedId )
        {
        iLog->Log(_L("Verification Failed: id=%d, expectedId=%d."), TUint(id), TUint(expectedId));
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathCount()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathCount(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathCount testing CMPXCollectionPath::Count() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathCount testing CMPXCollectionPath::Count() begin"));
    TInt err=KErrNone;
    TUint expectedCount;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedCount) )
        {
        iLog->Log(_L("Missing Parameter: Expected Count."));
        return KErrBadTestParameter;
        }

    TInt count = iMPXCollectionPath->Count();
    // Verification
    if ( count != expectedCount )
        {
        iLog->Log(_L("Verification Failed: count=%d, expectedCount=%d."), count, expectedCount);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathIndexOfId()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathIndexOfId(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathIndexOfId testing CMPXCollectionPath::IndexOfId() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathIndexOfId testing CMPXCollectionPath::IndexOfId() begin"));
    TInt err=KErrNone;
    TUint id;
    TInt expectedIndex;
    
    // read in parameters
    if ( aItem.GetNextInt(id) )
        {
        iLog->Log(_L("Missing Parameter: Item Id."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedIndex) )
        {
        iLog->Log(_L("Missing Parameter: Expected index."));
        return KErrBadTestParameter;
        }

    TMPXItemId itemId(id);
    TInt index = iMPXCollectionPath->IndexOfId(itemId);
    // Verification
    if ( index != expectedIndex )
        {
        iLog->Log(_L("Verification Failed: index=%d, expectedIndex=%d."), index, expectedIndex);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathIdOfIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathIdOfIndex(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathIdOfIndex testing CMPXCollectionPath::IdOfIndex() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathIdOfIndex testing CMPXCollectionPath::IdOfIndex() begin"));
    TInt err=KErrNone;
    TUint expectedId;
    TInt index;
    
    // read in parameters
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedId) )
        {
        iLog->Log(_L("Missing Parameter: Expected itemId."));
        return KErrBadTestParameter;
        }

    TMPXItemId id = iMPXCollectionPath->IdOfIndex(index);
    // Verification
    if ( TUint(id) != expectedId )
        {
        iLog->Log(_L("Verification Failed: id=%d, expectedId=%d."), TUint(id), expectedId);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathOpenAttributes()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathOpenAttributes(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathOpenAttributes testing CMPXCollectionPath::OpenAttributes() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathOpenAttributes testing CMPXCollectionPath::OpenAttributes() begin"));
    TInt err=KErrNone;
    
    TArray<TMPXAttribute> atts = iMPXCollectionPath->OpenAttributes();
    // Verification
    if ( atts.Count() == iMPXAttArray.Count() )
        {
        for (TInt i=0; i<atts.Count(); i++)
            {
            if ( iMPXAttArray.Find(atts[i]) == KErrNotFound )
                {
                iLog->Log(_L("Verification Failed: Attribute not Found, contentId=%d, attributeId=%d."), 
                        atts[i].ContentId(), atts[i].AttributeId());
                err = KErrUnexpectedValue;
                }
            }
        }
    else
        {
        iLog->Log(_L("Verification Failed: count=%d, expected count=%d."), 
                atts.Count(), iMPXAttArray.Count());
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathIndexLevel()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathIndexLevel(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathIndexLevel testing CMPXCollectionPath::Index() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathIndexLevel testing CMPXCollectionPath::Index() begin"));
    TInt err=KErrNone;
    TInt level;
    TInt expectedIndex;
    
    // read in parameters
    if ( aItem.GetNextInt(level) )
        {
        iLog->Log(_L("Missing Parameter: level."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedIndex) )
        {
        iLog->Log(_L("Missing Parameter: Expected index."));
        return KErrBadTestParameter;
        }

    TInt index = iMPXCollectionPath->Index(level);
    // Verification
    if ( index != expectedIndex )
        {
        iLog->Log(_L("Verification Failed: index=%d, expectedIndex=%d."), index, expectedIndex);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathIdLevel()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathIdLevel(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathIdLevel testing CMPXCollectionPath::Id() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathIdLevel testing CMPXCollectionPath::Id() begin"));
    TInt err=KErrNone;
    TInt level;
    TUint expectedId;
    
    // read in parameters
    if ( aItem.GetNextInt(level) )
        {
        iLog->Log(_L("Missing Parameter: level."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedId) )
        {
        iLog->Log(_L("Missing Parameter: Expected itemId."));
        return KErrBadTestParameter;
        }

    TMPXItemId id = iMPXCollectionPath->Id(level);
    // Verification
    if ( TUint(id) != expectedId )
        {
        iLog->Log(_L("Verification Failed: id=%d, expectedId=%d."), TUint(id), expectedId);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathStreamingL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathStreamingL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathStreamingL testing CMPXCollectionPath ExternalizeL and InternalizeL begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathStreamingL testing CMPXCollectionPath ExternalizeL and InternalizeL begin"));
    TInt err=KErrNone;
    
    iLog->Log(_L("Start ExternalizeL"));
    CBufBase* buffer = CBufFlat::NewL( 200 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    iMPXCollectionPath->ExternalizeL( writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );

    iLog->Log(_L("Start InternalizeL"));
    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );                
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->InternalizeL( readStream );
    // verification
    if ( path->Count() != iMPXCollectionPath->Count() )
        {
        iLog->Log(_L("Verification Failed: New Array Count=%d, Old Array Count=%d."), 
                path->Count(), iMPXCollectionPath->Count());
        err = KErrUnexpectedValue;
        }
    CleanupStack::PopAndDestroy( path );
    CleanupStack::PopAndDestroy( &readStream );
    CleanupStack::PopAndDestroy( buffer );
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathHandleChange()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollPathHandleChange(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathHandleChange testing CMPXCollectionPath::HandleChange() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathHandleChange testing CMPXCollectionPath::IdHandleChange() begin"));
    TInt err=KErrNone;
    TUint id;
    TUint deprecatedId;
    TInt change;
    TInt selection;
    TInt expectedSelection;
    TInt expectedResult;
    
    // read in parameters
    if ( aItem.GetNextInt(id) )
        {
        iLog->Log(_L("Missing Parameter: itemId."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(deprecatedId) )
        {
        iLog->Log(_L("Missing Parameter: Deprecated itemId."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(change) )
        {
        iLog->Log(_L("Missing Parameter: Change type."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedSelection) )
        {
        iLog->Log(_L("Missing Parameter: Expected selection."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }

    TMPXItemId itemId(id);
    TMPXItemId deprecatedItemId(deprecatedId);
    TInt result = iMPXCollectionPath->HandleChange(iCollectionPathUid, itemId, deprecatedItemId, 
            (CMPXCollectionPath::TMPXCollectionPathChange)change, selection);
    // Verification
    if ( selection != expectedSelection )
        {
        iLog->Log(_L("Verification Failed: selection=%d, expectedSelection=%d."), selection, expectedSelection);
        err = KErrUnexpectedValue;
        }
    else if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollectionpathContainerPathL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollectionpathContainerPathL testing CMPXCollectionPath::ContainerPathL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollectionpathContainerPathL testing CMPXCollectionPath::ContainerPathL() begin"));
    
    TInt err=KErrNone;
    iMPXCollectionPath->ContainerPathL();
    
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollectionpathContainerPathL testing CMPXCollectionPath::ContainerPathL() end")));
    iLog->Log(_L("CCommonTestClass::MpxCollectionpathContainerPathL testing CMPXCollectionPath::ContainerPathL() end"));
        
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollectionpathInsertL(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollectionpathInsertL testing CMPXCollectionPath::InsertL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollectionpathInsertL testing CMPXCollectionPath::InsertL() begin"));
    TInt err=KErrNone;
    TUint itemId;
    TInt apos = 1;
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
        {
        iLog->Log(_L("Missing Parameter: ItemId."));
        return KErrBadTestParameter;
        }
    // if first time, generate TUid and use that value to append.
    if ( iMPXCollectionPath->Levels() == 0 )
        {
        iCollectionPathUid.Uid(itemId);
        itemId = iCollectionPathUid.iUid;
        }
    TMPXItemId item(itemId);
    iMPXCollectionPath->InsertL(item,apos);
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollectionpathItems(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollectionpathItems testing CMPXCollectionPath::Items() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollectionpathItems testing CMPXCollectionPath::Items() begin"));
    
    TInt err=KErrNone;
    iMPXCollectionPath->Items();
    
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollectionpathItems testing CMPXCollectionPath::Items() end")));
    iLog->Log(_L("CCommonTestClass::MpxCollectionpathItems testing CMPXCollectionPath::Items() end"));

    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathReset()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollectionpathUpdate(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollectionpathUpdate testing CMPXCollectionPath::Update() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollectionpathUpdate testing CMPXCollectionPath::Update() begin"));
    TInt err=KErrNone;
    TInt index;
    TInt newId;
    if ( aItem.GetNextInt(index) )
        {
        iLog->Log(_L("Missing Parameter: Index."));
        return KErrBadTestParameter;
        }
    if ( aItem.GetNextInt(newId) )
        {
        iLog->Log(_L("Missing Parameter: newId."));
        return KErrBadTestParameter;
        }
    TMPXItemId newId1(newId);
    iMPXCollectionPath->Update(index,newId1);
    
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxDelete()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollectionpathDelete(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollectionpathDelete testing CMPXCollectionPath::~ begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollectionpathDelete testing CMPXCollectionPath::~ begin"));
    
    TInt err = KErrNone;
    delete iMPXCollectionPath;
    iMPXCollectionPath = NULL;
    
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxCollPathSetAttrsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxCollectionpathCollPathSetAttrsL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxCollPathSetAttrsL testing CMPXCollectionPath::Set() begin")));
    iLog->Log(_L("CCommonTestClass::MpxCollPathSetAttrsL testing CMPXCollectionPath::Set() begin"));
   
    TInt err=KErrNone;
    iMPXCollectionPath->Set(iMPXAttArray.Array());
    
    return err;
    }
// end of MPXCollectionPath mpxcollectionpath.h =====================================================
