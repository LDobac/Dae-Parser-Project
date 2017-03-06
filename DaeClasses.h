#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Math.h"

struct Dae_Basic
{
	std::string id;
	std::string name;
};

struct Dae_Image
	: public Dae_Basic
{
	std::string path;
};

struct Dae_Material
	: public Dae_Basic
{
	std::string sourceEffect;
};

struct Dae_Effect
	: public Dae_Basic
{
	struct Dae_Texture 
	{
		std::string sourceImage;
		std::string texcoord;

		bool wrapU;
		bool wrapV;

		char blendMode[10];
	};

	Dae_Texture textrue;

	Vec4 emission;
	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;

	float shininess;

	Vec4 reflective;
	float reflectivity;

	Vec4 transparent;
	float transparency;
};

struct Dae_Geometry
	: public Dae_Basic
{
	struct Dae_Index { unsigned short _0, _1, _2; };
	struct Dae_Triangle
	{
		size_t count;

		std::string materialSource;

		std::vector<Dae_Index> positionIndex;
		std::vector<Dae_Index> normalIndex;
		std::vector<Dae_Index> uvIndex;
	};

	std::vector<Vec3> position;
	std::vector<Vec3> normal;
	std::vector<Vec2> uv;

	std::vector<Dae_Triangle> triangles;
};

struct Dae_Controller
	: public Dae_Basic
{
	struct Dae_Skin
	{
		struct Dae_Joint
		{
			std::string name;

			Mat4 matrix;
		};

		std::string source;

		Mat4 bindShapeMatrix;

		std::vector<Dae_Joint> joints;
		std::vector<float> weights;
		std::vector<size_t> vertexInfluenceCount;
		std::vector<std::pair<size_t, size_t>> jointWeightIndex;
	};

	Dae_Skin skin;
};

struct Dae_Animation
	:public Dae_Basic
{
	std::vector<float> time;
	std::vector<Mat4> transfrom;
	std::vector<std::string> interpolation;

	std::string target;
};

struct Dae_VisualScene
	: public Dae_Basic
{
	struct Dae_Node : public Dae_Basic
	{
		std::string parentID;

		std::string type;

		Mat4 matrix;
	};

	std::vector<Dae_Node> nodes;

	float frameRatePerSec;
	float startTimeSec;
	float endTimeSec;
};

struct Dae_Data
{
	std::vector<Dae_Image> images;
	std::vector<Dae_Material> materials;
	std::vector<Dae_Effect> effects;
	std::vector<Dae_Geometry> geometries;
	std::vector<Dae_Controller> controllers;
	std::vector<Dae_Animation> animations;
	std::vector<Dae_VisualScene> visualScenes;
};