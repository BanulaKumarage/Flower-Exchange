#include "definitions.h"

bool OrderBook::buy_compare(Order a, Order b)
{
    return a.price > b.price;
}
bool OrderBook::sell_compare(Order a, Order b)
{
    return a.price < b.price;
}

void OrderBook::trade(Order &order)
{
    if (order.side == 2)
    {
        if (buy.empty())
        {
            sell.push_back(order);
            sort(sell.begin(), sell.end(), sell_compare);
            if (order.exec_status != 3)
                order.write_csv();
            return;
        }
        for (int j = 0; j < buy.size(); j++)
        {
            Order &buys = buy[j];
            if (buys.price >= order.price)
            {
                if (buys.quantity == order.quantity)
                {
                    order.exec_status = 2;
                    order.write_csv(buys.price);

                    buys.exec_status = 2;
                    buys.write_csv();
                    buy.erase(buy.begin() + j);

                    return;
                }
                if (buys.quantity < order.quantity)
                {
                    order.exec_status = 3;
                    order.quantity = order.quantity - buys.quantity;
                    order.write_csv(buys.quantity, buys.price);

                    buys.exec_status = 2;
                    buys.write_csv();
                    buy.erase(buy.begin() + j);

                    if (!buy.empty() || order.quantity != 0)
                        trade(order);

                    return;
                }
                else // buys.quantity > order.quantity
                {
                    order.exec_status = 2;
                    order.write_csv(buys.price);

                    buys.exec_status = 3;
                    buys.quantity = buys.quantity - order.quantity;
                    buys.write_csv(order.quantity);

                    return;
                }
            }
            else // buys.price < order.price
            {
                sell.push_back(order);
                sort(sell.begin(), sell.end(), sell_compare);
                if (order.exec_status != 3)
                    order.write_csv();
                return;
            }
        }
        sell.push_back(order);
        sort(sell.begin(), sell.end(), sell_compare);
        if (order.exec_status != 3)
            order.write_csv();
    }
    else // buy
    {
        if (sell.empty())
        {
            buy.push_back(order);
            sort(buy.begin(), buy.end(), buy_compare);
            if (order.exec_status != 3)
                order.write_csv();
            return;
        }
        for (int j = 0; j < sell.size(); j++)
        {
            Order &sells = sell[j];
            if (sells.price > order.price)
            {
                buy.push_back(order);
                sort(buy.begin(), buy.end(), buy_compare);
                if (order.exec_status != 3)
                    order.write_csv();
                return;
            }
            else if (sells.price <= order.price)
            {
                if (sells.quantity == order.quantity)
                {
                    order.exec_status = 2;
                    order.write_csv(sells.price);

                    sells.exec_status = 2;
                    sells.write_csv();
                    sell.erase(sell.begin() + j);

                    return;
                }
                if (sells.quantity < order.quantity)
                {
                    order.exec_status = 3;
                    order.quantity = order.quantity - sells.quantity;
                    order.write_csv(sells.quantity, sells.price);

                    sells.exec_status = 2;
                    sells.write_csv();
                    sell.erase(sell.begin() + j);

                    if (!sell.empty() || order.quantity != 0)
                        trade(order);

                    return;
                }
                else // sells.quantity > order.quantity
                {
                    order.exec_status = 2;
                    order.write_csv(sells.price);

                    sells.exec_status = 3;
                    sells.quantity = sells.quantity - order.quantity;
                    sells.write_csv(order.quantity);

                    return;
                }
            }
        }
        buy.push_back(order);
        sort(buy.begin(), buy.end(), buy_compare);
        if (order.exec_status != 3)
            order.write_csv();
    }
}

OrderBook::OrderBook(string Instrument)
{
    instrument = Instrument;
}

Order::Order(int i, string Client_Order_ID, string Instrument, int Side, int Exec_Status, int Quantity, double Price)
{
    order_id = "ord" + to_string(i);
    client_order_id = Client_Order_ID;
    instrument = Instrument;
    side = Side;
    exec_status = Exec_Status;
    quantity = Quantity;
    price = Price;
    reason = "";
}

