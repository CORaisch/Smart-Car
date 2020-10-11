#include "CollisionDetection/Collision_Detection.h"

//Macros
#define MIN(A, B)	((A) < (B) ? (A) : (B))
#define MAX(A, B)	((A) > (B) ? (A) : (B))
#define SIGN(A)		((A) < (0) ? (-1) : (1))

/*Easy Line Intersection Algorithm. Not for 3D!*/
bool intersectTriangles(const std::vector<Rendering::VertexFormat>& vertsA, const glm::mat4& modelA, const std::vector<Rendering::VertexFormat>& vertsB, const glm::mat4& modelB) {
	/* Da sich alle Dreiecke in der selben Ebene befinden [(x,y,0)], ist die schnellste Variante zu pruefen ob 2 */
	/* Dreiecke sich schneiden, indem man prueft ob die einzelnen Linien der Dreiecke sich schneiden.            */

	glm::vec2 vA[4], vB[4];
	glm::vec4 tA[3], tB[3];
	double A0, B0, C0, A1, B1, C1;
	double det, x, y, minx, miny, maxx, maxy;
	bool range0, range1;
	//Iterate through all Triangles of A
	for (int iA = 0; iA < vertsA.size(); iA += 3) { //3 Vertices per Triangle
		//Transform current Triangle A
		tA[0] = modelA * vertsA[iA].position;
		tA[1] = modelA * vertsA[iA + 1].position;
		tA[2] = modelA * vertsA[iA + 2].position;
		//Get current Triangle A
		vA[0] = glm::vec2(tA[0].x, tA[0].y);
		vA[1] = glm::vec2(tA[1].x, tA[1].y);
		vA[2] = glm::vec2(tA[2].x, tA[2].y);
		vA[3] = vA[0];
		//Iterate through all Lines of current Triangle A
		for (int jA = 0; jA < 3; jA++) {
			//Find Line Equation: A0*x + B0*y = C0, using Points vA[jA] and vA[jA+1]
			A0 = vA[jA + 1].y - vA[jA].y;
			B0 = vA[jA].x - vA[jA + 1].x;
			C0 = A0*vA[jA].x + B0*vA[jA].y;
			//For each Triangle of A Iterate through all Triangles of B
			for (int iB = 0; iB < vertsB.size(); iB += 3) { //3 Vertices per Triangle
				//Transform current Triangle B
				tB[0] = modelB * vertsB[iB].position;
				tB[1] = modelB * vertsB[iB + 1].position;
				tB[2] = modelB * vertsB[iB + 2].position;
				//Get current Triangle B
				vB[0] = glm::vec2(tB[0].x, tB[0].y);
				vB[1] = glm::vec2(tB[1].x, tB[1].y);
				vB[2] = glm::vec2(tB[2].x, tB[2].y);
				vB[3] = vB[0];
				//Iterate through all Lines of current Triangle B
				for (int jB = 0; jB < 3; jB++) {
					//Find Line Equation: A1*x + B1*x = C1, using Points vB[jB] and vB[jB+1]
					A1 = vB[jB + 1].y - vB[jB].y;
					B1 = vB[jB].x - vB[jB + 1].x;
					C1 = A1*vB[jB].x + B1*vB[jB].y;

					//Check Line Intersection
					det = A0*B1 - A1*B0;
					if (det != 0) { //If det == 0, Lines are Parallel. We dont have to check any more
						//Find intersection Points
						x = (B1*C0 - B0*C1) / det;
						y = (A0*C1 - A1*C0) / det;
						//Check if Intersection Point is inside Line Segment defined by vA[jA] and vA[jA+1]
						minx = MIN(vA[jA].x, vA[jA + 1].x);
						maxx = MAX(vA[jA].x, vA[jA + 1].x);
						miny = MIN(vA[jA].y, vA[jA + 1].y);
						maxy = MAX(vA[jA].y, vA[jA + 1].y);
						range0 = (minx <= x && x <= maxx) && (miny <= y && y <= maxy);
						//Check if Intersection Point is inside Line Segment defined by vB[jB] and vB[jB+1]
						minx = MIN(vB[jB].x, vB[jB + 1].x);
						maxx = MAX(vB[jB].x, vB[jB + 1].x);
						miny = MIN(vB[jB].y, vB[jB + 1].y);
						maxy = MAX(vB[jB].y, vB[jB + 1].y);
						range1 = (minx <= x && x <= maxx) && (miny <= y && y <= maxy);
						if (range0 && range1)
							return true;
					}
				}
			}
		}
	}
	return false;
}

/*'ray_pos' and 'ray_dir' expected to be normalized*/
float intersectRayTriangles(const glm::vec4& ray_pos, const glm::vec4& ray_dir, const glm::mat4& modelMat, const std::vector<Rendering::VertexFormat>& verts) {
	//Definitions
	glm::vec4 t[3];
	glm::vec2 a[4], v1, v2;
	float t1, t2, det;
	float t_nearest = FLT_MAX;
	//Define v3
	glm::vec2 v3(-ray_dir.y, ray_dir.x);
	//Iterate through all Triangles in 'verts'
	for (int i = 0; i < verts.size(); i += 3) {
		//Define t0, t1 and t2 (Transformed Vertices)
		t[0] = modelMat * verts[i].position;
		t[1] = modelMat * verts[i + 1].position;
		t[2] = modelMat * verts[i + 2].position;
		//Define a0, a1 and a2
		a[0] = glm::vec2(t[0].x, t[0].y);
		a[1] = glm::vec2(t[1].x, t[1].y);
		a[2] = glm::vec2(t[2].x, t[2].y);
		a[3] = a[0];
		//Iterate through all Line-Segments of current Triangle
		for (int j = 0; j < 3; j++) {	//Current Line-Seqment defined From a[j] to a[j+1]
			//Define v1 and v2
			v1 = glm::vec2(ray_pos.x, ray_pos.y) - a[j];
			v2 = a[j + 1] - a[j];
			//Find t1 and t2
			det = v2.x*v1.y - v2.y*v1.x;
			t1 = (SIGN(det)*glm::length(det)) / glm::dot(v2, v3);
			t2 = glm::dot(v1, v3) / glm::dot(v2, v3);
			//Check for Collision
			if (t1 >= 0 && t2 >= 0 && t2 <= 1) {
				//Lines intersected)
				if (t1 < t_nearest)
					t_nearest = t1;
			}
		}
	}
	//Return t of nearest Intersection
	return t_nearest;
}
