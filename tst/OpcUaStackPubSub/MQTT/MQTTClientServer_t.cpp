#include "unittest.h"

#include <iostream>
#include "OpcUaStackPubSub/MQTT/MQTTClientServer.h"

using namespace OpcUaStackPubSub;

BOOST_AUTO_TEST_SUITE(MQTTClientServer_)

BOOST_AUTO_TEST_CASE(MQTTClientServer_)
{
	std::cout << "MQTTClientServer_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(MQTTClientServer_construct)
{
	MQTTClientServerBase::SPtr mqttClient = constructMQTT();

	if (mqttClient->mqttIfEnabled()) {
		std::cout << "MQTTCLIENT ENABLED" << std::endl;
	}
	else {
		std::cout << "MQTTCLIENT DISABLED" << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(MQTTClientServer_init)
{
	MQTTClientServerBase::SPtr mqttClient = constructMQTT();

	if (mqttClient->mqttIfEnabled()) {
		mqttClient->init();
		mqttClient->cleanup();
	}
}

BOOST_AUTO_TEST_CASE(MQTTClientServer_startup)
{
	MQTTClientServerBase::SPtr mqttClient = constructMQTT();

	if (mqttClient->mqttIfEnabled()) {
		mqttClient->init();
		mqttClient->startup();
		mqttClient->shutdown();
		mqttClient->cleanup();
	}
}


BOOST_AUTO_TEST_SUITE_END()