%skeleton "lalr1.cc"
%require  "3.0.1"

%defines 

%define api.namespace {IPL}
%define api.parser.class {Parser}
%define api.location.type {IPL::location}

%define parse.trace

%code requires{
       #include "ast.cpp"
       #include "type.cpp"
       #include "location.hh"
       #include "symtab.cpp"
       
   namespace IPL {
      class Scanner;
   }
     extern Symtab gst;
  // # ifndef YY_NULLPTR
  // #  if defined __cplusplus && 201103L <= __cplusplus
  // #   define YY_NULLPTR nullptr
  // #  else
  // #   define YY_NULLPTR 0
  // #  endif
  // # endif

}

%printer { std::cerr << $$; } STRUCT
%printer { std::cerr << $$; } IDENTIFIER
%printer { std::cerr << $$; } VOID
%printer { std::cerr << $$; } INT
%printer { std::cerr << $$; } FLOAT
%printer { std::cerr << $$; } MAIN
%printer { std::cerr << $$; } PRINTF
%printer { std::cerr << $$; } OR_OP
%printer { std::cerr << $$; } AND_OP
%printer { std::cerr << $$; } EQ_OP
%printer { std::cerr << $$; } NE_OP
%printer { std::cerr << $$; } LE_OP
%printer { std::cerr << $$; } GE_OP
%printer { std::cerr << $$; } PTR_OP
%printer { std::cerr << $$; } INC_OP
%printer { std::cerr << $$; } INT_CONSTANT
%printer { std::cerr << $$; } FLOAT_CONSTANT
%printer { std::cerr << $$; } STRING_LITERAL
%printer { std::cerr << $$; } IF
%printer { std::cerr << $$; } ELSE
%printer { std::cerr << $$; } WHILE
%printer { std::cerr << $$; } FOR
%printer { std::cerr << $$; } RETURN
%printer { std::cerr << $$; } COMMENT




%parse-param { Scanner  &scanner  }
%locations
%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include <string>
   #include <vector>
   #include <utility>

   
   #include "scanner.hh"
   int nodeCount = 0;
   declaration_list_class* decl = nullptr;
//    parameter_list_class* param = nullptr;
   string a="";
   string fntype="null";
   std::map<string,abstract_astnode*> ast;
   std::map<string,int> struct_names;
   std::map<string,string> decl_var;
   vector<Entries_list> cur_pars;
   std::map<int,string> struct_err;
//    string ex="";
     int f=0;
   string temp_struct = "";
   int change = 1;
//    int param =0;
int lc_num=0;
std::map<int,string> asm_fun;
map<string,string> decl_entry;
map < string , vector<string>> asm_as;
int is_una=1;
string exp_name;
int no_times = 0;
vector<string> code_inst;
vector<string> truth_values;
int label_mark = 2;;
int present_labels = 1;
int fun_size;
int is_leave=0;
int print_there=0;
// int is_mem = 0;
vector<map<string,string>> fn_decl_list;
map<string,declaration_list_class*> struct_list;
// int pri_type=-1;
//identifier 0
// std::map<string,vector<string>> all_asm;

   
   
   
   

#undef yylex
#define yylex IPL::Parser::scanner.yylex

}




%define api.value.type variant
%define parse.assert

%start program



%token '\n'
%token <std::string> STRUCT OTHERS
%token <std::string> IDENTIFIER 
%token <std::string> VOID 
%token <std::string> INT 
%token <std::string> FLOAT 
%token <std::string> MAIN 
%token <std::string> PRINTF
%token <std::string> OR_OP 
%token <std::string> AND_OP
%token <std::string> EQ_OP
%token <std::string> NE_OP
%token <std::string> LE_OP
%token <std::string> GE_OP
%token <std::string> PTR_OP
%token <std::string> INC_OP
%token <std::string> INT_CONSTANT
%token <std::string> FLOAT_CONSTANT
%token <std::string> STRING_LITERAL
%token <std::string> IF
%token <std::string> ELSE
%token <std::string> WHILE
%token <std::string> FOR
%token <std::string> RETURN
%token <std::string> COMMENT
%token ',' ':' '(' ')' '+' '{' '}' '<' '>' '-' '*' '/' '!'

%nterm <type_specifier_class*> type_specifier
%nterm <fun_declarator_class*> fun_declarator 
%nterm <parameter_list_class*> parameter_list 
%nterm <parameter_declaration_class*> parameter_declaration
%nterm <declarator_class*> declarator_arr
%nterm <declarator_class*> declarator 
%nterm <declaration_list_class*> declaration_list
%nterm <declaration_class*> declaration
%nterm <declarator_list_class*> declarator_list
%nterm <abstract_astnode*> translation_unit
%nterm <abstract_astnode*> program
%nterm <abstract_astnode*> struct_specifier
%nterm <abstract_astnode*> function_definition
%nterm <abstract_astnode*> main_definition
%nterm <exp_astnode*> expression
%nterm <statement_astnode*> statement
%nterm <assignS_astnode*> assignment_statement



%nterm <abstract_astnode*> compound_statement
%nterm <seq_astnode*> statement_list
%nterm <assignE_astnode*> assignment_expression
%nterm <exp_astnode*> logical_and_expression
%nterm <exp_astnode*> equality_expression
%nterm <exp_astnode*> relational_expression
%nterm <exp_astnode*> additive_expression
%nterm <exp_astnode*> multiplicative_expression
%nterm <exp_astnode*> unary_expression
%nterm <exp_astnode*> postfix_expression
%nterm <exp_astnode*> primary_expression
%nterm <op_unary_astnode*> unary_operator
%nterm <expression_list_class*> expression_list
%nterm <proccall_astnode*> procedure_call
%nterm <printf_astnode*> printf_call
%nterm <statement_astnode*> selection_statement
%nterm <statement_astnode*> iteration_statement


%%
program:
     main_definition{
          $$=$1;
          ast.insert({"main",$1});
          
          cout<<".ident  \"GCC:    (Ubuntu 8.1.0-9ubuntu1-16.04.york1) 8.1.0\""<<endl;
          cout<<".section     .note.GNU-stack,\"\",@progbits"<<endl;
           is_leave=0;
     }
     |
     translation_unit main_definition{
          $$=$2;
          ast.insert({"main",$2});
          is_leave=0;
          cout<<".ident  \"GCC:    (Ubuntu 8.1.0-9ubuntu1-16.04.york1) 8.1.0\""<<endl;
          cout<<".section     .note.GNU-stack,\"\",@progbits"<<endl;
          
         
     }
     ;

translation_unit: 
       struct_specifier{
           
       }

       | translation_unit struct_specifier
       {
          
       }
       | translation_unit function_definition
       {
            
            $$=$2;
           ast.insert({a,$2});
          is_leave=0;
          
          
           
       }
       | function_definition
       {
         
          


          

           
           ast.insert({a,$1});
          is_leave=0;
            

       }
       ;

struct_specifier: 

       STRUCT IDENTIFIER {
            temp_struct = "struct "+$2;
            if(gst.Entries.find("struct "+$2)!=gst.Entries.end()) error(@$,"already declared struct "+$2);
            
            
        }'{' declaration_list '}' ';'
       { 
	     decl = $5;
          // cout<<"inseerting "<<$2<<endl;
          string tu = "struct "+$2;
          struct_list.insert({tu,$5});
          // cout<<struct_list[tu]->names.size()<<"asdfsd"<<endl;
          struct_err.clear();
	     gst.structinsert("struct",$2,$5->size,"-",decl);
          decl=nullptr;
          change =1;
          decl_var.clear();
          temp_struct="";
       }
       ;

function_definition:

       type_specifier{if(decl_entry.size()) fn_decl_list.push_back(decl_entry) ;decl_entry.clear();} fun_declarator compound_statement
       {      
          $$ = $4;
         
          gst.funinsert($3,$1->type,decl);

          decl = nullptr;
         
          decl_var.clear();
          cur_pars.clear();
            
            f=0;
            change = 1;
          if(is_leave)cout<<"leave\nret\n";
          else cout<<"leave\nret\n";
          is_leave =  0;
          cout<<".size  "<<a<<", .-"<<a<<endl;
          if(fn_decl_list.size()){
          decl_entry = fn_decl_list.back();
           
          fn_decl_list.pop_back();}
             
            
       }
       ;
