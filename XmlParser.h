#pragma once

#include <MsXml6.h>
#include <string>
#include <memory>
#include <functional>

#pragma comment(lib,"msxml6.lib")

class XmlElementList;
class XmlElement;

using XmlElementListPtr = std::shared_ptr<XmlElementList>;
using XmlElementPtr = std::shared_ptr<XmlElement>;

class XmlParser
{
private:
	IXMLDOMDocument* xmlDocument;
public:
	XmlParser();
	~XmlParser();

	bool OpenXML(const char* filePath);
	void Close();

	XmlElementListPtr GetElementList(const char* query);
	XmlElementPtr GetElement(const char* query);
};

class XmlElementList
{
private:
	IXMLDOMNodeList* nodeList;

	XmlElementList(IXMLDOMNodeList* nodeList) : nodeList(nodeList) { }
public:
	~XmlElementList() { nodeList->Release(); }

	static XmlElementList* Create(IXMLDOMNodeList* nodeList);

	XmlElementPtr GetElement(size_t index);

	size_t GetLength();
};

class XmlElement
{
private:
	IXMLDOMNode* node;

	XmlElement(IXMLDOMNode* node) : node(node) {}
public:
	~XmlElement() { node->Release(); }

	static XmlElement* Create(IXMLDOMNode* node);

	XmlElementListPtr GetChildElementList(const char* query);
	XmlElementPtr GetChildElement(const char* query);

	void GetAttribute(const char* attribute, std::string& buffer);
	void GetText(std::string& buffer);
};