string Order::transaction_time()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm *timeInfo = localtime(&currentTime);
    ostringstream oss;
    oss << std::put_time(timeInfo, "%Y%m%d-%H%M%S");
    
    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch() % chrono::seconds(1));
    oss << '.' << std::setfill('0') << std::setw(3) << milliseconds.count();

    return oss.str();
}

void Order::helper(int quantity, double price)
{
    ofstream file(outputfile, ios::app);
    if (file.is_open())
        file << order_id << "," << client_order_id << "," << instrument << "," << side << "," << exec_status << "," << quantity << "," << fixed << setprecision(2) << price << "," << reason << "," << transaction_time() << endl;
    file.close();
}

void Order::write_csv()
{
    helper(quantity, price);
}

void Order::write_csv(int Quantity)
{
    helper(Quantity, price);
}

void Order::write_csv(double Price)
{
    helper(quantity, Price);
}

void Order::write_csv(int Quantity, double Price)
{
    helper(Quantity, Price);
}

bool Order::is_valid()
{
    if (client_order_id.empty())
    {
        exec_status = 1;
        reason = "Invalid Client Order ID";
        return false;
    }

    if (!(instrument == "Rose" || instrument == "Lavender" || instrument == "Lotus" || instrument == "Tulip" || instrument == "Orchid"))
    {
        exec_status = 1;
        reason = "Invalid instrument";
        return 0;
    }

    if (!(side == 1 || side == 2))
    {
        exec_status = 1;
        reason = "Invalid side";
        return false;
    }

    if (quantity < 10 || quantity > 1000 || quantity % 10 != 0)
    {
        exec_status = 1;
        reason = "Invalid size";
        return false;
    }

    if (price <= 0.00)
    {
        exec_status = 1;
        reason = "Invalid price";
        return false;
    }

    return true;
}

int main()
{
    OrderBook roseOrderBook("Rose");
    OrderBook lavenderOrderBook("Lavender");
    OrderBook lotusOrderBook("Lotus");
    OrderBook tulipOrderBook("Tulip");
    OrderBook orchidOrderBook("Orchid");

    string inputfilename;

    cin >> inputfilename;
    auto pos = inputfilename.find('-');
    if (pos != string::npos) {
        outputfile = "results/" + inputfilename.substr(pos+1, 1) + "-execution_rep.csv";
    }


    ofstream file(outputfile, ofstream::trunc);
    if (file.is_open())
    {
        file << "Execution_Rep.csv" << endl;
        file << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time" << endl;
    }
    else
        return 0;

    file.close();



    int i = 0;
    ifstream inputFile;

    inputFile.open(inputfilename);
    string line = "";

    getline(inputFile, line);
    line = "";
    getline(inputFile, line);
    line = "";

    while (getline(inputFile, line))
    {
        i++;
        stringstream inputString(line);
        string Client_Order_Id;
        string Instrument;
        int Side;
        int Quantity;
        double Price;
        string tempString;
        int Exec_status = 0;

        getline(inputString, Client_Order_Id, ',');
        getline(inputString, Instrument, ',');

        getline(inputString, tempString, ',');
        Side = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Quantity = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Price = stold(tempString);

        Order order(i, Client_Order_Id, Instrument, Side, Exec_status, Quantity, Price);

        if (order.is_valid())
        {
            if (Instrument == "Rose")
                roseOrderBook.trade(order);
            else if (Instrument == "Lavender")
                lavenderOrderBook.trade(order);
            else if (Instrument == "Lotus")
                lotusOrderBook.trade(order);
            else if (Instrument == "Tulip")
                tulipOrderBook.trade(order);
            else if (Instrument == "Orchid")
                orchidOrderBook.trade(order);
        }
        else
            order.write_csv();

        line = "";
    }

    return 0;
}