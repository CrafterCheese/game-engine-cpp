#pragma once

#include <tinygltf/tiny_gltf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>

#pragma once

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <emscripten/html5.h>
	#include <GLES3/gl3.h>
#else
	#include <GL/glew.h>
#endif

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

struct alignas(16) MeshDraw
{
    GLuint vao;
    GLsizei count;
    GLenum indexType;
    GLenum mode;
	int textureIndex = -1;
};

struct alignas(16) rayHit
{
	float distance;
	glm::vec3 normal;
};
struct alignas(16) boundingbox
{
	glm::vec3 min;
	glm::vec3 max;

	std::optional<rayHit> intersectRay(const glm::vec3& ray, const glm::vec3& origin);
};
inline std::optional<rayHit> boundingbox::intersectRay(const glm::vec3& ray, const glm::vec3& origin)
{
	glm::vec3 dirFrac = 1.0f / ray;

	float t1 = (min.x - origin.x) * dirFrac.x;
	float t2 = (max.x - origin.x) * dirFrac.x;
	float t3 = (min.y - origin.y) * dirFrac.y;
	float t4 = (max.y - origin.y) * dirFrac.y;
	float t5 = (min.z - origin.z) * dirFrac.z;
	float t6 = (max.z - origin.z) * dirFrac.z;

	float tmin = std::max({std::min(t1, t2), std::min(t3, t4), std::min(t5, t6)});
	float tmax = std::min({std::max(t1, t2), std::max(t3, t4), std::max(t5, t6)});

	if (tmax < 0 || tmin > tmax) {return std::nullopt;}
	float t = (tmin >= 0) ? tmin : tmax;

	glm::vec3 normal(0.0f);
	if (t == t1 || t == t2) {normal.x = (t == t1) ? -1.0f : 1.0f;}
    else if (t == t3 || t == t4) {normal.y = (t == t3) ? -1.0f : 1.0f;}
    else if (t == t5 || t == t6) {normal.z = (t == t5) ? -1.0f : 1.0f;}

	return rayHit{t, normal};
}

bool loadModel(tinygltf::Model& model, const std::string& filename);

GLuint createTexture(const tinygltf::Model& model, int index);

class Model
{
	public:
		std::vector<MeshDraw> meshdata;
		std::vector<boundingbox> boundingboxes;
		boundingbox aabb;

		Model(const char* filename);
		void drawModel();
		void drawDepth();
		~Model();

	private:
		std::unordered_map<int, GLuint> textureMap;

		void bindNode(tinygltf::Model& model, const tinygltf::Node& node);
		void bindMesh(tinygltf::Model& model, tinygltf::Mesh& mesh);
		void bindAttrib(tinygltf::Model& model, int binding, int vecSize, int attribPos, bool collision);
		void createTexture(const tinygltf::Model& model, int index);
};