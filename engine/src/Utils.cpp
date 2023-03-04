#include "Utils.h"

namespace RoomEng{
	namespace Utils{
		glm::mat4 aiMat2glmMat(aiMatrix4x4 mat){
			glm::mat4 newMat;
			newMat[0][0] = mat.a1; newMat[1][0] = mat.a2; newMat[2][0] = mat.a3; newMat[3][0] = mat.a4;
			newMat[0][1] = mat.b1; newMat[1][1] = mat.b2; newMat[2][1] = mat.b3; newMat[3][1] = mat.b4;
			newMat[0][2] = mat.c1; newMat[1][2] = mat.c2; newMat[2][2] = mat.c3; newMat[3][2] = mat.c4;
			newMat[0][3] = mat.d1; newMat[1][3] = mat.d2; newMat[2][3] = mat.d3; newMat[3][3] = mat.d4;
			return newMat;
		}
	}
}