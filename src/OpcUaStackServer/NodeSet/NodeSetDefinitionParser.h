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

#ifndef __OpcUaStackServer_NodeSetDefinitionParser_h__
#define __OpcUaStackServer_NodeSetDefinitionParser_h__

#include <boost/property_tree/ptree.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardDataTypes/StructureDefinition.h"

using namespace OpcUaStackCore;

namespace OpcUaStackServer
{

	/**
	 * This class is used to encode end decode the Definition element in a node set
	 */
	class DLLEXPORT NodeSetDefinitionParser
	{
	  public:
		NodeSetDefinitionParser(void);
		~NodeSetDefinitionParser(void);

		/**
		 * This method is used to decode the Definition element from an opc ua node set.
		 *
		 * @param[in] ptreeValue			property tree with the node set
		 * @param[out] structureDefinition	structure type definition
		 * @return true if successful
		 */
		bool decode(
			boost::property_tree::ptree& ptreeValue,
			StructureDefinition::SPtr& structureDefinition
		);

		/**
		 * This method is used to encode the Definition element from an opc ua node set.
		 *
		 * @param[in] structureDefinition	structure type definition to encode
		 * @param[out] ptreeValue			property tree with the result node set
		 * @return true if successful
		 */
		bool encode(
			StructureDefinition::SPtr& structureDefinition,
			boost::property_tree::ptree& ptreeValue
		);

	  private:
		bool decodeStructureDefinition(
			boost::property_tree::ptree& ptreeValue,
			StructureDefinition::SPtr& structureDefinition
		);

		bool encodeStructureDefinition(
			StructureDefinition::SPtr& structureDefinition,
			boost::property_tree::ptree& ptreeValue
		);

		bool decodeStructureField(
			boost::property_tree::ptree& ptreeValue,
			StructureField::SPtr& structureField
		);

		bool encodeStructureField(
			StructureField::SPtr& structureField,
			boost::property_tree::ptree& ptreeValue
		);

	};


}

#endif