#include "Context.h"
#include "Visualizer.h"
#include "RadiationModel.h"
#include "EnergyBalanceModel.h"
#include "CanopyGenerator.h"
#include "SolarPosition.h"
#include "PlantArchitecture.h"

using namespace helios;

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
   plantarchitecture.loadPlantModelFromLibrary("grapevine_VSP_IL");
    //plantarchitecture.enableGroundClipping(); //for a custom ground height, pass the height as argument
   int plantID = plantarchitecture.buildPlantInstanceFromLibrary(nullorigin,0);
   plantarchitecture.advanceTime(365);
    float height = plantarchitecture.getPlantHeight(plantID);
	float age = plantarchitecture.getPlantAge(plantID);
	std::vector prim_leafs = plantarchitecture.getAllLeafUUIDs();
	std::vector prim_flowers = plantarchitecture.getAllFlowerUUIDs();
	std::vector prim_fruits = plantarchitecture.getAllFruitUUIDs();
	std::vector prim_plant=plantarchitecture.getAllPlantUUIDs(plantID);
	long int no_of_flowers = prim_flowers.size();
	long int no_of_leafs = prim_leafs.size();
	long int no_of_fruits = prim_fruits.size();
	long int no_plant = prim_plant.size();                                                                     
	//
   std::cout<< "Height: " << height<< std::endl;
   std::cout<< "Age: " << age<< std::endl;
   std::cout<<"flower primitive number "<<no_of_flowers<< std::endl;
   std::cout<<"fruits primitive number "<<no_of_fruits<< std::endl;
   std::cout<<"leaf primitive number "<<no_of_leafs<< std::endl;
   std::cout<<"plant primitive number "<<no_plant<< std::endl;
  ///////////////////////

	Visualizer vis_nir(500);
	//vis_nir.colorContextPrimitivesByObjectData("rank");
	vis_nir.buildContextGeometry(&context);
    vis_nir.plotInteractive(); //open an interactive graphics window 
	std::cout<<"DONE"<<std::endl; 

}
 
