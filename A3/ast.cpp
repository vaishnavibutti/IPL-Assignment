#include <iostream>
#include <vector>
#include <map>
#include <cstdarg>
#include <string>
#include <cstring>
#include<bits/stdc++.h>
using namespace std;
// #include "main.cpp"
// #include "location.hh"
// gg
extern map<string , string> decl_entry;
static vector<string> rstack {"%edi","%esi","%edx","%ecx","%ebx","%eax"};
static map<string,vector<string>> labels_map;
extern int label_mark;
extern int no_times;
static int instr_no=0;
static int in_assign = 0;
static int in_assign1 = 0;
static int new_offset ;
extern int present_labels ;
extern vector<string>code_inst;
extern vector<string> truth_val;
static vector<string> bools {"%al","%dl"};
// extern map<string,declaration_list_class*> struct_list;
// extern int lc_num;
static int is_pp=0;
static int fn_call = 0;
static int is_local = 0;
extern std::map<string,int> struct_names;
class abstract_astnode {
                   public:
                   virtual void print(int blanks) = 0;
                //    virtual int typee() {return 1;};

                   int x=5;
                   string type;
                   int lval;
                   // enum typeExp astnode_type;
};
static void printempty(const char* name){
  cout<<"\""<<name<<"\""<<"\n";
}
static void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
{   
	typedef vector<abstract_astnode*>* pv;
	va_list args;
	va_start(args, fmt);
	if ((astname != NULL) && (astname[0] != '\0'))
	{
		cout << "{ ";
		cout << "\"" << astname << "\"" << ": ";
	}
	cout << "{" << endl;
  // int j=0;
	while (*fmt != '\0')
	{
    // j++;
		if (*fmt == 'a')
		{
			char * field = va_arg(args, char *);
			abstract_astnode *a = va_arg(args, abstract_astnode *);
			cout << "\"" << field << "\": " << endl;
		
			a->print(0); 
		}
		else if (*fmt == 's')
		{
			char * field = va_arg(args, char *);
			char *str = va_arg(args, char *);
			cout << "\"" << field << "\": ";

			cout << str << endl;
		}
		else if (*fmt == 'i')
		{
			char * field = va_arg(args, char *);
			int i = va_arg(args, int);
			cout << "\"" << field << "\": ";

			cout << i;
		}
		else if (*fmt == 'f')
		{
			char * field = va_arg(args, char *);
			double f = va_arg(args, double);
			cout << "\"" << field << "\": ";
			cout << f;
		}
		else if (*fmt == 'l')
		{
			char * field = va_arg(args, char *);
			pv f =  va_arg(args, pv);

			cout << "\"" << field << "\": ";
			cout << "[" << endl;
            // cout << f->size() << endl;
			for (int i = 0; i < (int)f->size(); ++i)
			{
                                                                            // cout<<(*f)[i].x<<endl;
                                                                            // cout<<"ok\n" << (*f)[i]->typee()<<endl;
                                                                            // cout << 
				(*f)[i]->print(0);
                                //  cout << "notok\n";                                           // cout << "ok\n";
				if (i < (int)f->size() - 1)
					cout << "," << endl;
				else
					cout << endl;
			}
			cout << endl;
			cout << "]" << endl;
		}
		++fmt;
		if (*fmt != '\0')
			cout << "," << endl;
	}
	cout << "}" << endl;
	if ((astname != NULL) && (astname[0] != '\0'))
		cout << "}" << endl;
	va_end(args);
}

class exp_astnode : public abstract_astnode
{
 public:
 int value = 1;
//  vector<string> exp_asm = new vector<string>(0);
 int pri_type = -1;
 string name ;
 string jump;
 string base_name;
 string basen;
 vector<string> code;
 int is_arrow=0;
 int is_star = 0;
 int is_array = 0;

virtual void print(int blanks){};                  
};
class statement_astnode: public abstract_astnode
{
public:
virtual void print(int blanks){
  // cout << "ok-1\n";
  };
// virtual int typee(){return  2;};


};
class empty_astnode : public statement_astnode
{
//  public:
// virtual void print(int blanks){} 
public:
virtual void print(int blanks) override
                   {
                      //  printempty("empty");    
                   }
};


class seq_astnode: public statement_astnode
{                  public:
	 vector<statement_astnode*>* stats;
                   seq_astnode( vector<statement_astnode*>* s )
                   {
                                      stats = s;
                   }
                   seq_astnode(){stats =new vector<statement_astnode*>(0); }
                   seq_astnode( statement_astnode* s )
                   {


                                      // stats = vector<statement_astnode*> *(malloc(sizeof(s)));
                                      stats=new vector<statement_astnode*>(1);
                                      (*stats)[0]=s;
                                    //   cout << "after pushing  "<< stats->size() << endl; 
                                    //   cout << "type" << stypee()<< endl;

                                      // stats = knew;
                   }
                   void insert(statement_astnode* s){
                     stats->push_back(s);
                   }