main_definition:
     INT MAIN '(' ')'{fntype="int";f=1;change=0;a="main";if(decl_entry.size()) fn_decl_list.push_back(decl_entry) ;decl_entry.clear();} compound_statement{
          $$=$6;
          // fntype="int";
          f=0;
          change =1;
          gst.maininsert("int",decl);
            decl = nullptr;
             decl_var.clear();
          cur_pars.clear();
          cout<<"leave\nret\n";
          cout<<".size  "<<"main"<<", .-"<<"main"<<endl;
          is_leave = 0;
          if(fn_decl_list.size()){
           decl_entry = fn_decl_list.back();
          fn_decl_list.pop_back();}
          
     };

type_specifier:

       VOID
       {
          $$ = new type_specifier_class("void");
           if(change) fntype = "void";
           change =0;
       }
       | INT
       {
            $$ = new type_specifier_class("int");
            if(change){fntype = "int"; }
                       change =0;

            
            
       }
    
       | STRUCT IDENTIFIER
       {
           $$ = new type_specifier_class($1,$2);
           if(change){
                fntype = "struct "+$2;
                 if(gst.Entries.find(fntype)==gst.Entries.end()) error(@$,"\""+ fntype + "\" not declared");
         
                }
                      change =0;

          

       }
       ;

fun_declarator:
          IDENTIFIER '(' parameter_list ')'
          {
          //     param = $3;
          if(gst.Entries.find($1)!=gst.Entries.end()) error(@$,"the function " + $1 + " has a previous definition");
             $$ = new fun_declarator_class($1,$3);
             a = $1;
               f=1;
           vector<declarator_class*> pars= $3->pars;
           int count = 0;
           int size_entry = 0;
          for(int i=0; i<(pars).size();i++){
              declarator_class* decr = pars[i];
               int size_ent=decr->num*4;
                    if(decr->type.substr(0,6)=="struct"){
                         //   cout << entry.name << " " << struct_names[decr->type]<<"    "<<type << endl;
                         int ok = decr->type.find("*");
                         int o  = decr->type.find("[");
                         if(ok!=std::string::npos)
                              size_ent  = 4*decr->num;
                         else if(o!=std::string::npos)
                              size_ent  = struct_names[decr->type.substr(0,o)]*decr->num;
                         else 
                              size_ent = struct_names[decr->type]*decr->num;
                    }
              
                    string lo=to_string(count+8)+"(%ebp)";
                    count+=size_ent;
                    decl_entry.insert({decr->name,lo});
               
          }
             change = 0;
             
          }
          
          | IDENTIFIER '(' ')'
          {
               // param=nullptr;
               if(gst.Entries.find($1)!=gst.Entries.end()) error(@$,"the function " + $1 + " has a previous definition");

               $$ = new fun_declarator_class($1);
               a = $1;
              f=1;
               change = 0;
               
          }
          ;

parameter_list:
          parameter_declaration
          {
             $$ = new parameter_list_class($1);
             decl_var.insert({$1->pars[0]->name,$1->pars[0]->type});
             Entries_list entry1 ;
             entry1.name = $1->pars[0]->name;
             entry1.type = $1->pars[0]->type;
             cur_pars.push_back(entry1);
             string s_b = $1->pars[0]->type;
             s_b= s_b.substr(0,s_b.find('['));
             s_b= s_b.substr(0,s_b.find('*'));
             s_b= s_b.substr(0,s_b.find('('));
             if(gst.Entries.find(s_b)==gst.Entries.end() && $1->pars[0]->type.substr(0,6)=="struct") 
                   error(@$,"there exists no such "+s_b);
             
             
          }
          | parameter_list ',' parameter_declaration

          {
               
                $1->insert($3);
               $$ = $1;    
               if(decl_var.find($3->pars[0]->name)!=decl_var.end()) {
                                        // error(@$,"Incompatible assignment when assigning to type \""+$1->type+"\" from type \""+$3->type+"\"");
                    error(@$,"already declared variable--"+$3->pars[0]->name);

               }
               decl_var.insert({$3->pars[0]->name,$3->pars[0]->type});
                Entries_list entry2 ;
             entry2.name = $3->pars[0]->name;
             entry2.type = $3->pars[0]->type;
             cur_pars.push_back(entry2);
          //    if(gst.Entries.find($3->pars[0]->type)==gst.Entries.end() && $3->pars[0]->type.substr(0,6)=="struct") 
               string s_b = $3->pars[0]->type;
             s_b= s_b.substr(0,s_b.find('['));
             s_b= s_b.substr(0,s_b.find('*'));
             s_b= s_b.substr(0,s_b.find('('));
             if(gst.Entries.find(s_b)==gst.Entries.end() && $3->pars[0]->type.substr(0,6)=="struct") 
                   error(@$,"there exists no such "+s_b);
      
          }
          ;
parameter_declaration:
          type_specifier declarator

          {
               // int param
               if($1->type =="void"){
                    if($2->type[0]!='*'){
                         error(@$,"cant be of type void");

                    }
               }
               $$ = new parameter_declaration_class($1,$2);
          }
          ;
declarator_arr:
          IDENTIFIER
          {
              $$ = new declarator_class($1);

          }

          | declarator_arr '[' INT_CONSTANT ']'
          {
               $1->insert($3);
               $$ = $1;
               
               
          }
          ;

declarator:
          declarator_arr
          {
            $$ = $1;            

          }

          | '*' declarator
          {
             $$ = new declarator_class($2); 

          }
          ;

compound_statement:
          '{' '}'
          {
              
               $$= new seq_astnode();
          }

          | '{' statement_list '}'
          {
              $$ = $2;
          //     if(print_there) cout << ".section   .rodata\n";
              print_there=0;
               cout << ".globl "<< a << endl;
                    cout << ".type "<< a << ", @function\n";
                    cout << a <<":" <<endl;
                    cout << "pushl %ebp\nmovl %esp,%ebp\npushl %ebx\npushl %esi\npushl %edi\n";
                    
                         fun_size=0;

                    
                    

                    $2->print(0);

              
          }
          | '{' declaration_list '}'
          {
               $$ = new seq_astnode();
                decl = $2;
               

                
               int size=0;
               int k=0;
               for(int i=0;i<(decl->names).size();i++){
                    declarator_class* decr = (decl->names)[i];
                    int size_ent=decr->num*4;
                         
                    if(decr->type.substr(0,6)=="struct"){
                         //   cout << entry.name << " " << struct_names[decr->type]<<"    "<<type << endl;
                         int ok = decr->type.find("*");
                         int o  = decr->type.find("[");
                        
                         if(ok!=std::string::npos)
                              size_ent  = 4*decr->num;
                         else if(o!=std::string::npos)
                              size_ent  = struct_names[decr->type.substr(0,o)]*decr->num;
                         else {
                              size_ent = struct_names[decr->type]*decr->num;
                              // cout<<decr->type<<"in del"<<size_ent<<endl;
                              }
                    }
               if(k==0){
                    string lo=to_string(-size_ent-12)+"(%ebp)";
                    k=-size_ent;
                    // decl_entry.push_back(make_pair(decr->name,lo));
                    decl_entry.insert({decr->name,lo});
               }
               else{
                    string lo=to_string(k-size_ent-12)+"(%ebp)";
                    k=k-size_ent;
                    // decl_entry.push_back(make_pair(decr->name,lo));
                    decl_entry.insert({decr->name,lo});

               }
               size+=size_ent;
                                                  

               }
               // cout << "for----" << a << "   "<< size << endl;
               fun_size=size;
                     cout << ".globl "<< a << endl;
                    cout << ".type "<< a << ", @function\n";
                    cout << a <<":" <<endl;
                    cout << "pushl %ebp\nmovl %esp,%ebp\npushl %ebx\npushl %esi\npushl %edi\n";
                    cout << "subl  $"<<fun_size <<", %esp\n";
                         fun_size=0;
               print_there=0;

              
          }
          | '{' declaration_list 
          {
               decl=$2;
               int size=0;
               int k=-16;
               for(int i=0;i<(decl->names).size();i++){
                    declarator_class* decr = (decl->names)[i];
                    int size_ent=decr->num*4;
                    if(decr->type.substr(0,6)=="struct"){
                         //  cout<<decr->type<<"in here2"<<decr->num<<endl;
                         //   cout << entry.name << " " << struct_names[decr->type]<<"    "<<type << endl;
                         int ok = decr->type.find("*");
                         int o  = decr->type.find("[");
                         if(ok!=std::string::npos)
                              size_ent  = 4*decr->num;
                         else if(o!=std::string::npos)
                              size_ent  = struct_names[decr->type.substr(0,o)]*decr->num;
                         else 
                              {size_ent = struct_names[decr->type]*decr->num;
                              // cout<<struct_names[decr->type]<<"tyoe"<<endl;}
                              }
                    }
                    string lo;
                    if(decr->type.find('[')!=std::string::npos)
                     lo=to_string(k-size_ent+4)+"(%ebp)";
                    else 
                      lo=to_string(k)+"(%ebp)";
                    k=k-size_ent;
                    // decl_entry.push_back(make_pair(decr->name,lo));
                    decl_entry.insert({decr->name,lo});
                    
               
               size+=size_ent;
                                                  

               }
               // cout << "for----" << a << "   "<< size << endl;
               fun_size=size;
          }
          statement_list '}'
          {
               // if(print_there) cout << ".section   .rodata\n";
              print_there=0;
                    cout << ".globl "<< a << endl;
                    cout << ".type "<< a << ", @function\n";
                    cout << a <<":" <<endl;
                    cout << "pushl %ebp\nmovl %esp,%ebp\npushl %ebx\npushl %esi\npushl %edi\n";
                    cout << "subl  $"<<fun_size <<", %esp\n";
                         fun_size=0;

                    $4->print(0);

              $$ = $4;
              decl = $2;
         
             
          }
          ;

