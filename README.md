# Homework-3: MPI

```
Name : Mitansh Kayathwal
Roll No: 2021101026
Branch : CSE
Course : Distributed Systems , Monsoon '24
```

# **_Directory Structure_**

```
📂 Team65
├── 📁 1
│   └── 📄 1.cpp
│   └── 📄 1_brute_force.cpp
│   └── 📄 1_queries.cpp
│   └── 📄 1_tester.sh
│   └── 📄 1_test_gen.sh
├── 📁 2
│   └── 📄 2.cpp
│   └── 📄 2_brute_force.cpp
│   └── 📄 2_tester.sh
│   └── 📄 2_test_gen.sh
├── 📁 3
│   └── 📄 3.cpp
│   └── 📄 3_brute_force.cpp
│   └── 📄 3_tester.sh
│   └── 📄 3_test_gen.sh
├── 📁 4
│   └── 📄 4.cpp
│   └── 📄 4_brute_force.cpp
│   └── 📄 4_tester.sh
│   └── 📄 4_test_gen.sh
├── 📁 5
│   └── 📄 5.cpp
│   └── 📄 5_brute_force.cpp
│   └── 📄 5_tester.sh
│   └── 📄 5_test_gen.sh
└── 📄 Report.pdf
```

### To run the test cases and check results:

1. **Navigate to the directory** of the respective task (e.g., `cd 1` for Task 1).
2. The *.cpp files contain the parallelized MPI code for respective questions.
3. The *_brute_force.cpp file contains a cpp file with optimal brute-force code of the respective question.
4. The *_test_gen.cpp file contains the test-case generator script for the respective question.
5. The *_tester.sh contains the logic for first generating test-cases and then run the parallelized code and bruteforce code and compare outputs.
6. **Run the respective test scripts**:
   ```
    $ chmod +x 1_test_gen.sh
    $ ./1_test_gen.sh
   ```
7. In directory 1, 1_queries.cpp file contains the logic for Question-1 with Approach-2 mentioned in Report, i.e. distribution of queries.
