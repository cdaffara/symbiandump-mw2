// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Declaration of the ROCHCAFInterface class
// 
//

 
#ifndef ROCHCAFINTERFACE_H
#define ROCHCAFINTERFACE_H

#include <e32base.h>
#include <caf/importfile.h>
#include <caf/supplier.h>
#include <caf/supplieroutputfile.h>
#include <caf/metadataarray.h>
#include <caf/metadata.h>
#include <caf/caferr.h>


enum TROCHErrorReason	{
						EPanicInvalidCafState = 1
						
						};

using namespace ContentAccess;


/**
Content Access Framework (CAF) Wrapper class.
Provides encapsulation of the CAF functionality required by RO Content Handler.
The in source documentation refers to a CAF session. The following pseudocode
clarifies the concept of a CAF session.
@code
BEGIN
	WHILE processing RO
		Receive MIME header Content-Type
		Pass to CAF with RegisterL(Content-Type)
		IF Registered()
			Add MIME header metadata to ROCHCAFInterface class using AddToMetaDataL(metadata)
		ENDIF
		Detect start of MIME data section
		If CAF Registered()
			//BEGIN CAF session
			PrepareProcessingL()
			Create file handle for attachment write
			GetSuggestedAttachmentFileName(filename)
			StartProcessing(filename,filepath,file handle)
			WriteData() // Undecoded 1-N buffers in state machine
			Detect end of MIME data section
			WriteDataComplete()
			EndProcessingL()
			//END CAF Session
		ELSE
			Default behaviour
		ENDIF
	ENDWHILE
END
@endcode
@internalComponent
*/
class CROCHCAFInterface	:	public CBase
	{
public:
	IMPORT_C			CROCHCAFInterface(RFs& aFs);
	IMPORT_C virtual	~CROCHCAFInterface();
	IMPORT_C void		RegisterL(const TDesC8& aMimeLine);
	IMPORT_C void		Deregister();
	IMPORT_C void		AddToMetaDataL(const TDesC8& aMimeLine);
	IMPORT_C void		AddToMetaDataL(const TDesC8& aField, const TDesC8& aData);
	IMPORT_C void		EndProcessingL();
	IMPORT_C void 		PrepareProcessingL();
	IMPORT_C void		WriteDataL(const TDesC8& aData);
	IMPORT_C TBool		Registered() const;
	IMPORT_C TBool		Processing() const;
	IMPORT_C TInt		GetSuggestedAttachmentFileName(TDes& aFileName) const;
	IMPORT_C void		StartProcessing(const TDesC& aDefaultAttachmentFileName,const TDesC& aAttachmentFilePath,RFile& aStartFile);

private:		
	void 				NewFileL();
	void				WriteDataCompleteL();
private:
	/** A CAF class instance. Initialised prior to data write */
	CImportFile*			iImportFile;
	/** A CAF class instance. Used to register Content-type with CAF */
	CSupplier*				iSupplier;
	/** Store for the MIME Content-type */
	HBufC8*					iContentType;
	/** A CAF class instance. Stores MIME header attribute/value pairs. Appended by AddToMetaData() */
	CMetaDataArray*			iMetaData;
	/** Stores the filepath for the current CAF session. Initialised by StartProcessing()*/
	TBuf<KMaxPath>			iAttachmentFilePath;
	/** If open, handle is used for the next attachment file required by CAF. Initialised by StartProcessing() */
	RFile					iAttachmentFile;
	/** The localised default filename . Initialised by StartProcessing() */
	TPtrC					iDefaultAttachmentFileName;
	/** Reference to instantiators file server session */
	RFs&					iFsSession;
	
	};

#endif // ROCHCAFInterface_H
