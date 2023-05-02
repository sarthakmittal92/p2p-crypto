#include "include/header.hpp"

/**
 * @brief Constructor for the Event Class
 *
 * @param timestamp Timestamp of the event
 * @param type Type of event (Code given from 1 to 7)
 * @param p Peer associated with the event (may be generator, sender or reciever)
 * @param tran Transaction associated with the event
 * @param from Peer who sent the event (iff applicable)
 * @param block Block Associated with the event
 */
event::event(ld timestamp, int type, peer* p/*=nullptr*/, txn* tran/*=nullptr*/, peer* from/*=nullptr*/, blk* block/*=nullptr*/) {
    this->timestamp = timestamp;
    this->type = type;
    this->p = p;
    this->tran = tran;
    this->from = from;
    this->block = block;
}

/**
 * @brief Function to run the event
 * Checks the type of event it is and run the appropriate functions 
 *
 * @param sim Simulation which has the global variables
 */
void event::run(simulator& sim) {
	
    cout << "event run: type=" << type << '\n';
    switch (type) {
        case 1: {
            p->generate_txn(sim, this);
            break;
        }
        case 2: {
            // if(tran==nullptr) exit(1);
            // cout << this->tran->txn_id << '\n';
            p->forward_txn(sim, this);
            break;
        }
        case 3: {
            // if(from==nullptr || tran==nullptr) exit(1);
            // cout << this->tran->txn_id << '\n';
            p->hear_txn(sim, this);
            break;
        }
        case 4: {
            p->generate_blk(sim, this);
            break;
        }
        case 5: {
            p->forward_blk(sim, this);
            break;
        }
        case 6: {
            p->hear_blk(sim, this);
            break;
        }
        case 7: {
            p->update_tree(sim, this);
            break;
        }
        default: {
            cout << "incorrect event type: " << type << '\n';
            break;
        }
    }
}
