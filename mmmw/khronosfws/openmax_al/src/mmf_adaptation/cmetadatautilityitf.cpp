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
 * Description: Metadata backend engine
 *
 */

#include <string.h>
#include <ctype.h>
#include <uri8.h>
#include "cmetadatautilityitf.h"
#include "profileutilmacro.h"

CMetadataUtilityItf::CMetadataUtilityItf() :
    m_pS60Util(NULL), m_pHXUtil(NULL)
    {
    }

CMetadataUtilityItf::~CMetadataUtilityItf()
    {
    if (m_pS60Util)
        {
        delete m_pS60Util;
        }

    if (m_pHXUtil)
        {
        delete m_pHXUtil;
        }
    }

CMetadataUtilityItf* CMetadataUtilityItf::New(char* uri)
    {
    CMetadataUtilityItf* p_mdutilitf = new CMetadataUtilityItf;

    p_mdutilitf->ParseSource(uri); //ignore error

    return p_mdutilitf;
    }

TInt CMetadataUtilityItf::OpenSource(char* pOrigUri)
    {
    //Make a local copy for uri
    if (!pOrigUri)
        return XA_RESULT_PARAMETER_INVALID;

    int uriLen = strlen(pOrigUri);
    char* uri = new char[uriLen + 1];
    if (!uri)
        {
        return XA_RESULT_MEMORY_FAILURE;
        }
    strncpy(uri, pOrigUri, uriLen);
    uri[uriLen] = '\0';
    ////////////////////////////////////////////
    _LIT8(KFileSlash,"file:///");
    TInt fileslashlen = KFileSlash().Length();

    TPtr8 fileuri((TUint8*) uri, strlen(uri), strlen(uri));
    TPtr8 filepath = fileuri.RightTPtr(strlen(uri) - fileslashlen);

    TInt pos = filepath.LocateReverse(':');
    if (pos != KErrNotFound)
        {
        fileuri.Delete(fileslashlen + pos, 1);
        }

    TUriParser8 localfileUri;
    TInt ret = localfileUri.Parse(fileuri);
    if (ret == KErrNone)
        {
        HBufC* file = NULL;
        TRAP(ret,file = localfileUri.GetFileNameL());
        if (ret == KErrNone)
            {
            if (m_pS60Util)
                {
                ret = m_pS60Util->ParseSource(*file);
                }
            else if (m_pHXUtil)
                {
                ret = m_pHXUtil->ParseSource(*file);
                }
            else
                {
                ret = KErrNotFound;
                }
            }

        delete file;
        }

    if (uri)
        {
        delete[] uri;
        }

    if (ret != KErrNone)
        {
        //delete the utilities
        if (m_pS60Util)
            {
            delete m_pS60Util;
            m_pS60Util = NULL;
            }

        if (m_pHXUtil)
            {
            delete m_pHXUtil;
            m_pHXUtil = NULL;
            }

        }
    return ret;
    }

TInt CMetadataUtilityItf::ExtractUCS2(TDesC& inDes, char* outPtr, TInt maxLen)
    {
    TPtrC tempPtr = inDes.Left((maxLen / 2) - 1); //save last one for null terminator
    TInt outLen = tempPtr.Length() + 1;

    TPtr16 outDes((unsigned short*) outPtr, outLen);
    outDes.Copy(tempPtr);
    outDes.ZeroTerminate();

    return outLen * 2; //return size
    }

TInt CMetadataUtilityItf::CalculateNumMetadataItems(TUint*numItems)
    {
    *numItems = 0;

    if (m_pS60Util)
        {
        return m_pS60Util->CalculateNumMetadataItems(numItems);
        }
    else if (m_pHXUtil)
        {
        return m_pHXUtil->CalculateNumMetadataItems(numItems);
        }

    return KErrNone;
    }

