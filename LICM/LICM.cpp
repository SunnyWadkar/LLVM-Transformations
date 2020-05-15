// ECE 5984 S20 Assignment 3: LICM.cpp
// Group: Sunny Wadkar and Ramprasath Pitta Sekar

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include <algorithm>
#include <stack>

#include "dominance.h"

using namespace llvm;
using namespace std;

namespace {
  class LoopInvariantCodeMotion : public LoopPass {
  public:
    static char ID;
    LoopInvariantCodeMotion() : LoopPass(ID) { }
    virtual bool runOnLoop(Loop *L, LPPassManager &LPM) 
    {
        bool pass_ret = false;
        BasicBlock* preheader = L->getLoopPreheader();
        if(preheader == NULL)
        {
          return pass_ret;
        }
        std::vector<BasicBlock*> constituentLoopBlocks;
        LoopInfo& loopInfo = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
        Function* F = L->getBlocks().front()->getParent();
        Dominance* dp = new Dominance();
        dp->executeDominancePass(*F);
        getDFSOrder(*F);
        for(std::vector<BasicBlock*>::iterator i = DFSTraversal.begin(); i != DFSTraversal.end(); ++i) 
        {
          if(loopInfo.getLoopFor(*i) == L)
          {
            constituentLoopBlocks.push_back(*i);
            for(BasicBlock::iterator II = (*i)->begin(); II != (*i)->end(); ++II)
            {
              Instruction *I = &*II;
              if(isInvariant(I) && checkOperandInvariance(I,L))
              {
                loopInvariantStatements.push_back(I);
              }
            }
          }
        }
        std::vector<BasicBlock*> exitBlocks;
        for(int blockit = 0; blockit < (int)constituentLoopBlocks.size(); ++blockit)
        {
          BasicBlock* bb = constituentLoopBlocks[blockit];
          struct basicBlockProps* temp = dp->dFAResult[bb];
          for(int succit = 0; succit < (int) temp->succBlocks.size(); ++succit)
          {
            if(std::find(constituentLoopBlocks.begin(),constituentLoopBlocks.end(),temp->succBlocks[succit]) == constituentLoopBlocks.end())
            {
              exitBlocks.push_back(bb);
            }
          }
        }
        for(int i = 0; i < (int)loopInvariantStatements.size(); ++i)
        {
          BasicBlock* parentBlock = loopInvariantStatements[i]->getParent();
          // if(checkExitDominance(exitBlocks,parentBlock,dp->dominanceRelationMap)) // Assumption: Loop will atleast execute once
          {
            doCodeMotion(loopInvariantStatements[i],preheader);
            pass_ret = true;
          }
        }
        return pass_ret;
    }

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
			AU.addRequired<LoopInfoWrapperPass>();
		}

  private:
    std::vector<BasicBlock*> DFSTraversal;
    std::vector<Instruction*> loopInvariantStatements;

    bool isInvariant(Instruction* I)
    {
      return ((isSafeToSpeculativelyExecute(I)) && (!I->mayReadFromMemory()) && (!isa<LandingPadInst>(I)));
    }

    bool checkOperandInvariance(Instruction*I, Loop* L)
    {
      int numOperands = (int)I->getNumOperands();
      int opConstCtr = 0;
      for(int opc = 0; opc < numOperands; ++opc)
      {
        Value* op = I->getOperand(opc);
        if(dyn_cast<ConstantInt>(op))
        {
          opConstCtr++; 
        }
        if(opConstCtr == numOperands)
        {
          return true;
        }
        if(dyn_cast<Instruction>(op))
        {
          if(L->contains(I) && (std::find(loopInvariantStatements.begin(),loopInvariantStatements.end(),op) == loopInvariantStatements.end()))
          {
            return false;
          }
          else if (L->contains(I) && (std::find(loopInvariantStatements.begin(),loopInvariantStatements.end(),op) != loopInvariantStatements.end()))
          {
            return true;
          }
        }
      }
      return true;
    }

    bool checkExitDominance(std::vector<BasicBlock*> exitBlocks, BasicBlock* bb, std::map<BasicBlock*,std::vector<BasicBlock*>> dominanceInfo)
    {
      int blkcnt = 0;
      for(int j = 0; j<(int) exitBlocks.size(); ++j)
      {
        if(std::find(dominanceInfo[exitBlocks[j]].begin(),dominanceInfo[exitBlocks[j]].end(),bb) != dominanceInfo[exitBlocks[j]].end())
        {
          blkcnt++;
        } 
      }
      if(blkcnt == (int)exitBlocks.size())
      {
        return true;
      }
      return false;
    }

    void doCodeMotion(Instruction*I, BasicBlock* preHeader)
    {
      Instruction* end = &(preHeader->back());
      outs() << "Moving Instruction " << *I << "\n";
      I->moveBefore(end);
    }

    void getDFSOrder(Function& F)
    {
      stack<BasicBlock*> dfsStack;
      po_iterator<BasicBlock*> start = po_begin(&F.getEntryBlock());
      po_iterator<BasicBlock*> end = po_end(&F.getEntryBlock());
      while(start != end) 
      {
        dfsStack.push(*start);
        ++start;
      }
      while(!dfsStack.empty())
      {
        DFSTraversal.push_back(dfsStack.top());
        dfsStack.pop();
      }
    }
  };

  char LoopInvariantCodeMotion::ID = 0;
  RegisterPass<LoopInvariantCodeMotion> X("loop-invariant-code-motion", "ECE 5984 LICM");
}