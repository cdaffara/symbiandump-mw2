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


#ifndef LANDMARKCONSTANTS_H
#define LANDMARKCONSTANTS_H

#include <e32def.h>

// CONSTANTS


const TUint KTimeout = 10000000;


const TReal32 KHorizontalAcc = 10;
const TReal32 KVerticalAcc = 30;
const TReal64 KLatitude = 65.3;
const TReal64 KLongitude = 11.3;
const TReal32 KAltitude = 2;
const TReal32 KCoverageRadius = 50;

_LIT(KMediaLinks, "3");
_LIT(KMediaLink1, "https://www.aftonbladet.se");
_LIT(KMediaLink2, "http://www.idg.se");
_LIT(KMediaLink3, "http://www.svt.se");
_LIT(KCountry, "Sweden");

_LIT(KHorizontalSpeed, "126");
_LIT(KOrientation, "10");

_LIT(KMediaLinks2, "2");
_LIT(KMediaLink4, "https://www.hotmail.com");
_LIT(KMediaLink5, "http://www.sdsadsad.se");

_LIT(KXMLMediaLink1, "html/text/http://www.enator.se");
_LIT(KXMLMediaLink2, "http://www.sdsadsad.se");
_LIT(KXMLMediaLink3, "mailto:sergey@nokia.com");
_LIT(KXMLMediaLink4, "news:comp.infosystems.www.servers.unix");
_LIT(KXMLMediaLink5, "html/text/http://www.awdawdawd.se");
_LIT(KXMLMediaLink6, "http://www.glassbilen.se");
_LIT(KXMLMediaLink7, "//http://www.testofparsing.se");
_LIT(KXMLMediaLink8, "text/plain/mailto:jani@nokia.com");
_LIT(KXMLMediaLink9, "application/wordperfect 5.1/z://some_application.exe");
_LIT(KXMLMediaLink10, "text/plain/news:comp.infosystems.www.servers.dos");
_LIT(KXMLMediaLink11, "z:/some_link_to_somewhere1");
_LIT(KXMLMediaLink12, "z://some_link_to_somewhere2");
_LIT(KXMLMediaLink13, "text/link/z://some_link_to_somewhere3");
_LIT(KXMLMediaLink14, "image/png/some_png_file.html");

//This is not exported, (ignored due to parsing)
//_LIT(KXMLMediaLink13, "z:\\some_link_to_somewhere3");

_LIT(KCountry2, "Finland");
_LIT(KHorizontalSpeed2, "1");
_LIT(KOrientation2, "120");
_LIT(KLast, "Laaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaast");

const TUint KNoPosFields = 7;
const TInt KNoCategories = 3;

_LIT(KMbmFileName, "c:\\system\\test\\landmarkstestsicon.mbm");
const TInt KIconIndex = 0; // EMbmLandmarkstestsiconTelogo_25x20i;
const TInt KIconMaskIndex = 0;

const TReal32 KHorizontalAcc2 = 10;
const TReal32 KVerticalAcc2 = 30;
const TReal64 KLatitude2 = 45.3;
const TReal64 KLongitude2 = 21.2;
const TReal32 KAltitude2 = 2000;
const TReal32 KCoverageRadius2 = 360;

_LIT(KMbmFileName2, "c:\\system\\test\\landmarkstestsicon.mbm");
const TInt KIconIndex2 = 323; // EMbmLandmarkstestsiconTelogo_25x20i;


// Address Capabilities
_LIT(KCountryCode,"SE");
_LIT(KState, "-");
_LIT(KCounty, "County??");
_LIT(KCity, "Gothenborg");
_LIT(KDistrict,"Gårda");
_LIT(KStreet, "Vädursgatan 6");
_LIT(KStreetExtension, "");
_LIT(KLocationName, "TietoEnator");
_LIT(KPostalCode, "412 50");
_LIT(KLocality, "Gårda");
_LIT(KCrossing1, "Fabriksgatan");
_LIT(KCrossing2, "Åvägen");

// Building Capabilities
_LIT(KBuildingName, "BuildingName");
_LIT(KBuildingFloor, "7");
_LIT(KBuildingRoom, "Yellow");
_LIT(KBuildingZone, "BuildingZone");
_LIT(KBuildingTelephone, "031444444");

