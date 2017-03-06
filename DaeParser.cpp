#include "DaeParser.h"
#include "DebugTimer.h"
#include <regex>
#include <functional>

void DaeParser::GetIDAndNameAttribute(XmlElementPtr & element, std::string & id, std::string & name)
{
	std::string buffer;
	element->GetAttribute("id", buffer);
	id = buffer;

	element->GetAttribute("name", buffer);
	name = buffer;
}

Vec2 DaeParser::StringToVec2(const std::string& arg)
{
	size_t blankIndex = 0;
	size_t blankOffset = 0;
	std::string string = arg;
	float arr[2];

	for (int strideCount = 0; strideCount < 2; strideCount++)
	{
		blankIndex = string.find(' ', blankOffset);
		std::string buf = string.substr(blankOffset, blankIndex - blankOffset);
		float tmpFloat = static_cast<float>(atof(buf.c_str()));

		arr[strideCount] = tmpFloat;

		blankOffset = blankIndex + 1;
	}

	return Vec2(arr);
}

Vec3 DaeParser::StringToVec3(const std::string& arg)
{
	size_t blankIndex = 0;
	size_t blankOffset = 0;
	std::string string = arg;
	float arr[3];

	for (int strideCount = 0; strideCount < 3; strideCount++)
	{
		blankIndex = string.find(' ', blankOffset);
		std::string buf = string.substr(blankOffset, blankIndex - blankOffset);
		float tmpFloat = static_cast<float>(atof(buf.c_str()));

		arr[strideCount] = tmpFloat;

		blankOffset = blankIndex + 1;
	}

	return Vec3(arr);
}

Vec4 DaeParser::StringToVec4(const std::string& arg)
{
	size_t blankIndex = 0;
	size_t blankOffset = 0;
	std::string string = arg;
	float arr[4];

	for (int strideCount = 0; strideCount < 4; strideCount++)
	{
		blankIndex = string.find(' ', blankOffset);
		std::string buf = string.substr(blankOffset, blankIndex - blankOffset);
		float tmpFloat = static_cast<float>(atof(buf.c_str()));

		arr[strideCount] = tmpFloat;

		blankOffset = blankIndex + 1;
	}

	return Vec4(arr);
}

Mat4 DaeParser::StringToMat4(const std::string& arg)
{
	size_t blankIndex = 0;
	size_t blankOffset = 0;
	std::string string = arg;
	float matrix[16];

	for (int strideCount = 0; strideCount < 16; strideCount++)
	{
		blankIndex = string.find(' ', blankOffset);
		std::string buf = string.substr(blankOffset, blankIndex - blankOffset);
		float tmpFloat = static_cast<float>(atof(buf.c_str()));

		matrix[strideCount] = tmpFloat;

		blankOffset = blankIndex + 1;
	}

	return Mat4(matrix);
}

void DaeParser::RemoveCharFromString(std::string & str, const char charToRemove)
{
	//std::remove 배열 등등의 시작 이터레이터와 끝 이터레이터와 지울 값을 넣어주면 내부의 반복문에서 값을 대치시켜 삭제한다. 어떤 배열이든 삭제시 이터레이트가 바뀔 수 있으니 이터레이트를 반환한다.
	//str.erase 삭제하고 나면 삭제한 나머지의 공간이 남으니 지워주는 역할을 함
	str.erase(std::remove(str.begin(), str.end(), charToRemove), str.end());
}

void DaeParser::ReplaceCharFromString(std::string & str, const char beReplaceChar, const char replaceChar)
{
	std::replace(str.begin(), str.end(), beReplaceChar, replaceChar);
}

void DaeParser::_LibraryImages(Dae_Data & data)
{
	_ANALYZE_();
	XmlElementPtr imageRootNode = xmlParser->GetElement("COLLADA/library_images");
	if (imageRootNode == nullptr) { return; }

	auto& images = data.images;

	XmlElementListPtr imageNodeList = imageRootNode.get()->GetChildElementList("image");
	images.resize(imageNodeList->GetLength());

	for (size_t i = 0; i < imageNodeList->GetLength(); i++)
	{
		XmlElementPtr imageNode = imageNodeList->GetElement(i);
		GetIDAndNameAttribute(imageNode, images[i].id, images[i].name);

		XmlElementPtr pathElement = imageNode->GetChildElement("init_from");
		std::string buffer;
		pathElement.get()->GetText(buffer);
		images[i].path = buffer;
	}
}

