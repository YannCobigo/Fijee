#include "Point_vector.h"
//
// We give a comprehensive type name
//
typedef Domains::Point_vector DPv;
//
//
//
DPv::Point_vector(): Domains::Point()
{
  vector_[0] = 0.;
  vector_[1] = 0.;
  vector_[2] = 0.;
  //
  norm_ = 1.;
}
//
//
//
DPv::Point_vector(  float X, float Y, float Z, 
		    float Vx, float Vy, float Vz): Domains::Point(X,Y,Z)
{
  vector_[0] = Vx;
  vector_[1] = Vy;
  vector_[2] = Vz;
  //
  normalize();
}
//
//
//
DPv::Point_vector( const DPv& that ): Domains::Point(that),
				      norm_(that.norm_)
{
  vector_[0] = that.vector_[0];
  vector_[1] = that.vector_[1];
  vector_[2] = that.vector_[2];
}
////
////
////
//DPv::Point_vector( DPv&& that ):
//  pos_x_( 0 ),
//  pos_y_( 0 ),
//  tab_( nullptr )
//{
//  // pilfer the source
//  list_position_ = std::move( that.list_position_ );
//  pos_x_ =  that.get_pos_x();
//  pos_y_ =  that.get_pos_y();
//  tab_   = &that.get_tab();
//  // reset that
//  that.set_pos_x( 0 );
//  that.set_pos_y( 0 );
//  that.set_tab( nullptr );
//}
//
//
//
DPv::~Point_vector()
{
}
//
//
//
DPv& 
DPv::operator = ( const DPv& that )
{
  Domains::Point::operator = (that);
  //
  vector_[0] = that.vector_[0];
  vector_[1] = that.vector_[1];
  vector_[2] = that.vector_[2];
  //
  norm_ = that.norm_;
 
  //
  //
  return *this;
}
////
////
////
//DPv& 
//DPv::operator = ( DPv&& that )
//{
//  if( this != &that )
//    {
//      // initialisation
//      pos_x_ = 0;
//      pos_y_ = 0;
//      delete [] tab_;
//      tab_   = nullptr;
//      // pilfer the source
//      list_position_ = std::move( that.list_position_ );
//      pos_x_ =  that.get_pos_x();
//      pos_y_ =  that.get_pos_y();
//      tab_   = &that.get_tab();
//      // reset that
//      that.set_pos_x( 0 );
//      that.set_pos_y( 0 );
//      that.set_tab( nullptr );
//    }
//  //
//  return *this;
//}
//
//
//
bool
DPv::operator == ( const DPv& that )
{
  return ( Domains::Point::operator == (that) && 
	   vector_[0] == that.vector_[0] && 
	   vector_[1] == that.vector_[1] && 
	   vector_[2] == that.vector_[2] );
}
//
//
//
bool
DPv::operator != ( const DPv& that )
{
  return ( Domains::Point::operator != (that) && 
	   vector_[0] != that.vector_[0] && 
	   vector_[1] != that.vector_[1] && 
	   vector_[2] != that.vector_[2] );
}
//
//
//
DPv& 
DPv::operator + ( const DPv& Vector)
{
  DPv* vector = new DPv(*this);

  //
  //
  vector->vx() += Vector.vx();
  vector->vy() += Vector.vy();
  vector->vz() += Vector.vz();
  //
  normalize();

  //
  //
  return *vector;
}
//
//
//
DPv& 
DPv::operator += ( const DPv& Vector)
{
  //
  //
  vector_[0] += Vector.vx();
  vector_[1] += Vector.vy();
  vector_[2] += Vector.vz();
  //
  normalize();

  //
  //
  return *this;
}
//
//
//
DPv& 
DPv::operator - ( const DPv& Vector)
{
  DPv* vector = new DPv(*this);

  //
  //
  vector->vx() -= Vector.vx();
  vector->vy() -= Vector.vy();
  vector->vz() -= Vector.vz();
  //
  normalize();

  //
  //
  return *vector;
}
//
//
//
DPv& 
DPv::operator -= ( const DPv& Vector)
{
  //
  //
  vector_[0] -= Vector.vx();
  vector_[1] -= Vector.vy();
  vector_[2] -= Vector.vz();
  //
  normalize();

  //
  //
  return *this;
}
//
//
//
DPv& 
DPv::cross( const DPv& Vector )
{
  DPv* vector = new DPv();
  //
  vector->vx() = vector_[1] * Vector.vz() - vector_[2] * Vector.vy();
  vector->vy() = vector_[2] * Vector.vx() - vector_[0] * Vector.vz();
  vector->vz() = vector_[0] * Vector.vy() - vector_[1] * Vector.vx();
  //
  vector->normalize();
  //
  //
  return *vector;
}
//
//
//
float 
DPv::dot( const Point_vector& Vector ) const
{
  float scalar = 0.;
  //
  for( int coord = 0 ; coord < 3 ; coord++ )
    scalar += vector_[coord] * ( Vector.get_vector_() )[coord];

  //
  //
  return scalar;
}
//
//
//
float 
DPv::cosine_theta( const Point_vector& Vector ) const
{
  float cosine_theta = 0.;
  //
  if( norm_ == 0 || Vector.get_norm_() == 0 )
    {
      std::cerr << "Vector norm is null." << std::endl;
    }
  //
  cosine_theta = dot( Vector );
  cosine_theta /= norm_ * Vector.get_norm_();
  //
  //
  return cosine_theta;
}
//
//
//
void
DPv::normalize()
{
  norm_ = sqrt( dot(*this) );
}
//
//
//
std::ostream& 
Domains::operator << ( std::ostream& stream, 
		       const DPv& that)
{
//  std::for_each( that.get_list_position().begin(),
//		 that.get_list_position().end(),
//		 [&stream]( int Val )
//		 {
//		   stream << "list pos = " << Val << "\n";
//		 });
//  //
//  stream << "position x = " <<    that.get_pos_x() << "\n";
//  stream << "position y = " <<    that.get_pos_y() << "\n";
//  if ( &that.get_tab() )
//    {
//      stream << "tab[0] = "     << ( &that.get_tab() )[0] << "\n";
//      stream << "tab[1] = "     << ( &that.get_tab() )[1] << "\n";
//      stream << "tab[2] = "     << ( &that.get_tab() )[2] << "\n";
//      stream << "tab[3] = "     << ( &that.get_tab() )[3] << "\n";
//    }
  //
  return stream;
};
