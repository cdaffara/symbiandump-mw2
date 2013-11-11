/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of plugin info
*
*/



// INCLUDE FILES
#include <mpxuser.h>
#include <mpxlog.h>
#include <mpxplugininfo.h>


// CONSTANTS
_LIT8(KMPXTagMatch, "*<?>*");
_LIT8(KMPXTagEnd,"*<*?>*");

_LIT8(KMPXSupportUidsTag, "<p>");
_LIT8(KMPXPluginTypeTag, "<t>" );
_LIT8(KMPXPluginFlagTag, "<f>" );
_LIT8(KMPXPluginPriorityTag, "<i>" );
_LIT8(KMPXSupportSchemasTag,"<s>");
_LIT8(KMPXSupportExtensionsTag,"<e>");

_LIT8(KMPXSupportAppUidTag, "<a>");
_LIT8(KMPX0x, "0x");
_LIT8(KMPX0X, "0X");
const TUint8 KMPXSemicolon = ';';
const TInt KMPXTagLength = 3;


// ============================ INTERNAL CLASSES ==============================

/**
Utility class used to parse data separated by xml-style tags.
*/
class TaggedDataParser
    {
public:

    /**
     *Splits aData into xml-style tags and values, and gets aClient to process
     *each tag/value pair.
    */
    static void ParseTaggedDataL(const TDesC8& aData,
                                 MTaggedDataParserClient& aClient);

    /**
     * Converts a string to a Tint. The string may contain hex value or decimal value
    */
    static void ConvertText8ToTIntL(const TDesC8& aData, TInt& aInt);

    /**
    * Extracts elemements separated by semicolon into array descriptor
    *
    * @param aText original content to extract elements from
    * @param aArray the array to add the extracted elements to
    */
    static void ExtractIntoArrayL(const TDesC8& aText, CDesCArray*& aArray);

    /**
    * Add an item into descriptor array
    *
    * @param aItem the item to be added to the descriptor array
    * @param aArray the array to add the item to
    */
    static void AddItemIntoArrayL(TPtrC8& aItem, CDesCArray*& aArray);
    };

// ----------------------------------------------------------------------------
// Parser xml-styled data
// ----------------------------------------------------------------------------
//
void TaggedDataParser::ParseTaggedDataL(
    const TDesC8& aData,
    MTaggedDataParserClient& aClient)
    {
    TPtrC8 data(aData);
    TInt readPosition = 0;
    TBool moreData = data.Length() ? ETrue : EFalse;
    while (moreData)
        {
        // Assumes that this segment will begin with a tag
        TPtrC8 restOfData = data.Mid(readPosition);

        TInt endPos = restOfData.MatchF(KMPXTagMatch);
        if (endPos == KErrNotFound)
            User::Leave(KErrCorrupt);

        // extract the tag
        TPtrC8 tag = restOfData.Left(KMPXTagLength);
        readPosition += KMPXTagLength;
        // Find the next tag
        restOfData.Set(data.Mid(readPosition));
        endPos = restOfData.MatchF(KMPXTagMatch);

        TPtrC8 tagData;
        if (endPos == KErrNotFound)
            { // no more tag available
            endPos = restOfData.MatchF(KMPXTagEnd);
            if (endPos == KErrNotFound)
                { // If we didn't find a tag ender, we must be at the end of
                  // the data
                tagData.Set(restOfData);
                readPosition = restOfData.Length();
                }
            else
                {
                tagData.Set(restOfData.Left(endPos));
                }
            moreData = EFalse;
            }
        else
            {
            // strip the end tag from data if found
            TInt stripEndPos = restOfData.MatchF(KMPXTagEnd);
            if ( stripEndPos != KErrNotFound )
                {
                tagData.Set(restOfData.Left(stripEndPos));
                }
            else
                {
                tagData.Set(restOfData.Left(endPos));
                }
            readPosition += endPos;
            }
        aClient.ProcessTaggedDataL(tag, tagData);
        }
    }

