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
#include "FT_CPosTp47.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <LbsPosition.h>
#include <e32math.h>
#include <bautils.h>
#include <utf.h>
#include <xml/xmlparsererrors.h>

// CONSTANTS
_LIT(KFileName, "c:\\documents\\Tp47Data.xml");
_LIT(KFileTrace, "c:\\documents\\LandmarksTP47Trace.txt");

// See \epoc32\include\xml\XmlParserErrors.h for error codes


_LIT(KCorrectFile, "c:\\system\\test\\TestData\\CorrectFileFor_LandmarksTP47Trace.txt");

_LIT(KMimeWithNameAndTimeStampFile, "c:\\system\\test\\TestData\\Tp47Data_With_MIME_Name.xml");
_LIT(KErronousXMLFile, "c:\\system\\test\\TestData\\Tp47ErrounousXMLData.xml");

_LIT(KErronousXMLFileDataMissing1, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataMissingFields1.xml");
_LIT(KErronousXMLFileDataMissing2, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataMissingFields2.xml");
_LIT(KErronousXMLFileDataMissing3, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataMissingFields3.xml");

_LIT(KErronousXMLFileWrongOrder1, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataWrongOrder1.xml");
_LIT(KErronousXMLFileWrongOrder2, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataWrongOrder2.xml");
_LIT(KErronousXMLFileWrongOrder3, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataWrongOrder3.xml");

_LIT(KErronousXMLFileRepeatedTag, "c:\\system\\test\\TestData\\Tp47ErronousXMLDataRepeatedTag.xml");
_LIT(KErronousXMLFileUnclosedTag1, "c:\\system\\test\\TestData\\Tp47ErronousXMLDataUnclosedTag1.xml");
//_LIT(KErronousXMLFileUnclosedTag2, "c:\\system\\test\\TestData\\Tp47ErronousXMLDataUnclosedTag2.xml");

_LIT(KUnknownTags, "c:\\system\\test\\TestData\\Tp47UnknownTags.xml");
_LIT(KUnknownTrees, "c:\\system\\test\\TestData\\Tp47UnknownTrees.xml");

_LIT(KNoEndTag1, "c:\\system\\test\\TestData\\Tp47ErrounousXMLNoEndTag1.xml");
_LIT(KNoEndTag2, "c:\\system\\test\\TestData\\Tp47ErrounousXMLNoEndTag2.xml");

_LIT(KNoXMLFile1, "c:\\system\\test\\TestData\\Tp47ErronousFileNoXML1.xml");
_LIT(KNoXMLFile2, "c:\\system\\test\\TestData\\Tp47ErronousFileNoXML2.xml");

_LIT(KTestXMLInput1, "c:\\system\\test\\TestData\\Tp47XMLDataTestInput1.xml");

_LIT(KXMLFileWithLongCategoryName, "c:\\system\\test\\TestData\\Tp47XMLWithLongCategoryName.xml");
_LIT(KXMLFileWithLongLandmarkName, "c:\\system\\test\\TestData\\Tp47XMLWithLongName.xml");
_LIT(KXMLFileWithLongLatitudeField, "c:\\system\\test\\TestData\\Tp47XMLWithLongLatitudeField.xml");

_LIT(KXMLFileWithEmptyCollection, "c:\\system\\test\\TestData\\Tp47ErronousXMLFileEmptyCollection.xml");

_LIT(KXMLFileWithXMLTagAsDataField1, "c:\\system\\test\\TestData\\Tp47UsingXMLTagAsDataField1.xml");
_LIT(KXMLFileWithXMLTagAsDataField2, "c:\\system\\test\\TestData\\Tp47UsingXMLTagAsDataField2.xml");
_LIT(KXMLFileWithXMLTagAsDataField3, "c:\\system\\test\\TestData\\Tp47UsingXMLTagAsDataField3.xml");

_LIT(KErronousXMLFileTagInTag, "c:\\system\\test\\TestData\\Tp47ErronousXMLDataTagInTag1.xml");
//_LIT(KErronousXMLFileMissingLMPrefix, "c:\\system\\test\\TestData\\Tp47ErronousXMLDataMissingLMPrefix.xml");

_LIT(KXMLFileUTF8, "c:\\system\\test\\TestData\\Tp47DataUTF8.xml");
_LIT(KXMLFileUTF16, "c:\\system\\test\\TestData\\Tp47DataUTF16.xml");

_LIT(KXML_UTF_16_BE, "c:\\system\\test\\TestData\\TP47_UTF-16-bigEndian.xml");
_LIT(KXML_UTF_16_LE, "c:\\system\\test\\TestData\\TP47_UTF-16-littleEndian.xml");
_LIT(KXML_UTF_8, "c:\\system\\test\\TestData\\TP47_UTF-8.xml");
_LIT(KXML_UTF_CHINESE, "c:\\system\\test\\TestData\\TP47_UTF-Chinese.xml");

_LIT(KXMLFileWithEmptyMedialink, "c:\\system\\test\\TestData\\Tp47EmptyMediaLink.xml");
_LIT(KXMLFileWithEmptyCoordinates, "c:\\system\\test\\TestData\\Tp47EmptyCoordinates.xml");
_LIT(KXMLFileWithEmptyAdresseInfo, "c:\\system\\test\\TestData\\Tp47EmptyAdresseInfo.xml");

_LIT(KXMLFileWithErronousTextInTag1, "c:\\system\\test\\TestData\\Tp47ErronousTextInTag1.xml");
_LIT(KXMLFileWithErronousTextInTag2, "c:\\system\\test\\TestData\\Tp47ErronousTextInTag2.xml");
_LIT(KXMLFileWithErronousTextInTag3, "c:\\system\\test\\TestData\\Tp47ErronousTextInTag3.xml");
_LIT(KXMLFileWithErronousTextInTag4, "c:\\system\\test\\TestData\\Tp47ErronousTextInTag4.xml");
_LIT(KXMLFileWithErronousTextInTag5, "c:\\system\\test\\TestData\\Tp47ErronousTextInTag5.xml");
_LIT(KXMLFileWithErronousTextInTag6, "c:\\system\\test\\TestData\\Tp47ErronousTextInTag6.xml");

//KML test files
_LIT(KKMLFile, "c:\\system\\test\\TestData\\Tp47KML.kml");
_LIT(KKMLFile1, "c:\\system\\test\\TestData\\Tp47KMLfile1.kml");
_LIT(KKMLFile2, "c:\\system\\test\\TestData\\Tp47KMLfile2.kml");
_LIT(KKMLFile3, "c:\\system\\test\\TestData\\Tp47KMLfile3.kml");

_LIT(KKMLLongName, "c:\\system\\test\\TestData\\Tp47KMLlongName.kml");
_LIT(KKMLNameCaps, "c:\\system\\test\\TestData\\Tp47KMLNameCaps.kml");
_LIT(KKMLErroneousNameTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousNameTag.kml");
_LIT(KKMLErroneousNameTagPosition1, "c:\\system\\test\\TestData\\Tp47KMLErroneousNameTagPosition1.kml");
_LIT(KKMLErroneousNameTagPosition2, "c:\\system\\test\\TestData\\Tp47KMLErroneousNameTagPosition2.kml");
_LIT(KKMLEmptyName, "c:\\system\\test\\TestData\\Tp47KMLEmptyName.kml");
_LIT(KKMLMissingNameTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingNameTag1.kml");
_LIT(KKMLMissingNameTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingNameTag2.kml");
_LIT(KKMLNameWithSpecialCharacters1, "c:\\system\\test\\TestData\\Tp47KMLNameWithSpecialCharacters1.kml");
_LIT(KKMLNameWithSpecialCharacters2, "c:\\system\\test\\TestData\\Tp47KMLNameWithSpecialCharacters2.kml");

_LIT(KKMLEmptyPhoneNumber, "c:\\system\\test\\TestData\\Tp47KMLEmptyPhoneNumber.kml");
_LIT(KKMLErroneousPhoneNumberTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousPhoneNumberTag.kml");
_LIT(KKMLErroneousPhoneNumberTagPosition, "c:\\system\\test\\TestData\\Tp47KMLErroneousPhoneNumberTagPosition.kml");
_LIT(KKMLLongPhoneNumer, "c:\\system\\test\\TestData\\Tp47KMLLongPhoneNumer.kml");
_LIT(KKMLMissingPhoneTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingPhoneTag1.kml");
_LIT(KKMLMissingPhoneTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingPhoneTag2.kml");
_LIT(KKMLPhoneNumberCaps, "c:\\system\\test\\TestData\\Tp47KMLPhoneNumberCaps.kml");
_LIT(KKMLPhoneNumberWithText, "c:\\system\\test\\TestData\\Tp47KMLPhoneNumberWithText.kml");
_LIT(KKMLPhoneNumberWithSpecialCharacters1, "c:\\system\\test\\TestData\\Tp47KMLPhoneNumberWithSpecialCharacters1.kml");
_LIT(KKMLPhoneNumberWithSpecialCharacters2, "c:\\system\\test\\TestData\\Tp47KMLPhoneNumberWithSpecialCharacters2.kml");

_LIT(KKMLEmptyPoint, "c:\\system\\test\\TestData\\Tp47KMLEmptyPoint.kml");
_LIT(KKMLErroneousPointTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousPointTag.kml");
_LIT(KKMLErroneousPointTagPosition, "c:\\system\\test\\TestData\\Tp47KMLErroneousPointTagPosition.kml");
_LIT(KKMLMissingPointTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingPointTag1.kml");
_LIT(KKMLMissingPointTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingPointTag2.kml");
_LIT(KKMLPointCaps, "c:\\system\\test\\TestData\\Tp47KMLPointCaps.kml");
_LIT(KKMLPointTag1, "c:\\system\\test\\TestData\\Tp47KMLPointTag1.kml");
_LIT(KKMLPointTag2, "c:\\system\\test\\TestData\\Tp47KMLPointTag2.kml");
_LIT(KKMLPointTag3, "c:\\system\\test\\TestData\\Tp47KMLPointTag3.kml");

_LIT(KKMLEmptyDescription, "c:\\system\\test\\TestData\\Tp47KMLEmptyDescription.kml");
_LIT(KKMLDescriptionWithSpecialCharacters1, "c:\\system\\test\\TestData\\Tp47KMLDescriptionWithSpecialCharacters1.kml");
_LIT(KKMLDescriptionWithSpecialCharacters2, "c:\\system\\test\\TestData\\Tp47KMLDescriptionWithSpecialCharacters2.kml");
_LIT(KKMLDescriptionCaps, "c:\\system\\test\\TestData\\Tp47KMLDescriptionCaps.kml");
_LIT(KKMLLongDescription, "c:\\system\\test\\TestData\\Tp47KMLLongDescription.kml");
_LIT(KKMLErroneousDescriptionTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousDescriptionTag.kml");
_LIT(KKMLErroneousDescriptionTagposition, "c:\\system\\test\\TestData\\Tp47KMLErroneousDescriptionTagposition.kml");
_LIT(KKMLMissingDescriptionTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingDescriptionTag1.kml");
_LIT(KKMLMissingDescriptionTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingDescriptionTag2.kml");
_LIT(KKMLDescriptionWithCDATA, "c:\\system\\test\\TestData\\Tp47KMLDescriptionWithCDATA.kml");

_LIT(KKMLErroneousFolderTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousFolderTag.kml");
_LIT(KKMLFolderCaps, "c:\\system\\test\\TestData\\Tp47KMLFolderCaps.kml");
_LIT(KKMLMissingFolderTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingFolderTag1.kml");
_LIT(KKMLMissingFolderTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingFolderTag2.kml");
_LIT(KKMLEmptyFolderName, "c:\\system\\test\\TestData\\Tp47KMLEmptyFolderName.kml");
_LIT(KKMLLongFolderName, "c:\\system\\test\\TestData\\Tp47KMLLongFolderName.kml");
_LIT(KKMLFolderNameWithSpecialCharacters1, "c:\\system\\test\\TestData\\Tp47KMLFolderNameWithSpecialCharacters1.kml");
_LIT(KKMLFolderNameWithSpecialCharacters2, "c:\\system\\test\\TestData\\Tp47KMLFolderNameWithSpecialCharacters2.kml");

_LIT(KKMLDocumentNameWithSpecialCharacters1, "c:\\system\\test\\TestData\\Tp47KMLDocumentNameWithSpecialCharacters1.kml");
_LIT(KKMLDocumentNameWithSpecialCharacters2, "c:\\system\\test\\TestData\\Tp47KMLDocumentNameWithSpecialCharacters2.kml");
_LIT(KKMLEmptyDocumentName, "c:\\system\\test\\TestData\\Tp47KMLEmptyDocumentName.kml");
_LIT(KKMLLongDocumentName, "c:\\system\\test\\TestData\\Tp47KMLLongDocumentName.kml");
_LIT(KKMLDocumentCaps, "c:\\system\\test\\TestData\\Tp47KMLDocumentCaps.kml");
_LIT(KKMLErroneousDocumentTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousDocumentTag.kml");
_LIT(KKMLMissingDocumentTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingDocumentTag1.kml");
_LIT(KKMLMissingDocumentTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingDocumentTag2.kml");

_LIT(KKMLAddressCaps, "c:\\system\\test\\TestData\\Tp47KMLAddressCaps.kml");
_LIT(KKMLAddressWithSpecialCharacters1, "c:\\system\\test\\TestData\\Tp47KMLAddressWithSpecialCharacters1.kml");
_LIT(KKMLAddressWithSpecialCharacters2, "c:\\system\\test\\TestData\\Tp47KMLAddressWithSpecialCharacters2.kml");
_LIT(KKMLEmptyAddress1, "c:\\system\\test\\TestData\\Tp47KMLEmptyAddress1.kml");
_LIT(KKMLEmptyAddress2, "c:\\system\\test\\TestData\\Tp47KMLEmptyAddress2.kml");
_LIT(KKMLLongAddress1, "c:\\system\\test\\TestData\\Tp47KMLLongAddress1.kml");
_LIT(KKMLLongAddress2, "c:\\system\\test\\TestData\\Tp47KMLLongAddress2.kml");
_LIT(KKMLLongAddress3, "c:\\system\\test\\TestData\\Tp47KMLLongAddress3.kml");
_LIT(KKMLErroneousAddressTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousAddressTag.kml");
_LIT(KKMLErroneousAddressTagPosition, "c:\\system\\test\\TestData\\Tp47KMLErroneousAddressTagPosition.kml");
_LIT(KKMLMissingAddressTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingAddressTag1.kml");
_LIT(KKMLMissingAddressTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingAddressTag2.kml");

_LIT(KKMLCoordinateCaps, "c:\\system\\test\\TestData\\Tp47KMLCoordinateCaps.kml");
_LIT(KKMLMoreCoordinates, "c:\\system\\test\\TestData\\Tp47KMLMoreCoordinates.kml");
_LIT(KKMLErroneousCoordinateTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousCoordinateTag.kml");
_LIT(KKMLErroneousCoordinateTagPosition, "c:\\system\\test\\TestData\\Tp47KMLErroneousCoordinateTagPosition.kml");
_LIT(KKMLCoordinates1, "c:\\system\\test\\TestData\\Tp47KMLCoordinates1.kml");
_LIT(KKMLCoordinates2, "c:\\system\\test\\TestData\\Tp47KMLCoordinates2.kml");
_LIT(KKMLCoordinates3, "c:\\system\\test\\TestData\\Tp47KMLCoordinates3.kml");
_LIT(KKMLCoordinates4, "c:\\system\\test\\TestData\\Tp47KMLCoordinates4.kml");
_LIT(KKMLCoordinates5, "c:\\system\\test\\TestData\\Tp47KMLCoordinates5.kml");
_LIT(KKMLCoordinates6, "c:\\system\\test\\TestData\\Tp47KMLCoordinates6.kml");
_LIT(KKMLLongCoordinates1, "c:\\system\\test\\TestData\\Tp47KMLLongCoordinates1.kml");
_LIT(KKMLLongCoordinates2, "c:\\system\\test\\TestData\\Tp47KMLLongCoordinates2.kml");
_LIT(KKMLMissingCoordinateTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingCoordinateTag1.kml");
_LIT(KKMLMissingCoordinateTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingCoordinateTag2.kml");
_LIT(KKMLEmptyCoordinates1, "c:\\system\\test\\TestData\\Tp47KMLEmptyCoordinates1.kml");
_LIT(KKMLEmptyCoordinates2, "c:\\system\\test\\TestData\\Tp47KMLEmptyCoordinates2.kml");
_LIT(KKMLEmptyCoordinates3, "c:\\system\\test\\TestData\\Tp47KMLEmptyCoordinates3.kml");
_LIT(KKMLEmptyCoordinates4, "c:\\system\\test\\TestData\\Tp47KMLEmptyCoordinates4.kml");
_LIT(KKMLEmptyCoordinates5, "c:\\system\\test\\TestData\\Tp47KMLEmptyCoordinates5.kml");
_LIT(KKMLEmptyCoordinates6, "c:\\system\\test\\TestData\\Tp47KMLEmptyCoordinates6.kml");
_LIT(KKMLEmptyCoordinates7, "c:\\system\\test\\TestData\\Tp47KMLEmptyCoordinates7.kml");
_LIT(KKMLErroneousCoordinates1, "c:\\system\\test\\TestData\\Tp47KMLErroneousCoordinates1.kml");
_LIT(KKMLErroneousCoordinates2, "c:\\system\\test\\TestData\\Tp47KMLErroneousCoordinates2.kml");
_LIT(KKMLErroneousCoordinates3, "c:\\system\\test\\TestData\\Tp47KMLErroneousCoordinates3.kml");
_LIT(KKMLErroneousCoordinates4, "c:\\system\\test\\TestData\\Tp47KMLErroneousCoordinates4.kml");
_LIT(KKMLErroneousCoordinates5, "c:\\system\\test\\TestData\\Tp47KMLErroneousCoordinates5.kml");
_LIT(KKMLNegativeCoordinates, "c:\\system\\test\\TestData\\Tp47KMLNegativeCoordinates.kml");

_LIT(KKMLErroneousTextWithinPlacemark, "c:\\system\\test\\TestData\\Tp47KMLErroneousTextWithinPlacemark.kml");
_LIT(KKMLEmptyfile1, "c:\\system\\test\\TestData\\Tp47KMLEmptyfile1.kml");
_LIT(KKMLEmptyfile2, "c:\\system\\test\\TestData\\Tp47KMLEmptyfile2.kml");
_LIT(KKMLInvalidfile, "c:\\system\\test\\TestData\\Tp47KMLInvalidfile.kml");

_LIT(KKMLPlacemarkCaps, "c:\\system\\test\\TestData\\Tp47KMLPlacemarkCaps.kml");
_LIT(KKMLErroneousPlacemarkTag, "c:\\system\\test\\TestData\\Tp47KMLErroneousPlacemarkTag.kml");
_LIT(KKMLMissingPlacemarkTag1, "c:\\system\\test\\TestData\\Tp47KMLMissingPlacemarkTag1.kml");
_LIT(KKMLMissingPlacemarkTag2, "c:\\system\\test\\TestData\\Tp47KMLMissingPlacemarkTag2.kml");
_LIT(KKMLEmptyPlacemarkTag, "c:\\system\\test\\TestData\\Tp47KMLEmptyPlacemarkTag.kml");

_LIT(KKMLGandhiMuseum, "c:\\system\\test\\TestData\\Tp47KMLGandhi Museum.kml");
_LIT(KKMLIndusHeritageCentreIndia, "c:\\system\\test\\TestData\\Tp47KMLIndus Heritage Centre, India.kml");
_LIT(KKMLHiltonHotels, "c:\\system\\test\\TestData\\Tp47KMLhilton hotels near new york (1 - 10).kml");

_LIT(KKMLFile4, "c:\\system\\test\\TestData\\Tp47KMLfile4.kml");
_LIT(KKMLFile5, "c:\\system\\test\\TestData\\Tp47KMLfile5.kml");
_LIT(KKMLFile6, "c:\\system\\test\\TestData\\Tp47KMLfile6.kml");
_LIT(KKMLFile7, "c:\\system\\test\\TestData\\Tp47KMLfile7.kml");
_LIT(KKMLFile8, "c:\\system\\test\\TestData\\Tp47KMLfile8.kml");
_LIT(KKMLFile9, "c:\\system\\test\\TestData\\Tp47KMLfile9.kml");







// Maxmimum size for each line in trace file
const TInt KBufSize = 5000;

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp47::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::CloseTest()
    {
    iLog->Log(_L("CloseTest"));

    delete iEncoderBuffer;
    iEncoderBuffer = NULL;

    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();

    delete iDatabase;
    iDatabase=NULL;

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iOperation;
    iOperation = NULL;

    TRAPD(err, RemoveGlobalCategoriesL());
    if (err != KErrNone && err != KErrNotFound)
        {
        HBufC* buf = HBufC::NewLC( 128 );
        TPtr buffer = buf->Des();
            
        buffer.Format(_L("RemoveGlobalCategoriesL leaved with %d"), err);
        iLog->Log( buffer );
        
        CleanupStack::PopAndDestroy( buf );
        }

    iLog->Log(_L("CloseTest Done"));
    }

// ---------------------------------------------------------
// CPosTp47::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::StartL()
    {
    BaflUtils::EnsurePathExistsL( iFileSession, KFileName );

    // Use same lm db as in composite search test
    iDatabase = UseCompositeLandmarksDbFileL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    // Delete the trace file
    DeleteFileL(KFileTrace);

    delete iDatabase;
    iDatabase = NULL;

    iDatabase = CPosLandmarkDatabase::OpenL();
    	
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    // Do reset so that global categories are included
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    ExecuteAndDeleteLD(categoryManager->ResetGlobalCategoriesL());
    CleanupStack::PopAndDestroy(categoryManager);

    HBufC8* emptyBuffer=NULL;

    CreateXMLData(ETrue);

    PrintParsedDataFromEncoderL(KFileName, *emptyBuffer, KErrNone, KLmxMimeType );

    CheckXMLData(ETrue);

    // Use buffer instead of file
    CreateXMLData(EFalse);
    CheckXMLData(EFalse);

    iLandmarks.ResetAndDestroy();


	
    iLog->Log(_L("Mime With Name and Stamp"));
    PrintParsedDataFromEncoderL(KMimeWithNameAndTimeStampFile, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KMimeWithNameAndTimeStampFile, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Several Landmark File"));
    // XXX PrintParsedDataFromEncoderL(KSeveralLandmarkFile, *emptyBuffer, KErrPosLmUnknownFormat);

    iLog->Log(_L("Erronous XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFile, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFile, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Several LandmarkCollection File"));
    // XXX PrintParsedDataFromEncoderL(KSeveralLandmarkCollectionFile, *emptyBuffer, KErrPosLmUnknownFormat);

    iLog->Log(_L("1) Wrong Order in XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFileWrongOrder1, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileWrongOrder1, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("2) Wrong Order in XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFileWrongOrder2, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileWrongOrder2, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("3) Wrong Order in XML File"));
    // AdresseInfo can be sorted randomly without any error
    PrintParsedDataFromEncoderL(KErronousXMLFileWrongOrder3, *emptyBuffer, KErrNone, KLmxMimeType );
    // N/A for GPX

    iLog->Log(_L("1) Data missing in XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFileDataMissing1, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileDataMissing1, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("2) Data missing in XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFileDataMissing2, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileDataMissing2, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("3) Data missing in XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFileDataMissing3, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileDataMissing3, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Unknown tags in XML File"));
    PrintParsedDataFromEncoderL(KUnknownTags, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KUnknownTags, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Unknown trees in XML File"));
    PrintParsedDataFromEncoderL(KUnknownTrees, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KUnknownTrees, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("No End landmarkCollection tag in XML File"));
    PrintParsedDataFromEncoderL(KNoEndTag1, *emptyBuffer, -993, KLmxMimeType );
    PrintParsedDataFromEncoderL(KNoEndTag1, *emptyBuffer, -993, KGpxMimeType );

    iLog->Log(_L("No End landmark tag in XML File"));
    PrintParsedDataFromEncoderL(KNoEndTag2, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KNoEndTag2, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with different input formats in XML File"));
    PrintParsedDataFromEncoderL(KTestXMLInput1, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KTestXMLInput1, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with repeated tag XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFileRepeatedTag, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileRepeatedTag, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with unclosed tag XML File"));
    PrintParsedDataFromEncoderL(KErronousXMLFileUnclosedTag1, *emptyBuffer, -993, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileUnclosedTag1, *emptyBuffer, -993, KGpxMimeType );

//    Incorrect test
//    iLog->Log(_L("Test with unclosed tag XML File"));
//    PrintParsedDataFromEncoderL(KErronousXMLFileUnclosedTag2, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
//    PrintParsedDataFromEncoderL(KErronousXMLFileUnclosedTag2, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with a non XML File 1"));
    PrintParsedDataFromEncoderL(KNoXMLFile1, *emptyBuffer, -982, KLmxMimeType );
    PrintParsedDataFromEncoderL(KNoXMLFile1, *emptyBuffer, -982, KGpxMimeType );

    iLog->Log(_L("Test with a non XML File 2"));
    PrintParsedDataFromEncoderL(KNoXMLFile2, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType ); //temporarily changed KErrPosLmUnknownFormat to KErrNone
    PrintParsedDataFromEncoderL(KNoXMLFile2, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType ); //temporarily KErrPosLmUnknownFormat to KErrNone

    iLog->Log(_L("Test with a XML UTFW8 File"));
    PrintParsedDataFromEncoderL(KXMLFileUTF8, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileUTF8, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with a XML UTFW16 File"));
    PrintParsedDataFromEncoderL(KXMLFileUTF16, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileUTF16, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with a XML File with long category names"));
    PrintParsedDataFromEncoderL(KXMLFileWithLongCategoryName, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithLongCategoryName, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with a XML File with long landmark name"));
    PrintParsedDataFromEncoderL(KXMLFileWithLongLandmarkName, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithLongLandmarkName, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with a XML File with long latitude field"));
    PrintParsedDataFromEncoderL(KXMLFileWithLongLatitudeField, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithLongLatitudeField, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with a XML File with empty collection"));
    PrintParsedDataFromEncoderL(KXMLFileWithEmptyCollection, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    // N/A for GPX

    iLog->Log(_L("Test with a XML File with Landmark Tag in Landmark Tag"));
    PrintParsedDataFromEncoderL(KErronousXMLFileTagInTag, *emptyBuffer, EXmlRecursiveEntity, KLmxMimeType );
    PrintParsedDataFromEncoderL(KErronousXMLFileTagInTag, *emptyBuffer, EXmlRecursiveEntity, KGpxMimeType );

    iLog->Log(_L("Test with a XML File with XML Tag as data field 1"));
    PrintParsedDataFromEncoderL(KXMLFileWithXMLTagAsDataField1, *emptyBuffer, -993, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithXMLTagAsDataField1, *emptyBuffer, -993, KGpxMimeType );

    iLog->Log(_L("Test with a XML File with XML Tag as data field 2"));
    PrintParsedDataFromEncoderL(KXMLFileWithXMLTagAsDataField2, *emptyBuffer, -993, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithXMLTagAsDataField2, *emptyBuffer, -993, KGpxMimeType );

    iLog->Log(_L("Test with a XML File with XML Tag as data field 3"));
    PrintParsedDataFromEncoderL(KXMLFileWithXMLTagAsDataField3, *emptyBuffer, -993, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithXMLTagAsDataField3, *emptyBuffer, -993, KGpxMimeType );

//    Incorrect test
//    iLog->Log(_L("Test with a XML File with missing LM prefix"));
//    PrintParsedDataFromEncoderL(KErronousXMLFileMissingLMPrefix, *emptyBuffer, EXmlRecursiveEntity, KLmxMimeType );
//    PrintParsedDataFromEncoderL(KErronousXMLFileMissingLMPrefix, *emptyBuffer, EXmlRecursiveEntity, KGpxMimeType );
//
    iLog->Log(_L("Test with UTF-16-bigEndian.xml"));
    PrintParsedDataFromEncoderL(KXML_UTF_16_BE, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXML_UTF_16_BE, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with UTF-16-littleEndian.xml"));
    PrintParsedDataFromEncoderL(KXML_UTF_16_LE, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXML_UTF_16_LE, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with UTF-8.xml"));
    PrintParsedDataFromEncoderL(KXML_UTF_8, *emptyBuffer, KErrNone, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXML_UTF_8, *emptyBuffer, KErrNone, KGpxMimeType );

    iLog->Log(_L("Test with UTF-Chinese.xml"));
    PrintParsedDataFromEncoderL(KXML_UTF_CHINESE, *emptyBuffer, -982, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXML_UTF_CHINESE, *emptyBuffer, -982, KGpxMimeType );
    
    
    
    //testing kml files
    iLog->Log(_L("Test with Tp47KML.kml"));
	PrintParsedDataFromEncoderL(KKMLFile, *emptyBuffer, KErrNone, KKmlMimeType );
	
	iLog->Log(_L("Test with Tp47KMLfile1.kml"));
	PrintParsedDataFromEncoderL(KKMLFile1, *emptyBuffer, KErrNone, KKmlMimeType );
	
	iLog->Log(_L("Test with Tp47KMLfile2.kml"));
	PrintParsedDataFromEncoderL(KKMLFile2, *emptyBuffer, KErrNone, KKmlMimeType );
	
	iLog->Log(_L("Test with Tp47KMLfile3.kml"));
	PrintParsedDataFromEncoderL(KKMLFile3, *emptyBuffer, KErrNone, KKmlMimeType );
	
	//testing the name tag related cases
	iLog->Log(_L("Test with Tp47KMLlongName.kml"));
	PrintParsedDataFromEncoderL(KKMLLongName, *emptyBuffer, KErrNone, KKmlMimeType );
	
	iLog->Log(_L("Test with Tp47KMLNameCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLNameCaps, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousNameTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousNameTag, *emptyBuffer, -993, KKmlMimeType );			//EXmlTagMismatch = -993

	iLog->Log(_L("Test with Tp47KMLErroneousNameTagPosition1.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousNameTagPosition1, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousNameTagPosition2.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousNameTagPosition2, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyName.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyName, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingNameTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingNameTag1, *emptyBuffer, -993, KKmlMimeType );			//EXmlTagMismatch = -993

	iLog->Log(_L("Test with Tp47KMLMissingNameTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingNameTag2, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLNameWithSpecialCharacters1.kml"));
	PrintParsedDataFromEncoderL(KKMLNameWithSpecialCharacters1, *emptyBuffer, -996, KKmlMimeType ); // EXmlInvalidToken = -996

	iLog->Log(_L("Test with Tp47KMLNameWithSpecialCharacters2.kml"));
	PrintParsedDataFromEncoderL(KKMLNameWithSpecialCharacters2, *emptyBuffer, KErrNone, KKmlMimeType );
	
	//testing the PhoneNumber tag related cases
	iLog->Log(_L("Test with Tp47KMLEmptyPhoneNumber.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyPhoneNumber, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousPhoneNumberTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousPhoneNumberTag, *emptyBuffer, -993, KKmlMimeType ); 

	iLog->Log(_L("Test with Tp47KMLErroneousPhoneNumberTagPosition.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousPhoneNumberTagPosition, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongPhoneNumer.kml"));
	PrintParsedDataFromEncoderL(KKMLLongPhoneNumer, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingPhoneTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingPhoneTag1, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingPhoneTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingPhoneTag2, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPhoneNumberCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLPhoneNumberCaps, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPhoneNumberWithText.kml"));
	PrintParsedDataFromEncoderL(KKMLPhoneNumberWithText, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPhoneNumberWithSpecialCharacters1.kml"));
	PrintParsedDataFromEncoderL(KKMLPhoneNumberWithSpecialCharacters1, *emptyBuffer, -996, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPhoneNumberWithSpecialCharacters2.kml"));
	PrintParsedDataFromEncoderL(KKMLPhoneNumberWithSpecialCharacters2, *emptyBuffer, KErrNone, KKmlMimeType );
	
	//testing the point tag related test cases
	iLog->Log(_L("Test with Tp47KMLEmptyPoint.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyPoint, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousPointTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousPointTag, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousPointTagPosition.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousPointTagPosition, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingPointTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingPointTag1, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingPointTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingPointTag2, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPointCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLPointCaps, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPointTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLPointTag1, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPointTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLPointTag2, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLPointTag3.kml"));
	PrintParsedDataFromEncoderL(KKMLPointTag3, *emptyBuffer, KErrNone, KKmlMimeType );

	
	//testing the description tag related cases
	iLog->Log(_L("Test with Tp47KMLEmptyDescription.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyDescription, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLDescriptionWithSpecialCharacters1.kml"));
	PrintParsedDataFromEncoderL(KKMLDescriptionWithSpecialCharacters1, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLDescriptionWithSpecialCharacters2.kml"));
	PrintParsedDataFromEncoderL(KKMLDescriptionWithSpecialCharacters2, *emptyBuffer, -996, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLDescriptionCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLDescriptionCaps, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongDescription.kml"));
	PrintParsedDataFromEncoderL(KKMLLongDescription, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousDescriptionTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousDescriptionTag, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousDescriptionTagposition.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousDescriptionTagposition, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingDescriptionTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingDescriptionTag1, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingDescriptionTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingDescriptionTag2, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLDescriptionWithCDATA.kml"));
	PrintParsedDataFromEncoderL(KKMLDescriptionWithCDATA, *emptyBuffer, KErrNone, KKmlMimeType );
	
	//testing the folder tag related cases
	iLog->Log(_L("Test with Tp47KMLErroneousFolderTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousFolderTag, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFolderCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLFolderCaps, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingFolderTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingFolderTag1, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingFolderTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingFolderTag2, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyFolderName.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyFolderName, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongFolderName.kml"));
	PrintParsedDataFromEncoderL(KKMLLongFolderName, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFolderNameWithSpecialCharacters1.kml"));
	PrintParsedDataFromEncoderL(KKMLFolderNameWithSpecialCharacters1, *emptyBuffer, -996, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFolderNameWithSpecialCharacters2.kml"));
	PrintParsedDataFromEncoderL(KKMLFolderNameWithSpecialCharacters2, *emptyBuffer, KErrNone, KKmlMimeType );


	//testing the document tag related cases
	iLog->Log(_L("Test with Tp47KMLDocumentNameWithSpecialCharacters1.kml"));
	PrintParsedDataFromEncoderL(KKMLDocumentNameWithSpecialCharacters1, *emptyBuffer, -996, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLDocumentNameWithSpecialCharacters2.kml"));
	PrintParsedDataFromEncoderL(KKMLDocumentNameWithSpecialCharacters2, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyDocumentName.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyDocumentName, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongDocumentName.kml"));
	PrintParsedDataFromEncoderL(KKMLLongDocumentName, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLDocumentCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLDocumentCaps, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousDocumentTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousDocumentTag, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingDocumentTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingDocumentTag1, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingDocumentTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingDocumentTag2, *emptyBuffer, -993, KKmlMimeType );
	
	
	//testing the address tag related cases
	iLog->Log(_L("Test with Tp47KMLAddressCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLAddressCaps, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLAddressWithSpecialCharacters1.kml"));
	PrintParsedDataFromEncoderL(KKMLAddressWithSpecialCharacters1, *emptyBuffer, -996, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLAddressWithSpecialCharacters2.kml"));
	PrintParsedDataFromEncoderL(KKMLAddressWithSpecialCharacters2, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyAddress1.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyAddress1, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyAddress2.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyAddress2, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongAddress1.kml"));
	PrintParsedDataFromEncoderL(KKMLLongAddress1, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongAddress2.kml"));
	PrintParsedDataFromEncoderL(KKMLLongAddress2, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongAddress3.kml"));
	PrintParsedDataFromEncoderL(KKMLLongAddress3, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousAddressTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousAddressTag, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousAddressTagPosition.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousAddressTagPosition, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingAddressTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingAddressTag1, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingAddressTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingAddressTag2, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	//testing the coordinate tag related cases
	iLog->Log(_L("Test with Tp47KMLCoordinateCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLCoordinateCaps, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMoreCoordinates.kml"));
	PrintParsedDataFromEncoderL(KKMLMoreCoordinates, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousCoordinateTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousCoordinateTag, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousCoordinateTagPosition.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousCoordinateTagPosition, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLCoordinates1.kml"));
	PrintParsedDataFromEncoderL(KKMLCoordinates1, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLCoordinates2.kml"));
	PrintParsedDataFromEncoderL(KKMLCoordinates2, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLCoordinates3.kml"));
	PrintParsedDataFromEncoderL(KKMLCoordinates3, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLCoordinates4.kml"));
	PrintParsedDataFromEncoderL(KKMLCoordinates4, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLCoordinates5.kml"));
	PrintParsedDataFromEncoderL(KKMLCoordinates5, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLCoordinates6.kml"));
	PrintParsedDataFromEncoderL(KKMLCoordinates6, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongCoordinates1.kml"));
	PrintParsedDataFromEncoderL(KKMLLongCoordinates1, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLLongCoordinates2.kml"));
	PrintParsedDataFromEncoderL(KKMLLongCoordinates2, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingCoordinateTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingCoordinateTag1, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingCoordinateTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingCoordinateTag2, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyCoordinates1.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyCoordinates1, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyCoordinates2.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyCoordinates2, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyCoordinates3.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyCoordinates3, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyCoordinates4.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyCoordinates4, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyCoordinates5.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyCoordinates5, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyCoordinates6.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyCoordinates6, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyCoordinates7.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyCoordinates7, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousCoordinates1.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousCoordinates1, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousCoordinates2.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousCoordinates2, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousCoordinates3.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousCoordinates3, *emptyBuffer, KErrOverflow, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousCoordinates4.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousCoordinates4, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousCoordinates5.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousCoordinates5, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLNegativeCoordinates.kml"));
	PrintParsedDataFromEncoderL(KKMLNegativeCoordinates, *emptyBuffer, KErrNone, KKmlMimeType );
	
	//testing some invalid cases
	iLog->Log(_L("Test with Tp47KMLErroneousTextWithinPlacemark.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousTextWithinPlacemark, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyfile1.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyfile1, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyfile2.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyfile2, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLInvalidfile.kml"));
	PrintParsedDataFromEncoderL(KKMLInvalidfile, *emptyBuffer, -983, KKmlMimeType ); //EXmlMisplacedPi =  -983
	
	//testing the Placemark tag related cases
	iLog->Log(_L("Test with Tp47KMLPlacemarkCaps.kml"));
	PrintParsedDataFromEncoderL(KKMLPlacemarkCaps, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLErroneousPlacemarkTag.kml"));
	PrintParsedDataFromEncoderL(KKMLErroneousPlacemarkTag, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingPlacemarkTag1.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingPlacemarkTag1, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLMissingPlacemarkTag2.kml"));
	PrintParsedDataFromEncoderL(KKMLMissingPlacemarkTag2, *emptyBuffer, -993, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLEmptyPlacemarkTag.kml"));
	PrintParsedDataFromEncoderL(KKMLEmptyPlacemarkTag, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );
	
	//testing with kml files downloaded from google earth
	iLog->Log(_L("Test with Tp47KMLGandhi Museum.kml"));
	PrintParsedDataFromEncoderL(KKMLGandhiMuseum, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLIndus Heritage Centre, India.kml"));
	PrintParsedDataFromEncoderL(KKMLIndusHeritageCentreIndia, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLhilton hotels near new york (1 - 10).kml"));
	PrintParsedDataFromEncoderL(KKMLHiltonHotels, *emptyBuffer, KErrNone, KKmlMimeType );
	
	iLog->Log(_L("Test with Tp47KMLFile4.kml"));
	PrintParsedDataFromEncoderL(KKMLFile4, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFile5.kml"));
	PrintParsedDataFromEncoderL(KKMLFile5, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFile6.kml"));
	PrintParsedDataFromEncoderL(KKMLFile6, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFile7.kml"));
	PrintParsedDataFromEncoderL(KKMLFile7, *emptyBuffer, KErrNone, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFile8.kml"));
	PrintParsedDataFromEncoderL(KKMLFile8, *emptyBuffer, KErrPosLmUnknownFormat, KKmlMimeType );

	iLog->Log(_L("Test with Tp47KMLFile9.kml"));
	PrintParsedDataFromEncoderL(KKMLFile9, *emptyBuffer, -993, KKmlMimeType );

    // Now compare tha trace file against the correct one
    RFile correctFile;
    RFile file;

    iLog->Log(_L(">>>>>>>Comparing files<<<<<"));
    
    TFileName filename;
    CreateCorrectXmlFileL( KCorrectFile, filename );
    
    User::LeaveIfError(correctFile.Open(iFileSession, filename,  EFileRead));
    CleanupClosePushL(correctFile);
    User::LeaveIfError(file.Open(iFileSession, KFileTrace,  EFileRead));
    CleanupClosePushL(file);

    HBufC* correctLinebuffer = HBufC::NewLC( KBufSize );
            TPtr correctLine = correctLinebuffer->Des();    
            HBufC* linebuffer = HBufC::NewLC( KBufSize );
            TPtr line = linebuffer->Des();    
   	
    TInt err = KErrNone;
    TBool done = EFalse;
    TInt lineNr = 0;

    while (ReadLn(correctFile, correctLine) != KErrEof && !done)
        {
        lineNr++;
        err = ReadLn(file, line);
        if ( err == KErrEof ) 
            {
            done = ETrue;    
            }
        correctLine.Trim();
        line.Trim();
        if (correctLine.Compare(line) != KErrNone)
            {
            HBufC* buf = HBufC::NewLC( 128 );
            TPtr buffer = buf->Des();
            
            buffer.Format(_L("ERROR in TraceFile on line %d: "), lineNr);
            
            iLog->Log( buffer );
            iLog->Log( correctLine );
            iLog->Log( line );
            iErrorsFound++;
            CleanupStack::PopAndDestroy( buf );
            }
        }
		CleanupStack::PopAndDestroy(linebuffer);
    CleanupStack::PopAndDestroy(correctLinebuffer);
    CleanupStack::PopAndDestroy(&file);
    CleanupStack::PopAndDestroy(&correctFile);

    // EOF for trace file was reach before EOF in the correct file
    if (done)
        {
        iLog->Log(_L("ERROR: EOF for Trace file was reached to early"));
        iErrorsFound++;
        }

    iLog->Log(_L("Test with Empty MediaLink"));
    PrintParsedDataFromEncoderL(KXMLFileWithEmptyMedialink, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithEmptyMedialink, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with Empty Coordinates"));
    PrintParsedDataFromEncoderL(KXMLFileWithEmptyCoordinates, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithEmptyCoordinates, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with Empty AdresseInfo"));
    PrintParsedDataFromEncoderL(KXMLFileWithEmptyAdresseInfo, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    // N/A for GPX

    iLog->Log(_L("Test with Erronous Text in Tag1"));
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag1, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag1, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with Erronous Text in Tag2"));
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag2, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag2, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with Erronous Text in Tag3"));
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag3, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag3, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with Erronous Text in Tag4"));
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag4, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag4, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with Erronous Text in Tag5"));
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag5, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag5, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    iLog->Log(_L("Test with Erronous Text in Tag6"));
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag6, *emptyBuffer, KErrPosLmUnknownFormat, KLmxMimeType );
    PrintParsedDataFromEncoderL(KXMLFileWithErronousTextInTag6, *emptyBuffer, KErrPosLmUnknownFormat, KGpxMimeType );

    CheckIndexNotSupportedL();

    DoCancelTestL();

    if (iErrorsFound != KErrNone)// LogErrorAndLeave(_L("Errors found in TP47"));
        {
    	iLog->Log(_L("Errors found in TP47"));
    	User::Leave(-1);
        }
	}

// ---------------------------------------------------------
// CPosTp47::CheckIndexNotSupportedL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::CheckIndexNotSupportedL()
    {
    iLog->Log(_L("CheckIndexNotSupportedL"));

    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    iLandmarkParser->SetInputFileL(KFileName);

    // This function should leave with KErrNotSupported
    TRAPD(err, iOperation = iLandmarkParser->ParseContentL(ETrue));

    delete iOperation;
    iOperation = NULL;

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    if (err != KErrNotSupported)
    {
    	iLog->Log(_L("ParseContentL(ETrue) did not leave with KErrNotSupported"));
    	User::Leave(err);
    }

    iLandmarkParser = CPosLandmarkParser::NewL(KGpxMimeType);
    iLandmarkParser->SetInputFileL(KFileName);

    // This function should leave with KErrNotSupported
    TRAP(err, iOperation = iLandmarkParser->ParseContentL(ETrue));

    delete iOperation;
    iOperation = NULL;

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    if (err != KErrNotSupported)
    {
        iLog->Log(_L("ParseContentL(ETrue) (GPX) did not leave with KErrNotSupported"));
        User::Leave(err);
    }

    iLog->Log(_L("CheckIndexNotSupportedL Done"));
    }

// ---------------------------------------------------------
// CPosTp47::DoCancelTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::DoCancelTestL()
    {
    iLog->Log(_L("DoCancelTestL"));
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    iLandmarkParser->SetInputFileL(KFileName);

    iOperation = iLandmarkParser->ParseContentL();

    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;

    // 1 ) Cancel parse function by deleting iOperation
    delete iOperation;
    iOperation = NULL;

    TUint32 nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != 0)
    {
    	iLog->Log(_L("No landmarks should be found"));
    	User::Leave(KErrNone);
    }

    // No landmark should be found
    TRAPD(err, iLandmarkParser->LandmarkLC());
    if (err != KErrNotFound)
        {
        iLog->Log(_L("ERROR: No Landmark should be found"));
        iErrorsFound++;
        }

    // 1 ) Cancel parse function by changing input
    iOperation = iLandmarkParser->ParseContentL();

    // Get the first landmark
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != 1)
    {
    	iLog->Log(_L("One landmark should be found"));
    	User::Leave(KErrNone);
    }

    // Change input
    iLandmarkParser->SetInputFileL(KFileName);

    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != 0)
    {
    	iLog->Log(_L("No landmarks should be found"));
    	User::Leave(KErrNone);
    }

    // No landmark should be found
    TRAP(err, iLandmarkParser->LandmarkLC());
    if (err != KErrNotFound)
        {
        iLog->Log(_L("ERROR: No Landmark should be found"));
        iErrorsFound++;
        }

    delete iOperation;
    iOperation = NULL;
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    iLog->Log(_L("DoCancelTestL Done"));
    }

// ---------------------------------------------------------
// CPosTp47::CreateXMLData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::CreateXMLData(TBool aUseFile)
    {
    iLog->Log(_L("CreateXMLData"));
    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    if (aUseFile)
        {
        DeleteFileL(KFileName);
        // 2) Use file for encoding, set output file
        iLandmarkEncoder->SetOutputFileL(KFileName);
        }
    else
        {
        // 2) Use buffer for encoding
        iEncoderBuffer = iLandmarkEncoder->SetUseOutputBufferL();
        }

    // 300 characters
    _LIT(KLongFieldName, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    _LIT(KLongFieldDescription, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    _LIT(KLongFieldDescriptionExtra, "A________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________BA________B");
    _LIT(KLongFieldDescriptionExtra2, "Extra Extra Extra Extra Extra Extra Extra Extra Extra Extra Extra Extra");

    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, KLongFieldName);
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, KLongFieldDescription);
    iLandmarkEncoder->AddCollectionDataL((TPosLmCollectionDataId)(EPosLmCollDataContentSpecificDataBegin), KLongFieldDescriptionExtra);
    iLandmarkEncoder->AddCollectionDataL((TPosLmCollectionDataId)(EPosLmCollDataContentSpecificDataBegin + 1), KLongFieldDescriptionExtra2);

    iLog->Log(_L("Create Data..."));

    // 3) Add one landmark with all fields set to be encoded
    CPosLandmark* lm1 = CreateXMLLandmarkLC(_L("TP47Landmark1"));
    AddAllXMLFieldsL(lm1);

    iLandmarkEncoder->AddLandmarkL(*lm1);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);

    // 4) Add one more landmark to be encoded (id27 "LOG9" in categories 11-17)
    CPosLandmark* lm2 = iDatabase->ReadLandmarkLC(27);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 5) Add one category (pizzeria) to the latest added landmark
    CPosLandmarkCategory* landmarkCategory = categoryManager->ReadCategoryLC(1);

    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);

    // 6) Add one landmark to be encoded (id=18 "enat?r" in category 18)
    lm1 = iDatabase->ReadLandmarkLC(18);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    // Remove field 408 which is not exported
    lm1->RemovePositionField(408);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);

    // 7) Add one category (Frisör) to the latest added landmark
    landmarkCategory = categoryManager->ReadCategoryLC(18);

    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);

    // 8 ) Add one global category  (Entertainment) to the latest added landmark

    TPosLmItemId globalCategoryId = categoryManager->GetGlobalCategoryL(15000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
    }
    else
    {
    	iLog->Log(_L("Global category was not found1"));
    	User::Leave(-1);
    }

    // 9 ) Add one global category  (People) to the latest added landmark

    globalCategoryId = categoryManager->GetGlobalCategoryL(27000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else
    {
    	iLog->Log(_L("Global category was not found2"));
    	User::Leave(-1);
    }
    // 11) Add one landmark with all fields set to empty (_L(""))
    lm1 = CreateXMLLandmarkLC(_L("TP47LandmarkEmpty"));
    AddAllEmptyXMLFieldsL(lm1);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    // Remove field 402 (EPositionFieldCountryCode) which is not exported when empty
    lm1->RemovePositionField(402);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);

    // 12) Add one landmark with all fields set to be encoded
    lm1 = CreateXMLLandmarkLC(_L("TP47Landmark2"));
    AddAllXMLFieldsL(lm1);
    iLandmarkEncoder->AddLandmarkL(*lm1);

    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);

    // 13) Add one global category  (Shopping) to the latest added landmark
    globalCategoryId = categoryManager->GetGlobalCategoryL(36000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
    }
    else
    {
    	iLog->Log(_L("Global category was not found3"));
    	User::Leave(-1);
    }

    // 14) Add one global category  (Sports) to the latest added landmark
      globalCategoryId = categoryManager->GetGlobalCategoryL(42000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else
    	{
    	iLog->Log(_L("Global category was not found4"));
    	User::Leave(-1);
    	}

      // ) Finalize encoding
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    CleanupStack::PopAndDestroy(categoryManager);
    }

// ---------------------------------------------------------
// CPosTp47::CheckXMLData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::CheckXMLData(TBool aUseFile)
    {
    iLog->Log(_L("CheckXMLData"));

    // This number must be updated if any more landmarks are added
    const TInt KNrOfAddedLandmarks = 5;

    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    HBufC8* bufferContent = NULL;

    if (aUseFile)
        {
        iLog->Log(_L("----->Using FILE<-------"));
        iLandmarkParser->SetInputFileL(KFileName);
        }
    else
        {
        iLog->Log(_L("----->Using BUFFER<-------"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }

    iOperation = iLandmarkParser->ParseContentL();
    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;
    TInt number = 0;

    // 1) Get the first landmark, "TP47Landmark1", no categories
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    CPosLandmark* lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 0, _L("1) Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);
    CleanupStack::PopAndDestroy(lm);

    TUint32 nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != number)
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);
    }

    // 2) Get landmark, "LOG9", local category Pizzeria
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 1, _L("2) Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);
    CPosLandmarkCategory* category = iLandmarkParser->LandmarkCategoryLC(array[0]);
    CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(1);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);
    CleanupStack::PopAndDestroy(lm);

    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != number)
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);
    }

    // 3) Get landmark, "enato?r", local categories Frisör, Hunddagis and global categories Sports Ground, Gas Station
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 3, _L("3) Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);

    // check local category Frisör here
    category = iLandmarkParser->LandmarkCategoryLC(array[0]);
    cat = categoryManager->ReadCategoryLC(18);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    // check global category Sports Ground globalid=7 (id=33) here
    category = iLandmarkParser->LandmarkCategoryLC(array[1]);
   // cat = categoryManager->ReadCategoryLC(33);
    cat = categoryManager->ReadCategoryLC(31);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    //Check global category "id=3 Gas station" here
    category = iLandmarkParser->LandmarkCategoryLC(array[2]);
    //cat = categoryManager->ReadCategoryLC(29);
    cat = categoryManager->ReadCategoryLC(35);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    CleanupStack::PopAndDestroy(lm);
    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != number)// LogErrorAndLeave(_L("Wrong number of landmarks parsed"));
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);
    }

    // 4) Get landmark, TP47LandmarkEmpty, no categories
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 0, _L("4) Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);
    CleanupStack::PopAndDestroy(lm);

    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != number)
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);
    }

    // 5) Get landmark, TP47Landmark2, local categories Burger King and global categories Garage, Bus Stop
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 2, _L("5) Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);

    // check global category Garage globalid=8 (id=34) here
    category = iLandmarkParser->LandmarkCategoryLC(array[0]);
    //cat = categoryManager->ReadCategoryLC(34);
    cat = categoryManager->ReadCategoryLC(38);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    //Check global category "id=10 Bus Stop" here
    category = iLandmarkParser->LandmarkCategoryLC(array[1]);
   // cat = categoryManager->ReadCategoryLC(36);
    cat = categoryManager->ReadCategoryLC(40);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);
    CleanupStack::PopAndDestroy(lm);

    // Check that correct number of landmarks has been parsed
    if (number != KNrOfAddedLandmarks)
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);
    }
    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != KNrOfAddedLandmarks)
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);
    }

    if (!aUseFile) CleanupStack::PopAndDestroy(bufferContent);

    CleanupStack::PopAndDestroy(&array);
    CleanupStack::PopAndDestroy(categoryManager);

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iOperation;
    iOperation = NULL;

    // iEncoderBuffer was allocated in CreateXMLData
    delete iEncoderBuffer;
    iEncoderBuffer = NULL;

    iLog->Log(_L("CheckXMLData Done"));
    }

// ---------------------------------------------------------
// CPosTp47::PrintParsedDataFromEncoderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::PrintParsedDataFromEncoderL(
    const TDesC& aFile, 
    const TDesC8&  aBuffer, 
    TInt aExpectedStatusCode,
    const TDesC8& aMimeType)
    {
    iLog->Log(_L("===== PrintParsedDataFromEncoderL ======="));
   
   
    iLandmarkParser = CPosLandmarkParser::NewL(aMimeType);

    if (aFile != KNullDesC)
        {
        TFileName file( aFile );
        if ( aMimeType == KGpxMimeType )
            {
            TInt pos = file.Find( KXmlFileExt );
            file.Replace( pos, KXmlFileExt().Length(), KGpxFileExt );
            }
        iLog->Log(_L("FILE (%S)"), &file );
        TraceL(_L("----->Using FILE<-------"));
        iLog->Log(_L("Calling TraceL"));
        TraceL(file);
        iLog->Log(_L("Input file to be set"));
        iLandmarkParser->SetInputFileL(file);
        iLog->Log(_L("Input file set"));
        }
    else
        {
        iLog->Log(_L("BUFFER (size %d)"), aBuffer.Size() );
        TraceL(_L("----->Using BUFFER<-------"));
        iLandmarkParser->SetInputBuffer(aBuffer);
        }

    iOperation = iLandmarkParser->ParseContentL();
    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;
    TInt number = 0;

    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);
	iLog->Log(_L("Parse begin"));
    while (status == KPosLmOperationNotComplete)
        {
        iLog->Log(_L("--- Parsing ---------------------------"));
        TraceL(_L("------------------------------"));
        iOperation->NextStep(status, progress);

        // Wait for NextStep to complete
        User::WaitForRequest(status);
        if (status != KPosLmOperationNotComplete && status != KErrNone)
            {
            HBufC* buffer = HBufC::NewLC( 128);
            TPtr buf = buffer->Des();
                       
            buf.Format(_L("\tStatus %d"), status.Int());
            iLog->Log(buf);
            TraceL(buf);
            
            CleanupStack::PopAndDestroy( buffer );
            buffer = NULL;              
            }
        else
            {
            if ( iLandmarkParser->NumOfParsedLandmarks() )
                {
                // Get last parsed landmark
                CPosLandmark* lm = iLandmarkParser->LandmarkLC();
                TPtrC lmName;
                TPtrC catName;
                lm->GetLandmarkName(lmName);
                lm->GetCategoriesL(array);
                //iLog->Log(lmName);
                for (TInt i=0;i<array.Count();i++)
                    {
                    CPosLandmarkCategory* category = iLandmarkParser->LandmarkCategoryLC(array[i]);
                    category->GetCategoryName( catName );
                     
                    HBufC* buffer = HBufC::NewLC( 128 + catName.Length());
                    TPtr buf = buffer->Des();
                                            
                    if ( category->GlobalCategory()) 
                        {
                        buf.Append(_L("\tGlobal category: "));    
                        }
                    else 
                        {
                        buf.Append(_L("\tLocal category: "));    
                        }                        
                    buf.Append(catName);
                    iLog->Log(buf);
                    TraceL(buf);
                    CleanupStack::PopAndDestroy(buffer);
                    buffer = NULL;
                    CleanupStack::PopAndDestroy(category);
                    }
                    
                if ( aMimeType == KKmlMimeType )
                {
                	PrintLandmarkFieldsWithDescriptionL(*lm, ETrue);	
                }
                else
                {
                	PrintLandmarkFieldsL(*lm, ETrue);	
                }
                
                number++;
                CleanupStack::PopAndDestroy(lm);
                }
            }
        }

    iLog->Log(_L("--- Parsing end ---------------------------"));
    TraceL(_L("------------------------------"));
    CleanupStack::PopAndDestroy(&array);

    if (status.Int() != aExpectedStatusCode)
        {
        HBufC* buffer = HBufC::NewLC( 128 );
        TPtr buf = buffer->Des();
                    
        buf.Format(_L("\tERROR: Wrong status returned, was %d, should be %d"), status.Int(), aExpectedStatusCode);
        iLog->Log( buf );
        iErrorsFound++;
        
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;        
        }

    TUint32 nr = iLandmarkParser->NumOfParsedLandmarks();
    HBufC* buffer = HBufC::NewLC( 256 );
    TPtr buf = buffer->Des();
    buf.Format(_L("\tNr of Parsed landmarks %d, should be %d"), nr, number);
    iLog->Log(buf);
    TraceL(buf);

    CleanupStack::PopAndDestroy( buffer );
    buffer = NULL;
    if (nr != (TUint32)number)
        {
        iLog->Log(_L("\tERROR: Wrong number of landmarks parsed returned!"));
        iErrorsFound++;
        }

    TPosLmCollectionDataId lmId = iLandmarkParser->FirstCollectionDataId();

    iLog->Log(_L("--- CollectionData ---"));
    TraceL(_L("--- CollectionData ---"));

    if (lmId != EPosLmCollDataNone)
        {
        TPtrC first = iLandmarkParser->CollectionData(lmId);
        HBufC* buffer = HBufC::NewLC( first.Length() + 256 );
        TPtr buf = buffer->Des();
        buf.Zero();
        buf.Format(_L("\t(1)Collection Id: %d CollectionData: "), lmId);
        buf.Append(first);
        iLog->Log(buf);
        TraceL(buf);
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;      
        }
    else 
        {
        iLog->Log(_L("\tNo collection data found 1"));    
        }

    while (lmId != EPosLmCollDataNone)
        {
        lmId = iLandmarkParser->NextCollectionDataId(lmId);
        if (lmId != EPosLmCollDataNone)
        {
            TPtrC first = iLandmarkParser->CollectionData(lmId);
            
            HBufC* buffer = HBufC::NewLC( first.Length() + 256 );
            TPtr buf = buffer->Des();         
            buf.Zero();
            buf.Format(_L("\t(2)Collection Id: %d CollectionData: "), lmId);
            buf.Append(first);
            iLog->Log(buf);
            TraceL(buf);
            
            CleanupStack::PopAndDestroy( buffer );
            buffer = NULL;              
        }
        else 
            {
            iLog->Log(_L("\tNo collection data found 2"));   
            }
        }

    iLog->Log(_L("--- CollectionData ---"));
    TraceL(_L("--- CollectionData ---"));
    
	
    delete iOperation;
    iOperation = NULL;
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    iLog->Log(_L("====== PrintParsedDataFromEncoderL Done ========\n"));
    }

// ---------------------------------------------------------
// CPosTp47::PrintLandmarkFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::PrintLandmarkFieldsL(const CPosLandmark& aLandmark, TBool aTraceFlag)
    {
    TPositionFieldId sourceFieldId = aLandmark.FirstPositionFieldId();
    TLocality loc;

    TInt err;
    TPtrC landmarkName;
    err = aLandmark.GetLandmarkName(landmarkName);
    if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( landmarkName.Length() + 256 );
        TPtr buf = buffer->Des();
                    
        buf.Append( _L(" \tLandmark Name: "));
        buf.Append( landmarkName );
        iLog->Log( buf );
        if( aTraceFlag ) 
            {
            TraceL( buf );   
            }
            
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;  
        }

    err = aLandmark.GetPosition(loc);
    if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( 1024 );
        TPtr buf = buffer->Des();        
        TRealFormat format( 12, KRealFormatFixed );
        format.iPoint = TChar('.');
        format.iTriLen = KDoNotUseTriads;
        format.iPlaces = 6;
        
        TBuf<20> sLon, sLat, sVacc, sHacc, sAlt, sRad;
        sLon.Num( loc.Longitude(), format );
        sLat.Num( loc.Latitude(), format );
        sAlt.Num( loc.Altitude(), format );
        sVacc.Num( loc.VerticalAccuracy(), format );
        sHacc.Num( loc.HorizontalAccuracy(), format );
        buf.Format(
            _L("\tLM: Long %S Lat %S vertAcc %S horAcc %S alt %S "), 
            &sLon, &sLat, &sVacc, &sHacc, &sAlt);
        
        TReal32 sourceR;
        err = aLandmark.GetCoverageRadius(sourceR);
        if (err == KErrNone )
            {
            sRad.Num( sourceR, format );
            buf.AppendFormat(_L("srcRadius %S"), &sRad);
            }
        iLog->Log(buf);
        if (aTraceFlag) TraceL(buf);
        CleanupStack::PopAndDestroy(buffer);
        }

    while (sourceFieldId != EPositionFieldNone)
        {
        TPtrC sourceValue;
        aLandmark.GetPositionField(sourceFieldId, sourceValue);
        HBufC* buffer = HBufC::NewLC( sourceValue.Length() + 256 );
        TPtr buf = buffer->Des();  
        buf.Format(_L("\tIdField Id: %d Value:"), sourceFieldId);
        buf.Append(sourceValue);
        iLog->Log(buf);
        if ( aTraceFlag ) 
            {
            TraceL( buf );
            }
        sourceFieldId = aLandmark.NextPositionFieldId(sourceFieldId);
        
        CleanupStack::PopAndDestroy( buffer );
        }
    }
    
    
 // ---------------------------------------------------------
// CPosTp47::PrintLandmarkFieldsWithDescriptionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::PrintLandmarkFieldsWithDescriptionL(const CPosLandmark& aLandmark, TBool aTraceFlag)
    {
    TPositionFieldId sourceFieldId = aLandmark.FirstPositionFieldId();
    TLocality loc;
       

    TInt err;
    TPtrC landmarkName;
    TPtrC landmarkDescription;
    err = aLandmark.GetLandmarkName(landmarkName);
    if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( landmarkName.Length() + 256 );
        TPtr buf = buffer->Des();
        buf.Append(_L("\tLandmark Name: "));
        buf.Append(landmarkName);
        iLog->Log(buf);
        if( aTraceFlag ) 
            {
            TraceL( buf );   
            }
            
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;  
        }

    err = aLandmark.GetPosition(loc);
    if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( 1024 );
        TPtr buf = buffer->Des(); 
        TRealFormat format( 12, KRealFormatFixed );
        format.iPoint = TChar('.');
        format.iTriLen = KDoNotUseTriads;
        format.iPlaces = 6;
        
        TBuf<20> sLon, sLat, sVacc, sHacc, sAlt, sRad;
        sLon.Num( loc.Longitude(), format );
        sLat.Num( loc.Latitude(), format );
        sAlt.Num( loc.Altitude(), format );
        sVacc.Num( loc.VerticalAccuracy(), format );
        sHacc.Num( loc.HorizontalAccuracy(), format );
        buf.Format(
            _L("\tLM: Long %S Lat %S vertAcc %S horAcc %S alt %S "), 
            &sLon, &sLat, &sVacc, &sHacc, &sAlt);
        
        TReal32 sourceR;
        err = aLandmark.GetCoverageRadius(sourceR);
        if (err == KErrNone )
            {
            sRad.Num( sourceR, format );
            buf.AppendFormat(_L("srcRadius %S"), &sRad);
            }
        iLog->Log(buf);
        if (aTraceFlag) TraceL(buf);
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;  
        }
        
      //Get landmark description  
      err = aLandmark.GetLandmarkDescription(landmarkDescription);
      if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( landmarkDescription.Length() + 256 );
        TPtr buf = buffer->Des();         
        buf.Format(_L("\tLandmark Description: "));
        buf.Append(landmarkDescription);
        iLog->Log(buf);
        if ( aTraceFlag ) 
            {
            TraceL( buf );    
            }
        
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;         
        }  
    

    while (sourceFieldId != EPositionFieldNone)
        {
        TPtrC sourceValue;
        aLandmark.GetPositionField(sourceFieldId, sourceValue);
        
        HBufC* buffer = HBufC::NewLC( sourceValue.Length() + 256 );
        TPtr buf = buffer->Des();  
        buf.Format(_L("\tIdField Id: %d Value:"), sourceFieldId);
        buf.Append(sourceValue);
        iLog->Log(buf);
        if ( aTraceFlag ) 
            {
            TraceL( buf );
            }
        sourceFieldId = aLandmark.NextPositionFieldId(sourceFieldId);
        
		    CleanupStack::PopAndDestroy(buffer);    
		    }
}
// ---------------------------------------------------------
// CPosTp47::TraceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp47::TraceL(const TDesC& msg)
    {
    _LIT8( KEnd, "\r\n" );

    RFile file;
    TInt err = file.Open(iFileSession, KFileTrace, EFileWrite);
    if (err == KErrNotFound)
        {
        file.Create(iFileSession, KFileTrace, EFileWrite);
        }

    HBufC8* line8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( msg );
    
    TInt pos( 0 );
    file.Seek( ESeekEnd, pos );
    file.Write( *line8 );
    file.Write( KEnd );
    file.Close();

    delete line8;
    }

void CPosTp47::MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
                                     CPosLmCategoryManager& aCatMgr )
    {

    CPosLmOperation* operation1 = aLdb.RemoveAllLandmarksL();
    //Remove all landmarks from default db : Sync call
    ExecuteAndDeleteLD( operation1 );

    RArray<TPosLmItemId> catArray;
    CleanupClosePushL( catArray );

    // Get all the categories from the database and delete them if necessary
    CPosLmItemIterator* iter = aCatMgr.CategoryIteratorL();
    CleanupStack::PushL( iter );

    TUint count = iter->NumOfItemsL();
    if ( count > 0 )
        { // can only be called if there are some items
        iter->GetItemIdsL( catArray, 0, count );
        }

    // Remove all the categories here Sync call
    ExecuteAndDeleteLD(aCatMgr.RemoveCategoriesL( catArray ));
    CleanupStack::PopAndDestroy( 2 ); // catArray, iter
    }
//  End of File
