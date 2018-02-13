#include "XmlReader.h"

#include <cstdlib>
#include <cassert>

namespace XmlReader
{
	#pragma region XmlString
	XmlString::XmlString() : string()
	{
	}

	XmlString::XmlString(const char* other) : string(other)
	{
	}

	XmlString::XmlString(string other) : string(other)
	{
	}

	int XmlString::AsInt() const
	{
	return atoi(this->c_str());
	}

	float XmlString::AsFloat() const
	{
		return (float)atof(this->c_str());
	}

#pragma endregion

#pragma region XmlNode

	XmlNode LoadXmlFile(const string& path)
	{
		LIBXML_TEST_VERSION

			xmlDoc *doc = xmlReadFile(path.c_str(), NULL, 0);

		if (doc == NULL)
		{
			printf("error: could not parse file %s\n", path.c_str());
		}

		XmlReader::XmlNode result = XmlReader::XmlNode(xmlDocGetRootElement(doc));

		xmlFreeDoc(doc);
		xmlCleanupParser();

		return result;
	}

	XmlNode::XmlNode()
	{
	}

	XmlNode::XmlNode(const string& path)
	{
	}

	XmlNode::XmlNode(const xmlNode* other)
	{
		assert(other != NULL);
		assert(other->type == XML_ELEMENT_NODE);

		this->name = (char*)(other->name);

		for (xmlAttr* attr = other->properties; attr != NULL; attr = attr->next)
		{
			this->attributes[(char*)attr->name] = (char*)attr->children->content;
		}

		for (xmlNode* child = other->children; child != NULL; child = child->next)
		{
			if (child->type == XML_TEXT_NODE)
			{
				this->text = (char*)(child->content);
			}
			else if (child->type = XML_ELEMENT_NODE)
			{
				this->children.push_back(XmlNode(child));
			}
		}
	}

	const XmlString XmlNode::GetName() const
	{
		return this->name;
	}

	const XmlString XmlNode::GetText() const
	{
		return this->text;
	}

	const bool XmlNode::HasAttribute(const XmlString& name) const
	{
		for (auto iter = this->attributes.begin(); iter != this->attributes.end(); iter++)
			if (iter->first == name)
				return true;
		return false;
	}

	const XmlString XmlNode::GetAttribute(const XmlString& name) const
	{
		return this->attributes.at(name);
	}

	const XmlString XmlNode::GetAttribute(const XmlString& name, const XmlString& default) const
	{
		if (this->HasAttribute(name))
			return this->attributes.at(name);
		else
			return default;
	}

	const vector<XmlNode> XmlNode::GetChildren(const XmlString& name) const
	{
		vector<XmlNode> result = vector<XmlNode>();

		for (auto iter = this->children.begin(); iter != this->children.end(); iter++)
		{
			if ((name == "") || (iter->name == name))
				result.push_back(*iter);
		}

		return result;
	}

	#pragma endregion
}