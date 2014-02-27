#include "Intensity.h"
//
//
//
Solver::Intensity::Intensity():
  electric_variable_(""), index_( 0 ), I_( 0. ), surface_(0.), radius_(0.),
  label_("no_name")
{
  //
  //
  r0_values_     = Point();
  r0_projection_ = Point();
  e_values_      = Point();
  //
  impedance_ = std::complex<double>(0.,0.);

  not_yet_ = true;
}
//
//
//
Solver::Intensity::Intensity(const Intensity& that): 
  electric_variable_(that.electric_variable_), index_(that.index_), 
  I_(that.I_), label_(that.label_),
  r0_values_(that.r0_values_), r0_projection_(that.r0_projection_), e_values_(that.e_values_),
  impedance_(that.impedance_), surface_(that.surface_), radius_(that.radius_)
{
  not_yet_ = true;
  //
  boundary_cells_    = that.boundary_cells_;
  boundary_vertices_ = that.boundary_vertices_;
}
//
//
//
Solver::Intensity::Intensity( std::string Electric_variable, int Index, 
			      std::string Label, double Intensity,
			      Point X, Point V,double Re_z_l, double Im_z_l,
			      double Surface, double Radius): 
  electric_variable_(Electric_variable), index_( Index ), label_( Label ), I_( Intensity ), 
  r0_values_(X), e_values_(V), impedance_( (Re_z_l,Im_z_l) ), surface_(Surface), radius_(Radius) 
{
  r0_projection_ = Point();
  not_yet_ = true;
}
//
//
//
double 
Solver::Intensity::eval( const Point& x, const ufc::cell& cell) const
{

//version v2
  std::size_t cell_label[2] = {130,37697};
  //
  if( I_ != 0 )
    {
      //
      if ( r0_projection_.distance(x) < 1.e-3 )
	{
	  std::cout << label_ << " position: " << x << std::endl;
	  return I_ ;
	}
      else
	return 0.;
    }
  else
    return 0.;

//version v1
//  std::size_t cell_label[2] = {130,37697};
//  //
//  if( I_ != 0 )
//    {
//      //
//      auto boundary_cells_it = boundary_cells_.find( cell.index );
//      // We check if the vertex belong to a cell which has, at least, one facet on boundary
//      if ( boundary_cells_it != boundary_cells_.end() )
//	{ 
//	  // we check if the vertex belong to a boundary facet for the list
//	  for ( MeshEntity facet : boundary_cells_it->second )
//	    {
//	      for (VertexIterator v( facet ); !v.end(); ++v)
//		{
//		  if ( v->point().distance(x) < 1.e-3 
//		       /*&& ( cell_label[0] == cell.index || cell_label[1] == cell.index ) */ 
//		       /*&& facet_reservoir_.size() < 3*/ )
//		    {
//		      facet_reservoir_.push_back(v->index());
//		      std::cout << label_ << " Cell index: " << cell.index << std::endl;
//		      return I_ / boundary_vertices_.size();
//		    }
//		}
//	    }    
//	  //
//	  // if no vertex found: x does not belong to the boundary
//	  return 0.;
//	}
//      else
//	return 0.;
//    }
//  else
//    return 0.;

// version v0
//  std::size_t cell_label[2] = {130,37697};
//  //
//  if( I_ != 0 )
//    {
//      //
//      auto boundary_cells_it = boundary_cells_.find( cell.index );
//      // We check if the vertex belong to a cell which has, at least, one facet on boundary
//      if ( boundary_cells_it != boundary_cells_.end() )
//	{ 
////	  if( boundary_cells_it->second.size() == 1 ) // only top facets
////	    {
//	      // we check if the vertex belong to a boundary facet for the list
//	      for ( MeshEntity facet : boundary_cells_it->second )
//		{
//		  for (VertexIterator v( facet ); !v.end(); ++v)
//		    {
//		      if ( v->point().distance(x) < 1.e-3 
//			   /*&& ( cell_label[0] == cell.index || cell_label[1] == cell.index ) */ 
//			   /*&& facet_reservoir_.size() < 3*/ )
//			{
//			  facet_reservoir_.push_back(v->index());
//			  std::cout << label_ << " Cell index: " << cell.index << std::endl;
//			  return I_;
//			}
//		    }
//		}    
//	      //
//	      // if no vertex found: x does not belong to the boundary
//	      return 0.;
////	    }
////	  else
////	    return 0.;
//	}
//      else
//	return 0.;
//    }
//  else
//    return 0.;
}
//
//
//
Solver::Intensity&
Solver::Intensity::operator =( const Intensity& that )
{
  electric_variable_ = that.electric_variable_;
  index_  = that.index_;
  I_      = that.I_;
  label_  = that.label_;
  //
  //
  r0_values_     = that.get_r0_values_();
  r0_projection_ = that.get_r0_projection_();
  e_values_      = that.get_e_values_();
  //
  impedance_ = that.get_impedance_();
  //
  surface_   = that.get_surface_();
  radius_    = that.get_radius_();
  //
  boundary_cells_    = that.boundary_cells_;
  boundary_vertices_ = that.boundary_vertices_;
  not_yet_ = that.not_yet_;

  
  //
  //
  return *this;
}
//
//
//
void
Solver::Intensity::set_boundary_cells_( const std::map< std::size_t, std::list< MeshEntity  >  >& Boundary_cells )
{
  //
  //
  boundary_cells_ = Boundary_cells;

  //
  // Center prob projection on boundary
  for( auto cell : boundary_cells_ )
    for( auto facet : cell.second )
      for (VertexIterator v( facet ); !v.end(); ++v)
	{
	  //
	  //
	  if( r0_values_.distance ( v->point() ) < r0_values_.distance ( r0_projection_ ) )
	    r0_projection_ = v->point();

	  //
	  //
	  boundary_vertices_.insert( v->index() );
	}
}
//
//
//
std::ostream& 
Solver::operator << ( std::ostream& stream, 
		      const Solver::Intensity& that)
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
//  //
//  //
//  return stream;
};