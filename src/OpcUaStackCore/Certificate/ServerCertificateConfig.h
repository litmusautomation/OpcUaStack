/*
   Copyright 2018 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   ServerCertificateConfigrmationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaStackCore_ServerCertificateConfig_h__
#define __OpcUaStackCore_ServerCertificateConfig_h__

#include <string>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Certificate/ServerCertificate.h"

namespace OpcUaStackCore
{

	class DLLEXPORT ServerCertificateConfig
	{
	  public:
		ServerCertificateConfig(void);
		~ServerCertificateConfig(void);

		static bool parse(
			ServerCertificate::SPtr& serverCertificate,
			const std::string& configPrefix,
			Config* childConfig,
			const std::string& configurationFileName
		);
	};

}

#endif
