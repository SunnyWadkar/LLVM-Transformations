#ifndef DOMINANCE_H
#define DOMINANCE_H

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/PassAnalysisSupport.h"

#include "dataflow.h"

using namespace std;

namespace llvm{
    class Dominance{
        public:
            Dominance(){}
            std::vector<BasicBlock*> basicBlocks;
            std::map<BasicBlock*,std::vector<BasicBlock*>> dominanceRelationMap;
            std::map<BasicBlock*,basicBlockProps*> dFAResult;
            void executeDominancePass(Function& F);
            void printDominanceResult(std::map<BasicBlock*,std::vector<BasicBlock*>> dmap);
            void printDFAResults(std::map<BasicBlock*,basicBlockProps*> dFAHash);
            void printBasicBlockSet(std::vector<BasicBlock*> *x);
        private:
            std::map<BasicBlock*,basicBlockDeps*> bbSets;
            std::map<BasicBlock*,int> domainMap;
            std::map<int,BasicBlock*> revDomainMap;
            void genTFDependencies(Function &F, std::vector<BasicBlock*> domain);
            void buildDominanceMap(std::map<BasicBlock*,std::vector<BasicBlock*>> &dmap,std::map<BasicBlock*,basicBlockProps*> dFAHash);
            std::string get_block_reference(BasicBlock *BB);
    };
}

#endif