void DaeParser::_LibraryMaterials(Dae_Data & data)
{
	_ANALYZE_();
	XmlElementPtr materialRootNode = xmlParser->GetElement("COLLADA/library_materials");
	if (materialRootNode == nullptr) { return; }
	auto& materials = data.materials;

	XmlElementListPtr materialNodeList = materialRootNode.get()->GetChildElementList("material");
	materials.resize(materialNodeList->GetLength());
	for (size_t i = 0; i < materialNodeList->GetLength(); i++)
	{
		XmlElementPtr materialNode = materialNodeList->GetElement(i);
		GetIDAndNameAttribute(materialNode, materials[i].id, materials[i].name);

		XmlElementPtr instanceEffectNode = materialNode->GetChildElement("instance_effect");

		std::string buffer;
		instanceEffectNode->GetAttribute("url", buffer);
		materials[i].sourceEffect = buffer.substr(1);
	}
}

void DaeParser::_LibraryEffects(Dae_Data & data)
{
	_ANALYZE_();
	XmlElementPtr effectRootNode = xmlParser->GetElement("COLLADA/library_effects");
	if (effectRootNode == nullptr) { return; }
	auto& effects = data.effects;

	XmlElementListPtr effectNodeList = effectRootNode->GetChildElementList("effect");
	effects.resize(effectNodeList->GetLength());
	
	for (size_t i = 0; i < effectNodeList->GetLength(); i++)
	{
		XmlElementPtr effectNode = effectNodeList->GetElement(i);
		GetIDAndNameAttribute(effectNode, effects[i].id, effects[i].name);

		//profile_CG,profile_GLES,profile_GLSL,profile_COMMON
		XmlElementPtr profile = effectNode->GetChildElement("profile_COMMON");
		if (profile != nullptr)
		{
			XmlElementPtr phongNode = (profile->GetChildElement("technique"))->GetChildElement("phong");

			XmlElementPtr emissionNode = phongNode->GetChildElement("emission");
			if (emissionNode != nullptr)
			{
				XmlElementPtr colorNode = emissionNode->GetChildElement("color");

				std::string buffer;
				colorNode->GetText(buffer);
				Vec4 color = StringToVec4(buffer);

				effects[i].emission = color;
			}

			XmlElementPtr ambientNode = phongNode->GetChildElement("ambient");
			if (ambientNode != nullptr)
			{
				XmlElementPtr colorNode = ambientNode->GetChildElement("color");

				std::string buffer;
				colorNode->GetText(buffer);
				Vec4 color = StringToVec4(buffer);

				effects[i].ambient = color;
			}

			XmlElementPtr diffuseNode = phongNode->GetChildElement("diffuse");
			if (diffuseNode != nullptr)
			{
				XmlElementPtr colorNode = diffuseNode->GetChildElement("color");
				if (colorNode != nullptr)
				{
					std::string buffer;
					colorNode->GetText(buffer);
					Vec4 color = StringToVec4(buffer);

					effects[i].diffuse = color;
				}

				XmlElementPtr textureNode = diffuseNode->GetChildElement("texture");
				if (textureNode != nullptr)
				{
					std::string buffer;

					XmlElementPtr textureNode = diffuseNode->GetChildElement("texture");

					textureNode->GetAttribute("texture", buffer);
					effects[i].textrue.sourceImage = buffer;

					textureNode->GetAttribute("texcoord", buffer);
					effects[i].textrue.texcoord = buffer;
					//나머지 extra에 있는 엘리먼트들은 생략
				}
			}

			XmlElementPtr specularNode = phongNode->GetChildElement("specular");
			if (specularNode != nullptr)
			{
				XmlElementPtr colorNode = specularNode->GetChildElement("color");

				std::string buffer;
				colorNode->GetText(buffer);
				Vec4 color = StringToVec4(buffer);

				effects[i].specular = color;
			}

			XmlElementPtr shininessNode = phongNode->GetChildElement("shininess");
			if(shininessNode != nullptr)
			{
				XmlElementPtr floatNode = shininessNode->GetChildElement("float");

				std::string buffer;
				floatNode->GetText(buffer);

				effects[i].shininess = static_cast<float>(atof(buffer.c_str()));
			}

			XmlElementPtr reflectiveNode = phongNode->GetChildElement("reflective");
			if (reflectiveNode != nullptr)
			{
				XmlElementPtr colorNode = reflectiveNode->GetChildElement("color");

				std::string buffer;
				colorNode->GetText(buffer);
				Vec4 color = StringToVec4(buffer);

				effects[i].reflective = color;
			}

			XmlElementPtr reflectivityNode = phongNode->GetChildElement("reflectivity");
			if (reflectivityNode != nullptr)
			{
				XmlElementPtr floatNode = reflectivityNode->GetChildElement("float");

				std::string buffer;
				floatNode->GetText(buffer);

				effects[i].reflectivity = static_cast<float>(atof(buffer.c_str()));
			}

			XmlElementPtr transparentNode = phongNode->GetChildElement("transparent");
			if (transparentNode != nullptr)
			{
				XmlElementPtr colorNode = transparentNode->GetChildElement("color");

				std::string buffer;
				colorNode->GetText(buffer);
				Vec4 color = StringToVec4(buffer);

				effects[i].transparent = color;
			}

			XmlElementPtr transparencyNode = phongNode->GetChildElement("transparency");
			if (transparencyNode != nullptr)
			{
				XmlElementPtr floatNode = transparencyNode->GetChildElement("float");

				std::string buffer;
				floatNode->GetText(buffer);

				effects[i].reflectivity = static_cast<float>(atof(buffer.c_str()));
 			}
		}
	}
}

