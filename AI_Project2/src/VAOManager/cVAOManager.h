#pragma once
#include "..\globalOpenGL.h"
#include "cModelDrawInfo.h"
#include "..\XML\cXML.h"
#include <string>
#include <map>
#include <fstream>
#include "../MeshObj/cMeshObj.h"

#define MODEL_LOAD_BUFFER 10000
class cVAOManager
{
public:
	cVAOManager();
	~cVAOManager();

	struct sTrianglePLY
	{
		// The 3 vertex index values from the ply file
		unsigned int vertexIndices[3];
	};

	bool loadModelToVAO(std::string filename, cModelDrawInfo& drawInfo, unsigned int shaderProgramID);
	bool loadModelList(std::string filename, unsigned int shaderProgramID);
	bool FindDrawInfo(std::string filename, cModelDrawInfo& drawInfo);
	bool loadPLYFile(std::string filename, cModelDrawInfo& modelDrawInfo, std::string error);
	bool setInstanceObjScale(std::string meshObjName, float value);
	bool setInstanceObjWireframe(std::string meshObjName, bool value);
	bool setInstanceObjRGB(std::string meshObjName, glm::vec4 value);
	bool setInstanceObjVisible(std::string meshObjName, bool value);
	bool setInstanceObjLighting(std::string meshObjName, bool value);
	bool setInstanceObjSpecularPower(std::string meshObjName, glm::vec4 value);

	bool setInstanceObjPosition(std::string meshObjName, glm::vec4 value);

	glm::vec4 getInstanceObjPosition(std::string meshObjName);

	cMeshObj* findMeshObjAddr(std::string meshObjName);

	bool setTexture(std::string meshObjName, std::string textureFile, int arrPos);

	bool setTextureRatio(std::string meshObjName, int arrPos, float ratio);

	bool bindingChild(std::string meshChildObjName, std::string meshParentObjName);

	bool setSkyBoxFlag(std::string meshObjName, bool flag);

	bool setIslandModelFlag(std::string meshObjName, bool flag);

	bool setUseRGBColorFlag(std::string meshObjName, bool flag);

	bool setDungeonTexture(std::string meshObjName, std::string textureFile);

	bool setTorchTexture(std::string meshObjName, std::string textureFile, std::string markTextureFile);

	void createNewMeshOBJ(std::string InstantName, std::string ModelName,glm::vec3 pos, glm::vec3 rotate, glm::vec3 scale);

	std::map<std::string, cMeshObj*> mapInstanceNametoMeshObj;
	std::vector<cMeshObj*> pVecInstanceMeshObj;
	glm::vec3 cameraEyeFromXML;

private:
	std::map<std::string, cModelDrawInfo> mapModelNametoVAOID;
};

