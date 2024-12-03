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
        case 1:
            dryrun_test_sizing();
            break;
        case 2:
            test_command();
            break;
        default:
            std::cout<<"No such flow_id, return !!!"<<std::endl;
            list_flows();
            break;
    }
}   

void ABC_flow_manager::init_flow_hash_map(){
    flow_hash_map[0] = "DeepSyn";  
    flow_hash_map[1] = "TestSizing";
    flow_hash_map[2] = "TestCommand";
}

void ABC_flow_manager::dump_stats(){
    Abc_Frame_t *pAbc = Abc_FrameGetGlobalFrame();
    if(collect_flow_info){
        //ABC_function::Abc_CommandDumpStats();
    }
    ABC_function::Abc_CommandPrintStats(pAbc);
}

void ABC_flow_manager::dump_gates(){
    Abc_Frame_t *pAbc = Abc_FrameGetGlobalFrame();
    if(collect_flow_info){
        //ABC_function::Abc_CommandDumpGates();                    
    }
    ABC_function::Abc_CommandPrintGates(pAbc);
}

void ABC_flow_manager::dump_qor_data(const std::string &stage_name,int print_type){
    collect_flow_info = true;
    if(!collect_flow_info){
        return;
    }    
    ABC_function::Abc_CommandDumpQOR(stage_name,print_type);
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
    //INIT
    set_collect_flow_info(true);
    dump_qor_data("INIT",0);
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
      dump_stats();
    }
    dump_qor_data("DEEPSYN",0);    
      
    //MAP
    if(ABC_function::Abc_CommandMap(pAbc)){
      std::cout<<"MAP FAILED"<<std::endl;
    }else{
      std::cout<<"AFTER MAP PS:"<<std::endl;
      dump_stats();
      dump_gates();            
    }    
    dump_qor_data("MAP",1);
}

void ABC_flow_manager::dryrun_test_sizing(){

    Abc_Frame_t *pAbc = Abc_FrameGetGlobalFrame();
    if(pAbc == NULL){
        std::cout<<"Abc_FrameGetGlobalFrame failed"<<std::endl;
        return;
    }
    run_deepsyn();
    
    std::cout<<"####################"<<std::endl;
    std::cout<<"#  testing_sizing  #"<<std::endl;
    std::cout<<"####################"<<std::endl;    
    //STEP3: Try Sizing
    ABC_function::Scl_CommandTopo(pAbc);
    std::cout<<"\n";
    std::cout<<"#########"<<std::endl;
    std::cout<<"#  STA  #"<<std::endl;
    std::cout<<"#########"<<std::endl;
    ABC_function::Scl_CommandStime(pAbc);
    //std::cout<<"############"<<std::endl;
    //std::cout<<"#  Buffer  #"<<std::endl;
    //std::cout<<"############"<<std::endl;
    //ABC_function::Scl_CommandBuffer(pAbc);
    //std::cout<<"#########"<<std::endl;
    //std::cout<<"#  STA  #"<<std::endl;
    //std::cout<<"#########"<<std::endl;
    //ABC_function::Scl_CommandStime(pAbc);
    //dump_stats();
    //dump_stats();
    std::cout<<"############"<<std::endl;
    std::cout<<"#  Upsize  #"<<std::endl;
    std::cout<<"############"<<std::endl;
    ABC_function::Scl_CommandUpsize(pAbc);
    //dump_stats();
    std::cout<<"#########"<<std::endl;
    std::cout<<"#  STA  #"<<std::endl;
    std::cout<<"#########"<<std::endl;
    ABC_function::Scl_CommandStime(pAbc);
    std::cout<<"############"<<std::endl;
    std::cout<<"#  dnsize  #"<<std::endl;
    std::cout<<"############"<<std::endl;
    ABC_function::Scl_CommandDnsize(pAbc);
    ABC_function::Scl_CommandStime(pAbc);    
    dump_stats();
}

void ABC_flow_manager::test_command(){
    //test all command
    std::cout<<"####################"<<std::endl;
    std::cout<<"#  test_command    #"<<std::endl;
    std::cout<<"####################"<<std::endl;

    Abc_Frame_t *pAbc = abc::Abc_FrameGetGlobalFrame();
    ABC_function::Abc_CommandPrintStats(pAbc);
    ABC_function::Abc_CommandStrash(pAbc);
    ABC_function::Abc_CommandLogic(pAbc);
    ABC_function::Abc_CommandSweep(pAbc);
    ABC_function::Abc_CommandBalance(pAbc);
    ABC_function::Abc_CommandRewrite(pAbc);
    ABC_function::Abc_CommandRefactor(pAbc);
    ABC_function::Abc_CommandResubstitute(pAbc);
    ABC_function::Abc_CommandPrintStats(pAbc);
    ABC_function::Abc_CommandAbc9Get(pAbc);
    ABC_function::Abc_CommandAbc9Ps(pAbc);
    ABC_function::Abc_CommandAbc9DeepSyn(pAbc, 1, 100);
    ABC_function::Abc_CommandAbc9Ps(pAbc);
    ABC_function::Abc_CommandAbc9Put(pAbc);
    ABC_function::Abc_CommandPrintStats(pAbc);      
    ABC_function::Abc_CommandMap(pAbc);
    ABC_function::Abc_CommandPrintStats(pAbc);
    ABC_function::Abc_CommandPrintGates(pAbc);
    ABC_function::Scl_CommandTopo(pAbc);
    ABC_function::Scl_CommandUpsize(pAbc);
    ABC_function::Abc_CommandPrintStats(pAbc);
}


}  // namespace abc
