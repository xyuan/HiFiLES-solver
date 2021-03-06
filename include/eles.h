/*!
 * \file eles.h
 * \author - Original code: SD++ developed by Patrice Castonguay, Antony Jameson,
 *                          Peter Vincent, David Williams (alphabetical by surname).
 *         - Current development: Aerospace Computing Laboratory (ACL)
 *                                Aero/Astro Department. Stanford University.
 * \version 0.1.0
 *
 * High Fidelity Large Eddy Simulation (HiFiLES) Code.
 * Copyright (C) 2014 Aerospace Computing Laboratory (ACL).
 *
 * HiFiLES is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HiFiLES is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HiFiLES.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "array.h"
#include "input.h"

#if defined _GPU
#include "cuda_runtime_api.h"
#include "cusparse_v2.h"
#endif

class eles
{	
public:

  // #### constructors ####

  // default constructor

  eles();

  // default destructor

  ~eles();

  // #### methods ####

  /*! setup */
  void setup(int in_n_eles, int in_max_s_spts_per_ele);

  /*! setup initial conditions */
  void set_ics(double& time);

  /*! read data from restart file */
  void read_restart_data(ifstream& restart_file);

  /*! write data to restart file */
  void write_restart_data(ofstream& restart_file);

	/*! move all to from cpu to gpu */
	void mv_all_cpu_gpu(void);

	/*! move wall distance array to from cpu to gpu */
	void mv_wall_distance_cpu_gpu(void);

	/*! copy transformed discontinuous solution at solution points to cpu */
	void cp_disu_upts_gpu_cpu(void);

	/*! copy transformed discontinuous solution at solution points to gpu */
  void cp_disu_upts_cpu_gpu(void);

  void cp_grad_disu_upts_gpu_cpu(void);

  /*! copy determinant of jacobian at solution points to cpu */
  void cp_detjac_upts_gpu_cpu(void);

  /*! copy divergence at solution points to cpu */
  void cp_div_tconf_upts_gpu_cpu(void);

  /*! remove transformed discontinuous solution at solution points from cpu */
  void rm_disu_upts_cpu(void);

  /*! remove determinant of jacobian at solution points from cpu */
  void rm_detjac_upts_cpu(void);

  /*! calculate the discontinuous solution at the flux points */
  void extrapolate_solution(int in_disu_upts_from);

  /*! Calculate terms for some LES models */
  void calc_sgs_terms(int in_disu_upts_from);

  /*! calculate transformed discontinuous inviscid flux at solution points */
  void evaluate_invFlux(int in_disu_upts_from);
  
  /*! calculate divergence of transformed discontinuous flux at solution points */
  void calculate_divergence(int in_div_tconf_upts_to);
  
  /*! calculate normal transformed discontinuous flux at flux points */
  void extrapolate_totalFlux(void);
  
  /*! calculate subgrid-scale flux at flux points */
  void evaluate_sgsFlux(void);

  /*! calculate divergence of transformed continuous flux at solution points */
  void calculate_corrected_divergence(int in_div_tconf_upts_to);
  
  /*! calculate uncorrected transformed gradient of the discontinuous solution at the solution points */
  void calculate_gradient(int in_disu_upts_from);

  /*! calculate corrected gradient of the discontinuous solution at solution points */
  void correct_gradient(void);

  /*! calculate corrected gradient of the discontinuous solution at flux points */
  void extrapolate_corrected_gradient(void);

  /*! calculate corrected gradient of solution at flux points */
  //void extrapolate_corrected_gradient(void);

  /*! calculate transformed discontinuous viscous flux at solution points */
  void evaluate_viscFlux(int in_disu_upts_from);

  /*! calculate divergence of transformed discontinuous viscous flux at solution points */
  //void calc_div_tdisvisf_upts(int in_div_tconinvf_upts_to);

  /*! calculate normal transformed discontinuous viscous flux at flux points */
  //void calc_norm_tdisvisf_fpts(void);

  /*! calculate divergence of transformed continuous viscous flux at solution points */
  //void calc_div_tconvisf_upts(int in_div_tconinvf_upts_to);
  
  /*! advance solution using a runge-kutta scheme */
  void AdvanceSolution(int in_step, int adv_type);

  /*! Calculate element local timestep */
  double calc_dt_local(int in_ele);

  /*! get number of elements */
  int get_n_eles(void);

  // get number of ppts_per_ele
  int get_n_ppts_per_ele(void);

  // get number of peles_per_ele
  int get_n_peles_per_ele(void);

  // get number of verts_per_ele
  int get_n_verts_per_ele(void);

  /*! get number of solution points per element */
  int get_n_upts_per_ele(void);

  /*! get element type */
  int get_ele_type(void);

  /*! get number of dimensions */
  int get_n_dims(void);

  /*! get number of fields */
  int get_n_fields(void);
  
  /*! set shape */
  void set_shape(int in_max_n_spts_per_ele);

  /*! set shape node */
  void set_shape_node(int in_spt, int in_ele, array<double>& in_pos);

  /*! set bc type */
  void set_bctype(int in_ele, int in_inter, int in_bctype);

  /*! set bc type */
  void set_bdy_ele2ele(void);

  /*! set number of shape points */
  void set_n_spts(int in_ele, int in_n_spts);

  /*!  set global element number */
  void set_ele2global_ele(int in_ele, int in_global_ele);

  /*! get a pointer to the transformed discontinuous solution at a flux point */
  double* get_disu_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_field, int in_ele);
  
  /*! get a pointer to the normal transformed continuous flux at a flux point */
  double* get_norm_tconf_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_field, int in_ele);

  /*! get a pointer to the determinant of the jacobian at a flux point */
  double* get_detjac_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_ele);

  /*! get a pointer to the magnitude of normal dot inverse of (determinant of jacobian multiplied by jacobian) at flux points */
  double* get_tdA_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_ele);

  /*! get a pointer to the normal at a flux point */
  double* get_norm_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_dim, int in_ele);

  /*! get a CPU pointer to the coordinates at a flux point */
  double* get_loc_fpts_ptr_cpu(int in_inter_local_fpt, int in_ele_local_inter, int in_dim, int in_ele);

  /*! get a GPU pointer to the coordinates at a flux point */
  double* get_loc_fpts_ptr_gpu(int in_inter_local_fpt, int in_ele_local_inter, int in_dim, int in_ele);

  /*! get a pointer to delta of the transformed discontinuous solution at a flux point */
  double* get_delta_disu_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_field, int in_ele);

  /*! get a pointer to gradient of discontinuous solution at a flux point */
  double* get_grad_disu_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_dim, int in_field, int in_ele);

  /*! get a pointer to gradient of discontinuous solution at a flux point */
  double* get_normal_disu_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_field, int in_ele, array<double> temp_loc, double temp_pos[3]);
  
  /*! get a pointer to the normal transformed continuous viscous flux at a flux point */
  //double* get_norm_tconvisf_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_field, int in_ele);
  
  /*! get a pointer to the subgrid-scale flux at a flux point */
  double* get_sgsf_fpts_ptr(int in_inter_local_fpt, int in_ele_local_inter, int in_field, int in_dim, int in_ele);

  /*! set opp_0 */
  void set_opp_0(int in_sparse);
  
  /*! set opp_1 */
  void set_opp_1(int in_sparse);

  /*! set opp_2 */
  void set_opp_2(int in_sparse);

  /*! set opp_3 */
  void set_opp_3(int in_sparse);

  /*! set opp_4 */
  void set_opp_4(int in_sparse);

  /*! set opp_5 */
  void set_opp_5(int in_sparse);

  /*! set opp_6 */
  void set_opp_6(int in_sparse);

  /*! set opp_p */
  void set_opp_p(void);

  /*! set opp_p */
  void set_opp_inters_cubpts(void);

  /*! set opp_p */
  void set_opp_volume_cubpts(void);

  /*! set opp_r */
  void set_opp_r(void);

  /*! calculate position of the plot points */
  void calc_pos_ppts(int in_ele, array<double>& out_pos_ppts);

  void set_rank(int in_rank);

  virtual void set_connectivity_plot()=0;

  void set_disu_upts_to_zero_other_levels(void);

  array<int> get_connectivity_plot();

  /*! calculate solution at the plot points */
  void calc_disu_ppts(int in_ele, array<double>& out_disu_ppts);

  /*! calculate gradient of solution at the plot points */
  void calc_grad_disu_ppts(int in_ele, array<double>& out_grad_disu_ppts);

  /*! calculate diagnostic fields at the plot points */
  void calc_diagnostic_fields_ppts(int in_ele, array<double>& in_disu_ppts, array<double>& in_grad_disu_ppts, array<double>& out_diag_field_ppts);

  /*! calculate position of a solution point */
  void calc_pos_upt(int in_upt, int in_ele, array<double>& out_pos);

  /*! returns position of a solution point */
  double get_loc_upt(int in_upt, int in_dim);

  /*! set transforms */
  void set_transforms(void);
       
  /*! set transforms at the interface cubature points */
  void set_transforms_inters_cubpts(void);

  /*! set transforms at the volume cubature points */
  void set_transforms_vol_cubpts(void);

	/*! Calculate distance of solution points to no-slip wall */
	void calc_wall_distance(int n_seg_noslip_inters, int n_tri_noslip_inters, int n_quad_noslip_inters, array< array<double> > loc_noslip_bdy);

	/*! Calculate distance of solution points to no-slip wall in parallel */
	void calc_wall_distance_parallel(array<int> n_seg_noslip_inters, array<int> n_tri_noslip_inters, array<int> n_quad_noslip_inters, array< array<double> > loc_noslip_bdy_global, int nproc);

  /*! calculate position */
  void calc_pos(array<double> in_loc, int in_ele, array<double>& out_pos);

  /*! calculate derivative of position */
  void calc_d_pos(array<double> in_loc, int in_ele, array<double>& out_d_pos);
  
  /*! calculate second derivative of position */
  void calc_dd_pos(array<double> in_loc, int in_ele, array<double>& out_dd_pos);
  
  // #### virtual methods ####

  virtual void setup_ele_type_specific()=0;

  /*! prototype for element reference length calculation */
  virtual double calc_h_ref_specific(int in_eles) = 0;

  virtual int read_restart_info(ifstream& restart_file)=0;

  virtual void write_restart_info(ofstream& restart_file)=0;

  /*! Compute interface jacobian determinant on face */
  virtual double compute_inter_detjac_inters_cubpts(int in_inter, array<double> d_pos)=0;

  /*! evaluate nodal basis */
  virtual double eval_nodal_basis(int in_index, array<double> in_loc)=0;

  /*! evaluate nodal basis for restart file*/
  virtual double eval_nodal_basis_restart(int in_index, array<double> in_loc)=0;

  /*! evaluate derivative of nodal basis */
  virtual double eval_d_nodal_basis(int in_index, int in_cpnt, array<double> in_loc)=0;

  virtual void fill_opp_3(array<double>& opp_3)=0;

  /*! evaluate divergence of vcjh basis */
  //virtual double eval_div_vcjh_basis(int in_index, array<double>& loc)=0;

  /*! evaluate nodal shape basis */
  virtual double eval_nodal_s_basis(int in_index, array<double> in_loc, int in_n_spts)=0;

  /*! evaluate derivative of nodal shape basis */
  virtual void eval_d_nodal_s_basis(array<double> &d_nodal_s_basis, array<double> in_loc, int in_n_spts)=0;

  /*! evaluate second derivative of nodal shape basis */
  virtual void eval_dd_nodal_s_basis(array<double> &dd_nodal_s_basis, array<double> in_loc, int in_n_spts)=0;

  /*! Calculate SGS flux */
  void calc_sgsf_upts(array<double>& temp_u, array<double>& temp_grad_u, double& detjac, int ele, int upt, array<double>& temp_sgsf);

  /*! rotate velocity components to surface*/
  array<double> calc_rotation_matrix(array<double>& norm);

  /*! calculate wall shear stress using LES wall model*/
  void calc_wall_stress(double rho, array<double>& urot, double ene, double mu, double Pr, double gamma, double y, array<double>& tau_wall, double q_wall);

  /*! Wall function calculator for Breuer-Rodi wall model */
  double wallfn_br(double yplus, double A, double B, double E, double kappa);

  /*! Calculate element volume */
  virtual double calc_ele_vol(double& detjac)=0;

  double compute_res_upts(int in_norm_type, int in_field);

  /*! calculate body forcing at solution points */
  void calc_body_force_upts(array <double>& vis_force, array <double>& body_force);

  /*! add body forcing at solution points */
  void evaluate_bodyForce(array <double>& body_force);

  /*! Compute volume integral of diagnostic quantities */
  void CalcIntegralQuantities(int n_integral_quantities, array <double>& integral_quantities);

  void compute_wall_forces(array<double>& inv_force, array<double>& vis_force, double& temp_cl, double& temp_cd, ofstream& coeff_file, bool write_forces);

  array<double> compute_error(int in_norm_type, double& time);
  
  array<double> get_pointwise_error(array<double>& sol, array<double>& grad_sol, array<double>& loc, double& time, int in_norm_type);


