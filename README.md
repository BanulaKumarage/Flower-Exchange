# Flower-Exchange-Application

- The flower exchange is a system which supports basic trading. 
- An every given order is replied with an Execution Report by the application indicating the status of the order. 
- Orders sometimes could be rejected due to quantity limitations, invalid flower type, etc.

## Execute

- To generate the execution report for a given order create `.csv` file similar to the `order-*.csv` files in the `inputs` folder.
- Run the `Exchange.cpp` for the given order by following command.
```
g++ Exchange.cpp -o Exchange <<EOF
orders.csv
EOF
```
- The execution report will be generated in the `results` folder with the name `execution-report.csv`.

- To generate the execution report for the sample orders in the `inputs` folder run the `generate-results.sh` file. The execution reports will be generated in the `results` folder.

