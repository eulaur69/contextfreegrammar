#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <variant>

using nonterminal = std::string;
using terminal = std::string;

namespace cfg {
    class Node {
    private:
        std::string a; //terminal or non-terminal
        std::variant<Node*, int> b; //pointer or integer
        std::variant<Node*, int> c; //pointer or integer
        std::variant<Node*, int> d; //pointer or integer
    public:
        Node() {
            this->a = "";
            this->b = nullptr;
            this->c = nullptr;
            this->d = nullptr;
        }

        void setA(std::string a) {
            this->a = a;
        }
        void setB(std::variant<Node*, int> b) {
            this->b = b;
        }
        void setC(std::variant<Node*, int> c) {
            this->c = c;
        }
        void setD(std::variant<Node*, int> d) {
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

    class ProductionRule {
    private:
        nonterminal LHS;
        Node* RHS;
        Node* RHSlast;
        std::string RHSstring;
        bool isLastRuleWithSameLHS;
    public:
        ProductionRule() {
            LHS = ""; //nonterminal symbol
            RHS = new Node();
            isLastRuleWithSameLHS = 1;
            RHSstring = "";
        }
        ProductionRule(nonterminal LHS, std::vector<std::string> RHSarr) {
            this->LHS = LHS;
            this->RHS = new Node();
            this->isLastRuleWithSameLHS = 1;
            Node* current = this->RHS;
            this->RHSstring = "";
            for (int i = 0; i < RHSarr.size() - 1; i++) {
                this->RHSstring += RHSarr[i];
                current->setA(RHSarr[i]);
                current->setD(new Node());
                current = std::get<Node*>(current->getD());
            }
            current->setA(RHSarr.back());
            current->setD(nullptr);
            this->RHSlast = current;
        }
        nonterminal getLHS() {
            return this->LHS;
        }
        Node* getRHS() {
            return this->RHS;
        }
        Node* getRHSlast(){
            return this->RHSlast;
        }
        std::string getRHSstring() {
            return this->RHSstring;
        }
        void setLHS(nonterminal LHS) {
            this->LHS = LHS;
        }
        void setRHS(Node* RHS) {
            this->RHS = RHS;
        }
        bool checkisLastRuleWithSameLHS() {
            if (this->isLastRuleWithSameLHS) {
                return 1;
            }
            else return 0;
        }
        void setIsLastRuleWithSameLHS(bool status) {
            this->isLastRuleWithSameLHS = status;
        }
        //todo add adding,removal functions
    };

    class ContextFreeGrammar {
    private:
        std::vector<nonterminal> nonterminals;
        std::vector<terminal> terminals;
        std::vector<ProductionRule> rules;
        nonterminal startingSymbol;
        bool isTerminal(terminal t) {
            if (std::binary_search(terminals.begin(), terminals.end(), t)) {
                return 1;
            }
            else return 0;
        }
        bool isNonerminal(nonterminal nt) {
            if (std::binary_search(nonterminals.begin(), nonterminals.end(), nt)) {
                return 1;
            }
            else return 0;
        }
        void buildB() {
            std::map<nonterminal, Node*> firstLHSs;
            for (int i = 0;i < this->rules.size();i++) {
                ProductionRule rule = this->rules[i];
                nonterminal LHS = rule.getLHS();
                Node* RHS = rule.getRHS();
                if (i == 0) {
                    firstLHSs[LHS] = RHS;
                    rules[i].setIsLastRuleWithSameLHS(0);
                }
                else if (LHS != rules[i - 1].getLHS()) {
                    firstLHSs[LHS] = RHS;
                    rules[i - 1].setIsLastRuleWithSameLHS(1);
                }
                else {
                    rules[i].setIsLastRuleWithSameLHS(0);
                    continue;
                }
            }
            rules[rules.size() - 1].setIsLastRuleWithSameLHS(1);
            for (ProductionRule& rule : this->rules) {
                Node* RHS = rule.getRHS();
                while (RHS != nullptr) {
                    if (isTerminal(RHS->getA())) {
                        RHS->setB(nullptr);
                    }
                    else RHS->setB(firstLHSs[RHS->getA()]);
                    RHS = std::get<Node*>(RHS->getD());
                }
            }
        }
        void buildC() {
            for (int i = 0;i < this->rules.size();i++) {
                ProductionRule rule = this->rules[i];
                Node* RHS = rule.getRHS();
                if (i == this->rules.size() - 1) {
                    bool processedFirst = 0;
                    while (RHS != nullptr) {
                        if (!processedFirst) {
                            RHS->setC(nullptr);
                            processedFirst = 1;
                        }
                        else RHS->setC(-1);
                        RHS = std::get<Node*>(RHS->getD());
                    }
                }
                else {
                    ProductionRule nextRule = this->rules[i + 1];
                    bool processedFirst = 0;
                    while (RHS != nullptr) {
                        if (!processedFirst) {
                            if (rule.checkisLastRuleWithSameLHS()) {
                                RHS->setC(nullptr);
                            }
                            else RHS->setC(nextRule.getRHS());
                            processedFirst = 1;
                        }
                        else if (rule.checkisLastRuleWithSameLHS()) {
                            RHS->setC(nullptr);
                        }
                        else RHS->setC(-1);
                        RHS = std::get<Node*>(RHS->getD());
                    }
                }
            }
        }
    public:
        ContextFreeGrammar() {
            startingSymbol = "";
        }
        ContextFreeGrammar(std::vector<nonterminal> nonterminals, std::vector<terminal> terminals, std::vector<ProductionRule> rules, nonterminal startingSymbol) {
            this->nonterminals = nonterminals;
            this->terminals = terminals;
            std::sort(rules.begin(), rules.end(), [](ProductionRule p1, ProductionRule p2) {
                return p1.getLHS() < p2.getLHS();
                });
            this->rules = rules;
            this->startingSymbol = startingSymbol;
            std::sort(this->nonterminals.begin(), this->nonterminals.end());
            std::sort(this->terminals.begin(), this->terminals.end());
            buildB();
            buildC();
        }
        std::vector<nonterminal> getNonterminals() {
            return this->nonterminals;
        }
        std::vector<nonterminal> getTerminals() {
            return this->terminals;
        }
        std::vector<ProductionRule> getProductionRules() {
            return this->rules;
        }
        std::string getStartingSymbol() {
            return this->startingSymbol;
        }
        void setNonterminals(const std::vector<nonterminal>& newNonterminals) {
            this->nonterminals = newNonterminals;
        }

        void setTerminals(const std::vector<nonterminal>& newTerminals) {
            this->terminals = newTerminals;
        }

        void setProductionRules(const std::vector<ProductionRule>& newRules) {
            this->rules = newRules;
        }

        void setStartingSymbol(const std::string& newStartingSymbol) {
            this->startingSymbol = newStartingSymbol;
        }

    };
}

