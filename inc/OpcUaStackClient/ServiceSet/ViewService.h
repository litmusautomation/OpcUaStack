#ifndef __OpcUaStackClient_ViewService_h__
#define __OpcUaStackClient_ViewService_h__

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Component/Component.h"
#include "OpcUaStackCore/ServiceSet/ViewServiceTransaction.h"
#include "OpcUaStackClient/ServiceSet/Session.h"

using namespace OpcUaStackCore;

namespace OpcUaStackClient 
{

	class DLLEXPORT ViewServiceIf
	{
	  public:
        virtual void viewServiceBrowseResponse(ServiceTransactionBrowse::SPtr serviceTransactionBrowse) {};
	};

	class DLLEXPORT ViewService : public Component
	{
	  public:
		boost::shared_ptr<ViewService> SPtr;

		ViewService(void);
		~ViewService(void);

		void componentSession(Component* componentSession);
		void viewServiceIf(ViewServiceIf* viewServiceIf);

		void send(ServiceTransactionBrowse::SPtr serviceTransactionBrowse);

		//- Component -----------------------------------------------------------------
		void receive(OpcUaNodeId& typeId, Message::SPtr message);
		//- Component -----------------------------------------------------------------

	  private:
		Component* componentSession_;

		ViewServiceIf* viewServiceIf_;
	};

}

#endif
