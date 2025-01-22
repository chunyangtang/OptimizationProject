make

# ./main 50 500 0.8 0.2 1000 0.99 < BEN30-XY.txt &
# ./main 100 500 0.8 0.2 1000 0.99 < BEN30-XY.txt &
# ./main 200 500 0.8 0.2 1000 0.99 < BEN30-XY.txt &
# ./main 300 500 0.8 0.2 1000 0.99 < BEN30-XY.txt &

# wait

# ./main 200 500 0.8 0.1 1000 0.99 < BEN30-XY.txt &
# ./main 200 500 0.9 0.1 1000 0.99 < BEN30-XY.txt &
# # ./main 200 500 0.8 0.2 1000 0.99 < BEN30-XY.txt &
# ./main 200 500 0.9 0.2 1000 0.99 < BEN30-XY.txt &

# wait

# # ./main 200 500 0.9 0.1 1000 0.99 < BEN30-XY.txt &
# ./main 200 500 0.9 0.1 3000 0.99 < BEN30-XY.txt &
# ./main 200 500 0.9 0.1 1000 0.95 < BEN30-XY.txt &
# ./main 200 500 0.9 0.1 3000 0.95 < BEN30-XY.txt &

# wait

# ./main 200 200 0.9 0.1 1000 0.99 < BEN30-XY.txt &
# # ./main 200 500 0.9 0.1 1000 0.99 < BEN30-XY.txt &
# ./main 200 1000 0.9 0.1 1000 0.99 < BEN30-XY.txt &
# ./main 200 2000 0.9 0.1 1000 0.99 < BEN30-XY.txt &

# wait

# make clean
# make collect_data

# ./main 200 500 0.9 0.1 1000 0.99 < BEN30-XY.txt &

# wait

# make clean
# make

# ./main 200 500 0.9 0.1 1000 0.99 < BEN50-XY.txt &

# wait

# ./main 200 500 0.9 0.1 1000 0.99 < BEN75-XY.txt &

# wait
