//  Copyright (c) 2014, Yann Cobigo 
//  All rights reserved.     
//   
//  Redistribution and use in source and binary forms, with or without       
//  modification, are permitted provided that the following conditions are met:   
//   
//  1. Redistributions of source code must retain the above copyright notice, this   
//     list of conditions and the following disclaimer.    
//  2. Redistributions in binary form must reproduce the above copyright notice,   
//     this list of conditions and the following disclaimer in the documentation   
//     and/or other materials provided with the distribution.   
//   
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND   
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE   
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR   
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES   
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;   
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND   
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS   
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.   
//     
//  The views and conclusions contained in the software and documentation are those   
//  of the authors and should not be interpreted as representing official policies,    
//  either expressed or implied, of the FreeBSD Project.  
#include "Electrodes_injection.h"
//
//
//
Solver::Electrodes_injection::Electrodes_injection():
  time_(0.)
{}
//
//
//
Solver::Electrodes_injection::Electrodes_injection( double Time ):
  time_(Time)
{}
//
//
//
Solver::Electrodes_injection::Electrodes_injection(const Electrodes_injection& that)
{
  //
  // 
  electrodes_map_ = that.electrodes_map_;
  // 
  time_ = that.time_;
}
//
//
//
void 
Solver::Electrodes_injection::eval(Array<double>& values, const Array<double>& x, const ufc::cell& cell) const
{

  Point vertex( x[0], x[1], x[2] );

  //
  //
  double tempo_val = 0.;
  // go through all electrodes and check if we have a value.
  for ( auto intensity = electrodes_map_.begin() ; intensity != electrodes_map_.end() ; intensity++ )
    tempo_val += ( intensity->second ).eval( vertex, cell );

//  std::cout << "Electrodes_inj eval return: " << tempo_val << std::endl;
  //
  //
  values[0] = tempo_val;
}
//
//
//
Solver::Electrodes_injection&
Solver::Electrodes_injection::operator =( const Electrodes_injection& that )
{
  // 
  // 
  electrodes_map_ = that.electrodes_map_;
  //
  time_ = that.time_;
 
  //
  //
  return *this;
}
//
//
//
void 
Solver::Electrodes_injection::add_electrode( std::string Electric_variable, int Index, 
					     std::string Label, double I,
					     Point X, Point V, double Re_z_l, double Im_z_l, 
					     double Surface, double Radius  )
{
//  electrodes_vector_.push_back( Intensity(Electric_variable, Index, Label, I, 
//					  X, V, Re_z_l, Im_z_l, Surface, Radius) );
  electrodes_map_[Label] = Intensity(Electric_variable, Index, Label, I, 
				     X, V, Re_z_l, Im_z_l, Surface, Radius);
}
// 
// 
// 
void 
Solver::Electrodes_injection::add_measured_potential( std::string Label, double V, double I )
{
  if ( I == information(Label).get_I_() )
    {
      potential_measured_map_[Label] = V;
    }
  else
    {
      std::cerr << "Error: there is a mismatch between the current injection of the simulation and the measure" << std::endl;
      std::cerr << "Current at " << Label << " during the simulation is: " 
		<< information(Label).get_I_()
		<< ". For the measure, the current is: " << I
		<< std::endl;
      abort();
    }
}
//
//
//
std::ostream& 
Solver::operator << ( std::ostream& stream, 
		      const Solver::Electrodes_injection& that)
{
  //  //
  //  //
  //  stream 
  //    << "Dipole source -- index: " << that.get_index_() << " -- " 
  //    << "index cell: " << that.get_index_cell_() << "\n"
  //    << "Position:"
  //    << " (" << that.get_X_() << ", " << that.get_Y_() << ", " << that.get_Z_()  << ") " 
  //    << " -- direction: " 
  //    << " (" << that.get_VX_()  << ", " << that.get_VY_() << ", " << that.get_VZ_()  << ") \n"
  //    << "Intensity: " << that.get_Q_() << std::endl;
  
  //
  //
  return stream;
};
//
//
//
bool 
Solver::Electrodes_injection::inside( const Point& Vertex ) const
{
  //
  //
  bool inside_electrode = false;
  //
  for( auto electrode : electrodes_map_ )
//    if ( (electrode.second).get_I_() != 0. )
      if( (electrode.second).get_r0_values_().distance(Vertex) < (electrode.second).get_radius_() + .003 /* m */ )
	{
	  inside_electrode = true;
	}

  //
  //
  return inside_electrode;
}
//
//
//
bool 
Solver::Electrodes_injection::add_potential_value( const Point& Vertex, const double U )
{
  //
  //
  bool inside_electrode = false;
  //
  for( auto electrode : electrodes_map_ )
    if( (electrode.second).get_r0_values_().distance(Vertex) < (electrode.second).get_radius_() + .003 /* m */ )
      {
	(electrode.second).add_potential_value( U );
	inside_electrode = true;
      }

  //
  //
  return inside_electrode;
}
//
//
//
bool 
Solver::Electrodes_injection::add_potential_value( const std::string Electrode_label, const double U )
{
  //
  //
  bool inside_electrode = false;
  auto electrode = electrodes_map_.find( Electrode_label );
  // 
  if ( electrode != electrodes_map_.end() )
    {
      (electrode->second).add_potential_value( U );
      inside_electrode = true;
    }

  //
  //
  return inside_electrode;
}
//
//
//
std::tuple<std::string, bool> 
Solver::Electrodes_injection::inside_probe( const Point& Vertex ) const
{
  //
  //
  bool inside_electrode = false;
  std::string label;
  //
  for( auto electrode : electrodes_map_ )
    if( (electrode.second).get_r0_values_().distance(Vertex) < (electrode.second).get_radius_() + .003 /* m */ )
      {
	inside_electrode = true;
	label = electrode.first;
      }

  //
  //
  return std::make_tuple (label, inside_electrode);
}
//
//
//
void
Solver::Electrodes_injection::set_boundary_cells( const std::map< std::string, std::map< std::size_t, std::list< MeshEntity  >  >  >& Map_electrode_cells  )
{
  //
  for ( auto electrode = Map_electrode_cells.begin() ;  electrode != Map_electrode_cells.end() ; electrode++ )
    {
      //
      auto electrode_it = electrodes_map_.find( electrode->first );
      //
      if( electrode_it != electrodes_map_.end() )
	{
	  (electrode_it->second).set_boundary_cells_(electrode->second);
	}
    }
}
// 
// 
// 
void 
Solver::Electrodes_injection::punctual_potential_evaluation(const dolfin::Function& U, 
							    const std::shared_ptr< const Mesh > Mesh)
{
  // 
  const std::size_t num_vertices = Mesh->num_vertices();
  
  // Get number of components
  const std::size_t dim  = U.value_size();
  const std::size_t rank = U.value_rank();

  // Allocate memory for function values at vertices
  const std::size_t size = num_vertices * dim; // dim = 1
  std::vector<double> values(size);
  U.compute_vertex_values(values, *Mesh);

  
  // 
  // 
  for ( auto electrode = electrodes_map_.begin() ; 
	electrode != electrodes_map_.end() ; 
	electrode++ )
    {
      // Get the potential at the surface projection of the electrode center
      double potential = values[ (electrode->second).get_r0_projection_index_() ];
      // 
      (electrode->second).set_V_(potential);
    }
}
// 
// 
// 
void 
Solver::Electrodes_injection::surface_potential_evaluation(const dolfin::Function& U, 
							   const std::shared_ptr< const Mesh > Mesh)
{
  // 
  const std::size_t num_vertices = Mesh->num_vertices();
  
  // Get number of components
  const std::size_t dim  = U.value_size();
  const std::size_t rank = U.value_rank();

  // Allocate memory for function values at vertices
  const std::size_t size = num_vertices * dim; // dim = 1
  std::vector<double> values(size);
  U.compute_vertex_values(values, *Mesh);

  
  // 
  // 
  for ( auto electrode = electrodes_map_.begin() ; 
	electrode != electrodes_map_.end() ; 
	electrode++ )
    {
      // Clear old values
      (electrode->second).clear_electrical_potential_list();
      // Process new values
      for( auto cells_map : (electrode->second).get_boundary_cells_() )
	{
	  // 
	  // Retrieves the list of cell's facets and compute 
	  // the average electrical potential on the surface
	  for ( MeshEntity facet : cells_map.second )
	    {
	      double local_potential_value = 0.;
	      for ( VertexIterator vertex( facet ); !vertex.end(); ++vertex )
		{
		  //  Calcul of the cell potential
		  local_potential_value += values[ vertex->index() ];
		}
	      // We have 3 vertices on a facet
	      (electrode->second).add_potential_value( local_potential_value/3. );
	    }
	}
    }
}
// 
// 
// 
double 
Solver::Electrodes_injection::sum_of_squares()const
{
  // 
  // Dimention of the problem
  std::size_t dim = potential_measured_map_.size();

  //
  // Create the vector of differences between the measured and simulated potential
  Eigen::VectorXd U_phi;
  //  U_phi.resize(dim,1);
  U_phi = Eigen::VectorXd::Zero(dim,1);
  // fill up the vector and avoid injection electrodes
  for ( auto measure : potential_measured_map_ )
    U_phi(information(measure.first).get_index_(), 0) 
      = ( information(measure.first).get_I_() == 0. ? 
	  measure.second - information(measure.first).get_V_() : 
	  0.);

  //
  // Create the noise matrix
  Eigen::MatrixXd sigma;
  //  sigma.resize(dim, dim);
  // 
  sigma =  Eigen::MatrixXd::Identity(dim, dim);
   
  // 
  // 
  return U_phi.transpose() * sigma.inverse() * U_phi;
}
