#include "unittest.h"
#include "boost/asio.hpp"
#include "OpcUaStackCore/ServiceSet/RegisterNodesRequest.h"
#include "OpcUaStackCore/ServiceSet/RegisterNodesResponse.h"
#include "OpcUaStackCore/SecureChannel/MessageHeader.h"
#include "OpcUaStackCore/SecureChannel/SequenceHeader.h"
#include "OpcUaStackCore/Base/Utility.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaIdentifier.h"

#include <streambuf>
#include <iostream>

using namespace OpcUaStackCore;

BOOST_AUTO_TEST_SUITE(RegisterNodes_)

BOOST_AUTO_TEST_CASE(RegisterNodes_Title)
{
	std::cout << "RegisterNodes_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(RegisterNodes_Request)
{
	std::string str;
	uint32_t pos;
	OpcUaNodeId typeId;
	OpcUaGuid::SPtr opcUaGuidSPtr;
	OpcUaNodeId::SPtr nodeIdSPtr;
	RegisterNodesRequest::SPtr registerNodesRequestSPtr;
	MessageHeader::SPtr messageHeaderSPtr;
	SequenceHeader::SPtr sequenceHeaderSPtr;
	boost::posix_time::ptime ptime = boost::posix_time::from_iso_string("16010101T120000.000000000");
	
	// stream
	boost::asio::streambuf sb1;
	std::iostream ios1(&sb1);
	boost::asio::streambuf sb2;
	std::iostream ios2(&sb2);
	boost::asio::streambuf sb;
	std::iostream ios(&sb);

	// encode security header
	OpcUaInt32 secureChannelId;
	OpcUaInt32 secureTokenId;

	secureChannelId = 153451225;
	secureTokenId = 1;

	OpcUaNumber::opcUaBinaryEncode(ios1, secureChannelId);
	OpcUaNumber::opcUaBinaryEncode(ios1, secureTokenId);

	// encode sequence header
	sequenceHeaderSPtr = SequenceHeader::construct();
	sequenceHeaderSPtr->sequenceNumber(54);
	sequenceHeaderSPtr->requestId(4);
	sequenceHeaderSPtr->opcUaBinaryEncode(ios1);

	// encode TypeId
	typeId.nodeId(OpcUaId_RegisterNodesRequest_Encoding_DefaultBinary);
	typeId.opcUaBinaryEncode(ios1);

	// build
	registerNodesRequestSPtr = RegisterNodesRequest::construct();

	// build RequestHeader
	opcUaGuidSPtr = OpcUaGuid::construct();
	*opcUaGuidSPtr = "0D4455B2-8D2F-B74F-864F-0AF5945DD833";
	
	registerNodesRequestSPtr->requestHeader()->sessionAuthenticationToken().namespaceIndex(1);
	registerNodesRequestSPtr->requestHeader()->sessionAuthenticationToken().nodeId(opcUaGuidSPtr);
	registerNodesRequestSPtr->requestHeader()->time(ptime);
	registerNodesRequestSPtr->requestHeader()->requestHandle(0);
	registerNodesRequestSPtr->requestHeader()->returnDisagnostics(0);
	registerNodesRequestSPtr->requestHeader()->timeoutHint(300000);

	// build Parameter
	nodeIdSPtr = OpcUaNodeId::construct();
	nodeIdSPtr->namespaceIndex(2);
	nodeIdSPtr->nodeId<OpcUaUInt32>(123);

	registerNodesRequestSPtr->nodesToRegister()->set(nodeIdSPtr);

	// encode 
	registerNodesRequestSPtr->opcUaBinaryEncode(ios1);

	// encode MessageHeader
	messageHeaderSPtr = MessageHeader::construct();
	messageHeaderSPtr->messageType(MessageType_Message);
	messageHeaderSPtr->messageSize(OpcUaStackCore::count(sb1)+8);
	messageHeaderSPtr->opcUaBinaryEncode(ios2);

	// stream
	ios << ios2.rdbuf() << ios1.rdbuf();
	OpcUaStackCore::dumpHex(ios);
	
	std::stringstream ss;
	ss << "4d 53 47 46 52 00 00 00  d9 7a 25 09 01 00 00 00" 
	   << "36 00 00 00 04 00 00 00  01 00 30 02 04 01 00 0d"
	   << "44 55 b2 8d 2f b7 4f 86  4f 0a f5 94 5d d8 33 00"
	   << "00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 ff"
       << "ff ff ff e0 93 04 00 00  00 00 01 00 00 00 01 02"
       << "7b 00";

	BOOST_REQUIRE(OpcUaStackCore::compare(ios, ss.str(), pos) == true);

	// decode MessageHeader
	messageHeaderSPtr = MessageHeader::construct();
	messageHeaderSPtr->opcUaBinaryDecode(ios);
	BOOST_REQUIRE(messageHeaderSPtr->messageType() == MessageType_Message);

	// decode security header
	OpcUaNumber::opcUaBinaryDecode(ios, secureChannelId);
	BOOST_REQUIRE(secureChannelId == 153451225);
	OpcUaNumber::opcUaBinaryDecode(ios, secureTokenId);
	BOOST_REQUIRE(secureTokenId == 1);

	// decode sequence header
	sequenceHeaderSPtr = SequenceHeader::construct();
	sequenceHeaderSPtr->opcUaBinaryDecode(ios);
	BOOST_REQUIRE(sequenceHeaderSPtr->sequenceNumber() == 54);
	BOOST_REQUIRE(sequenceHeaderSPtr->requestId() == 4);

	// decode message type id
	typeId.opcUaBinaryDecode(ios);
	BOOST_REQUIRE(typeId.namespaceIndex() == 0);
	BOOST_REQUIRE(typeId.nodeId<OpcUaUInt32>() == OpcUaId_RegisterNodesRequest_Encoding_DefaultBinary);

	// decode 
	registerNodesRequestSPtr = RegisterNodesRequest::construct();
	registerNodesRequestSPtr->opcUaBinaryDecode(ios);

	str = *registerNodesRequestSPtr->requestHeader()->sessionAuthenticationToken().nodeId<OpcUaGuid::SPtr>();
	BOOST_REQUIRE(registerNodesRequestSPtr->requestHeader()->sessionAuthenticationToken().namespaceIndex() == 1);
	BOOST_REQUIRE(str == "0D4455B2-8D2F-B74F-864F-0AF5945DD833");
	BOOST_REQUIRE(registerNodesRequestSPtr->requestHeader()->time().dateTime() == ptime);
	BOOST_REQUIRE(registerNodesRequestSPtr->requestHeader()->requestHandle() == 0);
	BOOST_REQUIRE(registerNodesRequestSPtr->requestHeader()->returnDisagnostics() == 0);
	BOOST_REQUIRE(registerNodesRequestSPtr->requestHeader()->timeoutHint() == 300000);
	
	BOOST_REQUIRE(registerNodesRequestSPtr->nodesToRegister()->size() == 1);
	nodeIdSPtr = OpcUaNodeId::construct();
	registerNodesRequestSPtr->nodesToRegister()->get(nodeIdSPtr);
	BOOST_REQUIRE(nodeIdSPtr->namespaceIndex() == 2);
	BOOST_REQUIRE(nodeIdSPtr->nodeId<OpcUaUInt32>() == 123);
}


BOOST_AUTO_TEST_CASE(RegisterNodes_Response)
{
	uint32_t pos;
	std::string str;
	OpcUaNodeId typeId;
	OpcUaQualifiedName browseName;
	OpcUaLocalizedText displayName;
	OpcUaStatusCode statusCode;
	OpcUaNodeId::SPtr nodeIdSPtr;
	RegisterNodesResponse::SPtr registerNodesResponseSPtr;
	MessageHeader::SPtr messageHeaderSPtr;
	SequenceHeader::SPtr sequenceHeaderSPtr;
	boost::posix_time::ptime ptime;
	
	// test-time
	ptime = boost::posix_time::from_iso_string("16010101T120000.000000000");
	
	// stream
	boost::asio::streambuf sb1;
	std::iostream ios1(&sb1);
	boost::asio::streambuf sb2;
	std::iostream ios2(&sb2);
	boost::asio::streambuf sb;
	std::iostream ios(&sb);

	// encode security header
	OpcUaInt32 secureChannelId;
	OpcUaInt32 secureTokenId;

	secureChannelId = 153451225;
	secureTokenId = 1;

	OpcUaNumber::opcUaBinaryEncode(ios1, secureChannelId);
	OpcUaNumber::opcUaBinaryEncode(ios1, secureTokenId);

	// encode sequence header
	sequenceHeaderSPtr = SequenceHeader::construct();
	sequenceHeaderSPtr->sequenceNumber(54);
	sequenceHeaderSPtr->requestId(4);
	sequenceHeaderSPtr->opcUaBinaryEncode(ios1);

	// encode TypeId
	typeId.nodeId(OpcUaId_RegisterNodesResponse_Encoding_DefaultBinary);
	typeId.opcUaBinaryEncode(ios1);

	// build 
	registerNodesResponseSPtr = RegisterNodesResponse::construct();

	// build ResponseHeader
	statusCode = Success;
	registerNodesResponseSPtr->responseHeader()->time(ptime);
	registerNodesResponseSPtr->responseHeader()->requestHandle(0);
	registerNodesResponseSPtr->responseHeader()->serviceResult(statusCode);
	
	// build Parameter
	nodeIdSPtr = OpcUaNodeId::construct();
	nodeIdSPtr->namespaceIndex(2);
	nodeIdSPtr->nodeId<OpcUaUInt32>(123);

	registerNodesResponseSPtr->registeredNodeIds()->set(nodeIdSPtr);

	// encode 
	registerNodesResponseSPtr->opcUaBinaryEncode(ios1);

	// encode MessageHeader
	messageHeaderSPtr = MessageHeader::construct();
	messageHeaderSPtr->messageType(MessageType_Message);
	messageHeaderSPtr->messageSize(OpcUaStackCore::count(sb1)+8);
	messageHeaderSPtr->opcUaBinaryEncode(ios2);

	// stream
	ios << ios2.rdbuf() << ios1.rdbuf();
	OpcUaStackCore::dumpHex(ios);

	std::stringstream ss;
	ss << "4d 53 47 46 3c 00 00 00  d9 7a 25 09 01 00 00 00"
	   << "36 00 00 00 04 00 00 00  01 00 33 02 00 00 00 00"
	   << "00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00"
	   << "00 00 00 00 01 00 00 00  01 02 7b 00";

	BOOST_REQUIRE(OpcUaStackCore::compare(ios, ss.str(), pos) == true);

	// decode MessageHeader
	messageHeaderSPtr = MessageHeader::construct();
	messageHeaderSPtr->opcUaBinaryDecode(ios);
	BOOST_REQUIRE(messageHeaderSPtr->messageType() == MessageType_Message);

	// decode security header
	OpcUaNumber::opcUaBinaryDecode(ios, secureChannelId);
	BOOST_REQUIRE(secureChannelId == 153451225);
	OpcUaNumber::opcUaBinaryDecode(ios, secureTokenId);
	BOOST_REQUIRE(secureTokenId == 1);

	// decode sequence header
	sequenceHeaderSPtr = SequenceHeader::construct();
	sequenceHeaderSPtr->opcUaBinaryDecode(ios);
	BOOST_REQUIRE(sequenceHeaderSPtr->sequenceNumber() == 54);
	BOOST_REQUIRE(sequenceHeaderSPtr->requestId() == 4);

	// decode message type id
	typeId.opcUaBinaryDecode(ios);
	BOOST_REQUIRE(typeId.namespaceIndex() == 0);
	BOOST_REQUIRE(typeId.nodeId<OpcUaUInt32>() == OpcUaId_RegisterNodesResponse_Encoding_DefaultBinary);

	// decode 
	registerNodesResponseSPtr = RegisterNodesResponse::construct();
	registerNodesResponseSPtr->opcUaBinaryDecode(ios);

	BOOST_REQUIRE(registerNodesResponseSPtr->responseHeader()->time().dateTime() == ptime);
	BOOST_REQUIRE(registerNodesResponseSPtr->responseHeader()->requestHandle() == 0);
	BOOST_REQUIRE(registerNodesResponseSPtr->responseHeader()->serviceResult() == Success);

	BOOST_REQUIRE(registerNodesResponseSPtr->registeredNodeIds()->size() == 1);
	nodeIdSPtr = OpcUaNodeId::construct();
	registerNodesResponseSPtr->registeredNodeIds()->get(nodeIdSPtr);
	BOOST_REQUIRE(nodeIdSPtr->namespaceIndex() == 2);
	BOOST_REQUIRE(nodeIdSPtr->nodeId<OpcUaUInt32>() == 123);
}

BOOST_AUTO_TEST_SUITE_END()
