/*
   Copyright 2017 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de), Aleksey Timin (atimin@gmail.com)
 */

#include "OpcUaStackPubSub/Network/NetworkMessageCreator.h"

namespace OpcUaStackPubSub
{

	NetworkMessageCreator::NetworkMessageCreator(void)
	: ioThread_()
	, slotTimerElement_()
	, dataSetWriters_()
	, networkSender_()
	, publishInterval_(1000)
	, publisherIdEnabled_(false)
	, dataSetWriterIdEnabled_(false)
	, dataSetArrayEnabled_(false)
	, timestampEnabled_(false)
	, picosecondsEnabled_(false)
	, dataSetClassIdEnabled_(false)
	, promotedFieldsEnabled_(false)
	, publisherId_()
	{
	}

	NetworkMessageCreator::~NetworkMessageCreator(void)
	{
	}

	void
	NetworkMessageCreator::publishInterval(uint32_t publishInterval)
	{
		publishInterval_ = publishInterval;
	}

	void
	NetworkMessageCreator::ioThread(IOThread::SPtr& ioThread)
	{
		ioThread_ = ioThread;
	}

	bool
	NetworkMessageCreator::startup(void)
	{
		if (ioThread_.get() == nullptr) {
			return false;
		}

		// start publish timer loop
		slotTimerElement_ = constructSPtr<SlotTimerElement>();
		slotTimerElement_->callback().reset(boost::bind(&NetworkMessageCreator::publish, this));
		slotTimerElement_->expireTime(boost::posix_time::microsec_clock::local_time(), publishInterval_);
		ioThread_->slotTimer()->start(slotTimerElement_);

		return true;
	}

	bool
	NetworkMessageCreator::shutdown(void)
	{
		// stop publish timer loop
		ioThread_->slotTimer()->stop(slotTimerElement_);
		slotTimerElement_.reset();

		return true;
	}

	bool
	NetworkMessageCreator::deregisterDataSetWriterIf(uint32_t writerId)
	{
		// FIXME: todo
		return true;
	}

	bool
	NetworkMessageCreator::registerDataSetWriterIf(const DataSetWriterIf::SPtr& writerIf)
	{
		dataSetWriters_.push_back(writerIf);
		return true;
	}

	void
	NetworkMessageCreator::networkSenderIf(const NetworkSenderIf::SPtr& senderIf)
	{
		networkSender_ =  senderIf;
	}

	NetworkSenderIf::SPtr
	NetworkMessageCreator::networkSenderIf()
	{
		return networkSender_;
	}

	bool
	NetworkMessageCreator::publish()
	{

		//FIXME handle keyFrameCount
		//FIXME handle keepAliveTime

		NetworkMessage networkMessage;
		// Build network header
		NetworkMessageHeader::SPtr header = networkMessage.networkMessageHeader();
		header->publisherIdEnabled(publisherIdEnabled_);
		header->publisherId(publisherId_);
		header->dataSetWriterIdEnabled(dataSetWriterIdEnabled_);
		header->dataSetArrayEnabled(dataSetArrayEnabled_);
		header->timestampEnabled(timestampEnabled_);
		header->picoSecondsEnabled(picosecondsEnabled_);
		header->dataSetClassIdEnabled(dataSetClassIdEnabled_);
		header->promotedFieldsEnabled(promotedFieldsEnabled_);

		// Build payload header
		networkMessage.dataSetPayloadHeader()->dataSetArrayEnabled(dataSetArrayEnabled_);

		// Build payload
		DataSetMessageArray::SPtr messages = constructSPtr<DataSetMessageArray>();
		messages->resize(dataSetWriters_.size());

		for(WriterCollection::iterator it = dataSetWriters_.begin();
				it != dataSetWriters_.end(); it++) {
			DataSetMessage::SPtr msg;

			if ((*it)->publishTimeout(msg)) {
				messages->push_back(msg);
			}
		}

		networkMessage.dataSetPayload()->count(messages->size());
		networkMessage.dataSetPayload()->dataSetMessages(messages);
		networkMessage.dataSetPayload()->dataSetArrayEnabled(dataSetArrayEnabled_);

		networkSender_->send(networkMessage);

		return true;
	}

	void
	NetworkMessageCreator::publisherIdEnabled(bool publisherIdEnabled)
	{
		publisherIdEnabled_ = publisherIdEnabled;
	}

	bool
	NetworkMessageCreator::publisherIdEnabled() const
	{
		return publisherIdEnabled_;
	}

	void
	NetworkMessageCreator::publisherId(OpcUaVariant::SPtr publisherId)
	{
		publisherId_ = publisherId;
	}

	OpcUaVariant::SPtr
	NetworkMessageCreator::publisherId() const
	{
		return publisherId_;
	}

	NetworkMessageCreator::WriterCollection
	NetworkMessageCreator::dataSetWriterIfs() const
	{
		return dataSetWriters_;
	}

	void
	NetworkMessageCreator::dataSetWriterIdEnabled(bool dataSetWriterIdEnabled)
	{
		dataSetWriterIdEnabled_ = dataSetWriterIdEnabled;
	}

	bool
	NetworkMessageCreator::dataSetWriterIdEnabled() const
	{
		return dataSetWriterIdEnabled_;
	}

	void
	NetworkMessageCreator::dataSetArrayEnabled(bool dataSetArrayEnabled)
	{
		dataSetArrayEnabled_ = dataSetArrayEnabled;
	}

	bool
	NetworkMessageCreator::dataSetArrayEnabled() const
	{
		return dataSetArrayEnabled_;
	}

	void
	NetworkMessageCreator::timestampEnabled(bool timestampEnabled)
	{
		timestampEnabled_ = timestampEnabled;
	}

	bool
	NetworkMessageCreator::timestampEnabled() const
	{
		return timestampEnabled_;
	}

	void
	NetworkMessageCreator::picosecondsEnabled(bool picosecondsEnabled)
	{
		picosecondsEnabled_ = picosecondsEnabled;
	}

	bool
	NetworkMessageCreator::picosecondsEnabled() const
	{
		return picosecondsEnabled_;
	}

	void
	NetworkMessageCreator::dataSetClassIdEnabled(bool dataSetClassIdEnabled)
	{
		dataSetClassIdEnabled_ = dataSetClassIdEnabled;
	}

	bool
	NetworkMessageCreator::dataSetClassIdEnabled() const
	{
		return dataSetClassIdEnabled_;
	}

	void
	NetworkMessageCreator::promotedFieldsEnabled(bool promotedFieldsEabled)
	{
		promotedFieldsEnabled_ = promotedFieldsEabled;
	}

	bool
	NetworkMessageCreator::promotedFieldsEnabled() const
	{
		return promotedFieldsEnabled_;
	}
}
