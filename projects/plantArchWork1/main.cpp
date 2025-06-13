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

   PlantArchitecture plantarchitecture(&context); //SS_Okra
   plantarchitecture.loadPlantModelFromLibrary("strawberry_annual");
   plantarchitecture.buildPlantInstanceFromLibrary(nullorigin,0);
   plantarchitecture.advanceTime(400);

  ///////////////////////

	Visualizer vis_nir(500);
	vis_nir.buildContextGeometry(&context);
    vis_nir.plotInteractive(); //open an interactive graphics window 
	std::cout<<"DONE"<<std::endl; 

}
 
