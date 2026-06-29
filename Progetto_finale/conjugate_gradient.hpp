#pragma once
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD>

struct cg_result {
    Eigen::VectorXd x;
    bool converged;
    unsigned int iterations;
    double relative_residual;
};

double condA(const Eigen::MatrixXd& A)
{
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  Eigen::VectorXd singularValuesA = svd.singularValues();
  return singularValuesA.maxCoeff() / singularValuesA.minCoeff();
}

cg_result conjugate_gradient(const Eigen::MatrixXd& A, const Eigen::VectorXd& b)
{
    //Sappiamo che, se gli input sono validi, la matrice generata A non avrà determinanete nullo:
    //se B ha rango pieno per colonne (cioè le maglie sono i cicli fondamentali trovati correttamente con DFS o De Pina),
    //allora A = BᵀRB è simmetrica
    //definita positiva e il determinante è strettamente positivo:
    //la singolarità capita solo se hai sbagliato a scegliere i cicli e ne hai inserito uno dipendente dagli altri.

   
  const int n = static_cast<int>(b.size());
  Eigen::VectorXd x = Eigen::VectorXd::Zero(n);
  Eigen::VectorXd res = b - A * x;
  double res_norm_0 = res.norm();

  const unsigned int it_max = 10000;
  unsigned int it = 0;
  const double res_tol = 1.0e-12;

  Eigen::VectorXd p = res;

  while (it < it_max &&
         res.norm() > res_tol * res_norm_0)
  {
    const double alpha_k = ((p.transpose() * res) / (p.transpose() * A * p)).value();

    x = x + (alpha_k * p);
    res = b - (A * x);

    const double beta_k = ((p.transpose() * A * res)/(p.transpose() * A * p)).value();
    p = res - (beta_k * p);

    it++;
  }

  cg_result result;
  result.x = x;
  result.converged = (res.norm() <= res_tol * res_norm_0);
  result.iterations = it;
  result.relative_residual = (res_norm_0 > 0) ? res.norm() / res_norm_0 : 0.0;
  return result;
}