void DaeParser::_LibraryGeometries(Dae_Data & data)
{
	_ANALYZE_();
	XmlElementPtr geometryRootNode = xmlParser->GetElement("COLLADA/library_geometries");
	if (geometryRootNode == nullptr) { return; }
	auto& geometries = data.geometries;

	XmlElementListPtr geometryNodeList = geometryRootNode->GetChildElementList("geometry");
	geometries.resize(geometryNodeList->GetLength());

	for (size_t i = 0; i < geometryNodeList->GetLength(); i++)
	{
		XmlElementPtr geometryNode = geometryNodeList->GetElement(i);
		GetIDAndNameAttribute(geometryNode, geometries[i].id, geometries[i].name);
		XmlElementPtr meshNode = geometryNode->GetChildElement("mesh");

		XmlElementListPtr sourceList = meshNode->GetChildElementList("source");
		
		XmlElementPtr positionNode = sourceList->GetElement(0);
		{
			std::string buffer;

			XmlElementPtr floatArrayNode = positionNode->GetChildElement("float_array");
			floatArrayNode->GetAttribute("count", buffer);
			size_t positionCount = atoi(buffer.c_str()) / 3;
			geometries[i].position.resize(positionCount);

			floatArrayNode->GetText(buffer);

			size_t stringOffset = 0;
			for (size_t count = 0; count < positionCount; count++)
			{
				size_t strCursor = buffer.find("\n", stringOffset + 1);
				std::string curLine = buffer.substr(stringOffset, strCursor - stringOffset);
				stringOffset = strCursor;

				geometries[i].position[count] = StringToVec3(curLine);
			}
		}

		XmlElementPtr normalNode = sourceList->GetElement(1);
		{
			
			std::string buffer;

			XmlElementPtr floatArrayNode = normalNode->GetChildElement("float_array");
			floatArrayNode->GetAttribute("count", buffer);
			size_t normalCount = atoi(buffer.c_str()) / 3;
			geometries[i].normal.resize(normalCount);

			floatArrayNode->GetText(buffer);

			size_t stringOffset = 0;
			for (size_t count = 0; count < normalCount; count++)
			{
				size_t strCursor = buffer.find("\n", stringOffset + 1);
				std::string curLine = buffer.substr(stringOffset, strCursor - stringOffset);
				stringOffset = strCursor;

				geometries[i].normal[count] = StringToVec3(curLine);
			}
		}

		XmlElementPtr uvNode = sourceList->GetElement(2);
		{
			std::string buffer;

			XmlElementPtr floatArrayNode = uvNode->GetChildElement("float_array");
			floatArrayNode->GetAttribute("count", buffer);
			size_t uvCount = atoi(buffer.c_str()) / 2;
			geometries[i].uv.resize(uvCount);

			floatArrayNode->GetText(buffer);

			size_t stringOffset = 0;
			for (size_t count = 0; count < uvCount; count++)
			{
				size_t strCursor = buffer.find("\n", stringOffset + 1);
				std::string curLine = buffer.substr(stringOffset, strCursor - stringOffset);
				stringOffset = strCursor;

				geometries[i].uv[count] = StringToVec2(curLine);
			}
		}

		XmlElementListPtr triangleNodeList = meshNode->GetChildElementList("triangles");
		geometries[i].triangles.resize(triangleNodeList->GetLength());
		for (size_t triangleIndex = 0; triangleIndex < triangleNodeList->GetLength(); triangleIndex++)
		{
			std::string buffer;

			XmlElementPtr triangleNode = triangleNodeList->GetElement(triangleIndex);

			triangleNode->GetAttribute("count", buffer);
			size_t triangleCount = atoi(buffer.c_str());
			geometries[i].triangles[triangleIndex].count = triangleCount;
			geometries[i].triangles[triangleIndex].normalIndex.resize(triangleCount);
			geometries[i].triangles[triangleIndex].positionIndex.resize(triangleCount);
			geometries[i].triangles[triangleIndex].uvIndex.resize(triangleCount);

			triangleNode->GetAttribute("material",buffer);
			geometries[i].triangles[triangleIndex].materialSource = buffer;

			(triangleNode->GetChildElement("p"))->GetText(buffer);
			
			int inputSourceNumOffset = triangleNode->GetChildElementList("input")->GetLength() - 3;


			Dae_Geometry::Dae_Index tmpPosIndex;
			Dae_Geometry::Dae_Index tmpNorIndex;
			Dae_Geometry::Dae_Index tmpUvIndex;

			size_t strCursor = 0;
			for (size_t count = 0; count < (triangleCount * 3); count++)
			{
				if ((count % 3) == 0 && count)
				{
					geometries[i].triangles[triangleIndex].positionIndex[(count / 3) - 1] = tmpPosIndex;
					geometries[i].triangles[triangleIndex].normalIndex[(count / 3) - 1] = tmpNorIndex;
					geometries[i].triangles[triangleIndex].uvIndex[(count / 3) - 1] = tmpUvIndex;
				}

				std::string tmp;

				size_t blankPos = buffer.find(' ', strCursor + 1);
				tmp = buffer.substr(strCursor, blankPos - strCursor);
				strCursor = blankPos;
				*((&tmpPosIndex._0) + (count % 3)) = atoi(tmp.c_str());

				blankPos = buffer.find(' ', strCursor + 1);
				tmp = buffer.substr(strCursor, blankPos - strCursor);
				strCursor = blankPos;
				*((&tmpNorIndex._0) + (count % 3)) = atoi(tmp.c_str());

				blankPos = buffer.find(' ', strCursor + 1);
				tmp = buffer.substr(strCursor, blankPos - strCursor);
				strCursor = blankPos;
				*((&tmpUvIndex._0) + (count % 3)) = atoi(tmp.c_str());

				for (int i = 0; i < inputSourceNumOffset; i++)
				{
					strCursor = buffer.find(' ', strCursor + 1);
				}
			}
		}
	}
}

