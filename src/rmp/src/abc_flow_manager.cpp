#include "abc_flow_manager.h"
#include <fstream>
#include <iostream>
#include "abc_function.h"
namespace abc {
void ABC_flow_manager::run_designed_flow(int flow_id){        
    if(flow_hash_map.find(flow_id) == flow_hash_map.end()){
        std::cout<<"No such flow_id, return !!!"<<std::endl;            
        list_flows();            
        return;            
    }
    this->run_flow(flow_id);
    const std::string& flow_name = flow_hash_map[flow_id];        
    std::cout<<"Choose flow_id: "<<flow_id<<" flow_name: "<<flow_name<<std::endl;            
    run_flow(flow_id);
}
void ABC_flow_manager::list_flows(){
    for(auto& it: flow_hash_map){
        std::cout<<"flow_id: "<<it.first<<" flow_name: "<<it.second<<std::endl;
    }
}    
void ABC_flow_manager::run_flow(int flow_id){
    switch(flow_id){
        case 0:
            run_deepsyn();
            break;
        default:
            std::cout<<"No such flow_id, return !!!"<<std::endl;
            list_flows();
            break;
    }
}   

void ABC_flow_manager::init_flow_hash_map(){
    flow_hash_map[0] = "deepsyn";        
}


void ABC_flow_manager::run_deepsyn(){
    std::cout<<"####################"<<std::endl;
    std::cout<<"#    run_deepsyn   #"<<std::endl;
    std::cout<<"####################"<<std::endl;    

    Abc_Frame_t *pAbc = Abc_FrameGetGlobalFrame();
    if(pAbc == NULL){
        std::cout<<"Abc_FrameGetGlobalFrame failed"<<std::endl;
        return;
    }
    //STEP2: Try DeepSyn
    ABC_function::Abc_CommandAbc9Get(pAbc);
    ABC_function::Abc_CommandAbc9Ps(pAbc);
    if(ABC_function::Abc_CommandAbc9DeepSyn(pAbc, 1, 100)){
      std::cout<<"DeepSyn FAILED"<<std::endl;
    }else{
      std::cout<<"AFTER DeepSyn put and PS:"<<std::endl;
      ABC_function::Abc_CommandAbc9Ps(pAbc);
      ABC_function::Abc_CommandAbc9Put(pAbc);
      ABC_function::Abc_CommandPrintStats(pAbc);    
    }
      
    //MAP
    if(ABC_function::Abc_CommandMap(pAbc)){
      std::cout<<"MAP FAILED"<<std::endl;
    }else{
      std::cout<<"AFTER MAP PS:"<<std::endl;
      ABC_function::Abc_CommandPrintStats(pAbc);    
      ABC_function::Abc_CommandPrintGates(pAbc);        
    }
}


}  // namespace abc
