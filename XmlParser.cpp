#include "XmlParser.h"
#include <comdef.h>
#include "DebugTimer.h"

XmlParser::XmlParser()
	:xmlDocument(nullptr)
{
	CoInitialize(nullptr);

	CoCreateInstance(CLSID_DOMDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument2, reinterpret_cast<void**>(&xmlDocument));
}

XmlParser::~XmlParser()
{
	CoUninitialize();
}

bool XmlParser::OpenXML(const char * filePath)
{
	VARIANT_BOOL checkSuccess = 0;

	xmlDocument->load(_variant_t(filePath), &checkSuccess);
		
	if (checkSuccess)
	{
		return true;
	}

	return false;
}

void XmlParser::Close()
{
	xmlDocument->Release();
}

std::shared_ptr<XmlElementList> XmlParser::GetElementList(const char * query)
{
	IXMLDOMNodeList* tmpNodeList = nullptr;
	xmlDocument->selectNodes(_bstr_t(query), &tmpNodeList);

	return std::shared_ptr<XmlElementList>(XmlElementList::Create(tmpNodeList));
}

std::shared_ptr<XmlElement> XmlParser::GetElement(const char * query)
{
	IXMLDOMNode* tmpNode = nullptr;
	xmlDocument->selectSingleNode(_bstr_t(query), &tmpNode);

	return std::shared_ptr<XmlElement>(XmlElement::Create(tmpNode));
}










XmlElementList * XmlElementList::Create(IXMLDOMNodeList * nodeList)
{
	if (nodeList == nullptr)
	{
		return nullptr;
	}

	return new XmlElementList(nodeList);
}

std::shared_ptr<XmlElement> XmlElementList::GetElement(size_t index)
{
	IXMLDOMNode* tmpNode = nullptr;
	nodeList->get_item(index, &tmpNode);

	return std::shared_ptr<XmlElement>(XmlElement::Create(tmpNode));
}

size_t XmlElementList::GetLength()
{
	long length = 0;

	nodeList->get_length(&length);

	return static_cast<size_t>(length);
}









XmlElement * XmlElement::Create(IXMLDOMNode * node)
{
	if (node == nullptr)
	{
		return nullptr;
	}

	return new XmlElement(node);
}

std::shared_ptr<XmlElementList> XmlElement::GetChildElementList(const char * query)
{
	IXMLDOMNodeList* tmpNodeList = nullptr;
	node->selectNodes(_bstr_t(query), &tmpNodeList);

	return std::shared_ptr<XmlElementList>(XmlElementList::Create(tmpNodeList));
}

std::shared_ptr<XmlElement> XmlElement::GetChildElement(const char * query)
{
	IXMLDOMNode* tmpNode = nullptr;
	node->selectSingleNode(_bstr_t(query), &tmpNode);

	return std::shared_ptr<XmlElement>(XmlElement::Create(tmpNode));
}

void XmlElement::GetAttribute(const char * attribute, std::string& buffer)
{
	IXMLDOMNamedNodeMap* map = nullptr;
	IXMLDOMNode* attributeNode = nullptr;

	node->get_attributes(&map);

	map->getNamedItem(_bstr_t(attribute), &attributeNode);
	if (attributeNode == nullptr) { buffer = ""; return; }

	VARIANT var;
	attributeNode->get_nodeValue(&var);

	buffer = std::string(_bstr_t(var.bstrVal));

	attributeNode->Release();
	map->Release();
}

void XmlElement::GetText(std::string& buffer)
{
	BSTR string = nullptr;
	node->get_text(&string);

	buffer = std::string(_bstr_t(string));

	SysFreeString(string);
}
