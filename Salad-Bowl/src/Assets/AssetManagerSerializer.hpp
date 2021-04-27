#pragma once

#include "AssetManager.h"
#include "Util/Xml.h"

#include <fstream>

namespace Salad::Asset {

	class AssetManagerSerializer {
	
	public:
		void serialize(AssetManager& manager, const char* filepath) {
			using namespace Salad::Xml;

			XmlDocument doc = createDocument("asset_manager");
			XmlNode includeNode = doc.allocateNode("included_assets");
			std::string includesize = std::to_string(manager.m_IncludedAssets.size());
			includeNode.appendAttribute("size", includesize.c_str());
			doc.appendNodeToRoot(includeNode);

			for (auto it = manager.m_IncludedAssets.begin(); it != manager.m_IncludedAssets.end(); it++) {
				XmlNode assetNode = doc.allocateNode("asset");
				assetNode.setValue(it->first.c_str());
				assetNode.appendAttribute("dirty", it->second.dirty);
				includeNode.appendNode(assetNode);
			}

			std::ofstream file(filepath);
			file << doc;
			file.close();

			doc.close();
		}

		void deserialize(AssetManager& manager, const char* filepath) {
			using namespace Salad::Xml;

			XmlDocument doc = parseXmlFile(filepath, "asset_manager");
			const XmlNode root = doc.getRootNode();
			const XmlNode includeNode = root.getFirstChild("included_assets");
			includeNode.iterate("asset", [&manager](const XmlNode& node) -> bool {
				AssetData data;
				data.dirty = node.getAttributeAsBool("dirty");
				manager.includeAsset(node.getValue(), data);
				return false;
			});
		}

	};

}