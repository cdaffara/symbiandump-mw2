/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implement the operation: SetObjectPropList
*
*/


#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>

#include "csetobjectproplist.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdputility.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"
#include "cpropertysettingutility.h"

// -----------------------------------------------------------------------------
// CSendObject::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CSetObjectPropList::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CSetObjectPropList* self = new ( ELeave ) CSetObjectPropList( aFramework, aConnection, aDpConfig );

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::CSetObjectPropList
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CSetObjectPropList::CSetObjectPropList( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework, aConnection, 0, NULL),
        iObjectMgr( aFramework.ObjectMgr() ),
        iFs( aFramework.Fs() ),
        iDpConfig( aDpConfig ),
        iUnprocessedIndex ( 0 )
    {
    PRINT( _L( "Operation: SetObjectPropList(0x9806)" ) );
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::ConstructL
// 2nd Phase Constructor
// -----------------------------------------------------------------------------
//
void CSetObjectPropList::ConstructL()
    {
    CActiveScheduler::Add( this );

    iPropertyList = CMTPTypeObjectPropList::NewL();
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::~CSetObjectPropList
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSetObjectPropList::~CSetObjectPropList()
    {
    Cancel();
    delete iPropertyList;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::CheckRequestL
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CSetObjectPropList::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CSetObjectPropList::CheckRequestL" ) );

    TMTPResponseCode result = CRequestProcessor::CheckRequestL();
    if ( result == EMTPRespCodeObjectWriteProtected )
        {
        // Return AccessDenied for P4S pass rate, instead of EMTPRespCodeObjectWriteProtected
        result = EMTPRespCodeAccessDenied;
        }

    PRINT( _L( "MM MTP <= CSetObjectPropList::CheckRequestL" ) );
    return result;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::ServiceL
// SetObjectPropList request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CSetObjectPropList::ServiceL()
    {
    PRINT( _L( "MM MTP => CSetObjectPropList::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);
    
    // Recieve the data from the property list
    ReceiveDataL( *iPropertyList );
    PRINT( _L( "MM MTP <= CSetObjectPropList::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::DoHandleResponsePhaseL
// Completing phase for the request handler
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSetObjectPropList::DoHandleResponsePhaseL()
    {
    PRINT( _L( "MM MTP => CSetObjectPropList::DoHandleResponsePhaseL" ) );

    iElementCount = iPropertyList->NumberOfElements();
    iPropertyList->ResetCursor();
    if( iElementCount > 0 )
        {
        iUnprocessedIndex = 0;
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, iStatus.Int() );
        SetActive();
        }
    else
        {
        SendResponseL( EMTPRespCodeOK );
        }

    PRINT( _L( "MM MTP <= CSetObjectPropList::DoHandleResponsePhaseL" ) );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::SetObjectPropListL
// Set object proplist
// -----------------------------------------------------------------------------
//
TMTPResponseCode CSetObjectPropList::SetObjectPropListL( const CMTPTypeObjectPropListElement& aPropListElement )
    {
    PRINT( _L( "MM MTP => CSetObjectPropList::SetObjectPropListL" ) );

    TMTPResponseCode responseCode( EMTPRespCodeOK );

    TUint32 handle = aPropListElement.Uint32L( CMTPTypeObjectPropListElement::EObjectHandle );
    TUint16 propertyCode = aPropListElement.Uint16L( CMTPTypeObjectPropListElement::EPropertyCode );
    TUint16 dataType = aPropListElement.Uint16L( CMTPTypeObjectPropListElement::EDatatype );
    PRINT3( _L( "MM MTP <> handle = 0x%x, propertyCode = 0x%x, dataType = 0x%x" ),
        handle,
        propertyCode,
        dataType );

    responseCode = MmMtpDpUtility::CheckPropType( propertyCode, dataType );
    PRINT1( _L( "MM MTP <> CheckPropType response code is 0x%x" ), responseCode );

    if( responseCode != EMTPRespCodeOK )
        return responseCode;

    if ( iFramework.ObjectMgr().ObjectOwnerId( handle )
        == iFramework.DataProviderId() )
        {
        PRINT( _L( "MM MTP => CSetObjectPropList::SetObjectPropListL enter" ) );

        CMTPObjectMetaData* object = CMTPObjectMetaData::NewLC(); // + object
        iFramework.ObjectMgr().ObjectL( handle, *object );

        // Check the file attribution first. If it is Read-Only, nothing should be set into db which is inlined with P4S cases.
        TUint16 protectionStatus = EMTPProtectionNoProtection;
        protectionStatus = MmMtpDpUtility::GetProtectionStatusL( iFs, object->DesC( CMTPObjectMetaData::ESuid ) );
        if ( protectionStatus != EMTPProtectionNoProtection )
            {
            // NOTE: P4S expects AccessDenied response instead of ObjectWriteProtected
            responseCode = EMTPRespCodeAccessDenied; // EMTPRespCodeObjectWriteProtected;
            }
        else
            {
            switch ( propertyCode )
                {
                case EMTPObjectPropCodeStorageID:
                case EMTPObjectPropCodeObjectFormat:
                case EMTPObjectPropCodeProtectionStatus:
                case EMTPObjectPropCodeObjectSize:
                case EMTPObjectPropCodeParentObject:
                case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
                case EMTPObjectPropCodeDateAdded:
                case EMTPObjectPropCodeDateCreated:
                case EMTPObjectPropCodeDateModified:
                    {
                    responseCode = EMTPRespCodeAccessDenied;
                    }
                    break;

                case EMTPObjectPropCodeNonConsumable:
                    object->SetUint( CMTPObjectMetaData::ENonConsumable,
                        aPropListElement.Uint8L( CMTPTypeObjectPropListElement::EValue ) );
                    // TODO: need to reconsider,
                    // if propList comprise both non-consumable and objectFileName,
                    // ModifyObjectL would be called twice, need to investigate if it won't affect
                    // performance
                    iFramework.ObjectMgr().ModifyObjectL( *object );
                    break;

                case EMTPObjectPropCodeObjectFileName:
                    {
                    TPtrC suid( object->DesC( CMTPObjectMetaData::ESuid ) );
                    TPtrC ptr( aPropListElement.StringL( CMTPTypeObjectPropListElement::EValue ) );
                    if ( KMaxFileName < ptr.Length() )
                        responseCode = EMTPRespCodeInvalidDataset;
                    else
                        {
                        TFileName newSuid( ptr );
                        TInt err = MmMtpDpUtility::UpdateObjectFileName( iFramework.Fs(), suid, newSuid );
                        PRINT1( _L( "MM MTP <> Update object file name err = %d" ), err );
                        if ( KErrOverflow == err ) // full path name is too long
                            {
                            responseCode = EMTPRespCodeInvalidDataset;
                            }
                        else if ( KErrNone == err )    // TODO: ( KErrAlreadyExists == err )
                            {
                            TRAP( err, iDpConfig.GetWrapperL().RenameObjectL( *object, newSuid ) ); //Update MPX DB
    
                            PRINT1( _L( "MM MTP <> Rename MPX object file name err = %d" ), err );
                            // it is ok if file is not found in DB, following S60 solution
                            if ( KErrNotFound == err )
                                {
                                TRAP( err, iDpConfig.GetWrapperL().AddObjectL( *object ) );
                                PRINT1( _L( "MM MTP <> Add MPX object file name err = %d" ), err );
                                }

                            object->SetDesCL( CMTPObjectMetaData::ESuid, newSuid );
                            iFramework.ObjectMgr().ModifyObjectL( *object );
                            }
                        else if ( KErrInUse == err ) // object file is being used by other program
                            {
                            responseCode = EMTPRespCodeDeviceBusy;
                            }
                        else
                            {
                            responseCode = EMTPRespCodeGeneralError;
                            }
                        }
                    }
                    break;

                case EMTPObjectPropCodeName:
                case EMTPObjectPropCodeAlbumArtist:
                    {
                    CMTPTypeString* stringData = CMTPTypeString::NewLC(
                        aPropListElement.StringL( CMTPTypeObjectPropListElement::EValue ) );// + stringData

                    responseCode = iDpConfig.PropSettingUtility()->SetMetaDataToWrapper( iDpConfig,
                        propertyCode,
                        *stringData,
                        *object );

                    CleanupStack::PopAndDestroy( stringData );// - stringData
                    }
                    break;

                default:
                    {
                    responseCode = iDpConfig.PropSettingUtility()->SetSpecificObjectPropertyL( iDpConfig,
                        propertyCode,
                        *object,
                        aPropListElement );
                    }
                    break;
                }

            CleanupStack::PopAndDestroy( object ); // - object
            }
        }

    PRINT1( _L( "MM MTP <= CSetObjectPropList::SetObjectPropListL responseCode = 0x%x" ), responseCode );
    return responseCode;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::HasDataphase
// Decide if has data phase
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSetObjectPropList::HasDataphase() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::RunL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSetObjectPropList::RunL()
    {
    if ( iUnprocessedIndex <  iElementCount )
        {
        TMTPResponseCode responseCode = SetObjectPropListL( iPropertyList->GetNextElementL());

        if ( responseCode != EMTPRespCodeOK )
            {
            SendResponseL( responseCode, 1, &iUnprocessedIndex );
            }
        else
            {
            // Complete ourselves with current TRequestStatus
            // Increase index to process next handle on next round
            iUnprocessedIndex++;
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, iStatus.Int() );
            SetActive();
            }
        }
    else // all handles processed, can send data
        {
        SendResponseL( EMTPRespCodeOK );
        }
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::RunError
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSetObjectPropList::RunError( TInt aError )
    {
    if ( aError != KErrNone )
        PRINT1( _L( "MM MTP <> CGetObjectPropList::RunError aError = %d" ), aError );

    TRAP_IGNORE( SendResponseL( EMTPRespCodeGeneralError ) );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropList::DoCancel()
// Cancel the process
// -----------------------------------------------------------------------------
//
EXPORT_C void CSetObjectPropList::DoCancel()
    {
    }

// end of file
