#pragma once
#include<queue>
#include<stack>

template<typename T>
class FIFO{
private:
    std::queue<T> q;
public:
void put(const T& nodo){
    q.push(nodo);
}
T get(){
    T fondo = q.front();
    q.pop();
    return fondo;
}
bool empty(){
    return q.empty();
}
};

template<typename T>
class LIFO{
private:
std::stack<T> s;
public:

void put(const T& nodo){
    s.push(nodo);
}

T get(){
    T cima = s.top();
    s.pop();
    return cima;
}

bool empty(){
    return s.empty();
}


};