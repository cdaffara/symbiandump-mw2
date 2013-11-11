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
//

#include  <push/sislpushmsgutils.h>

#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"
#include <caf/rightsinfo.h>

_LIT(KTestAgent, "Reference Test Agent");
_LIT(KTestPassed,"DRM Test passed.Number of rights stored :\t%d expected rights : %d");
_LIT(KTestFailed, "DRM Test failed.Number of rights stored :\t%d expected rights : %d ");

_LIT8 ( KWBXmlROMessage, "\x03\x0E\x6A\x00\xC5\x05\x85\x06\x86\x07\x87\x01\x46\x47\x03\x31\x2E\x30\x00\x01\01\x49\x4A\x46\x48\x03\x63\x69\x64\x3A\x74\x65\x73\x74\x40\x66\x65\x72\x2E\x68\x72\x00\x01\x01\x4B\x4C\xC3\x10\xbd\x41\x30\x47\xc2\xf3\x10\x9a\x1e\x88\x2f\x9d\x81\x3d\x66\x82\x01\x01\x01\x4D\x4E\x52\x53\x03\x31\x00\x01\x01\x52\x54\x55\x03\x32\x30\x30\x36\x2D\x30\x33\x2D\x30\x31\x54\x30\x30\x3A\x30\x30\x3A\x30\x30\x00\x01\x56\x03\x32\x30\x30\x36\x2D\x30\x34\x2D\x33\x30\x54\x30\x30\x3A\x30\x30\x3A\x30\x30\x00\x01\x01\x01\x52\x57\x03\x50\x30\x59\x30\x4D\x33\x30\x44\x54\x31\x30\x48\x33\x30\x4D\x30\x53\x00\x01\x01\x01\x01\x01\x01" );
 
//The rights element attribute is invalid.
_LIT8 ( KInvalidWBXmlROMessage, "\x03\x0E\x6A\x00\xC5\x15\x85\x06\x86\x01\x46\x47\x03\x31\x2E\x30\x00\x01\01\x49\x4A\x46\x48\x01\x01\x01\x4D\x01\x01\x01" );

//Xml RO Response message with Rights Object.
_LIT8 ( KXmlROMessage, "<roap:roResponse xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" status=\"Success\"><deviceId><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>vXENc+Um/9/NvmYKiHDLaErK0gk=</hash></keyIdentifier></deviceId><riID><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>vXENc+Um/9/NvmYKiHDLaErK0gk=</hash></keyIdentifier></riID><nonce>32efd34de39sdwefqwer</nonce><roap:protectedRO><roap:ro id=\"n8yu98hy0e2109eu09ewf09u\" stateful=\"true\" version=\"1.0\"><o-ex:rights o-ex=\"http://odrl.net/1.1/ODRL-EX\" o-dd=\"http://odrl.net/1.1/ODRL-DD\" ds=\"http://www.w3.org/2000/09/xmldsig#/\"><o-ex:context><o-dd:version>1.0</o-dd:version></o-ex:context><o-ex:agreement><o-ex:asset><o-ex:context><o-dd:uid>cid:4567829547@foo.com</o-dd:uid></o-ex:context><ds:KeyInfo><ds:KeyValue>vUEwR8LzEJoeiC+dgT1mgg==</ds:KeyValue></ds:KeyInfo></o-ex:asset><o-ex:permission><o-dd:play/></o-ex:permission></o-ex:agreement></o-ex:rights></roap:ro></roap:protectedRO><signature>d93e5fue3susdskjhkjedkjrewh53209efoihfdse10ue2109ue1</signature></roap:roResponse>");