// ----------------------------------------------------------------------------
// Convert text into a integer
// ----------------------------------------------------------------------------
//
void TaggedDataParser::ConvertText8ToTIntL(
    const TDesC8& aData,
    TInt& aInt)
    {
    // Determine whether hex or decimal then parse as such
    if (((aData.FindF(KMPX0x) == 0) || (aData.FindF(KMPX0X) == 0)) &&
        (aData.Length() >= KMPXTagLength))
        {
        // only take the characters after "0x"
        TLex8 lex(aData.Right(aData.Length()-2));
        TUint value( 0 );
        User::LeaveIfError(lex.Val(value, EHex));
        aInt = value;
        }
    else if (aData.Length() > 0)
        {
        TLex8 lex(aData);
        TInt32 value(0);
        User::LeaveIfError(lex.Val(value));
        aInt = value;
        }
    else
        {
        User::Leave(KErrCorrupt);
        }
    }

// ---------------------------------------------------------------------------
// Extracts elemements separated by semicolon into array descriptor
// ----------------------------------------------------------------------------
//
void TaggedDataParser::ExtractIntoArrayL(
    const TDesC8& aText,
    CDesCArray*& aArray)
    {
    const TUint8* p = aText.Ptr();
    TInt startPos(0);
    TInt pos(0);
    TInt len=aText.Length();
    TPtrC8 element;

    while (pos < len)
        {
        if (*(p+pos) == KMPXSemicolon)
            {
            if (pos-startPos > 0)
                {
                element.Set(p+startPos, pos - startPos);
                AddItemIntoArrayL(element, aArray);
                }
            startPos = pos + 1;
            }
        ++pos;
        }
    if (startPos < len && pos - startPos > 0)
        {// last item
        element.Set(p+startPos, pos - startPos);
        AddItemIntoArrayL(element, aArray);
        }
    }

// ---------------------------------------------------------------------------
// Add an item into descriptor
// ----------------------------------------------------------------------------
//
void TaggedDataParser::AddItemIntoArrayL(TPtrC8& aItem, CDesCArray*& aArray)
    {
    HBufC* item = MPXUser::AllocL(aItem);
    CleanupStack::PushL(item);
    //ignore leave when insert the same item
    TRAP_IGNORE(aArray->InsertIsqL(*item));
    CleanupStack::PopAndDestroy(item);
    }


// ============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXPluginInfo::CMPXPluginInfo()
    {
    }