char* CMetadataUtilityItf::GetKey(TInt index)
    {
    if (m_pS60Util)
        {
        return m_pS60Util->GetKey(index);
        }
    else if (m_pHXUtil)
        {
        return m_pHXUtil->GetKey(index);
        }

    return NULL;
    }

TInt CMetadataUtilityItf::GetValueSize(TInt index)
    {
    if (m_pS60Util)
        {
        return m_pS60Util->GetValueSize(index);
        }
    else if (m_pHXUtil)
        {
        return m_pHXUtil->GetValueSize(index);
        }

    return 0;
    }

TInt CMetadataUtilityItf::GetValue(TInt index, char* data, TInt maxLength,
        TInt* outSize, TInt* encodingType)
    {
    if (m_pS60Util)
        {
        return m_pS60Util->GetValue(index, data, maxLength, outSize,
                encodingType);
        }
    else if (m_pHXUtil)
        {
        return m_pHXUtil->GetValue(index, data, maxLength, outSize,
                encodingType);
        }

    return 0;
    }

TInt CMetadataUtilityItf::ParseSource(char* uri)
    {
    char* tempPtr = NULL;
    char extension[MAX_EXTENSION_SIZE] =
        {
        0
        };

    tempPtr = strchr(uri, (int) '.');
    strncpy(extension, tempPtr, sizeof(tempPtr));

    for (unsigned int i = 0; i < sizeof(extension); i++)
        {
        extension[i] = tolower(extension[i]);
        }

    //if s60 util in use
    if (m_pS60Util)
        {
        //reset existing instace
        TInt ret = m_pS60Util->Reset();
        if (!CS60MetadataUtilityItf::IsSupportedExtension(extension))
            {
            delete m_pS60Util;
            m_pS60Util = NULL;
            }
        }

    if (m_pHXUtil)
        {
        //reset existing instace
        TInt ret = m_pHXUtil->Reset();
        if (CS60MetadataUtilityItf::IsSupportedExtension(extension))
            {
            delete m_pHXUtil;
            m_pHXUtil = NULL;
            }
        }

    if (!m_pS60Util && !m_pHXUtil)
        {

        if (CS60MetadataUtilityItf::IsSupportedExtension(extension))
            {
            m_pS60Util = CS60MetadataUtilityItf::New();
            }
        else
            {
            m_pHXUtil = CHXMetadataUtilityItf::New();
            }
        }

    return OpenSource(uri);
    }

bool CS60MetadataUtilityItf::IsSupportedExtension(char *extn)
    {
    if ((!strcasecmp(extn, ".mp3")) || (!strcasecmp(extn, ".wma"))
            || (!strcasecmp(extn, ".aac")) || (!strcasecmp(extn, ".wav"))
            || (!strcasecmp(extn, ".m4a")))
        {
        return true;
        }

    return false;
    }

CS60MetadataUtilityItf* CS60MetadataUtilityItf::New()
    {
    CS60MetadataUtilityItf* self = new CS60MetadataUtilityItf();

    TInt err = KErrGeneral;
    TRAP(err, self->ConstructL());

    if (err == KErrNone)
        {
        return self;
        }

    delete self;
    return NULL;
    }

CS60MetadataUtilityItf::CS60MetadataUtilityItf() :
    pMetaDataUtility(NULL)
    {
    }

void CS60MetadataUtilityItf::ConstructL()
    {
    TAG_TIME_PROFILING_BEGIN;
    pMetaDataUtility = CMetaDataUtility::NewL();
    TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
    }

CS60MetadataUtilityItf::~CS60MetadataUtilityItf()
    {
    if (pMetaDataUtility)
        {
        TInt err = KErrGeneral;

        TAG_TIME_PROFILING_BEGIN;
        TRAP(err, pMetaDataUtility->ResetL());
        delete pMetaDataUtility;
        TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
        }

    pMetaDataUtility = NULL;
    }