//</o-ex:context> is not present in the message after </uid>
_LIT8 ( KInValidXmlROMessage, "<roap:roResponse xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" status=\"Success\"><deviceId><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>vXENc+Um/9/NvmYKiHDLaErK0gk=</hash></keyIdentifier></deviceId><riID><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>vXENc+Um/9/NvmYKiHDLaErK0gk=</hash></keyIdentifier></riID><nonce>32efd34de39sdwefqwer</nonce><roap:protectedRO><roap:ro id=\"n8yu98hy0e2109eu09ewf09u\" stateful=\"true\" version=\"1.0\"><o-ex:rights o-ex=\"http://odrl.net/1.1/ODRL-EX\" o-dd=\"http://odrl.net/1.1/ODRL-DD\" ds=\"http://www.w3.org/2000/09/xmldsig#/\"><o-ex:context><o-dd:version>1.0</o-dd:version></o-ex:context><o-ex:agreement><o-ex:asset><o-ex:context><o-dd:uid>cid:4567829547@foo.com</o-dd:uid><ds:KeyInfo><ds:KeyValue>vUEwR8LzEJoeiC+dgT1mgg==</ds:KeyValue></ds:KeyInfo></o-ex:asset><o-ex:permission><o-dd:play/></o-ex:permission></o-ex:agreement></o-ex:rights></roap:ro></roap:protectedRO><signature>d93e5fue3susdskjhkjedkjrewh53209efoihfdse10ue2109ue1</signature></roap:roResponse>" );

//Trigger message for registration request.
_LIT8 ( KXmlRegReqTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" version=\"1.0\"><registrationRequest id=\"id1\"><riID><keyIdentifier xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\" xsi:type=\"roap:X509SPKIHash\"><hash>I94/ovDhizE5m/K6t5f1holSdLM=</hash></keyIdentifier></riID><riAlias>CoreMedia IOPServer</riAlias><nonce>FL2MM70k5OthJZrMte6r</nonce><roapURL>http://192.116.217.184:8080/iop-server/roap.cm;jsessionid=88C45AEB82C2755A641526A19B9050A7</roapURL></registrationRequest></roap:roapTrigger>");

//Trigger message for RO Aquisation.
_LIT8 ( KXmlROAquisationTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" version=\"1.0\"><roAcquisition id=\"id1\"><riID><keyIdentifier xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\" xsi:type=\"roap:X509SPKIHash\"><hash>I94/ovDhizE5m/K6t5f1holSdLM=</hash></keyIdentifier></riID><riAlias>CoreMedia IOP Test Server</riAlias><nonce>g5VsnV4L7qZzBMnH0idD</nonce><roapURL>http://192.116.217.184:8080/iop-server/roap.cm</roapURL><roID>offer6165.1.PphdCI0gysdL58zb_mid</roID><contentID>cid:cid6165_mid</contentID></roAcquisition></roap:roapTrigger>");

//Trigger message for Join Domain.
_LIT8 ( KXmlJoinDomainTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\"	version=\"1.0\"><joinDomain id=\"de32r23r4\"><riID><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>aXENc+Um/9/NvmYKiHDLaErK0fk=</hash></keyIdentifier></riID><nonce>sdfknjvfda438790fdjkl4rq</nonce><roapURL>http://ri.example.com/ro.cgi?tid=qw683hgew7d</roapURL><domainID>Domain-XYZ-001</domainID></joinDomain><signature>SomeSignature</signature></roap:roapTrigger>");

//Trigger message for Leave Domain.
_LIT8 ( KXmlLeaveDomainTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\"	version=\"1.0\"><leaveDomain id=\"de32r23r4\"><riID><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>aXENc+Um/9/NvmYKiHDLaErK0fk=</hash></keyIdentifier></riID><nonce>sdfknjvfda438790fdjkl4rq</nonce><roapURL>http://ri.example.com/ro.cgi?tid=qw683hgew7d</roapURL><domainID>Domain-XYZ-001</domainID></leaveDomain><signature>SomeSignature</signature></roap:roapTrigger>");

//Invalid Trigger message for registration request. </hash> is absent.
_LIT8 ( KInvalidXmlRegReqTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" version=\"1.0\"><registrationRequest id=\"id1\"><riID><keyIdentifier xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\" xsi:type=\"roap:X509SPKIHash\"><hash>I94/ovDhizE5m/K6t5f1holSdLM=</keyIdentifier></riID><riAlias>CoreMedia IOPServer</riAlias><nonce>FL2MM70k5OthJZrMte6r</nonce><roapURL>http://192.116.217.184:8080/iop-server/roap.cm;jsessionid=88C45AEB82C2755A641526A19B9050A7</roapURL></registrationRequest></roap:roapTrigger>");