                   virtual void print(int blanks) override
                   {
                                     for(int i=0;i<stats->size();i++){
                                       (*stats)[i]->print(0);
                                     }
                   }

};
class assignS_astnode : public statement_astnode
{
                   public:
                   exp_astnode *left, *right;
                //    int typee(){return 3;};
                   assignS_astnode(exp_astnode *l, exp_astnode *r)
                   {
                                      left = l;
                                      right = r;
                                      

                   }

                   virtual void print(int blanks) override {
                                    if(left->is_arrow==1){
                                      in_assign = 1;
                                      left->print(0);
                                      in_assign = 0;
                                       string u = rstack.back();
                                       rstack.pop_back();
                                       string lp = rstack.back();
                                       cout<<"movl  %eax, "<<lp<<endl;
                                       rstack.pop_back();
                                      rstack.push_back(u);
                                       right->print(0);
                                        if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                       if(new_offset!=0)cout << "movl %eax , "<<new_offset<<"("<< lp<<")" <<endl;
                                       else cout << "movl %eax , "<<"("<< lp<<")" <<endl;
                                       rstack.pop_back();
                                      //  rstack.pop_back();
                                       rstack.push_back(lp);
                                       rstack.push_back(u);
                                    }
                                    else if(left->is_star ==1){
                                      in_assign1 = 1;
                                      left->print(0);
                                      in_assign1 = 0;
                                      string u = rstack.back();
                                       rstack.pop_back();
                                       string lp = rstack.back();
                                       cout<<"movl  %eax, "<<lp<<endl;
                                       rstack.pop_back();
                                      rstack.push_back(u);
                                       right->print(0);
                                        if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                        cout << "movl %eax , "<<"("<< lp<<")" <<endl;
                                        rstack.pop_back();
                                      //  rstack.pop_back();
                                       rstack.push_back(lp);
                                       rstack.push_back(u);
                                    }
                                    else if(left->is_array==1){
                                      in_assign1=1;
                                      left->print(0);
                                      in_assign1 = 0;
                                      // cout<<"inassig"<<is_local<<endl;
                                      string u = rstack.back();
                                       rstack.pop_back();
                                       string lp = rstack.back();
                                       cout<<"movl  %eax, "<<lp<<endl;
                                       rstack.pop_back();
                                      rstack.push_back(u);
                                       right->print(0);
                                        if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                        if(!is_local)cout << "movl %eax , "<<"(" << lp <<")"<<endl;
                                        else cout << "movl %eax , " << lp <<endl;
                                        is_local = 0;
                                        rstack.pop_back();
                                      //  rstack.pop_back();
                                       rstack.push_back(lp);
                                       rstack.push_back(u);
                                    }
                                    else{
                                      
                                    right->print(0);
                                    if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                    cout << "movl %eax ,"<< left->base_name <<endl;}
                                    // else cout<<"movl %eax, "<<decl_entry[(left->basen)+]
                                    // cout << "im in  assignS  left name is  "<< left->base_name<< endl;
                   }
};


class return_astnode: public  statement_astnode
{
                   public:
                   exp_astnode * left;
                   return_astnode(exp_astnode* l){
                                      left = l;
                                      // if(type.compare(l->type)!=0){
                                      //   if(type.compare("FLOAT")==0 && l->type.compare("INT")){

                                      //   }
                                      // }
                   }
                   virtual void print(int blanks) override
                   {
                                    left->print(0);

                               
                   }
};
class if_astnode: public  statement_astnode
{
  public:
                   exp_astnode *left;
                   statement_astnode *middle,*right;
                   if_astnode(exp_astnode *l, statement_astnode *m, statement_astnode *r)
                   {
                                      left = l;
                                      middle = m;
                                      right = r;
                   }
                   virtual void print(int blanks) override
                   {
                                     left->print(0);
                                     cout<<"cmpl  $0, %eax\n";
                                     cout<<"je  .L"<<label_mark<<endl;
                                     int false_label_mark = label_mark;
                                     label_mark+=1;
                                     int next_label_mark = label_mark;
                                     label_mark+=1;
                                     middle->print(0);
                                     cout<<"jmp .L"<<next_label_mark<<endl;
                                     cout<<".L"<<false_label_mark<<":"<<endl;
                                     right->print(0);
                                     
                                     cout<<".L"<<next_label_mark<<":"<<endl;
                   }
};
class while_astnode: public  statement_astnode

