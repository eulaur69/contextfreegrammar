#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <variant>
#include <iostream>
#include "contextfreegrammar.h"

/*using nonterminal = std::string;
using terminal = std::string;

class Node{
    private:
        std::string a; //terminal or non-terminal
        std::variant<Node*, int> b; //pointer or integer
        std::variant<Node*, int> c; //pointer or integer
        std::variant<Node*, int> d; //pointer or integer (nullptr)
    public:
        Node(){
            a = "";
            b = 0;
            c = 0;
            d = 0;
        }

        void setA(std::string a){
            this->a = a;
        }
        void setB(std::variant<Node*, int> b){
            this->b = b;
        }
        void setC(std::variant<Node*, int> c){
            this->c = c;
        }
        void setD(std::variant<Node*, int> d){
            this->d = d;
        }

        std::string getA() const {
            return this->a;
        }
        std::variant<Node*, int> getB() const {
            return this->b;
        }

        std::variant<Node*, int> getC() const {
            return this->c;
        }

        std::variant<Node*, int> getD() const {
            return this->d;
        }
};

class ProductionRule{
    private:
        nonterminal LHS;
        Node* RHS;
        Node* RHSlast;
        bool isLastRuleWithSameLHS;
    public:
        ProductionRule(){
            LHS = ""; //nonterminal symbol
            RHS = new Node();
            isLastRuleWithSameLHS = 1;
        }
        ProductionRule(nonterminal LHS, std::vector<std::string> RHSarr){
            this->LHS = LHS;
            this->RHS = new Node();
            Node* current = this->RHS;

            for (int i = 0; i < RHSarr.size() - 1; i++) {
                current->setA(RHSarr[i]);
                current->setD(new Node());
                current = std::get<Node*>(current->getD());
            }
            current->setA(RHSarr.back());
            current->setD(nullptr);
            this->RHSlast = current;
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
        bool checkisLastRuleWithSameLHS(){
            if(this->isLastRuleWithSameLHS){
                return 1;
            } else return 0;
        }
        void setIsLastRuleWithSameLHS(bool status){
            this->isLastRuleWithSameLHS = status;
        }
        //todo add adding,removal functions
};

class ContextFreeGrammar{
    private:
        std::vector<nonterminal> nonterminals;
        std::vector<terminal> terminals;
        std::vector<ProductionRule> rules;
        nonterminal startingSymbol;
        bool isTerminal(terminal t){
            if(std::binary_search(terminals.begin(),terminals.end(),t)){
                return 1;
            } else return 0;
        }
        bool isNonerminal(nonterminal nt){
            if(std::binary_search(nonterminals.begin(),nonterminals.end(),nt)){
                return 1;
            } else return 0;
        }
    public:
        ContextFreeGrammar(){
            startingSymbol = "";
        }
        ContextFreeGrammar(std::vector<nonterminal> nonterminals, std::vector<terminal> terminals, std::vector<ProductionRule> rules, nonterminal startingSymbol){
            this->nonterminals = nonterminals;
            this->terminals = terminals;
            std::sort(rules.begin(),rules.end(), [](ProductionRule p1, ProductionRule p2){
                   return p1.getLHS() < p2.getLHS();
                });
            this->rules = rules;
            this->startingSymbol = startingSymbol;
            sort(this->nonterminals.begin(),this->nonterminals.end());
            sort(this->terminals.begin(),this->terminals.end());
            buildB();
            buildC();
        }
        void buildB(){
            std::map<nonterminal,Node*> firstLHSs;
            for(int i = 0;i<this->rules.size();i++){
                ProductionRule rule = this->rules[i];
                nonterminal LHS = rule.getLHS();
                Node* RHS = rule.getRHS();
                if(i == 0){
                    firstLHSs[LHS] = RHS;
                    rules[i].setIsLastRuleWithSameLHS(0);
                } else if(LHS != rules[i-1].getLHS()){
                    firstLHSs[LHS] = RHS;
                    rules[i-1].setIsLastRuleWithSameLHS(1);
                } else {
                    rules[i].setIsLastRuleWithSameLHS(0);
                    continue;
                }
            }
            rules[rules.size()-1].setIsLastRuleWithSameLHS(1);
            for(ProductionRule &rule : this->rules){
                Node* RHS = rule.getRHS();
                while(RHS != nullptr){
                    if(isTerminal(RHS->getA())){
                        RHS->setB(nullptr);
                    } else RHS->setB(firstLHSs[RHS->getA()]);
                    RHS = std::get<Node*>(RHS->getD());
                }
            }
        }
        void buildC(){
            for(int i = 0;i<this->rules.size();i++){
                ProductionRule rule = this->rules[i];
                Node *RHS = rule.getRHS();
                if(i == this->rules.size()-1){
                    bool processedFirst = 0;
                    while(RHS != nullptr){
                        if(!processedFirst){
                            RHS->setC(nullptr);
                            processedFirst = 1;
                        } else RHS->setC(-1);
                        RHS = std::get<Node*>(RHS->getD());
                    }
                } else {
                    ProductionRule nextRule = this->rules[i+1];
                    bool processedFirst = 0;
                    while(RHS != nullptr){
                        if(!processedFirst){
                            RHS->setC(nextRule.getRHS());
                            processedFirst = 1;
                        } else if(rule.checkisLastRuleWithSameLHS()){
                            RHS->setC(nullptr);
                        } else RHS->setC(-1);
                        RHS = std::get<Node*>(RHS->getD());
                    }
                }
            }
        }
};*/

int main(){
    std::vector<nonterminal> N = {"C","D"};
    std::vector<terminal> T = {"0","1"};
    std::vector<cfg::ProductionRule> P = {
        cfg::ProductionRule("C",{"D","C"}),
        cfg::ProductionRule("C",{"C","D"}),
        cfg::ProductionRule("D",{"0"}),
        cfg::ProductionRule("C",{"C","C","D"}),
        cfg::ProductionRule("C",{"D"}),
        cfg::ProductionRule("D",{"1"})
    };
    std::string S = N[0];
    cfg::ContextFreeGrammar cfg = cfg::ContextFreeGrammar(N,T,P,S);
    cfg::Node* root = cfg.getProductionRules()[4].getRHS();
    while(root != nullptr){
        cfg::Node* current = root;
        while(current != nullptr){
            std::cout << current->getA() << " ";
            current = std::get<cfg::Node*>(current->getD());
        }
        std::cout << std::endl;
        root = std::get<cfg::Node*>(root->getC());
    }
    std::cout << "yes";
}