protected:

  // #### members ####

  /*! viscous flag */
  int viscous;

  /*! LES flag */
  int LES;

  /*! SGS model */
  int sgs_model;

  /*! LES filter flag */
  int filter;

  /*! near-wall model */
  int wall_model;

  /*! number of elements */
  int n_eles;

  /*! number of elements that have a boundary face*/
  int n_bdy_eles;

  /*!  number of dimensions */
  int n_dims;

  /*!  number of prognostic fields */
  int n_fields;

  /*!  number of diagnostic fields */
  int n_diagnostic_fields;

  /*! order of solution polynomials */
  int order;

  /*! order of interface cubature rule */
  int inters_cub_order;

  /*! order of interface cubature rule */
  int volume_cub_order;

  /*! order of solution polynomials in restart file*/
  int order_rest;

  /*! number of solution points per element */
  int n_upts_per_ele;

  /*! number of solution points per element */
  int n_upts_per_ele_rest;

  /*! number of flux points per element */
  int n_fpts_per_ele;

  /*! number of vertices per element */
  int n_verts_per_ele;

  array<int> connectivity_plot;

  /*! plotting resolution */
  int p_res;

  /*! solution point type */
  int upts_type;

  /*! flux point type */
  int fpts_type;

  /*! number of plot points per element */
  int n_ppts_per_ele;

  /*! number of plot elements per element */
  int n_peles_per_ele;

  /*! Global cell number of element */
  array<int> ele2global_ele;

  /*! Global cell number of element */
  array<int> bdy_ele2ele;

  /*! Boundary condition type of faces */
  array<int> bctype;

  /*! number of shape points per element */
  array<int> n_spts_per_ele;

  /*! transformed normal at flux points */
  array<double> tnorm_fpts;

  /*! transformed normal at flux points */
  array< array<double> > tnorm_inters_cubpts;

  /*! location of solution points in standard element */
  array<double> loc_upts;

  /*! location of solution points in standard element */
  array<double> loc_upts_rest;

  /*! location of flux points in standard element */
  array<double> tloc_fpts;

  /*! location of interface cubature points in standard element */
  array< array<double> > loc_inters_cubpts;

  /*! weight of interface cubature points in standard element */
  array< array<double> > weight_inters_cubpts;

  /*! location of volume cubature points in standard element */
  array<double> loc_volume_cubpts;

  /*! weight of cubature points in standard element */
  array<double> weight_volume_cubpts;

  /*! transformed normal at cubature points */
	array< array<double> > tnorm_cubpts;

	/*! location of plot points in standard element */
	array<double> loc_ppts;
	
	/*! location of shape points in standard element (simplex elements only)*/
	array<double> loc_spts;
	
	/*! number of interfaces per element */
	int n_inters_per_ele;
	
	/*! number of flux points per interface */
	array<int> n_fpts_per_inter; 

	/*! number of cubature points per interface */
	array<int> n_cubpts_per_inter; 

	/*! number of cubature points per interface */
	int n_cubpts_per_ele; 

	/*! element type (0=>quad,1=>tri,2=>tet,3=>pri,4=>hex) */
	int ele_type; 
	
	/*! order of polynomials defining shapes */
	int s_order;
	
	/*! shape */
	array<double> shape;
	
	/*! temporary solution storage at a single solution point */
	array<double> temp_u;

	/*! temporary solution gradient storage */
	array<double> temp_grad_u;

	/*! Matrix of filter weights at solution points */
	array<double> filter_upts;

	/*! extra arrays for similarity model: Leonard tensors, velocity/energy products */
	array<double> Lu, Le, uu, ue;

	/*! temporary flux storage */
	array<double> temp_f;

	/*! temporary subgrid-scale flux storage */
	array<double> temp_sgsf;
	
	/*! storage for distance of solution points to nearest no-slip boundary */
	array<double> wall_distance;

	array<double> twall;

	/*! number of storage levels for time-integration scheme */
	int n_adv_levels;
	
  /*! determinant of Jacobian (transformation matrix) at solution points
   *  (J = |G|) */
	array<double> detjac_upts;
	
  /*! determinant of Jacobian (transformation matrix) at flux points
   *  (J = |G|) */
	array<double> detjac_fpts;

  /*! determinant of jacobian at volume cubature points. TODO: what is this really? */
	array< array<double> > vol_detjac_inters_cubpts;

	/*! determinant of volume jacobian at cubature points. TODO: what is this really? */
	array< array<double> > vol_detjac_vol_cubpts;

  /*! Full vector-transform matrix from physical->computational frame, at solution points
   *  [Determinant of Jacobian times inverse of Jacobian] [J*G^-1] */
  array<double> JGinv_upts;
	
  /*! Full vector-transform matrix from physical->computational frame, at flux points
   *  [Determinant of Jacobian times inverse of Jacobian] [J*G^-1] */
  array<double> JGinv_fpts;
	
  /*! Magnitude of transformed face-area normal vector at flux points
   *  [magntiude of (normal dot inverse transformation matrix)] [ |J*(G^-1)*(n*dA)| ] */
  array<double> tdA_fpts;

	/*! determinant of interface jacobian at flux points */
	array< array<double> > inter_detjac_inters_cubpts;

	/*! normal at flux points*/
	array<double> norm_fpts;
	
  /*! physical coordinates at flux points*/
  array<double> loc_fpts;

  /*! normal at interface cubature points*/
  array< array<double> > norm_inters_cubpts;

  /*!
        description: transformed discontinuous solution at the solution points
        indexing: \n
        matrix mapping:
        */
  array< array<double> > disu_upts;

	/*!
	time-averaged diagnostic fields at solution points
	*/
	array<double> u_average;
	array<double> v_average;
	array<double> w_average;

	/*!
	filtered solution at solution points for similarity and SVV LES models
	*/
	array<double> disuf_upts;

  /*! position at the plot points */
  array< array<double> > pos_ppts;

	/*!
	description: transformed discontinuous solution at the flux points \n
	indexing: (in_fpt, in_field, in_ele) \n
	matrix mapping: (in_fpt || in_field, in_ele)
	*/
	array<double> disu_fpts;

	/*!
	description: transformed discontinuous flux at the solution points \n
	indexing: (in_upt, in_dim, in_field, in_ele) \n
	matrix mapping: (in_upt, in_dim || in_field, in_ele)
	*/
	array<double> tdisf_upts;
	
	/*!
	description: subgrid-scale flux at the solution points \n
	indexing: (in_upt, in_dim, in_field, in_ele) \n
	matrix mapping: (in_upt, in_dim || in_field, in_ele)
	*/
	array<double> sgsf_upts;

	/*!
	description: subgrid-scale flux at the flux points \n
	indexing: (in_fpt, in_dim, in_field, in_ele) \n
	matrix mapping: (in_fpt, in_dim || in_field, in_ele)
	*/
	array<double> sgsf_fpts;

	/*!
	normal transformed discontinuous flux at the flux points
	indexing: \n
	matrix mapping:
	*/
	array<double> norm_tdisf_fpts;
	
	/*!
	normal transformed continuous flux at the flux points
	indexing: \n
	matrix mapping:
	*/
	array<double> norm_tconf_fpts;
	
	/*!
	divergence of transformed continuous flux at the solution points
	indexing: \n
	matrix mapping:
	*/
	array< array<double> > div_tconf_upts;
	
	/*! delta of the transformed discontinuous solution at the flux points   */
	array<double> delta_disu_fpts;

	/*! gradient of discontinuous solution at solution points */
	array<double> grad_disu_upts;
	
	/*! gradient of discontinuous solution at flux points */
	array<double> grad_disu_fpts;

	/*! transformed discontinuous viscous flux at the solution points */
	//array<double> tdisvisf_upts;
	
	/*! normal transformed discontinuous viscous flux at the flux points */
	//array<double> norm_tdisvisf_fpts;
	
	/*! normal transformed continuous viscous flux at the flux points */
	//array<double> norm_tconvisf_fpts;
		
	/*! transformed gradient of determinant of jacobian at solution points */
	array<double> tgrad_detjac_upts;
	
	/*! transformed gradient of determinant of jacobian at flux points */
	array<double> tgrad_detjac_fpts;

  array<double> d_nodal_s_basis;
  array<double> dd_nodal_s_basis;
  // TODO: change naming (comments) to reflect reuse