{
  public:
                   exp_astnode *left;
                   statement_astnode *right;
                   while_astnode(exp_astnode *l, statement_astnode *r)
                   {
                                      left = l;
                                      right = r;
                                      // astnode_type = WhileNode;             
                   }
                   virtual void print(int blanks) override
                   {
                                      cout << "jmp .L"<< label_mark << endl;
                                     label_mark+=1;
                                     int while_label_mark = label_mark-1;
                                     int true_label_mark = label_mark;
                                      cout << ".L"<<to_string(label_mark)<<":"<< endl;
                                      label_mark+=1;
                                      right->print(0);
                                      cout<<"jmp .L"<< while_label_mark<<endl;
                                      cout<<".L"<<to_string(while_label_mark)<<":"<<endl;
                                      left->print(0);
                                      cout<<"cmpl $0, %eax\n jne  .L"<<true_label_mark<<endl;

                                      
                   }
};
class for_astnode: public  statement_astnode
{   
  public:
                 exp_astnode *left,*middle1,*middle2;
                   statement_astnode *right;
                   for_astnode(exp_astnode *l, exp_astnode *m1, exp_astnode *m2, statement_astnode *r)
                   {
                                      left = l;
                                      middle1 = m1;
                                      middle2 = m2;
                                      right = r;
                   }
                   virtual void print(int blanks) override
                   {
                                   left->print(0);
                                   cout << "jmp .L"<< label_mark << endl;

                                     label_mark+=1;
                                     int while_label_mark = label_mark-1;
                                     int true_label_mark = label_mark;
                                      cout << ".L"<<to_string(label_mark)<<":"<< endl;
                                      label_mark+=1;
                                      right->print(0);
                                      middle2->print(0);
                                      cout<<"jmp .L"<< while_label_mark<<endl;
                                      cout<<".L"<<to_string(while_label_mark)<<":"<<endl;
                                      middle1->print(0);
                                      cout<<"cmpl $0, %eax\n jne  .L"<<true_label_mark<<endl;
                   }
};

class ref_astnode: public  exp_astnode
{
  public:
virtual void print(int blanks){}; 

};

class identifier_astnode: public  ref_astnode
{                  public:
                   char* stats;
                   string st;
                   identifier_astnode(string s){
                         st=s;
                        s = "\""+s+"\"";
                       
                                      stats=new char[s.length()+1];
                                      strcpy(stats,s.c_str());     
                                    // strcpy(stats,"\"");              
                                      // stats = s.c_str();
                                    //   cout << stats << endl;
                   }
                   virtual void print(int blanks) override
                   {
                                      // printAst(
                                      //                    "","s",
                                      //                    "identifier",stats
                                      // );
                                      // string s = stats;

                                          if(fn_call) cout<<"leal " << decl_entry[st] << ", " << rstack.back() << endl;
                                      else cout << "movl " << decl_entry[st] << ", " << rstack.back() << endl;
                   }
};

class proccall_astnode: public  statement_astnode
{
    public:
                   vector<exp_astnode*>*stats;
                   identifier_astnode* id;
                   proccall_astnode( identifier_astnode* fname,vector<exp_astnode *>* s )
                   {
                                      stats = s;
                                      id=fname;
                   }
                   proccall_astnode( identifier_astnode* fname )
                   {
                                    //   stats = s;
                                    // stats=NULL;
                                    stats=new vector<exp_astnode*>(0);
                                      id=fname;
                   }
                   virtual void print(int blanks) override
                   {
                      for(int i=stats->size()-1; i>=0; i--){
                                        (*stats)[i]->print(0);
                                        cout<<"pushl  %eax\n";
                                        if(is_pp){cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}

                      }
                                    cout<<"call "<<id->st<<endl;
                                    cout<<"add  $"<<to_string(4*stats->size())<<",  %esp\n";
                                     
                   }
};
class printf_astnode : public statement_astnode{
  public:
        int lc_num;
        vector<exp_astnode*>*stats;
        printf_astnode( int fname,vector<exp_astnode *>* s )
                   {
                                      stats = s;
                                      lc_num=fname;
                   }
                   printf_astnode( int fname )
                   {
                                    //   stats = s;
                                    // stats=NULL;
                                    stats=new vector<exp_astnode*>(0);
                                      lc_num=fname;
                   }
                   virtual void print(int blanks) override
                   {
                                    int lc_fn = lc_num;
                                    // lc_num+=1;
                                      for(int i=stats->size()-1; i>=0;i--){
                                        (*stats)[i]->print(0);
                                        cout<<"pushl  "<<rstack.back()<<endl;
                                      }
                                      cout<<"pushl  $.LC"<<lc_fn<<endl;
                                      cout<<"call printf\naddl  $"<<((stats->size())+1)*4<<", %esp"<<endl;
                   }

        
        
};



