/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common Exif-utilities for harvester
*
*/


#ifndef HARVESTEREXIFUTIL_H
#define HARVESTEREXIFUTIL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <ExifRead.h>
#include <ExifTag.h>
#include <apmstd.h>
#include "harvesterlog.h"
#include "harvestdata.h"
#include "mdeobject.h"
#include "mdesession.h"

class CExifModify;

// CLASS DECLARATION

/**
*  CHarvesterExifUtil
* 
*/
NONSHARABLE_CLASS( CHarvesterExifUtil ) : public CBase
{
public: // Constructors and destructor

	/**
		* Destructor.
		*/
	IMPORT_C ~CHarvesterExifUtil();

		/**
		* Two-phased constructor.
		*/
	IMPORT_C static CHarvesterExifUtil* NewL();

		/**
		* Two-phased constructor.
		*/
	IMPORT_C static CHarvesterExifUtil* NewLC();
	

	IMPORT_C void SetSession( CMdESession* aSession );
		/**
		* 
		*/
	IMPORT_C static TBool IsValidExifData(TPtr8 aData);

	IMPORT_C static TInt ReadExifDataL( CHarvestData& aHd, CFileData& aFileData );
	
	IMPORT_C static TTime ConvertExifDateTimeToSymbianTimeL( const TDesC8& aDateTime );
	
	IMPORT_C TInt ComposeExifDataL( CMdEObject& aObject, TPtr8 aImagePtr, HBufC8*& aModified );

	IMPORT_C void ComposeLocationL( CMdEObject* aLocation, TPtr8 aImagePtr, HBufC8*& aModified );
	
protected:
	void SetExifDefaultsL( CMdEObject& aMdeObject, CExifModify& aExifModify );
	void AddPropertyL( CMdEObjectDef& aObjectDef, CMdEObject& aMdeObject,
			const TDesC& aProperty, TUint32 aValue );
	void AddPropertyL( CMdEObjectDef& aObjectDef, CMdEObject& aMdeObject,
			const TDesC& aProperty, TUint16 aValue );
	TBool ModifyExifTagL( CMdEObject& aMdeObject, CExifModify& aExifModify,
			const TDesC& aProperty, TExifIfdType aIFD, TUint16 aTagID, TBool aRemove = EFalse );
	HBufC8* GetPropertyValueLC( const CMdEPropertyDef& aPropDef,
			const CMdEProperty& aProperty );
	CExifTag::TExifTagDataType ExifTagDataType( TUint16 aTagID, const CMdEPropertyDef& aPropDef );
	
	TBool CompareTag( TPtrC8 aMdeData, const CExifTag* aTag );
	
	static HBufC16* ReadExifTagL( const CExifRead& aReader, TExifIfdType aIFD, TUint16 aTagID );

	static void StripNulls( HBufC& aString );

private:

	/**
		* Constructor for performing 1st stage construction
		*/
	CHarvesterExifUtil();

	/**
		* default constructor for performing 2nd stage construction
		*/
	void ConstructL();
	
	static void ReadUserCommentL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadXResolutionL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadYResolutionL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadExposureBiasL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadExposureTimeL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadFNumberL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadShutterSpeedL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadApertureValueL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadFocalLengthL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadFocalLength35mmL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadFocalXPlaneResolutionL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadFocalYPlaneResolutionL( CHarvestData& aHd, CExifRead* aReader );
	
	static void ReadGPSLatitudeL( CHarvestData& aHd, CExifRead* aReader, TBool& aLatitude );
	
	static void ReadGPSLongitudeL( CHarvestData& aHd, CExifRead* aReader, TBool& aLatitude );
	
	static void ReadGPSAltitudeL( CHarvestData& aHd, CExifRead* aReader );
	
private:
	
	/** Pointer to mde session to use */
    CMdESession* iSession;
    
    /** Mde default namespace. NOT OWN. */
    CMdENamespaceDef* iDefaultNamespace;
    
    

};

#endif // HARVESTEREXIFUTIL_H