//Invalid Trigger message for RO Aquisation. </hash> is called after </KeyIdentifier>.
_LIT8 ( KInvalidXmlROAquisationTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" version=\"1.0\"><roAcquisition id=\"id1\"><riID><keyIdentifier xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" algorithm=\"http://www.w3.org/2000/09/xmldsig#sha1\" xsi:type=\"roap:X509SPKIHash\"><hash>I94/ovDhizE5m/K6t5f1holSdLM=</keyIdentifier></hash></riID><riAlias>CoreMedia IOP Test Server</riAlias><nonce>g5VsnV4L7qZzBMnH0idD</nonce><roapURL>http://192.116.217.184:8080/iop-server/roap.cm</roapURL><roID>offer6165.1.PphdCI0gysdL58zb_mid</roID><contentID>cid:cid6165_mid</contentID></roAcquisition></roap:roapTrigger>");

//Invalid Trigger message for Join Domain. </riID> is Absent.
_LIT8 ( KInvalidXmlJoinDomainTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\"	version=\"1.0\"><joinDomain id=\"de32r23r4\"><riID><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>aXENc+Um/9/NvmYKiHDLaErK0fk=</hash></keyIdentifier><nonce>sdfknjvfda438790fdjkl4rq</nonce><roapURL>http://ri.example.com/ro.cgi?tid=qw683hgew7d</roapURL><domainID>Domain-XYZ-001</domainID></joinDomain><signature>SomeSignature</signature></roap:roapTrigger>");

//Invalid Trigger message for Leave Domain.</leaveDomain> is absent.
_LIT8 ( KInvalidXmlLeaveDomainTriggerMessage, "<roap:roapTrigger xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\"	version=\"1.0\"><leaveDomain id=\"de32r23r4\"><riID><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>aXENc+Um/9/NvmYKiHDLaErK0fk=</hash></keyIdentifier></riID><nonce>sdfknjvfda438790fdjkl4rq</nonce><roapURL>http://ri.example.com/ro.cgi?tid=qw683hgew7d</roapURL><domainID>Domain-XYZ-001</domainID><signature>SomeSignature</signature></roap:roapTrigger>");

//The CID of this message is more than 128 characters.
_LIT8 ( KLargeXmlROMessage, "<roap:roResponse xmlns:roap=\"urn:oma:bac:dldrm:roap-1.0\" status=\"Success\"><deviceId><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>vXENc+Um/9/NvmYKiHDLaErK0gk=</hash></keyIdentifier></deviceId><riID><keyIdentifier xsi:type=\"roap:X509SPKIHash\"><hash>vXENc+Um/9/NvmYKiHDLaErK0gk=</hash></keyIdentifier></riID><nonce>32efd34de39sdwefqwer</nonce><roap:protectedRO><roap:ro id=\"n8yu98hy0e2109eu09ewf09u\" stateful=\"true\" version=\"1.0\"><o-ex:rights o-ex=\"http://odrl.net/1.1/ODRL-EX\" o-dd=\"http://odrl.net/1.1/ODRL-DD\" ds=\"http://www.w3.org/2000/09/xmldsig#/\"><o-ex:context><o-dd:version>1.0</o-dd:version></o-ex:context><o-ex:agreement><o-ex:asset><o-ex:context><o-dd:uid>cid:TheMissionOfTheOpenMobileAllianceIsToFacilitateGlobalUserAdoptionOfMobileDataServicesBySpecifyingMarketDrivenMobileServiceEnablersThatEnsureServiceInteroperabilityAcrossDevicesGeographiesServiceProvidersOperatorsAndNetwork</o-dd:uid></o-ex:context><ds:KeyInfo><ds:KeyValue>vUEwR8LzEJoeiC+dgT1mgg==</ds:KeyValue></ds:KeyInfo></o-ex:asset><o-ex:permission><o-dd:play/></o-ex:permission></o-ex:agreement></o-ex:rights></roap:ro></roap:protectedRO><signature>d93e5fue3susdskjhkjedkjrewh53209efoihfdse10ue2109ue1</signature></roap:roResponse>" );

