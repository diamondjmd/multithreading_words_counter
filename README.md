# Multithreading Words Counter
## Description
A multithreading program that counts the most frequent words in files.
The program has 3 versions, in order to compare its performance.

 ### - Version 1. Single-threaded implementation
Is located in src/single_thread_topk_words.cpp file 

 ### Version 2. Multithreading implementation strategy 1
A common dictionary with a mutex to protect it.
Is located in src/multi_thread_strategy_threads_with_shared_dict.cpp

 ### Version 3. Multithreading implementation strategy 2
Each thread have its own dictionary, and are merged at the end.
Is located in src/multi_thread_strategy_threads_with_own_dict.cpp.cpp

## Example of how to run the program
the program expects at least one argument with the path of .txt file to read. In example:
```
    ./src/single_thread_topk_words  ../data/gatsby.txt
```
or few files:

```
    ./src/single_thread_topk_words  ../data/gatsby.txt ../data/another_file.txt
```



