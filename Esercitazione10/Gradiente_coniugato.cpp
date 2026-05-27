#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD>
double condA(const Eigen::MatrixXd& A)
{
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  Eigen::VectorXd singularValuesA = svd.singularValues();
  return singularValuesA.maxCoeff() / singularValuesA.minCoeff();
}

int main(int argc, char **argv) 
{
  const double tol = 1.0e-10; //cambio perchè è difficile che l'errore relativo sia così basso
  for (unsigned int n=2; n<20; n++){ //provo per 18 iterazioni

  Eigen::MatrixXd B = Eigen::MatrixXd::Random(n, n);

  if (abs(B.determinant()) < 1.0e-15)
    continue;

  Eigen::MatrixXd A = B.transpose() * B;
  Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);

  Eigen::VectorXd b = A * x_ex;

  Eigen::VectorXd x = Eigen::VectorXd::Zero(n);
  Eigen::VectorXd res = b - A * x;
  double res_norm_0 = res.norm();

  const unsigned int it_max = 10000;
  unsigned int it = 0;
  const double res_tol = 1.0e-12;

  Eigen::VectorXd p= res;
  
  while (it < it_max &&
         res.norm() > res_tol * res_norm_0)
  {
    const double alpha_k = ((p.transpose() * res) / (p.transpose() * A * p)).value();
   

    x = x + (alpha_k * p);
    res = b-(A*x);

    const double beta_k = ((p.transpose() * A * res)/(p.transpose() * A * p)).value();
    p=res-(beta_k*p);
    


    it++;
  }

  const auto err_rel = (x_ex.norm() == 0.0) ? (x - x_ex).norm() :
                                              (x - x_ex).norm() / x_ex.norm();

 
  if (err_rel > tol){
    return EXIT_FAILURE;
  } 
} 


  return EXIT_SUCCESS;
}
