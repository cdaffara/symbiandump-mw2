/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Data transfer object for placeholder data
*
*/


#ifndef PLACEHOLDERDATA_H
#define PLACEHOLDERDATA_H

// INCLUDES
#include <e32std.h>
#include <e32cmn.h>
#include <e32def.h>
#include <e32base.h>


/**
 * A helper class which encapsulates data needed for 
 * creating placeholder objects.
 */
NONSHARABLE_CLASS( CPlaceholderData ) : public CBase
	{
	public:

	    /**
	     * Second phase construction
	     */
		IMPORT_C static CPlaceholderData* NewL();

	    /**
	     * Destructor.
	     */
		virtual ~CPlaceholderData();

	    /**
	     * Returns modification time - equals RFs::Modified()
	     * 
	     * @param  Last modification time
	     */				
		IMPORT_C TTime Modified();
		
	    /**
	     * Sets modification time - should be RFs::Modified()
	     * for binary files
	     * 
	     * @return  Last modification time
	     */		
		IMPORT_C void SetModified( TTime aModified );
		
	    /**
	     * Returns URI of the file/item.
	     * 
	     * @return  URI
	     */			
		IMPORT_C TPtr16 Uri();

	    /**
	     * Sets URI of the file/item.
	     * 
	     * @param aUri  URI of the file
	     */		
		IMPORT_C void SetUri( const TDesC& aUri );
		
	    /**
	     * Returns file size bytes.
	     * 
	     * @return  File size in bytes.
	     */					
		IMPORT_C TUint32 FileSize();

	    /**
	     * Sets file size bytes.
	     * 
	     * @param aUri  File size bytes.
	     */	
		IMPORT_C void SetFileSize( TUint32 aFileSize );
		
	    /**
	     * Returns MediaID which identifies the volume/drive where
	     * the file is located. Equals to TVolumeInfo::iUniqueID.
	     * 
	     * @return  MediaID
	     */				
		IMPORT_C TUint MediaId();
		
	    /**
	     * Sets MediaID which identifies the volume/drive where
	     * the file is located. Equals to TVolumeInfo::iUniqueID.
	     * 
	     * @param aMediaId  MediaID of the volume.
	     */	
		IMPORT_C void SetMediaId( TUint aMediaId );
		
	    /**
	     * Returns present state for the file.
	     * 
	     * @return  Present state.
	     */		
		IMPORT_C TInt PresentState();

	    /**
	     * Sets present state for the file
	     * 
	     * @param aState  Present state.
	     */			
		IMPORT_C void SetPresentState( TInt aState );

	    /**
	     * Returns whether file is pre-installed or not.
	     * 
	     * @return  Whether file is pre-installed or not.
	     */		
		IMPORT_C TInt Preinstalled();

	    /**
	     * Sets whether file is pre-installed or not.
	     * 
	     * @param aValue  Whether file is pre-installed or not.
	     */	
		IMPORT_C void SetPreinstalled( TInt aValue );
		
	protected:
	
	    /**
	     * Constructor
	     */	
		void ConstructL();
	
	private:
		
		/* URI of the placeholder */
		HBufC* iUri;
		
		/* Last modified time - RFs::Modified() */ 
		TTime iModified;
		
		/* File size in bytes */
		TUint32 iFileSize;
		
		/* Volume/Drive ID where file - TVolumeInfo::iUniqueID*/
		TUint32 iMediaId;
		
		/* Present/not present state */
		TInt iPresentState;
		
		/* Whether file is pre-installed or not */
		TInt iPreinstalled;
	};

#endif // PLACEHOLDERDATA_H

