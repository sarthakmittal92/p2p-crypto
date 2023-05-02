#include "include/header.hpp"

// https://stackoverflow.com/questions/16284629/undefined-reference-to-static-variable-c
ll txn::curr_txn_id = 0;
ll txn::txn_size = 8*(1<<10);       // 1 KB (bits)
ll txn::coinbase_fee = 50;

/**
 * @brief Constructor for the Transaction Class
 *
 * @param IDx Id of payer
 * @param coinbase Is it a coinbase Transaction
 * @param IDy Id of reciever
 * @param C Amount of coins
 */
txn::txn(int IDx, bool coinbase/*=false*/, int IDy/*=-1*/, ll C/*=-1*/) {
    // TxnID: IDx pays IDy C coins
    // TxnID: IDx mines 50 coins

    assert(IDx>=0 && IDx<=n);
    // assert(coinbase || (IDy>=0 && IDy<=n && C>=0));

    txn_id = txn::curr_txn_id++;
    this->IDx = IDx;
    this->IDy = IDy;
    this->coinbase = coinbase;
    this->C = (coinbase ? this->coinbase_fee : C);
}