TInt CS60MetadataUtilityItf::ParseSource(TDesC& fileName)
    {
    TInt err = KErrGeneral;

    if (pMetaDataUtility)
        {

        TAG_TIME_PROFILING_BEGIN;
        //open with the file handle
        TRAP(err, pMetaDataUtility->OpenFileL(fileName)); 
        TAG_TIME_PROFILING_END; 
        PRINT_TO_CONSOLE_TIME_DIFF;

        if (err == KErrNone)
            {
            uNumMetadataItems = pMetaDataUtility->MetaDataCount();
            }
        }

    return err;
    }

TInt CS60MetadataUtilityItf::CalculateNumMetadataItems(TUint* numItems)
    {
    TInt err = KErrGeneral;

    if (pMetaDataUtility && numItems)
        {
        *numItems = uNumMetadataItems;
        err = KErrNone;
        }

    return err;
    }

char* CS60MetadataUtilityItf::KeyMapping(TMetaDataFieldId fldID)
    {
    switch (fldID)
        {

        case EMetaDataSongTitle:
            {
            return "KhronosTitle";
            }
        case EMetaDataArtist:
            {
            return "KhronosArtist";
            }
        case EMetaDataAlbum:
            {
            return "KhronosAlbum";
            }
        case EMetaDataYear:
            {
            return "KhronosYear";
            }
        case EMetaDataComment:
            {
            return "KhronosComment";
            }
        case EMetaDataAlbumTrack:
            {
            return "KhronosTrackNumber";
            }
        case EMetaDataGenre:
            {
            return "KhronosGenre";
            }
        case EMetaDataComposer:
            {
            return "Composer"; //Non Standard
            }
        case EMetaDataCopyright:
            {
            return "KhronosCopyright";
            }
        case EMetaDataOriginalArtist:
            {
            return "Original Artist"; //Non Standard
            }
        case EMetaDataUserUrl:
        case EMetaDataUrl:
            {
            return "KhronosContentURL";
            }
        case EMetaDataJpeg:
            {
            return "attachedpicture";//"KhronosAlbumArtJPEG";
            }
        case EMetaDataVendor:
            {
            return "Vendor"; //Non Standard
            }
        case EMetaDataRating:
            {
            return "KhronosRating";
            }
        case EMetaDataDuration:
            {
            return "Duration"; //Non Standard
            }
        default:
            {
            }
        }

    return "UnSupported";
    }

TInt CS60MetadataUtilityItf::ValueEncoding(TMetaDataFieldId fldID)
    {
    switch (fldID)
        {
        case EMetaDataJpeg:
            {
            return CMetadataUtilityItf::EBinaryEncoding;
            }

        case EMetaDataSongTitle:
        case EMetaDataArtist:
        case EMetaDataAlbum:
        case EMetaDataYear:
        case EMetaDataComment:
        case EMetaDataAlbumTrack:
        case EMetaDataGenre:
        case EMetaDataComposer:
        case EMetaDataCopyright:
        case EMetaDataOriginalArtist:
        case EMetaDataUserUrl:
        case EMetaDataUrl:
        case EMetaDataVendor:
        case EMetaDataRating:
        case EMetaDataDuration:
        default:
            {
            return CMetadataUtilityItf::EUnicodeEncoding;
            }
        }

    }
char* CS60MetadataUtilityItf::GetKey(TInt index)
    {

    if (pMetaDataUtility && index < uNumMetadataItems)
        {

        TMetaDataFieldId fieldId;

        TAG_TIME_PROFILING_BEGIN;
        TInt err = KErrGeneral;
        TRAP(err, pMetaDataUtility->MetaDataFieldsL().FieldIdAt( index, fieldId )); 
        TAG_TIME_PROFILING_END; 
        PRINT_TO_CONSOLE_TIME_DIFF;

        if ((err == KErrNone) && (fieldId != EUnknownMetaDataField))
            {
            return KeyMapping(fieldId);
            }
        }
    return NULL;
    }