statement_list:
          statement
          {
               $$ = new seq_astnode($1);
          }

          | statement_list statement
          {
               $1->insert($2);
               $$=$1;
          }
          ;

statement:
          ';'
          {
              $$ = new empty_astnode();
          }

          | '{' statement_list '}'
          {
              $$=$2;
          }

          | selection_statement
          {
              $$=$1;
          }

          | iteration_statement
          {
              $$=$1;
          }

          | assignment_statement
          {
               $$ = $1;
          }

          | procedure_call
          {
               $$=$1;
          is_leave = 1;
               
          }
          | printf_call
          {
               $$=$1;
               is_leave=1;
          }

          | RETURN expression ';'
          {
               // cout << fntype << "----\n";
               // if(fntype.compare($2->type)==0){
               $$ = new return_astnode($2);
               
               // }
               
               // else if(fntype.compare("float")==0 && ($2->type).compare("int")==0){
               //      $2 = new op_unary_astnode("TO_FLOAT",$2);
               //      $$  = new return_astnode($2);
                    
               // }
               // else if(fntype.compare("int")==0 && ($2->type).compare("float")==0){
               //      $2 = new op_unary_astnode("TO_INT",$2);
               //      $$  = new return_astnode($2);
                    
               // }
               // else{
                    
               //      // error(@$," Incompatible type \""+$2->type+"\" returned, expected \""+fntype+"\"");
               // }

          }
          ;

assignment_expression:
          unary_expression '='{is_una=0;} expression
          {
               
               
               is_una=1;
               if($1->lval==0) {
                    error(@$,"left operand of assignment operation cannot be rval") ;
               }
          
              if(($1->type).compare("int")==0 && ($4->type).compare("float")==0){
                    $4 = new op_unary_astnode("TO_INT",$4);
                    $$ = new assignE_astnode($1,$4);
                    $$->type = $1->type;
               }
               else if(($1->type).compare("float")==0 && ($4->type).compare("int")==0){
                    $4 = new op_unary_astnode("TO_FLOAT",$4);
                    $$ = new assignE_astnode($1,$4);
                    $$->type = $1->type;
               }
               
              
               else { $$ = new assignE_astnode($1,$4);
                     $$->type=$1->type;}

          }
          ;

assignment_statement:
          assignment_expression ';'
          {
               
               $$ = new assignS_astnode($1->left,$1->right);
          }
          ;

procedure_call:
          IDENTIFIER '(' ')' ';'
          {
          //   identifier_astnode* temp = new identifier_astnode($1);  
               identifier_astnode*  temp = new identifier_astnode($1); 
              $$= new proccall_astnode(temp);
              
              if($1=="printf"|| $1=="scanf"){$$->type = "void";}
          //     else if($1=="mod"){$$->type = "int";}
              
              else{
                   
                    vector<Entries_list> tem ;
                    
                    if(a==$1){
                         
                         $$->type=fntype;
                         tem = cur_pars;
                        
                         
                    }
                    else if(gst.Entries.find($1)==gst.Entries.end())error(@$,"There exists no such function");
                    else {
                         $$->type = (gst.Entries[$1]).type;
                         tem = (gst.Entries[$1]).symbtab->pars;

                    }

                    if(tem.size()>0) error(@$,"Few  arguments provided");
                    
               }
           
           
          }
          | IDENTIFIER '(' expression_list ')' ';'
          {
              
               identifier_astnode* temp = new identifier_astnode($1);
               
               vector<exp_astnode *>* s = $3->exps;
               string ty;
               if($1=="printf"|| $1== "scanf"){ty = "void";}
          //     else if($1=="mod"){ty = "int";}
              else{
                    vector<Entries_list> tem;
                    if(a==$1){
                         tem = cur_pars; 
                         ty=fntype;
                    }
                    else if(gst.Entries.find($1)==gst.Entries.end())error(@$,"There exists no such function");
                    else{
                         tem= (gst.Entries[$1]).symbtab->pars;
                         ty=(gst.Entries[$1]).type;
                         reverse(tem.begin(),tem.end());
                    }
              
              
               // cout << tem.size()  << "  "<< s->size()<< "---------size\n";
               if(s->size()>tem.size()) error(@$,"Too many arguments provided");
               if(tem.size()>s->size()) error(@$,"Few  arguments provided");
               for(int i=0; i<tem.size();i++){                   
                    if(tem[i].type =="int" && (*s)[i]->type=="float"){
                    (*s)[i] = new op_unary_astnode("TO_INT",(*s)[i]);}
                    else if(tem[i].type =="float" && (*s)[i]->type=="int"){
                    (*s)[i] = new op_unary_astnode("TO_FLOAT",(*s)[i]);
                    }
                    else if(tem[i].type =="float" && (*s)[i]->type=="float"){}
                    else if (tem[i].type =="int" && (*s)[i]->type=="int"){}
                    else{
                         // (!tem[i].value) tem[i].type="void*";
                         if(!(*s)[i]->value) (*s)[i]->type="void*";
                         // (!(*s)[i]->value) (*s)[i].type="void*";
                    if(tem[i].type=="void*"){
                         if(count((*s)[i]->type.begin(),(*s)[i]->type.end(),'*')==0 && count((*s)[i]->type.begin(),(*s)[i]->type.end(),'[')==0){
                            error(@$,"Expected \""+tem[i].type+"\" but argument is of type \""+(*s)[i]->type+"\"") ;  
                         }
                         // else
                    }
                    else if((*s)[i]->type=="void*"){
                         if(count(tem[i].type.begin(),tem[i].type.end(),'*')==0 && count(tem[i].type.begin(),tem[i].type.end(),'[')==0){
                            error(@$,"Expected \""+tem[i].type+"\" but the argument of type \""+(*s)[i]->type+"\"") ;  
                         }
                    }

                    // cout << "ok im there\n";
                    else if(!compare_type(tem[i].type,(*s)[i]->type)){
                         error(@$,"Expected \""+tem[i].type+"\" but the argument of type \""+(*s)[i]->type+"\"") ;
                    }
                    // ty=tem[i].type;
               }
               }

              }
              $$ = new proccall_astnode(temp,$3->exps);
                $$->type = ty;


          }
          ;

