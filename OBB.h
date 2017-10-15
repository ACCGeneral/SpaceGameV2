#pragma once

#include <glm.hpp> // This is the main GLM header
#include <gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <vector>
#include <memory>
#include "projection.h"
#include "mesh.h"

//class for OBB collision

class OBB
{
private:

  glm::vec3 center;
  glm::mat3 rotation;
  glm::quat myquad;
  std::vector<glm::vec3> axes;
  std::vector<glm::vec3> origvertxes;
  std::vector<glm::vec3> vertexes;


public:

  OBB();

   void addobbsetup(std::vector<glm::vec3> meshes);
   void obbsetup(std::vector<mesh> meshes);
   void update(glm::vec3 pos,glm::quat newqu);
   bool collisontest2(std::shared_ptr<OBB> B, glm::vec3 &axisofcol, float &collover);
   bool collisontest1(std::shared_ptr<OBB> B);

   glm::mat3 returnrotation();

   projection projectonaxes(glm::vec3 axes);

   glm::vec3 returncenter()
   {
	   return center;
   }

};

//reference to http://www.dyn4j.org/2010/01/sat/#sat-mtv for help with collision theory