// ---------------------------------------------------------------------------
// By default Symbian 2nd phase constructor is private.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginInfo::ConstructL(
    const CImplementationInformation& aData )
    {
    iPluginName = aData.DisplayName().AllocL();
    iPluginUid = aData.ImplementationUid();
    iVersion = aData.Version();

    iSupportedSchemas = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    iSupportedMimeTypes = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);
    iSupportedExtensions = new(ELeave)CDesCArrayFlat(KMPXArrayGranularity);

    TaggedDataParser::ExtractIntoArrayL(aData.DataType(), iSupportedMimeTypes);
    // Parse the opaque data...
    TaggedDataParser::ParseTaggedDataL(aData.OpaqueData(), *this);
    // Add plugin type into supported ids array
    TInt err=iSupportedUids.InsertInOrder(iPluginTypeUid,MPXUser::CompareUids);
    if (KErrNone!=err && KErrAlreadyExists!=err)
        { // ignore duplicated items
        User::Leave(err);
        }
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXPluginInfo* CMPXPluginInfo::NewL(
    const CImplementationInformation& aData )
    {
    CMPXPluginInfo* self = CMPXPluginInfo::NewLC( aData );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXPluginInfo* CMPXPluginInfo::NewLC(
    const CImplementationInformation& aData )
    {
    CMPXPluginInfo* self = new ( ELeave ) CMPXPluginInfo();
    CleanupStack::PushL( self );
    self->ConstructL( aData );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXPluginInfo::~CMPXPluginInfo()
    {
    delete iPluginName;
    iSupportedUids.Close();
    delete iSupportedSchemas;
    delete iSupportedMimeTypes;
    delete iSupportedExtensions;
    }

// ---------------------------------------------------------------------------
// Process a tagged data
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginInfo::ProcessTaggedDataL(const TDesC8& aTag, const TDesC8& aData)
    {
    if (aTag==KMPXSupportUidsTag)
        {
        ExtractSupportedUidsL(aData);
        }
    else if (aTag==KMPXSupportAppUidTag)
        {
        TInt type(0);
        TaggedDataParser::ConvertText8ToTIntL(aData, type);
        iSupportedAppUid = TUid::Uid(type);
        MPX_DEBUG2("Plugin supported App Uid = %d", iSupportedAppUid);
        }
    else if (aTag==KMPXPluginTypeTag)
        {
        TInt type(0);
        TaggedDataParser::ConvertText8ToTIntL(aData, type);
        iPluginTypeUid = TUid::Uid(type);
        }
    else if (aTag==KMPXPluginFlagTag)
        {
        TInt value(0);
        TaggedDataParser::ConvertText8ToTIntL(aData, value);
        iFlags = (TUint)value;
        }
    else if (aTag==KMPXPluginPriorityTag)
        {
        iPriority = 0;
        TaggedDataParser::ConvertText8ToTIntL(aData, iPriority);
        }
    else if (aTag==KMPXSupportSchemasTag)
        {
        iSupportedSchemas->Reset();
        TaggedDataParser::ExtractIntoArrayL(aData, iSupportedSchemas);
        }
    else if (aTag==KMPXSupportExtensionsTag)
        {
        iSupportedExtensions->Reset();
        TaggedDataParser::ExtractIntoArrayL(aData, iSupportedExtensions);
        }
    else // let derived class to process the data
        {
        ProcessTaggedDataExL(aTag, aData);
        }
    }

// ---------------------------------------------------------------------------
// Default implementation of a extended tagged data
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginInfo::ProcessTaggedDataExL(
    const TDesC8& /*aTag*/,
    const TDesC8& /*aData*/)
    {
    // Do nothing; derived class can add extended tags
    }
// ---------------------------------------------------------------------------
// Returns plugin's display name.
// ---------------------------------------------------------------------------
//
const TDesC& CMPXPluginInfo::DisplayName() const
    {
    return iPluginName ? static_cast<const TDesC&>( *iPluginName ) : KNullDesC;
    }

// ---------------------------------------------------------------------------
// Returns plugin's implementation uid.
// ---------------------------------------------------------------------------
//
EXPORT_C const TUid& CMPXPluginInfo::ImplementationUid() const
    {
    return iPluginUid;
    }

// ---------------------------------------------------------------------------
// Returns plugin's supported uids.
// ---------------------------------------------------------------------------
//
const TArray<TUid> CMPXPluginInfo::SupportedUids() const
    {
    return iSupportedUids.Array();
    }

// ---------------------------------------------------------------------------
// Returns plugin's plugin type.
// ---------------------------------------------------------------------------
//
const TUid& CMPXPluginInfo::PluginType() const
    {
    return iPluginTypeUid;
    }

// ---------------------------------------------------------------------------
// Returns Plugin's supported App Uid.
// ---------------------------------------------------------------------------
//
const TUid& CMPXPluginInfo::SupportedAppUid() const
    {    
    return iSupportedAppUid;
    }

// ---------------------------------------------------------------------------
// Returns plugin's view priority.
// ---------------------------------------------------------------------------
//
TInt CMPXPluginInfo::Priority() const
    {
    return iPriority;
    }

// ---------------------------------------------------------------------------
// Extracts plugin supported Uids from data.
// ---------------------------------------------------------------------------
//
void CMPXPluginInfo::ExtractSupportedUidsL(const TDesC8& aData)
    {
    iSupportedUids.Reset();
    ExtractUidsFromTextL( aData, iSupportedUids );
    }

// ---------------------------------------------------------------------------
// Parse a text string to extract a list of UIDs
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginInfo::ExtractUidsFromTextL( const TDesC8& aData,
                                                    RArray<TUid>& aArray )
    {
    CleanupClosePushL(aArray);
    aArray.Reset();
    const TUint8* p = aData.Ptr();
    TInt startPos(0);
    TInt pos(0);
    TInt len=aData.Length();
    TPtrC8 element;
    TInt err(KErrNone);
    while (pos < len)
        {
        if (*(p+pos) == KMPXSemicolon)
            {
            if (pos-startPos > 0)
                {
                element.Set(p+startPos, pos - startPos);
                TInt val(0);
                TaggedDataParser::ConvertText8ToTIntL(element, val);
                err = aArray.InsertInOrder(TUid::Uid(val),
                                           MPXUser::CompareUids);
                if (KErrNone!=err && KErrAlreadyExists!=err)
                    { // ignore duplicated items
                    User::Leave(err);
                    }
                }
            startPos = pos + 1;
            }
        ++pos;
        }
    if (startPos < len && pos - startPos > 0)
        {// last item
        element.Set(p+startPos, pos - startPos);
        TInt val(0);
        TaggedDataParser::ConvertText8ToTIntL(element, val);
        aArray.AppendL(TUid::Uid(val));
        }
    CleanupStack::Pop();
    }

// ---------------------------------------------------------------------------
// Return supported mime types
// ---------------------------------------------------------------------------
//
const CDesCArray& CMPXPluginInfo::SupportedMimeTypes() const
    {
    return *iSupportedMimeTypes;
    }

// ---------------------------------------------------------------------------
// Return supported extensions
// ---------------------------------------------------------------------------
//

const CDesCArray& CMPXPluginInfo::SupportedExtensions() const
    {
    return *iSupportedExtensions;
    }

// ---------------------------------------------------------------------------
// Return supported schemas
// ---------------------------------------------------------------------------
//
const CDesCArray& CMPXPluginInfo::SupportedSchemas() const
    {
    return *iSupportedSchemas;
    }

// ----------------------------------------------------------------------------
// CMPXPluginInfo::SupportUids
// ----------------------------------------------------------------------------
//
TBool CMPXPluginInfo::SupportUids(const TArray<TUid>& aUids) const
    {
    return MPXUser::CompareOrderedUidArrays(iSupportedUids.Array(), aUids)>=0;
    }

// ----------------------------------------------------------------------------
// CMPXPluginInfo::Flags
// ----------------------------------------------------------------------------
//
EXPORT_C TUint CMPXPluginInfo::Flags() const
    {
    return iFlags;
    }

// ----------------------------------------------------------------------------
// CMPXPluginInfo::Flags
// ----------------------------------------------------------------------------
//
TInt CMPXPluginInfo::Version() const
    {
    return iVersion;
    }

// ----------------------------------------------------------------------------
// CMPXPluginInfo::ReferenceCount
// ----------------------------------------------------------------------------
//
TInt& CMPXPluginInfo::ReferenceCount()
    {
    return iReferenceCount;
    }

// ----------------------------------------------------------------------------
// CMPXPluginInfo::ComparePluginInfoByPriority
// ----------------------------------------------------------------------------
//
TInt CMPXPluginInfo::ComparePluginInfoByPriority(
    const CMPXPluginInfo& aPluginInfo1,
    const CMPXPluginInfo& aPluginInfo2)
    {
    TInt ret(0);
    if (aPluginInfo1.Priority() > aPluginInfo2.Priority())
        {
        ret=-1;
        }
    else if (aPluginInfo1.Priority() < aPluginInfo2.Priority())
        {
        ret=1;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// Extracts elemements separated by semicolon into array descriptor
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPluginInfo::ExtractIntoArrayL(const TDesC8& aText, 
                                                CDesCArray*& aArray)
    {
    TaggedDataParser::ExtractIntoArrayL(aText, aArray);
    }

//  End of File