//The CID of this message is more than 128 characters.
_LIT8 ( KLargeWBXmlROMessage, "\x03\x0E\x6A\x00\xC5\x05\x85\x06\x86\x01\x46\x47\x03\x31\x2E\x30\x00\x01\01\x49\x4A\x46\x48\x03\x63\x69\x64\x3A\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x73\x6F\x6D\x65\x64\x61\x74\x61\x00\x01\x01\x01\x4D\x01\x01\x01" );

const TInt KTotalRights=4;	//Change it when rights written to rights.db changes from the test code.
const TUint8 KWbXmlROMessageHeader[] = 
	{
	0xCB,  //Content type "application/vnd.oma.drm.rights+wbxml"
	0xAF,	//X-Wap-Application-ID
	0x88	// App type x-wap-application:drm.ua   //0x08
	};	
	
const TUint8 KXmlROMessageHeader[] = 
	{
	0xCA,  	//Content type "application/vnd.oma.drm.rights+xml"
	0xAF,	//X-Wap-Application-ID
	0x88	// App type x-wap-application:drm.ua   //0x08
	};	

//Commented as WbXml trigger messages are not supported currently.
/*const TUint8 KWbXmlTriggerMessageHeader[] = 
	{
	0xCD,  //application/vnd.oma.drm.roap-trigger+wbxml"
	0xAF,	//X-Wap-Application-ID
	0x88	// App type x-wap-application:drm.ua   //0x08
	};*/	
	
const TUint8 KXmlTriggerMessageHeader[] = 
	{
	0xCC,  //Content type "application/vnd.oma.drm.roap-trigger+xml"
	0xAF,	//X-Wap-Application-ID
	0x88	// App type x-wap-application:drm.ua   //0x08
	};	
	
/** set the test name for this test case
	@param void
	@return string
		returns test name
*/
const TDesC& CWapPushDRMMessageTest::TestName()
	{
	_LIT(KTextDRMMessageTest,"DRM Test");
	return KTextDRMMessageTest;
	}

/** specify the number of test messages to use */
void CWapPushDRMMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 14;
	}


/** create test messages to use where aTestCase represents the
	message index for multiple messages.

	@param aTestCase message index value for selecting a test message
	@return void
 */