void DaeParser::_LibraryController(Dae_Data & data)
{
	_ANALYZE_();
	XmlElementPtr controllerRootNode = xmlParser->GetElement("COLLADA/library_controllers");
	if (controllerRootNode == nullptr) { return; }
	auto& controllers = data.controllers;

	XmlElementListPtr controllerList = controllerRootNode->GetChildElementList("controller");
	controllers.resize(controllerList->GetLength());

	for (size_t i = 0; i < controllerList->GetLength(); i++)
	{
		XmlElementPtr controllerNode = controllerList->GetElement(i);
		GetIDAndNameAttribute(controllerNode, controllers[i].id, controllers[i].name);
		
		std::string buffer;
		XmlElementPtr skinNode = controllerNode->GetChildElement("skin");
		skinNode->GetAttribute("source", buffer);
		controllers[i].skin.source = buffer.substr(1);

		XmlElementPtr bindShpaeMatrixNode = skinNode->GetChildElement("bind_shape_matrix");
		bindShpaeMatrixNode->GetText(buffer);
		Mat4 bindShpaeMatrix = StringToMat4(buffer);
		controllers[i].skin.bindShapeMatrix = bindShpaeMatrix;

		XmlElementListPtr sourceList = skinNode->GetChildElementList("source");

		XmlElementPtr jointsNode = sourceList->GetElement(0);
		{
			std::string buffer;
			(jointsNode->GetChildElement("Name_array"))->GetAttribute("count", buffer);
			size_t jointCount = atoi(buffer.c_str());
			controllers[i].skin.joints.resize(jointCount);

			(jointsNode->GetChildElement("Name_array"))->GetText(buffer);

			ReplaceCharFromString(buffer, '\n',' ');
			size_t strOffset = 0;
			for (size_t count = 0; count < jointCount; count++)
			{
				size_t blankPos = buffer.find(' ', strOffset + 1);
				std::string name = buffer.substr(strOffset, blankPos - strOffset);
				
				controllers[i].skin.joints[count].name = name;

				strOffset = blankPos + 1;
			}
		}


		XmlElementPtr matrices = sourceList->GetElement(1);
		{
			std::string buffer;
			matrices->GetChildElement("float_array")->GetText(buffer);

			size_t stringOffset = 0;
			for (size_t count = 0; count < controllers[i].skin.joints.size(); count++)
			{
				size_t escapeSequencePos = buffer.find('\n', stringOffset + 1);
				std::string curLine = buffer.substr(stringOffset, escapeSequencePos - stringOffset);
				Mat4 matrix = StringToMat4(curLine);

				controllers[i].skin.joints[count].matrix = matrix;

				stringOffset = escapeSequencePos + 1;
			}
		}

		XmlElementPtr weights = sourceList->GetElement(2);
		{
			std::string buffer;
			weights->GetChildElement("float_array")->GetAttribute("count", buffer);
			size_t weightCount = atoi(buffer.c_str());
			controllers[i].skin.weights.resize(weightCount);

			weights->GetChildElement("float_array")->GetText(buffer);
			ReplaceCharFromString(buffer, '\n', ' ');

			size_t strOffset = 0;
			for (size_t count = 0; count < weightCount; count++)
			{
				size_t blankPos = buffer.find(' ', strOffset + 1);
				std::string weightBuf = buffer.substr(strOffset, blankPos - strOffset);
				
				controllers[i].skin.weights[count] = static_cast<float>(atof(weightBuf.c_str()));

				strOffset = blankPos + 1;
			}
		}

		XmlElementPtr vertexWeightsNode = skinNode->GetChildElement("vertex_weights");
		{
			
			std::string vcountBuf; vertexWeightsNode->GetChildElement("vcount")->GetText(vcountBuf);
			{
				std::string tmp; 
				vertexWeightsNode->GetAttribute("count", tmp);
				size_t size = atoi(tmp.c_str());
				controllers[i].skin.vertexInfluenceCount.resize(size);

				size_t strOffset = 0;
				for (size_t count = 0; count < size; count++)
				{
					size_t blankPos = vcountBuf.find(' ', strOffset + 1);
					std::string curNumber = vcountBuf.substr(strOffset, blankPos - strOffset);

					controllers[i].skin.vertexInfluenceCount[count] = atoi(curNumber.c_str());

					strOffset = blankPos + 1;
				}
			}

			std::string vBuf; vertexWeightsNode->GetChildElement("v")->GetText(vBuf);
			{
				size_t strOffset = 0;
				for (size_t count = 0; count < controllers[i].skin.weights.size() ; count++)
				{
					std::string curJoint;
					std::string curWeight;
					size_t blankOffset;

					blankOffset = vBuf.find(' ', strOffset + 1);
					curJoint = vBuf.substr(strOffset, blankOffset - strOffset);
					strOffset = blankOffset + 1;

					blankOffset = vBuf.find(' ', strOffset + 1);
					curWeight = vBuf.substr(strOffset, blankOffset - strOffset);
					strOffset = blankOffset + 1;
					
					controllers[i].skin.jointWeightIndex.push_back(std::pair<size_t, size_t>(atoi(curJoint.c_str()), atoi(curWeight.c_str())));
				}
			}
		}
	}
}