expression:
          logical_and_expression
          {
              $$ = $1;
              $$->base_name=$1->base_name;
              $$->basen=$1->basen;
              $$->type = $1->type;
              $$->value=$1->value;
              $$->lval = $1->lval;
     $$->is_array = $1->is_array;
             
              

          }

          | expression OR_OP logical_and_expression
          {
               $$ = new op_binary_astnode("OR_OP",$1,$3);
               
              
              
               $$->type = "int";
               $$->lval = 0;
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0 && count($1->type.begin(),$1->type.end(),'[')==0){
                    error(@$,"Invalid operand of ||, not scalar or pointer");
               }
               else if($1->type=="string") error(@$,"Invalid operand of ||, not scalar or pointer");
               
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0 && count($3->type.begin(),$3->type.end(),'[')==0){
                    error(@$,"Invalid operand of ||, not scalar or pointer");
               }
               else if($3->type=="string") error(@$,"Invalid operand of ||, not scalar or pointer");

          }
          ;

logical_and_expression:
          equality_expression
          {
              $$ = $1;
              $$->base_name=$1->base_name;
              $$->basen=$1->basen;
              $$->type = $1->type;
              $$->value=$1->value;
              $$->lval = $1->lval;
              $$->is_array = $1->is_array;
          }

          | logical_and_expression AND_OP equality_expression
          {

                $$ = new op_binary_astnode("AND_OP",$1,$3);
              
            
               $$->type = "int";
               $$->lval = 0;
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0 && count($1->type.begin(),$1->type.end(),'[')==0){
                    error(@$,"Invalid operand of &&, not scalar or pointer");
               }
               else if($1->type=="string") error(@$,"Invalid operand of &&, not scalar or pointer");
                if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0 && count($3->type.begin(),$3->type.end(),'[')==0){
                    error(@$,"Invalid operand of &&, not scalar or pointer");
               }
               else if($3->type=="string") error(@$,"Invalid operand of &&, not scalar or pointer");
          }
          ;

equality_expression:
          relational_expression
          {
              $$ = $1;
              $$->base_name=$1->base_name;
              $$->basen=$1->basen;
              $$->type = $1->type;
              $$->value=$1->value;
              $$->lval = $1->lval;
              $$->is_array = $1->is_array;
          }

          | equality_expression EQ_OP relational_expression
          {      
                
              if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("EQ_OP_INT",$1,$3);
               
               }
              
               else{
               if(!$1->value) $1->type="void*";
               if(!$3->value) $3->type="void*";
               if($1->type=="void*" || $3->type=="void*")  $$ = new op_binary_astnode("EQ_OP_INT",$1,$3);
               else if (compare_type($1->type,$3->type)){
                    // if($1->type==$3->type)
                    $$ = new op_binary_astnode("EQ_OP_INT",$1,$3);
               }
       
               else{
                    if(count($3->type.begin(),$3->type.end(),'[')> 0 && !$1->value) {$1->type="int";}
                    if(count($1->type.begin(),$1->type.end(),'[')> 0 && !$3->value) {$3->type="int";}
                     error(@$,"Invalid operands types for binary ==, "+ $1->type + " and "+$3->type);
               }
               }
              
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
                    error(@$,"incompatible types for == operator - " + $1->type +" and "+$3->type );
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"incompatible types for == operator - " + $1->type +" and "+$3->type );
               $$->type= "int";
               $$->lval = 0;
               string t = "je     .LT";
               vector<string>tt;
               tt.push_back(t);
               t = "jne  .LF";
               tt.push_back(t);
          }

          | equality_expression NE_OP relational_expression
          {
                     
             if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("NE_OP_INT",$1,$3);
               
               }
               
               else{
               if(!$1->value) $1->type="void*";
               if(!$3->value) $3->type="void*";
               if($1->type=="void*" || $3->type=="void*")  $$ = new op_binary_astnode("NE_OP_INT",$1,$3);
               else if (compare_type($1->type,$3->type)){
                    // if($1->type==$3->type)
                         $$ = new op_binary_astnode("NE_OP_INT",$1,$3);
               }
       
                else{
                    if(count($3->type.begin(),$3->type.end(),'[')> 0 && !$1->value) {$1->type="int";}
                    if(count($1->type.begin(),$1->type.end(),'[')> 0 && !$3->value) {$3->type="int";}
                     error(@$,"Invalid operands types for binary !=, "+ $1->type + " and "+$3->type);
               }
               }
     
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
                    error(@$,"incompatible types for != operator - " + $1->type +" and "+$3->type );
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"incompatible types for != operator - " + $1->type +" and "+$3->type );
               $$->type= "int";
               $$->lval = 0;
               string t = "jne     .LT";
               vector<string>tt;
               tt.push_back(t);
               t = "je  .LF";
               tt.push_back(t);
                   
              
         
          }
          ;

