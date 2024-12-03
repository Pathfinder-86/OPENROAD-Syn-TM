#ifndef ABC_FLOW_MANAGER_H
#define ABC_FLOW_MANAGER_H
#include <map>
#include <string>
namespace abc {
class ABC_flow_manager {
    private:
        int flow_id;
        bool collect_flow_info;
        std::map<int, std::string> flow_hash_map;                
    public:
        static ABC_flow_manager& get_instance() {
            static ABC_flow_manager instance;
            return instance;
        }        
        // run
        void run(){ run_flow(); }                
        void run_designed_flow(int flow_id);        
        void run_flow(int flow_id = 0);      /* flow_id ==0  is  deepsyn */
        
        // create flow
        int create_flow_by_manual(){ return 0; }
        int create_flow_by_script(const std::string& script_file_path){ return 0; }
        
        // helper
        void set_collect_flow_info(bool collect_flow_info) { this->collect_flow_info = collect_flow_info; }
        void list_flows();
        int get_flow_id() { return flow_id; }
        std::string get_flow_name(){ return flow_hash_map[flow_id]; }
        void init_flow_hash_map();
        void dump_stats();
        void dump_gates();
        void dump_qor_data(const std::string &stage_name, int print_type);
        
        // flow definition
        void run_deepsyn();          
        void test_command();    
        void dryrun_test_sizing();
    private:
        // Private constructor + initialization list
        ABC_flow_manager():flow_id(0),collect_flow_info(false) {
            init_flow_hash_map();
        }
        ~ABC_flow_manager() {} // Private destructor

        // Delete copy constructor and assignment operator
        ABC_flow_manager(const ABC_flow_manager&) = delete;
        ABC_flow_manager& operator=(const ABC_flow_manager&) = delete;
};
}
#endif