void DaeParser::_LibraryAnimations(Dae_Data & data)
{
	_ANALYZE_();

	XmlElementPtr animationRootNode = xmlParser->GetElement("COLLADA/library_animations");
	if (animationRootNode == nullptr) { return; }
	auto& animations = data.animations;

	XmlElementListPtr animationList = animationRootNode->GetChildElementList("animation");
	size_t animationNum = animationList->GetLength();
	animations.resize(animationNum);

	for (size_t i = 0; i < animationNum; i++)
	{
		XmlElementPtr animationNode = animationList->GetElement(i);
		GetIDAndNameAttribute(animationNode, animations[i].id, animations[i].name);

		XmlElementListPtr sourceList = animationNode->GetChildElementList("animation/source");

		XmlElementPtr timeNode = sourceList->GetElement(0);
		{
			std::string buffer;

			timeNode->GetChildElement("float_array")->GetAttribute("count", buffer);
			size_t timeElementCount = atoi(buffer.c_str());
			animations[i].time.resize(timeElementCount);

			timeNode->GetChildElement("float_array")->GetText(buffer);
			ReplaceCharFromString(buffer, '\n', ' ');

			size_t strCursor = 0;
			for (size_t count = 0; count < timeElementCount; count++)
			{
				size_t blankPos = buffer.find(' ', strCursor + 1);
				std::string tmp = buffer.substr(strCursor, blankPos - strCursor);

				animations[i].time[count] = atof(tmp.c_str());

				strCursor = blankPos + 1;
			}
		}

		XmlElementPtr transformNode = sourceList->GetElement(1);
		{
			std::string buffer;

			transformNode->GetChildElement("float_array")->GetAttribute("count", buffer);
			size_t transformCount = atoi(buffer.c_str()) / 16;
			animations[i].transfrom.resize(transformCount);

			transformNode->GetChildElement("float_array")->GetText(buffer);

			size_t strCursor = 0;
			for (size_t count = 0; count < transformCount; count++)
			{
				size_t blankPos = buffer.find('\n', strCursor + 1);
				std::string tmp = buffer.substr(strCursor, blankPos - strCursor);

				animations[i].transfrom[count] = StringToMat4(tmp);

				strCursor = blankPos + 1;
			}
		}

		XmlElementPtr interpolationNode = sourceList->GetElement(2);
		{
			std::string buffer;

			interpolationNode->GetChildElement("Name_array")->GetAttribute("count", buffer);
			size_t interpolationCount = atoi(buffer.c_str());
			animations[i].interpolation.resize(interpolationCount);

			interpolationNode->GetChildElement("Name_array")->GetText(buffer);
			ReplaceCharFromString(buffer, '\n', ' ');

			size_t strCursor = 0;
			for (size_t count = 0; count < interpolationCount; count++)
			{
				size_t blankPos = buffer.find(' ', strCursor + 1);
				std::string tmp = buffer.substr(strCursor, blankPos - strCursor);

				animations[i].interpolation[count] = tmp;

				strCursor = blankPos + 1;
			}
		}

		XmlElementPtr channelNode = animationNode->GetChildElement("animation")->GetChildElement("channel");
		std::string targetBuf;
		channelNode->GetAttribute("target", targetBuf);
		size_t offset = targetBuf.find('/');
		animations[i].target = targetBuf.substr(0, offset);
	}
}