#ifdef _GPU
  cusparseHandle_t handle;
#endif

  /*! operator to go from transformed discontinuous solution at the solution points to transformed discontinuous solution at the flux points */
  array<double> opp_0;
  array<double> opp_0_data;
  array<int> opp_0_cols;
  array<int> opp_0_b;
  array<int> opp_0_e;
  int opp_0_sparse;

#ifdef _GPU
  array<double> opp_0_ell_data;
  array<int> opp_0_ell_indices;
  int opp_0_nnz_per_row;
#endif

  /*! operator to go from transformed discontinuous inviscid flux at the solution points to divergence of transformed discontinuous inviscid flux at the solution points */
  array< array<double> > opp_1;
  array< array<double> > opp_1_data;
  array< array<int> > opp_1_cols;
  array< array<int> > opp_1_b;
  array< array<int> > opp_1_e;
  int opp_1_sparse;
#ifdef _GPU
  array< array<double> > opp_1_ell_data;
  array< array<int> > opp_1_ell_indices;
  array<int> opp_1_nnz_per_row;
#endif

  /*! operator to go from transformed discontinuous inviscid flux at the solution points to normal transformed discontinuous inviscid flux at the flux points */
  array< array<double> > opp_2;
  array< array<double> > opp_2_data;
  array< array<int> > opp_2_cols;
  array< array<int> > opp_2_b;
  array< array<int> > opp_2_e;
  int opp_2_sparse;
