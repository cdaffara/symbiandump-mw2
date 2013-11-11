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

#ifndef CMETADATA_UTILITY_ITF_H
#define CMETADATA_UTILITY_ITF_H

#ifdef __cplusplus

#include <e32base.h>
#include <e32std.h>
#include <e32hashtab.h>
#include <openmaxalwrapper.h>

#include <hxmetadatautil.h>
#include <MetaDataUtility.h>
#include <MetaDataFieldContainer.h>
#include <MetaDataField.hrh>

#define MAX_EXTENSION_SIZE 10
#define uint32 unsigned int

class CS60MetadataUtilityItf;
class CHXMetadataUtilityItf;

NONSHARABLE_CLASS(CMetadataUtilityItf)
    {
public:

    enum TValueEncodingType
        {
        EUnknownEncoding, EAsciiEncoding, EUnicodeEncoding, EBinaryEncoding,
        };

    static CMetadataUtilityItf* New(char* uri);

    CMetadataUtilityItf();
    virtual ~CMetadataUtilityItf();
    TInt ParseSource(char* uri);
    TInt OpenSource(char* uri);
    TInt CalculateNumMetadataItems(TUint*);
    char* GetKey(TInt index);
    TInt GetValueSize(TInt index);
    TInt GetValue(TInt index, char* data, TInt maxLength, TInt* outSize,
            TInt* encodingType);

    //helper function
    //extracts left part of input descriptor upto (maxLen-1) and copies content in outPtr
    //append a null terminator
    static TInt ExtractUCS2(TDesC& inDes, char* outPtr, TInt maxLen);

private:
    CS60MetadataUtilityItf* m_pS60Util;
    CHXMetadataUtilityItf* m_pHXUtil;
    };

NONSHARABLE_CLASS(CS60MetadataUtilityItf)
    {
public:

    static CS60MetadataUtilityItf* New();
    ~CS60MetadataUtilityItf();

    TInt CalculateNumMetadataItems(TUint*);

    char* GetKey(TInt index);
    TInt GetValueSize(TInt index);
    TInt GetValue(TInt index, char* data, TInt maxLength, TInt* outSize,
            TInt* encodingType);

    TInt ParseSource(TDesC&);

    char* KeyMapping(TMetaDataFieldId);
    TInt ValueEncoding(TMetaDataFieldId);

    static bool IsSupportedExtension(char* extn);

    TInt Reset();

private:

    CS60MetadataUtilityItf();
    void ConstructL();

    CMetaDataUtility* pMetaDataUtility; //S60MetadataUtility

    TUint uNumMetadataItems;

    };

NONSHARABLE_CLASS(CHXMetadataUtilityItf)
    {
public:

    static CHXMetadataUtilityItf* New();
    ~CHXMetadataUtilityItf();

    TInt CalculateNumMetadataItems(TUint*);

    char* GetKey(TInt index);

    TInt ValueEncoding(HXMetaDataKeys::EHXMetaDataId fldID);
    TInt GetValueSize(TInt index);
    TInt GetValue(TInt index, char* data, TInt maxLength, TInt* outSize,
            TInt* encodingType);

    TInt ParseSource(TDesC&);
    char* KeyMapping(HXMetaDataKeys::EHXMetaDataId);

    TInt Reset();

private:
    CHXMetadataUtilityItf();
    void ConstructL();

    CHXMetaDataUtility *pHXMetaDataUtility;

    TUint uNumMetadataItems;
    };

#else

extern void* mmf_metadata_utility_init(char*);
extern void mmf_metadata_utility_destroy(void* context);
extern XAresult mmf_metadata_utility_parse_source(void* , char*);
extern XAresult mmf_get_item_count(void* context, XAuint32* itemCount);
extern XAresult mmf_get_key_size(void* context, XAuint32 keyIndex, XAuint32* keySize);
extern XAresult mmf_get_key(void* context, XAuint32 index,XAuint32 keySize, XAMetadataInfo *pKey);
extern XAresult mmf_get_value_size(void* context, XAuint32 index, XAuint32 *pValueSize);
extern XAresult mmf_get_value(void* context, XAuint32 index, XAuint32 valueSize, XAMetadataInfo *pValue);
#endif //__cplusplus
#endif