void DaeParser::_LibraryVisualScene(Dae_Data & data)
{
	_ANALYZE_();
	XmlElementPtr visualRootNode = xmlParser->GetElement("COLLADA/library_visual_scenes");
	if (visualRootNode == nullptr) { return; }
	auto& visualScenes = data.visualScenes;

	XmlElementListPtr visualSceneList = visualRootNode->GetChildElementList("visual_scene");
	data.visualScenes.resize(visualSceneList->GetLength());

	std::function<void(const std::string& parentID, XmlElementListPtr nodeList, const size_t visualSceneIndex)> parseHierRecursive;
	parseHierRecursive = [&](const std::string& parentID, XmlElementListPtr nodeList,const size_t visualSceneIndex) {
		if (nodeList == nullptr) { return; }

		for (size_t index = 0; index < nodeList->GetLength(); index++)
		{
			XmlElementPtr node = nodeList->GetElement(index);
			
			std::string buf;
			Dae_VisualScene::Dae_Node nodeBuf;

			nodeBuf.parentID = parentID;

			node->GetAttribute("name",buf);
			if (buf == "") { return; }
			nodeBuf.name = buf;

			node->GetAttribute("id", buf);
			nodeBuf.id = buf;

			node->GetAttribute("type", buf);
			nodeBuf.type = buf;

			XmlElementPtr matrixNode = node->GetChildElement("matrix");
			Mat4 matrix;
			if (matrixNode != nullptr)
			{
				matrixNode->GetText(buf);
				matrix = StringToMat4(buf);
			}
			else
			{
				matrix._11 = 1.0f; matrix._12 = 0.0f; matrix._13 = 0.0f; matrix._14 = 0.0f;
				matrix._21 = 0.0f; matrix._22 = 1.0f; matrix._23 = 0.0f; matrix._24 = 0.0f;
				matrix._31 = 0.0f; matrix._32 = 0.0f; matrix._33 = 1.0f; matrix._34 = 0.0f;
				matrix._41 = 0.0f; matrix._42 = 0.0f; matrix._43 = 0.0f; matrix._44 = 1.0f;
			}
			nodeBuf.matrix = matrix;

			data.visualScenes[visualSceneIndex].nodes.push_back(nodeBuf);

			parseHierRecursive(nodeBuf.id, node->GetChildElementList("node"), visualSceneIndex);
		}
	};

	for (size_t i = 0; i < visualSceneList->GetLength(); i++)
	{
		XmlElementPtr visualSceneNode = visualSceneList->GetElement(i);

		GetIDAndNameAttribute(visualSceneNode, visualScenes[i].id, visualScenes[i].name);

		parseHierRecursive("", visualSceneNode->GetChildElementList("node"), i);

		XmlElementListPtr tecniqueNode = (visualSceneNode->GetChildElement("extra"))->GetChildElementList("technique");
		std::string buffer;

		XmlElementPtr frameRateNode = (tecniqueNode->GetElement(0))->GetChildElement("frame_rate");
		frameRateNode->GetText(buffer);
		visualScenes[i].frameRatePerSec = static_cast<float>(atof(buffer.c_str()));

		XmlElementPtr startTimeNode = (tecniqueNode->GetElement(1))->GetChildElement("start_time");
		startTimeNode->GetText(buffer);
		visualScenes[i].startTimeSec = static_cast<float>(atof(buffer.c_str()));

		XmlElementPtr endTimeNode = (tecniqueNode->GetElement(1))->GetChildElement("end_time");
		endTimeNode->GetText(buffer);
		visualScenes[i].endTimeSec = static_cast<float>(atof(buffer.c_str()));
	}
}

DaeParser::DaeParser()
{
	xmlParser = new XmlParser();
}

DaeParser::~DaeParser()
{
	delete xmlParser;
}

void DaeParser::Parse(const char * daePath, Dae_Data & data)
{
	_ANALYZE_();

	memset(&data, 0, sizeof(Dae_Data));

	if (!xmlParser->OpenXML(daePath)) 
	{ 
		return; 
	}

	_LibraryImages(data);
	_LibraryMaterials(data);
	_LibraryEffects(data);
	_LibraryGeometries(data);
	_LibraryVisualScene(data);
	_LibraryController(data);
	_LibraryAnimations(data);
}
