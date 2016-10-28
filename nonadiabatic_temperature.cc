/*
  Copyright (C) 2011 - 2015 by the authors of the ASPECT code.

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
  along with ASPECT; see the file doc/COPYING.  If not see
  <http://www.gnu.org/licenses/>.
*/


#include <aspect/postprocess/visualization/nonadiabatic_temperature.h>
#include <aspect/simulator_access.h>
#include <aspect/postprocess/visualization.h> //nanzhang pawsey


namespace aspect
{
  namespace Postprocess
  {
    namespace VisualizationPostprocessors
    {
      template <int dim>
      NonadiabaticTemperature<dim>::
      NonadiabaticTemperature ()
        :
        DataPostprocessorScalar<dim> ("nonadiabatic_temperature",
                                      update_values | update_q_points)
      {}



      template <int dim>
      void
      NonadiabaticTemperature<dim>::
      compute_derived_quantities_vector (const std::vector<Vector<double> >              &uh,
                                         const std::vector<std::vector<Tensor<1,dim> > > &,
                                         const std::vector<std::vector<Tensor<2,dim> > > &,
                                         const std::vector<Point<dim> > &,
                                         const std::vector<Point<dim> >                  &evaluation_points,
                                         std::vector<Vector<double> >                    &computed_quantities) const
      {
        const unsigned int n_quadrature_points = uh.size();
        Assert (computed_quantities.size() == n_quadrature_points,    ExcInternalError());
        Assert (computed_quantities[0].size() == 1,                   ExcInternalError());
        Assert (uh[0].size() == this->introspection().n_components,           ExcInternalError());

        for (unsigned int q=0; q<n_quadrature_points; ++q)
          {
            const double temperature = uh[q][this->introspection().component_indices.temperature];

            //computed_quantities[q](0) = temperature - this->get_adiabatic_conditions().temperature(evaluation_points[q]);
            double depth = this->get_geometry_model().depth(evaluation_points[q]); //nanzhang_pawsey
            const unsigned int idx = static_cast<unsigned int> ((::aspect::ave_temp.size()-1) * depth / this->get_geometry_model().maximal_depth());
            double delta_temperature = temperature-::aspect::ave_temp[idx];
            computed_quantities[q](0) = delta_temperature;

          }
      }
    }
  }
}


// explicit instantiations
namespace aspect
{
  namespace Postprocess
  {
    namespace VisualizationPostprocessors
    {
      ASPECT_REGISTER_VISUALIZATION_POSTPROCESSOR(NonadiabaticTemperature,
                                                  "nonadiabatic temperature",
                                                  "A visualization output object that generates output "
                                                  "for the non-adiabatic component of the pressure.")
    }
  }
}
