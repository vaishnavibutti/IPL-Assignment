#include<iostream>
#include<vector>
#include<string>
#include<string.h>
#include<map>
#include <algorithm>
// #include "ast.cpp"
extern std::map<string,int> struct_names;
#include "parser.tab.hh"
using namespace std;

class Entries_list{
                   public:
                   string name;
                   signed int offset=0;
                   string type;
                   int size_entry=0;
                   string scope;
                   bool operator()(const Entries_list&e1,const Entries_list&e2) const{
                         return e1.name<e2.name;
                   }

                   
};

class Act_record{
                   public:
                   vector<Entries_list> vars,pars;
                   std:: map<string,Entries_list> all_map;
                   Act_record(){}
                   void print(){
                        // vector<Entries_list> vars1,pars1;
                        // vars1 = vars;
                        // pars1 = pars;
                        // sstd::map<string,std::pair<>>
                              
                              // std::sort(vars1.begin(),vars1.end(),Entries_list());
                              // std::sort(pars1.begin(),pars1.end(),Entries_list());
                              // std:: map<string,Entries_list> all_map;
                                    cout<<"[\n";
                                    string t1,t2;
                                    int it = 0;
                              //   for(int i=0; i<vars1.size(); i++){
                              //             all_map.insert({vars[i].name,vars[i]});
                                                      
                                                      
                              //   }
                              //   int it1=0;


                              //   if(pars.size()>=1)cout<<"\n,\n";
                              //   for(int i=0; i<pars1.size(); i++){
                              //       all_map.insert({pars[i].name,pars[i]});
                              //   }
                                    for(auto i: all_map){
                                    if(it) cout<<"\n,\n";
                                          it+=1;
                                    //   t1 = vars1[i].name;
                                    t1= i.first;
                                          t1 = "\""+t1+"\"";
                                          t2 = i.second.type; 
                                          t2 = "\""+t2+"\"";
                                          string t3 = i.second.scope;
                                          t3="\""+t3+"\"";
                                          cout<<"["<<t1<<",         \"var\",         "<<  t3<<",         "<<i.second.size_entry<<",         "<<i.second.offset<<",\""<<i.second.type<<"\""<<"\n]";
                                    //   cout<<"\n,\n";


                                    }
                                    cout<<"\n]\n";
                   }

};
 
class Content{
        public:
                   string varfun;
                   int size;
                   string offset;
                   string type;
                   Act_record* symbtab;
                   Content(string s){
                                      this->varfun = s;
                   }
                   Content(){}
};

class Symtab{
                   public:
                   std::map<string,Content> Entries;
                   
