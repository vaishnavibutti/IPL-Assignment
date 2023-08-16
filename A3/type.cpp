#include <iostream>
#include <vector>
#include <map>
#include <cstdarg>
#include <string>
#include <cstring>
#include<algorithm>
using namespace std;

//removing void cases
static bool compare_type(string type1,string type2){
      string a= type1;
     string b = type2;
     a= a.substr(0,a.find('['));
     b=b.substr(0,b.find('['));
     a=a.substr(0,a.find('('));
     b=b.substr(0,b.find('('));
     a=a.substr(0,a.find('*'));
     b=b.substr(0,b.find('*'));
     // cout << "base type " << a << "->"<< type1 <<  "  ---------   " << b <<"->"<<type2<< endl;

     if(type1==type2) return 1;

     if(type1=="int"){
          if(type2=="float" ) return 1;
          else return 0;
     }

     if(type1=="float"){
          if(type2=="int" ) return 1;
          else return 0;
     }

    

  
   if(count(type1.begin(),type1.end(),'*')>0 && std::count(type1.begin(),type1.end(),'[')==0){
     //     if(type2.substr())
                         int s_c = count(type1.begin(),type1.end(),'*');
                         if(type2 == type1 || (std::count(type2.begin(),type2.end(),'[')==1&& a==b && count(type2.begin(),type2.end(),'*')==s_c-1 )){return 1;}
                         else{
                              // cout << "error " << type2 << "  ok  "<< type1 << count(type2.begin(),type2.end(),'[')<<endl;
                            return 0;}
                              // error(@$,"Incompatible assignment when assigning to type \""+tem[i].type+"\" from type \""+type1+"\"")     
                         }

 
 
 
 

 

 
     if(std::count(type1.begin(),type1.end(),'*')==0){
          if(std::count(type1.begin(),type1.end(),'[')==1){
               if(type2==a+"*" || (a==b && std::count(type2.begin(),type2.end(),'[')==1 &&  std::count(type2.begin(),type2.end(),'*')==0)){
                    return 1;
               }
               else return 0;
          }

          if(std::count(type1.begin(),type1.end(),'[')>1){
               
                if(count(type1.begin(),type1.end(),'[')==count(type2.begin(),type2.end(),'[') && a==b &&std::count(type2.begin(),type2.end(),'*')==0 ){
                    string tem1,tem2;
                    tem1 = type1;
                    tem2=type2;
                    // int k=-1;
                    int l_1 = tem1.find(']');
                    int l_2 = tem2.find(']');
                    tem1 = tem1.substr(l_1+1,tem1.size()-l_1);
                    tem2 = tem2.substr(l_2+1,tem2.size()-l_2);
                    if(tem1!=tem2) return 0;
                    else return 1;
                    }
               else if(count(type1.begin(),type1.end(),'[')==count(type2.begin(),type2.end(),'[')+1 && a==b && std::count(type2.begin(),type2.end(),')')==1 && std::count(type2.begin(),type2.end(),'*')==1){
                    string tem1,tem2;
                    tem1 = type1;
                    tem2=type2;
                    // int k=-1;
                    int l_1 = tem1.find(']');
                    int l_2 = tem2.find(')');
                    tem1 = tem1.substr(l_1+1,tem1.size()-l_1);
                    tem2 = tem2.substr(l_2+1,tem2.size()-l_2);
                    if(tem1!=tem2) return 0;
                    else return 1;

                    
               }
               else return 0;
          }

     }

     if(count(type1.begin(),type1.end(),'*')>0 && count(type1.begin(),type1.end(),'(')==0){
          if(std::count(type1.begin(),type1.end(),'[')==1){
               if((( std::count(type1.begin(),type1.end(),'*')==std::count(type2.begin(),type2.end(),'*') && std::count(type2.begin(),type2.end(),'[')==1) && a==b) || (std::count(type2.begin(),type2.end(),'*')==std::count(type1.begin(),type1.end(),'*')+1 && std::count(type2.begin(),type2.end(),'[')==0 && a==b))
                    return 1;
               else return 0;
          }
          else{
               
               if(a==b && (std::count(type1.begin(),type1.end(),'*')==std::count(type2.begin(),type2.end(),'*')) && std::count(type1.begin(),type1.end(),'[')==std::count(type2.begin(),type2.end(),'[') ){

                    string tem1,tem2;
                    tem1 = type1;
                    tem2=type2;
                    // int k=-1;
                    int l_1 = tem1.find(']');
                    int l_2 = tem2.find(']');
                    tem1 = tem1.substr(l_1+1,tem1.size()-l_1);
                    tem2 = tem2.substr(l_2+1,tem2.size()-l_2);
                    if(tem1!=tem2) return 0;
                    else return 1;

               }
               else if(a==b && (std::count(type1.begin(),type1.end(),'*')+1==std::count(type2.begin(),type2.end(),'*')) && std::count(type1.begin(),type1.end(),'[')-1==std::count(type2.begin(),type2.end(),'[') &&std::count(type2.begin(),type2.end(),'(')==1 ){

                    string tem1,tem2;
                    tem1 = type1;
                    tem2=type2;
                    // int k=-1;
                    int l_1 = tem1.find(']');
                    int l_2 = tem2.find(')');
                    tem1 = tem1.substr(l_1+1,tem1.size()-l_1);
                    tem2 = tem2.substr(l_2+1,tem2.size()-l_2);
                    if(tem1!=tem2) return 0;
                    else return 1;

               }
               else return 0;

               
          }
     }




     if(count(type1.begin(),type1.end(),'(')==1){
          // cout << "going into (" << endl;
          if( a==b && count(type1.begin(),type1.end(),'*') ==count(type2.begin(),type2.end(),'*') +1  &&  count(type1.begin(),type1.end(),'[')+1 ==count(type2.begin(),type2.end(),'[') ){
               string tem1,tem2;
                    tem1 = type1;
                    tem2=type2;
                    // int k=-1;
                    int l_1 = tem1.find(')');
                    int l_2 = tem2.find(']');
                   
                    tem1 = tem1.substr(l_1+1,tem1.size()-l_1);
                    tem2 = tem2.substr(l_2+1,tem2.size()-l_2);
                    //  cout << tem1 << "  " << tem2 << endl;
                    if(tem1!=tem2) return 0;
                    else return 1;

          }

               else return 0;
          
     }



     
     return 1;
                         
                    }
                   

