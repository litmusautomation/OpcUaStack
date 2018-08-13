/*
   Copyright 2018 Kai Huebl (kai@huebl-sgh.de)

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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackPubSub/MQTT/MQTTClientServer.h"

using namespace OpcUaStackCore;

namespace OpcUaStackPubSub
{

#ifdef USE_MOSQUITTO_CLIENT

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    //
    // callback
    //
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
	void on_log(struct mosquitto* mosq, void* obj, int level, const char* message)
	{
		printf("%s", message);
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// MQTTClientServer
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	uint32_t MQTTClientServer::mqttInstances_ = 0;

	MQTTClientServer::MQTTClientServer(void)
	: MQTTClientServerBase()
	, mosq_(nullptr)
	, clientId_("OpcUaPubSub")
	{
		if (mqttInstances_ == 0) {
			mosquitto_lib_init();

			int major, minor, revision;
			mosquitto_lib_version(&major, &minor, &revision);
			std::stringstream vers;

			vers << major << "." << minor << "." << revision;
			Log(Debug, "mosquitto lib init")
			    .parameter("Version", vers.str());
		}
		mqttInstances_++;
	}

	MQTTClientServer::~MQTTClientServer(void)
	{
		mqttInstances_--;
		if (mqttInstances_ == 0) {
			Log(Debug, "mosquitto lib cleanup");

			mosquitto_lib_cleanup();
		}
	}

	bool
	MQTTClientServer::init(void)
	{
		mosq_ = mosquitto_new(clientId_.c_str(), true, this);
		if (mosq_ == nullptr) {
			Log(Error, "mosquitto_new error");
			return false;
		}

		mosquitto_log_callback_set(mosq_, on_log);

		return true;
	}

	bool
	MQTTClientServer::cleanup(void)
	{
		if (mosq_ != nullptr) {
			mosquitto_destroy(mosq_);
		}
		return true;
	}

	bool
	MQTTClientServer::startup(void)
	{
		int rc = mosquitto_loop_start(mosq_);
		if (rc != MOSQ_ERR_SUCCESS) {
			return false;
		}
		return true;
	}

	bool
	MQTTClientServer::shutdown(void)
	{
		mosquitto_loop_stop(mosq_, true);
		return true;
	}

	bool
	MQTTClientServer::mqttIfEnabled(void)
	{
		return true;
	}

	MQTTClientServerBase::SPtr constructMQTT(void)
	{
		return constructSPtr<MQTTClientServer>();
	}

#else

	MQTTClientServerBase::SPtr constructMQTT(void)
	{
		return constructSPtr<MQTTClientServerBase>();
	}

#endif


}