relational_expression:
          additive_expression
          {
              $$ = $1;
              $$->base_name=$1->base_name;
              $$->basen=$1->basen;
              $$->type = $1->type;
              $$->value=$1->value;
              $$->lval = $1->lval;
              $$->is_array = $1->is_array;
          }

          | relational_expression '<' additive_expression
          {
               // cout<<$0->jump<<endl;
               
              if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("LT_OP_INT",$1,$3);
               
               }
               else if($1->type =="float" && $3->type == "float"){
                    $$ = new op_binary_astnode("LT_OP_FLOAT",$1,$3);}
               else if($1->type=="float" && $3->type == "int"){
                    $3 = new op_unary_astnode("TO_FLOAT",$3);
                    $$ = new op_binary_astnode("LT_OP_FLOAT",$1,$3);
                   
               }
               else if($3->type=="float" && $1->type == "int"){
                    $1 = new op_unary_astnode("TO_FLOAT",$1);
                    $$ = new op_binary_astnode("LT_OP_FLOAT",$1,$3);
               }
               else{
               if(!$1->value) $1->type="void*";
               if(!$3->value) $3->type="void*";
               // cout << "types "<< $1->type << "---" << $3->type << endl;
               if (compare_type($1->type,$3->type)){
                    // if(type1==type2)/
                         $$ = new op_binary_astnode("LT_OP_INT",$1,$3);
               }
       
                else{
                    if(count($3->type.begin(),$3->type.end(),'[')> 0 && !$1->value) {$1->type="int";}
                    if(count($1->type.begin(),$1->type.end(),'[')> 0 && !$3->value) {$3->type="int";}
                     error(@$,"Invalid operands types for binary < "+ $1->type + " and "+$3->type);
               }
               }
               if($1->type=="string" || $3->type =="string") error(@$,"incompatible types for < operator - " + $1->type +" and "+$3->type );
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
                    error(@$,"incompatible types for < operator - " + $1->type +" and "+$3->type );
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"incompatible types for < operator - " + $1->type +" and "+$3->type );
               $$->type= "int";
               $$->lval = 0;
               string t = "jl     .LT";
               vector<string>tt;
               tt.push_back(t);
               t = "jge  .LF";
               tt.push_back(t);
            
              
          }

          | relational_expression '>' additive_expression
          {
               
               if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("GT_OP_INT",$1,$3);
               
               }
               else if($1->type =="float" && $3->type == "float"){
                    $$ = new op_binary_astnode("GT_OP_FLOAT",$1,$3);}
               else if($1->type=="float" && $3->type == "int"){
                    $3 = new op_unary_astnode("TO_FLOAT",$3);
                    $$ = new op_binary_astnode("GT_OP_FLOAT",$1,$3);
                   
               }
               else if($3->type=="float" && $1->type == "int"){
                    $1 = new op_unary_astnode("TO_FLOAT",$1);
                    $$ = new op_binary_astnode("GT_OP_FLOAT",$1,$3);
               }
               else{
               if(!$1->value) $1->type="void*";
               if(!$3->value) $3->type="void*";
               if (compare_type($1->type,$3->type)){
                    // if($1->type==$3->type)
                         $$ = new op_binary_astnode("GT_OP_INT",$1,$3);
               }
       
               else{
                    if(count($3->type.begin(),$3->type.end(),'[')> 0 && !$1->value) {$1->type="int";}
                    if(count($1->type.begin(),$1->type.end(),'[')> 0 && !$3->value) {$3->type="int";}
                     error(@$,"Invalid operands types for binary >, "+ $1->type + " and "+$3->type);
               }
               }
               if($1->type=="string" || $3->type =="string") error(@$,"incompatible types for > operator - " + $1->type +" and "+$3->type );
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
                    error(@$,"incompatible types for > operator - " + $1->type +" and "+$3->type );
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"incompatible types for > operator - " + $1->type +" and "+$3->type );
               $$->type= "int";
               $$->lval = 0;
               string t = "jg     .LT";
               vector<string>tt;
               tt.push_back(t);
               t = "jle  .LF";
               tt.push_back(t);

          }

          | relational_expression LE_OP additive_expression
          {
               
              if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("LE_OP_INT",$1,$3);
               
               }
               else if($1->type =="float" && $3->type == "float"){
                    $$ = new op_binary_astnode("LE_OP_FLOAT",$1,$3);}
               else if($1->type=="float" && $3->type == "int"){
                    $3 = new op_unary_astnode("TO_FLOAT",$3);
                    $$ = new op_binary_astnode("LE_OP_FLOAT",$1,$3);
                   
               }
               else if($3->type=="float" && $1->type == "int"){
                    $1 = new op_unary_astnode("TO_FLOAT",$1);
                    $$ = new op_binary_astnode("LE_OP_FLOAT",$1,$3);
               }
               else{
               if(!$1->value) $1->type="void*";
               if(!$3->value) $3->type="void*";
               if (compare_type($1->type,$3->type)){
                    // if($1->type==$3->type)
                         $$ = new op_binary_astnode("LE_OP_INT",$1,$3);
               }
       
               else{
                    if(count($3->type.begin(),$3->type.end(),'[')> 0 && !$1->value) {$1->type="int";}
                    if(count($1->type.begin(),$1->type.end(),'[')> 0 && !$3->value) {$3->type="int";}
                     error(@$,"Invalid operands types for binary <=, "+ $1->type + " and "+$3->type);
               }
               }
               if($1->type=="string" || $3->type =="string") error(@$,"incompatible types for <= operator - " + $1->type +" and "+$3->type );
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
                    error(@$,"incompatible types for <= operator - " + $1->type +" and "+$3->type );
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"incompatible types for <= operator - " + $1->type +" and "+$3->type );
               $$->type= "int";
               $$->lval = 0;
             string t = "jle     .LT";
               vector<string>tt;
               tt.push_back(t);
               t = "jg  .LF";
               tt.push_back(t);

          }

          | relational_expression GE_OP additive_expression
          {
           
             if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("GE_OP_INT",$1,$3);
               
               }
               else if($1->type =="float" && $3->type == "float"){
                    $$ = new op_binary_astnode("GE_OP_FLOAT",$1,$3);}
               else if($1->type=="float" && $3->type == "int"){
                    $3 = new op_unary_astnode("TO_FLOAT",$3);
                    $$ = new op_binary_astnode("GE_OP_FLOAT",$1,$3);
                   
               }
               else if($3->type=="float" && $1->type == "int"){
                    $1 = new op_unary_astnode("TO_FLOAT",$1);
                    $$ = new op_binary_astnode("GE_OP_FLOAT",$1,$3);
               }
               else{
               if(!$1->value) $1->type="void*";
               if(!$3->value) $3->type="void*";
               if (compare_type($1->type,$3->type)){
                    // if($1->type==$3->type)
                         $$ = new op_binary_astnode("GE_OP_INT",$1,$3);
               }
       
                else{
                    if(count($3->type.begin(),$3->type.end(),'[')> 0 && !$1->value) {$1->type="int";}
                    if(count($1->type.begin(),$1->type.end(),'[')> 0 && !$3->value) {$3->type="int";}
                     error(@$,"Invalid operands types for binary >=, "+ $1->type + " and "+$3->type);
               }
               }
               if($1->type=="string" || $3->type =="string") error(@$,"incompatible types for >= operator - " + $1->type +" and "+$3->type );
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
                    error(@$,"incompatible types for >= operator - " + $1->type +" and "+$3->type );
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"incompatible types for >= operator - " + $1->type +" and "+$3->type );
               $$->type= "int";
               $$->lval = 0;
               string t = "jg     .LT";
               vector<string>tt;
               tt.push_back(t);
               t = "jle  .LF";
               tt.push_back(t);

          }
          ;