class arrayref_astnode: public  ref_astnode
{
  public:
  exp_astnode* exp,*post_exp;
  arrayref_astnode(exp_astnode* post,exp_astnode* e){
    post_exp=post;
    exp=e;
  }
  virtual void print(int blanks) override
                   {
                          // cout<<"in arrow"<<endl;
                                    string actual_loc = post_exp->base_name;
                                    // cout<<post_exp->base_name<<"ues"<<endl;
                                    actual_loc = actual_loc.substr(0,actual_loc.find('('));
                                    int lo = stoi(actual_loc);
                                    
                                   if(lo<0){
                                     exp->print(0);
                                     is_local = 1;
                                    //  cout<<is_local <<endl;
                                     cout<<"movl "<<actual_loc<<"(%ebp,"<<rstack.back()<<",4), "<<rstack.back()<<endl;
                                   }

                              
                                    else{
                                      // is_local = 0;
                                      string h = post_exp->base_name;
                                      exp->print(0);
                                      cout<<"imull $4,  "<<rstack.back()<<endl;;
                                      string ea = rstack.back();
                                      rstack.pop_back();
                                      cout<<"movl "<<ea<<", "<<rstack.back()<<endl;
                                      string off_i = rstack.back();
                                      rstack.pop_back();
                                      rstack.push_back(ea);
                                      cout<<"movl "<<h<<",  "<<rstack.back()<<endl;
                                      cout<<"addl "<<off_i<<",  "<<rstack.back()<<endl;
                                      if(!in_assign1)cout<<"movl ("<<rstack.back()<<"), "<<rstack.back()<<endl;
                                      rstack.pop_back();
                                      rstack.push_back(off_i);
                                      rstack.push_back(ea);

                                      
                                    }
                   }


};
class member_astnode: public  ref_astnode
{
  public:
  exp_astnode* exp;
  identifier_astnode* id;
  member_astnode(exp_astnode* e,identifier_astnode* i){
    exp=e;
    id=i;
  }
  virtual void print(int blanks){
    cout<<"movl "<<id->st<<",  "<<rstack.back()<<endl;
  }

};
class arrow_astnode: public  ref_astnode
{
   public:
  exp_astnode* exp;
  identifier_astnode* id;
  int n;
  arrow_astnode(exp_astnode* e,identifier_astnode* i,int no){
    exp=e;
    id=i;
    n = no;
  }
  virtual void print(int blanks){
    // cout<<"in arrow"<<exp->base_name<<exp->type<<exp->basen<<endl;
    // int a;

   cout<<"movl "<<id->st<<",  "<<rstack.back()<<endl;
   if(in_assign){new_offset = n;}
   else{if(n)
   cout<<"movl  "<<n<<"("<<rstack.back()<<"),  "<<rstack.back()<<endl;
   else {
    cout<<"movl  "<<"("<<rstack.back()<<"),  "<<rstack.back()<<endl; 
   }
   }
  }


};
class stringconst_astnode: public  exp_astnode
{
  public:
              char *num;
              stringconst_astnode(string i){
                                //  num = i;
                                // i = "\""+i+"\"";
                                // strcpy(num,i.c_str());
                                num =new char[i.length()+1];
                                strcpy(num,i.c_str());
              }     
              virtual void print(int blanks) override
              {
                                 printAst(
                                                    "","s",
                                                    "stringconst",num
                                 );
              }
};

class op_binary_astnode: public  exp_astnode
{
  public:
                 char* st;
                 exp_astnode *left, *right;
                 string type ;
                 string ju;
                 op_binary_astnode(string s, exp_astnode *l, exp_astnode *r)
                 {
                                    // string = s;
                                    left = l;
                                    right = r;
                                    s = "\"" + s + "\"";
                                    st = new char[s.length()+1];
                                    strcpy(st,s.c_str());
                                    // cout<<"opbinary"<<st<<" "<<endl;
                                    if(l->type!="float"&&r->type!="float") type = "int";
                                    else  type = "float";
                 } 

