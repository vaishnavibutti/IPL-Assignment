#include <iostream>
#include <vector>
#include <map>
#include <cstdarg>
#include <string>
#include <cstring>
using namespace std;
// #include "main.cpp"
// #include "location.hh"

class abstract_astnode 
{
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
	while (*fmt != '\0')
	{
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
                       printempty("empty");    
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
                                      printAst(
                                                         "","l",
                                                        "seq", stats
                                      );
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
                                     printAst(
                                                        "assignS","aa",
                                                        "left",left,
                                                        "right",right
                                     );
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
                                      printAst(
                                                         "","a",
                                                         "return",left
                                      );
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
                                      printAst(
                                                         "if","aaa",
                                                         "cond",left,
                                                         "then",middle,
                                                         "else",right
                                      );
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
                                      printAst(
                                                         "while","aa",
                                                         "cond",left,
                                                         "stmt",right
                                      );
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
                                      printAst(
                                                         "for","aaaa",
                                                         "init",left,
                                                         "guard",middle1,
                                                         "step",middle2,
                                                         "body", right
                                      );
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
                   
                   identifier_astnode(string s){
                        s = "\""+s+"\"";
                                      stats=new char[s.length()+1];
                                      strcpy(stats,s.c_str());     
                                    // strcpy(stats,"\"");              
                                      // stats = s.c_str();
                                    //   cout << stats << endl;
                   }
                   virtual void print(int blanks) override
                   {
                                      printAst(
                                                         "","s",
                                                         "identifier",stats
                                      );
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
                                      id=fname;
                   }
                   virtual void print(int blanks) override
                   {
                                      printAst(
                                                         "proccall","al",
                                                         "fname",id,
                                                         "params",stats
                                      );
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
                                      printAst(
                                                         "arrayref","aa",
                                                         "array",post_exp,
                                                         "index",exp
                                      );
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
    printAst(
                "member","aa",
                "struct", exp,
                "field", id
    );
  }

};
class arrow_astnode: public  ref_astnode
{
   public:
  exp_astnode* exp;
  identifier_astnode* id;
  arrow_astnode(exp_astnode* e,identifier_astnode* i){
    exp=e;
    id=i;
  }
  virtual void print(int blanks){
    printAst(
                "arrow","aa",
                "pointer", exp,
                "field", id
    );
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
                                    printAst(
                                                       "op_binary","saa",
                                                       "op",st,
                                                       "left",left,
                                                       "right", right
                                    );
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
                                    printAst(
                                                       "op_unary","sa",
                                                       "op",st,
                                                       "child",left
                                                       
                                    );
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
                                      printAst(
                                                         "assignE","aa",
                                                         "left",left,
                                                         "right",right
                                      );
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
                                      printAst(
                                                         "funcall","al",
                                                         "fname",id,
                                                         "params",stats
                                      );
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
              }     
              virtual void print(int blanks) override
              {
                                 printAst(
                                                    "","i",
                                                    "intconst",no
                                 );
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