TInt CS60MetadataUtilityItf::GetValueSize(TInt index)
    {
    if (pMetaDataUtility && index < uNumMetadataItems)
        {

        TMetaDataFieldId fieldId;
        TInt err = KErrGeneral;
        TRAP(err, pMetaDataUtility->MetaDataFieldsL().FieldIdAt( index, fieldId ));

        if ((err == KErrNone) && (fieldId != EUnknownMetaDataField))
            {
            const CMetaDataFieldContainer* iContainer = NULL;
            TRAP(err, iContainer = &pMetaDataUtility->MetaDataFieldsL());
            if (err == KErrNone)
                {
                if (ValueEncoding(fieldId)
                        == CMetadataUtilityItf::EUnicodeEncoding)
                    {
                    TPtrC field = iContainer->Field(fieldId);
                    if (field != KNullDesC)
                        {
                        return field.Size() + 2; //additional character (two bytes) for null terminator
                        }
                    }
                else //Binary
                    {
                    TPtrC8 field8 = iContainer->Field8(fieldId);
                    if (field8 != KNullDesC8)
                        {
                        return field8.Size();
                        }
                    }
                }
            }
        }

    return 0;
    }

TInt CS60MetadataUtilityItf::GetValue(TInt index, char* data, TInt maxLength, //in params
        TInt* outSize, TInt* encodingType) //out params
    {

    TInt retValueSize = 0;
    *encodingType = CMetadataUtilityItf::EUnknownEncoding;

    if (pMetaDataUtility && index < uNumMetadataItems)
        {

        TMetaDataFieldId fieldId;
        TInt err = KErrGeneral;
        TRAP(err, pMetaDataUtility->MetaDataFieldsL().FieldIdAt( index, fieldId ));

        if ((err == KErrNone) && (fieldId != EUnknownMetaDataField))
            {

            const CMetaDataFieldContainer* iContainer = NULL;
            TRAP(err, iContainer = &pMetaDataUtility->MetaDataFieldsL());
            if (err == KErrNone)
                {
                *encodingType = ValueEncoding(fieldId);
                if (*encodingType == CMetadataUtilityItf::EUnicodeEncoding)
                    {

                    TAG_TIME_PROFILING_BEGIN;
                    TPtrC field = iContainer->Field(fieldId);
                    TAG_TIME_PROFILING_END;
                    PRINT_TO_CONSOLE_TIME_DIFF;
                    if (field != KNullDesC)
                        {
                        *outSize = CMetadataUtilityItf::ExtractUCS2(field,
                                data, maxLength);
                        retValueSize = field.Size() + 2; //actual size
                        }
                    }
                else //Binary
                    {

                    TAG_TIME_PROFILING_BEGIN;
                    TPtrC8 field8 = iContainer->Field8(fieldId);
                    TAG_TIME_PROFILING_END;
                    PRINT_TO_CONSOLE_TIME_DIFF;
                    if (field8 != KNullDesC8)
                        {
                        *outSize
                                = (maxLength > field8.Size())
                                                              ? field8.Size()
                                                                 : maxLength;
                        memcpy(data, field8.Ptr(), *outSize);
                        retValueSize = field8.Size();
                        }
                    }
                }
            }
        }

    return retValueSize;
    }

TInt CS60MetadataUtilityItf::Reset()
    {
    TInt err = KErrNone;
    if (pMetaDataUtility)
        {
        TRAP(err, pMetaDataUtility->ResetL());
        }

    return err;
    }

CHXMetadataUtilityItf::CHXMetadataUtilityItf() :
    pHXMetaDataUtility(NULL)
    {
    }

void CHXMetadataUtilityItf::ConstructL()
    {
    TAG_TIME_PROFILING_BEGIN;
    pHXMetaDataUtility = CHXMetaDataUtility::NewL();
    TAG_TIME_PROFILING_END; 
    PRINT_TO_CONSOLE_TIME_DIFF;
    }