additive_expression:
          multiplicative_expression
          {
               $$ = $1;
               $$->base_name=$1->base_name;
               $$->basen=$1->basen;
               $$->type = $1->type;
               $$->value=$1->value;
               $$->lval = $1->lval;
               $$->is_star = $1->is_star;
               $$->is_array = $1->is_array;
               // cout << $$->type << "--"<< $1->type << "type in additive_expression\n";
          }

          | additive_expression '+' multiplicative_expression
          {
               
               $$ = new op_binary_astnode("PLUS_INT",$1,$3);
                              $$->type = $3->type;
               // if($1->type =="int" && $3->type == "int"){
               //      $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //      $$->type = $1->type;
               //      }
               // else if($1->type =="float" && $3->type == "float"){
               //      $$ = new op_binary_astnode("PLUS_FLOAT",$1,$3);$$->type = $1->type;}
               // else if($1->type=="float" && $3->type == "int"){
               //      $3 = new op_unary_astnode("TO_FLOAT",$3);
               //      $$ = new op_binary_astnode("PLUS_FLOAT",$1,$3);
               //      $$->type= $1->type;
               // }
               // else if($3->type=="float" && $1->type == "int"){
               //      $1 = new op_unary_astnode("TO_FLOAT",$1);
               //      $$ = new op_binary_astnode("PLUS_FLOAT",$1,$3);
               //      $$->type = $3->type;
               // }
               //new_addition
               // else if($1->type=="int"){
               //      if(count($3->type.begin(),$3->type.end(),'*')>0){
               //           if(count($3->type.begin(),$3->type.end(),'(')>0){
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $3->type;
               //           }
               //           else if(count($3->type.begin(),$3->type.end(),'[')>1){
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $3->type.substr(0,($3->type).find("[")) + "(*)"+ $3->type.substr(($3->type).find("[",($3->type).find("[")+1));
               //           }
               //           else if(count($3->type.begin(),$3->type.end(),'[')==1){
               //                 $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $3->type.substr(0,($3->type).find("[")) + "*";
               //           }
               //           else{
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $3->type;
               //           }
               //      }
               //      else{
               //            if(count($3->type.begin(),$3->type.end(),'[')>1){
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $3->type.substr(0,($3->type).find("[")) + "(*)"+ $3->type.substr(($3->type).find("[",($3->type).find("[")+1));
               //           }
               //           else if(count($3->type.begin(),$3->type.end(),'[')==1){
               //                 $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $3->type.substr(0,($3->type).find("[")) + "*";
               //           }
               //      }
               // }

               // else if($3->type=="int"){
               //      if(count($1->type.begin(),$1->type.end(),'*')>0){
               //           if(count($1->type.begin(),$1->type.end(),'(')>0){
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $1->type;
               //           }
               //           else if(count($1->type.begin(),$1->type.end(),'[')>1){
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $1->type.substr(0,($1->type).find("[")) + "(*)"+ $1->type.substr(($1->type).find("[",($1->type).find("[")+1));
               //           }
               //           else if(count($1->type.begin(),$1->type.end(),'[')==1){
               //                 $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $1->type.substr(0,($1->type).find("[")) + "*";
               //           }
               //           else{
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $1->type;
               //           }
               //      }
               //      else{
               //           if(count($1->type.begin(),$1->type.end(),'[')>1){
               //                $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                // int abc= ($1->type).find("[");
               //                $$->type = $1->type.substr(0,($1->type).find("[")) + "(*)"+ $1->type.substr(($1->type).find("[",($1->type).find("[")+1));
               //           }
               //           else if(count($1->type.begin(),$1->type.end(),'[')==1){
               //                 $$ = new op_binary_astnode("PLUS_INT",$1,$3);
               //                $$->type = $1->type.substr(0,($1->type).find("[")) + "*";
               //           }
               //      }
               // }
              

               // //this is to check int with pointers of suitable type like n*,n*n-array
               // else{
               //      error(@$,"incompatible types for binary operator +"+$1->type +" and "+$3->type);
               // }  
               //  if($1->type=="string" || $3->type =="string") error(@$,"incompatible types for + operator - " + $1->type +" and "+$3->type );
               // if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
               //      error(@$,"incompatible types for + operator - " + $1->type +" and "+$3->type );
               // if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
               //      error(@$,"incompatible types for + operator - " + $1->type +" and "+$3->type );
               $$->lval = 0; 
               $$->is_star = 0;
               //  cout<<"here"<<endl;
               // cout << "in addition type --" << $$->type<<endl;        

          }

          | additive_expression '-' multiplicative_expression
          {

               if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                    $$->type = $1->type;
                    }
               else if($1->type =="float" && $3->type == "float"){
                    $$ = new op_binary_astnode("MINUS_FLOAT",$1,$3);
                    $$->type = $1->type;}
               else if($1->type=="float" && $3->type =="int"){
                    $3 = new op_unary_astnode("TO_FLOAT",$3);
                    $$ = new op_binary_astnode("MINUS_FLOAT",$1,$3);
                    $$->type= $1->type;
               }
               else if($1->type=="int" && $3->type=="float"){
                    $1 = new op_unary_astnode("TO_FLOAT",$1);
                    $$ = new op_binary_astnode("MINUS_FLOAT",$1,$3);
                    $$->type = $3->type;
               }
               //new 
               else if($3->type=="int"){
                    if(count($1->type.begin(),$1->type.end(),'*')>0){
                         if(count($1->type.begin(),$1->type.end(),'(')>0){
                              $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                              $$->type = $1->type;
                         }
                         else if(count($1->type.begin(),$1->type.end(),'[')>1){
                              $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                              $$->type = $1->type.substr(0,($1->type).find("[")) + "(*)"+ $1->type.substr(($1->type).find("[",($1->type).find("[")+1));
                         }
                         else if(count($1->type.begin(),$1->type.end(),'[')==1){
                               $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                              $$->type = $1->type.substr(0,($1->type).find("[")) + "*";
                         }
                         else{
                              $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                              $$->type = $1->type;
                         }
                    }
                    else{
                         if(count($1->type.begin(),$1->type.end(),'[')>1){
                              $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                              // int abc= ($1->type).find("[");
                              $$->type = $1->type.substr(0,($1->type).find("[")) + "(*)"+ $1->type.substr(($1->type).find("[",($1->type).find("[")+1));
                         }
                         else if(count($1->type.begin(),$1->type.end(),'[')==1){
                               $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                              $$->type = $1->type.substr(0,($1->type).find("[")) + "*";
                         }
                    }
               }
               else if((count($3->type.begin(),$3->type.end(),'*')>0 ||count($3->type.begin(),$3->type.end(),'[')>0) &&(count($1->type.begin(),$1->type.end(),'*')>0 ||count($1->type.begin(),$1->type.end(),'[')>0)){
                     
                    if(!compare_type($1->type,$3->type) && $1->type!=$3->type) error(@$,"incompatible types for binary operator -"+$1->type +" and "+$3->type);
                    $$ = new op_binary_astnode("MINUS_INT",$1,$3);
                    // $$->type = $1->type;
                     $$->type = "int";
                    
               }
               else{
                    error(@$,"incompatible types for binary operator -"+$1->type +" and "+$3->type);
               }
                if($1->type=="string" || $3->type =="string") error(@$,"incompatible types for - operator - " + $1->type +" and "+$3->type );
               if($1->type.substr(0,6)=="struct" && count($1->type.begin(),$1->type.end(),'*')==0&&count($1->type.begin(),$1->type.end(),'[')==0)
                    error(@$,"incompatible types for - operator - " + $1->type +" and "+$3->type );
               if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0&&count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"incompatible types for - operator - " + $1->type +" and "+$3->type );
               $$->lval = 0;
               // cout << $$->type  << "int subtraction\n";
               $$->is_star = 0;
          }
          ;

unary_expression:
          postfix_expression
          {
               $$ = $1;
               $$->base_name=$1->base_name;
               $$->basen=$1->basen;
               $$->type = $1->type;
               $$->value=$1->value;
               $$->lval = $1->lval;
               $$->pri_type = $1->pri_type;
               $$->is_arrow = $1->is_arrow;
               $$->is_array = $1->is_array;
               // cout << $$->type<< "unary_type_postfix\n";
          }

          | unary_operator unary_expression
          {
               $1->left=$2;
               $1->change_type();
               $$=$1;
               $$->type = $1->type;
               $$->lval = $1->lval;
               std:: string so($1->st);
               if(so == "\"DEREF\""){
                    $$->is_star = 1;
                    // cout<<"yes"<<endl;
               }
               // cout << $$->type<< "unary_type\n";
               //  if($$->type=="erro") error(@$,"incorrect operand for the operator ");
               // else if($$->type=="error") error(@$,"incompatible with unary operator" +so+" for type" +$1->left->type);
          }
          ;

multiplicative_expression:
          unary_expression
          {
               
              $$ = $1;
              $$->base_name=$1->base_name;
              $$->basen=$1->basen;
               $$->type = $1->type;
               $$->value=$1->value;
               $$->lval = $1->lval;
               $$->pri_type = $1->pri_type;
               $$->is_arrow = $1->is_arrow;
               $$->is_star = $1->is_star;
               $$->is_array = $1->is_array;
               // cout << $$->type << "type in multiplicative_expression\n";
          }

          | multiplicative_expression '*' unary_expression
          {    
                $$ = new op_binary_astnode("MULT_INT",$1,$3);
                    $$->type = $1->type;
               // if($1->type =="int" && $3->type == "int"){
                   
               //      }
               // else if($1->type =="float" && $3->type == "float"){
               //      $$ = new op_binary_astnode("MULT_FLOAT",$1,$3);$$->type = $1->type;}
               // else if($1->type=="float" && $3->type == "int"){
               //      $3 = new op_unary_astnode("TO_FLOAT",$3);
               //      $$ = new op_binary_astnode("MULT_FLOAT",$1,$3);
               //      $$->type= $1->type;
               // }
               // else ($3->type=="float" && $1->type == "int"){
               //      // cout << "in multiplicative with float and int\n";
               //      $1 = new op_unary_astnode("TO_FLOAT",$1);
               //      $$ = new op_binary_astnode("MULT_FLOAT",$1,$3);
               //      $$->type = $3->type;
               //      // cout<<$$->type<<"in loop"<<endl;
               // }
               // else{
               //      error(@$,"incompatible with unary operator * -"+ $1->type + " and "+$3->type);
               // }
                $$->lval=0;
                $$->pri_type=0;
                $$->is_arrow =0;
                $$->is_star = 0;
                
          }

          | multiplicative_expression '/' unary_expression
          {
               

               if($1->type =="int" && $3->type == "int"){
                    $$ = new op_binary_astnode("DIV_INT",$1,$3);
                    $$->type = $1->type;
                    }
               else if($1->type =="float" && $3->type == "float"){
                    $$ = new op_binary_astnode("DIV_FLOAT",$1,$3);$$->type = $1->type;}
               else if($1->type=="float" && $3->type == "int"){
                    $3 = new op_unary_astnode("TO_FLOAT",$3);
                    $$ = new op_binary_astnode("DIV_FLOAT",$1,$3);
                    $$->type= $1->type;
               }
               else if($3->type=="float" && $1->type == "int"){
                    // cout << "in multiplicative with float and int\n";
                    $1 = new op_unary_astnode("TO_FLOAT",$1);
                    $$ = new op_binary_astnode("DIV_FLOAT",$1,$3);
                    $$->type = $3->type;
                    // cout<<$$->type<<"in loop"<<endl;
               }
               else{
                    error(@$,"incompatible with unary operator / -"+ $1->type + " and "+$3->type);
               }
                $$->lval=0;
                $$->is_arrow =0;
                $$->is_star = 0;
          }
          ;

