#include <iostream>
int main(){
    double ad[4]={0.0, 1.1, 2.2, 3.3};
    float af[8] = {0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
    int ai[3] = {0, 1, 2};
    
    int x=1;
    float y=1.1;
(&y)[1]=0;
std::cout <<"Come cambia x: "<<x<<"\n";

std::cout<< "Primo byte di ad: "<<&ad[0]<<"\n";
std::cout <<"Primo byte di af: "<<&af[0]<<"\n";
std::cout <<"Primo byte di ai: "<<&ai[0]<<"\n";
std::cout <<"Primo byte di x: "<<&x<<"\n";
std::cout <<"Primo byte di y: "<<&y<<"\n";
return 0;
}