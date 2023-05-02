#include "include/header.hpp"
#include <fstream>

// https://codeforces.com/blog/entry/61587
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 rng_64(chrono::steady_clock::now().time_since_epoch().count());
// mt19937& rng_to_use = rng;

int n = 0;
ld current_time = 0.0L;

int main(int argc, const char* argv[]) {

    ios_base::sync_with_stdio(false); cin.tie(0);

    // argument parser: https://github.com/jarro2783/cxxopts
    cxxopts::Options options(argv[0], "P2P Cryptocurrency Network");

    options.add_options()
        ("n, nodes", "number of nodes", cxxopts::value<int>()->default_value("10"))
        ("z0, slow", "percentage of slow nodes", cxxopts::value<ld>()->default_value("0.2"))
        ("z1, low", "percentage of low CPU nodes", cxxopts::value<ld>()->default_value("0.2"))
        ("Ttx, txn_interarrival_mean", "transactions' mean interarrival time", cxxopts::value<ld>()->default_value("100"))
        ("Tblk, blk_interarrival_mean", "blocks' mean interarrival time", cxxopts::value<ld>()->default_value("600"))
        ("min_ngbrs", "minimum number of neighbors per node", cxxopts::value<int>()->default_value("4"))
        ("max_ngbrs", "maximum number of neighbors per node", cxxopts::value<int>()->default_value("8"))
        ("seed", "random seed", cxxopts::value<int>()->default_value("0"))
        ("sim_time", "simulation time", cxxopts::value<ld>()->default_value("10000"))
    ;

    auto result = options.parse(argc, argv);

    n = result["n"].as<int>();
    ld z0 = result["z0"].as<ld>();
    ld z1 = result["z1"].as<ld>();
    ld Ttx = result["Ttx"].as<ld>();
    ld Tblk = result["Tblk"].as<ld>();
    int min_ngbrs = result["min_ngbrs"].as<int>();
    int max_ngbrs = result["max_ngbrs"].as<int>();
    int seed = result["seed"].as<int>();
    ld sim_time = result["sim_time"].as<ld>();

    simulator sim(seed, z0, z1, Ttx, Tblk, min_ngbrs, max_ngbrs, sim_time);
    sim.print_graph();

	cout << "STARTING" << endl;
    sim.run();
    event* e = new event(0, 7);

	cout << "NUMBER OF PEERS SAVED " << sim.peers_vec.size() <<  endl;
    ofstream outfile, outfile2;
    outfile.open("peers.txt");
    outfile2.open("ratios.txt");
    for(auto&& p : sim.peers_vec) {
		cout << "START " << endl;
        p.update_tree(sim,e);
        outfile << p.id << endl;
        for (blk* b:p.curr_tree) {
            cout << b->blk_id << "(" << p.blk_arrivals[b] << ")" << ":" << b->height << "(";
            if (b->parent != nullptr) {
                outfile << "id" <<  b->blk_id << "time" << p.blk_arrivals[b] << " id" << b->parent->blk_id << "time" << p.blk_arrivals[b->parent] << endl;
            }
            for (txn* t:b->txns) {
                cout << t->txn_id << ",";
            }
            cout << ") ";
        }
        int present = 0;
        blk* b_iter = p.latest_blk;
        while (b_iter->parent != nullptr) {
            if (b_iter->miner->id == p.id) {
                present++;
            }
            b_iter = b_iter->parent;
        }
        int total = 0;
        for (blk* b:p.blks_all) {
            if (b->miner->id == p.id) {
                total++;
            }
        }
        if (total != 0) {
            outfile2 << p.id << " (" << p.slow << "|" << p.lowCPU << "):" << float(present) / total << endl;
        }
        else {
            outfile2 << p.id << " (" << p.slow << "|" << p.lowCPU << "):" << -1 << endl;
        }
        cout << endl;
        cout << "Txns left: ";
        for (txn* t:p.txns_not_included) {
            cout << t->txn_id << " ";
        }
        cout << endl;
        p.print_all_txns();
        p.print_longest_chain();
		cout << "ONE DONE" << endl;
    }
    outfile.close();

    outfile.open("tree.txt");
    sim.print_entire_tree(outfile);
    outfile.close();

    ofstream out;
    out.open("lengths.txt");
    for (ll i=0; i<blk::curr_blk_id; i++) {
        auto it = blk::blk_id_to_blk_ptr.find(i);
        if(it == blk::blk_id_to_blk_ptr.end() || it->second==nullptr) continue;
        if (it->second->children.size() == 0) {
            out << it->second->height << endl;
        }
    }
    out.close();

	cout << "PROGRAM ENDING" << endl;

}
