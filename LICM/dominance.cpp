#include "dominance.h"

using namespace std;

namespace llvm{

    void Dominance::executeDominancePass(Function& F) 
    {
      for (BasicBlock &BB : F) 
      {
        basicBlocks.push_back(&BB);
      }
      BitVector boundaryCond(basicBlocks.size(), false);
      BitVector initCond(basicBlocks.size(), true);
      dataFlow* df = new dataFlow(basicBlocks.size(),INTERSECTION,FORWARD,boundaryCond,initCond);
      genTFDependencies(F,basicBlocks);
      df->executeDataFlowPass(F,bbSets);
      dFAResult = df->dataFlowHash;
      buildDominanceMap(dominanceRelationMap,df->dataFlowHash);
    //   printDominanceResult(dominanceRelationMap);
    }

    void Dominance::genTFDependencies(Function &F, std::vector<BasicBlock*> domain)
    {
      BitVector empty((int)domain.size(), false);
      int vectorIdx = 0;
      for(BasicBlock* bb : domain)
      {
        domainMap[bb] = vectorIdx;
        revDomainMap[vectorIdx] = bb;
        ++vectorIdx;
      }
      for(BasicBlock &BB : F)
      {
        struct basicBlockDeps* bbSet = new basicBlockDeps();
        bbSet->blockRef = &BB;
        bbSet->genSet = empty;
        bbSet->killSet = empty;
        bbSet->genSet.set(domainMap[bbSet->blockRef]); 
        bbSets[&BB] = bbSet;
      }
    }

    void Dominance::buildDominanceMap(std::map<BasicBlock*,std::vector<BasicBlock*>> &dmap,std::map<BasicBlock*,basicBlockProps*> dFAHash)
    {
      std::map<BasicBlock*,basicBlockProps*>::iterator it = dFAHash.begin();
      while(it != dFAHash.end())
      {
        struct basicBlockProps* temp = dFAHash[it->first];
        std::vector<BasicBlock*> domSet;
        for(int k = 0; k < (int)temp->bbInput.size(); k++)
        {
          if(temp->bbInput[k])
          {
            domSet.push_back(revDomainMap[k]);
          }
        }
        dmap[temp->block] = domSet;
        it++;
      }
    }

    void Dominance::printDominanceResult(std::map<BasicBlock*,std::vector<BasicBlock*>> dmap)
    {
      std::map<BasicBlock*,std::vector<BasicBlock*>>::iterator it = dmap.begin();
      while(it != dmap.end())
      {
        BasicBlock* bb = it->first;
        std::vector<BasicBlock*> domBB = it->second;
        outs() << "Basic Block Name : ";
        if(bb->hasName())
        {
          outs() << bb->getName() << "\n";
        }
        else
        {
          outs() << get_block_reference(bb) << "\n";
        }
        outs() << "Dominators :";
        printBasicBlockSet(&domBB);
        it++;
      }
    }

    void Dominance::printDFAResults(std::map<BasicBlock*,basicBlockProps*> dFAHash)
    {
      std::map<BasicBlock*,basicBlockProps*>::iterator it = dFAHash.begin();
      while(it != dFAHash.end())
      {
        struct basicBlockProps* temp = dFAHash[it->first];
        outs() << "Basic Block Name : ";
        if(temp->block->hasName())
        {
          outs() << temp->block->getName() << "\n";
        }
        else
        {
          outs() << get_block_reference(temp->block) << "\n";
        }
        std::vector<BasicBlock*> genbb;
        outs() << "gen[BB] : ";
        for(int m = 0; m < (int)temp->genSet.size(); m++)
        {
          if(temp->genSet[m])
          {
            genbb.push_back(revDomainMap[m]);
          }
        }
        printBasicBlockSet(&genbb);
        std::vector<BasicBlock*> killbb;
        outs() << "kill[BB] : ";
        for(int n = 0; n < (int)temp->killSet.size(); n++)
        {
          if(temp->killSet[n])
          {
            killbb.push_back(revDomainMap[n]);
          }
        }
        printBasicBlockSet(&killbb);
        std::vector<BasicBlock*> inbb;
        outs() << "IN[BB] : ";
        for(int k = 0; k < (int)temp->bbInput.size(); k++)
        {
          if(temp->bbInput[k])
          {
            inbb.push_back(revDomainMap[k]);
          }
        }
        printBasicBlockSet(&inbb);
        std::vector<BasicBlock*> outbb;
        outs() << "OUT[BB] : ";
        for(int l = 0; l < (int)temp->bbOutput.size(); l++)
        {
          if(temp->bbOutput[l])
          {
            outbb.push_back(revDomainMap[l]);
          }
        }
        printBasicBlockSet(&outbb);
        outs() << "\n";
        it++;
      }
    }

    std::string Dominance::get_block_reference(BasicBlock *BB)
    {
      std::string block_address;
      raw_string_ostream string_stream(block_address);
      BB->printAsOperand(string_stream, false);
      return string_stream.str();
    }

    void Dominance::printBasicBlockSet(std::vector<BasicBlock*> *x) 
    {
      bool first = true;
      outs() << "{";
      for (std::vector<BasicBlock*>::iterator it=x->begin(), iend=x->end(); it!=iend; ++it) 
      {
        BasicBlock* temp = *it;
        if (!first) 
        {
          outs() << ", ";
        }
        else 
        {
          first = false;
        }
        if(temp->hasName())
        {
          outs() << temp->getName();
        }
        else
        {
          outs() << get_block_reference(temp);
        }
      }
      outs() << "}\n";
    }
}