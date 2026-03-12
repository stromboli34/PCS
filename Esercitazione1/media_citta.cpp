#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, const char*argv[]){
   ifstream ifs(argv[1]);
   if(ifs.is_open()){
    for (int p=1;p<=3;p++){

        
        string location;
        double temp1;
        double temp2;
        double temp3;
        double temp4;
        ifs>>location>>temp1>>temp2>>temp3>>temp4;
    
    double s= (temp1+temp2+temp3+temp4);
    double m= s/4.0;
    
    cout << "Temperatura a "<< location << " e' "<< m << "\n";
    }
}
   
  
   return 0;
}