CHXMetadataUtilityItf* CHXMetadataUtilityItf::New()
    {
    CHXMetadataUtilityItf* self = new CHXMetadataUtilityItf();

    if (self)
        {
        TInt err = KErrGeneral;
        TRAP(err, self->ConstructL());

        if (err != KErrNone)
            {
            delete self;
            self = NULL;
            }
        }

    return self;
    }

CHXMetadataUtilityItf::~CHXMetadataUtilityItf()
    {
    if (pHXMetaDataUtility)
        {
        TInt err = KErrGeneral;

        TAG_TIME_PROFILING_BEGIN;
        TRAP(err, pHXMetaDataUtility->ResetL());
        delete pHXMetaDataUtility;
        TAG_TIME_PROFILING_END; 
        PRINT_TO_CONSOLE_TIME_DIFF;
        }

    pHXMetaDataUtility = NULL;
    }

TInt CHXMetadataUtilityItf::Reset()
    {
    TInt err = KErrNone;
    if (pHXMetaDataUtility)
        {
        TRAP(err, pHXMetaDataUtility->ResetL());
        }

    return err;
    }

TInt CHXMetadataUtilityItf::ParseSource(TDesC& fileName)
    {
    TInt err = KErrGeneral;

    if (pHXMetaDataUtility)
        {
        //open with the file handle
        TAG_TIME_PROFILING_BEGIN;
        TRAP(err, pHXMetaDataUtility->OpenFileL(fileName)); 
        TAG_TIME_PROFILING_END; 
        PRINT_TO_CONSOLE_TIME_DIFF;

        if (err == KErrNone)
            {
            return pHXMetaDataUtility->GetMetaDataCount(uNumMetadataItems);
            }
        }

    return err;
    }

TInt CHXMetadataUtilityItf::CalculateNumMetadataItems(TUint* numItems)
    {
    TInt err = KErrGeneral;

    if (pHXMetaDataUtility && numItems)
        {
        *numItems = uNumMetadataItems;
        err = KErrNone;
        }

    return err;
    }

char* CHXMetadataUtilityItf::KeyMapping(HXMetaDataKeys::EHXMetaDataId fldID)
    {

    switch (fldID)
        {

        case HXMetaDataKeys::EHXTitle:
            {
            return "KhronosTitle";
            }
        case HXMetaDataKeys::EHXPerformer:
            {
            return "KhronosArtist";
            }
        case HXMetaDataKeys::EHXDescription:
            {
            return "KhronosComment";
            }
        case HXMetaDataKeys::EHXGenre:
            {
            return "KhronosGenre";
            }
        case HXMetaDataKeys::EHXAuthor:
            {
            return "Composer"; //Non Standard
            }
        case HXMetaDataKeys::EHXCopyright:
            {
            return "KhronosCopyright";
            }
        case HXMetaDataKeys::EHXContentURI:
            {
            return "KhronosContentURL";
            }
        case HXMetaDataKeys::EHXDuration:
            {
            return "Duration"; //Non Standard
            }
        case HXMetaDataKeys::EHXClipBitRate:
            {
            return "ClipBitRate"; //non-standard
            }
        case HXMetaDataKeys::EHXVideoBitRate:
            {
            return "VideoBitRate"; // non-standard
            }
        case HXMetaDataKeys::EHXAudioBitRate:
            {
            return "AudioBitRate";
            }
        case HXMetaDataKeys::EHXCodec:
            {
            return "Codec";
            }
        case HXMetaDataKeys::EHXFrameSize:
            {
            return "Resolution";
            }
        case HXMetaDataKeys::EHXFramesPerSecond:
            {
            return "FrameRate";
            }
        case HXMetaDataKeys::EHXStreamCount:
            {
            return "Stream Count";
            }
        case HXMetaDataKeys::EHXLiveStream:
            {
            return "Live Stream";
            }
        case HXMetaDataKeys::EHXSeekable:
            {
            return "Seekable";
            }
        case HXMetaDataKeys::EHXContentType:
            {
            return "Content Type";
            }
        case HXMetaDataKeys::EHXFormat:
            {
            return "Format";
            }
        case HXMetaDataKeys::EHXQuality:
            {
            return "Quality";
            }
        case HXMetaDataKeys::EHXAbstract:
            {
            return "Abstract";
            }
        case HXMetaDataKeys::EHXMimeType:
            {
            return "MimeType";
            }
        case HXMetaDataKeys::EHXIconURI:
            {
            return "Icon URI";
            }
        case HXMetaDataKeys::EHXEPreviewURI:
            {
            return "Preview URI";
            }
        case HXMetaDataKeys::EHXContentID:
            {
            return "Content ID";
            }
        case HXMetaDataKeys::EHXInfoURL:
            {
            return "Info URL";
            }
        default:
            {
            }
        }

    return "UnSupported";
    }

