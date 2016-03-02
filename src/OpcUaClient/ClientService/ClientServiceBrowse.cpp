/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/ObjectPool.h"
#include "OpcUaClient/ClientCommand/CommandBrowse.h"
#include "OpcUaClient/ClientService/ClientServiceBrowse.h"

using namespace OpcUaStackCore;

namespace OpcUaClient
{

	ClientServiceBrowse::ClientServiceBrowse(void)
	: ClientServiceBase()
	, browseCompleted_()
	{
	}

	ClientServiceBrowse::~ClientServiceBrowse(void)
	{
	}

	ClientServiceBase::SPtr
	ClientServiceBrowse::createClientService(void)
	{
		return constructSPtr<ClientServiceBrowse>();
	}

	bool
	ClientServiceBrowse::run(ClientServiceManager& clientServiceManager, CommandBase::SPtr& commandBase)
	{
		OpcUaStatusCode statusCode;
		CommandBrowse::SPtr commandBrowse = boost::static_pointer_cast<CommandBrowse>(commandBase);

		// create new or get existing client object
		ClientAccessObject::SPtr clientAccessObject;
		clientAccessObject = clientServiceManager.getClientAccessObject(commandBrowse->session());
		if (clientAccessObject.get() == nullptr) {
			std::stringstream ss;
			ss << "get client access object failed:"
			   << " Session=" << commandBrowse->session();
			errorMessage(ss.str());
			return false;
		}

		// check session
		if (clientAccessObject->sessionService_.get() == nullptr) {
			std::stringstream ss;
			ss << "session object not exist: "
			   << " Session=" << commandBrowse->session();
			return false;
		}

		// get or create view service
		ViewService::SPtr viewService;
		viewService = clientAccessObject->createViewService();
		if (viewService.get() == nullptr) {
			std::stringstream ss;
			ss << "get client view service failed"
			   << " Session=" << commandBrowse->session();
			errorMessage(ss.str());
			return false;
		}

		// browse opc ua server information model
		ViewServiceBrowse viewServiceBrowse;
		viewServiceBrowse.viewService(viewService);
		viewServiceBrowse.nodeId(commandBrowse->nodeId());
		viewServiceBrowse.viewServiceBrowseIf(this);
		viewServiceBrowse.asyncBrowse();

		// wait for the end of the browse command
		browseCompleted_.waitForCondition();

		return true;
	}

	void
	ClientServiceBrowse::done(OpcUaStatusCode statusCode)
	{
		browseCompleted_.conditionTrue();
		std::cout << OpcUaStatusCodeMap::shortString(statusCode) << std::endl;
	}

	void
	ClientServiceBrowse::browseResult(OpcUaNodeId::SPtr& nodeId, ReferenceDescriptionArray::SPtr& referenceDescriptionArray)
	{
		std::cout << nodeId->toString() << std::endl;
		referenceDescriptionArrayOut(referenceDescriptionArray);
	}

	void
	ClientServiceBrowse::browseNextResult(OpcUaNodeId::SPtr& nodeId, ReferenceDescriptionArray::SPtr& referenceDescriptionArray)
	{
		referenceDescriptionArrayOut(referenceDescriptionArray);
	}

	void
	ClientServiceBrowse::referenceDescriptionArrayOut(ReferenceDescriptionArray::SPtr& referenceDescriptionArray)
	{
		for (uint32_t idx=0; idx<referenceDescriptionArray->size(); idx++) {
			ReferenceDescription::SPtr referenceDescription;
			referenceDescriptionArray->get(idx, referenceDescription);

			std::cout << "   "
				<< " " << referenceDescription->nodeClass()
				<< " " << referenceDescription->expandedNodeId()->toString()
				<< " " << referenceDescription->typeDefinition()->toString()
				<< " " << referenceDescription->referenceTypeId()->toString()
				<< " " << (referenceDescription->isForward() == 0x01 ? "true" : "true")
				<< " " << referenceDescription->displayName().toString()
				<< " " << referenceDescription->browseName().toString()
				<< std::endl;
		}
	}

}

