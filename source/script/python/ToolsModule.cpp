//
//  ToolsModule.cpp
//  Epsilon
//
//  Created by Scott Porter on 26/05/2014.
//

#include "ToolsModule.h"

#include "tools/procedural/Perlin.h"

void initTools()
{
	using namespace epsilon;
    
	// make Tools package
	object toolsModule( handle<>( borrowed( PyImport_AddModule("epsilon.tools") ) ) );
	scope().attr("tools") = toolsModule;
	scope toolsScope = toolsModule;
    
    
    //
    // The procedural module
    //
    
    object proceduralModule(handle<>(borrowed(PyImport_AddModule("epsilon.tools.procedural"))));
	toolsScope.attr("procedural") = proceduralModule;
	scope proceduralScope = proceduralModule;

    object perlin = class_<Perlin, boost::noncopyable>("perlin", no_init)
        .def("noise", &Perlin::Noise1D, (python::arg("wrap_x") = (int)(0) ))
    
        .def("noise", &Perlin::Noise2D, (python::arg("wrap_x") = (int)(0) )
                                      , (python::arg("wrap_y") = (int)(0) ))
    
        .def("noise", &Perlin::Noise2Dv)
    
        .def("noise", &Perlin::Noise3Dvi, (python::arg("wrap_x") = (int)(0) )
                                        , (python::arg("wrap_y") = (int)(0) )
                                        , (python::arg("wrap_z") = (int)(0) ))
    
        .def("noise", &Perlin::Noise3Dv, (python::arg("wrap") = (Vector3)(Vector3::ZERO) ))
    
        .def("noise", &Perlin::Noise3D, (python::arg("x_wrap") = (int)(0) )
                                      , (python::arg("y_wrap") = (int)(0) )
                                      , (python::arg("z_wrap") = (int)(0) ))
    

        .staticmethod("noise")
    ;
    
}