TInt CHXMetadataUtilityItf::ValueEncoding(HXMetaDataKeys::EHXMetaDataId fldID)
    {
    switch (fldID)
        {
        default:
            {
            return CMetadataUtilityItf::EUnicodeEncoding;
            }
        }

    }
char* CHXMetadataUtilityItf::GetKey(TInt index)
    {

    if (pHXMetaDataUtility && index < uNumMetadataItems)
        {
        HXMetaDataKeys::EHXMetaDataId id;
        HBufC* pDes;

        TAG_TIME_PROFILING_BEGIN;
        TInt err = pHXMetaDataUtility->GetMetaDataAt(index, id, pDes);
        TAG_TIME_PROFILING_END; 
        PRINT_TO_CONSOLE_TIME_DIFF;

        if (err == KErrNone)
            {
            return KeyMapping(id);
            }
        }
    return NULL;
    }

TInt CHXMetadataUtilityItf::GetValueSize(TInt index)
    {
    if (pHXMetaDataUtility && index < uNumMetadataItems)
        {
        HXMetaDataKeys::EHXMetaDataId id;
        HBufC* pDes;

        TInt err = pHXMetaDataUtility->GetMetaDataAt(index, id, pDes);

        if (err == KErrNone)
            {
            if (ValueEncoding(id) == CMetadataUtilityItf::EUnicodeEncoding)
                {
                return pDes->Size() + 2; //additional character (two bytes) for null terminator
                }
            else //Binary
                {
                //no support
                }
            }
        }

    return 0;
    }

TInt CHXMetadataUtilityItf::GetValue(TInt index, char* data, TInt maxLength, //in params
        TInt* outSize, TInt* encodingType) //out params
    {

    TInt retValueSize = 0;
    *encodingType = CMetadataUtilityItf::EUnknownEncoding;

    if (pHXMetaDataUtility && index < uNumMetadataItems)
        {
        HXMetaDataKeys::EHXMetaDataId id;
        HBufC* pDes;

        TAG_TIME_PROFILING_BEGIN;
        TInt err = pHXMetaDataUtility->GetMetaDataAt(index, id, pDes);
        TAG_TIME_PROFILING_END; 
        PRINT_TO_CONSOLE_TIME_DIFF;

        *encodingType = ValueEncoding(id);

        if (err == KErrNone)
            {
            if (*encodingType == CMetadataUtilityItf::EUnicodeEncoding)
                {
                *outSize = CMetadataUtilityItf::ExtractUCS2(*pDes, data,
                        maxLength);
                retValueSize = pDes->Size() + 2; //actual Size	
                }
            else //Binary
                {
                //no support
                }
            }
        }

    return retValueSize;
    }

