#pragma once

#include <string>
using std::string;

#include <map>
using std::map;

#include "XmlReader.h"
using XmlReader::XmlNode;

#include "Model.h"

namespace Models
{
	class Font
	{
	public:
		Font(const XmlNode& fontNode);

		void LoadXml(const XmlNode& fontNode);

		Model ModelText(string text);

	private:
		map<char, Model> letterModels;
		Vector2 size;
	};
}
