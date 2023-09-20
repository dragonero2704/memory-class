#include <iostream>
#include <thread>
using namespace std;

int main(){
    cout<<"I'm a dummy process"<<endl;
    int targetvar=567;
    cout<<"targetvar: "<<targetvar<<endl;
    cout<<"targetvar adress: "<<&targetvar<<endl;
    // system("pause");
    //this_thread::sleep_for(std::chrono::seconds(10));
    
    while(true){
        this_thread::sleep_for(std::chrono::seconds(1));
        targetvar++;
        cout<<&targetvar<<" : "<<targetvar<<endl;
    }
    return 0;
}