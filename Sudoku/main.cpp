//
//  main.cpp
//  Sudoku
//
//  Created by Juan Carlos Albahaca on 2017-12-23.
//  Copyright © 2017 Juan Carlos Albahaca. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <stack>


std::mt19937::result_type seed;
std::function<int()> int_rand;
void gen_array(int * dummy, int size);
void shuffle (int * arr, int size, int times);
void swap(int ** arr, int a, int b, int c, int d);
void print_sudoku(int ** sudoku);
bool check_repetition(int i, int j, int ** arr);
bool check_block(int i, int j, int ** arr);
bool check_row(int i, int j, int ** arr);
bool check_col(int i, int j, int ** arr);

struct SudoStruct {
    int *** sudo;
    int index[9][9];
    
    SudoStruct(){
        sudo = new int**[9];
        
        for(int i = 0; i<9; ++i) {
            sudo[i] = new int*[9];
            for(int j = 0; j<9; ++j) {
                sudo[i][j] = new int[9];
            }
        }
        int * arr = new int[9];
        for(int i = 0; i<9; ++i) {
            for(int j = 0; j<9; ++j) {
                index[i][j] = 0;
                gen_array(arr, 9);
                for(int k = 0; k<9; ++k)
                    sudo[i][j][k] = arr[k];
            }
        }
        delete [] arr;
    }
    ~SudoStruct() {
        for(int i = 0; i<9; ++i)
            for(int j = 0; j<9; ++j)
                delete [] sudo[i][j];
        delete [] sudo;
    }
};

struct RowColStruct {
    int row;
    int col;
};

int main(int argc, const char * argv[]) {
    int **sudoku = new int*[9];
    for(int i = 0; i<9; ++i) {
        sudoku[i] = new int[9];
        //memset(sudoku[i], 10, 9*sizeof(int));
    }
    
    
//    //memset(sudoku, 10, 81*sizeof(int));
    for(int i = 0; i<9; ++i) {
        for(int j = 0; j<9; ++j) {
            sudoku[i][j] = 10;
        }
    }
    
///////////////////////////////
    SudoStruct sudo;
    std::stack<RowColStruct> pos, copy;
    bool add = true;
    int i = 0;
    int j;
    while(i<9){
        if(add)
            j=0;
        add = true;
        while(j<9) {
            if(sudo.index[i][j] < 9) {
                sudoku[i][j] = sudo.sudo[i][j][sudo.index[i][j]];
                if(add)
                    pos.push({i,j});
                ++sudo.index[i][j];
//                print_sudoku(sudoku);
            } else if(sudo.index[i][j] >= 9){
                sudo.index[i][j] = 0;
                sudoku[i][j] = 10;
                pos.pop();
                pos.pop();
                i = pos.top().row;
                j = pos.top().col;
                continue;
            }
            while(check_repetition(i, j, sudoku)) {
                if(sudo.index[i][j] >= 9) {
                    sudo.index[i][j] = 0;
                    sudoku[i][j] = 100;
                    pos.pop();
                    i = pos.top().row;
                    j = pos.top().col;
//                    copy = pos;
//                    while(!copy.empty())
//                    {
//                        RowColStruct w = copy.top();
//                        std::cout << "{" << w.row << "," << w.col << "}" << std::endl;
//                        copy.pop();
//                    }
                    add = false;
                    break;
                } else if (sudo.index[i][j] < 9) {
                    sudoku[i][j] = sudo.sudo[i][j][sudo.index[i][j]];
                    ++sudo.index[i][j];
//                    print_sudoku(sudoku);
                    add = true;
                }
            }
            if(add)
                ++j;
        }
        if(add)
            ++i;
    }

    print_sudoku(sudoku);
    
    for(int i = 0; i<9; ++i){
        delete [] sudoku[i];
    }
    delete [] sudoku;
    
    return 0;
}

