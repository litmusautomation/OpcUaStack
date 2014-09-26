#ifndef __OpcUaStackCore_SetMonitoringModeResponse_h__
#define __OpcUaStackCore_SetMonitoringModeResponse_h__

#include <stdint.h>
#include "OpcUaStackCore/Base/ObjectPool.h"
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/SecureChannel/ResponseHeader.h"

namespace OpcUaStackCore
{

	class DLLEXPORT SetMonitoringModeResponse : public  ObjectPool<SetMonitoringModeResponse>
	{
	  public:
		SetMonitoringModeResponse(void);
		virtual ~SetMonitoringModeResponse(void);

		void results(const OpcUaStatusCodeArray::SPtr results);
		OpcUaStatusCodeArray::SPtr results(void) const;
		void diagnosticInfos(const OpcUaDiagnosticInfoArray::SPtr diagnosticInfos);
		OpcUaDiagnosticInfoArray::SPtr diagnosticInfos(void) const;

		void opcUaBinaryEncode(std::ostream& os) const;
		void opcUaBinaryDecode(std::istream& is);

	  private:
		OpcUaStatusCodeArray::SPtr resultArraySPtr_;
		OpcUaDiagnosticInfoArray::SPtr diagnosticInfoArraySPtr_;
	};

}

#endif