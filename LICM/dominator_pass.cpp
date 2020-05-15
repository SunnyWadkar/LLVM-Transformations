// ECE 5984 S20 Assignment 3: dominator_pass.cpp
// Group: Sunny Wadkar and Ramprasath Pitta Sekar

////////////////////////////////////////////////////////////////////////////////
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/PassAnalysisSupport.h"

#include "dominance.h"

using namespace llvm;
using namespace std;

namespace {
  class DominancePass : public FunctionPass {
  public:
    static char ID;
    DominancePass() : FunctionPass(ID) { }
    virtual bool runOnFunction(Function& F) 
    {
        Dominance* dp = new Dominance();
        dp->executeDominancePass(F);
        outs() << "Dominator Pass Domain: \n";
        dp->printBasicBlockSet(&dp->basicBlocks);
        // dp->printDominanceResult(dp->dominanceRelationMap);
        dp->printDFAResults(dp->dFAResult);
      return false;
    }

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
    }
  };

  char DominancePass::ID = 0;
  RegisterPass<DominancePass> X("dominators", "ECE 5984 Dominators");
}
