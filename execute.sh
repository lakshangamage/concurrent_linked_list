#!/bin/sh
# ----------------------------------------------------------------------------

# ----- Compile the source codes -----------------------------------------
gcc -g -Wall -pthread -o serial_linked_list serial_linked_list.c -lm
gcc -g -Wall -pthread -o mutex_linked_list mutex_linked_list.c -lm
gcc -g -Wall -pthread -o rw_linked_list rw_linked_list.c -lm

echo "Compilation Finished."

# ----- Run Serial Linked list -----------------------------------------
echo "...Running Serial List..."
./serial_linked_list  300 1000 10000 0.99 0.005 0.005
echo "Case 1 Finished..."
./serial_linked_list  300 1000 10000 0.90 0.05 0.05
echo "Case 2 Finished..."
./serial_linked_list  300 1000 10000 0.500 0.25 0.25
echo "Case 3 Finished..."

# ----- Run Mutex Linked list -----------------------------------------
echo "...Running Mutex List..."
./mutex_linked_list  300 1000 10000 1 0.99 0.005 0.005
./mutex_linked_list  300 1000 10000 2 0.99 0.005 0.005
./mutex_linked_list  300 1000 10000 4 0.99 0.005 0.005
./mutex_linked_list  300 1000 10000 8 0.99 0.005 0.005
echo "Case 1 Finished..."
./mutex_linked_list  300 1000 10000 1 0.90 0.05 0.05
./mutex_linked_list  300 1000 10000 2 0.90 0.05 0.05
./mutex_linked_list  300 1000 10000 4 0.90 0.05 0.05
./mutex_linked_list  300 1000 10000 8 0.90 0.05 0.05
echo "Case 2 Finished..."
./mutex_linked_list  400 1000 10000 1 0.500 0.25 0.25
./mutex_linked_list  300 1000 10000 2 0.500 0.25 0.25
./mutex_linked_list  300 1000 10000 4 0.500 0.25 0.25
./mutex_linked_list  300 1000 10000 8 0.500 0.25 0.25
echo "Case 3 Finished..."

# ----- Run Read Write Lock Linked list -----------------------------------------
echo "...Running Read Write Lock List..."
./rw_linked_list  300 1000 10000 1 0.99 0.005 0.005
./rw_linked_list  300 1000 10000 2 0.99 0.005 0.005
./rw_linked_list  300 1000 10000 4 0.99 0.005 0.005
./rw_linked_list  300 1000 10000 8 0.99 0.005 0.005
echo "Case 1 Finished..."
./rw_linked_list  300 1000 10000 1 0.90 0.05 0.05
./rw_linked_list  300 1000 10000 2 0.90 0.05 0.05
./rw_linked_list  300 1000 10000 4 0.90 0.05 0.05
./rw_linked_list  300 1000 10000 8 0.90 0.05 0.05
echo "Case 2 Finished..."
./rw_linked_list  400 1000 10000 1 0.500 0.25 0.25
./rw_linked_list  300 1000 10000 2 0.500 0.25 0.25
./rw_linked_list  300 1000 10000 4 0.500 0.25 0.25
./rw_linked_list  400 1000 10000 8 0.500 0.25 0.25
echo "Case 3 Finished..."


