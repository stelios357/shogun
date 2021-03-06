/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Sunil Mahendrakar, Soumyajit De, Heiko Strathmann, Bjoern Esser
 */

#include <shogun/lib/config.h>

#include <shogun/lib/SGMatrix.h>
#include <shogun/mathematics/eigen3.h>
#include <shogun/mathematics/linalg/linop/DenseMatrixOperator.h>
#include <shogun/mathematics/linalg/eigsolver/DirectEigenSolver.h>

using namespace Eigen;

namespace shogun
{

CDirectEigenSolver::CDirectEigenSolver()
	: CEigenSolver()
{
	SG_GCDEBUG("{} created ({})", this->get_name(), fmt::ptr(this))
}

CDirectEigenSolver::CDirectEigenSolver(
	CDenseMatrixOperator<float64_t>* linear_operator)
	: CEigenSolver((CLinearOperator<float64_t>*)linear_operator)
{
	SG_GCDEBUG("{} created ({})", this->get_name(), fmt::ptr(this))
}

CDirectEigenSolver::~CDirectEigenSolver()
{
	SG_GCDEBUG("{} destroyed ({})", this->get_name(), fmt::ptr(this))
}

void CDirectEigenSolver::compute()
{
	if (m_is_computed_min && m_is_computed_max)
	{
		SG_DEBUG("Minimum/maximum eigenvalues are already computed, exiting");
		return;
	}

	CDenseMatrixOperator<float64_t>* op
		=dynamic_cast<CDenseMatrixOperator<float64_t>*>(m_linear_operator);
	require(op, "Linear operator is not of CDenseMatrixOperator type!");

	SGMatrix<float64_t> m=op->get_matrix_operator();
	Map<MatrixXd> map_m(m.matrix, m.num_rows, m.num_cols);

	// compute the eigenvalues with Eigen3
	SelfAdjointEigenSolver<MatrixXd> eig_solver(map_m);
	m_min_eigenvalue=eig_solver.eigenvalues()[0];
	m_max_eigenvalue=eig_solver.eigenvalues()[op->get_dimension()-1];

	m_is_computed_min=true;
	m_is_computed_max=false;
}

}
