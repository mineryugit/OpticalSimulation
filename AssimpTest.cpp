#include <stdio.h>
#include <string>
using namespace std;
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#pragma comment(lib, "assimp-vc120-mt.lib")

//const string INPUTFILE = "../../data/model/bunny/reconstruction/bun_zipper_res3.ply";
const string INPUTFILE = "bun_zipper_res4.ply";

//assipmは直置き

void main(){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(INPUTFILE, 0);
	if (!scene){ printf("load error"); }
	printf("have camera?%d\n", scene->HasCameras());
	printf("have mesh?%d\n", scene->HasMeshes());
	printf("num of mesh:%d\n", scene->mNumMeshes);
	aiMesh* assimp_mesh = scene->mMeshes[0];
	printf("have born?%d\n", assimp_mesh->HasBones());
	printf("%d\n", assimp_mesh->HasFaces());
	printf("%d\n", assimp_mesh->mNumFaces);
	printf("%d\n", assimp_mesh->mFaces[0].mNumIndices);//3角系なら0,1,2..となっている
	printf("normal:%d\n", assimp_mesh->HasNormals());
	printf("%d\n", assimp_mesh->HasPositions());
	printf("%d\n", assimp_mesh->mNumVertices);//÷3して丁度みたいだ。
}
