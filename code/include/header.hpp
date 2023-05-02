#ifndef HEADER
#define HEADER

#include "cxxopts.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <cassert>
using namespace std;

typedef long long ll;
typedef long double ld;     // better precision [12 bytes]

extern mt19937 rng;
extern mt19937_64 rng_64;
// extern mt19937& rng_to_use;     // can't use auto (!?)

extern int n;
extern ld current_time;

class simulator;
class peer;
class event;
struct compare_events;
struct compare_events_desc;
class txn;
struct compare_txns;
class blk;
struct compare_blk_ptrs;

class event {
    public:
        ld timestamp;
        int type;
        peer* p;        // can this be done with just `peer& p`?
        txn* tran;
        peer* from;
        blk* block;

        event(ld timestamp, int type, peer* p=nullptr, txn* tran=nullptr, peer* from=nullptr, blk* block=nullptr);
        void run(simulator& sim);
};

struct compare_events {
    // compare events based on timestamp [ascending order]
    inline bool operator() (const event& e1, const event& e2) {
        if(e1.timestamp==e2.timestamp) return &e1<&e2;
        else return e1.timestamp<e2.timestamp;
    }
};

struct compare_events_desc {
    // compare events based on timestamp [descending order]
    inline bool operator() (const event& e1, const event& e2) {
        if(e1.timestamp==e2.timestamp) return &e1>&e2;
        else return e1.timestamp>e2.timestamp;
    }
};

struct compare_event_ptrs_desc {
    // compare event pointers based on timestamp [descending order]
    inline bool operator() (const event* const e1, const event* const e2) {
        if(e1->timestamp==e2->timestamp) return e1>e2;
        else return e1->timestamp>e2->timestamp;
    }
};

class txn {
    public:
        static ll curr_txn_id;
        static ll txn_size;
        static ll coinbase_fee;
        ll txn_id;
        int IDx, IDy;
        ll C;
        bool coinbase;
        txn(int IDx, bool coinbase=false, int IDy=-1, ll C=-1);
};

struct compare_txns {
    // compare events based on id [ascending order]
    inline bool operator() (const txn& txn1, const txn& txn2) {
        return txn1.txn_id<txn2.txn_id;
    }
};

struct compare_txn_ptrs {
    // compare transaction pointers based on txn_id
    inline bool operator() (const txn* const t1, const txn* const t2) {
        if(t1->txn_id==t2->txn_id) return t1<t2;
        else return t1->txn_id<t2->txn_id;
    }
};

class blk {
    public:
        static ll curr_blk_id;
        static ll max_blk_size;
        static map<ll,blk*> blk_id_to_blk_ptr;
        ll blk_size;
        ll blk_id;
        peer* miner;
        blk* parent;
        set<blk*> children;
        vector<txn*> txns;
        ll height;
        bool originally_invalid;
        blk(peer* miner, blk* parent, vector<txn*>& vec_txns);
        void update_parent(blk* new_parent);
};

struct compare_blk_ptrs {
    // compare block pointers based on height
    inline bool operator() (const blk* const b1, const blk* const b2) {
        if(b1->height==b2->height) return b1<b2;
        else return b1->height<b2->height;
    }
};

class peer {
    public:
        static blk* genesis;
        int id;
        ld next_time;
        vector<ld> curr_balances;
        bool slow, lowCPU;
                
        set<txn*, compare_txn_ptrs> txns_not_included;  // txns not included in any block till now
                                                        // (according to this node)
        set<ll> txns_all;   // IDs of all txns heard by this node till now
                            // (used for loop-less fwd-ing)
        map<ll,vector<ll>> txn_sent_to;
        
        ld fraction_hashing_power;
        
        blk* latest_blk;    // this peer's copy of the blockchain

        set<blk*> blks_all;
        map<blk*,ld> blk_arrivals;

        map<ll,vector<ll>> blk_sent_to;

        set<blk*, compare_blk_ptrs> blks_not_included;      // blks heard by this node, but not
                                                            // included in its blockchain copy
        set<blk*> curr_tree; // tree of the peer

        peer(int id);

        // txn related functions
        void generate_txn(simulator& sim, event* e);
        void forward_txn(simulator& sim, event* e);
        void hear_txn(simulator& sim, event* e);
        void print_all_txns();

        // blk related functions
        bool is_invalid(vector<ld> balances);
        void generate_blk(simulator& sim, event* e);
        void forward_blk(simulator& sim, event* e);
        void hear_blk(simulator& sim, event* e);
        bool check_blk(blk* b);
        bool check_blk_from_genesis(blk* b);
        void update_tree(simulator& sim, event* e);

        void print_longest_chain();
};

class simulator {
    private:
        int seed;

        /**
         * z0:
         * z1:
        */
        ld z0, z1;
        
        ld current_time;

        // pq for events
        priority_queue<event*, vector<event*>, compare_event_ptrs_desc> pq_events;
                                            // descending for min heap

        void create_graph(int a, int b);
        vector<int> pick_random(int n, int k);
        void dfs(int node, vector<unordered_set<int>>& adj_sets);

    public:
        ld Ttx; // txn mean time

        vector<vector<int>> adj;    // adjacency list representation
        vector<bool> visited;       // temporary; used for network creation

        // latencies
        ld fast_link_speed, slow_link_speed, queuing_delay_numerator;
        vector<vector<ld>> rho; // light propagation delay

        ld Tblk; // block interarrival time
        ld Simulation_Time;
        
        vector<peer> peers_vec;
        simulator(int seed, ld z0, ld z1, ld Ttx, ld Tblk, int min_ngbrs, int max_ngbrs, ld sim_time);
        void print_graph();
        void run();
        void push(event* e);

        void print_entire_tree(ostream& out);

};

#endif
