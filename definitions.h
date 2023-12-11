#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

string outputfile = "execution_rep.csv";

class Order
{
private:
    void helper(int, double);

public:
    string order_id;
    string client_order_id;
    string instrument;
    int side;
    int exec_status;
    int quantity;
    double price;
    string reason;

    Order(int, string, string, int, int, int, double);

    void write_csv();
    void write_csv(int);
    void write_csv(double);
    void write_csv(int, double);

    bool is_valid();
    string transaction_time();
};

class OrderBook
{
public:
    string instrument;
    vector<Order> buy;
    vector<Order> sell;
    void trade(Order &order);

    OrderBook(string);

private:
    static bool buy_compare(Order a, Order b);
    static bool sell_compare(Order a, Order b);
};
