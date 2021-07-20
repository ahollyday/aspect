/*
  Copyright (C) 2011 - 2019 by the authors of the ASPECT code.

  This file is part of ASPECT.

  ASPECT is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  ASPECT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ASPECT; see the file LICENSE.  If not see
  <http://www.gnu.org/licenses/>.
*/


#include <aspect/boundary_traction/GIA_traction.h>
#include <aspect/global.h>
#include <aspect/postprocess/sealevel.h>

namespace aspect
{
  namespace BoundaryTraction
  {
    template <int dim>
    GIAtraction<dim>::GIAtraction ()
    // void
    // GIAtraction<dim>::initialize()
    {}



    template <int dim>
    Tensor<1,dim>
    GIAtraction<dim>::
    boundary_traction (const types::boundary_id,
                       const Point<dim> &position,
                       const Tensor<1,dim> &normal_vector) const
    {
      // const double surface_load = SeaLevel<dim>::sea_level_equation(position);
      Postprocess::SeaLevel<dim> sealevel;
      const double surface_load = sealevel.sea_level_equation(position);
      return -surface_load * normal_vector;
    }



    template <int dim>
    void
    GIAtraction<dim>::update()
    {
      Interface<dim>::update ();
    }
  


    template <int dim>
    void
    GIAtraction<dim>::declare_parameters (ParameterHandler &prm)
    {
      prm.enter_subsection("Boundary traction model");
      {
        prm.enter_subsection("GIA traction");
        {
          // prm.declare_entry ("Coordinate system", "cartesian",
          //                    Patterns::Selection ("cartesian|spherical|depth"),
          //                    "A selection that determines the assumed coordinate "
          //                    "system for the function variables. Allowed values "
          //                    "are `cartesian', `spherical', and `depth'. `spherical' coordinates "
          //                    "are interpreted as r,phi or r,phi,theta in 2D/3D "
          //                    "respectively with theta being the polar angle. `depth' "
          //                    "will create a function, in which only the first "
          //                    "parameter is non-zero, which is interpreted to "
          //                    "be the depth of the point.");

          // GIAtraction::ParsedFunction<dim>::declare_parameters (prm, dim);
          // Utilities::AsciiDataBoundary<dim>::declare_parameters(prm,
                                                              // "$ASPECT_SOURCE_DIR/data/boundary-traction/ascii-data/test/",
                                                              // "box_2d_%s.%d.txt");
        }
        prm.leave_subsection();
      }
      prm.leave_subsection();
    }


    template <int dim>
    void
    GIAtraction<dim>::parse_parameters (ParameterHandler &prm)
    {
      prm.enter_subsection("Boundary traction model");
      {
        prm.enter_subsection("GIAtraction");
        {
          // coordinate_system = Utilities::Coordinates::string_to_coordinate_system(prm.get("Coordinate system"));
        }
        // try
        //   {
        //     boundary_traction_function.parse_parameters (prm);
        //   }
        // catch (...)
        //   {
        //     std::cerr << "ERROR: FunctionParser failed to parse\n"
        //               << "\t'Boundary traction model.Function'\n"
        //               << "with expression\n"
        //               << "\t'" << prm.get("Function expression") << "'"
        //               << "More information about the cause of the parse error \n"
        //               << "is shown below.\n";
        //     throw;
        //   }
        prm.leave_subsection();
      }
      prm.leave_subsection();
    }

  }
}

// explicit instantiations
namespace aspect
{
  namespace BoundaryTraction
  {
    ASPECT_REGISTER_BOUNDARY_TRACTION_MODEL(GIAtraction,
                                            "GIA traction",
                                            "Implementation of a model in which the boundary "
                                            "traction is given in terms of an explicit formula "
                                            "that is elaborated in the parameters in section "
                                            "``Boundary traction model|Function''. "
                                            "\n\n"
                                            "The formula you describe in the mentioned "
                                            "section is a semicolon separated list of traction components "
                                            "for each of the $d$ components of the traction vector. "
                                            "These $d$ formulas are interpreted as having units "
                                            "Pa.")
  }
}

