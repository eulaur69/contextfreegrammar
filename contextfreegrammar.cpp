#include <bits/stdc++.h>
#include <variant>

using namespace std;

using terminal = string;
using nonterminal = string;

class Node{
    private:
        string a; //terminal or non-terminal
        variant<Node*, int> b; //pointer or integer
        variant<Node*, int> c; //pointer or integer
        variant<Node*, int> d; //pointer or integer (nullptr)
    public:
        Node(){
            a = "";
            b = 0;
            c = 0;
            d = 0;
        }

        void setA(string a){
            this->a = a;
        }
        void setB(variant<Node*, int> b){
            this->b = b;
        }
        void setC(variant<Node*, int> c){
            this->c = c;
        }
        void setD(variant<Node*, int> d){
            this->d = d;
        }

        string getA() const {
            return this->a;
        }
        variant<Node*, int> getB() const {
            return this->b;
        }

        variant<Node*, int> getC() const {
            return this->c;
        }

        variant<Node*, int> getD() const {
            return this->d;
        }
};

class ProductionRule{
    private:
        nonterminal LHS;
        Node* RHS;
        bool isLastRuleWithSameLHS;
    public:
        ProductionRule(){
            LHS = ""; //nonterminal symbol
            RHS = new Node();
            isLastRuleWithSameLHS = 1;
        }
        ProductionRule(nonterminal LHS, vector<string> RHSarr){
            this->LHS = LHS;
            this->RHS = new Node();
            Node* current = this->RHS;

            for (int i = 0; i < RHSarr.size() - 1; i++) {
                current->setA(RHSarr[i]);
                current->setD(new Node());
                current = get<Node*>(current->getD());
            }
            current->setA(RHSarr.back());
            current->setD(nullptr);
        }
        nonterminal getLHS(){
            return this->LHS;
        }
        Node* getRHS(){
            return this->RHS;
        }
        void setLHS(nonterminal LHS){
            this->LHS = LHS;
        }
        void setRHS(Node* RHS){
            this->RHS = RHS;
        }
        //todo add adding,removal functions
};

class ContextFreeGrammar{
    private:
        vector<nonterminal> nonterminals;
        vector<terminal> terminals;
        vector<ProductionRule> rules;
        nonterminal startingSymbol;
        bool isTerminal(terminal t){
            if(binary_search(terminals.begin(),terminals.end(),t)){
                return 1;
            } else return 0;
        }
        bool isNonerminal(nonterminal nt){
            if(binary_search(nonterminals.begin(),nonterminals.end(),nt)){
                return 1;
            } else return 0;
        }
    public:
        ContextFreeGrammar(){
            startingSymbol = "";
        }
        ContextFreeGrammar(vector<nonterminal> nonterminals, vector<terminal> terminals, vector<ProductionRule> rules, nonterminal startingSymbol){
            this->nonterminals = nonterminals;
            this->terminals = terminals;
            sort(rules.begin(),rules.end(), [](ProductionRule p1, ProductionRule p2){
                   return p1.getLHS() < p2.getLHS();
                });
            this->rules = rules;
            this->startingSymbol = startingSymbol;
            sort(this->nonterminals.begin(),this->nonterminals.end());
            sort(this->terminals.begin(),this->terminals.end());
            buildB();
        }
        void buildB(){
            map<nonterminal,Node*> firstLHSs;
            for(int i = 0;i<this->rules.size();i++){
                ProductionRule rule = this->rules[i];
                nonterminal LHS = rule.getLHS();
                Node* RHS = rule.getRHS();
                if(i == 0){
                    firstLHSs[LHS] = RHS;
                } else if(LHS != rules[i-1].getLHS()){
                    firstLHSs[LHS] = RHS;
                } else continue;
            }
            for(ProductionRule &rule : this->rules){
                Node* RHS = rule.getRHS();
                while(RHS != nullptr){
                    if(isTerminal(RHS->getA())){
                        RHS->setB(nullptr);
                    } else RHS->setB(firstLHSs[RHS->getA()]);
                    RHS = get<Node*>(RHS->getD());
                }
            }
        }
        void buildC(){

        }
};

int main(){
    Node* main = new Node();
    Node *a = new Node();
    Node *b = new Node();
    Node *c = new Node();
    Node *d = new Node();
    main->setA("yes");
    main->setB(b);
    main->setB(1);
    vector<nonterminal> N = {"C","D"};
    vector<terminal> T = {"0","1"};
    vector<ProductionRule> P = {
        ProductionRule("C",{"D","C"}),
        ProductionRule("C",{"C","D"}),
        ProductionRule("D",{"0"}),
        ProductionRule("C",{"C","C","D"}),
        ProductionRule("C",{"D"}),
        ProductionRule("D",{"1"})
    };
    string S = N[0];
    ContextFreeGrammar cfg = ContextFreeGrammar(N,T,P,S);
    cout << "yes";
}