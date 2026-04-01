#include <iostream>
#include<concepts>
template<typename I> requires std::integral<I>
struct rational{
    I num_;
    I den_;
rational(){
    num_=0;
    den_=1;
}
rational(const I& num_i, const I& den_i){
   num_=num_i;
   den_=den_i;
   semplificazione();
}

rational& semplificazione(){
    if (den_==0){
        return(*this);
    }
else{
int r=1;
I A= num_;
I B=den_;
while (r!=0){
    r=A%B;
    
    A=B;
    B=r;   

}
num_=(num_/A);
den_=(den_/A);
return(*this);

}

}
rational& operator+=(const rational& other) {
    
    if (other.den_==0 && other.num_!=0 && num_!=0 && den_==0){
      return(*this);
    }
   
    else{
       num_=(num_*other.den_)+(other.num_*den_);
       den_=(den_*other.den_);
    
       
    
       return semplificazione();
}

       
}
rational& operator-=(const rational& other) {
     if (other.den_==0 && other.num_!=0 && num_!=0 && den_==0){
         return(*this);
    }
           
    
    else{
       num_=(num_*other.den_)-(other.num_*den_);
       den_=(den_*other.den_);
    
       
    
    

       return semplificazione();
    }
}
rational& operator*=(const rational& other) {

    num_*=other.num_;
    den_*=other.den_;
    return semplificazione(); 
}
rational& operator/=(const rational& other) {

    num_*=other.den_;
    den_*=other.num_;

    return semplificazione();
}

rational operator+(const rational& other) const{
    rational ret= *this;
    ret+=other;
    return ret;
}

rational operator-(const rational& other) const{
    rational ret= *this;
    ret-=other;
    return ret;
}
rational operator*(const rational& other) const{
    rational ret=*this;
    ret*=other;
    return ret;
}
rational operator/(const rational& other) const{
    rational ret=*this;
    ret/=other;
    return ret;
};




I num() const {return num_;}
I den() const {return den_;}
 
};

template<typename I>
std::ostream& operator<<(std::ostream& os, const rational<I>& a) {
    
    if (a.den() == 0) {
        if (a.num() == 0) {
            os << "NaN";
        } else {
            os << "inf";
        }
    } else 
        os << a.num() << "/" << a.den();
    
    

    return os;
}