void CWapPushDRMMessageTest::PrepareTestMessageL(TInt aTestCase)
	{
	// create hardcoded DRM test message.
	iMsg = NULL;
	switch ( aTestCase )
		{
	case 0 :
		//Pass WBXml RO message.
		WbXmlROMessageL();
		break;
	case 1:
		//Pass invalid WBXML RO message
		InvalidWbXmlROMessageL();
		break;
	case 2 :
		//Pass XML RO message.
		XmlROMessageL();
		break;
	case 3 :
		//Pass invalid XML RO message.
		InvalidXmlROMessageL();
		break;
	case 4:
		//Pass XML Registration request Trigger message.
		RegRequesttriggerMessageL();
		break;
	case 5:
		//Pass XML RO Aquisation Trigger message.
		ROAquisationTriggerMessageL();
		break;
	case 6:
		//Pass XML Join Domain Trigger message.
		JoinDomaintriggerMessageL();
		break;
	case 7:
		//Pass XML Leave Domain Trigger message.
		LeaveDomainTriggerMessageL();
		break;
	case 8:
		//Pass invalid XML Registration Request Trigger message.
		InvalidRegRequesttriggerMessageL();
		break;
	case 9 :
		//Pass invalid XML RO Aquisation Trigger message.
		InvalidROAquisationTriggerMessageL();
		break;
	case 10 :
		//Pass invalid XML Join Domain Trigger message.
		InvalidJoinDomaintriggerMessageL();
		break;
	case 11 :
		//Pass invalid XML Leave Domain Trigger message.
		InvalidLeaveDomainTriggerMessageL();
		break;
	case 12 :
		//Pass Large XML RO message. Cid>128
		LargeXmlROMessageL();
		break;
	case 13:
		//Pass Large WBXML RO message. Cid>128
		LargeWbXmlROMessageL();
		break;
	default :
		return;	
		}
	
	// copy data to iBodyBuf
	iBodyBuf.Delete(0,KBufferSize);
	iBodyBuf = iMsg->Des();
	delete iMsg;
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0001
	@SYMTestCaseDesc Test the DRM ROAP ROResponse push message delivery (XML format)
					 to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a XML Push Message using the Dummy stack, and see that
					the CDRMAppHandler and CROContentHandler are able to correctly store 
					the Rights in rights.db using test agent.
	@SYMTestExpectedResults	Test agent should write the rights data to rights.db.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::XmlROMessageL()
	{
	iMsg = KXmlROMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlROMessageHeader, sizeof(KXmlROMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0002
	@SYMTestCaseDesc Test the DRM push message delivery (WBXML format) to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a WBXML DRM Push Message using the Dummy stack, and see that
					the CDRMAppHandler and CROContentHandler are able to correctly store
					the Rights in rights.db using test agent.
	@SYMTestExpectedResults	Test agent should add the rights to rights.db.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::WbXmlROMessageL()
	{
	iMsg = KWBXmlROMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KWbXmlROMessageHeader, sizeof(KWbXmlROMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0003
	@SYMTestCaseDesc Test the DRM Trigger message delivery to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a valid DRM Push Message containing Registration Request trigger 
					using the Dummy stack, and see that the CDRMAppHandler and 
					CROAPTContentHandler are able to correctly parse the message using test agent.
	@SYMTestExpectedResults	Test agent should parse the message without any error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::RegRequesttriggerMessageL()
	{
	iMsg = KXmlRegReqTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0004
	@SYMTestCaseDesc Test the DRM Trigger message delivery to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a valid DRM Push Message containing RO acquisition trigger 
					using the Dummy stack, and see that the CDRMAppHandler and 
					CROAPTContentHandler are able to correctly parse the message using test agent.
	@SYMTestExpectedResults	Test agent should parse the message without any error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::ROAquisationTriggerMessageL()
	{
	iMsg = KXmlROAquisationTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0005
	@SYMTestCaseDesc Test the DRM Trigger message delivery to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a valid DRM Push Message containing Join Domain trigger using 
					the Dummy stack, and see that the CDRMAppHandler and CROAPTContentHandler 
					are able to correctly parse the message using test agent.
	@SYMTestExpectedResults	Test agent should parse the message without any error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::JoinDomaintriggerMessageL()
	{
	iMsg = KXmlJoinDomainTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0006
	@SYMTestCaseDesc Test the DRM Trigger message delivery to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a valid DRM Push Message containing Leave Domain trigger using 
					the Dummy stack, and see that the CDRMAppHandler and CROAPTContentHandler 
					are able to correctly parse the message using test agent.
	@SYMTestExpectedResults	Test agent should parse the message without any error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::LeaveDomainTriggerMessageL()
	{
	iMsg = KXmlLeaveDomainTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0007
	@SYMTestCaseDesc Test the Large DRM push message delivery (WBXML format) to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a large WBXML DRM Push Message with CID >128 using the Dummy stack, 
					and see that the CDRMAppHandler and CROContentHandler are able to correctly 
					store the Rights Object in the rights.db file using test agent
	@SYMTestExpectedResults	Test agent should add the large rights to rights.db.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::LargeWbXmlROMessageL()
	{
	iMsg = KLargeWBXmlROMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KWbXmlROMessageHeader, sizeof(KWbXmlROMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BV-0008
	@SYMTestCaseDesc Test the Large DRM push message delivery (XML format) to test agent.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send a large XML DRM Push Message with CID >128 using the Dummy stack, 
					and see that the CDRMAppHandler and CROContentHandler are able to correctly 
					store the Rights Object in the rights.db file using test agent
	@SYMTestExpectedResults	Test agent should add the large rights to rights.db.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::LargeXmlROMessageL()
	{
	iMsg = KLargeXmlROMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlROMessageHeader, sizeof(KXmlROMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BI-0009
	@SYMTestCaseDesc Test content handler reports appropriate error if WBXML tag's received 
					 are not found in string table.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send invalid DRM RO Message to test agent in WBXML format using dummy stack.
	@SYMTestExpectedResults	The ROContentHandler should report KErrCorrupt error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::InvalidWbXmlROMessageL()
	{
	iMsg = KInvalidWBXmlROMessage().AllocL();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KWbXmlROMessageHeader, sizeof(KWbXmlROMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BI-0010
	@SYMTestCaseDesc Test content handler reports appropriate error if the agent cannot 
					 Parse/Validate the XML message.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send invalid DRM RO Message to test agent in XML format using dummy stack.
	@SYMTestExpectedResults	The ROContentHandler should report KErrCorrupt error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::InvalidXmlROMessageL()
	{
	iMsg = KInValidXmlROMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlROMessageHeader, sizeof(KXmlROMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BI-0011
	@SYMTestCaseDesc Test content handler reports appropriate error if the agent cannot 
					 Parse/Validate the XML message.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send invalid DRM Registration Request Trigger Message in XML format 
					using dummy stack.
	@SYMTestExpectedResults	The ROAPTContentHandler should report KErrCorrupt error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::InvalidRegRequesttriggerMessageL()
	{
	iMsg = KInvalidXmlRegReqTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BI-0012
	@SYMTestCaseDesc Test content handler reports appropriate error if the agent cannot 
					 Parse/Validate the XML message.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send invalid DRM RO Acquisition Trigger Message in XML format 
					using dummy stack.
	@SYMTestExpectedResults	The ROAPTContentHandler should report KErrCorrupt error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::InvalidROAquisationTriggerMessageL()
	{
	iMsg = KInvalidXmlROAquisationTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BI-0013
	@SYMTestCaseDesc Test content handler reports appropriate error if the agent cannot 
					 Parse/Validate the XML message.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send invalid DRM Join Domain Trigger Message in XML format using dummy stack.
	@SYMTestExpectedResults	The ROAPTContentHandler should report KErrCorrupt error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::InvalidJoinDomaintriggerMessageL()
	{
	iMsg = KInvalidXmlJoinDomainTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}
	
/*	@SYMTestCaseID WP-CIT-BI-0014
	@SYMTestCaseDesc Test content handler reports appropriate error if the agent cannot 
					 Parse/Validate the XML message.
	@SYMTestStatus Implemented
	@SYMTestPriority 1
	@SYMTestActions Send invalid DRM Leave Domain Trigger Message in XML format using dummy stack.
	@SYMTestExpectedResults	The ROAPTContentHandler should report KErrCorrupt error.
	@SYMPREQ PREQ GT257-CR763
	@SYMTestType CIT
*/
void CWapPushDRMMessageTest::InvalidLeaveDomainTriggerMessageL()
	{
	iMsg = KInvalidXmlLeaveDomainTriggerMessage().AllocL ();
	iHeadersBuf.Delete(0,KBufferSize);
	iHeadersBuf.Append(KXmlTriggerMessageHeader, sizeof(KXmlTriggerMessageHeader));
	}

TBool CWapPushDRMMessageTest::ConfirmMessagesSavedL()
	{
	using namespace ContentAccess;
	CRightsManager *rightsManager=NULL;
	RStreamablePtrArray <CRightsInfo> rights;
	//Create the manager object.
	CManager *manager = CManager::NewL();
	CleanupStack::PushL(manager);
	//Now list out all registered agents 
	RArray <TAgent> agents;
	TRAPD(err, manager->ListAgentsL(agents));
	if(err==KErrNoMemory)
		{
		User::LeaveNoMemory();
		}
	CleanupClosePushL(agents);
	if(err==KErrNone && agents.Count()>0)
		{
		for(TInt cnt=0; cnt<agents.Count(); cnt++)
			{
			if(agents[cnt].Name().CompareF(KTestAgent)==0)
				{
				TRAP(err, rightsManager = manager->CreateRightsManagerL(agents[0]));
				CleanupStack::PushL(rightsManager);
				rightsManager->ListAllRightsL(rights);
				TInt rightsCount = rights[0]->RightsType();
				TBuf<KPushLogBuffer> buf;
				if(rightsCount==KTotalRights)
					{
					buf.Format(KTestPassed,rightsCount,KTotalRights);
					WPLPrintf(buf);	
					}
				else
					{
					buf.Format(KTestFailed,rightsCount,KTotalRights);
					WPLPrintf(buf);	
					}
				rights.Close();
				CleanupStack::PopAndDestroy(1);	//rightsManager
				}
			}
		}
	CleanupStack::PopAndDestroy(2); //agents, manager
	return ETrue;
	}

