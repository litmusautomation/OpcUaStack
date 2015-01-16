#ifndef __OpcUaStackClient_AttributeService_h__
#define __OpcUaStackClient_AttributeService_h__

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Component/Component.h"
#include "OpcUaStackCore/ServiceSet/AttributeServiceTransaction.h"
#include "OpcUaStackClient/ServiceSet/Session.h"

using namespace OpcUaStackCore;

namespace OpcUaStackClient 
{

	class DLLEXPORT AttributeServiceIf
	{
	  public:
        virtual void attributeServiceReadResponse(ServiceTransactionRead::SPtr serviceTransactionRead) {};
		virtual void attributeServiceWriteResponse(ServiceTransactionWrite::SPtr serviceTransactionWrite) {};
	};

	class DLLEXPORT AttributeService : public Component
	{
	  public:
		boost::shared_ptr<AttributeService> SPtr;

		AttributeService(void);
		~AttributeService(void);

		void componentSession(Component* componentSession);
		void attributeServiceIf(AttributeServiceIf* attributeServiceIf);

		void send(ServiceTransactionRead::SPtr serviceTransactionRead);
		void send(ServiceTransactionWrite::SPtr serviceTransactionWrite);

		//- Component -----------------------------------------------------------------
		void receive(OpcUaNodeId& typeId, Message::SPtr message);
		//- Component -----------------------------------------------------------------

	  private:
		Component* componentSession_;

		AttributeServiceIf* attributeServiceIf_;
	};

}

#endif
