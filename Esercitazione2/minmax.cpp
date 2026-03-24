#include <iostream>
#include<cmath>
int main(){
	static const int N=10;
	double arr[N]={2.2,1.2,3.4,5.6,8,12,14,5,7,9.1};
	double minimo=arr[1];
	double massimo=arr[1];
	double s=0.0;
	double var=0.0;
        for (int i=0;i<N;i++){
		minimo=std::min(minimo,arr[i]);
		massimo=std::max(massimo,arr[i]);
		s+=arr[i];

	}
	 for (int i=0;i<N;i++){
		var+=((arr[i])-(s/N))*((arr[i])-(s/N));
	 }	
std::cout <<"Minimo: "<<minimo << "\n";
std::cout<<"Massimo: " <<massimo << "\n";
std::cout << "Media: "<< s/N<< "\n";
std::cout <<"Deviazione standard: "<< std::sqrt(var/(N-1))<< "\n";
return 0;
}
