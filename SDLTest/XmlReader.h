#pragma once
#include <libxml/tree.h>

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

namespace XmlReader
{
	class XmlString : public std::string
	{
	public:
		XmlString();
		XmlString(const char* other);
		XmlString(string other);

		int AsInt() const;
		float AsFloat() const;
	};

	class XmlNode
	{
	public:
		XmlNode();
		XmlNode(const string& path);
		XmlNode(const xmlNode* other);

		const XmlString GetName() const;
		const XmlString GetText() const;
		const bool HasAttribute(const XmlString& name) const;
		const XmlString GetAttribute(const XmlString& name) const;
		const XmlString GetAttribute(const XmlString& name, const XmlString& default) const;
		const vector<XmlNode> GetChildren(const XmlString& name = "") const;

	private:
		string name;
		string text;
		map<string, string> attributes;
		vector<XmlNode> children;
	};

	XmlNode LoadXmlFile(const string& path);
}