extern "C"
    {

    void* mmf_metadata_utility_init(char* uri)
        {
        return CMetadataUtilityItf::New(uri);
        }

    void mmf_metadata_utility_destroy(void* context)
        {
        delete ((CMetadataUtilityItf*) context);
        }

    XAresult mmf_metadata_utility_parse_source(void* context, char* uri)
        {
        TInt err = ((CMetadataUtilityItf*) context)->ParseSource(uri);

        if (err == KErrNone)
            {
            return XA_RESULT_SUCCESS;
            }

        return XA_RESULT_PARAMETER_INVALID;
        }

    XAresult mmf_get_item_count(void* context, XAuint32* itemCount)
        {
        if (itemCount)
            {
            TInt err =((CMetadataUtilityItf*) (context))->CalculateNumMetadataItems(
                                    (TUint *) itemCount);
            if (err == KErrNone)
                {
                return XA_RESULT_SUCCESS;
                }
            }

        return XA_RESULT_PARAMETER_INVALID;
        }

    XAresult mmf_get_key_size(void* context, XAuint32 keyIndex,
            XAuint32* keySize)
        {
        char* key = ((CMetadataUtilityItf*) (context))->GetKey(keyIndex);
        if (key && keySize)
            {
            *keySize = (strlen(key) + sizeof(XAMetadataInfo));

            return XA_RESULT_SUCCESS;
            }

        return XA_RESULT_PARAMETER_INVALID;
        }

    XAresult mmf_get_key(void* context, XAuint32 index, XAuint32 keySize,
            XAMetadataInfo *pKey)
        {
        XAresult ret = XA_RESULT_PARAMETER_INVALID;

        TInt keyDataSize = keySize - sizeof(XAMetadataInfo) + 1;
        char* ascKey = ((CMetadataUtilityItf*) (context))->GetKey(index);

        if (ascKey && keyDataSize)
            {
            TInt ascKeySize = strlen(ascKey);
            TInt outSize = (ascKeySize >= keyDataSize)
                                                       ? (keyDataSize - 1)
                                                          : ascKeySize;

            pKey->size = outSize + 1;
            pKey->encoding = XA_CHARACTERENCODING_ASCII;
            strcpy((char *) (pKey->langCountry), "en-us");
            strncpy((char *) (pKey->data), ascKey, outSize);
            pKey->data[outSize] = '\0';

            if (ascKeySize >= keyDataSize)
                {
                ret = XA_RESULT_BUFFER_INSUFFICIENT;
                }
            else
                {
                ret = XA_RESULT_SUCCESS;
                }
            }

        ret = XA_RESULT_SUCCESS;
        return ret;
        }

    XAresult mmf_get_value_size(void* context, XAuint32 index,
            XAuint32 *pValueSize)
        {
        if (pValueSize)
            {
            *pValueSize = ((CMetadataUtilityItf*) (context))->GetValueSize(
                    index) + sizeof(XAMetadataInfo) - 1; 
            //XAMetadataInfo already includes one byte for Data

            return XA_RESULT_SUCCESS;
            }

        return XA_RESULT_PARAMETER_INVALID;
        }

    XAresult mmf_get_value(void* context, XAuint32 index, XAuint32 valueSize,
            XAMetadataInfo *pValue)
        {
        XAresult ret = XA_RESULT_PARAMETER_INVALID;
        TInt dataSize = valueSize - sizeof(XAMetadataInfo) + 1;
        TInt outLen = 0, encodingType = CMetadataUtilityItf::EUnknownEncoding;

        if (dataSize > 0)
            {

            TInt actualDataSize =
                    ((CMetadataUtilityItf*) (context))->GetValue(index,
                            (char*) pValue->data, dataSize, &outLen,
                            &encodingType);

            pValue->size = outLen;
            pValue->encoding
                    = (encodingType == CMetadataUtilityItf::EUnicodeEncoding) 
                    ? XA_CHARACTERENCODING_UTF16LE : XA_CHARACTERENCODING_BINARY;
            strcpy((char *) (pValue->langCountry), "en-us");

            if (!actualDataSize)
                {
                return XA_RESULT_INTERNAL_ERROR;
                }
            if (actualDataSize > dataSize)
                {
                ret = XA_RESULT_BUFFER_INSUFFICIENT;
                }
            else
                {
                ret = XA_RESULT_SUCCESS;
                }
            }

        return ret;
        }
    }

