#pragma once

#include "XmlParser.h"
#include "DaeClasses.h"

class DaeParser
{
private:
	XmlParser* xmlParser;

private:
	void _LibraryImages(Dae_Data& data);
	void _LibraryMaterials(Dae_Data& data);
	void _LibraryEffects(Dae_Data& data);
	void _LibraryGeometries(Dae_Data& data);
	void _LibraryController(Dae_Data& data);
	void _LibraryAnimations(Dae_Data& data);
	void _LibraryVisualScene(Dae_Data& data);
public:
	DaeParser();
	~DaeParser();

	void GetIDAndNameAttribute(XmlElementPtr& element, std::string& id, std::string& name);
	Vec2 StringToVec2(const std::string& arg);
	Vec3 StringToVec3(const std::string& arg);
	Vec4 StringToVec4(const std::string& arg);
	Mat4 StringToMat4(const std::string& arg);

	void RemoveCharFromString(std::string& str, const char charToRemove);
	void ReplaceCharFromString(std::string& str, const char beReplaceChar, const char replaceChar);

	void Parse(const char* daePath, Dae_Data& data);
};