_LIT(KName, "Micke's Pub");
_LIT(KName2, "Drivhuset");
_LIT(KCategoryName, "Pub");
_LIT(KCategoryName2, "Resturant");

_LIT(KDescription, "jada jada jada jada jada jada jada jada jada jada jada");
//_LIT(KDescription, "jada");
_LIT(KDescription2, "asddsadsads gg");

//const TPosLmGlobalCategory KGlobalCategoryId = 123;

const TInt KNoMultipleClients=5;

const TInt KNoAddRequests=10;

_LIT(KDefaultDb, "eposlm.ldb");
_LIT(KTp25TestDb, "TP25Test.ldb");
_LIT(KTp131TestDb, "TP131Test.ldb");
_LIT(KTp1TestDb, "TP1Test.ldb");
_LIT(KTp4TestDb, "TP4Test.ldb");
_LIT(KTp10TestDb, "TP10Test.ldb");
_LIT(KTp8TestDb, "TP8Test.ldb");
_LIT(KDefaultDbPath, "c:\\system\\data\\");
_LIT(KEmptyCopyPath, "c:\\system\\data\\eposEmpty.ldb");
_LIT(KDb20, "EPOSLM_020.LDB");
_LIT(KDb40, "EPOSLM_040.LDB");
_LIT(KDb60, "EPOSLM_060.LDB");
_LIT(KDb80, "EPOSLM_080.LDB");
_LIT(KDb105, "EPOSLM_105.LDB");
_LIT(KDb20Uri, "file://C:EPOSLM_020.LDB");
_LIT(KDb40Uri, "file://C:EPOSLM_040.LDB");
_LIT(KDb60Uri, "file://C:EPOSLM_060.LDB");
_LIT(KDb80Uri, "file://C:EPOSLM_080.LDB");
_LIT(KDb105Uri, "file://C:EPOSLM_105.LDB");
_LIT(KDbNamePath, "c:\\private\\100012a5\\DBS_101FE978_EPOSLM_%d.LDB");
_LIT(KDbName, "eposlm_%d.ldb");

// Note: This is defined in landmarks
_LIT(KLmDbPath, "c:\\private\\100012a5\\DBS_101FE978_");
_LIT(KLmFullDefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_EPOSLM.LDB");
_LIT(KLmTp25DefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_TP25TEST.LDB");
_LIT(KLmTp131DefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_TP131TEST.LDB");
_LIT(KLmTp1DefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_TP1TEST.LDB");
_LIT(KLmTp4DefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_TP4TEST.LDB");
_LIT(KLmTp8DefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_TP8TEST.LDB");
_LIT(KLmTp10DefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_TP10TEST.LDB");

_LIT(KTestPath, "c:\\system\\test\\testdata\\");

_LIT(KLandmarksEmptyDb, "eposEmpty.ldb");

_LIT(KGlobalCategoryResFileWINS, "c:\\resource\\EPOSLMGLOBALCATEGORIES.R01");
_LIT(KGlobalCategoryResFileROM, "c:\\resource\\EPOSLMGLOBALCATEGORIES.R01");
_LIT(KGlobalCategoryResFileCOPY, "c:\\resource\\COPY_EPOSLMGLOBALCATEGORIES.R01");

#ifdef __WINS__
_LIT(KCopyDbPath, "c:\\");
_LIT(KLandmarksDb, "c:\\system\\test\\TestData\\eposlm.ldb");

_LIT(KCopiedDb, "c:\\eposlm.ldb");
_LIT(KCompositeLandmarksDb, "c:\\system\\test\\TestData\\compositeeposlm.ldb");
_LIT(K1000LandmarksDb, "eposlm_1000_LMs.ldb");
_LIT(K5000LandmarksDb, "eposlm_5000_LMs.ldb");
#else
_LIT(KCopiedDb, "c:\\system\\test\\eposlm.ldb");
_LIT(KCopyDbPath, "c:\\system\\test\\");
_LIT(KLandmarksDb, "c:\\system\\test\\TestData\\eposlm.ldb");
_LIT(KCompositeLandmarksDb, "c:\\system\\test\\TestData\\compositeeposlm.ldb");

_LIT(K1000LandmarksDb, "eposlm_1000_LMs.ldb");
_LIT(K5000LandmarksDb, "eposlm_5000_LMs.ldb");
#endif

#endif      // LANDMARKCONSTANTS_H

// End of File