                 virtual void print(int blanks) override
                 {
                  //  cout<<"here  "<<st<<endl;
                  
                 
                 left->print(0);
                 if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                 string k = rstack.back();
                
                   rstack.pop_back();
                   int b = 0;
                  if(k=="%eax"){
                    b = 1;
                    string g = rstack.back();
                    // cout<<"g here is"<<g<<endl;
                    cout<<"movl %eax, "<<g<<endl;
                    k = g;
                    // cout<<rstack[rstack.size()-2]<<"beforepop"<<rstack[rstack.size()-1]<<endl;
                    rstack.pop_back();
                    // cout<<rstack[rstack.size()-2]<<"affterpop"<<rstack[rstack.size()-1]<<endl;
                     rstack.push_back("%eax");
                    //  cout<<"right now top is "<<rstack.back()<<" "<<k<<endl;
                  }
                 
                 right->print(0);
                 if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                 if(st[1]=='D'){
                   
                  rstack.pop_back();   //eax remove
                 
                  string l=rstack.back();
                   int a = 0;
                   if(l=="%edx"){
                        a = 1;
                        rstack.pop_back();
                        l = rstack.back();
                   }
                   cout << "movl  %eax  ," << l << endl;
                   cout << "movl  "<< k << ", %eax\n";
                   cout << "cltd\n";
                   cout << "idivl "<< rstack.back() << endl;
                   
                  //  rstack.push_back(l);
                  
                   if(a) rstack.push_back("%edx");
                    rstack.push_back(k);
                    rstack.push_back("%eax");


                  

                   return;

                 }
                 
                //  if(st[1]=='D')
                // cout<<"reached"<<endl;
                //  cout <<"cltd\n"<< "idivl  " << rstack[rstack.size()-1]<< endl;
                 if(st[1]=='P')  {
                    if(left->type.find('*')!=std::string::npos ){
                          cout<<"imull  $4"<<",  "<<rstack.back()<<endl;
                   } 
                   else if(right->type.find('*')!=std::string::npos){
                      cout<<"imull  $4"<<",  "<<k<<endl;
                   }
                    cout << "addl " << k << ", " << rstack.back() << endl;
                    
                 }
                 if(st[1]=='M' && st[2]=='I') {
                  //  cout<<left->type<<"minus"<<endl;
                    // cout<<right->type<<"minus"<<endl;
                   if(left->type.find('*')!=std::string::npos ){
                          cout<<"imull  $4"<<",  "<<rstack.back()<<endl;
                   } 
                   else if(right->type.find('*')!=std::string::npos){
                      cout<<"imull  $4"<<",  "<<k<<endl;
                   }
                   cout << "subl " << rstack.back()<< ", " << k << endl;
                   cout<<"movl  "<<k<<",  "<<rstack.back()<<endl;
                }
                 if(st[1]=='M' && st[2]=='U') cout << "imull " <<k << ", " << rstack.back() << endl;
                 if(st[1]=='O') {
                   
                   cout << "addl " << k<< ", " << rstack.back() << endl;
                  //  cout << "sge " << k << ", " << k << ", "<< "$1"<< endl;
                  cout << "cmpl "<< "$1" << ", " <<rstack.back() << endl;
                    cout << "setge "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);

                 }
                 if(st[1]=='A') {
                  // cout<<"in and"<<endl;
                   cout << "cmpl "<< "$0" << ", " <<k << endl;
                    cout << "setne "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< k << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                     cout << "cmpl "<< "$0" << ", " <<rstack.back() << endl;
                    cout << "setne "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                     ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                  cout<<"andl "<<k<<",  "<<rstack.back()<<endl;
                 }
                 if(st[1]=='L' && st[2]=='T'){
                  //  cout << "slt "<< k << ", "<< k << ", "<< rstack.back() << endl;
                    cout << "cmpl "<< rstack.back()  << ", " <<k<< endl;
                    cout << "setl "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                 }
                 if(st[1]=='L' && st[2]=='E'){
                  //  cout << "sle "<< k << ", "<< k << ", "<< rstack.back() << endl;
                 cout << "cmpl "<< rstack.back()  << ", " <<k<< endl;
                     cout << "setle "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                 }
                 if(st[1]=='G' && st[2]=='T'){
                  //  cout << "sgt "<< k << ", "<< k << ", "<< rstack.back() << endl;
                 cout << "cmpl "<<rstack.back()  << ", " <<k<< endl;
                //  cout<<"asdfdsfasdf"<<bools.back()<<
                     cout << "setg "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                 }
                 if(st[1]=='G' && st[2]=='E'){
                  //  cout << "sge "<< k << ", "<< k << ", "<< rstack.back() << endl;
                    cout << "cmpl "<< rstack.back()  << ", " <<k<< endl;
                     cout << "setge "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                }
                 if(st[1]=='E'){
                  //  cout << "seq "<< k << ", " << k << ", "<< rstack.back() << endl;
                 cout << "cmpl "<< rstack.back()  << ", " <<k << endl;
                     cout << "sete "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                 }
                 if(st[1]=='N'){
                  //  cout << "sne "<< k << ", " << k << ", "<< rstack.back() << endl;
                cout << "cmpl "<< rstack.back()  << ", " <<k << endl;
                    cout << "setne "<<bools.back()<<"\n";
                    cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                    string ty = bools.back();
                    bools.pop_back();
                    bools.insert(bools.begin(),ty);
                 }
                 if(b){
                       string l = rstack.back();
                  rstack.pop_back();
                 rstack.push_back(k);
                 rstack.push_back(l);
                //  cout<<k<<"------------ "<<l<<endl;
                 }
                 else{
                   cout<<"movl  "<<rstack.back()<<",  "<<k<<endl;
                   rstack.push_back(k);
                 }
                 


                 }
};
class op_unary_astnode: public  exp_astnode
{
  public:
                 char* st;
                 exp_astnode *left;
                 string type;
                 op_unary_astnode(string s, exp_astnode *l)
                 {
                                s = "\"" + s + "\"";
                                    st = new char[s.length()+1];
                                    strcpy(st,s.c_str()); 
                                    // st = s;
                                    left = l;
                                    if(s.compare("TO_FLOAT")==0) type = "float";
                                    if(s.compare("TO_INT")==0) type = "int";
                                    else type = l->type;
                                  
                 }  
                 op_unary_astnode(string s)
                 {
                                    // st = s;
                                    s = "\"" + s + "\"";
                                    // cout<<s<<endl;
                                    st = new char[s.length()+1];
                                    strcpy(st,s.c_str());
                                    
                                    
                                    // left = l;
                                  
                 } 
                 void change_type(){
                   string s = st;
                  //  s=s;
                  
                  //  cout << s <<"string s = st" << "--" <<s[8]<<s.compare("ADDRESS\0")<<endl;
                   if(s=="\"NOT\""){
                     type = "int";
                     lval = 0;
                     if(left->type.substr(0,6)=="struct" && left->type.find('*')==string::npos &&left->type.find('[')==string::npos ){
                       type = "error";
                     }
                   }
                   else if(s=="\"PP\""){
                     if(!left->lval) {type = "erro"; return;}
                     if(left->type.find('[')!=string::npos){type = "error"; return ;}
                     if(left->type.substr(0,6)=="struct" && left->type.find('*')==0) {type = "error"; return;}
                     if(left->type=="string") {type ="error";return;}
                      type = left->type;
                      lval = 0;
                   }
                   else if (s=="\"UMINUS\""){
                     if(left->type == "int" ||  left->type == "float"){type = left->type;}
                     else type="error";
                   }
                   else if(s=="\"DEREF\""){
                     if(left->type=="void*"){type="error";return;}
                     if(left->type.find('*')==std::string::npos && left->type.find('[')==std::string::npos){
                       type="erro";
                       return;
                     }
                     if((left->type).find("(")!=std::string::npos){
                       type = (left->type).erase((left->type).find("("),3);
                     }
                     else if((left->type).find('[')!=std::string::npos){
                       int lops = (left->type).find('[');
                       int hops = (left->type).find(']');
                       type = (left->type).erase(lops,hops-lops+1);
                       
                     }
                     else if((left->type).find('*')!=std::string::npos){
                       type = (left->type).erase((left->type).find("*"),1);
                     }
                     else type="error";
                     lval = 1;
                   } 
                   else if(s.compare("\"ADDRESS\"")==0){
                    //  cout << "in address \n";      
                     if(!left->lval) {this->type = "erro"; return;}
                     lval = 0;
                    //  cout << "in address \n";            
                     if((left->type).find("(")!=std::string::npos) this->type ="error";
                     else if ((left->type).find("[")==std::string::npos) this->type = left->type + "*";
                     else if ((left->type).find("[")!=std::string::npos) this->type = left->type.substr(0,(left->type).find("[")) + "(*)"+ left->type.substr((left->type).find("["));
                   }
                 }
                 virtual void print(int blanks) override
                 {
                                  // cout<<st<<"asdkfhkdsf"<<endl;
                                  if(st[1]=='U'){
                                    left->print(0);
                                    if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                    cout<<"negl "<<rstack.back()<<endl;
                                  }
                                  if(st[1]=='N'){
                                      left->print(0);
                                      if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                      cout<<"cmpl $0, "<<rstack.back()<<endl;
                                      // cout<<"sete %al\nmovzbl %al,  "<<rstack.back()<<endl;
                                      cout << "sete "<<bools.back()<<"\n";
                                      cout << "movzbl "<<bools.back()<<", "<< rstack.back() << endl;
                                      string ty = bools.back();
                                      bools.pop_back();
                                      bools.insert(bools.begin(),ty);
                                  }
                                  if(st[1]=='P'){
                                    left->print(0);
                                    // cout<<"here"<<is_pp<<endl;
                                    is_pp = 1;
                                    // cout<<"addl $1, "<<rstack.back()<<endl;
                                  }
                                  if(st[1]=='A'){
                                    string lop = (left->base_name).substr(0,(left->base_name).find('('));
                                    int u = stoi(lop);
                                    // cout<<left->type<<struct_names[left->type]<<endl;
                                    if(struct_names[left->type])
                                    u = u - (struct_names[left->type]-4);
                                    else 
                                     u = u;
                                    lop = to_string(u)+"(%ebp)";
                                    cout<<"leal "<<lop<<",  "<<rstack.back()<<endl;
                                  }
                                  if(st[1]=='D'){
                                    left->print(0);
                                    if(!in_assign1){
                                      cout<<"movl ("<<rstack.back()<<"),  "<<rstack.back()<<endl;
                                    }
                                    

                                  }
                 }
};
class assignE_astnode: public  exp_astnode
{
                   public:
                   exp_astnode *left, *right;
                   assignE_astnode(exp_astnode *l, exp_astnode *r)
                   {
                                      left = l;
                                      right = r;

                   }
                   virtual void print(int blanks) override
                   {
                    if(left->is_arrow==1){
                                       in_assign = 1;
                                      left->print(0);
                                      in_assign = 0;
                                       string u = rstack.back();
                                       rstack.pop_back();
                                       string lp = rstack.back();
                                       cout<<"movl  %eax, "<<lp<<endl;
                                       rstack.pop_back();
                                      rstack.push_back(u);
                                       right->print(0);
                                        if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                       if(new_offset!=0)cout << "movl %eax , "<<new_offset<<"("<< lp<<")" <<endl;
                                       else cout << "movl %eax , "<<"("<< lp<<")" <<endl;
                                       rstack.pop_back();
                                      //  rstack.pop_back();
                                       rstack.push_back(lp);
                                       rstack.push_back(u);
                       }
                       else if(left->is_star ==1){
                                      in_assign1 = 1;
                                      left->print(0);
                                      in_assign1 = 0;
                                      string u = rstack.back();
                                       rstack.pop_back();
                                       string lp = rstack.back();
                                       cout<<"movl  %eax, "<<lp<<endl;
                                       rstack.pop_back();
                                      rstack.push_back(u);
                                       right->print(0);
                                        if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                        cout << "movl %eax , "<<"("<< lp<<")" <<endl;
                                        rstack.pop_back();
                                      //  rstack.pop_back();
                                       rstack.push_back(lp);
                                       rstack.push_back(u);
                                    }
                       else if(left->is_array==1){
                                      in_assign1=1;
                                      left->print(0);
                                      in_assign1 = 0;
                                      string u = rstack.back();
                                       rstack.pop_back();
                                       string lp = rstack.back();
                                       cout<<"movl  %eax, "<<lp<<endl;
                                       rstack.pop_back();
                                      rstack.push_back(u);
                                       right->print(0);
                                        if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                         if(!is_local)cout << "movl %eax , "<<"(" << lp <<")"<<endl;
                                        cout << "movl %eax , "<< lp <<endl;
                                        is_local = 0;
                                        rstack.pop_back();
                                      //  rstack.pop_back();
                                       rstack.push_back(lp);
                                       rstack.push_back(u);
                    }
                                    else{
                                      
                                    right->print(0);
                                    if(is_pp) {cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                    cout << "movl %eax ,"<< left->base_name <<endl;}
                                      // );
                   }
};
class funcall_astnode: public  exp_astnode
{
  public:
                    vector<exp_astnode *>* stats;
                    // char* id;
                    identifier_astnode* id;