                   void structinsert(string typeee, string name, int size,string type,declaration_list_class* decl ){
                                      Content temp("struct");
                   
                                      temp.size =0;
                                      temp.offset="\"-\"";
                                      temp.type = type;
                                //       int a = name;
                                      int k=0;
                                      if(decl){
                                            Act_record* te = new Act_record();
                                            for(int i=0; i<(decl->names).size();i++){
                                                  declarator_class* decr = (decl->names)[i];
                                                  Entries_list entry;
                                                  entry.name = decr->name;
                                                  entry.type = decr->type;
                                                   entry.size_entry=decr->num*4;
                                                // cout << decr->name << "may\n";
                                                if(entry.type.substr(0,6)=="struct"){
                                                      // cout << "frgthyju         " << entry.type << endl;
                                                      //   cout << entry.name << " " << struct_names[entry.type]<<"    "<<type << endl;
                                                      int ok = entry.type.find("*");
                                                      int o  = entry.type.find("[");
                                                            if(ok!=std::string::npos)
                                                                  entry.size_entry  = 4*decr->num;
                                                      
                                                            else if(o!=std::string::npos)
                                                                  entry.size_entry  = struct_names[entry.type.substr(0,o)]*decr->num;
                                                            else 
                                                                  entry.size_entry = struct_names[entry.type]*decr->num;
                                                      //    entry.size_entry  = struct_names[entry.type];
                                                      //    cout << "after everything    "<< entry.size_entry << endl;
                                                         
                                                  }
                                                  entry.scope = "local";
                                                // if(k==0){
                                                      entry.offset = k;k=k+entry.size_entry;
                                                // }
                                                // else{
                                                //       entry.offset=k;k=k+
                                                temp.size+=entry.size_entry;
                                                te->all_map.insert({entry.name,entry});
                                                  te->vars.push_back(entry);
                                            }

                                            temp.symbtab = te;
                                          //   temp.symbtab->print();

                                      }
                                          Entries.insert({"struct "+name,temp});
                                          struct_names["struct "+name] = temp.size;
                                      // Entries_list* temp = new Entries_list();
                                      
                                      
                   }
                   void funinsert(fun_declarator_class* fund,string type,declaration_list_class*decl){
                          Content temp("fun");
                   
                                      temp.size =0;
                                      temp.offset="0";
                                      temp.type = type;
                                //       int a = name;
                                      signed int k=0;
                                      int y= 12;
                                    //   cout << "in symtab\n";
                                    //   if(decl){
                                            Act_record* te = new Act_record();
                                            if(decl)
                                            {
                                            for(int i=0; i<(decl->names).size();i++){
                                                  declarator_class* decr = (decl->names)[i];
                                                  Entries_list entry;
                                                  entry.name = decr->name;
                                                  entry.type = decr->type;
                                                  entry.size_entry = decr->num*4;
                                                //   cout << k << "ok\n";
                                                
                                                // cout << entry.type << "   ---->      " << entry.type.find("*")<< "   -----    "<<  << endl;

                                                  if(entry.type.substr(0,6)=="struct"){
                                                      //   cout << entry.name << " " << struct_names[entry.type]<<"    "<<type << endl;
                                                      int ok = entry.type.find("*");
                                                      int o  = entry.type.find("[");
                                                            if(ok!=std::string::npos)
                                                                  entry.size_entry  = 4*decr->num;
                                                      
                                                            else if(o!=std::string::npos)
                                                                  entry.size_entry  = struct_names[entry.type.substr(0,o)]*decr->num;
                                                            else 
                                                                  entry.size_entry = struct_names[entry.type]*decr->num;

                                                  }
                                                  if(k==0){entry.offset = -entry.size_entry;k=entry.offset;}
                                                  
                                                  else{
                                                        entry.offset= k-entry.size_entry;
                                                        k = entry.offset;
                                                      //   cout << entry.offset << endl;
                                                  }
                                                  entry.scope = "local";
                                                  te->all_map.insert({entry.name,entry});
                                                  te->vars.push_back(entry);
                                            }
                                            }
                                          //   for(int i=0; i<(fund->pars).size();i++){
                                                for(int i = (fund->pars).size()-1;i>=0;i--){
                                                  declarator_class* decr = (fund->pars)[i];
                                                  Entries_list entry;
                                                  entry.name = decr->name;
                                                  entry.type = decr->type;
                                                  entry.size_entry = decr->num*4;
                                                  if(entry.type.substr(0,6)=="struct"){
                                                      //   cout << entry.name << " " << struct_names[entry.type]<<"    "<<type << endl;
                                                      int ok = entry.type.find("*");
                                                      int o  = entry.type.find("[");
                                                            if(ok!=std::string::npos)
                                                                  entry.size_entry  = 4*decr->num;
                                                      
                                                            else if(o!=std::string::npos)
                                                                  entry.size_entry  = struct_names[entry.type.substr(0,o)]*decr->num;
                                                            else 
                                                                  entry.size_entry = struct_names[entry.type]*decr->num;
                                                      //    entry.size_entry  = struct_names[entry.type.substr(entry.type.find("*")+1)];
                                                  }
                                                //   if(y==12){entry.offset = 12;y=entry.size_entry+y;}
                                                  
                                                //   else{
                                                      //   entry.offset= k-entry.size_entry;
                                                      //   k = entry.offset;
                                                      entry.offset = y;
                                                      y=y+entry.size_entry;
                                                      entry.scope = "param";
                                                      //   cout << entry.offset << endl;
                                                //   }
                                                te->all_map.insert({entry.name,entry});
                                                  te->pars.push_back(entry);
                                            }

                                            temp.symbtab = te;
                                            //temp.symbtab->print();

                                    //   }
                                          Entries.insert({fund->name,temp});
                                      // Entries_list* temp = new Entries_list();
                   }
                   
                   void printgst(){
                        

                         cout<<"[";
                         int it=0;
                                      for (const auto &entry : Entries)
{  if(it) cout<<",\n";
      cout<<"[         ";
it+=1;
	if (entry.second.varfun == "struct"){
            
            string s = entry.first;
            s = "\"" +s + "\"";
            cout<<s<<",         \"struct\",         \"global\",         "<<entry.second.size<<",         \"-\",\"-\"\n]";
      


      }
      else{
            string s = entry.first;
            s = "\"" +s + "\"";
            string t = entry.second.type;
            t = "\""+t+"\"";
            cout<<s<<",         \"fun\",         \"global\",         "<<entry.second.size<<",         "<<entry.second.offset<<","<<t<<"\n]";


      }
     

                       
                        // if(entry.second.varfun=="fun"){
                        //       // cout<<"hi";
                        //       entry.second.symbtab->print();
                        // }
                        // // entry.second.symbtab->print();}
                        // cout<<"printed\n";
}
cout<<"\n]\n";
                   }




      // string gettype(declaration_list_class* decl, parameter_list_class* param,string id){
      //       vector<declarator_class*> vars;
      //       if (decl)
      //             vars = decl->names;
      //       if(param)
      //       vars.insert((vars).end(),(param->pars).begin(),(param->pars).end());
      //       for(int i=0; i<vars.size(); i++){
      //             if(id.compare(vars[i]->name)==0){
      //                   return vars[i]->type;
      //             }
      //       }
      //       return "not";

      // }
      string constgettype(string type){
            return type;
      }
};


