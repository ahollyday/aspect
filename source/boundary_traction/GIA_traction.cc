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

namespace aspect
{
  namespace BoundaryTraction
  {
    template <int dim>
    GIAtraction<dim>::GIAtraction ()
    {}

    template <int dim>
    Tensor<1,dim>
    Function<dim>::
    boundary_traction (const types::boundary_id,
                       const Point<dim> &position,
                       const Tensor<1,dim> &) const
    {
      // Tensor<1,dim> traction;
      // Utilities::NaturalCoordinate<dim> point =
      //   this->get_geometry_model().cartesian_to_other_coordinates(position, coordinate_system);
      // for (unsigned int d=0; d<dim; ++d)
      //   traction[d] = boundary_traction_function.value(Utilities::convert_array_to_point<dim>(point.get_coordinates()),d);

      // return traction;
    }

    // template <int dim>
    // void
    // GIAtraction<dim>::initialize ()
    // {
    //   for (const auto &bv : this->get_boundary_traction())
    //     if (bv.second.get() == this)
    //       boundary_ids.insert(bv.first);

    //   AssertThrow(*(boundary_ids.begin()) != numbers::invalid_boundary_id,
    //               ExcMessage("Did not find the boundary indicator for the traction ascii data plugin."));

    //   Utilities::AsciiDataBoundary<dim>::initialize(boundary_ids,
    //                                                 1);
    // }

    // MAYBE USE THIS FOR ACCESSING GEOID POSTPROCESSOR?
    // template <>
    // std::pair<std::pair<double, std::pair<std::vector<double>,std::vector<double> > >, std::pair<double, std::pair<std::vector<double>,std::vector<double> > > >
    // Geoid<3>::dynamic_topography_contribution(const double &outer_radius,
    //                                           const double &inner_radius) const
    // {
    //   // Get a pointer to the dynamic topography postprocessor.
    //   const Postprocess::DynamicTopography<3> &dynamic_topography =
    //     this->get_postprocess_manager().template get_matching_postprocessor<Postprocess::DynamicTopography<3> >();
    // }

    // template <int dim>
    // Tensor<1,dim>
    // AsciiData<dim>::
    // boundary_traction (const types::boundary_id boundary_indicator,
    //                    const Point<dim> &position,
    //                    const Tensor<1,dim> &normal_vector) const
    // {
    //   const double pressure = Utilities::AsciiDataBoundary<dim>::get_data_component(boundary_indicator,
    //                                                                                 position,
    //                                                                                 0);
    //   return -pressure * normal_vector;;
    // }


    // template <int dim>
    // void
    // AsciiData<dim>::update()
    // {
    //   Interface<dim>::update ();
    //   Utilities::AsciiDataBoundary<dim>::update();
    // }


 template <int dim>
    void
    GIAtraction<dim>::declare_parameters (ParameterHandler &prm)
    {
      prm.enter_subsection("Boundary traction model");
      {
        prm.enter_subsection("GIAtraction");
        {
          prm.declare_entry ("Coordinate system", "cartesian",
                             Patterns::Selection ("cartesian|spherical|depth"),
                             "A selection that determines the assumed coordinate "
                             "system for the function variables. Allowed values "
                             "are `cartesian', `spherical', and `depth'. `spherical' coordinates "
                             "are interpreted as r,phi or r,phi,theta in 2D/3D "
                             "respectively with theta being the polar angle. `depth' "
                             "will create a function, in which only the first "
                             "parameter is non-zero, which is interpreted to "
                             "be the depth of the point.");

          Functions::ParsedFunction<dim>::declare_parameters (prm, dim);
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
          coordinate_system = Utilities::Coordinates::string_to_coordinate_system(prm.get("Coordinate system"));
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
                                            "ascii data",
                                            "Implementation of a model in which the boundary "
                                            "traction is derived from files containing pressure data "
                                            "in ascii format. The pressure given in the data file is "
                                            "applied as traction normal to the surface of a given boundary, "
                                            "pointing inwards. Note the required format of the "
                                            "input data: The first lines may contain any number of comments "
                                            "if they begin with `#', but one of these lines needs to "
                                            "contain the number of grid points in each dimension as "
                                            "for example `# POINTS: 3 3'. "
                                            "The order of the data columns "
                                            "has to be `x', `Pressure [Pa]' in a 2d model and "
                                            " `x', `y', `Pressure [Pa]' in a 3d model, which means that "
                                            "there has to be a single column "
                                            "containing the pressure. "
                                            "Note that the data in the input "
                                            "files need to be sorted in a specific order: "
                                            "the first coordinate needs to ascend first, "
                                            "followed by the second in order to "
                                            "assign the correct data to the prescribed coordinates. "
                                            "If you use a spherical model, "
                                            "then the data will still be handled as Cartesian, "
                                            "however the assumed grid changes. `x' will be replaced by "
                                            "the radial distance of the point to the bottom of the model, "
                                            "`y' by the azimuth angle and `z' by the polar angle measured "
                                            "positive from the north pole. The grid will be assumed to be "
                                            "a latitude-longitude grid. Note that the order "
                                            "of spherical coordinates is `r', `phi', `theta' "
                                            "and not `r', `theta', `phi', since this allows "
                                            "for dimension independent expressions.")
  }
}