void print_sudoku(int ** sudoku) {
    std::cout << "   ---   ---   ---      ---   ---   ---      ---   ---   ---   " << std::endl <<" | ";
    for(int i = 0; i<9; ++i) {
        if (i > 0 && i % 3 == 0) {
            std::cout << std::endl << "   ---   ---   ---      ---   ---   ---      ---   ---   ---   " << std::endl;
        }
        for(int j = 0; j<9; ++j) {
            if (j == 8) {
                std::cout << " " << sudoku [i][j] << " " << " | " << std::endl << "   ---   ---   ---      ---   ---   ---      ---   ---   ---   " << std::endl;
            }
            if (j != 8 && (j+1) % 3 == 0) {
                std::cout << " " << sudoku [i][j] << " " << " |  | " ;
            }
            if (j == 0 && i != 0) {
                std::cout << " | ";
            }
            if (j != 8 && (j+1) % 3 != 0){
                std::cout << " " << sudoku [i][j] << " " << " | " ;
            }
        }
    }
}

void gen_array(int * dummy, int size){
    for(int i = 0; i<size; ++i)
        dummy[i] = i+1;
    shuffle(dummy, size, 100);
}

void shuffle (int * arr, int size, int times) {
    //Random integer generator
    seed = static_cast<std::mt19937::result_type>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    int_rand = std::bind(std::uniform_int_distribution<int>(0,size-1),
                              std::mt19937(seed));
    for(int i=0; i<times; ++i){
        int a, b, c;
        a = int_rand();
        b = int_rand();
        c = arr[a];
        arr[a] = arr[b];
        arr[b] = c;
    }
}

void swap(int ** arr, int a, int b, int c, int d) {
    int e = arr[a][b];
    arr[a][b] = arr[c][d];
    arr[c][d] = e;
}

bool check_repetition(int i, int j, int ** arr) {
//    bool a = check_block(i, j, arr);
//    bool b = check_row(i, j, arr);
//    bool c = check_col(i, j, arr);
//    std::cout << "\ncheck" << a << b << c << std::endl;
    if(check_block(i, j, arr) || check_row(i, j, arr) || check_col(i, j, arr))
        return true;
    return false;
};

bool check_block(int i, int j, int ** arr) {
    if((i - 1)% 3 == 0 && (j - 1) % 3 == 0){
        for(int c = -1; c<=1; ++c)
            for( int d = -1; d<=1; ++d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if(i % 3 == 0 && j % 3 == 0) {
        for(int c = 0; c<=2; ++c)
            for( int d = 0; d<=2; ++d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if((i - 2)% 3 == 0 && (j - 2) % 3 == 0) {
        for(int c = 0; c>=-2; --c)
            for( int d = 0; d>=-2; --d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if(i % 3 == 0 && (j - 2) % 3 == 0) {
        for(int c = 0; c<=2; ++c)
            for( int d = 0; d>=-2; --d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if((i - 2)% 3 == 0 && j % 3 == 0) {
        for(int c = 0; c>=-2; --c)
            for( int d = 0; d<=2; ++d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if((i - 1)% 3 == 0 && j % 3 == 0) {
        for(int c = -1; c<=1; ++c)
            for( int d = 0; d<=2; ++d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if(i % 3 == 0 && (j - 1) % 3 == 0) {
        for(int c = 0; c<=2; ++c)
            for( int d = -1; d<=1; ++d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if((i - 1)% 3 == 0 && (j - 2) % 3 == 0) {
        for(int c = -1; c<=1; ++c)
            for( int d = 0; d>=-2; --d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    } else if((i - 2)% 3 == 0 && (j - 1) % 3 == 0) {
        for(int c = 0; c>=-2; --c)
            for( int d = -1; d<=1; ++d) {
                if (c !=0 && d!= 0 && arr[i+c][j+d] == arr[i][j])
                    return true;
            }
    }
    return false;
};

bool check_row(int i, int j, int ** arr) {
    for(int l = j; l>=0; --l) {
        if(l != j && arr[i][j] == arr[i][l])
            return true;
    }
    for(int r = j; r<9; ++r) {
        if(r != j &&arr[i][j] == arr[i][r])
            return true;
    }
    return false;
};

bool check_col(int i, int j, int ** arr) {
    for(int u = i; u>=0; --u) {
        if(u != i && arr[i][j] == arr[u][j])
            return true;
    }
    for(int d = i; d<9; ++d) {
        if(d != i && arr[i][j] == arr[d][j])
            return true;
    }
    return false;
};


