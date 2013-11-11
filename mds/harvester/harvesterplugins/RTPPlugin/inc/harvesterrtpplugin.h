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
* Description:  Harvests metadata for rtp video file
*
*/

#ifndef C_HARVESTERRTPPLUGIN_H
#define C_HARVESTERRTPPLUGIN_H

#include <harvesterplugin.h>
#include <e32std.h>
#include <apmstd.h>

// FORWARD DECLARATION 
class CHarvesterData;
class CMdEObject;
class CMdEObjectDef;

const TInt KMaxMimeTypeLength( KMaxDataTypeLength );
const TInt KMaxProgramTitle( 255 ); //to fit in db

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester OMA DRM plug-in.
 */
class CHarvesterRtpPluginPropertyDefs : public CBase
	{
	public:
		// Common property definitions
		CMdEPropertyDef* iCreationDatePropertyDef;
		CMdEPropertyDef* iLastModifiedDatePropertyDef;
		CMdEPropertyDef* iSizePropertyDef;
		CMdEPropertyDef* iItemTypePropertyDef;
		CMdEPropertyDef* iTitlePropertyDef;
		CMdEPropertyDef* iDefaultFolderPropertyDef;
	
		// Media property definitions
		CMdEPropertyDef* iCaptureDatePropertyDef;
		CMdEPropertyDef* iDurationPropertyDef;

		// Video property definitions
		CMdEPropertyDef* iAgeProfilePropertyDef;
		CMdEPropertyDef* iRecordingFlagsPropertyDef;
		
	private:
		CHarvesterRtpPluginPropertyDefs();
	
		void ConstructL(CMdEObjectDef& aObjectDef);

	public:	
        static CHarvesterRtpPluginPropertyDefs* NewL();
        void SetByObjectDefL( CMdEObjectDef& aObjectDef) ;
	};

/**
 *  Data class for rtp harvester
 *
 *  Includes clip metadata from metaheader
 *
 */
class CHarvesterRtpClipDetails : public CBase
    {
public:
	
/** NewL */
static CHarvesterRtpClipDetails* NewL()
    {
    CHarvesterRtpClipDetails* self = new (ELeave) CHarvesterRtpClipDetails;
    return self;
    }

/** Destructor */
virtual ~CHarvesterRtpClipDetails()
    {
    }

public:

    /**
     * Mime type
     */
     TBuf<KMaxMimeTypeLength> iMimeType;

     /**
     * Modification date
     */
     TTime iModifiedDate;

     /**
      * File size
      */
     TUint32 iFileSize;
     
    /**
    * Program title including service name and program name
    */
    TBuf<KMaxProgramTitle> iTitle;       
    
    /**
    * Recording duration.
    */
    TReal32 iDuration;

    /**
    * Clip's quality. 
    */
    TUint8 iQuality; 

    /**
    * Recording currently ongoing state.
    */
    TBool iRecOngoing;

    /**
    * Recording completed status.
    */
    TBool iRecCompleted;

    /**
    * Recording failed status.
    */
    TBool iRecFailed;
    
    /**
    * Parental rate
    */
    TInt iParental;
    
    /**
    * Post acquisition rule
    */
    TBool iPostRule;        
    
private:

    /**
     * Default constructor
     */
    CHarvesterRtpClipDetails()
    {
    }
};

/**
 *  This class implements RTP harvester plugin.
 *
 *  @lib harvesterrtpplugin.dll
 *  @since S60 S60 v3.x
 */
class CHarvesterRtpPlugin : public CHarvesterPlugin
    {
public:

    /**
     * Constructs a new CHarvesterRtpPlugin implementation.
     * 
     * @return A pointer to the new CHarvesterRtpPlugin implementation
     */
    static CHarvesterRtpPlugin* NewL();

    /**
    * Destructor.
    */
    virtual ~CHarvesterRtpPlugin();

    // from base class CHarvesterPlugin
    
    /**
    * Harvest file. 
    * 
    * @param aHarvesterData  
    */
    void HarvestL( CHarvesterData* aHarvesterData );

protected: // from CHarvesterPlugin
    
    void GetMimeType( const TDesC& aUri, TDes& aMimeType );
    
private:

    /**
    * C++ constructor
    */
    CHarvesterRtpPlugin();

    /**
    * 2nd phase constructor
    */
    void ConstructL();
    
    /**
     * Gathers data from file to meta data object. 
     * Leaves if mime type is not supported.
     *
     * @param aMetadataObject  A reference to meta data object to gather the data. 
     */
     void GatherDataL( CMdEObject& aMetadataObject, CHarvesterRtpClipDetails& aClipDetails );
     
     /**
      * Recognize file mime type
      *
      * @param aFilePath File path for recognizer
      * @param aFileBuffer Data buffer for file recognition
      * @return KErrNone if file is recognized, otherwise symbian error code.
      */
     TInt RecognizeFile( const TDesC& aFilePath, const TDesC8& aFileBuffer, CHarvesterRtpClipDetails& aClipDetails );
      
      /**
       * Check if mime type is supported
       * @param aMimeBuf Mime type 
       * @return KErrNone if mime type is supported
       * @return KErrNotSupported if mime type is not supported
       */
     TInt CheckIfMimeSupported( const TDesC& aMimeBuf );
     
     /** */
     void HandleObjectPropertiesL( CHarvesterData& aHD, CHarvesterRtpClipDetails& aClipDetails, TBool aIsAdd);
     
     void InitPropDefsL( CMdEObjectDef& aObjectDef );

private: // data
	CHarvesterRtpPluginPropertyDefs* iPropDefs;
    };

#endif // C_HARVESTERWMVPLUGIN_H
