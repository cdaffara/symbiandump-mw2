/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/


// INCLUDE FILES
#include    "CCLFItemImpl.h"
#include    "CCLFDbItemContainer.h"
#include    "CLFConsts.h"
#include    "CLF2MdEConsts.h"
#include    <f32file.h>
#include    "MGDebugPrint.h"
#include    <mdesession.h>
#include    <mdetextproperty.h>
#include    <mdenumberproperty.h>
#include    <mdeobjectdef.h>
#include    <mdeconstants.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFItemImpl::CCLFItemImpl
// -----------------------------------------------------------------------------
//
CCLFItemImpl::CCLFItemImpl( CCLFContainerItem& aContainerItem,
                                   CCLFDbItemContainer& aDbItemContainer )
    : iItem( aContainerItem ),
      iMdEObject( aContainerItem.DbItem() ),
      iDbItemContainer( aDbItemContainer )
    {
    iItem.IncreaseReferenceCount();
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::NewLC
// -----------------------------------------------------------------------------
//
CCLFItemImpl* CCLFItemImpl::NewLC( CCLFContainerItem& aContainerItem,
                                   CCLFDbItemContainer& aDbItemContainer )
    {
    CCLFItemImpl* self =
            new ( ELeave ) CCLFItemImpl( aContainerItem, aDbItemContainer );

    CleanupStack::PushL( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::NewL
// -----------------------------------------------------------------------------
//
CCLFItemImpl* CCLFItemImpl::NewL( CCLFContainerItem& aContainerItem,
                                   CCLFDbItemContainer& aDbItemContainer )
    {   
    return new( ELeave ) CCLFItemImpl( aContainerItem, aDbItemContainer );
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::~CCLFItemImpl
// Destructor
// -----------------------------------------------------------------------------
//
CCLFItemImpl::~CCLFItemImpl()
    {
    iDbItemContainer.ReleaseItem( &iItem );
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::ItemId
// -----------------------------------------------------------------------------
//
TCLFItemId CCLFItemImpl::ItemId() const
    {
    TCLFItemId id = (TCLFItemId) iMdEObject.Id();
    return id;
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::DataType
// -----------------------------------------------------------------------------
//
TCLFItemDataType CCLFItemImpl::DataType( const TCLFFieldId aFieldId ) const
    {
    MG_DEBUG2( DT, "[CLF]\t CCLFItemImpl::DataType aFieldId: %d", aFieldId );

    TCLFItemDataType ret = ECLFItemDataTypeNull;
    const TDesC& property = MapFieldToMdEProperty( aFieldId );

    if ( property != KNullDesC )
        {
        const CMdEPropertyDef* propertyDef = NULL;
        TRAP_IGNORE( propertyDef = &iMdEObject.Def().GetPropertyDefL( property ));

        switch ( propertyDef->PropertyType())
            {
            case EPropertyBool:
            case EPropertyInt8:
            case EPropertyUint8:
            case EPropertyInt16:
            case EPropertyUint16:
            case EPropertyInt32:
            case EPropertyUint32:
            case EPropertyInt64:
            case EPropertyReal32:
            case EPropertyReal64:
                {
                ret = ECLFItemDataTypeTInt32;
                break;
                }
            case EPropertyTime:
                {
                ret = ECLFItemDataTypeTTime;
                break;
                }
            case EPropertyText:
                {
                ret = ECLFItemDataTypeDesC;
                break;
                }
            default:
                {
                /* Fall through */
                break;
                }
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::DeduceFieldFromURI
// -----------------------------------------------------------------------------
//
TBool CCLFItemImpl::DeduceFieldFromURI( const TCLFFieldId aFieldId, TPtrC& aValue ) const
    {
    TBool done = ETrue;
    TPtrC path = iMdEObject.Uri();
    TParsePtrC nameParse( path );

    switch ( aFieldId )
        {
        case ECLFFieldIdFileName:
            {
            aValue.Set( nameParse.Name() );
            break;
            }
        case ECLFFieldIdFileExtension:
            {
            aValue.Set( nameParse.Ext() );
            break;
            }
        case ECLFFieldIdPath:
            {
            aValue.Set( nameParse.Path() );
            break;
            }
        case ECLFFieldIdDrive:
            {
            aValue.Set( nameParse.Drive() );
            break;
            }
        case ECLFFieldIdFileNameAndPath:
            {
            aValue.Set( path );
            break;
            }
        default:
            {
            done = EFalse; // Field deduction from URI did not work.
            break;
            }
        }
    return done;
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::GetMediaTypeL
// -----------------------------------------------------------------------------
//
TInt CCLFItemImpl::GetMediaTypeL( CMdEObjectDef& aObjectDef, TInt32& aValue ) const
    {
    const TDesC* mdemediatypes[] =
        {
        &MdeConstants::Audio::KAudioObject,
        &MdeConstants::Video::KVideoObject,
        &MdeConstants::Image::KImageObject,
        };
    const TInt clfmediatypes[] =
        {
        ECLFMediaTypeSound,
        ECLFMediaTypeVideo,
        ECLFMediaTypeImage,
        };
    const TPtrC name = aObjectDef.Name();
    TInt32 error = KErrNotFound;

    aValue = ECLFMediaTypeUnknown; // Default to this.
    for ( TUint i = 0; i < sizeof( clfmediatypes ) / sizeof( TInt ); ++i )
        {
        if ( name.Compare( *mdemediatypes[ i ] ) == 0 )
            {
            aValue = clfmediatypes[ i ];
            error = KErrNone;
            break;
            }
        }

    if ( aValue == ECLFMediaTypeSound )
        {
        DetermineIfMusicL( aObjectDef, aValue );
        }
    return error;
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::DetermineIfMusicL
// -----------------------------------------------------------------------------
//
void CCLFItemImpl::DetermineIfMusicL( CMdEObjectDef& aObjectDef, TInt32& aValue ) const
    {
    // This is a hard-coded list used to determine if the media type is music. CLF uses a
    // separate MIME type manager to map MIME types to CLF media types, but this is located on
    // the CLF server side, which this wrapper removes/obsoletes. The MIME type manager uses a
    // resource file for mapping MIME types.
    const TDesC* clfwrappermusictypes[] =
        {
        &KCLFWrapperMusicTypeMpeg,
        &KCLFWrapperMusicTypeAac,
        &KCLFWrapperMusicTypeMp3,
        &KCLFWrapperMusicTypeX_Mp3,
        &KCLFWrapperMusicTypeMp4,
        &KCLFWrapperMusicType3gpp,
        &KCLFWrapperMusicTypeM4a,
        &KCLFWrapperMusicType3gpp2,
        &KCLFWrapperMusicTypeMpeg4,
        &KCLFWrapperMusicTypeX_Pn_RealAudio,
        &KCLFWrapperMusicTypeX_Ms_Wma,
        };

    const CMdEPropertyDef* propertyDef = NULL;
    propertyDef = &aObjectDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );

    if ( propertyDef )
        {
        // Get the actual data
        CMdEProperty* prop = NULL;
        iMdEObject.Property( *propertyDef, prop );
        if ( prop )
            {
            const TInt num = sizeof ( clfwrappermusictypes ) / sizeof( TDesC* );

            for ( TInt i = 0; i < num; ++i )
                {
                if ( *clfwrappermusictypes[ i ] == prop->TextValueL() )
                    {
                    aValue = ECLFMediaTypeMusic;
                    break;
                    }
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::GetField
// -----------------------------------------------------------------------------
//
TInt CCLFItemImpl::GetField( const TCLFFieldId aFieldId, TPtrC& aValue ) const
    {
    MG_DEBUG2( GF, "[CLF]\t CCLFItemImpl::GetField (TPtrC) aFieldId: %d", aFieldId );
    TInt error = KErrNotFound;

    if ( DeduceFieldFromURI( aFieldId, aValue ))
        {
        error = KErrNone;
        }
    else
        {
        // Get property
        const TDesC& property = MapFieldToMdEProperty( aFieldId );
        if ( property != KNullDesC )
            {
            CMdEPropertyDef* propertyDef = NULL;
            TRAP_IGNORE( propertyDef = &iMdEObject.Def().GetPropertyDefL( property ));

            if ( propertyDef )
                {
                // Get the actual data
                CMdEProperty* prop = NULL;
                const TInt index = iMdEObject.Property( *propertyDef, prop );
                if ( prop )
                    {
                    if ( index >= 0 )
                        {
                        CMdETextProperty* textprop = ( CMdETextProperty* ) prop;
                        aValue.Set( textprop->Value() );
                        MG_DEBUG2( GF2, "[CLF]\t CCLFItemImpl::GetField value %S", &aValue );
                        error = KErrNone;          
                        }
                    }
                }
            }
        }

    MG_DEBUG2( GF3, "[CLF]\t CCLFItemImpl::GetField (TPtrC) end %d", error );
    return error;   
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::GetField
// -----------------------------------------------------------------------------
//
TInt CCLFItemImpl::GetField( const TCLFFieldId aFieldId, TInt32& aValue ) const
    {
    MG_DEBUG2( GF4, "[CLF]\t CCLFItemImpl::GetField (TInt) aFieldId: %d", aFieldId );
    TInt error = KErrNotFound;

    // Get property
    const TDesC& property = MapFieldToMdEProperty( aFieldId );
    if ( property != KNullDesC )
        {
        CMdEPropertyDef* propertyDef = NULL;
        TRAP_IGNORE( propertyDef = &iMdEObject.Def().GetPropertyDefL( property ));

        if ( propertyDef )
            {
            // Get the actual data
            CMdEProperty* prop = NULL;
            const TInt index = iMdEObject.Property( *propertyDef, prop );
            if ( prop )
                {
                if ( index >= 0 )
                    {
                    // The following code is a Gallery behavior fix.
                    if ( &property == &MdeConstants::MediaObject::KTrackProperty )
                        {
                        error = KErrNone;
                        TRAP( error, aValue = prop->Uint16ValueL());
                        }
                    else
                        {
                        error = KErrNone;
                        TRAP( error, aValue = prop->Uint32ValueL());
                        }
                    }
                }
            }
        }
    else
        {
        // Media type is checked here.
        if ( aFieldId == ECLFFieldIdMediaType )
            {
            CMdEObjectDef& objectDef = iMdEObject.Def();
            TRAP( error, error = GetMediaTypeL( objectDef, aValue ));
            }
        }
 
    return error;   
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::GetField
// -----------------------------------------------------------------------------
//
TInt CCLFItemImpl::GetField( const TCLFFieldId aFieldId, TTime& aValue ) const
    {
    MG_DEBUG2( GF6, "[CLF]\t CCLFItemImpl::GetField (TTime) aFieldId: %d", aFieldId );
    TInt error = KErrNotFound;

    // Get property
    const TDesC& property = MapFieldToMdEProperty( aFieldId );
    if ( property != KNullDesC )
        {
        CMdEPropertyDef* propertyDef = NULL;
        TRAP_IGNORE( propertyDef = &iMdEObject.Def().GetPropertyDefL( property ));

        if ( propertyDef )
            {
            // Get the actual data
            CMdEProperty* prop = NULL;
            const TInt index = iMdEObject.Property( *propertyDef, prop );
            if ( prop )
                {
                if ( index >= 0 )
                    {
                    CMdETimeProperty* timeprop = ( CMdETimeProperty* ) prop;
                    TRAP_IGNORE( aValue = timeprop->Value() );  // Value() is leaving function
                    error = KErrNone;
                    }
                }
            }
        }

    return error;     
    }

// -----------------------------------------------------------------------------
// CCLFItemImpl::MapFieldToMdEProperty
// -----------------------------------------------------------------------------
//
const TDesC& CCLFItemImpl::MapFieldToMdEProperty( const TCLFFieldId aField )
    {
    MG_DEBUG2( MFP, "[CLF]\t CCLFItemImpl::MapFieldToMdEProperty aField: %d", aField );
    const TDesC* ret;

    switch ( aField )
        {
        /// integer
        case ECLFFieldIdFileSize:
            {
            ret = &MdeConstants::Object::KSizeProperty;
            break;
            }
        /// File date field:
        /// date/time
        case ECLFFieldIdFileDate: 
            {
            ret = &MdeConstants::Object::KLastModifiedDateProperty;
            break;
            }
        /// Mime type field:
        /// descriptor
        case ECLFFieldIdMimeType:
            {
            ret = &MdeConstants::Object::KItemTypeProperty;
            break;
            }
        /// Music file song name tag:
        /// descriptor
        case ECLFFieldIdSongName:
            {
            ret = &MdeConstants::Object::KTitleProperty;
            break;
            }
        /// Music file artist tag:
        /// descriptor
        case ECLFFieldIdArtist:
            {
            ret = &MdeConstants::MediaObject::KArtistProperty;
            break;
            }
        /// Music file album tag:
        /// descriptor
        case ECLFFieldIdAlbum:
            {
            ret = &MdeConstants::Audio::KAlbumProperty;
            break;
            }
        /// Music file genre tag:
        /// descriptor
        case ECLFFieldIdGenre:
            {
            ret = &MdeConstants::MediaObject::KGenreProperty;
            break;
            }
        /// Music file track number tag:
        /// integer
        case ECLFFieldIdTrackNumber:
            {
            ret = &MdeConstants::MediaObject::KTrackProperty;
            break;
            }
        /// Music file composer tag:
        /// descriptor
        case ECLFFieldIdComposer:
            {
            ret = &MdeConstants::Audio::KComposerProperty;
            break;
            }
        default:
            {
            ret = &KNullDesC;
            break;
            }
        }
    return *ret;
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::CCLFModifiableItemImpl
// -----------------------------------------------------------------------------
//
CCLFModifiableItemImpl::CCLFModifiableItemImpl()
    {
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::NewL
// -----------------------------------------------------------------------------
//
CCLFModifiableItemImpl* CCLFModifiableItemImpl::NewL()
    {
    return new ( ELeave ) CCLFModifiableItemImpl();
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::~CCLFModifiableItemImpl
// -----------------------------------------------------------------------------
//
CCLFModifiableItemImpl::~CCLFModifiableItemImpl()
    {
    iIntMap.Close();
    iTimeMap.Close();
    
    THashMapIter<TCLFFieldId, HBufC*> hashIter(iTextMap);
    for ( HBufC* const* p = hashIter.NextValue(); p; p = hashIter.NextValue() )
        {
        delete *p;
        p = NULL;
        }
    iTextMap.Close();
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::ItemId
// -----------------------------------------------------------------------------
//
TCLFItemId CCLFModifiableItemImpl::ItemId() const
    {
    const TInt id = 1;
    return id;
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::DataType
// -----------------------------------------------------------------------------
//
TCLFItemDataType CCLFModifiableItemImpl::DataType( const TCLFFieldId aFieldId ) const
    {
    if (iIntMap.Find(aFieldId))
        {
        return ECLFItemDataTypeTInt32;
        }
    else if (iTimeMap.Find(aFieldId))
        {
        return ECLFItemDataTypeTTime;
        }
    else if (iTextMap.Find(aFieldId))
        {
        return ECLFItemDataTypeDesC;
        }
    else
        {
        return ECLFItemDataTypeNull;
        }
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::GetField
// -----------------------------------------------------------------------------
//
TInt CCLFModifiableItemImpl::GetField( const TCLFFieldId aFieldId,
                                       TPtrC& aValue ) const
    {
    if (iTextMap.Find(aFieldId))
        {
        aValue.Set( (*iTextMap.Find(aFieldId))->Des() );
        return KErrNone;
        }
        
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::GetField
// -----------------------------------------------------------------------------
//
TInt CCLFModifiableItemImpl::GetField( const TCLFFieldId aFieldId,
                                       TInt32& aValue ) const
    {
    if (iIntMap.Find(aFieldId))
        {
        aValue = *iIntMap.Find(aFieldId);
        return KErrNone;
        }
        
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::GetField
// -----------------------------------------------------------------------------
//
TInt CCLFModifiableItemImpl::GetField( const TCLFFieldId aFieldId,
                                       TTime& aValue ) const
    {
    if (iTimeMap.Find(aFieldId))
        {
        aValue = TTime(*iTimeMap.Find(aFieldId));
        return KErrNone;
        }
       
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::AddFieldL
// -----------------------------------------------------------------------------
//
void CCLFModifiableItemImpl::AddFieldL( TCLFFieldId aFieldID,
                                        const TDesC& aValue )
    {
    iTextMap.InsertL( aFieldID, aValue.AllocL() );
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::AddFieldL
// -----------------------------------------------------------------------------
//
void CCLFModifiableItemImpl::AddFieldL( TCLFFieldId aFieldID, TInt32 aValue )
    {
    iIntMap.InsertL( aFieldID, aValue );
    }

// -----------------------------------------------------------------------------
// CCLFModifiableItemImpl::AddFieldL
// -----------------------------------------------------------------------------
//
void CCLFModifiableItemImpl::AddFieldL( TCLFFieldId aFieldID,
                                        const TTime& aValue )
    {
    iTimeMap.InsertL( aFieldID, aValue.Int64() );
    }

//  End of File
