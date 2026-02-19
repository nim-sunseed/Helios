#include "Context.h"
#include "Visualizer.h"
#include "RadiationModel.h"
#include "EnergyBalanceModel.h"
#include "CanopyGenerator.h"
#include "SolarPosition.h"
#include "PlantArchitecture.h"

using namespace helios;

  struct Bounds {
    double minx =  std::numeric_limits<double>::infinity();
    double miny =  std::numeric_limits<double>::infinity();
    double minz =  std::numeric_limits<double>::infinity();
    double maxx = -std::numeric_limits<double>::infinity();
    double maxy = -std::numeric_limits<double>::infinity();
    double maxz = -std::numeric_limits<double>::infinity();

    void expand(double x, double y, double z) {
      if (x < minx) minx = x;
      if (y < miny) miny = y;
      if (z < minz) minz = z;
      if (x > maxx) maxx = x;
      if (y > maxy) maxy = y;
      if (z > maxz) maxz = z;
    }
    bool valid() const { return minx <= maxx && miny <= maxy && minz <= maxz; }
  };

int main(){

	//DEFINE A GROUND PATCH 
	Context context;
	vec3 center_ground(0,0,0);
	vec2 size_ground(1,1);
	int2 ndiv(10,10);      //note the int2, doesnt work with vec2
	int2 ndiv1(10,10);
	vec3 increment(1,0,0);
	SphericalCoord rot(1,0,0);  //zero rot, first coord is radius
	std::vector<uint> tileid = context.addTile(center_ground,size_ground,rot,ndiv); //Added ground to context

   /// plant architecture plugin here

   PlantArchitecture plantarchitecture(&context); 
   //plantarchitecture.optionalOutputObjectData("rank");
   plantarchitecture.loadPlantModelFromLibrary("dragonfruit");
    //plantarchitecture.enableGroundClipping(); //for a custom ground height, pass the height as argument
   int UUID_crop = plantarchitecture.buildPlantInstanceFromLibrary(nullorigin,0);
   plantarchitecture.advanceTime(340);


   //////////////
    float height = plantarchitecture.getPlantHeight(UUID_crop);
	float age = plantarchitecture.getPlantAge(UUID_crop);
	std::vector prim_leafs = plantarchitecture.getAllLeafUUIDs();
	std::vector prim_flowers = plantarchitecture.getAllFlowerUUIDs();
	std::vector prim_fruits = plantarchitecture.getAllFruitUUIDs();
	std::vector prim_plant=plantarchitecture.getAllPlantUUIDs(UUID_crop);
	long int no_of_flowers = prim_flowers.size();
	long int no_of_leafs = prim_leafs.size();
	long int no_of_fruits = prim_fruits.size();
	long int no_plant = prim_plant.size(); 
	//context.get	                                                                    
	//
   std::cout<< "Height: " << height<< std::endl;
   std::cout<< "Age: " << age<< std::endl;
   std::cout<<"flower primitive number "<<no_of_flowers<< std::endl;
   std::cout<<"fruits primitive number "<<no_of_fruits<< std::endl;
   std::cout<<"leaf primitive number "<<no_of_leafs<< std::endl;
   std::cout<<"plant primitive number "<<no_plant<< std::endl;
   
  ///////////////////////

  // --------------------------
  // 1) Use writePlantMeshVertices() to export vertices to a file
  // --------------------------
  std::string vert_filename = "plant_" + std::to_string(UUID_crop) + "_verts.txt";
  // NOTE: signature may vary slightly depending on your Helios build.
  // If the function returns bool/void adjust as needed.
  plantarchitecture.writePlantMeshVertices(UUID_crop, vert_filename);

  // --------------------------
  // 2) Parse the file and compute bounds
  //    (Assumes file contains numeric triples like: x y z per line.
  //     The parser skips lines that don't contain 3 doubles.)
  // --------------------------
  Bounds b;
  std::ifstream ifs(vert_filename);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open vertex file: " << vert_filename << "\n";
  } else {
    std::string line;
    while (std::getline(ifs, line)) {
      if (line.empty()) continue;
      std::istringstream ss(line);
      double x, y, z;
      if (!(ss >> x >> y >> z)) {
        // try to ignore header or non-numeric lines
        continue;
      }
      b.expand(x,y,z);
    }
    ifs.close();

    if (!b.valid()) {
      std::cout << "No valid vertices found in " << vert_filename << "\n";
    } else {
      double x_span = b.maxx - b.minx;
      double y_span = b.maxy - b.miny;
      double height_span = b.maxz - b.minz; // z assumed vertical
      std::cout << std::fixed << std::setprecision(4);
      std::cout << "Computed bounds for plant " << UUID_crop << " (from " << vert_filename << ")\n";
      std::cout << "  x: [" << b.minx << ", " << b.maxx << "] span = " << x_span << "\n";
      std::cout << "  y: [" << b.miny << ", " << b.maxy << "] span = " << y_span << "\n";
      std::cout << "  z: [" << b.minz << ", " << b.maxz << "] height = " << height_span << "\n";
    }
  }



  //////////////////////

	Visualizer vis_nir(500);
	//vis_nir.colorContextPrimitivesByObjectData("rank");
	vis_nir.buildContextGeometry(&context);
    vis_nir.plotInteractive(); //open an interactive graphics window 
	std::cout<<"DONE"<<std::endl; 

}
 