postfix_expression:
          primary_expression
          {
                 $$ = $1;
                 $$->base_name=$1->base_name;$$->basen=$1->basen;
                 $$->type = $1->type;
                 $$->value=$1->value;
                  $$->lval = $1->lval;
                  $$->is_arrow = $1->is_arrow;
                  $$->is_star = $1->is_star;
               //    $$->pri_type=$1->pri_type;
               
               //   cout << $$->type<< "primary_type\n";
          }

          | postfix_expression '[' expression ']'
          {
               // cout<<"prestn here"<<endl;
               $$= new arrayref_astnode($1,$3);
               string ty = $1->type;
               string in = $3->type;
               
               
              

               if($1->type.find('(')!=string::npos){
                    int fir = $1->type.find('(');
               int las = $1->type.find(')');
               // cout << fir << "  "<< las << endl;
               // $$->type = "";
               $$->type = $1->type.substr(0,fir)+$1->type.substr(las+1,$1->type.size()-las);
               }
               else if($1->type.find('[')!=string::npos){
                    int fir = $1->type.find('[');
               int las = $1->type.find(']');
               // cout << fir << "  "<< las << endl;
               // $$->type = "";
               $$->type = $1->type.substr(0,fir)+$1->type.substr(las+1,$1->type.size()-las);
               }
               else{
                    // $$->type=$$->type.substr(0,$1->type.find('*'));
                    // cout << "before * " << 
                    $$->type=$1->type.substr(0,$1->type.length()-1);
               }
             
                 $$->lval = 1;
                 $$->is_array = 1;
               
          }

          | IDENTIFIER '(' ')'
          {
               is_leave=1;
            identifier_astnode*  temp = new identifier_astnode($1); 
              $$= new funcall_astnode(temp);
              $$->lval = 0;
              if($1=="printf"||$1== "scanf"){$$->type = "void";}
          //     else if($1=="mod"){$$->type = "int";}
              else{
                    vector<Entries_list> tem ;
                   if(a==$1){
                         $$->type=fntype;
                       tem = cur_pars;
                   }
                    else if(gst.Entries.find($1)==gst.Entries.end())error(@$,"There exists no such function");
                    else {
                         $$->type = (gst.Entries[$1]).type;
                         //     cout << $1 << "   name   "<< $$->type << endl;
                         tem = (gst.Entries[$1]).symbtab->pars;
                    }
                    if(tem.size()>0) error(@$,"Few  arguments provided");
               }
          
               // $$->type = (gst.Entries[$1]).type;
               // cout <<  (gst.Entries[$1]).type << "   name   "<< $$->type << endl;

              
          }

          | IDENTIFIER '(' expression_list ')'
          {
           is_leave=1;

              identifier_astnode* temp = new identifier_astnode($1);
               
               vector<exp_astnode *>* s = $3->exps;
               string ty;
               if($1=="printf"|| $1== "scanf"){ty = "void";}
          //     else if($1=="mod"){ty = "int";}
              else{
                    vector<Entries_list> tem;
                    if(a==$1){
                         tem = cur_pars; 
                              ty=fntype;
                    }
                   
               
               
               // cout << tem.size()  << "  "<< s->size()<< "---------size\n";
              

              }
              $$ = new funcall_astnode(temp,$3->exps);
                $$->type = ty;
                 $$->lval = 0;
               
          }

          | postfix_expression '.' IDENTIFIER
          {
              string yi;
              string new_type = "";
              string new_basen = "";
              int lo;
               if(struct_list[$1->type]){
                    
                         
                   string v = decl_entry[$1->basen];
                   
                   if(v.size()!=0){
                   v = v.substr(0,v.find('('));
                    lo = stoi(v);
                    }
                    else {

                         v = $1->base_name;
                        v = v.substr(0,v.find('('));
                        lo = stoi(v);
                   }
              
                   declaration_list_class* decl1 = struct_list[$1->type];
             
                   for(int i=(decl1->names).size()-1; i>=0;i--){
                         declarator_class* decr = (decl1->names)[i];
                         int size_ent=decr->num*4;
                         if(decr->type.substr(0,6)=="struct"){
                              //   cout << entry.name << " " << struct_names[decr->type]<<"    "<<type << endl;
                              int ok = decr->type.find("*");
                              int o  = decr->type.find("[");
                              if(ok!=std::string::npos)
                                   size_ent  = 4*decr->num;
                              else if(o!=std::string::npos)
                                   size_ent  = struct_names[decr->type.substr(0,o)]*decr->num;
                              else 
                                   size_ent = struct_names[decr->type]*decr->num;
                              
                              
                         }
                        
                          string lot=to_string(lo)+"(%ebp)";
                    // cout<<decr->name <<"nameand loca"<<lo<<size_ent<<endl;
                   
                    // // cout<<lo<<endl;

                    // cout<<decr->name <<"nameand loca"<<lo<<endl;
                   if(decr->name==$3) {yi = to_string(lo)+"(%ebp)"; new_type = decr->type;
                    // cout<<decr->type<<"in else"<<decr->name<<" "<<decr->num<< " "<<lo<<endl;
                              new_basen = $3;}
                    lo=lo-size_ent;
                    // decl_entry.insert({yu,yi});
                   
                    // cout<<$1->basen<<decr->name;

                         
                   }
              }
          //     
              identifier_astnode* temp = new identifier_astnode(yi);

               // if(gst.Entries.find($1->type)==gst.Entries.end())
          
               $$ = new member_astnode($1,temp);
               // cout<<"jjjjsdfsdfj"<<endl;
               // cout << $1->type<<"---------"<< endl; 
               
               // if($1->type.substr(0,6)!="struct") error(@$,"It is not of the type struct");
               // if(count($1->type.begin(),$1->type.end(),'[')>0 || count($1->type.begin(),$1->type.end(),'*')>0) error(@$,"Cant access the struct like this");

               // if(gst.Entries.find($1->type)==gst.Entries.end()) error(@$,"the struct variable is not declared");
               // else{
                    // vector<Entries_list> tem = (gst.Entries[$1->type]).symbtab->vars;
                    // int i;
                    // for( i=0;i<tem.size();i++){
                    //      if(tem[i].name==$3) {$$->type=tem[i].type;break;}
                    // }
                    // if(i==tem.size()) error(@$,"there is no variable "+$3+" in the "+$1->type);
               // }
               $$->lval=1;
                $$->base_name = yi;
                $$->type = new_type;
                $$->basen = new_basen;
   
          }

          | postfix_expression PTR_OP IDENTIFIER
          {
           
             string yi;
              string new_type = "";
              string new_basen = "";
              int lo;
          //     cout<<"present"<<$1->type<<endl;
              $1->type = ($1->type).substr(0,($1->type).find('*'));
              $1->type = ($1->type).substr(0,($1->type).find('('));
               if(struct_list[$1->type]){
                    
                         
                   string v = decl_entry[$1->basen];
                   
                   if(v.size()!=0){
                   v = v.substr(0,v.find('('));
                    lo = stoi(v);
                    }
                    else {

                         v = $1->base_name;
                        v = v.substr(0,v.find('('));
                        lo = stoi(v);
                   }
              
                   
                        
                    
                   
                   yi = to_string(lo)+"(%ebp)"; 
                  
               //     cout<<yi<<$1->type<<"here"<<$1->base_name<<$1->basen<<endl;

                         
                    }
             
              identifier_astnode* temp = new identifier_astnode(yi);
                   int a;
                    declaration_list_class* decl1 = struct_list[$1->type];         
                   for(int i=0; i<(decl1->names).size(); i++){
                         declarator_class* decr = (decl1->names)[i];
                         // cout<<decr->name<<endl;
                            if(decr->name==$3){
                              a = 4*i; break;
                              cout<<a<<endl;
                            }
                              
                              
                    }
               $$ = new arrow_astnode($1,temp,a);
               $$->lval=1;
                $$->base_name = yi;
                $$->type = new_type;
                $$->basen = new_basen;
                $$->is_arrow = 1;
          }

          | postfix_expression INC_OP
          {
               op_unary_astnode* temp = new op_unary_astnode("PP",$1);
               temp->change_type();
               $$=temp;
                $$->lval = 0;
               $$->type= temp->type;
               std:: string so(temp->st);
               if($$->type=="erro") error(@$,"incorrect operand for the operator ");
               else if($$->type=="error") error(@$,"incompatible with unary operator" +so+" for type" +temp->left->type);
               
          }
          ;

