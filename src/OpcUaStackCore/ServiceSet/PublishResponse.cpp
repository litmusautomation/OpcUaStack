#include "OpcUaStackCore/ServiceSet/PublishResponse.h"

namespace OpcUaStackCore
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// OpcUa PublishResponse
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	PublishResponse::PublishResponse(void)
	: ObjectPool<PublishResponse>()
	, responseHeaderSPtr_(ResponseHeader::construct())
	, subscriptionId_()
	, availableSequenceNumberArraySPtr_(OpcUaUInt32Array::construct())
	, moreNotifications_()
	, notficiationMessage_()
	, statusCodeArraySPtr_(OpcUaStatusCodeArray::construct())
	, diagnosticInfoArraySPtr_(OpcUaDiagnosticInfoArray::construct())
	{
	}

	PublishResponse::~PublishResponse(void)
	{
	}

	void 
	PublishResponse::responseHeader(const ResponseHeader::SPtr responseHeader)
	{
		responseHeaderSPtr_ = responseHeader;
	}

	ResponseHeader::SPtr 
	PublishResponse::responseHeader(void) const
	{
		return responseHeaderSPtr_;
	}

	void 
	PublishResponse::subscriptionId(const OpcUaUInt32& subscriptionId)
	{
		subscriptionId_ = subscriptionId;
	}
	
	OpcUaUInt32 
	PublishResponse::subscriptionId(void) const
	{
		return subscriptionId_;
	}
	
	void 
	PublishResponse::availableSequenceNumbers(const OpcUaUInt32Array::SPtr availableSequenceNumbers)
	{
		availableSequenceNumberArraySPtr_ = availableSequenceNumbers;
	}

	OpcUaUInt32Array::SPtr 
	PublishResponse::availableSequenceNumbers(void) const
	{
		return availableSequenceNumberArraySPtr_;
	}
	
	void 
	PublishResponse::moreNotifications(const OpcUaBoolean& moreNotifications)
	{
		moreNotifications_ = moreNotifications;
	}
	
	OpcUaBoolean 
	PublishResponse::moreNotifications(void) const
	{
		return moreNotifications_;
	}
	
	void 
	PublishResponse::notificationMessage(const NotificationMessage& notificationMessage)
	{
		notficiationMessage_ = notificationMessage;
	}
	
	NotificationMessage& 
	PublishResponse::notificationMessage(void)
	{
		return notficiationMessage_;
	}

	void 
	PublishResponse::results(const OpcUaStatusCodeArray::SPtr results)
	{
		statusCodeArraySPtr_ = results;
	}
	
	OpcUaStatusCodeArray::SPtr 
	PublishResponse::results(void) const
	{
		return statusCodeArraySPtr_;
	}
	
	void 
	PublishResponse::diagnosticInfos(const OpcUaDiagnosticInfoArray::SPtr diagnosticInfos)
	{
		diagnosticInfoArraySPtr_ = diagnosticInfos;
	}

	OpcUaDiagnosticInfoArray::SPtr 
	PublishResponse::diagnosticInfos(void) const
	{
		return diagnosticInfoArraySPtr_;
	}

	void 
	PublishResponse::opcUaBinaryEncode(std::ostream& os) const
	{
		responseHeaderSPtr_->opcUaBinaryEncode(os);
		OpcUaNumber::opcUaBinaryEncode(os, subscriptionId_);
		availableSequenceNumberArraySPtr_->opcUaBinaryEncode(os);
		OpcUaNumber::opcUaBinaryEncode(os, moreNotifications_);
		notficiationMessage_.opcUaBinaryEncode(os);
		statusCodeArraySPtr_->opcUaBinaryEncode(os);
		diagnosticInfoArraySPtr_->opcUaBinaryEncode(os);
	}
	
	void 
	PublishResponse::opcUaBinaryDecode(std::istream& is)
	{
		responseHeaderSPtr_->opcUaBinaryDecode(is);
		OpcUaNumber::opcUaBinaryDecode(is, subscriptionId_);
		availableSequenceNumberArraySPtr_->opcUaBinaryDecode(is);
		OpcUaNumber::opcUaBinaryDecode(is, moreNotifications_);
		notficiationMessage_.opcUaBinaryDecode(is);
		statusCodeArraySPtr_->opcUaBinaryDecode(is);
		diagnosticInfoArraySPtr_->opcUaBinaryDecode(is);
	}
}