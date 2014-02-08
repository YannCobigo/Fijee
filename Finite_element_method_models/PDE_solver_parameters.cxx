#include "PDE_solver_parameters.h"
//
// We give a comprehensive type name
//
typedef Solver::PDE_solver_parameters SDEsp;
typedef struct stat stat_file;
//
//
//
SDEsp*
SDEsp::parameters_instance_ = NULL;
//
//
//
SDEsp::PDE_solver_parameters()
{
  //
  // Check on ENV variables
  Utils::Fijee_environment fijee;
  //
  files_path_        = fijee.get_fem_path_();
  files_path_output_ = fijee.get_fem_output_path_();
  files_path_result_ = fijee.get_fem_result_path_();
}
//
//
//
SDEsp::PDE_solver_parameters( const SDEsp& that ){}
//
//
//
SDEsp::~PDE_solver_parameters()
{}
//
//
//
SDEsp& 
SDEsp::operator = ( const SDEsp& that )
{
  //
  return *this;
}
//
//
//
SDEsp* 
SDEsp::get_instance()
{
  if( parameters_instance_ == NULL )
    parameters_instance_ = new SDEsp();
  //
  return parameters_instance_;
}
//
//
//
void 
SDEsp::kill_instance()
{
  if( parameters_instance_ != NULL )
    {
      delete parameters_instance_;
      parameters_instance_ = NULL;
    }
}
//
//
//
void 
SDEsp::init()
{
  //
  //  dolfin::parameters["num_threads"] = 4;

  //
  // Back end parameters
  // Allowed values are: [PETSc, STL, uBLAS, Epetra, MTL4, ViennaCL].
  // Epetra in Trilinos
  // uBLAS needs UMFPACK
  dolfin::parameters["linear_algebra_backend"] = "ViennaCL";
  //  info(solver.parameters,true) ;
  //  info(parameters,true) ;

  //
  // Cholesky: umfpack


  //
  //    krylov_solver            |    type  value          range  access  change
  //    ------------------------------------------------------------------------
  //    absolute_tolerance       |  double  1e-15             []       0       0
  //    divergence_limit         |  double  10000             []       0       0
  //    error_on_nonconvergence  |    bool   true  {true, false}       0       0
  //    maximum_iterations       |     int  10000             []       0       0
  //    monitor_convergence      |    bool  false  {true, false}       0       0
  //    nonzero_initial_guess    |    bool  false  {true, false}       0       0
  //    relative_tolerance       |  double  1e-06             []       0       0
  //    report                   |    bool   true  {true, false}       0       0
  //    use_petsc_cusp_hack      |    bool  false  {true, false}       0       0
  //
  // cg - bicgstab - gmres
  linear_solver_ = "cg";
  // ilut - ilu0 - block_ilu{t,0} - jacobi - row_scaling
  preconditioner_ = "row_scaling";
  //
  maximum_iterations_ = 10000;
  //
  relative_tolerance_ = 1.e-08 /*1.e-06*/;

  //
  // Dispatching information
  number_of_threads_ = 2;
}
//
//
//
std::ostream& 
Solver::operator << ( std::ostream& stream, 
		      const SDEsp& that)
{
  stream << " Pattern Singleton\n";
  //
  return stream;
}