                   funcall_astnode( identifier_astnode* fname,vector<exp_astnode *>* s )
                   {
                                      stats = s;
                                      id=fname;
                   }
                   funcall_astnode( identifier_astnode* fname)
                   {
                                    //   stats = s;
                                    stats=new vector<exp_astnode*>(0);
                                    

                                      id=fname;
                   }
                   virtual void print(int blanks) override
                   {
                                    for(int i=stats->size()-1; i>=0; i--){
                                        if((*stats)[i]->type.find('[')!=std::string::npos)
                                        fn_call = 1;
                                        (*stats)[i]->print(0);
                                        fn_call = 0;
                                        cout<<"pushl  %eax\n";
                                        if(is_pp){cout<<"addl $1, "<<rstack.back()<<endl; is_pp=0;}
                                    }
                                    cout<<"call "<<id->st<<endl;
                                    cout<<"add  $"<<to_string(4*stats->size())<<",  %esp\n";
                   }
};

class expression_list_class{
    public:
        vector<exp_astnode *>* exps;
        expression_list_class(exp_astnode * s){
            exps=new vector<exp_astnode*>(1);
                                      (*exps)[0]=s;
        }
        void insert(exp_astnode * a){
            exps->push_back(a);
        }

};
class intconst_astnode: public  exp_astnode
{
  public:
              char* num;
              int no;
              intconst_astnode(string i){
                                //  num = stoi(i);
                                // i = "\"" + i+"\"";
                                // num =new char[i.length()+1];
                                // strcpy(num,i.c_str());
                                no = stoi(i);
                                // cout << "created a int const node\n" ;
              }     
              virtual void print(int blanks) override
              {
                                //  printAst(
                                //                     "","i",
                                //                     "intconst",no
                                //  );
  // cout << "trying to print \n" ;
                                cout << "movl  $" <<no << ", "<< rstack.back() << endl; 
              }
};
class floatconst_astnode: public  exp_astnode
{
  public:
              char* num;
              float no;
              floatconst_astnode(string i){
                                //  num = stof(i);
                                // i = "\"" + i+"\"";
                                // num =new char[i.length()+1];
                                // strcpy(num,i.c_str());
                                no = stof(i);
              }     
              virtual void print(int blanks) override
              {
                                 printAst("","f",
                                                    "floatconst",no
                                 );
              }
};
// extern std::map<string,abstract_astnode*> ast;




// class declarator_class{};
class declarator_class{
                        public:
                   int num;
                   bool isarray=false;
                   string name;
                   string type;
                  //  location_type locate;
                  int locate;
                  // IPL::Parser::location_type locate;
                  string basetype;
                   