#ifdef _GPU
  array< array<double> > opp_2_ell_data;
  array< array<int> > opp_2_ell_indices;
  array<int> opp_2_nnz_per_row;
#endif

  /*! operator to go from normal correction inviscid flux at the flux points to divergence of correction inviscid flux at the solution points*/
  array<double> opp_3;
  array<double> opp_3_data;
  array<int> opp_3_cols;
  array<int> opp_3_b;
  array<int> opp_3_e;
  int opp_3_sparse;
#ifdef _GPU
  array<double> opp_3_ell_data;
  array<int> opp_3_ell_indices;
  int opp_3_nnz_per_row;
#endif

  /*! operator to go from transformed solution at solution points to transformed gradient of transformed solution at solution points */
  array< array<double> >  opp_4;
  array< array<double> >  opp_4_data;
  array< array<int> > opp_4_cols;
  array< array<int> > opp_4_b;
  array< array<int> > opp_4_e;
  int opp_4_sparse;
#ifdef _GPU
  array< array<double> > opp_4_ell_data;
  array< array<int> > opp_4_ell_indices;
  array< int > opp_4_nnz_per_row;
#endif

  /*! operator to go from transformed solution at flux points to transformed gradient of transformed solution at solution points */
  array< array<double> > opp_5;
  array< array<double> > opp_5_data;
  array< array<int> > opp_5_cols;
  array< array<int> > opp_5_b;
  array< array<int> > opp_5_e;
  int opp_5_sparse;
