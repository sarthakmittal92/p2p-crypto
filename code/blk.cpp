#include "include/header.hpp"

ll blk::curr_blk_id = 0;
ll blk::max_blk_size = 8*(1<<20);       // 1 MB (bits)
map<ll,blk*> blk::blk_id_to_blk_ptr;

/**
 * @brief Constructor for Blk Class
 *
 * @param miner Pointer to the miner mining the block
 * @param parent Parent of the current block, null if no parent 
 * @param vec_txns A vector of all transactions that are saved in the block
 */
blk::blk(peer* miner, blk* parent, vector<txn*>& vec_txns) {
    this->blk_id = blk::curr_blk_id++;
    this->miner = miner;
    this->txns = vec_txns;
    this->blk_size = 0;
    this->update_parent(parent);
    this->originally_invalid = false;
    // cout << "[MAP] " << this->blk_id << ":" << this << endl; blk::blk_id_to_blk_ptr[this->blk_id] = this;
}


/**
 * @brief Will connect the block to the parent updating the children of the block
 *
 * @param new_parent Block that is the parent of current block
 */
void blk::update_parent(blk* new_parent) {

    this->parent = new_parent;
    if (new_parent != nullptr) { // If New Parent is already present
        cout << this->blk_id << " " << new_parent->blk_id << endl;
        new_parent->children.insert(this);
        this->height = new_parent->height + 1;
    }
    else { // If the current block is genesis block
        this->height = 0;
    }

}