primary_expression:
          IDENTIFIER
          {
               $$ = new identifier_astnode($1);
               // cout<<$1<<"in identifier"<<decl_entry[$1];
              $$->base_name=decl_entry[$1];
              $$->basen = $1;
               // $$->location = decl_entry[$1];
               // $$->type = gst.gettype(decl,param,$1);
               if(decl_var.find($1)==decl_var.end()){
                    error(@$,"Variable Not defined -- "+$1);
               }
               $$->type = decl_var[$1];
               $$->lval = 1;

               if(is_una){
               
               exp_name = $1;
               }

  
          }
          | INT_CONSTANT
          {
               $$ = new intconst_astnode($1);
               // $$->type = gst.constgettype("int");
               $$->type = "int";
               if($1=="0") $$->value = 0;
                $$->lval = 0;
                $$->pri_type=1+stoi($1);
               
               
          }
     

          | '(' expression ')'
          {
               $$=$2;
               $$->base_name=$2->base_name;
               $$->basen=$2->basen;
               $$->type = $2->type;
                $$->lval = $2->lval;
                $$->value = $2->value;
                $$->pri_type = $2->pri_type;
               
               // cout << $$->type<< "primary_()_type\n";
          }
          ;

expression_list:
          expression
          {
               $$ = new expression_list_class($1);
               no_times = 0;
          }

          | expression_list ','{no_times=0;} expression
          {
               $1->insert($4);
               $$=$1;
          }    
          ;

unary_operator:
          '-'
          {
              $$ = new op_unary_astnode("UMINUS"); 
          }

          | '!'
          {
               $$ = new op_unary_astnode("NOT"); 

          }

          | '&'
          {
               $$ = new op_unary_astnode("ADDRESS");
          }

          | '*'
          {
               $$ = new op_unary_astnode("DEREF");
              
          }
          ;

selection_statement: 
          IF '(' expression ')' statement ELSE statement
          {
             $$ = new if_astnode($3,$5,$7); 
          //    cout<<"completed"<<endl;
             if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0 && count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"expression should be int or float or pointer or string");
          }
          ;

iteration_statement:
          WHILE '(' expression ')' statement
          {
              $$ = new while_astnode($3,$5);
              if($3->type.substr(0,6)=="struct" && count($3->type.begin(),$3->type.end(),'*')==0 && count($3->type.begin(),$3->type.end(),'[')==0)
                    error(@$,"expression should be int or float or pointer or string");
          }

          | FOR '(' assignment_expression ';' expression ';' assignment_expression ')' statement
          {
              $$ = new for_astnode($3,$5,$7,$9);
              if($5->type.substr(0,6)=="struct" && count($5->type.begin(),$5->type.end(),'*')==0 && count($5->type.begin(),$5->type.end(),'[')==0)
                    error(@$,"expression should be int or float or pointer or string");
          }    
          ;

declaration_list:
          declaration
          {
              $$ = new declaration_list_class($1);
              decl = new declaration_list_class($1);
              
          }

          | declaration_list declaration
          {
             $1->insert($2);
             decl->insert($2);
               $$ = $1;
          }
          ;

declaration:
          type_specifier declarator_list ';'
          {
               int new1 = 0;
              $$ = new declaration_class($1,$2);
              if($1->type.substr(0,6)=="struct" && f){
               //     cout << "checking struct\n";
                    string tt=$1->type;
                    tt=tt.substr(0,tt.find('['));
                    tt=tt.substr(0,tt.find('*'));
                    tt=tt.substr(0,tt.find('('));
                   if(gst.Entries.find(tt)==gst.Entries.end()) error(@$,"there exists no such "+tt);
              }
               else if($1->type.substr(0,6)=="struct" && !f){
               //     cout << "checking struct\n";
                    string tt=$1->type;
                    tt=tt.substr(0,tt.find('['));
                    tt=tt.substr(0,tt.find('*'));
                    tt=tt.substr(0,tt.find('('));
                    if(temp_struct==tt) new1 = 1;
                   if(gst.Entries.find(tt)==gst.Entries.end()&& temp_struct!=tt) error(@$,"there exists no such "+tt);
               //     else if(temp_struct==tt){
                    //     cout<<tt<<"here"<<temp_struct<<"asd"<$1->type<<endl;
                    //     if(count(($1->type).begin(),($1->type).end(),'*')==0  && count(($1->type).begin(),($1->type).end(),'[')==0  ){
                              // error(@$,"cant declare like this "+temp_struct);
                    //     }
                   }
              
              
              
              for(int i=0;i<$2->names.size();i++){
                         
                              if(decl_var.find($2->names[i]->name)!=decl_var.end()) {
                                   // if(f)
                                        error(@$,"already declared variable--"+$2->names[i]->name);
                                   // else
                                   // string err= 
                                   // {
                                        // struct_err.insert({@$.begin.line,"already declared variable--"+$2->names[i]->name});
                                        // break;
                                        // }
                              }
                              if(new1 ){
                                   string new2 = $2->names[i]->type;
                                   if(count((new2).begin(),(new2).end(),'*')==0 && count((new2).begin(),(new2).end(),'[')==0){
                                        error(@$,"cant declare like this "+temp_struct);
                                   }
                              }
                              decl_var.insert({$2->names[i]->name,$2->names[i]->type});
                              // cout << decl_var[$2->names[i]->name] << "---"<< $2->names[i]->name<< endl;
                    if($1->type =="void"){
                         if($2->names[i]->type.substr(0,5)!="void*"){
                         // if($2->type[0]!='*'){
                              // if(f)
                                   error(@$,"cant be of type void");
                              // else{
                                   // struct_err.insert({@$.begin.line,"cant be of type void"});
                                   // break;
                                   // }

                         }
                    }

              }
              for(int i=0;i<$$->names.size();i++){
               //     $$->names[i]->locate = @$;
               
               // cout<<@$.begin.line<<endl;
               $$->names[i]->locate = @$.begin.line;
               // cout << "making locate  " << $$->names[i]->locate << endl;
              
              }
              
               
          }
          ;

declarator_list:
          declarator
          {
               $$ = new declarator_list_class($1);
          }

          | declarator_list ',' declarator
          {
             $1->insert($3);
               $$ = $1;   
          }
          ;
printf_call:
     PRINTF'(' STRING_LITERAL ')' ';'{
          // identifier_astnode*  temp = new identifier_astnode("printf"); 
          //     $$= new proccall_astnode(temp);
          // int fn_num = lc_num;
          $$ = new printf_astnode(lc_num); 
          if(print_there==0){
               cout << ".section   .rodata\n";
          }
              $$->type="void";
                    print_there=1;
                    cout << ".LC" << lc_num<<":"<< endl;
                    cout << ".string    " << $3 << endl;
                    cout << ".text\n";
                    lc_num++;

     }
     | PRINTF '(' STRING_LITERAL ',' expression_list ')' ';'{
          // vector<exp_astnode *>* s = $3->exps;
          // identifier_astnode*  temp = new identifier_astnode("printf"); 
          if(print_there==0){
               cout << ".section   .rodata\n";
          }
          //  int fn_num = lc_num;
              $$= new printf_astnode(lc_num,$5->exps);
              cout << ".LC" << lc_num<<":"<< endl;
               cout << ".string    " << $3 << endl;
               cout << ".text\n";
               lc_num++;
               print_there=1;


     };



%%
void IPL::Parser::error( const location_type &l, const std::string &err_message )
{
  
   cout << "Error at line "<<l.begin.line <<": "<< err_message << "\n";
   exit(1);
}