#ifdef _GPU
  array< array<double> > opp_5_ell_data;
  array< array<int> > opp_5_ell_indices;
  array<int> opp_5_nnz_per_row;
#endif

  /*! operator to go from transformed solution at solution points to transformed gradient of transformed solution at flux points */
  array<double> opp_6;
  array<double> opp_6_data;
  array<int> opp_6_cols;
  array<int> opp_6_b;
  array<int> opp_6_e;
  int opp_6_sparse;
#ifdef _GPU
  array<double> opp_6_ell_data;
  array<int> opp_6_ell_indices;
  int opp_6_nnz_per_row;
#endif

  /*! operator to go from discontinuous solution at the solution points to discontinuous solution at the plot points */
  array<double> opp_p;

  array< array<double> > opp_inters_cubpts;
  array<double> opp_volume_cubpts;

  /*! operator to go from discontinuous solution at the restart points to discontinuous solution at the solutoin points */
  array<double> opp_r;

  /*! dimensions for blas calls */
  int Arows, Acols;
  int Brows, Bcols;
  int Astride, Bstride, Cstride;

  /*! general settings for mkl sparse blas */
  char matdescra[6];

  /*! transpose setting for mkl sparse blas */
  char transa;

  /*! zero for mkl sparse blas */
  double zero;

  /*! one for mkl sparse blas */
  double one;

  /*! number of fields multiplied by number of elements */
  int n_fields_mul_n_eles;

  /*! number of dimensions multiplied by number of solution points per element */
  int n_dims_mul_n_upts_per_ele;

  int rank;
  int nproc;

  /*! reference element length */
  array<double> h_ref;
  
  /*! element local timestep */
  array<double> dt_local;
  double dt_local_new;
  array<double> dt_local_mpi;

};