                  //  declarator_class(declarator_class* dec,string num){
                  //                     this->num = (dec->num)*stoi(num);
                  //                     this->isarray = true;
                  //                     this->type = this->type + '['+
                  //  }
                  void insert(string int_con){
                      this->type = this->type + '['+int_con +']';
                      this->isarray = true;
                      this->num = (this->num)*stoi(int_con);

                  }
                   declarator_class(string num){
                                      this->num = 1;
                                      this->type="";
                                      this->name = num;
                   }
                   declarator_class(declarator_class* pointer){
                                      if(pointer->isarray){
                                                         this->num=pointer->num;
                                                         this->isarray=true;
                                      }
                                      else {this->num = 1;}
                                      this->type = "*"+pointer->type;
                                      this->name = pointer->name;

                   }
};

class declarator_list_class{
                   public:
                   int num;
                   vector<declarator_class*> names;
                   
                   declarator_list_class(declarator_class* dec){
                                      this->num = dec->num;
                                      names.push_back(dec);

                   }
                  //  declarator_list_class(declarator_list_class* dec1, declarator_class* dec2){
                  //                     this->num = (dec1->num)+(dec2->num);

                  //  }
                  void insert(declarator_class* dec){
                        names.push_back(dec);
                        this->num = (this->num)+(dec->num);
                  }
};

class type_specifier_class{
                   public:
                   int size;
                   string type;
                   type_specifier_class(string s){
                           size = 4;
                           type = s;
                          //  if(s=="void") size=0;
                   }
                   type_specifier_class(string w, string iden){
                          //  gst.printgst();
                          type = "struct " + iden;
                  
                   }
};

class declaration_class{
                   public:
                   int size;
                   vector<declarator_class*> names;
                   declaration_class(type_specifier_class* typ, declarator_list_class* dec){
                                      this->size = (typ->size)*(dec->num);
                                      this->names = dec->names;

                                      for(int i=0; i<(this->names).size();i++){
                                        (this->names)[i]->type = typ->type +(this->names)[i]->type;
                                        (this->names)[i]->basetype = typ->type;
                                      }

                   }
};

class declaration_list_class{
                   public:
                   int size;
                   vector<declarator_class*> names;
                   declaration_list_class(declaration_class* dec){
                                      this->size = dec->size;
                                      this->names = dec->names;
                   }
                   void insert(declaration_class* dec){
                                      this->size = (dec->size)+(this->size);
                                      (this->names).insert((this->names).end(),(dec->names).begin(),(dec->names).end());
                   }
};

class parameter_declaration_class{

        public:
        vector<declarator_class*> pars;
        parameter_declaration_class(type_specifier_class* typ, declarator_class* decl){
                          // cout << pars.size() << "ok\n";
                          this->pars.push_back(decl);
                          pars[0]->type = typ->type+pars[0]->type;

        }
};

class parameter_list_class{
        public:
        vector<declarator_class*> pars;
                   
                   parameter_list_class(parameter_declaration_class* dec){
                                      
                                      this->pars = dec->pars;

                   }

                  void insert(parameter_declaration_class* dec){
                        (this->pars).insert((this->pars).end(),(dec->pars).begin(),(dec->pars).end());
                        
                  }
};

class fun_declarator_class{
 public:
 string name;
 vector<declarator_class*> pars;
 fun_declarator_class(string s){
                                    // name=new char[s.length()+1];
                                      // strcpy(name,s);
                                      name = s; 
 }
 fun_declarator_class(string s, parameter_list_class* par_list){
    name = s;
    this->pars = par_list->pars;
 }
 
};
// class declaration_list_class{};
// class declaration_class{};
// class fun_declarator_class{};
// class declarator_list{};
// class parameter_declaration_class{};
// class parameter_list_class{};

// class fun_decla

