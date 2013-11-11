/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Encapsulates message real addressa and alias
*
*/



#ifndef T_ATTACHMENTFILE_H
#define T_ATTACHMENTFILE_H

#include <e32base.h> 
#include <apmstd.h>

// Forward declarations
class RFs;
class RFile;
class TDesC16;
class RApaLsSession;
class CDesC16Array;
class CRichText;
class CMsvSession;
class CFileMan;

NONSHARABLE_CLASS (CSendUiAttachment) : public CBase
	{
public:

	enum TSendUiAttachmentType
		{
		EAttachmentHandle,
		EAttachmentPath
		};
		
	enum TSendUiAttachmentFlags
	    {
	    EAttachmentTemporary = 0x01  // The file will be deleted at object destruction
	    };
	
public:	

    /**
     * NewLC
     *
     * Member variables iPath, iHandle and iSize are guaranteed to have a correct values
     * after creation. Owns created iHandle.
     * 
     * @return Pointer to a valid CSendUiAttachment object.
     */  
    static CSendUiAttachment* NewLC( const TDesC16* aPath, RFs& aRFs );

   /**
    * NewLC
    *
    * @param aHandle Does not take ownership of aHandle. iHandle is not own.
    */
    static CSendUiAttachment* NewLC( const RFile* aHandle );
	
   /**
    * NewLC
    *
    * @param aBodyText Object of CRichText. Temporary attachment file is created, which
    *        takes name of first characters of the CRichText object. The created iHandle
    *        is own.
    */
    static CSendUiAttachment* NewLC( const CRichText& aBodyText, RFs& aRFs );
	
	void ConstructL( const TDesC16* aPath, RFs& aRFs );
	
	void ConstructL( const CRichText& aBodyText, RFs& aRFs );
	
	CSendUiAttachment( );
	
	virtual ~CSendUiAttachment();

    /**
    * InitAttachmentArrayLCC
    *
    * Static init function for importing file paths and handles into CSendUiAttachment
    * array.
    */
    IMPORT_C static CArrayPtrFlat<CSendUiAttachment>* InitAttachmentArrayLCC(
        const CDesC16Array& aPaths,
        const RArray<RFile>& aHandles,
        RFs& aFSs );
    
    
    /**
     * Returns type of the attachment
     * @return 	EAttachmentHandle or EAttachmentPath
     */   
	IMPORT_C TInt Type();

    /**
    * Creates temporary path
    *
    * This is used to save the temporary body text attachment. See above.
    * @since Series 60 v3.2
    * @param aTempFileName Name of a temporary file
    */  
    IMPORT_C static void CreateTempPathL( 
        TDes16& aCompletePath, 
        TFileName& aTempFileName,
        RFs& aRFs );

    /**
     * @return A path to the attachment
     */   
    inline const TDesC16* Path();
        
    /**
     * @return A handle to the attachment
     */   
    inline const RFile* Handle();
    
    /**
     * Set MimeType of the attachment
     * @return None
     */   
    inline void SetMimeType( TDataType aMimeType );
    
    /**
     * Return Mime type descriptor
     * @return Mime type description
     */   
    inline const TPtrC8 MimeType();
    
    /**
     * @return Size of file. Size is set during construction.
     */   
    inline TInt Size();
    
    /**
     * Set flag(s)
     * @param aFlag
     */   
	inline void SetFlags( TInt aFlag );
    
    /**
     * File flags
     * @return Flags.
     */   	
	inline TBool Flag( TInt aFlag );

    
private:

    /**
     * Size of attachment. Initialized in construction. 
     */   
	TInt 	iSize;

    /**
     * Flags. 
     */   
	TInt    iFlags;

    /**
     * MimeType 
     */   
    TDataType iMimeType;

    /**
     * Handle to the attachment. 
     * Owns, if iPath is not NULL.
     * Does not own, if iPath is NULL.
     *
     * Note: See NewLC functions for clarification of ownership.
     */   
	const RFile* iHandle;
	
    /**
     * Path to the attachment. 
     * Owns.
     */   
	TDesC16* iPath;

    /**
     * FileManager. Used for file operations with the temporary file.
     * Owns.
     */
    CFileMan* iFileManager;
	};


#include "csenduiattachment.inl"	

#endif